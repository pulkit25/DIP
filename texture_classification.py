#NAME: Pulkit Pattnaik
#19 March 2019
import numpy as np
from sklearn.cluster import k_means
from sklearn.decomposition import PCA

#DEFINE CONSTANTS
pad_size = 2
w = 128
h = 128

texture_images = np.zeros([12, w, h])
for i in range(12):
    # READ IMAGES
    texture_images[i, :, :] = np.fromfile('HW4_Images/texture%d.raw'%(i + 1), dtype = np.uint8, sep = '').reshape([w, h]).astype(np.float16)
    #MEAN SUBTRACTION
    texture_images[i, :, :] -= np.mean(texture_images[i, :, :])

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
energies = np.zeros([12, 25])
for m in range(12):
    padded_image = np.pad(texture_images[m, :, :], ((pad_size, pad_size),(pad_size, pad_size)), 'reflect')
    for i in range(h):
        for j in range(w):
            for k in range(25):
                filtered_image[i, j, k] = np.sum(padded_image[i: i + 5,j: j + 5] * filters_2d[k, :, :])
    energies[m, :] = 1 / (w * h) * np.sum(np.abs(filtered_image), axis = (0,1)).reshape([1, 25])
    mean_energy = np.mean(energies[m, :])
    stddev_energy = np.std(energies[m, :])
    energies[m, :] = (energies[m, :] - mean_energy) / stddev_energy
    print("Image: %d"%m)

print(energies[0, :] - energies[1, :])
print(energies[4, :] - energies[7, :])
print(energies[2, :] -  energies[3, :])
print(energies[5, :] - energies[8, :])
print(energies[10, :] - energies[11, :])
result = k_means(energies, n_clusters = 4)
print("Without PCA:")
print("Feature vectors:")
print(result[0])
print("Clusters:")
print(result[1])
result_pca = k_means(PCA(n_components = 3).fit_transform(energies), n_clusters = 4)
print("With PCA:")
print("Feature vectors:")
print(result_pca[0])
print("Clusters:")
print(result_pca[1])