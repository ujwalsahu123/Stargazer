# check avg caliberated magnitude . 
# like in acc the magnitude is 1 then nice caliberated xyz . 
# so like that , here magneto magnetic field value 43.7 , insted of 1  .   
# but idk if this concept is right or not

#this code checks the avg magnitude of the microtesla data . 
# xtra-(also if you change the offset and float and filename then can give avg magnitude of lsb also.)

import numpy as np

#put here new offset values . 

########################################   update the offset values . 

A = np.array([[1.168384, 0.004008, -0.001287], 
              [ 0.004008, 1.229647, -0.087849],
              [-0.001287, -0.087849, 1.265104]])
b = np.array([-2.508501, 32.722110, -8.102990])


# Function to calculate the magnitude of the vector
def calculate_magnitude(x, y, z):
    return np.sqrt(x**2 + y**2 + z**2)

# Function to read the magnetometer data from the file
def read_mag_data(filename):
    raw_data = []
    with open(filename, 'r') as file:
        for line in file:
            # Assuming each line contains the x, y, z values separated by spaces
            values = line.split()
            if len(values) == 3:
                raw_data.append([float(v) for v in values])  # Convert to integer since data are integers
    return np.array(raw_data)

# Read raw magnetometer data from file1.txt
raw_data = read_mag_data('mag/scaled-to-microtesla.txt')

# Calculate average magnitude of raw readings
raw_magnitudes = [calculate_magnitude(x, y, z) for x, y, z in raw_data]
avg_raw_magnitude = np.mean(raw_magnitudes)
print(f"Average magnitude of raw readings: {avg_raw_magnitude}")

# Apply calibration: Apply transformation matrix to the raw data
N = len(raw_data)
calib_data = np.zeros((N, 3), dtype='float')

for i in range(N):
    curr_meas = np.array([raw_data[i, 0], raw_data[i, 1], raw_data[i, 2]])
    calib_data[i, :] = A @ (curr_meas - b)  # Apply calibration

# Calculate average magnitude of calibrated readings
calib_magnitudes = [calculate_magnitude(x, y, z) for x, y, z in calib_data]
avg_calib_magnitude = np.mean(calib_magnitudes)
print(f"Average magnitude of calibrated readings: {avg_calib_magnitude}")
