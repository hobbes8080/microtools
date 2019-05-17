#pragma once


/* OUTPUT_PARAMETERS */

struct output_parameters {
  double FOV;				// the area surveyed in micrometer^2
  double TVD_small;				// combined length of vessels of type capillary per area unit
  double TVD_large;				// combined length of vessels of type venule per area unit
  double CBV_small;				// capillary blood volume per area unit (based on TVD) -> is in effect capillary blood volume density
  double CBV_large;				// capillary blood volume per area unit (based on TVD)
  double MVC;			// mean capillary velocity -> mean is weighted by length
  double HVC;			// hererogeneity (=SD) of capillary velocity
  double MVV;			// mean venular velocity
  double HVV;			// heterogeneity (=SD) of venular velocity
  /* double AAV;				// average absolute velocity: normalized for length of individual vessel (meaning: weighted average by length) */
  /* double AACV;			// average absolute capillary velocity */
  /* double AAVV;			// average absoolute venular velocity */
  /* double HAV;				// heterogeneity of AAV; normalized for length (standard deviation) */
  /* double HACV;			// heterogeneity of AACV; normalized for length (standard deviation) */
  /* double HAVV;			// heterogeneity of AAVV; normalized for length (standard deviation) */
  double MHC;				// mean capillary hematocrit; normalized for length; MAYBE normalize by volume instead of length
  double HHC;					/* heterogeneity (weighted sd) of cHct */
  double MHV;					/* (weighted) mean hct venules */
  double HHV;					/*  (weighted) sd hct venules = heterogeneity */
  double APSI;			// average perfused speed index: based on FCD vessels

  double CBFD_small;			/* capillary blood flow = area * MVC(weighted mean velocity)  [mm^3/sec] */
  double CBFD_large;
  double TVA_small;   /* total vascular area (total cross section of the capillaries) [mm^2]*/
  double TVA_large;
  double hct_dis_large;		/* "discharge hematocrit"(flow of hemoglobin) [mm^3/sec für Hb] */
  double hct_dis_small;

  double FCD;			/* FCD=TVD_small*PPV */
  double PPV;			/* weighted mean of the categorical variable: proportion of perfused vessels */
  
  int pxhoriz, pxvert, fps, frames; /* properties of the video clip that are relevant for data analysis and reporting */

  bool quality_accepted, quality_duration, quality_illumination, quality_stability,
    quality_focus, quality_pressure, quality_content;

  output_parameters();
  /* output_parameters(int rows, int cols, std::vector<vessel> VASA); */
  void clear();
  void calc_FOV(int rows, int cols, double conv_fact);
  void calc_TVD(std::vector<vessel> VASA);
  void calc_STD(std::vector<vessel> VASA);
  void calc_HCT(std::vector<vessel> VASA);
  void write_out();
};


/* INPUT PARAMETERS */

struct ridgedetect_parameters{
  /* steger parameters */
  double sigma_width;		/* [um], width of detection scope */
  double min_length;		/* [um] */
  double steger_high;
  double steger_low;
};

struct segmentation_parameters : public ridgedetect_parameters{
  /* clahe multi-histogram contrast normalization parameters */
  double clahe_cliplimit;
  int clahe_gridsize;
};

struct std_parameters : public ridgedetect_parameters {
  /* STD  */
  double min_velocity;		/* [um/s], threshold below which std_ridges are considered artifacts. this could be set to the same value as "std_low_velocity", as soon as artifact detection is good enough so that it is not necessary anymore to use this threshold for artifact detection */
  double low_velocity;		/* um/s, threshold below which vessels are considered "no-flow" for PPV calculation */
  double hyperdynamic_velocity;	/* um/s, threshold above which a vessel is considered hyperdynamic, as long as it is below vessel specific vmax */
  double max_velocity;		/* [um/s], not used */
  double squeeze;		/* [um], discard start and end of each detected ridge within STDs in order to minimize noise */
  double min_straightness;
  double preblur_width;		/* how to blur the individual frames before generating STDs  */
  int algo;		/* 1: use new calc_vel algorithm, 0: use old calc_vel algorithm (old rbc path classification, no barcode sign detection)  */
  double cutoff_vessel_notperfused;	/* min. proportion of "low velocity" STD ridges needed in a vessel to classify vessel as not perfused  */
  double cutoff_vessel_hyperdynamic;   /* min. proportion of "hyperdynamic" STD ridges needed in a vessel to classify vessel as hyperdynamic - this will be in parallel to the classification of a video as hyperdynamic based on all detected ridges within a video file  */
  double cutoff_barcode;	/* min. proportion of "now flow" STD ridges needed in a vessel to classify vessel as zero flow (barcode sign)  */
};

