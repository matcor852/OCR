import struct, glob, os, random, math
from PIL import Image





print()
root, f1, f2 = "D:/Code/C/OCR/NeuralNetwork/curated/", "hcd_784.bin", "hcd_784_test"
training = open(root + f1, "wb")
validation = open(root + f2, "wb")

c1, c2, start, stop = 0, 0, 48, 57
for i in range(start, stop+1):
    os.chdir(root+str(i))
    files = glob.glob("*.png");
    validate = random.sample(files, math.floor(len(files)/10))
    files = list(set(files) - set(validate))
    for file in validate:
        print("\rsaving folder {0}/{1}, file {2}              ".format(str(i), str(stop), file), end='')
        path = root + "{0}/{1}".format(str(i), file)
        im = Image.open(path, 'r')
        im = im.resize((28,28))
        data = list(im.getdata())
        M, m = max(data), min(data)
        if M-m == 0:
            print(M, m, data)
        div = M-m if M-m != 0 else 1
        data = [(x-m)/div for x in data]
        data.insert(0, i)
        validation.write(struct.pack('%sf' % 785, *data))
        c2 += 1
    for file in files:
        print("\rsaving folder {0}/{1}, file {2}              ".format(str(i), str(stop), file), end='')
        path = root + "{0}/{1}".format(str(i), file)
        im = Image.open(path, 'r')
        im = im.resize((28,28))
        data = list(im.getdata())
        M, m = max(data), min(data)
        if M-m == 0:
            print(M, m, data)
        div = M-m if M-m != 0 else 1
        data = [(x-m)/div for x in data]
        data.insert(0, i)
        training.write(struct.pack('%sf' % 785, *data))
        c1 += 1

training.close()
validation.close()
if os.path.exists(root + "hcd_784_{0}_training.bin".format(c1)):
    os.remove(root + "hcd_784_{0}_training.bin".format(c1))
if os.path.exists(root + "hcd_784_{0}_validation.bin".format(c2)):
    os.remove(root + "hcd_784_{0}_validation.bin".format(c2))
os.rename(root + f1, root + "hcd_784_{0}_training.bin".format(c1))
os.rename(root + f2, root + "hcd_784_{0}_validation.bin".format(c2))








