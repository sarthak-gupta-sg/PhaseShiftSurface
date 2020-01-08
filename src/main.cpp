//std C++ headers
#include <experimental/filesystem> //for file operations
#include <iomanip>   //for hex setw put_time etc.
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

//C headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//MyHeaders
#include "fizeau.h"

//OpenCV
#include <opencv2/opencv.hpp>


using namespace std;


struct node
{
    //basic setup variables
    int step_size;
    int step;
    int num_of_images;
    int milliseconds;
    int direction;
    float calibration_factor;
    string cmd;
    int mode;
};

struct node init;

/*
int acquireImage()
{
    system("python3 scripts/Program.py");
    std::this_thread::sleep_for(std::chrono::milliseconds(5)); //1 second margin to load drivers by OS
    return 0;
}
*/

void save()
{
    cout << "*** Begin Data Save ***" << endl;

    //Save Data
    try
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
        auto path = "Default-" + oss.str();

        //std::experimental::filesystem::copy(path, "Images/", std::experimental::filesystem::copy_options::overwrite_existing);
        //std::experimental::filesystem::remove_all("Images/");
        std::experimental::filesystem::create_directories(path);
        std::experimental::filesystem::copy("Images/", path );
        std::experimental::filesystem::copy("Data/", path );

        //necessary for underlying OS file operations to complete
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;        
    }

    cout << "*** End Data Save ***" << endl;
    return;
}


int driver()
{
    Fizeau output;
    //STEP 2: Resize 5 images ==============================================
    if(output.resize() != 0)
    {
        cout << "Image Resize Failed." << endl;        
        return -1;
    }
    cout << "*** Image Resize Completed ***" << endl;
    //======================================================================

    //STEP 3: Check whether all files are of same dimensions ===============
    if(output.checksize() != 0)
    {
        cout << "Image Check Size Failed." << endl;
        return -1;
    }
    cout << "*** Image Checking Completed ***" << endl;
    //======================================================================

    //STEP 4: Prepare an Image solely for disply ===========================
    if(output.displaymask() != 0)
    {
        cout << "Display Mask generation Failed." << endl;
        return -1;
    }
    cout << "*** Display Mask Generation Completed ***" << endl;
    //======================================================================
/*
    //Perform Fringe Normalization
    if(output.normalise() != 0)
    {
        msgBox.setText("Image Normalization Failed.");
        msgBox.exec();
        return -1;
    }
    cout << "*** End Normalization ***" << endl << endl << endl << endl;
    //======================================================================
*/

    //STEP 5: Perform wrapping =============================================
    if(output.wrap() != 0)
    {
        cout << "Phase Wrapping Failed." << endl;
        return -1;
    }
    cout << "*** Phase Wrappping Completed ***" << endl;
    //======================================================================


    //STEP 6: Perform unwrapping ===========================================
    if(output.unwrap() != 0)
    {
        cout << "Phase Unwrapping Failed." << endl;
        return -1;
    }
    cout << "*** Phase Unwrapping Completed ***" << endl;
    //======================================================================

    //STEP 7: Perform zernike ==============================================
    if(output.zernike() != 0)
    {
        cout << "Zernike Processing Failed." << endl;
        return -1;
    }
    cout << "*** Zernike Processing Completed ***" << endl;
    //======================================================================

    //Generate Profile
    system("python3 scripts/Plots.py 1");
    system("python3 scripts/Plots.py 2");
    system("python3 scripts/Plots.py 3");
    system("python3 scripts/Plots.py 4 true");
    
    return 0;
}

int main(int argc, char ** argv)
{
    system("clear");

    //Load Drivers for Serial Communication
    cout << "Loading serial port drivers" << endl << endl;
    system("sudo modprobe pl2303");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //1 second margin to load drivers by OS

    //Destroy all old data
    std::experimental::filesystem::remove_all("Data");
    std::experimental::filesystem::create_directories("Data");
    std::experimental::filesystem::create_directories("Images");
    
    //Copy images from selected directory to 'Images' directory
    std::experimental::filesystem::remove_all("Images");
    std::experimental::filesystem::create_directories("Images");
    std::experimental::filesystem::copy( argv[1], "Images/");
    
    //main class driver
    driver();
    
    //save results
    save();


    return 0;
}
