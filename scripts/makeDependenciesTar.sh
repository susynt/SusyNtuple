#!/bin/bash

#
# Create a tar file containing the current packages we depend on.
# Assuming ROOT is already available from ROOTSYS (in particular,
# the ROOTSYS requirement comes from RootCore).
#
# Prerequisites:
# - ROOT must be available from the ROOTSYS env var (this requirement
#   comes from RootCore)
# - valid CERN.CH kerberos ticket

# davide.gerbaudo@gmail.com
# March 2014

SVNOFF="svn+ssh://svn.cern.ch/reps/atlasoff/"
SVNPHYS="svn+ssh://svn.cern.ch/reps/atlasphys/"

BASE_DIR=`pwd`
TMP_DIR="/tmp/SusyNt_dependencies_`date +%F`"
TAR_FILE="/tmp/SusyNt_dependencies.tgz"


function clean_and_create_temp_dir {
    local TMP_DIR=$1
    if [[ -d ${TMP_DIR} ]]
        then
        rm -rf ${TMP_DIR}
    fi
    mkdir -p ${TMP_DIR}
}

function checkout_packages {
    local TMP_DIR=$1
    # Tags to checkout
    rootCoreURL=${SVNOFF}"/PhysicsAnalysis/D3PDTools/RootCore/tags/RootCore-00-02-99"
    bTagURL=${SVNOFF}"/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface/tags/CalibrationDataInterface-00-03-06"
    mt2URL=${SVNPHYS}"/Physics/SUSY/Analyses/WeakProduction/Mt2/tags/Mt2-00-00-01"
    trigURL=${SVNPHYS}"/Physics/SUSY/Analyses/WeakProduction/DGTriggerReweight/tags/DGTriggerReweight-00-00-29"
    reweightUtilsURL=${SVNOFF}"/PhysicsAnalysis/AnalysisCommon/ReweightUtils/tags/ReweightUtils-00-02-13"
    jvfURL=${SVNOFF}"/Reconstruction/Jet/JetAnalysisTools/JVFUncertaintyTool/tags/JVFUncertaintyTool-00-00-04"

    cd ${TMP_DIR}    
    svn co ${rootCoreURL} RootCore
    svn co ${mt2URL} Mt2
    svn co ${trigURL} DGTriggerReweight
    svn co ${reweightUtilsURL} ReweightUtils
    svn co ${jvfURL} JVFUncertaintyTool
    # DG why is this one special?
    svn co ${bTagURL}/cmt ${bTagURL}/CalibrationDataInterface ${bTagURL}/Root ${bTagURL}/src CalibrationDataInterface
}


function checkout_SUSYTools {    
    local TMP_DIR=$1
    susyURL=${SVNOFF}"/PhysicsAnalysis/SUSYPhys/SUSYTools/tags/SUSYTools-00-03-19"
    # special treatment: only pick the minimum number of required files
    mkdir -p SUSYTools/SUSYTools SUSYTools/Root SUSYTools/cmt SUSYTools/data
    svn export ${susyURL}/SUSYTools/SUSYCrossSection.h SUSYTools/SUSYTools/SUSYCrossSection.h
    svn export ${susyURL}/SUSYTools/BTagCalib.h SUSYTools/SUSYTools/BTagCalib.h
    svn export ${susyURL}/Root/SUSYCrossSection.cxx SUSYTools/Root/SUSYCrossSection.cxx
    svn export ${susyURL}/Root/BTagCalib.cxx SUSYTools/Root/BTagCalib.cxx
    svn export ${susyURL}/cmt/Makefile.RootCore SUSYTools/cmt/Makefile.RootCore
    svn co     ${susyURL}/data SUSYTools/data
    # Modify the SUSYTools Makefile dependencies
    sed -i "s/^PACKAGE_DEP.*/PACKAGE_DEP = CalibrationDataInterface/" SUSYTools/cmt/Makefile.RootCore
}

function cleanup_temp_dir {
    rm -rf $1
}

#
# main
#

clean_and_create_temp_dir ${TMP_DIR}
cd ${TMP_DIR}
checkout_packages ${TMP_DIR}
checkout_SUSYTools ${TMP_DIR}
tar czf ${TAR_FILE} ./*
cleanup_temp_dir ${TMP_DIR}
cd ${BASE_DIR}
echo "Created dependencies tar ${TAR_FILE}"
ls -lh ${TAR_FILE}
