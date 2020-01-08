# PhaseShiftSurface

This project is a collaborative software for Laser Optics Group at our center. The objective is to design a fully functional low cost Fizeau Interferometer. The complete software performs acquisition of a sequence of 5 images from a camera (choice of Baumer, Basler, PCO, TIS), stepper motor control for the interferometer, computation of wavefront follwed by wavefront analysis. The subset of software (in this repository) computes wavefront from a set of 5 Fizeau interferograms and then performs wavefront analysis using Zernike Kernels.

## Requirements for compilation
1. This project has been compiled and tested on Ubuntu 18.04 LTS amd64. Work is undergoing to port software to ARM based SBCs. 
2. OpenCV, main and contrib.
3. Python3, Matplotlib, Numpy - for 2D and 3D plotting purpose.
4. All packages available in Ubuntu repositories have been used so as to keep the deployment simple. Install them by:
```
sudo apt install libopencv-dev libopencv-contrib-dev python3-matplotlib
```
This will pull in all required dependencies.


## Folder Organization
1. The 'include' directory contains header declarations.
2. The 'scripts' directory contains Python3 scripts to create various 2D and 3D surface maps.
3. The 'src' directory contains the C++ source code.

## Compile
1. Create a directory named 'build' in current directory
2. Change to 'build' and issue following commands: 
```
cmake .. -DCMAKE_INSTALL_PREFIX=</path/where/you/have/write/permissions>
make && make install
```

## Usage
### IMPORTANT
There is an assumption that there would be total of 5 images named as {1,2,3,4,5}.bmp in the folder name supplied on command line. This is because the full version of software currently deployed, acquires a sequence of 5 images automatically, saves them as {1,2,3,4,5}.bmp and forwards them for computation.

1. Change to 'PhaseShiftSurface' directory in the path you have specified during compilation
2. Issue following command: 
```
./fizeau <path/to/folder/containing/phase/shifted/images>
```
