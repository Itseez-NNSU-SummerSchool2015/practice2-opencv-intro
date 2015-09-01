#include "application.hpp"
#include "processing.hpp"
#include <time.h>

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
    guiState.medianButtonPlace = Rect(20, display.rows - 60, 120, 40);
    guiState.offButtonPlace = Rect(160, display.rows - 60, 120, 40);
	guiState.saveButtonPlace = Rect(300, display.rows - 60, 120, 40);
	guiState.pxlButtonPlace = Rect(440, display.rows - 60, 120, 40);
	guiState.greyButtonPlace = Rect(580, display.rows - 60, 120, 40);
	guiState.cannyButtonPlace = Rect(720, display.rows - 60, 120, 40);

    rectangle(display, guiState.medianButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.offButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.saveButtonPlace,
			  Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.pxlButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.greyButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.cannyButtonPlace,
			  Scalar(128, 128, 128), CV_FILLED);

    putText(display, "median", 
        Point(guiState.medianButtonPlace.x + 6,
              guiState.medianButtonPlace.y + guiState.medianButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "off", 
        Point(guiState.offButtonPlace.x + guiState.offButtonPlace.width / 2 - 20,
              guiState.offButtonPlace.y + guiState.offButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "save",
		Point(guiState.saveButtonPlace.x + guiState.saveButtonPlace.width / 2 - 30,
			  guiState.saveButtonPlace.y + guiState.saveButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "pixel",
		Point(guiState.pxlButtonPlace.x + guiState.pxlButtonPlace.width / 2 - 30,
			  guiState.pxlButtonPlace.y + guiState.pxlButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "grey",
		Point(guiState.greyButtonPlace.x + guiState.greyButtonPlace.width / 2 - 30,
			  guiState.greyButtonPlace.y + guiState.greyButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "canny",
		Point(guiState.cannyButtonPlace.x + guiState.cannyButtonPlace.width / 2 - 40,
			  guiState.cannyButtonPlace.y + guiState.cannyButtonPlace.height / 2 + 10),
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
		std::stringstream ss;
		ss << "image" << time(0) <<".jpg" ;
		imwrite(ss.str(), display);
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
    if (onButtonClicked(elems->medianButtonPlace, x, y))
    {
        elems->state = Application::OnFilter;
		elems->filter = MEDIAN;
        return;
    }
    if (onButtonClicked(elems->offButtonPlace, x, y))
    {
        elems->state = Application::OffFilter;
        return;
    }
	if (onButtonClicked(elems->saveButtonPlace, x,y))
	{
		elems->saveState = true;
		return;
	}
	if (onButtonClicked(elems->pxlButtonPlace, x, y))
    {
        elems->state = Application::OnFilter;
		elems->filter = PIXELIZED;
		return;
    }
    if (onButtonClicked(elems->greyButtonPlace, x, y))
    {
        elems->state = Application::OnFilter;
		elems->filter = CVT_CONVERT_GRAY;
        return;
    }
	if (onButtonClicked(elems->cannyButtonPlace, x,y))
	{
		elems->state = Application::OnFilter;
		elems->filter = CANNY;
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

