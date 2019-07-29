import numpy as np
import matplotlib.pyplot as plt
import configparser

#config = configparser.ConfigParser()
#config.read("D:\\NPCSag\\config.ini")
#step_size = config['Parameters']['step_size']
#calibration_factor = config['Parameters']['calibration_factor']

filename = "wrapData/sawTooth.csv"

xVal = []
yVal = []
with open(filename) as f:
    for line in f:
        #print(line)
        #a, b, c = line.split(',')
        b, c = line.split(',')
        #xVal.append(int(a) * int(step_size))
        xVal.append(int(b))
        #yVal.append(float(c) * float(calibration_factor) )
        yVal.append(float(c))
                
plt.figure(num=None, figsize=(20, 19), dpi=100, facecolor='w', edgecolor='k')
plt.plot(xVal, yVal, marker='x')
#plt.axis([0, 5000, 50, 20])
#plt.annotate('max sag', xy=(xVal[9], yVal[9]), xytext = (xVal[9], yVal[9] + 15), arrowprops=dict(facecolor='black', shrink=0.05),)
plt.xlabel('Pixels')
plt.ylabel('Phi (0 to 2 * PI)')

plt.title("Fizeau Profile.")

plt.legend("Fizeau Profile")

plt.show()

#print(xVal)
#print(yVal)
