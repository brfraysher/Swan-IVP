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
svn co https://oceanai.mit.edu/svn/moos-ivp-aro/trunk/ ~/moos-ivp || { echo "---Error cloning moos-ivp tree... Is subversion installed?---" ; exit 1; }

echo "---Installing dependencies---"
sudo apt install -y g++  cmake  xterm || { echo "---Error in first apt install---" ; exit 1; }
sudo apt install -y libfltk1.3-dev  freeglut3-dev  libpng-dev  libjpeg-dev || { echo "---Error in second install---" ; exit 1; }
sudo apt install -y libxft-dev  libxinerama-dev   libtiff5-dev || { echo "---Error in third install---" ; exit 1; }

echo "---Entering moos-ivp---"
cd ~/moos-ivp/

echo "---Building moos---"
./build-moos.sh || { echo "---Error building moos---" ; exit 1; }

echo "---Building ivp modules---"
./build-ivp.sh || { echo "---Error building Ivp modules---" ; exit 1; }

echo "---Entering repository directory---"
cd $repoDir

echo "---Building repository modules---"
./build.sh || { echo "---Error building repository modules---" ; exit 1; }

echo "---Adding moos-ivp binaries path to .bashrc file---"
echo "export PATH=$PATH:~/moos-ivp/bin:$repoDir/bin" >> ~/.bashrc

echo "---Success!!!---"
echo ""
echo ""
echo "--- Run the below command to source your .bashrc file ---"
echo "---   source ~/.bashrc   ---"