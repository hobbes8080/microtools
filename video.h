#pragma once

extern "C" {
#include "microtools_development/kernel/lines.h"
}

/* opencv */
#include <opencv2/opencv.hpp>

struct video {

  /* note that as of C++11, it is allowed to initialize non-static variables in the "struct" section. these initializations are basically moved to the initializer function. */
  
  std::vector<cv::Mat> frames;		// original data frames, changed by stabilization, border removal and initial contrasting; used for mean image generation and vessel detection
  std::vector<cv::Mat> frames_std;	// frames, changed by -f option etc, used for std generation and analysis
  std::vector<cv::Mat> frames_hct;     	// frames, changed by -g option etc, used for ribbon generation and analysis
  double fps;	                        // frames per second	   
  int length;				// total number of frames
  double conv_fact, conv_fact_c;	/* latter is used for conv_fact_corr vector */
  int frame_fullwidth;
  double FOV_fact;
  cv::Point max_traj;		/* max. amplitude of stabilization trajectory  in x and y axis*/
  double Lin;			/* input luminance */
  double mean_avasc;		/* mean avascular luminostiy */

  video();
  void erase();
  bool empty();
  bool load(std::string filename, bool verbose);
  void view(double zoom);
  cv::Mat calc_avg(int start, int end);		/* calculate mean image */
  void calc_cont(std::vector<double> clahe_cliplimit, std::vector<int> clahe_gridsize);		/* calculate contrasted mean images */
  video extract_ribbon(contour line);
  video extract_squareribbon(contour line);
  void write_vid(std::string filename);
  video crop_black_edges();	/* it doesnt make a lot of sense for this to be recursive, since from the moment of black edge crop or also stabilization, the new video should be used for all further processing... */
  /* void calc_vessel_mask(std::vector<vessel::vessel> VASA); //--this cannot be defined here since vessel class definitions cannot exist here - it would create recursive include statements. this could be remedied by an overdue redefinition of classes in microtools*/
  void calc_Lin(double pixel_cutoff);

  /* parameters that mainly serve automated quality grading */
  double foc, stdbylength, venularstdstraightness, min_ven_rbcv;
  int framefps;
  double illum_maxloc, illum_maxval;

  /* output parameters from out_par that describe the video */
  /* future implementation */

  /* void calc_params(int PROGRAM_STATE, std::vector<vessel::vessel> VASA);		/\* calculate the parameters above, in the future maybe integrate out_par parameters that pertain to the video also in the video class *\/ -- do this in the program class since the vessels belong to the program class and not the video class (because vessel class contains video class for hct ribbons, which precludes it from containing the vessels class.... ---- This mess needs to be cleaned up in a rewrite, it was a nice idea that increased efficiency or at least programming efficiency in the early days but decreases efficiency as complexity increases*/
  /* VASA has to be passed on to this function because it is a member of the program class... */
    
  bool quality_stability(bool verbose, double fov, double foc_limit_low, double foc_limit_high, double fov_limit_low);	/* these functions return true for passed, false for negative. they may save their results to a variable and only re-calculate the first time they are called. but in a first iteration they will re-calculate everytime they are called in order to ensure accuracy. */
  bool quality_duration(bool verbose, int framesfps_limit_low);	/* the quality functions need to check if everything they need has been calcualted in the video structure (avg_img, etc) */
  bool quality_content(bool verbose, double TVD, int content_limit_low, int content_limit_high);
  bool quality_focus(bool verbose, int PROGRAM_STATE, double stdbylength_limit_low);
  bool quality_illumination(bool verbose, double maxloc_lim_low, double maxloc_lim_high);
  bool quality_pressure(bool verbose, int PROGRAM_STATE, double ven_rbcv, double venstdstraight_limit_low, double ven_rbcv_limit_low);
  
  /* denoising functions */
  void denoise(bool verbose);
  
  /* histogram calculation and manipulation functions */
  int multipeakAvgimgHistAnalysis(cv::Mat &src);
  std::vector<cv::Mat> singlepeakAvgimgHistAnalysis(cv::Mat &src, std::vector<cv::Mat> &frames, bool verbose);
  void singlepeakIndframeHistAnalysis(bool verbose);
  /* histogram functions -end- */
  cv::Mat avg_img;		/* mean image */
  cv::Mat avg_img_orig;		/* original mean image (before histogram shift) */
  std::vector<cv::Mat> cont_img;           /* contrasted mean image (contrasted for vessel detection) */
  cv::Mat avg_img_with_vessels;
  cv::Mat avg_img_only_vessels;
  cv::Mat avg_img_with_bwvessels;

  cv::Mat img_mask_vessel; /* background black, vessel area white */
  cv::Mat img_mean_vessel; /* background black, vessel area original colors */
  cv::Mat img_mean_avascular; /* background original colors, vessel area black */

  cv::Mat avg_img_SO2;
  cv::Mat avg_img_SO2delta;

  
  enum VIDEOTYPE {CCT_mha, AVA_avi, OXY_int, VIDEOTYPE_NUM_ITEMS};
  VIDEOTYPE videotype;
  enum STABILIZATIONTYPE {stabilization_unstable, stabilization_external, stabilization_microtools}; /* stab_undefined does not exist. the fact if stabilization is requested when converting the file will define if stabilization is deemed to have happened externally, or if requested it has been done by microtools */
  STABILIZATIONTYPE stabilizationtype;
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
      {AVA_avi, "a"},
      {OXY_int, "i"}
    };
  std::map<STABILIZATIONTYPE, std::string> MText_stabilizationtype =
    {
      {stabilization_unstable, "u"},
      {stabilization_external, "e"},
      {stabilization_microtools, "s"}
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
