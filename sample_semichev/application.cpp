#include "application.hpp"
#include "processing.hpp"

#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int Application::parseArguments(int argc, const char **argv, 
        Application::Parameters &params)
{
    if (argc < 2)
    {
        return 1;
    }
    params.imgFileName = std::string(argv[1]);
    return 0;
}

int Application::getFrame(const std::string &fileName, Mat& src)
{
    src = imread(fileName);
    if (src.empty())
    {
        return 1;
    }
    return 0;
}

int Application::processFrame(const Mat& src, Mat& dst)
{
    processor.processFrame(src, dst, guiState.filter);

    if (dst.empty())
    {
        return 1;
    }

    return 0;
}

int Application::drawButtons(Mat &display)
{
    guiState.onButtonPlace = Rect(20, display.rows - 60, 120, 40);
    guiState.offButtonPlace = Rect(160, display.rows - 60, 120, 40);
    guiState.saveButtonPlace = Rect(300, display.rows - 60, 120, 40);
    guiState.grayButtonPlace = Rect(440, display.rows - 60, 120, 40);
    guiState.pixelButtonPlace = Rect(580, display.rows - 60, 120, 40);
    guiState.cannyButtonPlace = Rect(720, display.rows - 60, 120, 40);

    rectangle(display, guiState.onButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.offButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.saveButtonPlace, 
              Scalar(0, 0, 128), CV_FILLED);
    rectangle(display, guiState.grayButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.pixelButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.cannyButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);

    putText(display, "on", 
        Point(guiState.onButtonPlace.x + guiState.onButtonPlace.width / 2 - 15,
              guiState.onButtonPlace.y + guiState.onButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "off", 
        Point(guiState.offButtonPlace.x + guiState.offButtonPlace.width / 2 - 20,
              guiState.offButtonPlace.y + guiState.offButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "Save", 
        Point(guiState.saveButtonPlace.x + guiState.saveButtonPlace.width / 2 - 30,
              guiState.saveButtonPlace.y + guiState.saveButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "gray", 
        Point(guiState.grayButtonPlace.x + guiState.onButtonPlace.width / 2 - 30,
              guiState.grayButtonPlace.y + guiState.onButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "pixel", 
        Point(guiState.pixelButtonPlace.x + guiState.offButtonPlace.width / 2 - 30,
              guiState.pixelButtonPlace.y + guiState.offButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "canny", 
        Point(guiState.cannyButtonPlace.x + guiState.saveButtonPlace.width / 2 - 40,
              guiState.cannyButtonPlace.y + guiState.saveButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);

    return 0;
}

int Application::showFrame(const std::string &caption, 
        const Mat& src, Mat& dst)
{
    if (guiState.state == OffFilter)
    {
        src.copyTo(dst);
    }
    else if (guiState.state == OnFilter)
    {
        processFrame(src, dst);
    }
    else
    {
        return 1;
    }

    Mat display(src.rows, src.cols + dst.cols, src.type());
    Mat srcRoi = display(Rect(0, 0, src.cols, src.rows));
    src.copyTo(srcRoi);
    Mat dstRoi = display(Rect(src.cols, 0, dst.cols, dst.rows));
    dst.copyTo(dstRoi);       
    
    if (guiState.saveState)
    {
        time_t rawTime;
        struct tm* timeInfo;
        time(&rawTime);
        timeInfo = localtime(&rawTime);
        std::ostringstream oss;
        oss << rawTime;
        string str = oss.str();
        imwrite("picture_" + str + ".png", display);
        guiState.saveState = false;
    }

    drawButtons(display);
    
    namedWindow(caption);  
    imshow(caption, display);
    setMouseCallback(caption, onButtonsOnOffClick, &guiState);
    char key = waitKey(1);

    return key;
}

void onButtonsOnOffClick(int eventId, int x, int y, int flags, void *userData)
{
    if (eventId != EVENT_LBUTTONDOWN)
    {
        return;
    }
    Application::GUIElementsState *elems = 
        (Application::GUIElementsState *)userData;
    if (onButtonClicked(elems->onButtonPlace, x, y))
    {
        elems->state = Application::OnFilter;
        elems->filter=Processing::MEDIAN;
        return;
    }
    if (onButtonClicked(elems->offButtonPlace, x, y))
    {
        elems->state = Application::OffFilter;
        return;
    }
    if (onButtonClicked(elems->saveButtonPlace, x, y))
    {
        elems->saveState = true;
        return;
    }
    if (onButtonClicked(elems->grayButtonPlace, x, y))
    {
        elems->state = Application::OnFilter;
        elems->filter=Processing::CVT_CONVERT_GRAY;
        return;
    }
    if (onButtonClicked(elems->pixelButtonPlace, x, y))
    {
        elems->state = Application::OnFilter;
        elems->filter=Processing::PIXELIZED;
        return;
    }
    if (onButtonClicked(elems->cannyButtonPlace, x, y))
    {
        elems->state = Application::OnFilter;
        elems->filter=Processing::CANNY;
        return;
    }
}

bool onButtonClicked(cv::Rect buttonPlace, int x, int y)
{
    if (x < buttonPlace.x || x > buttonPlace.x + buttonPlace.width ||
        y < buttonPlace.y || y > buttonPlace.y + buttonPlace.height)
    {
        return false;
    }
    return true;
}

