#!/usr/bin/env bash

###  Created by Andrew Sucato

echo "---Entering head of repository---"
cd ..
repoDir=$(pwd)

echo "---Entering home directory---"
cd ~/

echo "---Updating Repository---"
sudo apt update -y

echo "---Cloning moos-ivp tree---"
sudo apt install -y subversion
svn co https://oceanai.mit.edu/svn/moos-ivp-aro/trunk/ ~/moos-ivp || { echo "---Error cloning moos-ivp tree... Is subversion installed?---" ; exit 1; }

echo "---Installing moos dependencies---"
sudo apt install -y g++  cmake  xterm || { echo "---Error in first apt install---" ; exit 1; }
sudo apt install -y libfltk1.3-dev  freeglut3-dev  libpng-dev  libjpeg-dev || { echo "---Error in second install---" ; exit 1; }
sudo apt install -y libxft-dev  libxinerama-dev   libtiff5-dev || { echo "---Error in third install---" ; exit 1; }

### Install wjwwood/serial library from git ###
echo "---Downloading catkin source---"
git clone https://github.com/ros/catkin || { echo "---Error downloading catkin source---" ; exit 1;  }

echo "---Installing catkin dependencies---"
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu `lsb_release -sc` main" > /etc/apt/sources.list.d/ros-latest.list'
wget http://packages.ros.org/ros.key -O - | sudo apt-key add - || { echo "---Error adding ROS repo---" ; exit 1; }
sudo apt install -y cmake python-catkin-pkg python-empy python-nose python-setuptools libgtest-dev build-essential || { echo "---Error installing catkin dependencies---" ; exit 1; }
sudo apt update -y
sudo apt upgrade -y

echo "---Moving into catkin directory---"
cd ~/catkin

echo "---Building and installing catkin---"
mkdir build && cd build && cmake ../ && make && sudo make install || { echo "---Error building and installing catkin---" ; exit 1; }

echo "---Cloning serial repository---"
cd ~
git clone https://github.com/wjwwood/serial.git || { echo "---Error cloning serial git repository---" ; exit 1; }

echo "---Building and installing serial library---"
cd serial
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DBUILD_SHARED_LIBS=OFF || { echo "---Error creating cmake files---"; exit 1; }
make || { echo "---Error building serial library---" ; exit 1; }
sudo make install || { echo "---Error installing serial library---" ; exit 1; }

### Build MOOS & MOOS IVP packages ###
echo "---Entering moos-ivp---"
cd ~/moos-ivp/

echo "---Building moos---"
./build-moos.sh || { echo "---Error building moos---" ; exit 1; }

echo "---Building ivp modules---"
./build-ivp.sh || { echo "---Error building Ivp modules---" ; exit 1; }

### Build our repository ###
echo "---Entering repository directory---"
cd $repoDir

echo "---Building repository modules---"
./build.sh || { echo "---Error building repository modules---" ; exit 1; }

echo "---Adding moos-ivp binaries path to .bashrc file---"
echo "export PATH=$PATH:~/moos-ivp/bin:$repoDir/bin" >> ~/.bashrc

### Clean Up ###
echo "---Cleaning up!---"
cd ~
sudo rm -rf serial/ catkin/

echo "---Success!!!---"
echo ""
echo ""
echo "--- Run the below command to source your .bashrc file ---"
echo "---   source ~/.bashrc   ---"
