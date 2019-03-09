#pragma once

extern "C" {
#include "microtools_development/kernel/lines.h"
}

/* opencv */
#include <opencv2/opencv.hpp>


struct video {

  /* note that as of C++11, it is allowed to initialize non-static variables in the "struct" section. these initializations are basically moved to the initializer function. */
  
  std::vector<cv::Mat> frames;		// original data frames	
  double fps;	                        // frames per second	   
  int length;				// total number of frames
  double conv_fact;
  cv::Point max_traj;		/* max. amplitude of stabilization trajectory  in x and y axis*/

  video();
  void erase();
  bool empty();
  bool load(std::string filename, bool verbose);
  void view(double zoom);
  void calc_avg();		/* calculate mean image */
  void calc_cont(std::vector<double> clahe_cliplimit, std::vector<int> clahe_gridsize);		/* calculate contrasted mean images */
  video extract_ribbon(contour line);
  video extract_squareribbon(contour line);
  void write_vid(std::string filename);
  video crop_black_edges();	/* it doesnt make a lot of sense for this to be recursive, since from the moment of black edge crop or also stabilization, the new video should be used for all further processing... */

  /* denoising functions */
  std::vector<cv::Mat> denoise(std::vector<cv::Mat> &frames);
  
  /* histogram calculation and manipulation functions */
  int multipeakAvgimgHistAnalysis(cv::Mat &src);
  std::vector<cv::Mat> singlepeakAvgimgHistAnalysis(cv::Mat &src, std::vector<cv::Mat> &frames, bool verbose);
  std::vector<cv::Mat> singlepeakIndframeHistAnalysis(cv::Mat &avg, std::vector<cv::Mat> &frames, bool verbose);
  /* histogram functions -end- */
  cv::Mat avg_img;		/* mean image */
  cv::Mat avg_img_orig;		/* original mean image (before histogram shift) */
  std::vector<cv::Mat> cont_img;           /* contrasted mean image (contrasted for vessel detection) */
  cv::Mat avg_img_with_vessels;
  cv::Mat avg_img_only_vessels;
  cv::Mat avg_img_with_bwvessels;

  enum VIDEOTYPE {CCT_mha, AVA_avi, VIDEOTYPE_NUM_ITEMS};
  VIDEOTYPE videotype;
  enum CAMERATYPE {camera_undefined, cytocam_IDF, microscan_SDF, CAMERATYPE_NUM_ITEMS};
  CAMERATYPE cameratype=camera_undefined;
  enum SPECIESTYPE {species_undefined, pig, rat, human_adult, human_pediatrics, SPECIESTYPE_NUM_ITEMS};
  SPECIESTYPE speciestype=species_undefined;
  enum TISSUETYPE {tissue_undefined, sublingual, skmuscle, intestinal, kidney, liver, TISSUETYPE_NUM_ITEMS};
  TISSUETYPE tissuetype=tissue_undefined;
  std::string MText_firstchar = "h"; /* this is the first character used in the video file extesion for video files saved by microtools */
  std::map<VIDEOTYPE, std::string> MText_videotype =
    {
      {CCT_mha, "m"},
      {AVA_avi, "a"}
    };
  std::map<CAMERATYPE, std::string> MText_cameratype =
    {
      {camera_undefined, "x"},
      {cytocam_IDF, "i"},
      {microscan_SDF, "s"}
    };
  std::map<SPECIESTYPE, std::string> MText_speciestype =
    {
      {species_undefined, "x"},
      {pig, "p"},
      {rat, "r"},
      {human_adult, "h"},
      {human_pediatrics, "i"}
    };
  std::map<TISSUETYPE, std::string> MText_tissuetype =
    {
      {tissue_undefined, "x"},
      {sublingual, "s"},
      {skmuscle, "m"},
      {intestinal, "i"},
      {kidney, "k"},
      {liver, "l"}
    };
};
