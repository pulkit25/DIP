#NAME: Pulkit Pattnaik
#03 March 2019
import numpy as np
import matplotlib.pyplot as plt

img = np.fromfile('Home Work No. 3 Images/classroom.raw', dtype=np.uint8, sep='').reshape([1072, 712])
N, M = img.shape
print(img.shape)
Vc = M / 2
Uc = N / 2
f = 600
K1 = -0.3536
K2 = 0.1730

x = np.zeros(img.shape)
y = np.zeros(img.shape)
r = np.zeros(img.shape)
out_img = np.zeros(img.shape)
x_corrected = np.zeros(img.shape)
y_corrected = np.zeros(img.shape)

for i in range(N):
    for j in range(M):
        x[i,j] = (i - Uc) / f
        y[i,j] = (j - Vc) / f
        r[i,j] = np.sqrt(np.square(x[i,j]) + np.square(y[i,j]))
        x_corrected[i,j] = x[i,j] * (1 + K1 * np.square(r[i,j]) + K2 * np.square(np.square(r[i,j])))
        y_corrected[i,j] = y[i,j] * (1 + K1 * np.square(r[i,j]) + K2 * np.square(np.square(r[i,j])))
        x_corrected[i,j] = x_corrected[i,j] * f + Uc
        y_corrected[i,j] = y_corrected[i,j] * f + Vc
        out_img[i,j] = img[int(x_corrected[i,j]), int(y_corrected[i,j])]

plt.imshow(out_img)
out_img.astype(np.uint8).tofile('corrected_distortion.raw')
