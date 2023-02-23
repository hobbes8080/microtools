# MicroTools
MicroTools implements an advanced computer vision algorithm permitting automated analysis of handheld vital microcsopy (HVM) image sequences recorded of the human and porcine sublingual microcirculation.

Version 63 of MicroTools has been described in detail and validated against the current gold standard in the following manuscript: "An automated software tool called MicroTools for quantifying capillary density and red blood cell velocity in handheld vital microscopy".

# Releases
Initial release: test release, purpose: test release infrastructure

Current release: MicroTools version 63

# Compilation
## OpenCV
To compile on Ubuntu 18.10, install the following packages:

For ffmpeg support (necessary to open and write .avi files with cap.open):
```
libv4l-dev ffmpeg libavutil-dev pkg-config libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavresample-dev
# Update for Ubuntu 22.04 and up:
libv4l-dev ffmpeg libavutil-dev pkg-config libavcodec-dev libavformat-dev libavutil-dev libswscale-dev
```

For gtk support (necessary to play video):
```
libgtk2.0-dev
```

Fetch and compile openCV:
```
cd ~/microtools/opencv-stable
cd opencv-stable
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git # alternatively, download known working version, eg. 3.4.3
mkdir build
cd build
rm -R * # recommended if step is repeated
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_FFMPEG=ON -D WITH_TBB=ON -D WITH_GTK=ON -D WITH_V4L=ON -D WITH_OPENGL=ON -D WITH_CUBLAS=ON -D WITH_QT=OFF -D BUILD_TIFF=ON ../opencv
make -j12
```

## ITK
To compile on Ubuntu 18.10, install the following packages:
```
uuid-dev libtclap-dev libtiff-dev libpng-dev
```

You may have to build the latest version of cmake:
```
sudo apt install libssl-dev
#in the cmake source directory, enter
cmake -D CMAKE_USE_OPENSSL=TRUE .
make -j12
sudo make install
```

Fetch and compile openCV:
```
cd ~/microtools/ITK-stable
git clone https://github.com/InsightSoftwareConsortium/ITK.git # alternatively, download known working version, eg. 14.3.1
mkdir build
cd build
rm -R * # recommended if step is repeated
cmake -D BUILD_SHARED_LIBS=ON -D CMAKE_INSTALL_PREFIX=/usr/local -D Module_ITKVideoBridgeOpenCV=ON -D OpenCV_DIR=../../OpenCV-3.4.3/build ../ITK
# Update for OpenCV 4.7.0 bridge from ITK 5.3.0:
cmake -D BUILD_SHARED_LIBS=ON -D CMAKE_INSTALL_PREFIX=/usr/local -D Module_ITKVideoBridgeOpenCV=ON -D OpenCV_DIR=../../OpenCV-4.7.0/build ../ITK
make -j12
```

## Optional: set library paths
```
# current session
export PATH=$PATH:/usr/local/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
# permanently
nano ~/.profile
export PATH=$PATH:/usr/local/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
export PATH=$PATH:/usr/local/share
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/share
```

## Microtools

Fetch:
```
cd ~/microtools/microtools
git init
git clone <remote repository URL>  # https://github.com/hobbes8080/microtools.git
```

Build and package project with scripted versioning using the automated build/versioning facility
```
./build.sh
```

Alternatively build manually. Step 1: invoke cmake to generate makefiles (from emacs IDE):
```
M-x compile
cmake .
# to re-make all makefiles:
rm -rf ./CMakeFiles
rm -f Makefile
```

Step 2: compilation (from emacs IDE):
```
M-x compile
make -k
# to re-compile all files:
make -k clean
```

# Packaging
To package on Ubuntu 18.10, install the following packages:
```
checkinstall
```
## OpenCV
```
sudo checkinstall --pkgname opencv-microtools --pkgversion 3.4.3-MT63
# Update MT70
sudo checkinstall --pkgname opencv-microtools --pkgversion 4.7.0-MT70
```
## ITK
```
sudo checkinstall --pkgname itk-microtools --pkgversion 14.3.1-MT59
# Update MT70
sudo checkinstall --pkgname itk-microtools --pkgversion 5.3.0-MT70
```
## MicroTools
```
sudo checkinstall --pkgname microtools --pkgversion 63 -y --default
# Update MT70
sudo checkinstall --pkgname microtools --pkgversion 70 -y --default
```
## manual install (example)
```
sudo apt install ./microtools_20181130-1_amd64.deb
```
## manual un-install
```
sudo apt remove microtools
```

# Notes
In addition to contributing to the present repository, we invite researchers to participate in microtools_development source code in the context of mutual scientific collaboration. Please contact: matthias@microcirculation.network

# Acknowledgements
The OpenCV advanced computer vision library (Bradski G (2000) Open source computer vision. Dr Dobb’s Journal of Software Tools 2000:184404319)

The National Library of Medicine Insight Toolkit (Yoo TS, Ackerman MJ, Lorensen WE, et al (2002) Engineering and algorithm design for an image processing Api: a technical report on ITK--the Insight Toolkit. Stud Health Technol Inform 85:586–592)

Carsten Steger (Steger C (1998) An unbiased detector of curvilinear structures. IEEE Transactions on pattern analysis and machine intelligence 20:113–125)

Microsoft getopt implementation (2016) for Windows compatibility
