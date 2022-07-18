# SFND 2D Feature Tracking - Mid Term Project Report

I would like to report on my findings on the performance of following detector and descriptor combinations. Based on that I would like to propose the top 3 detector / descriptors combinations.

I have worked on this project on my local machine with Linux, because it is convenient for me. I built OpenCV 4.6.0 from source with `OPENCV_ENABLE_NONFREE` option enabled. However, some combinations of detectors and descriptors resulted in errors for me. I have recorded those combinations with `NA` in the following spreadsheet along with combinations, which worked. I would like to request the reviewer to kindly advise me, if the following observations are reasonable.

| **Detectors**     | **Descriptors** | **#KeyPoints(avg)** | **Key Point Detection Time(avg)** | **Description Extraction Time(avg)** |
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

