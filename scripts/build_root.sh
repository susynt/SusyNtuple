#!/bin/sh

# Script to build root on a recent version of ubuntu, see
#  http://docs.travis-ci.com/user/ci-environment/
#
# davide.gerbaudo@gmail.com, April 2014

# Get ahold of a vm running 12.04.4 LTS from
#  http://cloud-images.ubuntu.com/precise/current/

# install requirements (see
#  http://root.cern.ch/drupal/content/build-prerequisites)
sudo apt-get update
sudo apt-get install \
     x11-common libx11-6 x11-utils libX11-dev \
     libgsl0-dev gsl-bin libxpm-dev libxft-dev g++ gfortran build-essential \
     g++ libjpeg-turbo8-dev libjpeg8-dev libjpeg8-dev libjpeg-dev \
     libtiff4-dev libxml2-dev libssl-dev libgnutls-dev libgmp3-dev \
     libpng12-dev libldap2-dev libkrb5-dev freeglut3-dev libfftw3-dev \
     python-dev libmysqlclient-dev libgif-dev libiodbc2 libiodbc2-dev \
     subversion libxext-dev libxmu-dev libimlib2 gccxml

# build
mkdir /tmp/build_root
mkdir /tmp/install_root
cd /tmp/build_root/
wget  ftp://root.cern.ch/root/root_v5.34.22.source.tar.gz
tar xzf root_v5.34.22.source.tar.gz
mv root root_v5.34.22
cd root_v5.34.22/
export ROOTSYS=/tmp/install_root/
./configure linuxx8664gcc --enable-explicitlink --enable-soversion --enable-minuit2 --enable-python --enable-roofit --enable-xml --enable-tmva --disable-xrootd --fail-on-missing
make

# package
make dist
ls -lh /tmp/build_root/root_v5.34.22.linuxx8664gcc-gcc-4.6.tar.gz
