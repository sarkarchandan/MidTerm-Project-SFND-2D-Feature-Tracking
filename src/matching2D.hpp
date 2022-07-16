#ifndef matching2D_hpp
#define matching2D_hpp

#include <stdio.h>
#include <iostream>
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

void detKeypointsHarris(
    std::vector<cv::KeyPoint> &keypoints,
    cv::Mat &img,
    bool bVis = false);

/**
 * @brief Detect key points in image using the traditional
 * Shi-Thomasi detector.
 *
 * @param keypoints Key points to be detected as output.
 * @param img Source image, where key points are to be detected.
 * @param bVis Boolean flag for visualization of key points.
 */
void detKeypointsShiTomasi(
    std::vector<cv::KeyPoint> &keypoints,
    cv::Mat &img,
    bool bVis = false);
void detKeypointsModern(
    std::vector<cv::KeyPoint> &keypoints,
    cv::Mat &img,
    std::string detectorType,
    bool bVis = false);
void descKeypoints(
    std::vector<cv::KeyPoint> &keypoints,
    cv::Mat &img,
    cv::Mat &descriptors,
    std::string descriptorType);

/**
 * @brief Finds best matches for key points in two camera images
 * based on several matching methods. Source image is the second
 * most recent and reference image is the most recent image in
 * the ring buffer.
 *
 * @param kPtsSource Detected key points from the source image.
 * @param kPtsRef Detected key points from the reference image.
 * @param descSource Key point descriptors from the source image.
 * @param descRef Key point descriptors from the reference image.
 * @param matches Set of key point matches.
 * @param descriptorType Type of the key point descriptor,one of
 * gradient based `DES_HOG` or binary `DES_BINARY`.
 * @param matcherType Type of the key point descriptor matcher
 * algorithm, one of `MAT_BF` for brute force matcher or `MAT_FLANN`
 * for kdtree based matcher.
 * @param selectorType Type of algorithm to use in order to select
 * best matching key point, one of `SEL_NN` for nearest neighbors or
 * `SEL_KNN` for k nearest neighbors.
 */
void matchDescriptors(
    std::vector<cv::KeyPoint> &kPtsSource,
    std::vector<cv::KeyPoint> &kPtsRef,
    cv::Mat &descSource,
    cv::Mat &descRef,
    std::vector<cv::DMatch> &matches,
    std::string descriptorType,
    std::string matcherType,
    std::string selectorType);

#endif /* matching2D_hpp */
