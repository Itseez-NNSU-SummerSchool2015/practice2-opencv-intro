#include "application.hpp"
#include "processing.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <string>

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
    processor.processFrame(src, dst);

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

	guiState.filterBlurButtonPlace = Rect(20, display.rows - 120, 120, 40); 
	guiState.filterCannyButtonPlace = Rect(160, display.rows - 120, 120, 40);
	guiState.filterPixelizeButtonPlace = Rect(300, display.rows - 120, 120, 40);
	guiState.filterGrayscaleButtonPlace = Rect(440, display.rows - 120, 120, 40);

    rectangle(display, guiState.onButtonPlace, Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.offButtonPlace, Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.saveButtonPlace, Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.filterBlurButtonPlace, Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.filterCannyButtonPlace, Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.filterPixelizeButtonPlace, Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.filterGrayscaleButtonPlace, Scalar(128, 128, 128), CV_FILLED);

    putText(display, "on", 
        Point(guiState.onButtonPlace.x + guiState.onButtonPlace.width / 2 - 15,
              guiState.onButtonPlace.y + guiState.onButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "off", 
        Point(guiState.offButtonPlace.x + guiState.offButtonPlace.width / 2 - 20,
              guiState.offButtonPlace.y + guiState.offButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "save", 
        Point(guiState.saveButtonPlace.x + guiState.saveButtonPlace.width / 2 - 20,
              guiState.saveButtonPlace.y + guiState.saveButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "Canny", 
        Point(guiState.filterCannyButtonPlace.x + guiState.filterCannyButtonPlace.width / 2 - 20,
              guiState.filterCannyButtonPlace.y + guiState.filterCannyButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "Blur", 
        Point(guiState.filterBlurButtonPlace.x + guiState.filterBlurButtonPlace.width / 2 - 20,
              guiState.filterBlurButtonPlace.y + guiState.filterBlurButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "Pixelize", 
        Point(guiState.filterPixelizeButtonPlace.x + guiState.filterPixelizeButtonPlace.width / 2 - 20,
              guiState.filterPixelizeButtonPlace.y + guiState.filterPixelizeButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
	putText(display, "Grayscale", 
        Point(guiState.filterGrayscaleButtonPlace.x + guiState.filterGrayscaleButtonPlace.width / 2 - 20,
              guiState.filterGrayscaleButtonPlace.y + guiState.filterGrayscaleButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);

    return 0;
}

int Application::showFrame(const std::string &caption, const Mat& src, Mat& dst)
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
    
    drawButtons(display);
    
    namedWindow(caption);  
    imshow(caption, display);

	butonClickHandleArgs.dstBuf = &dst;
	setMouseCallback(caption, onButtonClick, this);

    char key = waitKey(1);

    return key;
}

void Application::onButtonClick(int eventId, int x, int y, int flsgs, void *userData) 
{
	if (eventId != EVENT_LBUTTONDOWN) 
	{
		return;
	}
	Application* app = static_cast<Application*>(userData);
	
	if (isButtonClicked(app->guiState.onButtonPlace, x, y))
    {
        app->guiState.state = Application::OnFilter;
        return;
    }
    if (isButtonClicked(app->guiState.offButtonPlace, x, y))
    {
        app->guiState.state = Application::OffFilter;
        return;
    }
	if (isButtonClicked(app->guiState.saveButtonPlace, x, y)) 
	{
		imwrite(std::to_string((long long)clock()) + ".png", *app->butonClickHandleArgs.dstBuf);
		return;
	}
	if (isButtonClicked(app->guiState.filterBlurButtonPlace, x, y)) 
	{
		app->processor.setFilterType(FilterType::Blur);
		return;
	}
	if (isButtonClicked(app->guiState.filterCannyButtonPlace, x, y)) 
	{
		app->processor.setFilterType(FilterType::Canny);
		return;
	}
	if (isButtonClicked(app->guiState.filterGrayscaleButtonPlace, x, y)) 
	{
		app->processor.setFilterType(FilterType::Grayscale);
		return;
	}
	if (isButtonClicked(app->guiState.filterPixelizeButtonPlace, x, y)) 
	{
		app->processor.setFilterType(FilterType::Pixelize);
		return;
	}
}

bool Application::isButtonClicked(cv::Rect buttonPlace, int x, int y)
{
    if (x < buttonPlace.x || x > buttonPlace.x + buttonPlace.width ||
        y < buttonPlace.y || y > buttonPlace.y + buttonPlace.height)
    {
        return false;
    }
    return true;
}

