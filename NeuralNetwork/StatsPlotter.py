
import matplotlib.pyplot as plt
import numpy as np


x = [0]
y = []

with open("D:/Code/TP/C/OCR/NeuralNetwork/stats.txt", "r") as f:
    for line in f:
        try:
            x.append(x[-1]+1)
            y.append(float(line))
        except Exception:
            x = x[:-1]
    x = x[1:]

xp = np.array(x)
yp = np.array(y)

plt.plot(xp, yp, color='b', linewidth=1, antialiased=True)
#plt.axline((0, 0.25), (x[-1], 0.25), color='r', linewidth=1, antialiased=True)
plt.xlabel("Iteration")
plt.ylabel("Error")
plt.title("Min : " + str(min(y)))
plt.show()





