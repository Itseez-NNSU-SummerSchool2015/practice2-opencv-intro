#include "application.hpp"
#include "processing.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <string>
#include <iostream>

using namespace cv;

Application::Application()
{
    guiState.state = WindowState::OnFilter;
}

int Application::parseArguments(int argc, const char** argv, Application::Parameters& params)
{
    if (argc < 2)
    {
        return 1;
    }
    params.imgFileName = std::string(argv[1]);
    return 0;
}

int Application::getFrame(const std::string& fileName, Mat& src)
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

void create_button(const std::string& text, const Rect& rect, Mat& display)
{
    rectangle(display, guiState.onButtonPlace, Scalar(128, 128, 128), CV_FILLED);
    putText(display, text,
        Point(rect.x + rect.width / 2 - 15,
              rect.y + rect.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, CV_COLOR(0, 0, 0), 2);  
}

int Application::drawButtons(Mat& display)
{
    guiState.onButtonPlace = Rect(20, display.rows - 60, 120, 40);
    guiState.offButtonPlace = Rect(160, display.rows - 60, 120, 40);
    guiState.saveButtonPlace = Rect(300, display.rows - 60, 120, 40);

    guiState.filterBlurButtonPlace = Rect(20, display.rows - 120, 120, 40); 
    guiState.filterCannyButtonPlace = Rect(160, display.rows - 120, 120, 40);
    guiState.filterPixelizeButtonPlace = Rect(300, display.rows - 120, 120, 40);
    guiState.filterGrayscaleButtonPlace = Rect(440, display.rows - 120, 120, 40);

    create_button("on", guiState.onButtonPlace, display);
    create_button("off", guiState.offButtonPlace, display); 
    create_button("save", guiState.saveButtonPlace, display); 
    create_button("Canny", guiState.filterCannyButtonPlace, display); 
    create_button("Blur", guiState.filterBlurButtonPlace, display); 
    create_button("Pixelize", guiState.filterPixelizeButtonPlace, display); 
    create_button("Grayscale", guiState.filterGrayscaleButtonPlace, display); 

    return 0;
}

int Application::showFrame(const std::string &caption, const Mat& src, Mat& dst)
{
    switch (guiState.state) {
    case WindowState::OffFilter:
        src.copyTo(dst);
        break;

    case WindowState::OnFilter:
        processFrame(src, dst);
        break;

    default: { throw std::exception("Unexpected window state."); }

    Mat display(src.rows, src.cols + dst.cols, src.type());
    Mat srcRoi = display(Rect(0, 0, src.cols, src.rows));
    src.copyTo(srcRoi);
    Mat dstRoi = display(Rect(src.cols, 0, dst.cols, dst.rows));
    dst.copyTo(dstRoi);       
    
    drawButtons(display);
    
    namedWindow(caption);  
    imshow(caption, display);

    buttonClickHandleArgs.dstBuf = &dst;
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
        app->guiState.state = WindowState::OnFilter;
        return;
    }
    if (isButtonClicked(app->guiState.offButtonPlace, x, y))
    {
        app->guiState.state = WindowState::OffFilter;
        return;
    }
    if (isButtonClicked(app->guiState.saveButtonPlace, x, y)) 
    {
        std::stringstream ss;
        ss << clock() << ".png";
        imwrite(ss.str(), *app->butonClickHandleArgs.dstBuf);
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

bool Application::isButtonClicked(const Rect& buttonPlace, int x, int y)
{
    if (x < buttonPlace.x || x > buttonPlace.x + buttonPlace.width ||
        y < buttonPlace.y || y > buttonPlace.y + buttonPlace.height)
    {
        return false;
    }
    return true;
}

