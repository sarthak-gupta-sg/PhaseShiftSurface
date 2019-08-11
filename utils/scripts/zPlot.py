'''
======================
3D surface (color map)
======================

Demonstrates plotting a 3D surface colored with the coolwarm color map.
The surface is made opaque by using antialiased=False.

Also demonstrates using the LinearLocator and custom formatting for the
z axis tick labels.
'''

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np
import sys

#fig = plt.figure(figsize=(20,10))
fig = plt.figure()
ax = fig.gca(projection='3d')

# Make data.
X = np.arange(0, 512, 1)
Y = np.arange(0, 512, 1)
X, Y = np.meshgrid(X, Y)
Z = np.genfromtxt(sys.argv[1], delimiter=',')[:,:-1]

# Plot the surface.
surf = ax.plot_surface(X, Y, Z, cmap = cm.coolwarm, linewidth = 2, antialiased = True)
ax.axis('equal')

# Customize the z axis.
# ax.set_zlim(-2.0, 2.0)
# ax.zaxis.set_major_locator(LinearLocator(10))
# ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

# Add a color bar which maps values to colors.
fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()
