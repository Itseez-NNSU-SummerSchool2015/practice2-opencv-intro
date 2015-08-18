#include <opencv2/core/core.hpp>
#include <iostream>

#include "application.hpp"

using namespace std;
using namespace cv;

enum ErrorCode {
    OK,
    WRONG_ARGUMENTS,
    WRONG_INPUT,
    CANT_PROCESS
};

int main(int argc, const char **argv)
{
    Application app;
    Application::Parameters params;

    if (app.parseArguments(argc, argv, params) != 0)
    {
        cout << "sample_template <image_name>" << endl;
        cout << "<image_name> - image name for filtering" << endl;
        return WRONG_ARGUMENTS;
    }

    Mat src;
    if (app.getFrame(params.imgFileName, src) != 0)
    {
        cout << "Error: \'src\' image is null or empty!" << endl;
        return WRONG_INPUT;
    }

    const std::string caption = "OpenCV Sample";
    char key = 0;
    Mat dst(src.rows, src.cols, src.type());
    while (key != 27) // Esc
    {
        key = app.showFrame(caption, src, dst);
    }

	return OK;
}
