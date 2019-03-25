#pragma once

/* opencv */
#include <opencv2/opencv.hpp>


/* histogram calculation and manipulation structs  */
struct InHistLength
{
  int pos1;
  int pos2;
  int size()
  {
    return pos2 - pos1 +1;
  }
};

struct PeakInfo
{
  int pos;
  int left_size;
  int right_size;
  float value;
};

/* histogram structs -end- */


/* the following global functions are located in video.cpp */
PeakInfo peakInfo(int pos, int left_size, int right_size, float value);

std::vector<int> getLocalMaximum(cv::InputArray _src, int smooth_size, int neighbor_size, float peak_per);

std::vector<PeakInfo> findPeaks(cv::InputArray _src, int window_size);

Mat drawHistogram(Mat &hist, int hist_h, int hist_w, int hist_size, Scalar color, int type);

int drawPeaks(Mat &histImage, vector<int>& peaks, int hist_size, Scalar color);

Mat drawHistogramSimple(Mat &b_hist, vector<int> peaks, int hist_h, int hist_w, int histSize);
