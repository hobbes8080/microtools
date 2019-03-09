# cmake_minimum_required(VERSION 2.8)
# project( microtools )

# # find_package( OpenCV REQUIRED )
# # find_package( OpenCV REQUIRED PATHS "/home/matthias/microtools_core/lib/include/opencv" "/home/matthias/microtools_core/lib/include/opencv2" "/home/matthias/microtools_core/opencv-stable/build" )
# find_package( OpenCV REQUIRED PATHS "/home/matthias/microtools_core/lib/include/opencv" "/home/matthias/microtools_core/lib/include/opencv2" "/home/matthias/microtools_core/opencv-stable/build" )

# include_directories( ${OpenCV_INCLUDE_DIRS} ) 

# # SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
# SET(GCC_COVERAGE_LINK_FLAGS    "-lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core" )

# # SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
# SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )


# # target_link_libraries( microtools ${OpenCV_LIBS} )
# target_link_libraries( microtools ${OpenCV_LIBS} )


cmake_minimum_required(VERSION 3.5)
project( microtools.core )

set(CMAKE_CXX_COMPILER "g++")

# set program installation directory
set(CMAKE_INSTALL_PREFIX /usr/local/bin)

# opencv default install
find_package( OpenCV 3.4.3 EXACT REQUIRED ) #prev. 3.3.1, current max. ver. 4.0, only one working for ITK comp.  3.4
# -> microtools compiles with opencv 4.0, but 3.4 is used for ITK compilation
include_directories( ${OpenCV_INCLUDE_DIRS} )

# opencv local install
# find_package( OpenCV REQUIRED NO_DEFAULT_PATH PATHS "/home/matthias/microtools_core/lib/include/opencv" )
# include_directories( ${OpenCV_INCLUDE_DIRS} )

# ITK default install
find_package( ITK 4.13 EXACT REQUIRED ) #prev.4.13, curent max. ver. 5.0, works but unreliable, most reliable version is 4.13
include( ${ITK_USE_FILE} )

# std::experimental::filesystem
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
# the following steps are not needed anymore as of gcc 8.x+
# add_library( stdc++fs UNKNOWN IMPORTED )
# set_property( TARGET stdc++fs PROPERTY IMPORTED_LOCATION "/usr/lib/gcc/x86_64-linux-gnu/7/libstdc++fs.a" )
# target_link_libraries(${PROJECT_NAME} stdc++fs)

add_executable( microtools.core main.cpp program.cpp menu.cpp click.cpp ridge.cpp segment.cpp stabilize.cpp track.cpp vessel.cpp video.cpp misc.cpp out_params.cpp ITK.cpp ./kernel/alloc.c ./kernel/convol.c ./kernel/correct.c ./kernel/error.c ./kernel/link.c ./kernel/normal.c ./kernel/position.c ./kernel/thresh.c ./kernel/width.c )

install(TARGETS microtools.core DESTINATION ${CMAKE_INSTALL_PREFIX} )

install(PROGRAMS microtools DESTINATION ${CMAKE_INSTALL_PREFIX} )

target_link_libraries( microtools.core ${OpenCV_LIBS} ${ITK_LIBRARIES} stdc++fs )
