#!/bin/bash

#
# This script will update SUSYTools/data to the latest trunk version
# It will not touch anything else within SUSYTools
#

target="svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/SUSYPhys/SUSYTools/trunk/data"
if [ $# -gt 0 ]; then target=$1; fi

dir="$ROOTCOREDIR/../SUSYTools/data"
if [ ! -d $dir ]; then 
        echo "$dir doesn't exist. Did you setup RootCore first?"
        return || exit
fi

cd $dir

currentURL=`svn info | grep "^URL" | awk '{print $2}'`
currentRev=`svn info | grep "^Revision" | awk '{print $2}'`

echo "Current version"
#svn info | grep "^URL"
#svn info | grep "^Revision"
echo "URL: $currentURL"
echo "Rev: $currentRev"

sleep .5s
echo

if [[ $target == $currentURL ]]; then
        echo "Updating to trunk"
        command="svn update"
else
        echo "Switching to"
        echo $target
        command="svn switch $target"
fi

sleep .5s
$command || return || exit

