#EE569 Homework Assignment #4
#NAME: Pulkit Pattnaik
#USC ID: 6879618446
#USC email: pattnaik@usc.edu
#Submission date: 19 March 2019
import numpy as np
from sklearn.cluster import k_means
from sklearn.decomposition import PCA
import cv2

#DEFINE CONSTANTS
pad_size = 2
w = 510
h = 510
n_size = 17
n_pad_size = 8

# READ IMAGES
texture_image = np.fromfile('HW4_Images/comb.raw', dtype = np.uint8, sep = '').reshape([w, h]).astype(np.float16)
#MEAN SUBTRACTION
# texture_image -= np.mean(texture_image)

#LAWS FILTERS
filters_1d = np.zeros([5, 5])
filters_1d[0, :] = np.array([1,4,6,4,1])
filters_1d[1, :] = np.array([-1,-2,0,2,1])
filters_1d[2, :] = np.array([-1,0,2,0,-1])
filters_1d[3, :] = np.array([-1,2,0,-2,1])
filters_1d[4, :] = np.array([1,-4,6,-4,1])
filters_2d = np.zeros([25, 5, 5])
for i in range(5):
    for j in range(5):
        filters_2d[5 * i + j, :, :] = np.dot(filters_1d[i, :].reshape([-1, 1]), filters_1d[j, :].reshape([1, -1]))

#CONVOLUTION
filtered_image = np.zeros([w, h, 25])
energies = np.zeros([w, h, 25])
padded_image = np.pad(texture_image, ((pad_size, pad_size), (pad_size, pad_size)), 'reflect')
for i in range(h):
    for j in range(w):
        for k in range(25):
            filtered_image[i, j, k] = np.sum(padded_image[i: i + 5,j: j + 5] * filters_2d[k, :, :])

padded_filtered_image = np.zeros([w + 2 * n_pad_size, h + 2 * n_pad_size, 25])
for i in range(25):
    padded_filtered_image[:, :, i] = np.pad(filtered_image[:, :, i], ((n_pad_size, n_pad_size), (n_pad_size, n_pad_size)), 'reflect')

for i in range(h):
    for j in range(w):
        energies[i, j, :] = 1 / (n_size * n_size) * np.sum(np.abs(padded_filtered_image[i: i + n_size, j: j + n_size]), axis = (0,1)).reshape([1, 25])
        # mean_energy = np.mean(energies[i, j, :])
        # stddev_energy = np.std(energies[i, j, :])
        # energies[i, j, :] = (energies[i, j, :] - mean_energy) / stddev_energy
        energies[i, j, :] /= energies[i, j, 0]

result = k_means(energies.reshape([-1, 25]), n_clusters = 7)
pixels = result[1]
pixels[pixels == 1] = 42
pixels[pixels == 2] = 84
pixels[pixels == 3] = 126
pixels[pixels == 4] = 168
pixels[pixels == 5] = 210
pixels[pixels == 6] = 255

np.array(pixels).reshape([w, h]).astype(np.uint8).tofile('texture_seg_without_pca.raw')
cv2.dilate(cv2.erode(pixels ,cv2.getStructuringElement(cv2.MORPH_CROSS,(5, 5)),iterations = 3), cv2.getStructuringElement(cv2.MORPH_CROSS,(5, 5)),iterations = 3).astype(np.uint8).tofile('texture_seg_without_pca_enhanced.raw')

result_pca = k_means(PCA(n_components = 3).fit_transform(energies.reshape([-1, 25])), n_clusters = 7)
pixels = result_pca[1]
pixels[pixels == 1] = 42
pixels[pixels == 2] = 84
pixels[pixels == 3] = 126
pixels[pixels == 4] = 168
pixels[pixels == 5] = 210
pixels[pixels == 6] = 255

np.array(pixels).reshape([w, h]).astype(np.uint8).tofile('texture_seg_with_pca.raw')
cv2.dilate(cv2.erode(pixels ,cv2.getStructuringElement(cv2.MORPH_CROSS,(5, 5)),iterations = 3), cv2.getStructuringElement(cv2.MORPH_CROSS,(5, 5)),iterations = 3).astype(np.uint8).tofile('texture_seg_without_pca_enhanced.raw')