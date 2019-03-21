#NAME: Pulkit Pattnaik
#03 March 2019
import numpy as np

orig = np.fromfile('Home Work No. 3 Images/hat.raw', dtype=np.uint8, sep='')
orig = orig.reshape([512, 512])
warped = np.zeros([512, 512])
#4 triangles
for i in range(4):
    if i == 0:
        x0, y0 = 255, 255
        x1, y1 = 127 + 255, 127
        x2, y2 = 127, 127
        x3, y3 = 511, 0
        x4, y4 = 255, 0
        x5, y5 = 0, 0
        u4, v4 = 255, 127
        out = np.dot(np.array([[x0, x1, x2, x3, u4, x5],[y0, y1, y2, y3, v4, y5]]), np.linalg.pinv(np.array([[1, 1, 1, 1, 1, 1], [x0, x1, x2, x3, x4, x5], [y0, y1, y2, y3, y4, y5], [x0 * x0, x1 * x1, x2 * x2, x3 * x3, x4 * x4, x5 * x5], [x0 * y0, x1 * y1, x2 * y2, x3 * y3, x4 * y4, x5 * y5], [y0 * y0, y1 * y1, y2 * y2, y3 * y3, y4 * y4, y5 * y5]])))
        for y in range(256):
            for x in range(y, 512 - y):
                mat = np.dot(out, np.array([1, x, y, x * x, x * y, y * y]).T).astype(int)
                u = mat[0]
                v = mat[1]
                warped[v, u] = orig[y, x]

    elif i == 1:
        x0, y0 = 255, 255
        x1, y1 = 127, 127 + 255
        x2, y2 = 127 + 255, 127 + 255
        x3, y3 = 0, 511
        x4, y4 = 255, 511
        x5, y5 = 511, 511
        u4, v4 = 255, 127 + 255
        out = np.dot(np.array([[x0, x1, x2, x3, u4, x5], [y0, y1, y2, y3, v4, y5]]), np.linalg.pinv(np.array(
            [[1, 1, 1, 1, 1, 1], [x0, x1, x2, x3, x4, x5], [y0, y1, y2, y3, y4, y5],
             [x0 * x0, x1 * x1, x2 * x2, x3 * x3, x4 * x4, x5 * x5],
             [x0 * y0, x1 * y1, x2 * y2, x3 * y3, x4 * y4, x5 * y5],
             [y0 * y0, y1 * y1, y2 * y2, y3 * y3, y4 * y4, y5 * y5]])))
        print(out)
        for y in range(256, 512):
            for x in range(512 - y, y):
                mat = np.dot(out, np.array([1, x, y, x * x, x * y, y * y]).T).astype(int)
                u = mat[0]
                v = mat[1]
                warped[v, u] = orig[y, x]

    elif i == 2:
        x0, y0 = 255, 255
        x1, y1 = 127, 127
        x2, y2 = 127, 127 + 255
        x3, y3 = 0, 0
        x4, y4 = 0, 255
        x5, y5 = 0, 511
        u4, v4 = 127, 255
        out = np.dot(np.array([[x0, x1, x2, x3, u4, x5], [y0, y1, y2, y3, v4, y5]]), np.linalg.pinv(np.array(
            [[1, 1, 1, 1, 1, 1], [x0, x1, x2, x3, x4, x5], [y0, y1, y2, y3, y4, y5],
             [x0 * x0, x1 * x1, x2 * x2, x3 * x3, x4 * x4, x5 * x5],
             [x0 * y0, x1 * y1, x2 * y2, x3 * y3, x4 * y4, x5 * y5],
             [y0 * y0, y1 * y1, y2 * y2, y3 * y3, y4 * y4, y5 * y5]])))
        for y in range(512):
            for x in range(0, min(y, 512 - y)):
                mat = np.dot(out, np.array([1, x, y, x * x, x * y, y * y]).T).astype(int)
                u = mat[0]
                v = mat[1]
                warped[v, u] = orig[y, x]
    elif i == 3:
        x0, y0 = 255, 255
        x1, y1 = 127 + 255, 127 + 255
        x2, y2 = 127 + 255, 127
        x3, y3 = 511, 511
        x4, y4 = 511, 255
        x5, y5 = 511, 0
        u4, v4 = 127 + 255, 255
        out = np.dot(np.array([[x0, x1, x2, x3, u4, x5], [y0, y1, y2, y3, v4, y5]]), np.linalg.pinv(np.array(
            [[1, 1, 1, 1, 1, 1], [x0, x1, x2, x3, x4, x5], [y0, y1, y2, y3, y4, y5],
             [x0 * x0, x1 * x1, x2 * x2, x3 * x3, x4 * x4, x5 * x5],
             [x0 * y0, x1 * y1, x2 * y2, x3 * y3, x4 * y4, x5 * y5],
             [y0 * y0, y1 * y1, y2 * y2, y3 * y3, y4 * y4, y5 * y5]])))
        for y in range(512):
            for x in range(max(y, 512 - y), 512):
                mat = np.dot(out, np.array([1, x, y, x * x, x * y, y * y]).T).astype(int)
                u = mat[0]
                v = mat[1]
                warped[v, u] = orig[y, x]

warped.astype(np.uint8).tofile('warped.raw')