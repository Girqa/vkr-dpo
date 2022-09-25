from PySide6.QtSerialPort import QSerialPortInfo
from serial import Serial
from time import time

MICROBIT_PID = 60000
MICROBIT_VID = 4292

def open_port(port: QSerialPortInfo) -> None:
    serial = Serial(port.portName(), 115200)
    if serial.is_open:
        print(f'Serial port {port.portName()} is opened!')
    return serial

def read_serial_port(serial: Serial) -> float:
    serial.flush()
    msg = serial.readline()
    text = str(msg, 'utf-8')
    return text

def get_ports() -> list:
    ports = QSerialPortInfo().availablePorts()
    ports_list = [port for port in ports]
    return ports_list

def get_active_port(ports: list) -> QSerialPortInfo:
    for port in ports:
        pid = port.productIdentifier()
        vid = port.vendorIdentifier()
        if pid == MICROBIT_PID and vid == MICROBIT_VID:
            return port
    return None


def connect_serial():
    ports = get_ports()
    active_port = get_active_port(ports)
    serial = open_port(active_port)
    start = time()
    while time() - start < 5:
        read_serial_port(serial)
    return serial



