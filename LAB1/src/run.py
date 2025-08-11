import subprocess
import numpy as np
from pathlib import Path

cwd = Path.cwd()
avg_time = []
result = subprocess.run("make",shell = True)
for i in range(1,8):
    sum = np.array([0.0,0.0,0.0,0.0,0.0])
    for time in range(5):
        result = subprocess.run(f"make run-sharpen INPUT={i} OUTPUT={i}_app_output",capture_output=True,text=True)
        output = result.stdout
        lines = output.strip().split("\n")
        #print(output)
        lines = lines[3:]
        values = []
        for line in lines:
            values.append(int(line.split(":")[1].split()[0]))
        values = np.array(values)
        #print(values)
        #print("\n")
        sum += values
    #print(sum)
    sum = sum/5
    #sum.tolist()
    sum = np.array2string(sum, separator=" ")
    avg_time.append(sum)
    #print(avg_time)
    #print("\n")
with open("Time.txt","w") as f:
    for i in range(1,8):
        f.write(f"For image {i}.ppm\n")
        clean_line = avg_time[i-1].strip().strip("[]")
        row = list(map(float, clean_line.split()))
        f.write(f"execution time for reading ppm file: {row[0]} ms\n")
        f.write(f"execution time for smoothening: {row[1]} ms\n")
        f.write(f"execution time for finding details: {row[2]} ms\n")
        f.write(f"execution time for sharpening: {row[3]} ms\n")
        f.write(f"execution time for writing ppm file: {row[4]} ms\n")
        f.write("\n")
print("Time.txt file created successfully!\n")
