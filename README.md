[![Build Status](https://travis-ci.org/gerbaudo/SusyNtuple.svg?branch=master)](https://travis-ci.org/gerbaudo/SusyNtuple)

SusyNtuple
----------

A RootCore package for common UCI SUSY analysis
-----------------------------------------------

For useful information on using these packages, please consult the TWiki:
https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/UCISusyNtuples

Link to code in SVN browser
https://svnweb.cern.ch/trac/atlasinst/browser/Institutes/UCIrvine/SUSYAnalysis/SusyNtuple

To checkout the package trunk  
`svn co svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/UCIrvine/SUSYAnalysis/SusyNtuple/trunk SusyNtuple`

Or the current recommended tag (get the latest tag from the TWiki)  
`svn co svn+ssh://svn.cern.ch/reps/atlasinst/Institutes/UCIrvine/SUSYAnalysis/SusyNtuple/tags/SusyNtuple-XX-YY-ZZ SusyNtuple`

To make a tag, use the script/makeTag.sh script (only for developers)  
`./scripts/makeTag.sh SusyNtuple-XX-YY-ZZ "Some message"`



Note that if you are not planning on producing SusyNt files, you do not need
to check out the SusyCommon, MultiLep, or SUSYTools packages. Instead, you
can use this install script which will checkout the minimal RootCore package
content necessary to read and analyze SusyNt. It will check out a few classes
from SUSYTools rather than the entire package with all of its dependencies.  
`source SusyNtuple/scripts/installMinimalSUSYTools.sh`

If you've already done the minimal installation above but want to update
the SUSYTools/data content (e.g., to pick up updated cross sections),
then use the following script.  
`source SusyNtuple/scripts/refreshSUSYToolsData.sh`

If you plan to produce SusyNt or just want to have the whole machinery
available to look at, then check out the SusyCommon package and use the
install instructions from there. In that case, this package will be checked
out automatically.

for examples of how to utilize this package, you can see the example cutflow
codes below, or see my personal analysis code here:
https://svnweb.cern.ch/trac/atlasinst/browser/Institutes/UCIrvine/sfarrell/SusyAna


Examples on how to run
----------------------

We have include some example codes that demonstrate the usage of SusyNt and
can reproduce the 2L and 3L cutflows.
It is recommended that you write your own analysis loopers which inherit from SusyNtAna
(or from the cutflow classes)

Currently there are 3 executables.  They all have the same command line options.
Specify SusyNt files either by specific file path, a fileList, or an input directory

Executable for simple SusyNt testing
`SusyNtTest`

Executables 2L and 3L cutflows
`Susy2LepCF`
`Susy3LepCF`

If you run with option `-h` you will get the list of command line options.


Overview of the package
-----------------------
The up-to-date doxygen documentation is available at
[this link](http://gerbaudo.github.io/SusyNtuple/doxygen-html).

Below is a brief (and somewhat outdated) description of the main classes.
- SusyDefs: typedefs, constants, globals, etc.
- SusyNt: Definitions for the SusyNt classes: particles, jets, event variables, etc.
- SusyNtObject: Manages SusyNt read/write of branches
- SusyNtTools Class with low-level functions for applying selections on SusyNt files
- SusyNtAna
  - Analysis looper class, inherits from TSelector, used to process the SusyNt
  - Also inherits from SusyNtTools and allows to easily select all baseline and signal objects
- DilTrigLogic: Class which implements the current 2-lepton trigger logic
- TrilTrigLogic: Class which implements the current 3-lepton trigger logic
- WhTruthExtractor:
  - A class to extract from d3pd the truth information for simplifiedModel_wA_noslep_WH samples
  - This should maybe be moved into SusyCommon
- SusyNtTruthAna: A looper class for analyzing only the truth particles from SusyNt
