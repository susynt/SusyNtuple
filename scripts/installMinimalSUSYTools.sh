#!/bin/bash

#
# This script will checkout and compile the minimal content from SUSYTools
# needed for reading the SusyNt. The functionality needed:
#   - BTagCalib
#   - SUSYCrossSection
#

# If an argument is passed, it is taken to be the SUSYTools tag name
tag="SUSYTools-00-03-14"
if [ $# -gt 0 ]; then tag=$1; fi

basePath="svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/"
url="${basePath}${tag}"

echo "Installing minimal SUSYTools with tag $tag"
echo "from $url"

cd $ROOTCOREDIR/..
mkdir SUSYTools

svn co $url/SUSYTools/SUSYCrossSection.h $url/SUSYTools/BTagCalib.h SUSYTools/SUSYTools/
svn co $url/Root/SUSYCrossSection.h $url/Root/BTagCalib.h SUSYTools/Root/

