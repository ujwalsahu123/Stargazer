# get lsb readings and store in file.
# no scaling here . no invert magz here 
#  we will use another convert.py to convert lsb.txt to microteslt.txt and invert magz

import csv
import time
import serial
import numpy as np

# GLOBAL VARIABLES
SER_PORT = 'COM5'  # Serial port
SER_BAUD = 115200  # Serial baud rate
SAMPLE_FREQ = 10  # Frequency to record magnetometer readings at [Hz]
T_SAMPLE = 30  # Total time to read magnetometer readings [sec]
OUTPUT_FILENAME = 'mag/magRawData-lsb.txt'  # Output data file name

class SerialPort:
    """Create and read data from a serial port."""
    def __init__(self, port, baud=9600):
        self.port = port
        self.baud = baud
        self.ser = serial.Serial(self.port, self.baud, timeout=1)
        self.ser.flushInput()
        self.ser.flushOutput()

    def Read(self):
        bytesToRead = self.ser.readline()
        decodedMsg = bytesToRead.decode('utf-8')
        return decodedMsg.strip()

    def Close(self):
        self.ser.close()

# Initialize serial communication
Arduino = SerialPort(SER_PORT, SER_BAUD)
N = 1000  # Number of readings
measurements = np.zeros((N, 3), dtype='int')

# Flush out bad readings
for _ in range(4):
    data = Arduino.Read().split(',')
    time.sleep(0.25)

# Collect magnetometer data
for currMeas in range(N):
    data = Arduino.Read().split(',')  # Split into separate values
    mx, my, mz = int(data[0]), int(data[1]), int(data[2])  # Raw magnetometer data
    print('%d %d %d' % (mx, my, mz))
    
    # Store data to array
    measurements[currMeas, 0] = mx
    measurements[currMeas, 1] = my
    measurements[currMeas, 2] = mz

# Write data to file after measurements
Arduino.Close()
print('Sensor Reading Complete!')
print('Writing data to {} ...'.format(OUTPUT_FILENAME))

# Save raw data to file
for i in range(N):
    with open(OUTPUT_FILENAME, 'a', newline='') as f:
        writer = csv.writer(f, delimiter='\t')
        writer.writerow([measurements[i, 0], measurements[i, 1], measurements[i, 2]])
