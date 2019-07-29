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


int checksize()
{
	std::vector<int> rows, cols;
    
    Mat image[5];
    for(int i = 0; i < 5; ++i)
	{
		image[i] = imread("Images/" + std::to_string(i + 1) + ".bmp", IMREAD_GRAYSCALE);
		if(image[i].data == NULL)
		{
			std::cout << "Image Read Fail. Number or Type mismatch detected" << std::endl;
			return -1;
		}
		std::cout << "* For image[" + std::to_string(i + 1) + "] width is :" << image[i].cols << " and height is :" << image[i].rows << std::endl; 
		rows.push_back(image[i].rows);
		cols.push_back(image[i].cols);
	}

    sort(rows.begin(), rows.end());
    sort(cols.begin(), cols.end());
    
    auto rowVal = rows[0];
    auto colVal = cols[0];
    for(auto i = 0; i < 5; i++)
    {
		if(rowVal != rows[i] || colVal != cols[i])
		{
			std::cout << "Images size mismatch" << std::endl;
			return -2;
		}
	}
    
 	//Release the Matrices
	for(int i = 0; i < 5; ++i)
	{
		image[i].release();
	}	   
	return 0;
}
