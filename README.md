# SFND 2D Feature Tracking - Mid Term Project Report

I would like to report on my findings on the performance of following detector and descriptor combinations. Based on that I would like to propose the top 3 detector / descriptors combinations.

I have worked on this project on my local machine with Linux, because it is convenient for me. I built OpenCV 4.6.0 from source with `OPENCV_ENABLE_NONFREE` option enabled. However, some combinations of detectors and descriptors resulted in errors for me. I have recorded those combinations with `NA` in the following spreadsheet along with combinations, which worked. I would like to request the reviewer to kindly advise me, if the following observations are reasonable. With that I would like to address the points mentioned in the evaluation rubric of the mid-term project.

1. **MP.1 Data Buffer Optimization**
As per the requirement of the project, I have implemented a templated class called `RingBuffer` in the *dataStructures.h* header file. This class wraps a vector. Apart from specifying the type, template argument includes an additional parameter called maxLength, which denotes the number of objects would be contained by the buffer in memory. In the implementation I have used the buffer size as 2. When we try to push a new objet to RingBuffer at the rear end and the buffer is already at the maxLength, it removes the object at the front to make room for the new object. As per my understanding, both push at the rear and pop from front can happen in O(1) time. RingBuffer class also overloads the subscript operator, which encapsulates a similar logic accessor logic, that the project had before ring buffer. We do this in order to keep the base implementation simple and focus on exploring the detectors and descriptor algorithms.

2. **MP.2 KeyPoint Detection**
The function `detKeyPointsModern` declared in *matching2D.hpp* and defined in *matching2D_Student.cpp* provides implementation of key point detection using FAST, AKAZE, BRISK, ORB, SIFT algorithms from OpenCV. For Harris corner detection as dedicated function `detKeyPointsHarris` has been implemented and in this implementation we use non maximal suppression taking reference from the practice exercises during lectures. For all algorithms the time required in milliseconds in order to detect key points has been logged. The performance part of this writeup derives the average time over all images from these logged times.

3. **MP.3 KeyPoint Removal**
In the file *MidTermProject_Camera_Student.cpp*, right after invoking `detKeyPointsModern` function to detect key points a rectangular area was predefined as cv::Rect object, which encapsulates the area covered by the preceding vehicle. In order to filter key points and just focus on the key points on the preceding vehicle we have taken an iterative approach. The cv::Rect object provides us with the coordinate of the top left point of the rectangular area along with the width and height. Using these information, we can infer the upper and lower bounds of both x and y coordinates. Based on these we can then check if a given key point falls inside these upper and lower bounds. We have used this semantic to filter in only those key points, which are inside the said bound in the vector `keyPointOI`. We then updated the DataFrame object for the image with only the key points of interest using this vector instead of all the detected key points.

4. **MP.4 KeyPoint Descriptors**
The function `descKeyPoints` declared in *matching2D.hpp* and defined in *matching2D_Student.cpp* instantiates the appropriate *DescriptorExtractor* object based on the provided *descriptorType*. Depending on this argument we instantiate one of the BRIEF, ORB, FREAK, AKAZE, and SIFT DescriptorExtractors to create distinctive feature vectors of the surrounding area of given key points. A common approach is taken for key points description extraction i.e., upon instantiating a given kind of *DescriptorExtractor* we use the `compute` method in order to derive the feature vectors as collection in a cv::Mat object. The time required in milliseconds is logged. Like key point detection we derive the average time required in milliseconds for each descriptor algorithm and use that to evaluate the descriptor performance later in this section.

