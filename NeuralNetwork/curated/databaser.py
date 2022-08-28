import struct, glob, os
from PIL import Image





print()
root, f = "D:/Code/C/OCR/NeuralNetwork/curated/", "hcd_784.bin"
out = open(root + f, "wb")

c, start, stop = 0, 48, 57
for i in range(start, stop+1):
    os.chdir(root+str(i))
    for file in glob.glob("*.png"):
        print("\rsaving folder {0}/{1}, file {2}".format(str(i), str(stop), file), end='')
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
        out.write(struct.pack('%sf' % 785, *data))
        c += 1

out.close()
os.rename(root + f, root + "hcd_784_{0}.bin".format(c))








