# Importing Libraries
import serial
import time
arduino = serial.Serial(port='/dev/cu.usbmodem142101', baudrate=115200, timeout=.1)
def write_read(x):
    x = x + '\r'
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data
while True:
    num = "2"
    while 1:
        value = write_read(num)
        time.sleep(1)
        print(value) # printing the value