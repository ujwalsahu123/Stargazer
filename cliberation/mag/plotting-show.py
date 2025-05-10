#just plot the raw_data or scaled_raw_data from .txt file . any data lsb or microT ,it just plots .

import numpy as np
import matplotlib.pyplot as plt

# Read raw data from the text file (mag-readings.txt)
rawData = np.genfromtxt('mag/motor-calib/readings.txt', delimiter='\t')  # Assuming tab-delimited data

# Check if the data was loaded properly
if rawData.size == 0:
    print("Error: No data found in mag-readings.txt")
else:
    print("Data loaded successfully!")

# Create a figure for 3D plotting
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the raw data (X, Y, Z magnetometer readings)
ax.scatter(rawData[:, 0], rawData[:, 1], rawData[:, 2], color='r', label='Raw Data')

# Set axis labels
ax.set_xlabel('X [uT]')
ax.set_ylabel('Y [uT]')
ax.set_zlabel('Z [uT]')
ax.set_title('3D Magnetometer Data')

# Show the legend
ax.legend()

# Display the plot
plt.show()
