#!/bin/bash

# It is better to run kinit before this script

# Run this from your work dir, not within this package
asetup AtlasPhysics,17.2.2.1.2,setup,here

# For now, we will need SUSYTools
svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-00-63 SUSYTools
python SUSYTools/python/install.py

# Configure RootCore
cd RootCore
./configure
source scripts/setup.sh
cd ..

# Compile everything
$ROOTCOREDIR/scripts/build.sh


