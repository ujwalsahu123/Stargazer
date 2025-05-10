# Convert mag-data-lsb.txt to scaled-to-microtesla.txt 
# also it inverted Z-axis for alignment with other sensors.

import numpy as np

# Function to convert LSB to microtesla and invert Z-axis
def lsb_to_microtesla_with_inversion(lsb_values):
    microtesla_values = []
    
    # The scaling for LSB to microtesla is (1 Gauss = 100 microtesla)
    for values in lsb_values:
        scaled_x = (values[0] - 131072.0) / 131072.0
        scaled_y = (values[1] - 131072.0) / 131072.0
        scaled_z = (values[2] - 131072.0) / 131072.0
        
        # Convert from Gauss to microtesla (1 Gauss = 100 microtesla)
        scaled_x *= 8 * 100
        scaled_y *= 8 * 100
        scaled_z *= 8 * 100
        
        # Invert the Z-axis
        inverted_z = -scaled_z
        
        # Append scaled and inverted data
        microtesla_values.append([scaled_x, scaled_y, inverted_z])
    
    return microtesla_values

# Input and output file paths
input_file = 'mag/magRawData-lsb.txt'
output_file = 'mag/scaled-to-microtesla.txt'

# Read the raw LSB data from the input file 
with open(input_file, 'r') as f:
    lsb_data = [list(map(int, line.split())) for line in f.readlines()]

# Convert LSB values to microtesla and apply Z-axis inversion
microtesla_data = lsb_to_microtesla_with_inversion(lsb_data)

# Write the microtesla data with inverted Z-axis to the output file
with open(output_file, 'w') as f:
    for data in microtesla_data:
        f.write(f"{data[0]} {data[1]} {data[2]}\n")

print(f"Conversion completed with Z-axis inversion and saved to {output_file}")
