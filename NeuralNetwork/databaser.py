import glob, os, random, math, pandas
from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
from parse import parse


size = (28,28)
oSize = size[0]*size[1]

print()

root = "D:/Code/C/OCR/NeuralNetwork/DataSets/"

"""
def loadMNIST(file = ""):
    actTrain = parse()
    mode = "ab" if file else "wb"
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
"""

def loadSpec(folder, hotV, Sfile = ("", "")):
    os.chdir(root+folder)
    files = glob.glob("*.png");
    c1, c2 = 0, 0
    if Sfile != ("", ""):
        training = open(root + Sfile[0], "ab")
        validation = open(root + Sfile[1], "ab")
        c1, c2 = int(parse("hcd_{0}_{1}_training.bin", Sfile[0])[1]), int(parse("hcd_{0}_{1}_validation.bin", Sfile[1])[1])
    c2 += math.floor(len(files)/10)
    c1 += len(files) - math.floor(len(files)/10)
    if Sfile == ("", ""):
        training = open(root + "hcd_{0}_{1}_training.bin".format(oSize,c1), "wb")
        validation = open(root + "hcd_{0}_{1}_validation.bin".format(oSize,c2), "wb")
    validate = random.sample(files, math.floor(len(files)/10))
    files = list(set(files) - set(validate))
    for file in validate:
        print("\rsaving folder {0}, file {1}              ".format(folder, file), end='')
        path = root + "{0}/{1}".format(folder, file)
        im = Image.open(path, 'r').convert("L")
        data = [2*x/255-1 for x in im.getdata()]
        np.array(hotV).tofile(validation, sep='', format='%L')
        np.array(data).tofile(validation, sep='', format='%d')
    validation.close()
    print()
    for file in files:
        print("\rsaving folder {0}, file {1}              ".format(folder, file), end='')
        path = root + "{0}/{1}".format(folder, file)
        im = Image.open(path, 'r').convert("L")
        data = [2*x/255-1 for x in im.getdata()]
        np.array(hotV).tofile(training, sep='', format='%L')
        np.array(data).tofile(training, sep='', format='%d')
    training.close()
    if Sfile != ("", ""):
        os.rename(root + Sfile[0], root + "hcd_{0}_{1}_training.bin".format(oSize,c1))
        os.rename(root + Sfile[1], root + "hcd_{0}_{1}_validation.bin".format(oSize,c2))


#loadSpec("_", 16)
loadSpec("_", 16, ("hcd_784_60000_training.bin", "hcd_784_10000_validation.bin"))
#loadMNIST()








