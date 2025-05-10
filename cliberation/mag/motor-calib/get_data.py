# so you can choose to caliberate it using blocking or non blocking . 
#both are fine , non blocking just takes values very rapidly , so more data .

# non blocking is better i think , since more data values we get . motor slowly rotates and 
# we also get more values which are totally good i saw . so go with non blocking .

# also on a free day you can test both out . (calib and get offset and compare offset)
#
# dont touch the code , just make the setup better . motor1 pitch (it auto does =clock, anticlock)
# and motor 2 easy yaw . 
# motor1 (pitch) is more smooth rotation so 360 or 380 pitch move , then 15/20/etc yaw move .and so onn .



import csv
import time
import serial
import numpy as np

# Configuration Constants
SER_PORT = 'COM5'             # Serial port
SER_BAUD = 115200             # Serial baud rate
OUTPUT_FILENAME = 'mag/motor-calib/readings.txt'  # Output file name

class SerialPort:
    """Handles serial communication with the Arduino."""
    def __init__(self, port, baud):
        self.port = port
        self.baud = baud
        try:
            self.ser = serial.Serial(self.port, self.baud, timeout=1)
            self.ser.flushInput()
            self.ser.flushOutput()
            print(f"Connected to {self.port} at {self.baud} baud.")
        except Exception as e:
            print(f"Error connecting to serial port: {e}")
            raise

    def read_line(self):
        """Reads a line of data from the serial port."""
        try:
            line = self.ser.readline().decode('utf-8').strip()
            return line
        except Exception as e:
            print(f"Error reading from serial: {e}")
            return None

    def close(self):
        """Closes the serial port."""
        self.ser.close()
        print("Serial port closed.")


def main():
    # Initialize an empty list to dynamically store data
    measurements = []

    # Start serial communication
    try:
        arduino = SerialPort(SER_PORT, SER_BAUD)
    except Exception:
        return

    print("Starting data collection. Press Ctrl+C to stop.")

    try:
        while True:
            # Read and parse data from the serial port
            raw_data = arduino.read_line()
            if raw_data:
                try:
                    mx, my, mz = map(int, raw_data.split(','))
                    print(f"Read: MX={mx}, MY={my}, MZ={mz}")

                    # Stop data collection if all values are 0
                    if mx == 0 and my == 0 and mz == 0:
                        print("Received 0,0,0. Stopping data collection.")
                        break

                    # Append the reading to the list
                    measurements.append([mx, my, mz])

                except ValueError:
                    print(f"Invalid data format: {raw_data}")
            else:
                time.sleep(0.1)  # Small delay if no data is received

    except KeyboardInterrupt:
        print("\nData collection interrupted by user.")

    finally:
        # Close the serial port
        arduino.close()

        if measurements:
            print(f"Collected {len(measurements)} valid readings.")

            print(f"Writing data to {OUTPUT_FILENAME} ...")
            with open(OUTPUT_FILENAME, 'w', newline='') as f:
                writer = csv.writer(f, delimiter='\t')
                writer.writerow(["MX", "MY", "MZ"])  # Header row
                writer.writerows(measurements)
            print(f"Data written")
        else:
            print("No valid data collected.")

if __name__ == "__main__":
    main()
