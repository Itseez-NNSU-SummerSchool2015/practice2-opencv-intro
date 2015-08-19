#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>

using std::vector;
using namespace cv;

void Processing::processFrameMedian(const cv::Mat& src, cv::Mat& dst)
{
	srand(time(NULL));

    src.copyTo(dst);

    int x = rand() % src.rows;
    int y = rand() % src.cols;

    while (x > 400 || y > 400)
    {
    	x = rand() % src.rows;
    	y = rand() % src.cols;
    }

    cv::Rect region(x, y, 100, 100);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}

void Processing::processFrameGrey(const cv::Mat& src, cv::Mat& dst)
{
	srand(time(NULL));

    src.copyTo(dst);

    int x = rand() % src.rows;
    int y = rand() % src.cols;

    while (x > 400 || y > 400)
    {
    	x = rand() % src.rows;
    	y = rand() % src.cols;
    }

    cv::Rect region(x, y, 100, 100);
    Mat roi = dst(region);

    Mat grey;
    cvtColor(roi, grey, CV_BGR2GRAY, 1);
    vector<Mat> channels(3);

    channels[0] = grey;
    channels[1] = grey;
    channels[2] = grey;

    merge(channels, roi);

    rectangle(dst, region, Scalar(255, 0, 0));
}

void Processing::processFrameCanny(const cv::Mat& src, cv::Mat& dst)
{
	srand(time(NULL));

    src.copyTo(dst);

    int x = rand() % src.rows;
    int y = rand() % src.cols;

    while (x > 400 || y > 400)
    {
    	x = rand() % src.rows;
    	y = rand() % src.cols;
    }

    cv::Rect region(x, y, 100, 100);
    Mat roi = dst(region);

    Mat edges;
    Canny(roi, edges, 60, 120);
    vector<Mat> channels(3);

    channels[0] = edges;
    channels[1] = edges;
    channels[2] = edges;

    merge(channels, roi);

    rectangle(dst, region, Scalar(255, 0, 0));	
}

void Processing::processFramePixel(const cv::Mat& src, cv::Mat& dst)
{
	srand(time(NULL));

    src.copyTo(dst);

    int x, y;
    do
    {
    	x = rand() % src.rows;
    	y = rand() % src.cols;
    }
    while (x > 400 || y > 400);

    cv::Rect region(x, y, 100, 100);

    std::cout << region.x << " " << region.y << std::endl;

    Mat roi = dst(region);

    //Mat cir = cv::Mat::zeros(roi.size(), CV_8UC1);
    int bsize = 10;

    for (int i = 0; i < roi.rows; i += bsize)
    {
        for (int j = 0; j < roi.cols; j += bsize)
        {
            cv::Rect rect = cv::Rect(j, i, bsize, bsize) & 
                            cv::Rect(0, 0, roi.cols, roi.rows);

            Mat sub_src(roi, rect);
            sub_src.setTo(cv::mean(roi(rect)));

/*
            cv::circle(
                cir, 
                cv::Point(j+bsize/2, i+bsize/2), 
                bsize/2-1, 
                CV_RGB(255,255,255), -1, CV_AA
            );
            */
        }
    }
/*
    cv::Mat cir_32f;
    cir.convertTo(cir_32f, CV_32F);
    cv::normalize(cir_32f, cir_32f, 0, 1, cv::NORM_MINMAX);

    cv::Mat dst_32f;
    roi.convertTo(dst_32f, CV_32F);

    std::vector<cv::Mat> channels;
    cv::split(dst_32f, channels);
    for (int i = 0; i < channels.size(); ++i)
        channels[i] = channels[i].mul(cir_32f);

    cv::merge(channels, dst_32f);
    dst_32f.convertTo(roi, CV_8U);
*/
    rectangle(dst, region, Scalar(255, 0, 0));
}