#ifndef FIZEAU_H
#define FIZEAU_H

#include <opencv2/opencv.hpp>

class Fizeau
{
private:
    cv::Mat phaseMat = cv::Mat(512, 512, CV_32FC1);
    cv::Mat mask = cv::Mat(512, 512, CV_32FC1);

public:
    float piston;
    float xTilt;
    float yTilt;
    float astigmatism;
    float defocus;
    float astig45;
    float pv;
    float rms;

public:
    int displaymask();

    int resize();

    int checksize();

    int normalise();

    int wrap();

    int unwrap();

    int zernike();

private:
    int normaliseRow(std::vector<float>, std::vector<int> & , int );
    void readZernike(std::string, std::vector<double> & );
};
#endif // FIZEAU_H
