// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;


int resize()
{
	
	Mat image[5];

	for(int i = 0; i < 5; ++i)
	{
		image[i] = imread("Images/" + std::to_string(i + 1) + ".bmp", IMREAD_GRAYSCALE);

		long width = image[i].cols, 
			 height = image[i].rows;
	
		std::cout << "For image[" + std::to_string(i + 1) + "] original Width is :" << width << " and height is :" << height << " and channels is :" << image[i].channels() << std::endl; 

		long half = width/2;
		int left = half - 256,
		    right = half + 256;
		    
		long heightHalf = height/2;
		int top = heightHalf -  256,
		    bottom =  heightHalf + 256;
		    
		Rect roi(left, top, 512, 512);
		

		cv::Mat crop = image[i](roi);
		//cv::imshow("crop", crop);
		//cv::waitKey(0);
		width = crop.cols, 
		height = crop.rows;
	
		std::cout << "For image[" + std::to_string(i + 1) + "] modified Width is :" << width << " and height is :" << height << " and channels is :" << crop.channels() << std::endl << std::endl; 
		imwrite("Images/" + std::to_string(i + 1) + ".bmp", crop);
	}
	return 0;
}
