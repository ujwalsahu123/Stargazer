#ok , so the first thing is go and get offset data of mag , gyro , acc . (20,000+ ,1000+ readings)

# Now , we do (raw xyz math offset) to get calib xyz , and then we send that to sf .
 
# 
# so  

# we just get raw xyz data of all mag , acc , gryo from arduino code . mag , acc , gyro
# . and then in python we do calculation ( raw math offset ) to get calib xyz .
# also we do  raw * 0.000061... in python only , then do math offset . 
# good xyz make using offset math . and then in python only we use sensor fusion (somehow -there is 
# library availabe , also we can use they arduino library ?idk)
# and then 
# print that RPY , and also do tuning in python . 

# so only get raw xyz from arduino , and do math with offset in py to get calib xyz
# , and then give to sf . 

# tell the entire plan to gpt the start .
#1 catch arduino raw data - using serial , etc etc stuff 
#2 do scale of that raw values of mag , acc , gyro 
#3 do math of scaled raw and offset -> to get calib xyz
#4 send calib xyz of all 3 sensor to sensor fusion
#5 get the sensor fusion rpy and print , also do tuning (fiter , etc)



#####################update the unit as per the updateMARG wants . and as per that unit only get offset from magneto.



import time
import numpy as np
from ahrs.filters import Madgwick
import serial
from scipy.spatial.transform import Rotation as R

# Calibration parameters for Magnetometer and Accelerometer
A_mag = np.array([
    [1.168384, 0.004008, -0.001287],
    [0.004008, 1.229647, -0.087849],
    [-0.001287, -0.087849, 1.265104]
])
b_mag = np.array([-2.508501, 32.722110, -8.102990])

A_acc = np.array([
    [0.996986, 0.000024, -0.000549],
    [0.000024, 1.001778, 0.000595],
    [-0.000549, 0.000595, 1.003680]
])
b_acc = np.array([0.005380, -0.014230, 0.009798])

# Scaling factors for sensor values
GYRO_SCALING_FACTOR = 0.0000764  # rad/s per LSB  /////////////  ????????????????
ACCEL_SCALING_FACTOR = 0.00006103515625  # m/s^2 per LSB
MAG_SCALING_FACTOR = 8.0 / 131072.0 * 100.0  # Specific to magnetometer

# Initialize Madgwick filter
madgwick = Madgwick(frequency=20)  # Sample period = 1/20 seconds

# Serial port setup for communication with Arduino
serial_port = "COM5"  # Change this to your serial port
baud_rate = 115200
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Low-pass filter parameters
alpha = 0.1  # Low-pass filter coefficient (between 0 and 1)

# Helper functions
def scale_and_calibrate(raw_data, A, b):
    """Scale and calibrate the raw sensor data."""
    raw_data = np.array(raw_data)
    calibrated_data = A @ (raw_data - b)
    return calibrated_data

def low_pass_filter(data, prev_data):
    """Apply a low-pass filter to smooth sensor data."""
    return alpha * data + (1 - alpha) * prev_data

def quaternion_to_rpy(q):
    """Convert quaternion to Roll, Pitch, Yaw (RPY)."""
    w, x, y, z = q
    # Avoid numerical issues with small values
    sin_roll = 2 * (w * x + y * z)
    cos_roll = 1 - 2 * (x ** 2 + y ** 2)
    roll = np.arctan2(sin_roll, cos_roll)

    sin_pitch = 2 * (w * y - z * x)
    pitch = np.arcsin(np.clip(sin_pitch, -1.0, 1.0))  # Clipping to avoid NaN values

    sin_yaw = 2 * (w * z + x * y)
    cos_yaw = 1 - 2 * (y ** 2 + z ** 2)
    yaw = np.arctan2(sin_yaw, cos_yaw)

    return np.degrees(roll), np.degrees(pitch), np.degrees(yaw)