5. **MP.5 Descriptor Matching**
The function `matchDescriptors` declared in *matching2D.hpp* and defined in *matching2D_Student.cpp* implements the logic for matching a given set of key point from an image captured in previous time step (source) and an image captured in current time step (reference). This convention is also referred as query image and train image in OpenCV literature. This function, however does not take the actual images, rather the key points and respective descriptors from the source image and reference image. Additionally it takes a *descriptorType*, *matcherType*, and *selectorType*.
    - Depending on *descriptorType* denoting a binary descriptor or gradient based one, we decide on whether to use L2_NORM or Hamming Distance to measure the distance between the feature vectors. By incorporating the XOR operation Hamming Distance can be very efficient for computing the distance for binary feature vectors, compared to L2_NORM for gradient based feature vectors.
    - Depending on *matcherType* denoting a brute-force or FLANN(using efficient kdtree approach) based matching preference we instantiate either cv::BFMatcher or FLANN-based DescriptorMatcher object.
    - Depending on *selectorType* denoting either a nearest neighbor or k-nearest neighbor matching algorithm, we either use `match` or `knnMatch` method of the DescriptorMater. The k-nearest neighbor approach incorporates some additional processing because we derive 2 best matches in a reference image for a given feature descriptor in the source image. We also needed to use a separate buffer for keeping the collection of cv::DMatch objects for this approach.

6. **MP.6 Descriptor Distance Ratio**
The use of k-nearest neighbor based selector has the added complexity of deriving the best match out of k (in our implementation k = 2) computed matches. For that we implement Lowe's distance ratio test. In the `matchDescriptor` function we have defined a pre-agreed distance ratio threshold of 0.8. Since we assumed k = 2 from beginning our iterative implementation is simpler in this regard. We iterate of each pair of best matches for the reference image and check if the first match (which is the best match) is within the defined distance ratio threshold from the second based match and include it in the final set of cv::DMatch objects if it is true.

7. **Combined (MP.7, MP.8, MP.9) - Performance Evaluation 1, 2, 3**

We start with recording number of detected key points in total and for the preceding vehicle (indicated in the following table as `PV`) using the key point detector algorithms (MP.7), which we have made use of in the implementation. We also record the time for each case.

| **Image/Detector** | **Harris** | **Shi-Tomasi** | **FAST** | **BRISK** | **ORB** | **AKAZE** | **SIFT** |
|---------|---------|---------|---------|---------|---------|---------|---------|
|image0|Total: 53, PV: 10, 17.64 ms|---------|---------|---------|---------|---------|---------|
|image1|Total: 62, PV: 13, 11.23 ms|---------|---------|---------|---------|---------|---------|
|image2|Total: 63, PV: 17, 11.94 ms|---------|---------|---------|---------|---------|---------|
|image3|Total: 69, PV: 17, 10.63 ms|---------|---------|---------|---------|---------|---------|
|image4|Total: 82, PV: 20, 10.80 ms|---------|---------|---------|---------|---------|---------|
|image5|Total: 73, PV: 18, 9.40 ms|---------|---------|---------|---------|---------|---------|
|image6|Total: 69, PV: 17, 10.03 ms|---------|---------|---------|---------|---------|---------|
|image7|Total: 108, PV: 25, 10.12 ms|---------|---------|---------|---------|---------|---------|
|image8|Total: 81, PV: 20, 9.78 ms|---------|---------|---------|---------|---------|---------|
|image9|Total: 68, PV: 19, 9.49 ms|---------|---------|---------|---------|---------|---------|




> TODO: Count the number of key points on the preceding vehicle rect precisely as well and make a separate table.
> TODO: Verify against the rubric before submission.

