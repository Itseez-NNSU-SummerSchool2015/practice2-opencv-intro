#include "application.hpp"
#include "processing.hpp"
#include "time.h"
#include "stdlib.h"
#include <sstream>

#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

bool testfunction() { return true; }

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

int Application::processFrame(const Mat& src, Mat& dst, Processing::FilterType filter)
{
    processor.processFrame(src, dst, filter);

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
	guiState.CVT_CONVERT_GRAY_ButtonPlace = Rect(20, display.rows - 110, 120, 40);
    guiState.PIXELIZED_ButtonPlace = Rect(160, display.rows - 110, 120, 40);
	guiState.CANNY_ButtonPlace = Rect(300, display.rows - 110, 120, 40);
	/*cv::Rect CVT_CONVERT_GRAY_ButtonPlace;
        cv::Rect PIXELIZED_ButtonPlace;
		cv::Rect CANNY_ButtonPlace;*/
    rectangle(display, guiState.onButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.offButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.saveButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
	 rectangle(display, guiState.CVT_CONVERT_GRAY_ButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.PIXELIZED_ButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.CANNY_ButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);

    putText(display, "on", 
        Point(guiState.onButtonPlace.x + guiState.onButtonPlace.width / 2 - 15,
              guiState.onButtonPlace.y + guiState.onButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "off", 
        Point(guiState.offButtonPlace.x + guiState.offButtonPlace.width / 2 - 20,
              guiState.offButtonPlace.y + guiState.offButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "save", 
        Point(guiState.saveButtonPlace.x + guiState.saveButtonPlace.width / 2 - 30,
              guiState.saveButtonPlace.y + guiState.saveButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "GRAY", 
        Point(guiState.CVT_CONVERT_GRAY_ButtonPlace.x + guiState.CVT_CONVERT_GRAY_ButtonPlace.width / 2 - 35,
              guiState.CVT_CONVERT_GRAY_ButtonPlace.y + guiState.CVT_CONVERT_GRAY_ButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "PIX", 
        Point(guiState.PIXELIZED_ButtonPlace.x + guiState.PIXELIZED_ButtonPlace.width / 2 - 25,
              guiState.PIXELIZED_ButtonPlace.y + guiState.PIXELIZED_ButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "CANNY", 
        Point(guiState.CANNY_ButtonPlace.x + guiState.CANNY_ButtonPlace.width / 2 - 45,
              guiState.CANNY_ButtonPlace.y + guiState.CANNY_ButtonPlace.height / 2 + 10),
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
        processFrame(src, dst, guiState.filter);
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
	  // получить текущее время
	  // сгенерировать название изображения
	  // <image_name> - сгенерированное название изображения
	  //                с меткой текущего времени
	  // вызвать функцию сохранения imwrite(<image_name>, display)
	  // сбросить значение guiState.saveState в false
		std::stringstream ss;
		ss << "image" << time(0) << ".jpg";
		imwrite(ss.str(),display);
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
	if (onButtonClicked(elems->CVT_CONVERT_GRAY_ButtonPlace, x, y))
	{
		elems->state = Application::OnFilter;
		elems->filter=Processing::CVT_CONVERT_GRAY;
		return;
	}
	if (onButtonClicked(elems->PIXELIZED_ButtonPlace, x, y))
	{
		elems->state = Application::OnFilter;
		elems->filter=Processing::PIXELIZED;
		return;
	}
	if (onButtonClicked(elems->CANNY_ButtonPlace, x, y))
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

