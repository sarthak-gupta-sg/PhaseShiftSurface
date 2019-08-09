# FizeauOpenCV

This software computes wavefront from of Fizeau interferograms and then performs analysis using Zernike Kernels. Follwing functions are implemented:
1. Phase computation (src/wrap.cpp)
2. Phase unwrapping (src/unwrap.cpp) - Based on Histogram Phase Unwrapping from OpenCV Contrib
3. Wavefront Analysis (src/zernike.cpp)

## Folder Organization
1. The bin directory contains the binaries and associated data files.
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
