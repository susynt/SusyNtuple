[![Build Status](https://travis-ci.org/gerbaudo/SusyNtuple.svg?branch=master)](https://travis-ci.org/gerbaudo/SusyNtuple)

SusyNtuple
----------

[![Join the chat at https://gitter.im/susynt/SusyNtuple](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/susynt/SusyNtuple?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

A RootCore package for common UCI SUSY analysis
-----------------------------------------------

For useful information on using these packages, please consult the TWiki:
https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/UCISusyNtuples

Overview of the package
-----------------------
See the Doxygen documentation class description under the tab `Classes`
at *[this link](http://gerbaudo.github.io/SusyNtuple/doxygen-html)*.

TODO: add an intro page to the doxygen directory.


If you only want to *read* the ntuples: see susynt/susynt-read
If you also want to *make* the ntuples: see susynt/susynt-write


Examples on how to run
----------------------

The `util` directory holds some example code demonstrating the usage
of `SusyNt`.

It is recommended that you write your own analysis loopers which
inherit from SusyNtAna.

Executable for simple SusyNt testing
`SusyNtTest`

Executables 2L and 3L cutflows
`Susy2LepCF`
`Susy3LepCF`

If you run with option `-h` you will get the list of command line options.


Obsolete links
--------------
Link to old code in SVN browser
https://svnweb.cern.ch/trac/atlasinst/browser/Institutes/UCIrvine/SUSYAnalysis/SusyNtuple
