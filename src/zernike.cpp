#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/phase_unwrapping.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h> 
#include <iomanip>
 
using namespace cv;
using namespace std;

//Functions
void readZernike(std::string path, int num, std::vector<float> & v)
{ 
	
	std::cout << std::fixed;
    std::cout << std::setprecision(5);
	
	fstream fin;
	fin.open(path, ios::in); 

	// Read the Data from the file as String Vector 
	string line, word, temp; 

	float val;
	
	while (fin) { 		
		// read an entire row and store it in a string variable 'line' 
		getline(fin, line); 

		// used for breaking words 
		stringstream s(line); 

		// read every column data of a row and store it in a string variable, 'word' 		
		while (s >> word ) {			
			//VERY IMPORTANT -- CONVERT TO FLOAT
			val = std::stof(word);
			v.push_back( val );		
		}		
	} 
	return;
}
	
int zernike()
{ 
	
	String inputPath = "Data/unwrappedData.yml";
    
    FileStorage fsInput(inputPath, FileStorage::READ);
    
    
    //Declare Matrices
    Mat uPhaseMap;
    Mat zernCoeff[16];
    Mat mulPhaseMap;
    
    //Read first input from file
    fsInput["phaseValues"] >> uPhaseMap;
    fsInput.release();

	//Read all Zernike Kernels
	std::vector<float> arr;
	for(int num = 0; num < 16; ++num)
	{
		arr.clear();
		cout << num << endl;
		zernCoeff[num] = Mat(512, 512, CV_32FC1, cv::Scalar(0.0)); 
		
		//Read second input from file
		// File pointer 
		ifstream fin("zernike-coeff/u" + std::to_string(num + 1) + ".txt");
		// Open an existing file 
		std::string path = "zernike-coeff/u" + std::to_string(num + 1) + ".txt";
		cout << path << endl;

		readZernike(path, num, arr);
		
		for(auto i = 0; i < 512; ++i)
			for(auto j = 0; j < 512; ++j)
				zernCoeff[num].at<float>(i, j) = arr[i * 512 + j]; 
		
		fin.close();
	}//for closes
		
	//Do multiplication
	//float lambda = 1.0f;
	float lambda = 0.6328f / (4 * acos(-1));
	
	cv::Mat lambdaPhaseMap = uPhaseMap * lambda ;
	
	double numerator[6], denominator[6];
	float ratio[6];
	
	numerator[0] = lambdaPhaseMap.dot( zernCoeff[0] );
	numerator[1] = lambdaPhaseMap.dot( zernCoeff[1] );
	numerator[2] = lambdaPhaseMap.dot( zernCoeff[2] );
	numerator[3] = lambdaPhaseMap.dot( zernCoeff[3] );
	numerator[4] = lambdaPhaseMap.dot( zernCoeff[4] );
	numerator[5] = lambdaPhaseMap.dot( zernCoeff[5] );
	
	denominator[0] = zernCoeff[0].dot( zernCoeff[0] );
	denominator[1] = zernCoeff[1].dot( zernCoeff[1] );
	denominator[2] = zernCoeff[2].dot( zernCoeff[2] );
	denominator[3] = zernCoeff[3].dot( zernCoeff[3] );
	denominator[4] = zernCoeff[4].dot( zernCoeff[4] );
	denominator[5] = zernCoeff[5].dot( zernCoeff[5] );
	
	ratio[0] = numerator[0] / denominator[0]; 
	ratio[1] = numerator[1] / denominator[1]; 
	ratio[2] = numerator[2] / denominator[2];
	ratio[3] = numerator[3] / denominator[3]; 
	ratio[4] = numerator[4] / denominator[4]; 	 
	ratio[5] = numerator[5] / denominator[5]; 	 
	
	cout << "\nRatio[0] (Piston) : " << ratio[0] 
		 << "\nRatio[1] (X Tilt) : " << ratio[1]
		 << "\nRatio[2] (Y Tilt) : " << ratio[2]
		 << "\nRatio[3] (Astigmatism) : " << ratio[3]
		 << "\nRatio[4] (De Focus) : " << ratio[4]
		 << "\nRatio[5] (Astigmatism 45 degree) : " << ratio[5]
		 << endl;
		 
	/*
	cout << "\nnumerator[0] : " << numerator[0] 
		 << "\nnumerator[1] : " << numerator[1]
		 << "\nnumerator[2] : " << numerator[2]
		 << "\nnumerator[3] : " << numerator[3]
		 << "\nnumerator[4] : " << numerator[4]		 
		 << "\nnumerator[5] : " << numerator[5]		  
		 << endl;
		 
	cout << "\ndenominator[0] : " << denominator[0] 
		 << "\ndenominator[1] : " << denominator[1]
		 << "\ndenominator[2] : " << denominator[2]
		 << "\ndenominator[3] : " << denominator[3]
		 << "\ndenominator[4] : " << denominator[4]		 
		 << "\ndenominator[5] : " << denominator[5]		  
		 << endl;
	*/
	
	//Mask
	cv::Mat mask(512, 512, CV_32FC1, cv::Scalar(0.0));
	
	for(auto i = 0; i < 512; ++i)
	{
		for(auto j = 0; j < 512; ++j)
		{
			auto a = pow(i - 256, 2);
			auto b = pow(j - 256, 2); 
			if(sqrt(a + b) <= 256)
				mask.at<float>(i, j) = 1.0f;
			else
				mask.at<float>(i, j) = 0.0f;
		}
	}
	
	cv::Mat resultMap( 512, 512, CV_32FC1, cv::Scalar(0.0) );
		
	//Subtract all required abberations
	resultMap = lambdaPhaseMap.mul( mask ) 
				- (  zernCoeff[0] * ratio[0] )  		//Piston
				- (  zernCoeff[1] * ratio[1] ) 			//X Tilt
				- (  zernCoeff[2] * ratio[2] )			//Y Tilt
			//	- (  zernCoeff[3] * ratio[3] ) 			//Astigmatism
			//	- (  zernCoeff[4] * ratio[4] )			//Defocus
			//	- (  zernCoeff[5] * ratio[5] )			//Astigmatism - 45 degree
				;	
	
	//resultMap =   zernCoeff * ratio ;	
	//resultMap =   zernCoeff * -1.0f;	
	
	
	//Write out the resultMap as CSV - For Plotting by MatPlotLib and OriginLab (if needed)        
    std::ofstream umap;
    umap.open("Data/zmap.csv");
    
    for(auto i = 0; i < resultMap.rows; ++i)
    {
		for(auto j = 0; j < resultMap.cols; ++j)
		{
			//cout << zernCoeff.at<float>(i, j) << std::endl;
			//if ( resultMap.at<float>(i, j) < -0.1f ) resultMap.at<float>(i, j) = resultMap.at<float>(i - 1, j - 1);
			
			umap << resultMap.at<float>(i, j) << ",";
		}
		umap << std::endl;
	}
	std::cout << "* CSV file writing finished" << std::endl;
	umap.close();
	//Writing to CSV finished
	
	
	//Find peak to valley	
	double min, max;
	cv::minMaxLoc(resultMap, &min, &max);	
	float t = acos(-1);
	std::cout << "* Peak is : " << max << " and Valley is : " << min << " and PV is (in microns) " << ( max - min ) << " and PI " << t << std::endl;
	
	Mat resultMap8;
        //wPhaseMap8, 
        
    
    resultMap.convertTo(resultMap8, CV_8U, 255, 128);
    //resultMap.convertTo(resultMap8, CV_8U, 0, 255);
    //uPhaseMap.convertTo(uPhaseMap8, CV_8U, 1, 128);
    imwrite("Data/wavefront.bmp", resultMap8);
	
	return 0;
}
