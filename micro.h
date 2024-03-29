#pragma once


/* clibs */
#include <iostream>
#include <cstring>
#include <vector>
#include <cassert>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <thread>
#include <sstream>
#include <string>

/* Steger */
extern "C" { 
#include "microtools_development/kernel/lines.h"
}

/* opencv */
#include <opencv2/opencv.hpp>

/* ITK: see ITK.h */
/* #define ITK_IO_FACTORY_REGISTER_MANAGER */
/* #include <itkImageFileReader.h> */
/* #include <itkImage.h> */
/* #include <itkVideoStream.h> */
/* #include <itkVideoFileReader.h> */
/* #include <itkOpenCVImageBridge.h> */
/* #include <itkOpenCVVideoIOFactory.h> */

/* own */
#include "video.h"
#include "vessel.h"
#include "out_params.h"
#include "ITK.h"

// Handle include for getopt() diffently for Win32 and Linux
#ifdef _WIN32
	#include "getopt/getopt.h"
#elif __linux__
	#include <unistd.h>
#endif

/* handle directory operations per OS */
#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

/* handle filesystem class per OS */
/* #ifdef _WIN32 */
/* 	#include <experimental/filesystem> */
/*     namespace fs = std::experimental::filesystem; */
/* #else */
/*     #include <experimental/filesystem> */
/*     namespace fs = std::experimental::filesystem; */
/* #endif */
#include <filesystem>
namespace fs = std::filesystem;

//#define _CRT_SECURE_NO_WARNINGS // needs to work

using namespace std;		/* dirty..... */
using namespace cv;

//global constant definitions



/* const int SMOOTHING_RADIUS = 100; // In frames. The larger the more stable the video, but less reactive to sudden panning */
const int HORIZONTAL_BORDER_CROP = 0; // In pixels. Crops the border to reduce the black borders from stabilisation being too noticeable.

/* the net two entries will be obsolete */
const double RBC_vol = 90;			// in femtolitre = cubic micrometer
const double RBC_DIAMETER = 7;		// in micrometer

const double pi = 3.14159265359;

#define red Vec3b (0, 0, 255)
#define green Vec3b (0, 255, 0)
#define blue Vec3b (255, 0, 0)
#define yellow Vec3b (0, 255, 255)
#define cyan Vec3b (255, 255, 0)

#define darkred Vec3b (0, 0, 191)
#define darkgreen Vec3b (0, 191, 0)
#define darkblue Vec3b (191, 0, 0)
#define darkyellow Vec3b (0, 191, 191)
#define darkcyan Vec3b (191, 191, 0)

#define black Vec3b (0, 0, 0)
#define white Vec3b (255, 255, 255)
#define grey50 Vec3b (127, 127, 127)
#define grey75 Vec3b (64, 64, 64)
#define grey25 Vec3b (191, 191, 191)
#define grey35 Vec3b (166, 166, 166)
#define grey65 Vec3b (89, 89, 89)
#define grey90 Vec3b (25, 25, 25)
#define grey10 Vec3b (230, 230, 230)

#define black_alpha50 Scalar(255, 255, 255, 127)
#define black_alpha25 Scalar(255, 255, 255, 64)
#define black_alpha75 Scalar(255, 255, 255, 191)



// global struct declarations

// class definition

class program {
	public:
		// config params used by vessel structures (should eventually all be static and loaded from a config file) in the program constructor
		double sigma;
		double low;
		double high;
		int MINIMAL_VESSEL_LENGTH;						// the minumum amount of pixels for a vessel must have to be accepted
		static double OVERLAP_THRESHOLD;						// the maximal allowed average minimal distance between two contours before the smaller one is considered overlap and discarded
		double CONTRAST_CLIP_LIMIT;						// the clip limit used in contrasting the AVERAGE_IMAGE
		int CONTRAST_TILE_GRID;							// the tile grid size used in contrasting the AVERAGE_IMAGE
  enum state {unloaded, loaded, stabilized, segmented, diagramed, ribboned, diagramed_ribboned};

		
		static int MINIMAL_DISPLACEMENT;				// the minimal amount of elapsed frames a STD contour must have to be accepted (to be converted into a minimal speed factor)
		static double CONVERSION_DISTANCE;				// the amount of micrometers per pixel
		static int VESSEL_DIAMETER_CUTOFF;				// cut-off for determining the detected vessel type (venule)
		static int FCD_CUT_OFF;							// cut-off velocity to determine if a capillary is perfused

  inline static vector<cv::Mat>stat_frames_orig; // this is a static copy of the original frames, to make accessible for luminosity analysis in the vessel class.
  inline static int Lout_cutoff_dep;
  
		static std::string input_file_path;
		static std::string input_file_name;
		static std::string input_file_path_name;
		static std::string param_file_path_name;
		static std::string options;

		static bool write_files_requested;
		static bool write_all_files_requested;
		static bool write_raw_files_requested;

		inline static bool detect_requested;
		inline static bool STD_requested;
		inline static bool HCT_requested;
  // note: all member variables that do not have the inline definition here, need to be initialized in the class (program.cpp)
  
		static bool CLI_state;
		static bool verbose;
		static bool advoutput;
		static bool menu_filename_ask;
		static bool stabilization_requested;
		static bool stab_alpha_requested;
		static bool border_requested;
		static bool quality_requested;
		static bool convert_requested;
		static bool bright_requested;
		static bool framebright_requested;
		static bool denoising_requested;
		static bool detection_requested;
		static bool iterate;

