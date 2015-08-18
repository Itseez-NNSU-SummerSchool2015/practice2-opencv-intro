#include "application.hpp"
#include "processing.hpp"
#include "ctime"
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
	guiState.newButtom = Rect(300,display.rows - 60, 120,40);
	guiState.FilterMEDIAN =  Rect(450,display.rows - 60, 120,40);

	guiState.FilterCVT_CONVERT_GRAY =  Rect(600,display.rows - 60, 120,40);
	guiState.FilterPIXELIZED =  Rect(750,display.rows - 60, 120,40);
	guiState.FilterCANNY =  Rect(900 ,display.rows - 60, 120,40);

	rectangle(display, guiState.newButtom, 
		Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.onButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
    rectangle(display, guiState.offButtonPlace, 
              Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.FilterMEDIAN, 
              Scalar(128, 128, 128), CV_FILLED);


	rectangle(display, guiState.FilterCVT_CONVERT_GRAY, 
              Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.FilterPIXELIZED, 
              Scalar(128, 128, 128), CV_FILLED);
	rectangle(display, guiState.FilterCANNY, 
              Scalar(128, 128, 128), CV_FILLED);


    putText(display, "on", 
        Point(guiState.onButtonPlace.x + guiState.onButtonPlace.width / 2 - 15,
              guiState.onButtonPlace.y + guiState.onButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);
    putText(display, "off", 
        Point(guiState.offButtonPlace.x + guiState.offButtonPlace.width / 2 - 20,
              guiState.offButtonPlace.y + guiState.offButtonPlace.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);


	putText(display, "my_but", 
		Point(guiState.newButtom.x + guiState.newButtom.width / 2 - 55,
              guiState.newButtom.y + guiState.newButtom.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);


	putText(display, "median", 
		Point(guiState.FilterMEDIAN.x + guiState.FilterMEDIAN.width / 2 -50,
              guiState.FilterMEDIAN.y + guiState.FilterMEDIAN.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);

	putText(display, "gray", 
		Point(guiState.FilterCVT_CONVERT_GRAY.x + guiState.FilterCVT_CONVERT_GRAY.width / 2-45,
              guiState.FilterCVT_CONVERT_GRAY.y + guiState.FilterCVT_CONVERT_GRAY.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);

	putText(display, "pixel", 
		Point(guiState.FilterPIXELIZED.x + guiState.FilterPIXELIZED.width / 2 - 60,
              guiState.FilterPIXELIZED.y + guiState.FilterPIXELIZED.height / 2 + 10),
        FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 0), 2);

	putText(display, "canny",
		Point(guiState.FilterPIXELIZED.x + guiState.FilterPIXELIZED.width / 2  + 85,
              guiState.FilterPIXELIZED.y + guiState.FilterPIXELIZED.height / 2 + 10),
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

    if (guiState.save)
	{
		
	  // получить текущее время
		time_t rawtime;
		struct tm * timeinfo;

		time (&rawtime);
		timeinfo = localtime(&rawtime);

	
	  // сгенерировать название изображения
		string str = std::to_string(rawtime); 
		string name_img = "image_sample_ekaterina_maljutina_"  ;  
		 
	  // <image_name> - сгенерированное название изображения
	  //                с меткой текущего времени

		imwrite(name_img  +str +  ".jpg" , display);

		guiState.save = false;
	  // вызвать функцию сохранения imwrite(<image_name>, display)
	  // сбросить значение guiState.saveState в false
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
        return;
    }
    if (onButtonClicked(elems->offButtonPlace, x, y))
    {
        elems->state = Application::OffFilter;
        return;
    }
	if (onButtonClicked(elems->newButtom, x, y))
    {
		elems->save = true;
        return;
    }
	if (onButtonClicked(elems->FilterMEDIAN, x, y))
    {
		elems->state = Application::OnFilter;
		elems->filter = Processing::MEDIAN;
        return;
    }
	if (onButtonClicked(elems->FilterCVT_CONVERT_GRAY, x, y))
    {
		elems->state = Application::OnFilter;
		elems->filter = Processing::CVT_CONVERT_GRAY;
        return;
    }
	if (onButtonClicked(elems->FilterPIXELIZED, x, y))
    {
		elems->state = Application::OnFilter;
		elems->filter = Processing::PIXELIZED;
        return;
    }
	if (onButtonClicked(elems->FilterCANNY, x, y))
    {
		elems->state = Application::OnFilter;
		elems->filter = Processing::CANNY;
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

