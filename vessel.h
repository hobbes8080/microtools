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
  
  video ribbon;			/* the Hct ribbon */
  video ribbon_enh_with_blobs;	/* the Hct enhanced ribbon with all detected blobs */
  cv::Mat std;			/* the SPT image */
  cv::Mat std_with_ridges;	/* the SPT image with all accepted (green) and unaccepted (red) ridges */
  cv::Mat std_ridges_only;

  double length;		/* vessel length [mm] */
  double length_h3;		/* length corrected for linking. volume, flow and DHb are affected, RBCv and Hct not */
  double diameter;      	/* vessel diameter [mm] */
  double diameter_SD;           // maybe handy in noise selection (noise has high SD)
  double volume;		/* vessel volume [mm^3] */
  double volume_h3;		/* volume corrected for linking */
  double hemat;
  double hemat_SD;
  double num_blobs;
  double num_rbc;
  double velocity;  		/* vessel flow velocity from STD [um*s^-1] */
  double velocity_SD;		// may be handy in vessel selection for STD velocity measurements
  double flow;			/* CURRENTLY NOT IN USE. this is currently calculated at the video level. it may be interesting to calculate it at the vessel level in the future in order to have weighted SD (heterogeneity) for flow. vessel flow from STD and vessel volume [ul*s^-1] */

  /* double num_std_ridges; */
  /* double num_std_ridges_accepted; */

  vessel();
  vessel(contour l, int n, double conv_fact_l);
  void calc_length(double conv_fact_u);
  void calc_width(double conv_fact_u);
  void calc_volume();
  void calc_rib(video in);
  void calc_hemat();
  void calc_hemat_auto(double conv_fact_l);
  void calc_STD(video in);
  void calc_vel(double s, double l, double h, int min, double fps, double conv_fact_l);
  void calc_vel_orig(double s, double l, double h, int min, double fps, double conv_fact_l);
  void show_vel();
  void draw(cv::Mat& img);	/* draws in color */
  void draw_bw(cv::Mat& img);	/* draws light BW with greyscales */
  void draw_bw_2(cv::Mat& img);	/* draws dark BW with dotted lines */
  
};			// holds all data concerning a single vessel, in addition to its contour base.
