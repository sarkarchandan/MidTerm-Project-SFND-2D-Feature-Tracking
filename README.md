# SFND 2D Feature Tracking - Mid Term Project Report

I would like to report on my findings on the performance of the detector and descriptor combinations. Based on that I would like to propose the top 3 detector + descriptors.

I have worked on this project on my local machine with Linux, because it is convenient for me. I built OpenCV 4.6.0 from source with `OPENCV_ENABLE_NONFREE` option enabled. However, some combinations of detectors and descriptors resulted in errors for me. In order to double check I copied my implementation in the Udacity workspace and came with the same result as recorded in the spreadsheet in respective sections below.

> I have recorded those combinations, which did not work for me with an `NA` in the second spreadsheet of combined sections 7, 8 and 9 below. I would like to request the reviewer to kindly advise me, if the following observations are reasonable. With that I would like to address the points mentioned in the evaluation rubric of the mid-term project.

1. **MP.1 Data Buffer Optimization**

As per the requirement of the project, I have implemented a templated class called `RingBuffer` in the *dataStructures.h* header file. This class wraps a vector. Apart from specifying the object type, template includes an additional parameter called maxLength, which denotes the number of objects, that would be contained by the buffer in memory. In the implementation I have used the buffer size as 2. When we try to push a new objet to RingBuffer at the rear end and the buffer is already at the maxLength, it removes the object at the front to make room for the new object. RingBuffer class also overloads the subscript operator, which encapsulates a similar accessor logic, that the project had before implementing ring buffer. We do this in order to keep the base implementation simple and focus on exploring the detectors and descriptor algorithms as main objective.

2. **MP.2 KeyPoint Detection**

The function `detKeyPointsModern` declared in *matching2D.hpp* and defined in *matching2D_Student.cpp* provides implementation of key point detection using FAST, AKAZE, BRISK, ORB, and SIFT algorithms from OpenCV. For Harris corner detection a dedicated function `detKeyPointsHarris` and for Shi-Tomasi another dedicated function `detKeypointsShiTomasi` have been implemented. For the implementation of Harris detection we use non maximal suppression, taking reference from the practice exercises during lectures. For all algorithms the times required in milliseconds in order to detect key points have been logged. The performance part of this writeup derives the average time over all images from these logged times.

3. **MP.3 KeyPoint Removal**

In the file *MidTermProject_Camera_Student.cpp*, right after invoking `detKeyPointsModern` function to detect key points a rectangular area was predefined as *cv::Rect* object, which encapsulates the area covered by the preceding vehicle. In order to filter key points and just focus on the key points on the preceding vehicle we have taken an iterative approach. The cv::Rect object provides us with the coordinate of the top left point of the rectangular area along with the width and height. Using these information, we can infer the upper and lower bounds of both x and y coordinates. Based on these we can then check if a given key point falls inside these lower and upper bounds. We have used this semantic to filter-in only those key points, which are inside the said bound in the vector `keyPointOI`. We then updated the DataFrame object for a given image with only the key points of interest using this vector instead of all the detected key points.

4. **MP.4 KeyPoint Descriptors**

The function `descKeyPoints` declared in *matching2D.hpp* and defined in *matching2D_Student.cpp* instantiates the appropriate *cv::DescriptorExtractor* object based on the provided *descriptorType*. Depending on this argument we instantiate one of the BRIEF, ORB, FREAK, AKAZE, and SIFT DescriptorExtractors to create distinctive feature vectors of the surrounding area of given key points. A common approach is taken for key points description extraction i.e., upon instantiating a given kind of *DescriptorExtractor* we use the `compute` method in order to derive the feature vectors as collection in a *cv::Mat* object. The time required in milliseconds is logged. Like key point detection we derive the average time required in milliseconds for each descriptor algorithm and use that to evaluate the descriptor performance combined with detectors later in the performance section.

5. **MP.5 Descriptor Matching**

