#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/phase_unwrapping.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace cv;
using namespace std;

static const char* keys =
{
    "{@inputPath | | Path of the wrapped phase map saved in a yaml file }"
    "{@outputUnwrappedName | | Path of the unwrapped phase map to be saved in a yaml file and as an 8 bit png}"
};

static void help()
{
    cout << "\nThis example shows how to use the \"Phase unwrapping module\" to unwrap a phase map"
            " saved in a yaml file (see extra_data\\phase_unwrapping\\data\\wrappedpeaks.yml)."
            " The mat name in the file should be \"phaseValue\". The result is saved in a yaml file"
            " too. Two images (wrapped.png and output_name.png) are also created"
            " for visualization purpose."
            "\nTo call: ./example_phase_unwrapping_unwrap <input_path> <output_unwrapped_name> \n"
         << endl;
}

//int main(int argc, char **argv)
int unwrap()
{
    
    //CommandLineParser parser(argc, argv, keys);
    //String inputPath = parser.get<String>(0);
    //String outputUnwrappedName = parser.get<String>(1);
    
    phase_unwrapping::HistogramPhaseUnwrapping::Params params;
    String inputPath = "Data/wrappedData.yml";
    String outputUnwrappedName = "Data/unwrappedData";
    
    if( inputPath.empty() || outputUnwrappedName.empty() )
    {
        help();
        return -1;
    }
    
    FileStorage fsInput(inputPath, FileStorage::READ);
    FileStorage fsOutput(outputUnwrappedName + ".yml", FileStorage::WRITE);
    
    //Declare Matrices
    Mat wPhaseMap;
    Mat uPhaseMap;    
    Mat reliabilities;
    
    fsInput["phaseValues"] >> wPhaseMap;
    fsInput.release();
    params.width = wPhaseMap.cols;
    params.height = wPhaseMap.rows;
    
    Ptr<phase_unwrapping::HistogramPhaseUnwrapping> phaseUnwrapping = phase_unwrapping::HistogramPhaseUnwrapping::create(params);
    phaseUnwrapping->unwrapPhaseMap(wPhaseMap, uPhaseMap);
    fsOutput << "phaseValues" << uPhaseMap;
    fsOutput.release();
    
    phaseUnwrapping->getInverseReliabilityMap(reliabilities);
    
    Mat uPhaseMap8, 
        wPhaseMap8, 
        reliabilities8;
    
    wPhaseMap.convertTo(wPhaseMap8, CV_8U, 255, 128);
    uPhaseMap.convertTo(uPhaseMap8, CV_8U, 1, 128);
    reliabilities.convertTo(reliabilities8, CV_8U, 255,128);
    
    /*
    imshow("reliabilities", reliabilities);
    imshow("wrapped phase map", wPhaseMap8);
    imshow("unwrapped phase map", uPhaseMap8);
	*/
    
    //Write out the uPhaseMap as CSV - For Plotting by MatPlotLib and OriginLab (if needed)
    std::ofstream umap;
    umap.open(outputUnwrappedName + ".csv");
    
    for(int i = 0; i < uPhaseMap.rows; ++i)
    {
		for(int j = 0; j < uPhaseMap.cols; ++j)
		{
			umap << uPhaseMap.at<float>(i, j) << ",";
		}
		umap << std::endl;
	}
	std::cout << "* CSV file writing finished" << std::endl;
	umap.close();
	//Writing to CSV finished
    
    std::cout << "* Unwrapped Phase (as png) file writing finished" << std::endl;
    imwrite(outputUnwrappedName + ".png", uPhaseMap8);
    
    std::cout << "* Reliabilities representation (as png) file writing finished" << std::endl;
    imwrite("Data/reliabilities.png", reliabilities8);
    
    return 0;
}
