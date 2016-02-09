[![Build Status](https://travis-ci.org/susynt/SusyNtuple.svg?branch=master)](https://travis-ci.org/susynt/SusyNtuple)

SusyNtuple
==========

For quick help:
[![Join the chat at https://gitter.im/susynt/SusyNtuple](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/susynt/SusyNtuple?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

A RootCore package for common UCI SUSY analyses
-----------------------------------------------

For useful information on using these packages, and on the most recent
ntuples please consult the TWiki:
https://twiki.cern.ch/twiki/bin/view/Atlas/SusyNtRun2

Quickstart
----------

How to set up your area:

- If you only want to **read** the ntuples: see
  [susynt/susynt-read](https://github.com/susynt/susynt-read)

- If you also want to **make** the ntuples: see
  [susynt/susynt-write](https://github.com/susynt/susynt-write)

Overview of the package
-----------------------

This package defines the objects stored in a root ntuple, as well as
the methods used to retrieve the objects and to perform the kinematic
selections. The systematic variations are also stored within the
ntuple.

The objects are described in the Doxygen documentation at
**[this link](http://susynt.github.io/SusyNtuple/doxygen-html)**.
under the tab `Classes`.

Within the event loop, the objects are selected by calling
`SusyNtAna::selectObjects()`. Analysis-specific criteria are
implemented in the `<Object>Selector` classes.

It is recommended that you write your own analysis loopers which
inherit from SusyNtAna.

Examples on how to run
----------------------

The `util` directory holds some example code demonstrating the usage
of `SusyNtAna`.

Executable for simple SusyNt testing
`SusyNtTest`

Executables 2L and 3L cutflows
`Susy2LepCF`
`Susy3LepCF`

If you run with option `-h` you will get the list of command line
options.


Obsolete links
--------------
Link to old code in SVN browser
https://svnweb.cern.ch/trac/atlasinst/browser/Institutes/UCIrvine/SUSYAnalysis/SusyNtuple

Link to old (Run 1) twiki
https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/UCISusyNtuples
