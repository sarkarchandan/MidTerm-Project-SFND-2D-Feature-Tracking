#include <numeric>

#include <opencv2/highgui.hpp>

#include "matching2D.hpp"

void detKeypointsHarris(
    std::vector<cv::KeyPoint> &keypoints,
    cv::Mat &img,
    bool bVis)
{
    int blockSize = 4;     /*Size of neighborhood for computing covariance matrix of intensity gradients*/
    int kernelSize = 3;    /*Kernel size for computing Sobel gradient*/
    int minResponse = 100; /*Minimum gradient response*/
    double k = 0.04;       /*Constant k applied to Harris and Shi-Tomasi corner detection*/
    // Apply corner detection
    double t = static_cast<double>(cv::getTickCount());
    cv::Mat corners = cv::Mat::zeros(/*size*/ img.size(), /*type*/ CV_32FC1);
    cv::Mat dstNorm, dstNormScaled;
    cv::cornerHarris(
        /*src*/ img,
        /*dst*/ corners,
        /*blockSize*/ blockSize,
        /*ksize*/ kernelSize,
        /*k*/ k,
        /*borderType*/ cv::BORDER_DEFAULT);
    cv::normalize(
        /*src*/ corners,
        /*dst*/ dstNorm,
        /*alpha*/ 0,
        /*beta*/ 255,
        /*norm_type*/ cv::NORM_MINMAX,
        /*dtype*/ CV_32FC1,
        /*mask*/ cv::Mat());
    cv::convertScaleAbs(/*src*/ dstNorm, /*dst*/ dstNormScaled);
    // Apply non-maximal suppression and convert matrix to a vector of key points
    double maxOverlap = 0.; /*Maximum permissible overlap between two detected features*/
    for (uint r = 0; r < dstNorm.rows; r++)
    {
        for (uint c = 0; c < dstNorm.cols; c++)
        {
            int response = static_cast<int>(dstNorm.at<float>(r, c));
            if (response > minResponse)
            {
                cv::KeyPoint keyPoint;
                keyPoint.pt = cv::Point2f(/*_x*/ c, /*_y*/ r);
                keyPoint.size = 2 * kernelSize;
                keyPoint.response = response;
                bool flagOverlap = false;
                for (auto it = keypoints.begin(); it != keypoints.end(); it++)
                {
                    double overlap = cv::KeyPoint::overlap(/*kp1*/ keyPoint, /*kp2*/ *it);
                    if (overlap > maxOverlap)
                    {
                        flagOverlap = true;
                        if (keyPoint.response > it->response)
                        {
                            *it = keyPoint;
                            break;
                        }
                    }
                }
                if (flagOverlap == false)
                {
                    keypoints.push_back(keyPoint);
                }
            }
        }
    }
    t = (static_cast<double>(cv::getTickCount()) - t) / cv::getTickFrequency();
    std::cout << "Harris detection with n = "
              << keypoints.size()
              << " keypoints in " << 1000 * t << " ms"
              << "\n";
    // Visualize results is needed
    if (bVis)
    {
        cv::Mat visImage = img.clone();
        cv::drawKeypoints(
            /*image*/ img,
            /*keypoints*/ keypoints,
            /*outImage*/ visImage,
            /*color*/ cv::Scalar::all(-1),
            /*flags*/ cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        std::string windowName = "Harris Corner Detector Results";
        cv::namedWindow(windowName, cv::WINDOW_FULLSCREEN);
        cv::imshow(windowName, visImage);
        cv::waitKey(0);
    }
}

void detKeypointsShiTomasi(
    std::vector<cv::KeyPoint> &keypoints,
    cv::Mat &img,
    bool bVis)
{
    // Compute detector parameters based on image size
    int blockSize = 4;                                                 /*Size of neighborhood for computing covariance matrix of intensity gradients*/
    double maxOverlap = 0.0;                                           /*Maximum permissible overlap between two detected features*/
    double minDistance = (1.0 - maxOverlap) * blockSize;               /*Minimum distance between two detected features*/
    int maxCorners = img.rows * img.cols / std::max(1.0, minDistance); /*Maximum number of features to be detected*/
    double qualityLevel = 0.01;                                        /*Minimum accepted quality of detected features, related to the eigen values of covariance matrix*/
    double k = 0.04;                                                   /*Constant k applied to Harris and Shi-Tomasi corner detection*/
    // Apply corner detection
    double t = static_cast<double>(cv::getTickCount());
    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(
        /*image*/ img,
        /*corners*/ corners,
        /*maxCorners*/ maxCorners,
        /*qualityLevel*/ qualityLevel,
        /*minDistance*/ minDistance,
        /*mask*/ cv::Mat(),
        /*blockSize*/ blockSize,
        /*useHarrisDetector*/ false,
        /*k*/ k);
    // Add corners to result vector
    for (auto it = corners.begin(); it != corners.end(); ++it)
    {
        cv::KeyPoint newKeyPoint;
        newKeyPoint.pt = cv::Point2f((*it).x, (*it).y);
        newKeyPoint.size = blockSize;
        keypoints.push_back(newKeyPoint);
    }
    t = (static_cast<double>(cv::getTickCount()) - t) / cv::getTickFrequency();
    std::cout << "Shi-Tomasi detection with n = "
              << keypoints.size()
              << " keypoints in " << 1000 * t << " ms"
              << "\n";
    // Visualize results is needed
    if (bVis)
    {
        cv::Mat visImage = img.clone();
        cv::drawKeypoints(
            /*image*/ img,
            /*keypoints*/ keypoints,
            /*outImage*/ visImage,
            /*color*/ cv::Scalar::all(-1),
            /*flags*/ cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        std::string windowName = "Shi-Tomasi Corner Detector Results";
        cv::namedWindow(windowName, cv::WINDOW_FULLSCREEN);
        cv::imshow(windowName, visImage);
        cv::waitKey(0);
    }
}

void detKeypointsModern(
    std::vector<cv::KeyPoint> &keypoints,
    cv::Mat &img,
    std::string detectorType,
    bool bVis)
{
    double t = static_cast<double>(cv::getTickCount());
    if (detectorType.compare("FAST") == 0)
    {
        cv::FastFeatureDetector::DetectorType type = cv::FastFeatureDetector::TYPE_9_16;
        cv::Ptr<cv::FastFeatureDetector> detector = cv::FastFeatureDetector::create(
            /*threshold*/ 30,
            /*nonMaximalSuppression*/ true,
            /*type*/ type);
        detector->detect(/*images*/ img, /*keypoints*/ keypoints);
        t = (static_cast<double>(cv::getTickCount()) - t) / cv::getTickFrequency();
        std::cout << "FAST detection with n = "
                  << keypoints.size()
                  << " keypoints in " << 1000 * t << " ms"
                  << "\n";
        return;
    }
    else if (detectorType.compare("AKAZE") == 0)
    {
        cv::Ptr<cv::FeatureDetector> detector = cv::AKAZE::create();
        detector->detect(/*images*/ img, /*keypoints*/ keypoints);
        t = (static_cast<double>(cv::getTickCount()) - t) / cv::getTickFrequency();
        std::cout << "AKAZE detection with n = "
                  << keypoints.size()
                  << " keypoints in " << 1000 * t << " ms"
                  << "\n";
        return;
    }
    else if (detectorType.compare("BRISK") == 0)
    {
        cv::Ptr<cv::FeatureDetector> detector = cv::BRISK::create();
        detector->detect(/*images*/ img, /*keypoints*/ keypoints);
        t = (static_cast<double>(cv::getTickCount()) - t) / cv::getTickFrequency();
        std::cout << "BRISK detection with n = "
                  << keypoints.size()
                  << " keypoints in " << 1000 * t << " ms"
                  << "\n";
        return;
    }
    else if (detectorType.compare("ORB") == 0)
    {
        cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
        detector->detect(/*images*/ img, /*keypoints*/ keypoints);
        t = (static_cast<double>(cv::getTickCount()) - t) / cv::getTickFrequency();
        std::cout << "ORB detection with n = "
                  << keypoints.size()
                  << " keypoints in " << 1000 * t << " ms"
                  << "\n";
        return;
    }
    else if (detectorType.compare("SIFT") == 0)
    {
        cv::Ptr<cv::FeatureDetector> detector = cv::SIFT::create();
        detector->detect(/*images*/ img, /*keypoints*/ keypoints);
        t = (static_cast<double>(cv::getTickCount()) - t) / cv::getTickFrequency();
        std::cout << "SIFT detection with n = "
                  << keypoints.size()
                  << " keypoints in " << 1000 * t << " ms"
                  << "\n";
        return;
    }
    else
    {
        std::stringstream ss;
        ss << detectorType << " feature detector not implemented";
        throw std::runtime_error(ss.str());
    }
}

void descKeypoints(
    std::vector<cv::KeyPoint> &keypoints,
    cv::Mat &img,
    cv::Mat &descriptors,
    std::string descriptorType)
{
    // BRIEF, ORB, FREAK, AKAZE, SIFT
    // select appropriate descriptor
    cv::Ptr<cv::DescriptorExtractor> extractor;
    if (descriptorType.compare("BRISK") == 0)
    {
        int threshold = 30;        /*FAST/AGAST detection threshold score*/
        int octaves = 3;           /*Detection octaves (use 0 to do single scale)*/
        float patternScale = 1.0f; /*Apply this scale to the pattern used for sampling the neighborhood of a key point.*/
        extractor = cv::BRISK::create(threshold, octaves, patternScale);
    }
    else if (descriptorType.compare("BRIEF") == 0)
    {
        cv::xfeatures2d::BriefDescriptorExtractor::create();
    }
    else if (descriptorType.compare("ORB") == 0)
    {
        extractor = cv::ORB::create();
    }
    else if (descriptorType.compare("FREAK") == 0)
    {
        cv::xfeatures2d::FREAK::create();
    }
    else if (descriptorType.compare("AKAZE") == 0)
    {
        extractor = cv::AKAZE::create();
    }
    else if (descriptorType.compare("SIFT") == 0)
    {
        extractor = cv::SiftDescriptorExtractor::create();
    }
    else
    {
        std::stringstream ss;
        ss << descriptorType << " key point descriptor not implemented";
        throw std::runtime_error(ss.str());
    }
    // perform feature description
    double t = static_cast<double>(cv::getTickCount());
    extractor->compute(
        /*image*/ img,
        /*keypoints*/ keypoints,
        /*descriptors*/ descriptors);
    t = (static_cast<double>(cv::getTickCount()) - t) / cv::getTickFrequency();
    std::cout << descriptorType
              << " descriptor extraction in "
              << 1000 * t
              << " ms"
              << "\n";
}

void matchDescriptors(
    std::vector<cv::KeyPoint> &kPtsSource,
    std::vector<cv::KeyPoint> &kPtsRef,
    cv::Mat &descSource, cv::Mat &descRef,
    std::vector<cv::DMatch> &matches,
    std::string descriptorType,
    std::string matcherType,
    std::string selectorType)
{
    // Configure matcher
    bool crossCheck = false;
    cv::Ptr<cv::DescriptorMatcher> matcher;

    if (matcherType.compare("MAT_BF") == 0)
    {
        int normType = descriptorType.compare("DES_BINARY") == 0
                           ? cv::NORM_HAMMING
                           : cv::NORM_L2;
        matcher = cv::BFMatcher::create(/*normType*/ normType, /*crossCheck*/ crossCheck);
    }
    else if (matcherType.compare("MAT_FLANN") == 0)
    {
        // Due to a potential bug in OpenCV FLANN based matching implementation we
        // safeguard by converting the descriptors to floating point
        if (descSource.type() != CV_32F)
        {
            descSource.convertTo(/*m*/ descSource, /*rtype*/ CV_32F);
            descRef.convertTo(/*m*/ descRef, /*rtype*/ CV_32F);
        }
        matcher = cv::DescriptorMatcher::create(/*matcherType*/ cv::DescriptorMatcher::FLANNBASED);
    }
    // Perform matching task
    if (selectorType.compare("SEL_NN") == 0)
    {
        // Nearest neighbor (best match).
        // Finds the best match for each descriptor in desc1
        matcher->match(
            /*queryDescriptor*/ descSource,
            /*trainDescriptor*/ descRef,
            /*matches*/ matches);
    }
    else if (selectorType.compare("SEL_KNN") == 0)
    {
        std::vector<std::vector<cv::DMatch>> knnMatches;
        // k nearest neighbors (k=2)
        matcher->knnMatch(
            /*queryDescriptors*/ descSource,
            /*trainDescriptors*/ descRef,
            /*matches*/ knnMatches,
            /*k*/ 2);
        // Perform Lowe's distance ratio test
        const float distanceRatioThreshold = 0.8f;
        for (std::vector<cv::DMatch> const &vec : knnMatches)
        {
            // If the first descriptor's matching distance is less than the
            // second one by the distance ratio threshold, we consider the first
            // match.
            if (vec[0].distance < distanceRatioThreshold * vec[1].distance)
            {
                matches.push_back(vec[0]);
            }
        }
    }
}