//Driver program that calls out other modules
#include <iostream>
#include <experimental/filesystem>
#include <chrono>
#include <thread>
#include "resize.h"
#include "checksize.h"
#include "wrap.h"
#include "unwrap.h"
#include "zernike.h"

using namespace std;

int main(int argc, char **argv)
{
    system("clear");
    cout << "\t\t\tPhase wrapping and unwrapping program" << endl << endl;
            
    //Get the path of directory containing input images from the program arguments
    //Expected that the directory will contain files named {1,2,3,4,5}.bmp
    std::string path(argv[1]);
    //std::experimental::filesystem from_path = path;
    
    //Copy files to working directory
    std::cout << path << std::endl;
    try
    {
		//std::experimental::filesystem::copy(path, "Images/", std::experimental::filesystem::copy_options::overwrite_existing);
		std::experimental::filesystem::remove_all("Images/");
		std::experimental::filesystem::create_directories("Images/");
		std::experimental::filesystem::copy(path, "Images/");
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
    
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
	system("python3 scripts/zPlot.py Data/unwrappedData.csv");
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
	system("python3 scripts/zPlot.py Data/zmap.csv");
	cout << "*** End Plotting ***" << endl << endl;
	
	cout << "*** PROGRAM COMPLETES SUCCESSFULLY ***" << endl << endl;
    return 0;
}
