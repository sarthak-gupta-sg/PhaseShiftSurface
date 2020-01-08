//std C++ Headers
#include <iostream>
#include <fstream>
#include <stdio.h>

//OpenCV Headers
#include <opencv2/opencv.hpp>
#include <opencv2/phase_unwrapping.hpp>

//Custom Headers
#include "include/spline.h"
#include "include/Util.h"
#include "include/fizeau.h"
#include "include/zernike.h"

using namespace cv;
using namespace std;

int Fizeau::displaymask()
{
    //This function prepares one image only for display purposes
    cv::Mat mask(512, 512, CV_8UC1, Scalar(0));

    for(auto i = 0; i < 512; ++i)
    {
        for(auto j = 0; j < 512; ++j)
        {
            auto a = pow(i - 256, 2);
            auto b = pow(j - 256, 2);
            if(sqrt(a + b) <= 240)
                mask.at<uchar>(i, j) = 1;
            else
                mask.at<uchar>(i, j) = 0;
        }
    }

    cv::Mat image = imread("Images/5.bmp", IMREAD_GRAYSCALE);
    cv::imwrite("Images/display.bmp", image.mul(mask));

    return 0;
}

int Fizeau::resize()
{
    Mat image[5];

    for(int i = 0; i < 5; ++i)
    {
        image[i] = imread("Images/" + std::to_string(i + 1) + ".bmp", IMREAD_GRAYSCALE);
        if(image[i].data == NULL)
        {
            std::cout << "Image Read Fail. File error detected" << std::endl;
            return -1;
        }

        long width = image[i].cols,
             height = image[i].rows;

        std::cout << "For image[" + std::to_string(i + 1) + "] original Width is :" << width << " and height is :" << height << " and channels is :" << image[i].channels() << std::endl;

        long half = width/2;
        int left = half - 256;

        long heightHalf = height/2;
        int top = heightHalf -  256;

        Rect roi(left, top, 512, 512);

        cv::Mat crop = image[i](roi);

        width = crop.cols,
        height = crop.rows;

        std::cout << "For image[" + std::to_string(i + 1) + "] modified Width is :" << width << " and height is :" << height << " and channels is :" << crop.channels() << std::endl << std::endl;

        imwrite("Images/" + std::to_string(i + 1) + ".bmp", crop);
    }

    return 0;
}

int Fizeau::checksize()
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
        rows.push_back(image[i].rows);
        cols.push_back(image[i].cols);
    }

    std::sort(rows.begin(), rows.end());
    std::sort(cols.begin(), cols.end());

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
    for(auto i = 0; i < 5; ++i)
    {
        image[i].release();
    }
    return 0;
}

using vec = vector<double>;

int Fizeau::normaliseRow(vector<float> signal, vector<int> & intensity, int flag = 1)
//signal - carries the data
//intensity & - return data of interpolated intensities
//flag - finding maxima or minima
{
    vector<float> in = signal;
    vector<int> out;

    if(flag == -1) //we are dealing with minimas
    std::for_each(in.begin(), in.end(), [](float & d) { d *= -1;});

    //Find Peaks
    findPeaks(in, out);

    cout << "Maxima found:" << endl;

    for(uint i = 0; i < out.size(); ++i)
    {
        //cout<<in[out[i]]<<" ";
        cout << out[i] << " " << in[out[i]] << endl;
    }

    //Fit Spline
    vec x(out.size());
    vec y(out.size());
    for(uint i = 0; i < out.size(); ++i)
    {
        x[i] = out[i];
        y[i] = flag * in[out[i]];
    }


    tk::spline s;
    s.set_points(x, y);


    for(auto j = 0; j < 512; ++j)
    {
        intensity.push_back( s(j) );
    }
    cout << endl;

    return 0;
}

int Fizeau::normalise()
{
    cv::Mat image[5];

    for(auto i = 0; i < 5; ++i)
    {
        image[i] = cv::imread("Images/" + std::to_string(i + 1) + ".bmp", cv::IMREAD_GRAYSCALE);
    }
    cv::Mat output(512, 512, CV_8UC1);

    //float signal[512];
    for(auto z = 0; z < 5; ++z)
    {
        for(auto m = 0; m < image[z].rows; ++m)
        {
            vector<float> signal;
            for(auto n = 0; n < image[z].cols; ++n)
            {
                signal.push_back( (float)image[z].at<uchar>(m, n) );
            }

            //Find all maximas of a row
            vector<int> intmax;
            normaliseRow(signal, intmax, 1);

            //Find all minimas of a row
            vector<int> intmin;
            normaliseRow(signal, intmin, -1);

            double norm[512];
            for(auto i = 0; i < 512; ++i)
            {
                norm[i] =  150.0 * ( signal[i] - intmin[i] ) / (intmax[i] - intmin[i] ) ;
                //cout << norm[i] << " " ;

                /*
                if(norm[i] > 255.0)
                    output.at<uchar>(m, i) = 255;
                else
                    output.at<uchar>(m, i) = (uchar)norm[i];
                */
                output.at<uchar>(m, i) = (uchar)norm[i];
                //cout << (int)output.at<uchar>(m, i) << endl;
            }

        }//for closes

        cv::imwrite("Images/" + std::to_string(z + 1) + ".bmp", output);
    }
    return 0;
}

