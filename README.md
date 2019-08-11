# FizeauOpenCV

This software project was done as part of collaboration for our Laser Optics Group at our center. The objective is to design a fully functional Fizeau Interferometer. 
This software computes wavefront from of Fizeau interferograms and then performs analysis using Zernike Kernels. Follwing functions are implemented:
1. Phase computation (src/wrap.cpp)
2. Phase unwrapping (src/unwrap.cpp) - Based on Histogram Phase Unwrapping from OpenCV Contrib
3. Wavefront Analysis (src/zernike.cpp)

## Requirements
1. This module has been compiled and tested on Ubuntu 18.04 LTS. Other Linux OS have not been tested, but they should work. 
2. OpenCV, main and contrib.
3. Python3, Matplotlib, Numpy - for 3D plotting purpose.
4. All packages available in Ubuntu repositories have been used so as to keep the deployment simple. Install them by:
```
sudo apt install libopencv-dev libopencv-contrib-dev python3-matplotlib
```
This will pull all required dependencies.


## Folder Organization
1. The bin directory contains the binaries and associated data files. This directory is automatically generated by the build process.
2. The include directory contains header declarations.
3. The scripts directory contains Python3 scripts to create 3D surface maps.
4. The src directory contains the C++ source code.

## Compile
1. Create a directory named bin
2. Go to directory build and issue following commands: 
```
cmake .
make && make install
```

## Usage
1. Change to bin directory
2. Issue following command: 
```
./driver.out <path/to/folder/containing/phase/shifted/images>
```
