#Common
import sys
import numpy as np
import matplotlib.pyplot as plt

#For 3d
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import LinearLocator, FormatStrFormatter


def linePlot():
	plt.figure(num=None, facecolor='w', edgecolor='k')
	plt.xlabel('Pixels')
	plt.ylabel('Waves')
	
	# Trial
	xVal = range(512)
	Z = np.genfromtxt('Data/zmap.csv', delimiter=',')[:,:-1]
	X = Z[256,:]
	Y = Z[:,256]
	
	plt.grid(linestyle='-')
	plt.scatter(xVal, X, label='X', marker='.')
	plt.scatter(xVal, Y, label='Y', marker='.')
	#plt.axis([0, 5000, 50, 20])
	
	plt.title("Line Profile.")
	plt.show()
	
	#print(xVal)
	
def sinePlot():
	#fig = plt.figure(figsize=(20,10))
	fig = plt.figure()
	
	# Make data.
	X = np.genfromtxt('Data/interMap.csv', delimiter=',')[:,:-1]
	#f = lambda x: math.sin ( x * ( 4 * 3.14 / 0.6328) )
	#Y = f(X)
	Y = np.cos(X * ( 4 * 3.14 / 0.6328) )
	#Y = np.cos(X)
	plt.imshow(Y, cmap='gray')
	plt.show()
	
def contourPlot():
	fig = plt.figure()

	# Make data.
	X = np.arange(0, 512, 1)
	Y = np.arange(0, 512, 1)
	X, Y = np.meshgrid(X, Y)
	Z = np.genfromtxt('Data/zmap.csv', delimiter=',')[:,:-1]
	
	# Plot the surface.
	plt.contourf(X, Y, Z, 512, cmap = 'coolwarm', antialiased = True)
	fig.savefig('Data/contour.png', bbox_inches='tight')
	plt.close()
	
def zPlot():
	#fig = plt.figure(figsize=(10,5))
	fig = plt.figure()
	ax = fig.gca(projection='3d')
	
	# Make data.
	X = np.arange(0, 512, 1)
	Y = np.arange(0, 512, 1)
	X, Y = np.meshgrid(X, Y)
	Z = np.genfromtxt('Data/zmap.csv', delimiter=',')[:,:-1]
	
	# Plot the surface.
	surf = ax.plot_surface(X, Y, Z, cmap = 'coolwarm', linewidth = 1, antialiased = False) #To make the plot opaque antialiased = False
	#surf = ax.contour3D(X, Y, Z, 512, cmap = cm.Wistia, antialiased = True)
	ax.axis('equal')
	ax.view_init(elev=55)
	
	# Customize the z axis.
	ax.set_zlim(Z.min(), Z.max() * 2)
	ax.zaxis.set_major_locator(LinearLocator(10))
	ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))
	
	# Add a color bar which maps values to colors.
	fig.colorbar(surf, shrink=0.5, aspect=5)
	
	#Set axes labels
	ax.set_xlabel('$Pixels$', fontsize=10, rotation=150)
	ax.set_ylabel('$Pixels$', fontsize=10)
	ax.set_zlabel(r'$waves$', fontsize=10, rotation=60)
	#ax.set_zlabel(r'$\lambda (waves)$', fontsize=30, rotation=60)
	
	fig.savefig('Data/plot.png', bbox_inches='tight')
	
	if sys.argv[2] == "true":
		plt.show()
	
	plt.close()

if __name__ == '__main__':
    switcher = {
        "1": linePlot,
        "2": sinePlot,
        "3": contourPlot,
        "4": zPlot,
    }
    # Get the function from switcher dictionary
    # func = switcher.get(sys.argv[1], "Invalid Argument")
    # func()
    # OR Execute the function directly
    switcher[sys.argv[1]]()
    