int Fizeau::wrap()
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


int Fizeau::unwrap()
{
    phase_unwrapping::HistogramPhaseUnwrapping::Params params;
    String inputPath = "Data/wrappedData.yml";
    String outputUnwrappedName = "Data/unwrappedData";

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

    //Calculating for future application
    phaseUnwrapping->getInverseReliabilityMap(reliabilities);

    return 0;
}


void Fizeau::readZernike(string path, vector<double> & v)
{
    fstream fin;
    fin.open(path, ios::in);

    // Read the Data from the file as String Vector
    string line, word;

    float val;

    while (fin) {
        // read an entire row and store it in a string variable 'line'
        getline(fin, line);

        // used for breaking words
        stringstream s(line);

        // read every column data of a row and store it in a string variable, 'word'
        while (s >> word ) {
            //VERY IMPORTANT -- CONVERT TO FLOAT
            val = std::stod(word);
            v.push_back( val );
        }
    }
    fin.close();
    return;
}

int Fizeau::zernike()
{
    //Read Unwrapped Data from file===========================================================
    Mat uPhaseMap;
    String inputPath = "Data/unwrappedData.yml";

    FileStorage fsInput(inputPath, FileStorage::READ);
    fsInput["phaseValues"] >> uPhaseMap;
    fsInput.release();
    //=========================================================================================

    //Read all Zernike Kernels================================================================
    cv::Mat zernCoeff[6];

    /*
    std::vector<double> arr;
    for(auto num = 0; num < 6; ++num)
    {
        arr.clear();
        zernCoeff[num] = Mat(512, 512, CV_32FC1, cv::Scalar(0.0));

        //Path to Zernike Coefficients files
        std::string path = "zernike-coeff/v" + std::to_string(num + 1) + ".txt";        

        readZernike(path, arr);

        for(auto i = 0; i < 512; ++i)
            for(auto j = 0; j < 512; ++j)
                zernCoeff[num].at<float>(i, j) = arr[i * 512 + j];
    }
    */

    //Initialize Zernike Matrices
    for(auto i = 0; i < 6; ++i)
    {
        zernCoeff[i] = cv::Mat(512, 512, CV_32F, v[i]);
    }

    //Prepare Zernike in unit form
    for(auto i = 0; i < 6; ++i)
        zernCoeff[i] /= std::sqrt( zernCoeff[i].dot( zernCoeff[i] ) );

    //Check Zernike Quality
    double zernmax, zernmin;
    for(auto i = 0; i < 6; ++i)
    {
        cv::minMaxLoc(zernCoeff[i], &zernmin, &zernmax);
        cout << "For Zernike[" << (i + 1) << "] : " << endl;
        cout << "min value and max value are: "
             << zernmin << " " << zernmax << endl;
        cout << "Dot Product with self: " << zernCoeff[i].dot(zernCoeff[i]) << endl;
    }

    //=========================================================================================

    //PRINCIPAL FORMULA========================================================================
    float optical_path_diff = 0.6328f / (4 * acos(-1));

    cv::Mat heightMap = uPhaseMap * optical_path_diff ;

    double numerator[6],
           denominator[6],
           ratio[6];

    for(auto i = 0; i < 6; ++i)
    {
        numerator[i] = heightMap.dot( zernCoeff[i] );
        denominator[i] = zernCoeff[i].dot( zernCoeff[i] );
        ratio[i] = numerator[i] / denominator[i];        
    }
    //=========================================================================================

    //Prepare Mask=============================================================================
    cv::Mat mask(512, 512, CV_32FC1, cv::Scalar(0.0));

    for(auto i = 0; i < 512; ++i)
    {
        for(auto j = 0; j < 512; ++j)
        {
            auto a = pow(i - 256, 2);
            auto b = pow(j - 256, 2);
            if(sqrt(a + b) <= 240)
                mask.at<float>(i, j) = 1.0f;
            else
                mask.at<float>(i, j) = 0.0f;
        }
    }
    //=========================================================================================

    //Subtract all required abberations========================================================
    cv::Mat resultMap( 512, 512, CV_32FC1, cv::Scalar(0.0) );

    resultMap = // ( zernCoeff[0] * ratio[0] ) + 		//Piston
                // ( zernCoeff[1] * ratio[1] ) +		//X Tilt
                // ( zernCoeff[2] * ratio[2] ) +		//Y Tilt
                 ( zernCoeff[3] * ratio[3] ) +			//Astigmatism
                 ( zernCoeff[4] * ratio[4] ) +			//Defocus
                 ( zernCoeff[5] * ratio[5] )            //Astigmatism - 45 degree
            ;

    resultMap = resultMap.mul( mask );


    //Fill the OUTPUT==========================================================================
    double lambda = 0.6328;

    //OUTPUT 1. Find peak to valley
    double min, max;
    cv::minMaxLoc(resultMap, &min, &max);
    pv = (max - min) / lambda;

    //OUTPUT 2. Find RMS
    //Only for RMS calculations
    cv::Mat resultMap2( 512, 512, CV_32FC1, cv::Scalar(0.0) );
    resultMap2 = heightMap.mul( mask )
                - (  zernCoeff[0] * ratio[0] )  		//Piston
                - (  zernCoeff[1] * ratio[1] ) 			//X Tilt
                - (  zernCoeff[2] * ratio[2] )			//Y Tilt
             //   - (  zernCoeff[3] * ratio[3] ) 			//Astigmatism
             //   - (  zernCoeff[4] * ratio[4] )			//Defocus
             //   - (  zernCoeff[5] * ratio[5] )			//Astigmatism - 45 degree
                ;

    //Find RMS with formula: sqrt(X -Y^2)
    cv::Mat resMapAddSquared;
    cv::pow(resultMap2, 2, resMapAddSquared);
    double X = cv::sum(resMapAddSquared)[0] / (512 * 512);
    double Y = cv::sum(resultMap2)[0] / (512 * 512);
    rms = std::sqrt( X - std::pow(Y, 2) ) / lambda;

    //OUTPUT n. Find ALL OTHER VALUES
    //Find min and max of Zi * Ri
    double zrMin[6], zrMax[6];
    for(auto i = 0; i < 6; ++i)
    {
        cv::minMaxLoc(zernCoeff[i] * ratio[i], (zrMin + i), &zrMax[i]) ;
        //cout << "min value and max value are: " << zrMin[i] << " " << zrMax[i] << endl;
    }

    piston = (zrMax[0] - zrMin[0]) / lambda ;
    xTilt = (zrMax[1] - zrMin[1]) / lambda;
    yTilt = (zrMax[2] - zrMin[2]) / lambda;
    astigmatism = (zrMax[3] - zrMin[3]) / lambda;
    defocus = (zrMax[4] - zrMin[4]) / lambda;
    astig45 = (zrMax[5] - zrMin[5]) / lambda;

    cout << "\n(Piston) : " << piston
         << "\n(xTilt) : " << xTilt
         << "\n(yTilt) : " << yTilt
         << "\n(Astigmatism) : " << astigmatism
         << "\n(De Focus) : " << defocus
         << "\n(Astigmatism 45 degree) : " << astig45
         << endl;
    //=========================================================================================

    //Write out the resultMap as CSV - 3D Plotting=============================================

    //Pull up the negative values by bringing all 0 to below min values
    for(auto i = 0; i < resultMap.rows; ++i)
        for(auto j = 0; j < resultMap.cols; ++j)
            if(resultMap.at<float>(i, j) == 0)
                resultMap.at<float>(i, j) += min;

    std::ofstream umap;
    umap.open("Data/zmap.csv");

    for(auto i = 0; i < resultMap.rows; ++i)
    {
        for(auto j = 0; j < resultMap.cols; ++j)
        {
            umap << resultMap.at<float>(i, j) / lambda << ",";
        }
        umap << std::endl;
    }    
    umap.close();
    //=========================================================================================

    //To generate original interferogram=======================================================
    cv::Mat interMap( 512, 512, CV_32FC1, cv::Scalar(0.0) );
    interMap =  ( zernCoeff[0] * ratio[0] ) + 		//Piston
                ( zernCoeff[1] * ratio[1] ) +			//X Tilt
                ( zernCoeff[2] * ratio[2] ) +			//Y Tilt
                ( zernCoeff[3] * ratio[3] ) +			//Astigmatism
                ( zernCoeff[4] * ratio[4] ) +			//Defocus
                ( zernCoeff[5] * ratio[5] )            //Astigmatism - 45 degree
            ;
    interMap = interMap.mul( mask );

    //Write out interMap to generate the original interferogram
    umap.open("Data/interMap.csv");
    for(auto i = 0; i < interMap.rows; ++i)
    {
        for(auto j = 0; j < interMap.cols; ++j)
        {
            umap << interMap.at<float>(i, j) << ",";
        }
        umap << std::endl;
    }    
    umap.close();
    //=========================================================================================
    return 0;
}
