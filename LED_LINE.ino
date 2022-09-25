#define NUM_LEDS 60
#include "FastLED.h"
#define PIN 14
#define connectionPin 19

CRGB leds[NUM_LEDS];
byte counter;

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(200);
  pinMode(13, OUTPUT);
  pinMode(connectionPin, INPUT);

}

void blink() {
  for (int i = 0; i < NUM_LEDS; i++ ) {         // от 0 до первой трети
    leds[i].setRGB(counter + i*5, 0, 0);
    // умножение i уменьшает шаг радуги
  }
  counter += 3;        // counter меняется от 0 до 255 (тип данных byte)
  FastLED.show();
  delay(5);         // скорость движения радуги
  FastLED.clear();
  FastLED.showColor((0, 0, 0));
}


void loop() {
  if (digitalRead(connectionPin)){
    for (int j=0; j < 50; j++){blink();}    
  }
}