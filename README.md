# FizeauOpenCV

This software computes wavefront from of Fizeau interferograms and then performs analysis using Zernike Kernels. Follwing functions are implemented:
1. Phase computation (src/wrap.cpp)
2. Phase unwrapping (src/unwrap.cpp) - Based on Histogram Phase Unwrapping from OpenCV Contrib
3. Wavefront Analysis (src/zernike.cpp)

## Details
1. The bin directory contains the binaries and associated data files.

## Usage
./driver.out <path/to/folder/containing/phase/shifted/images>