| **Detectors**     | **Descriptors** | **#Total KeyPoints(avg)** | **Key Point Detection Time(avg)** | **Description Extraction Time(avg)** |
|---------------|-----------------|-------------|------------|-----------|
|    Harris     |      BRIEF      |     NA      |     NA     |    NA     |
|    Harris     |      ORB        |     73      | 27.318 ms  | 1.89597 ms|
|    Harris     |      FREAK      |     NA      |     NA     |    NA     |
|    Harris     |      AKAZE      |     NA      |     NA     |    NA     |
|    Harris     |      SIFT       |     73      | 36.715 ms  | 48.713 ms |
|    Shi-Tomasi |      BRIEF      |     NA      |     NA     |    NA     |
|    Shi-Tomasi |      ORB        |    1342     | 25.743 ms  | 2.9001 ms |
|    Shi-Tomasi |      FREAK      |     NA      |     NA     |    NA     |
|    Shi-Tomasi |      AKAZE      |     NA      |     NA     |    NA     |
|    Shi-Tomasi |      SIFT       |    1342     | 30.240 ms  | 54.306 ms |
|    FAST       |      BRIEF      |     NA      |     NA     |    NA     |
|    FAST       |      ORB        |    1787     | 2.9077 ms  | 3.0288 ms |
|    FAST       |      FREAK      |     NA      |     NA     |    NA     |
|    FAST       |      AKAZE      |     NA      |     NA     |    NA     |
|    FAST       |      SIFT       |    1787     | 3.4484 ms  | 56.491 ms |
|    BRISK      |      BRIEF      |     NA      |     NA     |    NA     |
|    BRISK      |      ORB        |    2711     | 125.970 ms | 4.3090 ms |
|    BRISK      |      FREAK      |     NA      |     NA     |    NA     |
|    BRISK      |      AKAZE      |     NA      |     NA     |    NA     |
|    BRISK      |      SIFT       |    2711     | 129.169 ms | 35.420 ms |
|    ORB        |      BRIEF      |     NA      |     NA     |    NA     |
|    ORB        |      ORB        |    500      |  22.271 ms | 15.769 ms |
|    ORB        |      FREAK      |     NA      |     NA     |    NA     |
|    ORB        |      AKAZE      |     NA      |     NA     |    NA     |
|    ORB        |      SIFT       |    500      |  21.483 ms | 88.016 ms |
|    AKAZE      |      BRIEF      |     NA      |     NA     |    NA     |
|    AKAZE      |      ORB        |    1343     | 108.201 ms | 3.0905 ms |
|    AKAZE      |      FREAK      |     NA      |     NA     |    NA     |
|    AKAZE      |      AKAZE      |    1343     | 121.697 ms | 61.772 ms |
|    AKAZE      |      SIFT       |    1343     | 112.540 ms | 18.326 ms |
|    SIFT       |      BRIEF      |     NA      |     NA     |    NA     |
|    SIFT       |      ORB        |     NA      |     NA     |    NA     |
|    SIFT       |      FREAK      |     NA      |     NA     |    NA     |
|    SIFT       |      AKAZE      |     NA      |     NA     |    NA     |
|    SIFT       |      SIFT       |    1386     | 133.008 ms | 57.978 ms |

## My analysis and recommendations

First of all, in my implementation I have found, that `ORB` is the algorithm, which reliably works when combined with most of the key point detectors. Overall, I have found `ORB` to be the most efficient descriptor algorithm as well. On the detector side I have found `FAST` to be the most efficient detector. I have found in my experiments, that the `FAST` and `ORB` is the most efficient combinations of algorithms.

Firstly, I would like to list top 3 detectors in terms of efficiency, first one being the most efficient among three.
1. `FAST` (most efficient in key point detection)
2. `ORB`
3. `Shi-Tomassi` (although Harris featured slightly better time, but in terms of number of key points detected, Shi-Tomasi has beaten Harris)

Secondly, the top 3 descriptors.
1. `ORB` (most efficient in feature description)
2. `SIFT`
3. `AKAZE`

Based on this, the top 3 combinations I would like to recommend with my current implementation are the following.
1. `FAST` & `ORB` (Overall the best combination both in terms of detected key points and time required).
2. `ORB` & `ORB` 
3. `FAST` & `SIFT`

Quite surprisingly, when I used `ORB` algorithm both for key points detection and description it did not come up to be as efficient as combining `FAST` and `ORB`, which I found as the overall best combination. For all the combinations, which worked for me I have found mostly true positives along with some false positives. I have not focused on this outcome for my recommendation.

Thank you for reviewing my project. Please advise me, if I have implemented something wrongly and what I could do better to improve the observations, I recorded above.

