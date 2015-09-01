#include "processing.hpp"
#include "time.h"
#include <cmath>

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);

    cv::Rect region((int)((sin((long double)(clock() / CLOCKS_PER_SEC *3))*src.rows/4)+src.rows/4),
                    (int)((sin((long double)(clock() / CLOCKS_PER_SEC *3))*src.cols/4)+src.cols/4),
                    src.rows/2, src.cols/2);

    /*if (y % (src.cols/2) > 1)
    {
        if (x % (src.rows/2) > 1)
            x=x+1;
        else x=0;

        y=y+1;
    }
    else y=0;*/

    //cv::Rect region(src.rows/4, src.cols/4, src.rows/2, src.cols/2);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
