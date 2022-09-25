#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>                  // I2C LCD
#include <Wire.h>                 // I2C
#include <ESP8266WiFi.h>  // Встроена в библиотеку ESP8266
#include <PubSubClient.h>


#define ONE_WIRE_BUS 0 // Пин подключения OneWire шины, 0 (D3)
#define diodPin 14 // пин выхода D5 для диода
#define connectionPin 12 // D6 pin ардуино

// create LCD object
LiquidCrystal_I2C LCD (0x27 , 20, 4);

OneWire oneWire(ONE_WIRE_BUS); // Подключаем бибилотеку OneWire
DallasTemperature sensors(&oneWire); // Подключаем бибилотеку DallasTemperature
DeviceAddress adreses {0x28, 0xFF, 0x94, 0x32, 0x60, 0x17, 0x03, 0x6A};
float temperature;
float excessTimer = 0.0, excessStart = 0.0;
int otsechka = 25;                // Базавая уставка по временно допустимому перегреву
int maxTemp = 30;                 // Предельная температура
float timeSetting = 60*120*1000;  // Уставка по времени в миллисекундах
char* tempState = "Normal";

const char* ssid = "Rush"; 
const char* password =  "09012000"; 
const char* mqttServer = "193.233.67.51";
const int mqttPort = 1883;
const char* graph = "/graph";
const char* state = "/state";
const char* setting = "/setting";

WiFiClient espClient;
PubSubClient client(espClient);


class Led{
  private:
    int pin;
    bool state;
  public:
    Led(int initPin, bool initState = false){
      pinMode(initPin, OUTPUT);
      digitalWrite(initPin, initState);
      pin = initPin;
      state = initState;
    }

    void on(){
      if (state){
        return;
      }
      state = true;
      digitalWrite(pin, state);
    }

    void off(){
      if (!state){
        return;
      }
      state = false;
      digitalWrite(pin, state);
    }
};


Led led(diodPin);


void setup(void)
{  
  pinMode(connectionPin, OUTPUT);

  Serial.begin(115200);     // Задаем скорость соединения с последовательным портом 
  sensors.begin();          // Иницилизируем датчики
                            // Инициализируем дисплей
  LCD.init();
  LCD.backlight();

  WiFi.begin(ssid, password);      // Подключаемся к wi-fi
  Serial.println('Connecting: ');
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    String client_id = "ESP8266Client-";
    client_id += String(WiFi.macAddress());
    if (client.connect(client_id.c_str())) {
 
      Serial.println("connected");  
    } 
    else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      Serial.println();
      delay(2000);
    }
  }

  client.subscribe(setting);
}


void callback(char* topic, byte* payload, unsigned int length) {
 
  char settingValue[length];

  for (int i = 0; i < length; i++) {
    settingValue[i] = (char)payload[i];
  }
  otsechka = atof(settingValue); // Поменяли уставку 
}


void blink_display(LiquidCrystal_I2C lcd, int startTime)
{
  lcd.noBacklight();
  delay(500);
  lcd.backlight();
  delay(500);
  lcd.noBacklight();
  delay(500);
  lcd.backlight();
}


void loop(void)
{
  client.loop();
  if (tempState != "Shutdown"){
    sensors.requestTemperatures();
    temperature = sensors.getTempC(adreses);
    LCD.setCursor(5, 1);
    LCD.print("t = ");
    LCD.print(temperature);

    Serial.print(temperature); // Выводим температуру с датчика
    Serial.print(' ');

    char t[5];
    dtostrf(temperature, 5, 2, t);  // Создали переменную для отправки данных на график

    if (temperature > otsechka && temperature < maxTemp)
    {
      tempState = "Critical";

      led.on();

      if (excessStart == 0.0)   // Запускаем таймер 
      {
        excessStart = millis();
        blink_display(LCD, excessStart);
        digitalWrite(connectionPin, HIGH);
      }
      else if(excessTimer >= timeSetting)
      {
        tempState = "Overheating";
      }

      excessTimer = millis() - excessStart;

      client.publish(graph, t);
      client.publish(state, tempState);
      LCD.setCursor(5, 1);
      LCD.print("t = ");
      LCD.print(temperature);
      delay(100);
      LCD.clear();
    }
    else if (temperature >= maxTemp){
      tempState = "Shutdown";
    }
    else
    {
      tempState = "Normal";
      led.off();

      digitalWrite(connectionPin, LOW);

      excessStart = 0.0;       // Сброс таймера
      excessTimer = 0.0;
      client.publish(graph, t);
      client.publish(state, tempState);
      delay(100);
    }

    Serial.print(tempState);       // Выводим состояние
    Serial.print(' ');
    Serial.println(otsechka);  // Выводим уставку


    LCD.setCursor(3, 0);      // Выводим данные на дисплей
    LCD.print(tempState);
    LCD.setCursor(0, 2);
    LCD.print("Cur. setting: ");
    LCD.print(otsechka);
    LCD.setCursor(0, 3);
    LCD.print("Duration: ");
    LCD.print(excessTimer / 1000);
  }
}