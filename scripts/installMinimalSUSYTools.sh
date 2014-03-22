#!/bin/bash

#
# This script will checkout and compile the minimal content from SUSYTools
# needed for reading the SusyNt. The functionality needed:
#   - BTagCalib
#   - SUSYCrossSection
# 
# This script is currently for testing, so things will probably get moved around later
#

# Run this from your work dir, not from within this package
#asetup 17.2.9.1,here,slc5
SVNOFF="svn+ssh://svn.cern.ch/reps/atlasoff/"
SVNPHYS="svn+ssh://svn.cern.ch/reps/atlasphys/"

# Tags to checkout
susyURL="$SVNOFF/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-03-14"
rootCoreURL="$SVNOFF/PhysicsAnalysis/D3PDTools/RootCore/tags/RootCore-00-02-99"
bTagURL="$SVNOFF/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface/tags/CalibrationDataInterface-00-03-06"
mt2URL="$SVNPHYS/Physics/SUSY/Analyses/WeakProduction/Mt2/tags/Mt2-00-00-01"
trigURL="$SVNPHYS/Physics/SUSY/Analyses/WeakProduction/DGTriggerReweight/tags/DGTriggerReweight-00-00-29"
reweightUtilsURL="$SVNOFF/PhysicsAnalysis/AnalysisCommon/ReweightUtils/tags/ReweightUtils-00-02-13"
jvfURL="$SVNOFF/Reconstruction/Jet/JetAnalysisTools/JVFUncertaintyTool/tags/JVFUncertaintyTool-00-00-04"

# Install fresh RootCore
svn co $rootCoreURL RootCore || exit $?

# Checkout CalibrationDataInterface
svn co $bTagURL/cmt $bTagURL/CalibrationDataInterface $bTagURL/Root $bTagURL/src CalibrationDataInterface || exit $?

# Additional checkouts for 2-lepton
svn co $mt2URL Mt2 || exit $?
svn co $trigURL DGTriggerReweight || exit $?
svn co $reweightUtilsURL ReweightUtils || exit $?
svn co $jvfURL JVFUncertaintyTool || exit $?

# Checkout minimal SUSYTools
mkdir -p SUSYTools/SUSYTools SUSYTools/Root SUSYTools/cmt SUSYTools/data
svn export $susyURL/SUSYTools/SUSYCrossSection.h SUSYTools/SUSYTools/SUSYCrossSection.h
svn export $susyURL/SUSYTools/BTagCalib.h SUSYTools/SUSYTools/BTagCalib.h
svn export $susyURL/Root/SUSYCrossSection.cxx SUSYTools/Root/SUSYCrossSection.cxx
svn export $susyURL/Root/BTagCalib.cxx SUSYTools/Root/BTagCalib.cxx
svn export $susyURL/cmt/Makefile.RootCore SUSYTools/cmt/Makefile.RootCore
svn co $susyURL/data SUSYTools/data || exit $?

# Modify the SUSYTools Makefile dependencies
sed -i "s/^PACKAGE_DEP.*/PACKAGE_DEP = CalibrationDataInterface/" SUSYTools/cmt/Makefile.RootCore

# Configure RootCore
cd RootCore
./configure || exit $?
source scripts/setup.sh || exit $?
cd ..

# Compile everything
$ROOTCOREDIR/scripts/build.sh || exit $?

echo "Installation successful"
echo "You may want to update SUSYTools/data to the trunk,"
echo "in which case use the refreshSUSYToolsData.sh script"
