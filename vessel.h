#pragma once

#include "video.h"

extern "C" {
	#include "microtools_development/kernel/lines.h"
}

struct vessel {
  int id;
  enum VESSEL_TYPE { capillary, venule, arteriole, undetermined, VESSEL_TYPE_NUM_ITEMS };
  enum RIDGE_VEL_TYPE { low, normal, hyperdynamic, high, artifact, noflow, VEL_TYPE_NUM_ITEMS }; /* the low group will contain low velocity and noise; in the future, also pendulum flow could be detected (venules->presure artifacts) */
  
  VESSEL_TYPE type;
  contour line;
  std::vector<contour> std_contours; /* this vector contains all detected STD ridges */
  std::vector<double> ridge_vel;     /* this vector contains the velocities for all detected contours */
  std::vector<double> ridge_straightness; /* measure of straightness between 0 (very curvy) and 1 (completely straight): proportion of length of straight line between start and endpoint of ridge AND actual ridge length */
  double vmax;			     /* this variable contains the vessel-specific max. velocity that can be detected in the STD */
  std::vector<RIDGE_VEL_TYPE> ridge_vel_type; /* this vector contains classification of the all the detected STD ridges */
  int num_vel_type[VEL_TYPE_NUM_ITEMS];	/* number of vessels for each VEL_TYPE */
  int num_ridges_total;
  int num_ridges_interpretable;
  
  bool vessel_perfused;
  bool vessel_hyperdynamic;

  int P1x, P1y, Pnx, Pny;	/* start- and endpoints. used to match vessels recognized in two similar videos */
  
  video ribbon;			/* the Hct ribbon */
  video ribbon_enh_with_blobs;	/* the Hct enhanced ribbon with all detected blobs */
  cv::Mat std;			/* the SPT image */
  cv::Mat std_with_ridges;	/* the SPT image with all accepted (green) and unaccepted (red) ridges */
  cv::Mat std_ridges_only;

  cv::Mat img_mask_vessel; /* background black, vessel area white */
  cv::Mat img_mask_vessel_start; /* background black, vessel area white */
  cv::Mat img_mask_vessel_end; /* background black, vessel area white */
  cv::Mat img_mean_vessel; /* background black, vessel area original colors */

  double length;		/* vessel length [mm] */
  double length_h3;		/* length corrected for linking. volume, flow and DHb are affected, RBCv and Hct not */
  double diameter;      	/* vessel diameter [mm] */
  double diameter_SD;           // maybe handy in noise selection (noise has high SD)
  double diameter_h4;		/* diameter corrected for overestimation of capillary boundaries by time-based rbc locus. volume, flow, hct are affected, ribbon generation and thus blob detection are not */
  double volume;		/* vessel volume [mm^3] */
  double volume_h3;		/* volume corrected for linking */
  double hemat;
  double hemat_SD;
  double num_blobs;
  double num_rbc;
  double velocity;  		/* vessel flow velocity from STD [um*s^-1] */
  double velocity_SD;		// may be handy in vessel selection for STD velocity measurements
  double flow;			/* CURRENTLY NOT IN USE. this is currently calculated at the video level. it may be interesting to calculate it at the vessel level in the future in order to have weighted SD (heterogeneity) for flow. vessel flow from STD and vessel volume [ul*s^-1] */
  bool flowdirection_reversed;

  double Lout_mean;		/* temporal mean luminosity within vessels*/
  double Lout_mean_start, Lout_mean_end;
  double Lout_frame;		/* temporal mean luminosity within vessels*/
  double Lout_frame_start, Lout_frame_end;
  double Lout_blob;/* temporal mean luminosity within RBCs*/
  double Lout;			/* greater of Lout_mean and Lout_blob: reduces errors due to plasma in mean images VS plasma next to blobs */
  double Lout_start, Lout_end;
  double OD;			/* optical density */
  double OD_start, OD_end;
  double SO2, SO2_raw, SO2_trans; // SO2_raw is the raw value before normalization, SO2_trans is the normalized value before resetting outliers
  double SO2_start, SO2_end;
  double SO2_a_v_position;	
  double SO2_delta_intravessel;
  double corresp_vessel_compl_wavelength;
  double a_v_rank;
  double SO2_vis; // SO2 value for visualization (mix between per-vessel and per-video)

  /* double num_std_ridges; */
  /* double num_std_ridges_accepted; */

  vessel();
  vessel(contour l, int n, double conv_fact_l, cv::Mat img_mean_orig);
  void calc_length(double conv_fact_u);
  void calc_width(double conv_fact_u);
  void calc_volume();
  void calc_Lout_vessel();
  void calc_vessel_mask(cv::Mat img_mean_orig);
  void calc_rib(video in);
  void calc_hemat();
  void calc_hemat_auto(double conv_fact_l, bool verbose);
  void calc_STD(video in);
  void calc_vel(double s, double l, double h, int min, double fps, double conv_fact_l, double conv_fact_cx);
  void calc_vel_orig(double s, double l, double h, int min, double fps, double conv_fact_l);
  void show_vel();
  void draw(cv::Mat& img);	/* draws in color */
  void draw_bw(cv::Mat& img);	/* draws light BW with greyscales */
  void draw_bw_2(cv::Mat& img);	/* draws dark BW with dotted lines */
  void draw_SO2(cv::Mat &img);
  
};			// holds all data concerning a single vessel, in addition to its contour base.
