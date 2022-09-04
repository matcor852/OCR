
import matplotlib.pyplot as plt
import numpy as np


x = []
y = []

with open("D:/Code/C/OCR/NeuralNetwork/stats.txt", "r") as f:
    for line in f:
        try:
            sp = line.split(' ')
            x.append(int(sp[0]))
            y.append(float(sp[1].rstrip()))
        except Exception:
            x = x[:-1]


xp = np.array(x)
yp = np.array(y)

plt.plot(xp, yp, color='b', linewidth=1, antialiased=True)
#plt.axline((0, 0.25), (x[-1], 0.25), color='r', linewidth=1, antialiased=True)
plt.show()





