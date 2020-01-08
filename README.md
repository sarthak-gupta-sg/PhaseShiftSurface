# FizeauOpenCV

This project is a collaborative software for Laser Optics Group at our center. The objective is to design a fully functional low cost Fizeau Interferometer. The complete software performs acquisition of a sequence of 5 images from a camera (choice of Baumer, Basler, PCO, TIS), stepper motor control for the interferometer, computation of wavefront follwed by wavefront analysis. The subset of software (in this repository) computes wavefront from a set of 5 Fizeau interferograms and then performs wavefront analysis using Zernike Kernels. Follwing functions are implemented:

1. Resizing Images (src/resize.cpp) - converting images to 512x512 pixels to fit the Zernike Kernels size.
2. Check Size (src/checksize.cpp) - Performs input sanitization and checks.
3. Phase computation (src/wrap.cpp)
4. Phase unwrapping (src/unwrap.cpp) - Based on Histogram Phase Unwrapping from OpenCV Contrib
5. Wavefront Analysis (src/zernike.cpp)
6. Driver for all modules above (src/driver.cpp)

## Requirements for compilation
1. This module has been compiled and tested on Ubuntu 18.04 LTS amd64. Work is undergoing to port software to ARM based SBCs. 
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
3. The utils/scripts directory contains Python3 scripts to create 3D surface maps.
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
3. There is an assumption that there would be total of 5 images named as {1,2,3,4,5}.bmp in the folder name supplied on command line. This is because the version of software currently deployed, acquires a sequence of 5 images automatically, saves them as {1,2,3,4,5}.bmp and forwards them for computation.
