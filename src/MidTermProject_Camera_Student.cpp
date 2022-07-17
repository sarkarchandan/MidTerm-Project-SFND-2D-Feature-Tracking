/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"

/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{

    /* INIT VARIABLES AND DATA STRUCTURES */

    // Data location
    std::string dataPath = "../";                                    /*Base path for data*/
    std::string imgBasePath = dataPath + "images/";                  /*Derived path for camera input*/
    std::string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; /*Camera image prefix*/
    std::string imgFileType = ".png";                                /*File extension*/
    int imgStartIndex = 0;                                           /*First file index to load*/
    int imgEndIndex = 9;                                             /*Last file index to load*/
    int imgFillWidth = 4;                                            /*No. of digits which make up the file index (e.g. img-0001.png)*/

    // Data buffer properties
    int dataBufferSize = 2;            /*No. of images which are held in memory (ring buffer) at the same time*/
    std::vector<DataFrame> dataBuffer; /*List of data frames which are held in memory at the same time*/
    bool bVis = false;                 /*Boolean flag for visualizing result*/

    /* MAIN LOOP OVER ALL IMAGES */

    for (size_t imgIndex = imgStartIndex; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
    {
        /* LOAD IMAGE INTO BUFFER */

        // Assemble filenames for current index
        std::ostringstream imgNumber;
        imgNumber << std::setfill('0') << std::setw(imgFillWidth) << imgStartIndex + imgIndex;
        std::string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

        // load image from file and convert to grayscale
        cv::Mat img, imgGray;
        img = cv::imread(/*filename*/ imgFullFilename);
        cv::cvtColor(/*src*/ img, /*dst*/ imgGray, /*code*/ cv::COLOR_BGR2GRAY);

        //// STUDENT ASSIGNMENT
        //// TASK MP.1 -> replace the following code with ring buffer of size dataBufferSize

        // push image into data frame buffer
        // TODO: Implement RingBuffer
        DataFrame frame;
        frame.cameraImg = imgGray;
        dataBuffer.push_back(frame);

        //// EOF STUDENT ASSIGNMENT
        std::cout << "#1 : LOAD IMAGE INTO BUFFER done"
                  << "\n";

        /* DETECT IMAGE KEYPOINTS */

        // extract 2D keypoints from current image
        std::vector<cv::KeyPoint> keypoints; // create empty feature list for current image
        std::string detectorType = "FAST";

        //// STUDENT ASSIGNMENT
        //// TASK MP.2 -> add the following keypoint detectors in file matching2D.cpp and enable string-based selection based on detectorType
        //// -> HARRIS, FAST, BRISK, ORB, AKAZE, SIFT

        if (detectorType.compare("SHITOMASI") == 0)
        {
            detKeypointsShiTomasi(
                /*keypoints*/ keypoints,
                /*img*/ imgGray,
                /*bVis*/ false);
        }
        else if (detectorType.compare("HARRIS") == 0)
        {
            detKeypointsHarris(
                /*keypoints*/ keypoints,
                /*img*/ imgGray,
                /*bVis*/ bVis);
        }
        else
        {
            detKeypointsModern(
                /*keypoints*/ keypoints,
                /*img*/ imgGray,
                /*detectorType*/ detectorType,
                /*bVis*/ bVis);
        }
        //// EOF STUDENT ASSIGNMENT

        //// STUDENT ASSIGNMENT
        //// TASK MP.3 -> only keep keypoints on the preceding vehicle

        // only keep keypoints on the preceding vehicle
        bool bFocusOnVehicle = true;
        cv::Rect vehicleRect(535, 180, 180, 150);
        std::vector<cv::KeyPoint> keyPointsOI; /*Key points of interest*/
        if (bFocusOnVehicle)
        {
            for (auto const &kpt : keypoints)
            {
                if (
                    kpt.pt.x >= vehicleRect.x &&
                    kpt.pt.x <= vehicleRect.x + vehicleRect.width &&
                    kpt.pt.y >= vehicleRect.y &&
                    kpt.pt.y <= vehicleRect.y + vehicleRect.height)
                {
                    keyPointsOI.push_back(kpt);
                }
            }
        }
        //// EOF STUDENT ASSIGNMENT

        // optional : limit number of keypoints (helpful for debugging and learning)
        bool bLimitKpts = false;
        if (bLimitKpts)
        {
            int maxKeypoints = 50;

            if (detectorType.compare("SHITOMASI") == 0)
            {
                // There is no response info, so keep the first 50 as they are sorted
                // in descending quality order
                keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
            }
            cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
            std::cout << " NOTE: Keypoints have been limited!"
                      << "\n";
        }

        // Push key points of interest and descriptor for current frame to end of data
        // buffer
        (dataBuffer.end() - 1)->keyPoints = keyPointsOI;
        std::cout << "#2 : DETECT KEYPOINTS done"
                  << "\n";

        /* EXTRACT KEYPOINT DESCRIPTORS */

        //// STUDENT ASSIGNMENT
        //// TASK MP.4 -> add the following descriptors in file matching2D.cpp and enable string-based selection based on descriptorType
        //// -> BRIEF, ORB, FREAK, AKAZE, SIFT

        cv::Mat descriptors;
        std::string descriptorType = "ORB"; // BRIEF, ORB, FREAK, AKAZE, SIFT
        descKeypoints(
            /*keypoints*/ (dataBuffer.end() - 1)->keyPoints,
            /*img*/ (dataBuffer.end() - 1)->cameraImg,
            /*descriptors*/ descriptors,
            /*descriptorType*/ descriptorType);
        //// EOF STUDENT ASSIGNMENT

        // push descriptors for current frame to end of data buffer
        (dataBuffer.end() - 1)->descriptors = descriptors;

        std::cout << "#3 : EXTRACT DESCRIPTORS done"
                  << "\n";

        if (dataBuffer.size() > 1) // Wait until at least two images have been processed
        {
            /* MATCH KEYPOINT DESCRIPTORS */

            std::vector<cv::DMatch> matches;
            std::string matcherType = "MAT_BF";        // MAT_BF, MAT_FLANN
            std::string descriptorType = "DES_BINARY"; // DES_BINARY, DES_HOG
            std::string selectorType = "SEL_NN";       // SEL_NN, SEL_KNN

            //// STUDENT ASSIGNMENT
            //// TASK MP.5 -> add FLANN matching in file matching2D.cpp
            //// TASK MP.6 -> add KNN match selection and perform descriptor distance ratio filtering with t=0.8 in file matching2D.cpp

            matchDescriptors(
                /*kPtsSource*/ (dataBuffer.end() - 2)->keyPoints,
                /*kPtsRef*/ (dataBuffer.end() - 1)->keyPoints,
                /*descSource*/ (dataBuffer.end() - 2)->descriptors,
                /*descRef*/ (dataBuffer.end() - 1)->descriptors,
                /*matches*/ matches,
                /*descriptorType*/ descriptorType,
                /*matcherType*/ matcherType,
                /*selectorType*/ selectorType);

            //// EOF STUDENT ASSIGNMENT

            // store matches in current data frame
            (dataBuffer.end() - 1)->kptMatches = matches;

            std::cout << "#4 : MATCH KEYPOINT DESCRIPTORS done"
                      << "\n";

            // visualize matches between current and previous image
            bVis = true;
            if (bVis)
            {
                cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                cv::drawMatches(
                    /*img1*/ (dataBuffer.end() - 2)->cameraImg,
                    /*keypoints1*/ (dataBuffer.end() - 2)->keyPoints,
                    /*img2*/ (dataBuffer.end() - 1)->cameraImg,
                    /*keypoints2*/ (dataBuffer.end() - 1)->keyPoints,
                    /*matches1to2*/ matches,
                    /*outImg*/ matchImg,
                    /*matchColor*/ cv::Scalar::all(-1),
                    /*singlePointColor*/ cv::Scalar::all(-1),
                    /*matchesMask*/ std::vector<char>(),
                    /*flags*/ cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
                std::string windowName = "Matching keypoints between two camera images";
                cv::namedWindow(windowName, 7);
                cv::imshow(windowName, matchImg);
                std::cout << "Press key to continue to next image"
                          << "\n";
                cv::waitKey(0); // Wait for key to be pressed
            }
            bVis = false;
        }
    } // eof loop over all images
    return EXIT_SUCCESS;
}