		static bool denoised;
		static bool framebrighted;

		string ver;


		/* static string csv_separator; */

		static input_parameters in_params;

		static output_parameters out_par;

		void calc_quality_params(); /* calculate foc etc parameters (properties of the video class) that pertain to automated quality checking */
		
		program();
		bool set_state(state NEW_STATE);
		void menu();
		bool load();
		void set_zoom();
		void playback();
		void cut_vid();
		bool stab();
		cv::Point stabalgorithm_sop(int SMOOTHING_RADIUS, bool writevideo, bool scale_shear,
					    double track_threshold1, double track_threshold2, int track_apertureSize,
					    int flow_maxLevel, double flow_minEigThreshold,
					    double flow_width, double flow_height, double flow_search_epsilon, int flow_search_maxCount);
		bool stabalgorithm_sim();		
		bool stabalgorithm_kal();		
		void avg();
		bool seg();
		//void segment();
		//void segment_double_pass();
		void draw_vessels (Mat &img);
		void show_vessels ();
		int click();
		bool gen_std();
		void show_std();
		bool gen_rib();
		void view_rib();
		void view_hemat();
		void track_vessels();
		void track_RBC();
		void print_out_params();
		void write_out_files();
		void read_parameter_file();
		void squareribbon();
		bool cropborders();


	private:
		state PROGRAM_STATE;

		double VIDEO_ZOOM;
		double RIBBON_ZOOM;

		int rows, cols;
		int VESSELS_ANALYZED;
  		video VIDEO_FILE;
		video VIDEO_FILE_stab; /* this is a copy of VIDEO_FILE that is created after stabilization and will not be affected by further processing. may be used to export the stabilized video file */
		video VIDEO_FILE_unstab; /* this is a copy of the original file before stabilization and is used if file conversion is requested in addition to stabilization */
		/* output_parameters out_params; */
		/* std::string OPENED_FILE; */
		cv::Mat AVERAGE_IMAGE;
		/* cv::Mat AVERAGE_IMAGE_ENH; */
		cv::Mat VESSEL_SELECT;
		vector<vessel> VASA;


};

// global function declarations

//CLAHE
void clahe(Mat &img, double clip, int grid);
//misc
int max_vector(vector<int> a);
int min_vector(vector<int> a);
double max_vector(vector<double> a);
double min_vector(vector<double> a);
bool detect_params_change(int a, int b, int c, int d, int e, int f);
void create_vessel_select(cv::Mat &img, vector<contour> cont, int rows, int cols);
void read(char& input);
//ridge detection
std::vector<contour> segment(std::vector<cv::Mat> img, std::vector<segmentation_parameters> seg_params, double OVERLAP_THRESHOLD_FINE, double OVERLAP_THRESHOLD_COARSE, bool verbose);
void ridge_detect(cv::Mat in_img, vector<contour> &contours, double s, double h, double l, int mode, int min, bool compute_width);
void show_ridges(cv::Mat &img, vector<contour> contours, bool SHOW_EDGES, bool SHOW_CENTRE, bool SHOW_NORMAL);
void show_ridge(cv::Mat &img, contour line, int id, bool SHOW_EDGES, bool SHOW_CENTRE, bool SHOW_NORMAL, Vec3b color_centre, Vec3b color_edge);
void show_ridge_2(cv::Mat &img, cv::Mat img_mask_vessel, contour line,
		  int line_thickness, bool SHOW_DIRECTION,
		  bool SHOW_CENTRE, Vec3b color_centre, int skip_centre,
		  bool SHOW_EDGES, Vec3b color_edge, int skip_edge,
		  bool SHOW_NORMAL, Vec3b color_normal, int skip_normal,
		  int id, bool SHOW_ID_TWICE,  Vec3b color_id,
		  bool FILL, Vec3b color_fill, double FILL_ALPHA);
double mean_vessel_value(cv::Mat img_mask_vessel, cv::Mat img_mean_vessel, int mask_target, bool bwconvert, int pixel_cutoff, bool select_bright);
double mean_vessel_value_byframe(cv::Mat img_mask_vessel, int mask_target, bool bwconvert, int pixel_cutoff, bool select_bright);
void calc_pervideo_vessel_mask(cv::Mat &img_mask_vessel, cv::Mat &img_mean_vessel, cv::Mat &img_mean_avascular, cv::Mat avg_img_orig, std::vector<vessel> VASA);
//space time diagrams
void contour_displacement(contour line, int &dx, int &dy);
//averaging and standard deviation
double avg_vector(vector<double> a);
double avg_vector(vector<int> a);
double SD_vector(vector<double> a);
double SD_vector(vector<int> a);
//tracking
void kalman(video in, contour line, double conv);
// parameter change
bool detect_params_change(int a, int b, int c, int d, int e, int f);
bool detect_params_change(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l);
// keypoint comparison
bool compare_keypoints(vector<cv::KeyPoint> K, KeyPoint key, double max_dist);
// contour comparison
bool comp_cont(contour a, contour b);
double compare_contour(contour a, contour b, double OVERLAP_THRESHOLD_COARSE);
vector<contour> merge_contours(vector<contour> A, vector<contour> B, double threshold_overlap_fine, double overlap_threshold_coarse);

static bool abs_compare_double(double a, double b);
static bool abs_compare(int a, int b);

double focus(cv::Mat img);
