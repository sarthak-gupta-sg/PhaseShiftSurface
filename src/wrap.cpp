// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;


int wrap()
{
	
    Mat image[5];
    for(int i = 0; i < 5; ++i)
	{
		image[i] = imread("Images/" + std::to_string(i + 1) + ".bmp", IMREAD_GRAYSCALE);
	}
	
	long width = image[0].cols, 
		 height = image[0].rows;	

	//Calculate Phase Values
	float numerator = 0;
	float denominator = 0;
	Mat phaseValues(height, width, CV_32FC1);	//do not use double values (CV_64FC1) as that type is not matched by unwrapping portion

	for (auto i = 0; i < height; ++i)
	{
		for (auto j = 0; j < width; ++j)
		{
			numerator = ( 2 * (image[1].at<uchar>(i, j) - image[3].at<uchar>(i, j)) );
			denominator = ( 2 * image[2].at<uchar>(i, j) - image[0].at<uchar>(i, j) - image[4].at<uchar>(i, j) );
			phaseValues.at<float>(i, j) = std::atan2(numerator, denominator) + acos(-1);			
		}
	}
		
	//WRITE THE YAML FILE
	std::string filename = "Data/wrappedData.yml";
	FileStorage fs(filename, FileStorage::WRITE);
	fs << "phaseValues" << phaseValues;
	fs.release();

	//Release the Matrices
	for(int i = 0; i < 5; ++i)
	{
		image[i].release();
	}		
		
	return 0;
}
