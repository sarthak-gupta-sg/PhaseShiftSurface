/*
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
*/
#include <iostream>
#include <chrono>
#include <thread>
#include "serial.h"
#include "grabimage.h"
#include "resize.h"
#include "checksize.h"
#include "wrap.h"
#include "unwrap.h"
#include "zernike.h"

using namespace std;
//using namespace cv;

int main(int argc, char **argv)
{
    system("clear");
    cout << "\t\t\tPhase wrapping and unwrapping program" << endl << endl;
            
    //Resize 5 images
    cout << "*** Begin Image Resize ***" << endl;
    if(resize() != 0)
    {
		cout << "Image Resize Failed" << endl;
		return -1;
	}
    cout << "*** End Image Resize ***" << endl << endl << endl << endl;
     
    
    //Check whether all files are of same dimensions else flag error
    cout << "*** Begin Input Checking ***" << endl;
	if(checksize() != 0)
	{
		cout << "Image Size Check Failed" << endl;
		return -1;
	}
    cout << "*** End Input Checking ***" << endl << endl << endl << endl;
    
    
    //Perform wrapping
    cout << "*** Begin Phase wrapping ***" << endl;
    if(wrap() != 0)
    {
		cout << "Wrapping Failed" << endl;
		return -1;
	}
	
	//Generate Test Profile for wrapped phases
	
	//char sawFile[] = "matplot.py";
	//FILE* fp = _Py_fopen(sawFile, "r");
	//Py_Initialize();
	//PyRun_SimpleFile(fp, sawFile);
	//Py_Finalize();
	
	//system("python3 scripts/matplot.py");
	cout << "*** End Phase wrapping ***" << endl << endl << endl << endl;
	
	
	//Perform unwrapping
    cout << "*** Begin Phase Unwrapping ***" << endl;
    if(unwrap() != 0)
    {
		cout << "UnWrapping Failed" << endl;
		return -1;
	}
	cout << "*** End Phase Unwrapping ***" << endl << endl << endl << endl;
	
	
	//Generate Profile
	cout << "*** Begin Profile Plotting ***" << endl;
	system("python3 scripts/plot3d.py");
	cout << "*** End Plotting ***" << endl << endl;
	
	
	//Perform zernike
    cout << "*** Begin Zernike Processing ***" << endl;
    if(zernike() != 0)
    {
		cout << "Zernike Processing Failed" << endl;
		return -1;
	}
	cout << "*** End Zernike Processing ***" << endl << endl << endl << endl;
	
	//Generate Profile
	cout << "*** Begin Profile Plotting ***" << endl;
	system("python3 scripts/zernike_plot.py Data/zmap.csv");
	cout << "*** End Plotting ***" << endl << endl;
	
	cout << "*** PROGRAM COMPLETES SUCCESSFULLY ***" << endl << endl;
    return 0;
}