/* in the following all parameters used for the analysis are defined. an instance of this will be a static component of the program class, and used to read in all analysis parameters from the config file. */
struct input_parameters{

  /* MAP that is used to read in defaults of values from the conf file */
  /* the map is translated into the input_parameters structure by translate_to_struct() */
  std::map<std::string, std::vector<std::string>> param_map;
  std::map<std::string, std::vector<std::string>> param_map_second;
  std::map<std::string, std::vector<std::string>> param_map_default;
  
  /* vessel detection */
  int seg_num_passes;
  int seg_passoverlap_finethres;	/* [um], in the fine analysis of overlap, this is the lower limit to discard the shorter of two separate vessels */
  int seg_passoverlap_coarsethres;	/* [um], in the coarse analysis of overlap, this is lower limit to perform fine analysis of overlap */
  std::vector<segmentation_parameters> seg_params;
  double seg_h2;		/* [mm mm-2] correction of human - to machine differences in vessel connection */
  double seg_h3;		/* [1] correction of human - to machine differences in vessel connection */

  int convert_params;		/* if 1, steger etc. params will be corrected with conv_fact; if 0, they will not be corrected and used in the algorithm as they are */
  int convert_resolution;	/* if 1, frame resolution is scaled according to *(current conv_fact)/(avi ava conv_fact). in this way, full frame analysis, e.g. of mha files, is enabled without changing the detection parameters that were calibrated to avi ava resolution. then, conv_fact for further analysis is set to the avi_ava value regardless of the currently read file format. in this way, if convert_params is set to 1, the conversion will automatically be neglected, which is of course desired since the resolution has changed. */

  /* STD */
  std_parameters std_params;	/* parameters for ridge detection within space-time diagrams */
  
  /* misc */
  std::string csv_separator;
  int mha_fps;			/* frames per second used to read .mha files (we do not read the xml file) */
  std::vector<double> conv_fact_vect;/* conversion factor: um/px , for different types of video*/
  std::vector<double> conv_fact_corr;
  std::vector<int> frame_fullwidth_vect;/* width of a full frame (a frame without black borders). this is used to correct things such as clahe_gridsize and steger_high and steger_low for a reduced frame size when black borders are removed */
  /* double conv_fact;		/\* conversion factor: um/px -> this will NOT be filled by the config file, but by the appropriate member from conv_fact_vect when a video file is opened *\/ */
  double cap_diam_cutoff;	/* cutoff in diameter between venules and capillaries */
  double cutoff_hyperdynamic_pervideo; /* min. proportion of "hyperdynamic" STD ridges needed in a video file regardless of vessel to classify the video as hyperdynamic - this will be in parallel to the per vessel classification  */

  /* quality limits */
  double foc_limit_low;
  double foc_limit_high;
  double fov_limit_low;

  double maxloc_lim_low;
  double maxloc_lim_high;

  double venstdstraight_limit_low;
  double ven_rbcv_limit_low;

  double stdbylength_limit_low;

  int framesfps_limit_low;

  int content_limit_low;
  int content_limit_high;

  /* stabilization */
  int stab_mean_frames;
  int stab_smoothing_radius;
  double track_threshold1;
  double track_threshold2;
  int track_apertureSize;
  int flow_maxLevel;
  double flow_minEigThreshold;
  double flow_width, flow_height, flow_search_epsilon;
  int flow_search_maxCount;
  
  /* HCT */
  int blb_min_threshold;
  int blb_max_threshold;
  double blb_min_dist;
  int blb_filterbyarea;
  int blb_min_area;
  int blb_max_area;
  int blb_filterbycolor;
  int blb_color;
  int blb_filterbycircularity;
  double blb_min_circularity;
  double blb_max_circularity;
  int blb_filterbyconvexity;
  double blb_min_convexity;
  double blb_max_convexity;
  int blb_filterbyinertia;
  double blb_min_inertiaratio;

  // HCT other parameters
  int blb_max_move;
  int rbc_diameter;
  int rbc_volume;
  
  // HCT ribbon enhancement parameters
  int rib_min_canny;
  int rib_max_canny;
  int rib_gauss_sigma;
  int rib_contrast;

    /* FUNCTIONS */
  input_parameters();		/* constructor */
  void read_conf(std::string conf_filename, bool verbose);
  void set_defaults(bool verbose);
  void translate_to_struct(bool verbose);		/* translates values stored in the map to the structure */
  void display_conf();
  void convert_params_func(double conv_fact, bool verbose, double FOV_fact);
};


  
