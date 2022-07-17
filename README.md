# SFND 2D Feature Tracking - Mid Term Project Report

I have worked on this project mainly on my local machine. I built OpenCV 4.6.0 from source and enabled the `OPENCV_ENABLE_NONFREE` option. However, some combinations of detectors and descriptors resulted in error for me. Hence, I have recorded my observations for the combinations, which worked for me.

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
