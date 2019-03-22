#EE569 Homework Assignment #4
#NAME: Pulkit Pattnaik
#USC ID: 6879618446
#USC email: pattnaik@usc.edu
#Submission date: 19 March 2019
import cv2
import numpy as np
from sklearn.cluster import KMeans

one = np.zeros([28, 28, 5])
zero = np.zeros([28, 28, 5])
for i in range(5):
    one[:, :, i] = np.fromfile('HW4_Images/one_1.raw', dtype = np.uint8).reshape([28, 28])
    zero[:, :, i] = np.fromfile('HW4_Images/zero_1.raw', dtype = np.uint8).reshape([28, 28])

sift = cv2.xfeatures2d.SIFT_create(128)
kp = []
des = []
for i in range(5):
    kp1, des1 = sift.detectAndCompute(one[:, :, i].astype(np.uint8), None)
    kp2, des2 = sift.detectAndCompute(zero[:, :, i].astype(np.uint8), None)
    for d in des1:
        des.append(d)
    for d in des2:
        des.append(d)

result = KMeans(n_clusters = 2).fit(des)
img = np.fromfile('HW4_Images/eight.raw', dtype = np.uint8).reshape([28, 28])
kp, des = sift.detectAndCompute(img, None)
histo = np.zeros(2)
nkp = np.size(kp)

for d in des:
    idx = result.predict([d])
    histo[idx] += 1/nkp

print("Predicted class: %d"%np.argmax(histo))