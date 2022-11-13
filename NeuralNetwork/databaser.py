import struct, glob, os, random, math, pandas
from PIL import Image
import matplotlib.pyplot as plt


size = (28,28)
oSize = size[0]*size[1]+1

print()

root, f1, f2 = "D:/Code/C/OCR/NeuralNetwork/DataSets/", "hcd_{0}.bin".format(oSize-1), "hcd_{0}_test".format(oSize-1)
training = open(root + f1, "wb")
validation = open(root + f2, "wb")


def loadMNIST():
    c1, c2 = 0, 0
    df = pandas.read_csv(root+"mnist_train.csv")
    for pict in df.itertuples():
        data = list(pict)
        print("\rsaving train picture {0}              ".format(str(data[0])), end='')
        vls = [2*x/255-1 for x in data[2:]]
        vls.insert(0, data[1])
        training.write(struct.pack('%sf' % oSize, *vls))
        c1 += 1
    df = pandas.read_csv(root+"mnist_test.csv")
    for pict in df.itertuples():
        data = list(pict)
        print("\rsaving test picture {0}              ".format(str(data[0])), end='')
        vls = [2*x/255-1 for x in data[2:]]
        vls.insert(0, data[1])
        validation.write(struct.pack('%sf' % oSize, *vls))
        c2 += 1
    return c1, c2


def loadSpec():
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
            data.insert(0, i)
            validation.write(struct.pack('%sf' % oSize, *data))
            c2 += 1
        for file in files:
            print("\rsaving folder {0}/{1}, file {2}              ".format(str(i), str(stop), file), end='')
            path = root + "{0}/{1}".format(str(i), file)
            im = Image.open(path, 'r').convert("L")
            data = [2*x/255-1 for x in im.getdata()]
            data.insert(0, i)
            training.write(struct.pack('%sf' % oSize, *data))
            c1 += 1
    return c1, c2



#c1, c2 = loadSpec()
c1, c2 = loadSpec()



training.close()
validation.close()
if os.path.exists(root + "hcd_{0}_{1}_training.bin".format(oSize-1,c1)):
    os.remove(root + "hcd_{0}_{1}_training.bin".format(oSize-1,c1))
if os.path.exists(root + "hcd_{0}_{1}_validation.bin".format(oSize-1,c2)):
    os.remove(root + "hcd_{0}_{1}_validation.bin".format(oSize-1,c2))
os.rename(root + f1, root + "hcd_{0}_{1}_training.bin".format(oSize-1,c1))
os.rename(root + f2, root + "hcd_{0}_{1}_validation.bin".format(oSize-1,c2))








