# at stable positioning - the magnitude value must me 1 . or closest to one . 
# and using this code you can see the avg magnitude of the data , and by that you can 
# know if you offset values are good or bad .
import numpy as np

# Calibration matrix (A^-1) and bias vector (b)
A = np.array([[0.996986, 0.000024, -0.000549], 
              [0.000024, 1.001778, 0.000595],
              [-0.000549, 0.000595, 1.003680]])

b = np.array([0.005380, -0.014230, 0.009798])

# File path
file2 = 'acc/acceldata_ism.txt'

# Function to apply calibration and calculate magnitude
def calculate_avg_magnitude(file_path):
    magnitudes = []
    with open(file_path, 'r') as f:
        for line in f.readlines():
            raw_reading = np.array(list(map(float, line.split())))
            calibrated_reading = A @ (raw_reading - b)  # Apply calibration
            magnitude = np.linalg.norm(calibrated_reading)  # Calculate magnitude
            magnitudes.append(magnitude)
    return np.mean(magnitudes)

# Main script
def main():
    avg_magnitude = calculate_avg_magnitude(file2)
    print(f"[INFO]: Average magnitude of calibrated readings: {avg_magnitude}")

if __name__ == "__main__":
    main()
