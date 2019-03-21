#NAME: Pulkit Pattnaik
#03 March 2019
import numpy as np

def bilinear_interpolation(pixel11: np.uint8, pixel12: np.uint8, pixel21: np.uint8, pixel22: np.uint8, y1: int, y2: int, y: int, x1: int, x2: int, x: int) -> np.uint8:
    pixel_int = ((x2 - x) * (y2 - y) / ((x2 - x1) * (y2 - y1))) * pixel11 + ((x2 - x) * (y - y1) / ((x2 - x1) * (y2 - y1))) * pixel12 + ((x - x1) * (y2 - y) / ((x2 - x1) * (y2 - y1))) * pixel21 + ((x - x1) * (y - y1) / ((x2 - x1) * (y2 - y1))) * pixel22
    return np.uint8(pixel_int)

orig = np.fromfile('Home Work No. 3 Images/lighthouse.raw', dtype=np.uint8, sep='')
orig = orig.reshape([512, 512])

hole_locs = np.zeros([3,2])
ylocs, xlocs = np.where(orig == 255)
ctr = 0
for xloc, yloc in zip(xlocs, ylocs):
    if xloc + 160 < orig.shape[0]:
        if np.sum(orig[yloc: yloc + 160, xloc: xloc + 160]) == 160 * 160 * 255:
            hole_locs[ctr, 0] = xloc
            hole_locs[ctr, 1] = yloc
            ctr += 1

holeimg = np.zeros([256, 256, 3])
holeimg[:,:,0] = np.fromfile('Home Work No. 3 Images/lighthouse2.raw', dtype=np.uint8, sep='').reshape([256, 256])
holeimg[:,:,1] = np.fromfile('Home Work No. 3 Images/lighthouse1.raw', dtype=np.uint8, sep='').reshape([256, 256])
holeimg[:,:,2] = np.fromfile('Home Work No. 3 Images/lighthouse3.raw', dtype=np.uint8, sep='').reshape([256, 256])
#FLAGS = 0 means downscale and 1 means upscale
flagx = 0
flagy = 0
for i in  range(3):
    yindices, xindices = np.where(holeimg[:,:,i] != 255)
    minx = np.min(xindices)
    maxx = np.max(xindices)
    miny = np.min(yindices)
    maxy = np.max(yindices)

    x1, y1 = xindices[np.where(yindices == miny)[0][0]], miny
    x2, y2 = xindices[np.where(yindices == maxy)[0][-1]], maxy
    x3, y3 = minx, yindices[np.where(xindices == minx)[0][0]]
    x4, y4 = maxx, yindices[np.where(xindices == maxx)[0][-1]]
    if i == 0:
        theta = np.pi / 2 - np.arctan((y4 - y2) / (x4 - x2))
    elif i == 2:
        theta = - np.pi / 2 - np.arctan((y4 - y2) / (x4 - x2))
    else:
        theta = - np.arctan((y4 - y2) / (x4 - x2))
    scalex = np.ceil(160 / np.sqrt(np.square(x1 - x3) + np.square(y1 - y3)) * 104) / 100
    scaley = np.ceil(160 / np.sqrt(np.square(x1 - x4) + np.square(y1 - y4)) * 104) / 100
    midptx = (x1 + x2) / 2
    midpty = (y1 + y2) / 2
    if scalex > 1:
        flagx = 1
    if scaley > 1:
        flagy = 1
    holeptx = hole_locs[i, 0]
    holepty = hole_locs[i, 1]

    tx = - midptx - 1
    ty = - midpty - 1
    rotation_matrix = np.array([[np.cos(theta), -np.sin(theta), 0], [np.sin(theta), np.cos(theta), 0], [0, 0, 1]])
    scaling_matrix = np.array([[scalex, 0, 0], [0, scaley, 0], [0, 0, 1]])
    translational_matrix_1 = np.array([[1, 0, tx], [0, 1, ty], [0, 0, 1]])
    translational_matrix_2 = np.array([[1, 0, -tx], [0, 1, -ty], [0, 0, 1]])
    reverse_mapping = np.linalg.pinv(np.dot(translational_matrix_2, np.dot(rotation_matrix, np.dot(scaling_matrix, translational_matrix_1))))

    coordx = 0
    for x in np.arange(midptx - 80, midptx + 79):
        coordy = 0
        for y in np.arange(midpty - 80, midpty + 79):
            out = np.dot(reverse_mapping, np.array([x, y, 1]).T)
            u = out[0]
            v = out[1]
            u1 = int(np.floor(u))
            u2 = int(np.ceil(u))
            v1 = int(np.floor(v))
            v2 = int(np.ceil(v))
            orig[int(holepty) + coordy, int(holeptx) + coordx] = holeimg[int(v), int(u), i]#bilinear_interpolation(holeimg[v1, u1, i], holeimg[v1, u2, i], holeimg[v2, u1, i], holeimg[v2, u2, i], v1, v2, v, u1, u2, u)
            coordy += 1
        coordx += 1
orig.astype(np.uint8).tofile('lighthouse_new.raw')