The function `matchDescriptors` declared in *matching2D.hpp* and defined in *matching2D_Student.cpp* implements the logic for matching a given set of key points from an image captured in previous time step (source) and an image captured in current time step (reference). This convention is also referred as query image and train image in OpenCV literature. This function, however does not take the actual images, rather the key points and respective descriptors from the source image and reference image. Additionally it takes a *descriptorType*, *matcherType*, and *selectorType*.
    - Depending on **descriptorType** denoting a binary descriptor or gradient based one, we decide on whether to use L2_NORM or Hamming Distance to measure the distance between the feature vectors. By incorporating the XOR operation Hamming Distance can be very efficient for computing the distance for binary feature vectors, as opposed to L2_NORM for gradient based feature vectors.
    - Depending on **matcherType** denoting a brute-force or FLANN(using efficient kdtree approach) based matching preference, we instantiate either *cv::BFMatcher* or FLANN-based *cv::DescriptorMatcher* object.
    - Depending on **selectorType** denoting either a nearest neighbor or k-nearest neighbor matching algorithm, we either use `match` or `knnMatch` method of the *cv::DescriptorMatcher* object. The k-nearest neighbor approach incorporates some additional processing because we derive 2 best matches in a reference image for a given feature descriptor in the source image. We also needed to use a separate buffer for keeping the collection of *cv::DMatch* objects for this approach.

6. **MP.6 Descriptor Distance Ratio**

The use of k-nearest neighbor based selector has the added complexity of deriving the best match out of k (in our implementation k = 2) computed matches. For that we implement Lowe's distance ratio test. In the `matchDescriptor` function we have defined a pre-agreed distance ratio threshold of `0.8`. Since we assumed k = 2 from beginning our iterative implementation is simpler in this regard. We iterate over each pair of best matches for the reference image and check if the first match (which is the best match) is within the defined distance ratio threshold from the second based match and include it in the final set of *cv::DMatch* objects if it is true.

7. **Combined (MP.7, MP.8, MP.9) - Performance Evaluation 1, 2, 3**

We start by counting the number of detected key points (MP.7). In the following table we have recorded three pieces of information, total number of detected key points, time consumed to detect the key points and the number of key points detected specifically for the rectangular area denoting the preceding vehicle, for each image. This last information is captured by the `PV` keyword in the following table.

| **Image/Detector** | **Harris** | **Shi-Tomasi** | **FAST** | **BRISK** | **ORB** | **AKAZE** | **SIFT** |
|---------|---------|---------|---------|---------|---------|---------|---------|
|image0|Total: 53, PV: 10, 17.64 ms|Total: 1370, PV: 127, 17.63 ms|Total: 1824, PV: 149, 1.11 ms|Total: 2757, PV: 254, 61.92 ms|Total: 500, PV: 91, 12.96 ms|Total: 1351, PV: 162, 57.31 ms|Total: 1438, PV: 137, 78.41 ms|
|image1|Total: 62, PV: 13, 11.23 ms|Total: 1301, PV: 120, 9.62 ms|Total: 1832, PV: 152, 0.93 ms|Total: 2777, PV: 274, 56.06 ms|Total: 500, PV: 102, 6.31 ms|Total: 1327, PV: 157, 46.09 ms|Total: 1371, PV: 131, 55.24 ms|
|image2|Total: 63, PV: 17, 11.94 ms|Total: 1361, PV: 123, 10.81 ms|Total: 1810, PV: 152, 1.02 ms|Total: 2741, PV: 276, 55.20 ms|Total: 500, PV: 106, 5.26 ms|Total: 1311, PV: 159, 45.58 ms|Total: 1380, PV: 121, 55.18 ms|
|image3|Total: 69, PV: 17, 10.63 ms|Total: 1358, PV: 120, 9.41 ms|Total: 1817, PV: 157, 0.85 ms|Total: 2735, PV: 275, 55.22 ms|Total: 500, PV: 113, 5.31 ms|Total: 1351, PV: 154, 46.16 ms|Total: 1335, PV: 135, 54.51 ms|
|image4|Total: 82, PV: 20, 10.80 ms|Total: 1333, PV: 120, 10.44 ms|Total: 1793, PV: 149, 0.78 ms|Total: 2757, PV: 293, 55.07 ms|Total: 500, PV: 109, 6.05 ms|Total: 1360, PV: 162, 46.44 ms|Total: 1305, PV: 134, 55.27 ms|
|image5|Total: 73, PV: 18, 9.40 ms|Total: 1284, PV: 115, 9.71 ms|Total: 1796, PV: 150, 0.72 ms|Total: 2695, PV: 275, 59.27 ms|Total: 500, PV: 124, 5.18 ms|Total: 1347, PV: 163, 47.06 ms|Total: 1369, PV: 139, 55.11 ms|
|image6|Total: 69, PV: 17, 10.03 ms|Total: 1322, PV: 114, 10.73 ms|Total: 1788, PV: 157, 0.74 ms|Total: 2715, PV: 289, 63.24 ms|Total: 500, PV: 129, 4.97 ms|Total: 1363, PV: 173, 44.86 ms|Total: 1396, PV: 136, 58.66 ms|
|image7|Total: 108, PV: 25, 10.12 ms|Total: 1366, PV: 125, 10.09 ms|Total: 1695, PV: 152, 0.85 ms|Total: 2628, PV: 268, 55.01 ms|Total: 500, PV: 127, 6.15 ms|Total: 1331, PV: 175, 49.44 ms|Total: 1382, PV: 147, 55.48 ms|
|image8|Total: 81, PV: 20, 9.78 ms|Total: 1389, PV: 112, 9.40 ms|Total: 1749, PV: 139, 1.07 ms|Total: 2639, PV: 260, 54.17 ms|Total: 500, PV: 124, 5.21 ms|Total: 1358, PV: 175, 50.97 ms|Total: 1463, PV: 156, 55.49 ms|
|image9|Total: 68, PV: 19, 9.49 ms|Total: 1339, PV: 113, 9.78 ms|Total: 1770, PV: 144, 0.86 ms|Total: 2672, PV: 250, 59.96 ms|Total: 500, PV: 125, 5.06 ms|Total: 1331, PV: 175, 46.31 ms|Total: 1422, PV: 135, 55.91 ms|

