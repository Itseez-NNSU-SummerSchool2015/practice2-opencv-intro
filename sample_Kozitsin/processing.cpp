#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <stdlib.h>
#include <time.h>
#include <vector>

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

    const int kSize = 11;
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

    const int kSize = 11;
    Mat edges;
    Canny(roi, edges, 60, 120);
    vector<Mat> channels(3);

    channels[0] = edges;
    channels[1] = edges;
    channels[2] = edges;

    merge(channels, roi);

    rectangle(dst, region, Scalar(255, 0, 0));	
}
