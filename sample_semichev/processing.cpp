#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
int move = 0;
bool isMovingRight = true;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);
    if (isMovingRight)
        if (move >= 50)
            isMovingRight = false;
        else
        {
            cv::Rect region(src.rows/4 + move, src.cols/4, src.rows/2 + 10, src.cols/2 + 100);
            move++;
                Mat roi = dst(region);

            const int kSize = 11;
            medianBlur(roi, roi, kSize);

            rectangle(dst, region, Scalar(255, 0, 0));
        }
    else
        if (move <= -50)
            isMovingRight = true;
        else
        {
            cv::Rect region(src.rows/4 + move, src.cols/4, src.rows/2 - 10, src.cols/2 - 100);
            move--;
                Mat roi = dst(region);

            const int kSize = 11;
            medianBlur(roi, roi, kSize);

            rectangle(dst, region, Scalar(255, 0, 0));
        }
}