From the above table we can infer some initial ideas about the performance for each algorithm. Harris detector being a legacy implementation can be kept aside since it has detected the lowest number of key points (keeping in mind that, we specifically incorporated non-maximal suppression for the same). It means, that the number of true positive detection should be proportionally less. Shi-Tomasi detector algorithm has performed far better than Harris, both in terms of number of detected key points as well as time efficiency. When it comes to contemporary detection algorithms, we can clearly see `FAST` being a clear winner in terms of efficiency and number of detected key points. It is followed by `ORB`, and `BRISK` being at the third position. Shi-Tomasi algorithm featured better time efficiency compared to BRISK, however BRISK detected more key points, which means proportionally more true positives.

Now we'd deal with MP.8 and MP.9 together with the following spreadsheet, where we have tried all combinations of detectors and descriptors to match key points on the preceding vehicle. In order to keep this table concise, we have averaged the number of total detected key points, time required to detect all key points, and finally, time required for extracting distinctive feature vectors, over all the images.

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

First of all, in my experiments I have found, that `ORB` is the most versatile descriptor algorithm, which reliably works when combined with most of the key point detectors. Overall, I have found `ORB` to be the most efficient descriptor algorithm as well. On the detector side as previously stated, I have found `FAST` to be the most efficient one. The results above suggests, that the `FAST` and `ORB` is the most efficient combinations of algorithms.

Firstly, I would like to list top 3 detectors in terms of time efficiency and performance once again at a glance, first one being the most efficient among three.
1. `FAST` (most efficient in key point detection and time efficiency)
2. `ORB`
3. `BRISK` (although Shi-Tomassi featured slightly better time, but in terms of number of key points detected, BRISK is more reliable)

Secondly, top 3 descriptors at a glance.
1. `ORB` (most efficient in feature description and time efficiency)
2. `SIFT`
3. `AKAZE`

My recommendations for top 3 combinations of detector and descriptor algorithms looks at overall performance specifically in the second spreadsheet above, where they are combined. The parameters are number of key points detected and time efficiency.
1. `FAST` & `ORB` (Overall the best combination both in terms of detected key points and time efficiency).
2. `ORB` & `ORB` 
3. `FAST` & `SIFT`

Quite surprisingly, when I used `ORB` algorithm both for key points detection and description it did not come up to be as time-efficient as combining `FAST` and `ORB`, which I found as the overall best combination. For all the combinations, which worked for me, I have found mostly true positives along with some false positives on the preceding vehicle. I have not specifically focused on this aspect for my evaluation.

Thank you for reviewing my project. Please advise me, if I have implemented something wrongly and what I could do better to improve the observations, I recorded above.


