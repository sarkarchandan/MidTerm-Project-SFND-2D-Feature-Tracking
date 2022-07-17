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

/// Defines a generic ring buffer with constant size.
template <typename T, size_t maxLength>
class RingBuffer
{
    std::vector<T> vec;

public:
    /// Initializes the ring buffer.
    RingBuffer()
    {
        vec.reserve(maxLength);
    }

    /**
     * @brief Inserts an element at
     * the rear end of the ring buffer.
     *
     * @param data Data to insert.
     */
    void push(T const &data)
    {
        if (vec.size() == maxLength)
        {
            vec.erase(vec.begin());
        }
        vec.push_back(data);
    }

    /**
     * @brief Overloads subscript operator
     * in order implement accessor to the
     * underlying elements of the ring buffer.
     *
     * @param index Index for accessing the
     * element. This implementation tries to
     * be as close as possible to the earlier
     * implementation of accessing w.r.t. rear
     * end e.g., index 1 would access the most
     * recent element inserted in the buffer,
     * index 2 would access the second most
     * recent element etc..
     * @return T& Data from the buffer.
     */
    T &operator[](uint index)
    {
        if (index > maxLength)
        {
            throw std::runtime_error("index out of range");
        }
        return *(vec.end() - index);
    }

    /**
     * @brief Returns the current size of
     * the ring buffer.
     *
     * @return size_t Size of the ring
     * buffer.
     */
    size_t size() const
    {
        return vec.size();
    }
};

#endif /* dataStructures_h */
