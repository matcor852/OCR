from PIL import Image
import random, time, matplotlib, sys, glob, os
import numpy as np
import albumentations as A
import matplotlib.pyplot as plt

random.seed(time.time_ns())

transforms = ["A.Blur(p=1.0)",
              "A.MedianBlur(p=1.0)",
              "A.Defocus(radius=(3,5), p=1.0)",
              "A.GaussianBlur(sigma_limit=9, p=1.0)",
              "A.GlassBlur(sigma=0.6, max_delta=1, p=1.0)",
              "A.MotionBlur(blur_limit=9, allow_shifted=True, p=1.0)",
              "A.PixelDropout(dropout_prob=0.04, drop_value=0, p=1.0)",
              "A.Perspective(scale=(0.05, 0.1), keep_size=True, p=1.0)",
              "A.Rotate(limit=25, interpolation=1, border_mode=4, p=1.0)",
              "A.SafeRotate(limit=25, interpolation=1, border_mode=4, p=1.0)",
              "A.PixelDropout(dropout_prob=0.0025, drop_value=255, p=1.0)",
              "A.ZoomBlur(max_factor=(1.1,1.8), step_factor=(0.01, 0.02), p=1.0)",
              "A.Downscale(scale_min=0.4, scale_max=0.8, interpolation=None, p=1.0)",
              "A.GridDistortion(num_steps=5, distort_limit=0.3, interpolation=1, border_mode=4, p=1.0)",
              "A.ElasticTransform(alpha=1, sigma=5, alpha_affine=5, interpolation=1, border_mode=4, p=1.0)",
              "A.OpticalDistortion(distort_limit=0.6, shift_limit=8, interpolation=1, border_mode=4, p=1.0)",
              "A.ShiftScaleRotate(shift_limit=0.1, scale_limit=0.08, rotate_limit=20, interpolation=1, p=1.0)",
              "A.CoarseDropout(max_holes=100, max_height=2, max_width=2, min_holes=50, min_height=1, min_width=1, fill_value=0, p=1.0)"]


def Augment(root):
    for i in range(10):
        os.chdir(root+str(i))
        files = glob.glob("*.png");
        for file in files:
            image = np.array(Image.open(root+str(i)+"/"+file))
            for trsf in transforms:
                combined = A.Compose([
                    eval("A.ShiftScaleRotate(shift_limit=0.1, scale_limit=0.08, rotate_limit=20, interpolation=1, p=1.0)"),
                    eval(trsf)
                    ])
                name = trsf[trsf.find('A.')+len('A.'):trsf.find("(")]
                matplotlib.image.imsave("{0}-{1}_{2}.png".format(i, name, random.randint(1, sys.maxsize)), combined(image=image)['image'])

            for _ in range(len(transforms)):
                pckd = random.sample(transforms, random.randint(2,6))
                combined = A.Compose([eval(trsf) for trsf in pckd])
                matplotlib.image.imsave("{0}-{1}Combined_{2}.png".format(i, len(pckd), random.randint(1, sys.maxsize)), combined(image=image)['image'])

def Resize(root):
    for i in range(2,3):
        os.chdir(root+str(i))
        files = glob.glob("*.png");
        for file in files:
            image = Image.open(root+str(i)+"/"+file)
            image = image.resize((28,28), Image.ANTIALIAS)
            image.save(file)

def GenNoise(root, nb):
    plt.gray()
    os.chdir(root)
    image = np.zeros((28,28), dtype='float')
    matplotlib.image.imsave("_-0.png", image)
    i = 1
    while i < nb:
        tmp = A.PixelDropout(dropout_prob=random.uniform(0.001, 0.001), drop_value=255, p=1.0)(image=image)['image']
        tmp = A.GlassBlur(sigma=0.5, max_delta=3, p=1.0)(image=tmp)['image']
        #plt.imshow(tmp)
        #plt.show()
        if set(map(tuple, image)).symmetric_difference(set(map(tuple, tmp))) != set():
            matplotlib.image.imsave("_-{0}.png".format(i), tmp)
            i += 1



root = "D:/Code/C/OCR/NeuralNetwork/DataSets/_/"

GenNoise(root, 20000)























