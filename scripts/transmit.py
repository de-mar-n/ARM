#!/usr/bin/env python
import serial, hashlib, sys, os, struct
from time import sleep

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
sdata = struct.pack('>i', size) + sha256.digest() + sdata
print("Data: " + str(sdata))
uart = serial.Serial('/dev/ttyUSB0', 115200)
r = b''
print(uart)
for d in sdata:
    print("tr: " + str(bytes([d])))
    uart.write(bytes([d]))
    counter += 1
    sleep(0.005)
    if counter % 100 == 0:
        r = r + uart.read(100)

print("end")
r = r + uart.read(counter % 100)
print("re: " + str(r))
print ("Equal: " + str(sdata == r))
uart.close()

