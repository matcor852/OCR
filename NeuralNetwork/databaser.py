import glob, os, random, math, pandas
from PIL import Image
import matplotlib.pyplot as plt
import numpy as np


size = (28,28)
oSize = size[0]*size[1]

print()

root, f1, f2 = "D:/Code/C/OCR/NeuralNetwork/DataSets/", "hcd_{0}.bin".format(oSize-1), "hcd_{0}_test".format(oSize-1)


def loadMNIST(appendMode = False):
    mode = "ab" if appendMode else "wb"
    training = open(root + f1, mode)
    validation = open(root + f2, mode)
    c1, c2 = 0, 0
    df = pandas.read_csv(root+"mnist_train.csv")
    for pict in df.itertuples():
        data = list(pict)
        print("\rsaving train picture {0}              ".format(str(data[0])), end='')
        vls = [2*x/255-1 for x in data[2:]]
        np.array(data[1]).tofile(training, sep='', format='%L')
        np.array(vls).tofile(training, sep='', format='%d')
        c1 += 1
    print()
    df = pandas.read_csv(root+"mnist_test.csv")
    for pict in df.itertuples():
        data = list(pict)
        print("\rsaving test picture {0}              ".format(str(data[0])), end='')
        vls = [2*x/255-1 for x in data[2:]]
        np.array(data[1]).tofile(validation, sep='', format='%L')
        np.array(vls).tofile(validation, sep='', format='%d')
        c2 += 1
    training.close()
    validation.close()
    if os.path.exists(root + "hcd_{0}_{1}_training.bin".format(oSize,c1)):
        os.remove(root + "hcd_{0}_{1}_training.bin".format(oSize,c1))
    if os.path.exists(root + "hcd_{0}_{1}_validation.bin".format(oSize,c2)):
        os.remove(root + "hcd_{0}_{1}_validation.bin".format(oSize,c2))
    os.rename(root + f1, root + "hcd_{0}_{1}_training.bin".format(oSize,c1))
    os.rename(root + f2, root + "hcd_{0}_{1}_validation.bin".format(oSize,c2))


def loadSpec(appendMode = False):
    mode = "ab" if appendMode else "wb"
    training = open(root + f1, mode)
    validation = open(root + f2, mode)
    c1, c2 = 0, 0
    start, stop = 0, 9
    for i in range(start, stop+1):
        os.chdir(root+str(i))
        files = glob.glob("*.png");
        validate = random.sample(files, math.floor(len(files)/10))
        files = list(set(files) - set(validate))
        for file in validate:
            print("\rsaving folder {0}/{1}, file {2}              ".format(str(i), str(stop), file), end='')
            path = root + "{0}/{1}".format(str(i), file)
            im = Image.open(path, 'r').convert("L")
            data = [2*x/255-1 for x in im.getdata()]
            np.array(i).tofile(validation, sep='', format='%L')
            np.array(data).tofile(validation, sep='', format='%d')
            c2 += 1
        print()
        for file in files:
            print("\rsaving folder {0}/{1}, file {2}              ".format(str(i), str(stop), file), end='')
            path = root + "{0}/{1}".format(str(i), file)
            im = Image.open(path, 'r').convert("L")
            data = [2*x/255-1 for x in im.getdata()]
            np.array(i).tofile(training, sep='', format='%L')
            np.array(data).tofile(training, sep='', format='%d')
            c1 += 1
    training.close()
    validation.close()
    if os.path.exists(root + "hcd_{0}_{1}_training.bin".format(oSize,c1)):
        os.remove(root + "hcd_{0}_{1}_training.bin".format(oSize,c1))
    if os.path.exists(root + "hcd_{0}_{1}_validation.bin".format(oSize,c2)):
        os.remove(root + "hcd_{0}_{1}_validation.bin".format(oSize,c2))
    os.rename(root + f1, root + "hcd_{0}_{1}_training.bin".format(oSize,c1))
    os.rename(root + f2, root + "hcd_{0}_{1}_validation.bin".format(oSize,c2))



#loadSpec()
#loadMNIST()








