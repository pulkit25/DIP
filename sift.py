#EE569 Homework Assignment #4
#NAME: Pulkit Pattnaik
#USC ID: 6879618446
#USC email: pattnaik@usc.edu
#Submission date: 19 March 2019
import cv2
import numpy as np
from operator import attrgetter
import matplotlib.pyplot as plt

river1 = np.fromfile('HW4_Images/river1.raw', dtype = np.uint8).reshape([1024, 768, 3])
river2 = np.fromfile('HW4_Images/river2.raw', dtype = np.uint8).reshape([1024, 768, 3])
gray1 = cv2.cvtColor(river1, cv2.COLOR_BGR2GRAY)
gray2 = cv2.cvtColor(river2, cv2.COLOR_BGR2GRAY)

sift = cv2.xfeatures2d.SIFT_create()
kp1, des1 = sift.detectAndCompute(gray1, None)
kp2, des2 = sift.detectAndCompute(gray2, None)
# strongest_kp1_index = kp1.index(max(kp1, key = attrgetter('response')))

bf = cv2.BFMatcher()
matches = bf.match(des1, des2)
matches = sorted(matches, key = lambda x:x.distance)
#matches = sorted(matches, key = lambda x:x.distance)
img3 = np.array([])
draw_params = dict(matchColor = (0,0,255), flags = 2)
img3 = cv2.drawMatches(river1, kp1, river2, kp2, matches[:1], img3, **draw_params)
plt.imshow(img3),plt.show()