#ifndef dataStructures_h
#define dataStructures_h

#include <vector>
#include <opencv2/core.hpp>

/// Represents the available sensor implementation at a given point in time
struct DataFrame
{
    cv::Mat cameraImg;                   /*camera image*/
    std::vector<cv::KeyPoint> keyPoints; /*2D key points within camera image*/
    cv::Mat descriptors;                 /*key point descriptors*/
    std::vector<cv::DMatch> kptMatches;  /*key point matches between previous and current frame*/
};

#endif /* dataStructures_h */
