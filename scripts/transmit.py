#!/usr/bin/env python3
import serial, hashlib, sys, os, struct
from time import sleep

r = b''

def sendToUart(uart, data, blocksize=256):
    global r
    counter = 0
    for d in data:
        print("tr: " + str(bytes([d])))
        uart.write(bytes([d]))
        counter += 1
        sleep(0.01)
        if counter % 100 == 0:
            r = r + uart.read(100)
        if counter % blocksize == 0:
            sleep(0.5)
    r = r + uart.read(counter % 100)


if len(sys.argv) < 2:
    print("Missing arg");
    exit(1)
BUF_SIZE=65536

sha256 = hashlib.sha256()

size = os.path.getsize(sys.argv[1])

counter = 0

sdata = b''
with open (sys.argv[1], 'rb') as f:
    while True:
        data = f.read(BUF_SIZE)
        sdata += data
        if not data:
            break
        sha256.update(data)
        
print("Size: {0}".format(size))
print("SHA256: {0}".format(sha256.hexdigest()))
print("Data: " + str(sdata))
uart = serial.Serial('/dev/ttyUSB0', 9600)
print(uart)
print("Send Size")

sendToUart(uart, str(size).zfill(10).encode('utf-8'))
sleep(0.5)

sendToUart(uart, sha256.digest())
sleep(0.5)

sendToUart(uart, sdata)


print("re: " + str(r))
print ("Equal: " + str(sdata == r))
uart.close()

