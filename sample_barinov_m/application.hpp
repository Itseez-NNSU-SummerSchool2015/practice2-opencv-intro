#pragma once

#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>

#include "processing.hpp"

bool onButtonClicked(cv::Rect buttonPlace, int x, int y);
void onButtonsOnOffClick(int eventId, int x, int y, int flags, void *userData);

class Application
{
 public:
    enum WindowState
    {
        OnFilter,
        OffFilter
    };
    struct Parameters
    {
        std::string imgFileName;
    };
    struct GUIElementsState
    {
        WindowState state;
        cv::Rect onButtonPlace;
        cv::Rect offButtonPlace;
    };
    int parseArguments(int argc, const char **argv, Parameters &params);
    int getFrame(const std::string &fileName, cv::Mat& src);
    int processFrame(const cv::Mat& src, cv::Mat& dst);
    int showFrame(const std::string &caption, 
                  const cv::Mat& src, cv::Mat& dst);
    friend void onButtonsOnOffClick(int eventId, int x, int y, 
                                    int flags, void *userData);
    Application() 
    { 
        guiState.state = OnFilter;
    };

 private:
    Processing processor;
    GUIElementsState guiState;

    int drawButtons(cv::Mat &display);
    
    friend bool onButtonClicked(cv::Rect buttonPlace, int x, int y);
};
