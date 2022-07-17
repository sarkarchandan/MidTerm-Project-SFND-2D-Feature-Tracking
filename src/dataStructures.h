#ifndef dataStructures_h
#define dataStructures_h

#include <vector>
#include <queue>
#include <deque>

#include <opencv2/core.hpp>

/// Represents the available sensor implementation at a given point in time
struct DataFrame
{
    cv::Mat cameraImg;                   /*camera image*/
    std::vector<cv::KeyPoint> keyPoints; /*2D key points within camera image*/
    cv::Mat descriptors;                 /*key point descriptors*/
    std::vector<cv::DMatch> kptMatches;  /*key point matches between previous and current frame*/
};

template <typename T, uint maxLength, typename Container = std::deque<T>>
class RingBuffer : public std::queue<T, Container>
{
public:
    void push(T const &data)
    {
        if (this->size() == maxLength)
        {
            this->c.pop_front();
        }
        this->c.push_back(data);
    }

    T &operator[](uint index)
    {
        if (index < maxLength)
        {
            return this->c[index];
        }
        throw std::range_error("index out of range");
    }
};

#endif /* dataStructures_h */
