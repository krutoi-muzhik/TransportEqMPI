import numpy as np
import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

NT = 100
NX = 100

L = 10
T = 1.0
h = L / NX
tau = T / NT

x_range = np.arange(0, L, h)
t_range = np.arange(0, T, tau)

u = np.zeros((NT, NX))
with open('dump.txt') as file:
    for n in range(NT):
        u[n] = np.array([file.readline().split()])

t_array = np.zeros(x_range.size * t_range.size)
x_array = np.zeros(x_range.size * t_range.size)
u_array = np.zeros(x_range.size * t_range.size)

for i in range(t_range.size):
    for j in range(x_range.size):
        t_array[i * x_range.size + j] = t_range[i]
        x_array[i * x_range.size + j] = x_range[j]
        u_array[i * x_range.size + j] = u[i, j]

fig = plt.figure(figsize = (12, 12))
ax = fig.add_subplot(projection = '3d')
ax.scatter(x_array, t_array, u_array)

plt.show()
