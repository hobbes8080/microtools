#pragma once

//http://insightsoftwareconsortium.github.io/ITKBarCamp-doc/ITK/VideoProcessingUsingOpenCVBridge/index.html

/* #define ITK_IO_FACTORY_REGISTER_MANAGER */
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <itkImage.h>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkExtractImageFilter.h"
#include <itkOpenCVImageBridge.h>
#include "video.h"

//cv::Mat processFrame(const cv::Mat& inputImage);
//void processAndSaveVideo(cv::VideoCapture& vidCap, const std::string& filename);
bool read_mha(video* vid, std::string filename, bool verbose);
