import glob, os, random, math, pandas
from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
from parse import parse


size = (28,28)
oSize = size[0]*size[1]

print()

root = "D:/Code/TP/C/OCR/NeuralNetwork/DataSets/"
ratio = 7


def loadMNIST(file):
    training = open(root + "hcd_{0}_training.bin".format(oSize), "wb")
    validation = open(root + "hcd_{0}_validation.bin".format(oSize), "wb")
    df = pandas.read_csv(root + file)
    c1, c2, buffer = 0, 0, ratio
    for i, pict in enumerate(df.itertuples()):
        io = validation if buffer == 0 else training
        data = list(pict)
        if data[2] == 0: continue
        print("\rsaving train picture {0}              ".format(str(data[1])), end='')
        vls = [2*x/255-1 for x in data[3:]]
        np.array(data[2]).tofile(io, sep='', format='%L')
        np.array(vls).tofile(io, sep='', format='%d')
        if buffer == 0:
            c2 += 1
            buffer = ratio
        else: c1 += 1
        buffer -= 1
    training.close()
    validation.close()
    os.rename(root + "hcd_{0}_training.bin".format(oSize), root + "hcd_{0}_{1}_training.bin".format(oSize,c1))
    os.rename(root + "hcd_{0}_validation.bin".format(oSize), root + "hcd_{0}_{1}_validation.bin".format(oSize,c2))

def loadEMNIST(files: tuple, Sfile = ("", "")):
    if Sfile != ("", ""):
        training = open(root + Sfile[0], "ab")
        validation = open(root + Sfile[1], "ab")
        c1, c2 = int(parse("hcd_{0}_{1}_training.bin", Sfile[0])[1]), int(parse("hcd_{0}_{1}_validation.bin", Sfile[1])[1])
    else:
        training = open(root + "hcd_{0}_training.bin".format(oSize), "wb")
        validation = open(root + "hcd_{0}_validation.bin".format(oSize), "wb")
        c1, c2 = 0, 0
    df = pandas.read_csv(root + files[0])
    for i,pict in enumerate(df.itertuples()):
        data = list(pict)
        if data[1] == 0: continue
        print("\rsaving train picture {0}              ".format(str(data[0])), end='')
        vls = [2*x/255-1 for x in data[2:]]
        np.array(data[1]).tofile(training, sep='', format='%L')
        np.array(vls).tofile(training, sep='', format='%d')
        c1 += 1
    training.close()
    print()
    df = pandas.read_csv(root + files[1])
    for i,pict in enumerate(df.itertuples()):
        data = list(pict)
        if data[1] == 0: continue
        print("\rsaving test picture {0}              ".format(str(i)), end='')
        vls = [2*x/255-1 for x in data[2:]]
        np.array(data[1]).tofile(validation, sep='', format='%L')
        np.array(vls).tofile(validation, sep='', format='%d')
        c2 += 1
    validation.close()
    if Sfile != ("", ""):
        os.rename(root + Sfile[0], root + "hcd_{0}_{1}_training.bin".format(oSize,c1))
        os.rename(root + Sfile[1], root + "hcd_{0}_{1}_validation.bin".format(oSize,c2))
    else:
        os.rename(root + "hcd_{0}_training.bin".format(oSize), root + "hcd_{0}_{1}_training.bin".format(oSize,c1))
        os.rename(root + "hcd_{0}_validation.bin".format(oSize), root + "hcd_{0}_{1}_validation.bin".format(oSize,c2))


def loadSpec(folder, hotV, Sfile = ("", "")):
    os.chdir(root+folder)
    files = glob.glob("*.png");
    c1, c2 = 0, 0
    if Sfile != ("", ""):
        training = open(root + Sfile[0], "ab")
        validation = open(root + Sfile[1], "ab")
        c1, c2 = int(parse("hcd_{0}_{1}_training.bin", Sfile[0])[1]), int(parse("hcd_{0}_{1}_validation.bin", Sfile[1])[1])
    c2 += math.floor(len(files)/ratio)
    c1 += len(files) - math.floor(len(files)/ratio)
    if Sfile == ("", ""):
        training = open(root + "hcd_{0}_{1}_training.bin".format(oSize,c1), "wb")
        validation = open(root + "hcd_{0}_{1}_validation.bin".format(oSize,c2), "wb")
    validate = random.sample(files, math.floor(len(files)/ratio))
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


#loadEMNIST(("emnist-byclass-train.csv", "emnist-byclass-test.csv"))
#loadSpec("_", 16, ("hcd_784_38400_training.bin", "hcd_784_6400_validation.bin"))
#loadEMNIST(("mnist_train.csv", "mnist_test.csv"))
loadSpec("_", 0, ("hcd_784_54077_training.bin", "hcd_784_9020_validation.bin"))







