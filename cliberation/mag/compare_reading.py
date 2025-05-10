# compare the reading of 2 files  . (basically use to compare last setup reading , and now setup redings
# sometimes they will be different , ex- you have headphone this time then different)
# not usefull code , but just to compare readings of 2 time .

import numpy as np
import matplotlib.pyplot as plt

# Load data from the first file
file1 = 'mag-readings.txt'
data1 = np.genfromtxt(file1, delimiter='\t')

# Load data from the second file
file2 = 'xtra-reading.txt'
data2 = np.genfromtxt(file2, delimiter='\t')

# Check if data is loaded properly
if data1.size == 0 or data2.size == 0:
    print("Error: One or both files are empty or not found!")
else:
    print("Data loaded successfully!")

# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot data from file1 in red
ax.scatter(data1[:, 0], data1[:, 1], data1[:, 2], color='r', label='File 1 Data')

# Plot data from file2 in blue
ax.scatter(data2[:, 0], data2[:, 1], data2[:, 2], color='b', label='File 2 Data')

# Set axis labels
ax.set_xlabel('X [uT]')
ax.set_ylabel('Y [uT]')
ax.set_zlabel('Z [uT]')
ax.set_title('3D Magnetometer Data: File 1 vs File 2')

# Show the legend
ax.legend()

# Display the plot
plt.show()
