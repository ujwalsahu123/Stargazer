# this code get the lsb data and then stores it into txt file .
# it also scales from lsb to g . since scaling to g is imp . 
# since magneto only takes g unit data and then give offset . 
# and we need offset to get calib , so scale lsb to g .

import os
import math
import pandas as pd
import serial

# Configuration
MAX_MEAS = 2000  # Maximum number of measurements
AVG_MEAS = 25   # Number of samples to average for one reading
SER_PORT = 'COM6'  # Update to your Arduino's COM port (e.g., 'COM4' on Windows, '/dev/ttyUSB0' on Linux)
SER_BAUD = 115200  # Baud rate for serial communication
FILENAME = os.path.join(os.getcwd(), 'acc/acceldata_ism.txt')  # File to save accelerometer data

# SerialPort Class to manage Arduino communication
class SerialPort:
    def __init__(self, port, baud=9600):
        if not isinstance(port, str):
            raise TypeError('Port must be a string.')
        if not isinstance(baud, int):
            raise TypeError('Baud rate must be an integer.')

        self.port = port
        self.baud = baud
        self.ser = serial.Serial(self.port, self.baud, timeout=None)
        self.ser.flushInput()
        self.ser.flushOutput()

    def read(self, clean_end=True) -> str:
        """Read data from the serial port."""
        self.ser.flushInput()
        try:
            bytes_to_read = self.ser.readline()
            decoded_msg = bytes_to_read.decode('utf-8')
            if clean_end:
                decoded_msg = decoded_msg.rstrip()  # Remove newline characters
            return decoded_msg
        except Exception as e:
            print(f"[ERROR]: Serial read error: {e}")
            return ""

    def close(self):
        """Close the serial port."""
        self.ser.close()

# Function to record a single data point (averaged over AVG_MEAS readings)
def record_data_point(ser: SerialPort) -> tuple:
    ax, ay, az = 0.0, 0.0, 0.0
    for _ in range(AVG_MEAS):
        try:
            data = ser.read().split(',')
            ax_now = (int(data[0])) * 0.00006103515625
            ay_now = (int(data[1])) * 0.00006103515625
            az_now = (int(data[2])) * 0.00006103515625
        except (ValueError, IndexError) as e:
            print(f"[ERROR]: Data parsing error: {e}")
            ser.close()
            raise SystemExit("[ERROR]: Exiting due to invalid serial data.")
        ax += ax_now
        ay += ay_now
        az += az_now

    return ax / AVG_MEAS, ay / AVG_MEAS, az / AVG_MEAS

# Function to save the data to a file
def save_data_to_file(data: list, filename: str, delimiter: str = '\t'):
    # Check if file exists and is non-empty
    file_exists = os.path.exists(filename) and os.path.getsize(filename) > 0
    
    # Convert the data to a DataFrame
    df = pd.DataFrame(data)
    
    # Append data to file
    with open(filename, 'a') as file:
        df.to_csv(file, sep=delimiter, index=False, header=not file_exists)  # Write header only if file is empty

    print(f"[INFO]: Data appended to {filename}")

# Main function
def main():
    ser = SerialPort(SER_PORT, baud=SER_BAUD)
    data = []  # To store accelerometer readings

    print('[INFO]: Place the sensor level and stationary on a flat surface.')
    input('[INPUT]: Press Enter to begin calibration...')

    for _ in range(MAX_MEAS):
        user_input = input("[INPUT]: Ready for measurement? Type 'm' to measure or 'q' to quit: ").lower()
        if user_input == 'm':
            ax, ay, az = record_data_point(ser)

            # Avg values will be floats, preserving high precision in the file
            print(f'[INFO]: Avg Readings -> ax: {ax}, ay: {ay}, az: {az}')
            data.append([ax, ay, az])  # Store the readings
        elif user_input == 'q':
            print('[INFO]: Saving data and exiting...')
            save_data_to_file(data, FILENAME)
            ser.close()
            print('[INFO]: Done!')
            return
        else:
            print(f"[ERROR]: Unknown input '{user_input}'. Saving data and exiting!")
            save_data_to_file(data, FILENAME)
            ser.close()
            return

    print("[WARNING]: Maximum number of measurements reached. Saving data...")
    save_data_to_file(data, FILENAME)
    ser.close()
    print('[INFO]: Done!')

# Run the program
if __name__ == '__main__':
    main()
