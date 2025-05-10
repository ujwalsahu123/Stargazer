# plotting raw data and caliberated data . 

import csv
import numpy as np
import matplotlib.pyplot as plt

# Define calibration parameters (replace these with the ones you get from Magneto calibration)

##################################  update the offset values ..

A = np.array([[1.168384, 0.004008, -0.001287], 
              [ 0.004008, 1.229647, -0.087849],
              [-0.001287, -0.087849, 1.265104]])
b = np.array([-2.508501, 32.722110, -8.102990])

# Read raw data from file (assuming data is saved as tab-separated)
rawData = np.genfromtxt('mag/scaled-to-microtesla.txt', delimiter=' ')  # Read raw measurements

# Calibration: Apply transformation matrix to the raw data
N = len(rawData)
calibData = np.zeros((N, 3), dtype='float')
for i in range(N):
    currMeas = np.array([rawData[i, 0], rawData[i, 1], rawData[i, 2]])
    calibData[i, :] = A @ (currMeas - b)  # Apply calibration

# Calculate the center (mean) value for raw and calibrated data
raw_center = np.mean(rawData, axis=0)
calib_center = np.mean(calibData, axis=0)

# Output the center values in terminal
print(f"Raw Data Center: X={raw_center[0]}, Y={raw_center[1]}, Z={raw_center[2]}")
print(f"Calibrated Data Center: X={calib_center[0]}, Y={calib_center[1]}, Z={calib_center[2]}")

# Calculate the distance from the origin (magnitude of the center)
raw_center_magnitude = np.linalg.norm(raw_center)
calib_center_magnitude = np.linalg.norm(calib_center)

# Output the center magnitudes in terminal
print(f"Raw Data Center Magnitude: {raw_center_magnitude:.6f}")
print(f"Calibrated Data Center Magnitude: {calib_center_magnitude:.6f}")

# # Plot XY data
plt.figure()
plt.plot(rawData[:, 0], rawData[:, 1], 'b*', label='Raw Meas.')
plt.plot(calibData[:, 0], calibData[:, 1], 'r*', label='Calibrated Meas.')
plt.title('XY Magnetometer Data')
plt.xlabel('X [uT]')
plt.ylabel('Y [uT]')
plt.legend()
plt.grid()
plt.axis('equal')

# # Plot YZ data
plt.figure()
plt.plot(rawData[:, 1], rawData[:, 2], 'b*', label='Raw Meas.')
plt.plot(calibData[:, 1], calibData[:, 2], 'r*', label='Calibrated Meas.')
plt.title('YZ Magnetometer Data')
plt.xlabel('Y [uT]')
plt.ylabel('Z [uT]')
plt.legend()
plt.grid()
plt.axis('equal')

# # Plot XZ data
plt.figure()
plt.plot(rawData[:, 0], rawData[:, 2], 'b*', label='Raw Meas.')
plt.plot(calibData[:, 0], calibData[:, 2], 'r*', label='Calibrated Meas.')
plt.title('XZ Magnetometer Data')
plt.xlabel('X [uT]')
plt.ylabel('Z [uT]')
plt.legend()
plt.grid()
plt.axis('equal')

#3d plot

# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')

# for i in range(N):
#     xraw = rawData[i, 0]
#     yraw = rawData[i, 1]
#     zraw = rawData[i, 2]

#     xcalib = calibData[i, 0]
#     ycalib = calibData[i, 1]
#     zcalib = calibData[i, 2]
    
#     ax.scatter(xraw, yraw, zraw, color='r')  # Raw data in red
#     ax.scatter(xcalib, ycalib, zcalib, color='b')  # Calibrated data in blue

# ax.set_title('3D Scatter Plot of Magnetometer Data')
# ax.set_xlabel('X [uT]')
# ax.set_ylabel('Y [uT]')
# ax.set_zlabel('Z [uT]') 


plt.show()
