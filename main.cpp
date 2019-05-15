#include "micro.h"
#include "version.h"

#define MAX_DIR_CHARS 256
#define STD_CONF_NAME "microtools.conf"

// program version is defined in version.h

int main(int argc, char* argv[]) {
  // init message
  std::cout << "Microtools CORE version " << ver_def << "\nmicrotools_devs 2017-2019\n";

  program P;
  int argct;
  int opt;
  // int flag_edit=0, flag_detect=0, flag_std=0, flag_hct=0;
  bool flag_edit=false, flag_detect=false, flag_std=false, flag_hct=false, flag_write=false, flag_write_raw=false, flag_stabilization=false, flag_stab_alpha=false, flag_border=false, flag_quality=false, flag_convert=false, flag_bright=false, flag_framebright=false, flag_denoising=false, flag_write_all=false, flag_verbose=false;
  char* input_file=NULL;
  char* param_file=NULL;
  char* iter_file=NULL;
  char* char_tmp=NULL;

  std::string file_tmp;
  std::size_t found;

  // std::string usage=" [-dshewrvmngtbf] -i [inputfile] -p [parameter file]";

  // the iteration parameters will be included in the parameters file, so no separate file will be needed.
  // get version info from header file
  P.ver = ver_def;

  P.CLI_state=false;
  if(argc>1){
    // save argument string for output in results file
    // first_arge = argv[1];
    // P.options = argv[1];
    // P.options.assign(argv + 1, argv + argc);
    // Parse command line arguments and evoke use cases for specific command line arguments
    while ((opt = getopt(argc, argv, "dshewkrmngtvbfoi:p:")) != -1) {
      // write arg to options string for inclusion in the results file
      // P.options.append(opt, 1);
      P.options += opt;
      switch (opt) {
	// file args
      case 'i':
	input_file = optarg;
	break;
      case 'p':
	param_file = optarg;
	break;
      case 'q':			// has been removed since quality check is not an option but a result
	// perform quality check
	flag_quality=true;
	break;
      case 'b':
	// perform histogram shift  before analysis (potential data loss!)
	flag_bright=true;
	break;
      case 'f':
	// perform per-frame histogram shift  before analysis (potential data loss!)
	flag_framebright=true;
	break;
      // case 'q':
      // 	iter_file = optarg;
      // 	break;
	// analysis args
      case 'v':
	flag_convert=true;
	P.CLI_state=true;
	break;
      case 'd':
	flag_detect = true;
	P.CLI_state=true;
	break;
      case 's':
	flag_std = true;
	P.CLI_state=true;
	break;
      case 'h':
	flag_hct = true;
	P.CLI_state=true;
	break;
      case 'm':
	flag_stabilization=true;
	break;
      case 'n':
	flag_stab_alpha=true;
	break;
      case 'g':
	flag_denoising=true;
	break;
      case 't':
	flag_border=true;
	// P.CLI_state=true;
	break;
	// other options
      case 'e':
	flag_edit = true;
	break;
      case 'k':
	flag_write_all=true;
	break;
      case 'w':
	flag_write=true;
	break;
      case 'r':
	flag_write_raw=true;
	break;
      case 'o':
	flag_verbose=true;
	break;
      default: /* '?' */
	std::cerr << "Invalid arguments. Terminating..." << endl;
		  // << endl << "Usage: " << argv[0] << usage << endl;
	exit(0);
      }

    }

    if(flag_verbose){
      std::cout << "CLI arguments detected" << endl;
      printf("detect=%d; std=%d, hct=%d, edit=%d; write_files:%d; write_raw:%d, stabilize:%d, optind=%d; argc=%d\n",
	     flag_detect, flag_std, flag_hct, flag_edit, flag_write, flag_write_raw, flag_stabilization, optind, argc);
      printf("inputfile=%s; paramfile=%s\n", input_file, param_file);

      if(optind<argc){
	// there is a name argument present, currently not processed
	printf("name argument = %s\n", argv[optind]);
      }
    }
  }

  // implement flags
  // --implement defaults. these flags will be on by defalut. specifying them will turn them off.
  flag_framebright=!flag_framebright;
  flag_border=!flag_border;
  // --implement dependencies
  if(flag_stab_alpha)
    flag_stabilization=true;	// CAVE watch order, since flag_stab_alpha also implies flag_border :-)
  if(flag_stabilization)
    flag_border=true;
  if(flag_write_raw)
    flag_write=true;
  if(flag_write_all)
    flag_write=true;
  if(flag_write)
    P.CLI_state=true;
  // --set global status variables
  P.write_files_requested=flag_write;
  P.write_all_files_requested=flag_write_all;
  P.write_raw_files_requested=flag_write_raw;
  P.stabilization_requested=flag_stabilization;
  P.stab_alpha_requested=flag_stab_alpha;
  P.border_requested=flag_border;
  P.quality_requested=flag_quality;// has been removed since quality check is not an option but a result
  P.convert_requested=flag_convert;
  P.bright_requested=flag_bright;
  P.verbose=flag_verbose;
  // if(flag_framebright){
  //   P.framebright_requested=flag_framebright;
  //   P.bright_requested=false;	// -f excludes -b
  // }
  // - f does not exclude -b
  P.framebright_requested=flag_framebright;
  P.denoising_requested=flag_denoising;

  if(P.verbose)
    std::cout << "Processing parameter file paths..." << endl;
  // if no parameter file is given, use the application's path and standard name
  if(param_file==NULL){
    program::param_file_path_name.assign(GetCurrentDir(char_tmp, MAX_DIR_CHARS));
    program::param_file_path_name = program::param_file_path_name + "/" + STD_CONF_NAME;
  }
  else{
    program::param_file_path_name.assign(param_file);
    // if no path is given, pre-append current directory
    if(program::param_file_path_name.find("/\\")==string::npos){
      file_tmp.assign(GetCurrentDir(char_tmp, MAX_DIR_CHARS));
      program::param_file_path_name=file_tmp+"/"+program::param_file_path_name;
    }
  }
  if(P.verbose)
    std::cout << "param_file_path_name=" << program::param_file_path_name << endl;

  if(iter_file!=NULL){
    // ---------------------
    // perform iteration operations
    // ---------------------
    if(param_file!=NULL){
      std::cerr << "Error: No standard parameter file may be given when itreation is requested." << endl;
		  // << "Usage: " << argv[0] << " " << usage << endl;
	exit(0);
    }
    
  }
  else{
    if(P.CLI_state){
      // ---------------------
      // perform CLI operations
      // ---------------------
      // if no input file is given, terminate
      if(input_file==NULL){
	std::cerr << "Error: Input file must be given when invoking analysis via CLI." << endl;
		  // << "Usage: " << argv[0] << " " << usage << endl;
	exit(0);
      }
      else{
	// set directory for output files to the same directory as the input file is located
	file_tmp.assign(input_file);
	found = file_tmp.find_last_of("/\\"); // handle linux as well as windows paths
	program::input_file_name=file_tmp.substr(found+1);
	program::input_file_path=file_tmp.substr(0, found);
	// if path and name are the same, then set path to current dir
	if(program::input_file_path==program::input_file_name){
	  program::input_file_path.assign(GetCurrentDir(char_tmp, MAX_DIR_CHARS));
	}
	if(P.verbose)
	  std::cout << "input_file_path=" << program::input_file_path << "; input_file_name=" << program::input_file_name << endl;
      }
    
      // config file reading will be performed automatically when set_state does initialization
      
      // invoke all further prossessing functions here according to the flags given
      // if(flag_detect||flag_std||flag_hct){
      if(flag_convert){
	P.set_state(program::state::loaded);
      }
      if(flag_stabilization || flag_border){
	P.set_state(program::state::stabilized);
      }
      if(flag_detect){
	P.set_state(program::state::segmented);
      }
      if(flag_std){
	P.set_state(program::state::diagramed);
      }
      if(flag_hct){
	P.set_state(program::state::ribboned);
      }

      // write output files
      P.write_out_files();

    }
    else{
      // ---------------------
      // enter menu interface
      // ---------------------

      // read config file
      P.read_parameter_file();    

      // if no input file is given, set directory for output files to current directory
      if(input_file==NULL){
	program::input_file_path.assign(GetCurrentDir(char_tmp, MAX_DIR_CHARS));
	if(P.verbose)
	  std::cout << "input_file_path=" << program::input_file_path << endl;
	P.menu_filename_ask=true;

	// process some CLI options that are also valid in the menu interface
	P.stabilization_requested=flag_stabilization;
	
	// when using menu interface, always set default param_file_path_name
	// program::param_file_path_name.assign(GetCurrentDir(char_tmp, MAX_DIR_CHARS));
	// program::param_file_path_name = program::param_file_path_name + "/" + STD_CONF_NAME;
	// std::cout << "param_file_path_name=" << program::param_file_path_name << endl;
      }
      else{
	// set directory for output files to the same directory as the input file is located
	file_tmp.assign(input_file);
	found = file_tmp.find_last_of("/\\"); // handle linux as well as windows paths
	program::input_file_name=file_tmp.substr(found+1);
	program::input_file_path=file_tmp.substr(0, found);
	// if path and name are the same, then set path to current dir
	if(program::input_file_path==program::input_file_name){
	  program::input_file_path.assign(GetCurrentDir(char_tmp, MAX_DIR_CHARS));
	}
	if(P.verbose)
	  std::cout << "input_file_path=" << program::input_file_path << "; input_file_name=" << program::input_file_name << endl;
	// since input file is given, perform load
	P.menu_filename_ask=false;
	P.set_state(program::state::stabilized); // always perform stabilization instead of loading, since stabilization is then controlled by the stabilization_requested flag
      }

    
      // invoke menu interface
      P.menu();
    
      return 1;
    }
  }
}