def slerp(q1, q2, t):
    """
    Perform Spherical Linear Interpolation (SLERP) between two quaternions.
    """
    q1 = q1 / np.linalg.norm(q1)
    q2 = q2 / np.linalg.norm(q2)

    dot_product = np.dot(q1, q2)

    # Ensure the shortest path by flipping one quaternion if necessary
    if dot_product < 0.0:
        q2 = -q2
        dot_product = -dot_product

    # Clamp the dot product to avoid numerical issues
    dot_product = np.clip(dot_product, -1.0, 1.0)

    # Compute the angle between the quaternions
    theta = np.arccos(dot_product)

    # If the angle is small, linearly interpolate
    if np.abs(theta) < 1e-6:
        return (1 - t) * q1 + t * q2

    # Perform SLERP
    sin_theta = np.sin(theta)
    q_interp = (np.sin((1 - t) * theta) / sin_theta) * q1 + (np.sin(t * theta) / sin_theta) * q2
    return q_interp

def read_serial_data():
    """Read sensor data from serial."""
    line = ser.readline().decode('utf-8').strip()
    if line:
        return list(map(float, line.split(',')))
    return None

def main():
    q = np.array([1.0, 0.0, 0.0, 0.0])  # Initial quaternion
    prev_q = q  # Previous quaternion for SLERP
    prev_gyro = np.array([0.0, 0.0, 0.0])  # Previous gyro data for smoothing

    # Wait 3 seconds for sensor stabilization
    print("Waiting for sensor stabilization...")
    time.sleep(3)

    print("Starting sensor data processing...")
    sample_time = 1.0 / 20.0  # Sample period (20 Hz)

    try:
        while True:
            start_time = time.time()

            # Read and process sensor data from the serial
            data = read_serial_data()
            if not data or len(data) != 9:
                continue  # Skip incomplete or erroneous data

            magX, magY, magZ, accX, accY, accZ, gyroX, gyroY, gyroZ = data

            # Calibrate magnetometer data
            magX, magY, magZ = (magX - 131072.0) * MAG_SCALING_FACTOR, (magY - 131072.0) * MAG_SCALING_FACTOR, (magZ - 131072.0) * MAG_SCALING_FACTOR
            magZ = -magZ  # Invert Z-axis for correct orientation

            # Calibrate accelerometer data
            accX, accY, accZ = accX * ACCEL_SCALING_FACTOR, accY * ACCEL_SCALING_FACTOR, accZ * ACCEL_SCALING_FACTOR

            # Calibrate gyroscope data
            gyroX, gyroY, gyroZ = gyroX * GYRO_SCALING_FACTOR, gyroY * GYRO_SCALING_FACTOR, gyroZ * GYRO_SCALING_FACTOR

            # Apply low-pass filtering to accelerometer and gyroscope
            accX = low_pass_filter(accX, prev_gyro[0])
            accY = low_pass_filter(accY, prev_gyro[1])
            accZ = low_pass_filter(accZ, prev_gyro[2])
            gyroX, gyroY, gyroZ = low_pass_filter(gyroX, prev_gyro[0]), low_pass_filter(gyroY, prev_gyro[1]), low_pass_filter(gyroZ, prev_gyro[2])

            # Calibrate sensor data
            calib_acc = scale_and_calibrate([accX, accY, accZ], A_acc, b_acc)
            calib_mag = scale_and_calibrate([magX, magY, magZ], A_mag, b_mag)

            # Update quaternion using Madgwick filter
            q = madgwick.updateMARG(
                q, gyr=[gyroX, gyroY, gyroZ],  # Gyroscope data in rad/s
                acc=[calib_acc[0] * 9.80665, calib_acc[1] * 9.80665, calib_acc[2] * 9.80665],  # Accelerometer data in m/s²
                mag=[calib_mag[0] * 1000, calib_mag[1] * 1000, calib_mag[2] * 1000]  # Magnetometer data
            )

            # Smooth quaternion transitions with SLERP
            t = 0.1  # Interpolation factor (adjust for smoother transitions)
            q = slerp(prev_q, q, t)

            # Convert quaternion to RPY
            roll, pitch, yaw = quaternion_to_rpy(q)

            # Print the RPY values
            print(f"Roll: {roll:.2f}, Pitch: {pitch:.2f}, Yaw: {yaw:.2f}")

            # Update previous quaternion and gyro data for the next iteration
            prev_q = q
            prev_gyro = np.array([gyroX, gyroY, gyroZ])

            # Timing control for consistent loop frequency
            elapsed_time = time.time() - start_time
            if elapsed_time < sample_time:
                time.sleep(sample_time - elapsed_time)

    except KeyboardInterrupt:
        print("Exiting...")

    finally:
        ser.close()

if __name__ == "__main__":
    main()
