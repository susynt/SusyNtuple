#ifndef SusyNtuple_SusyNt_h
#define SusyNtuple_SusyNt_h


///  SusyNt
/**
   This header file contains definitions for the SusyNt tree structure
   The classes here were adapted from UCINtuple classes but designed to
   be more streamlined for quick analysis.  They should hold the
   minimal content for doing analysis.  If an advanced study requires
   many additional variables, it might be preferred to write a new
   derived class rather than clutter theses classes up.
*/


// this warning will be used to annoy people when we are ready to get rid of this file
// DG-2015-04-18
// #warning "you should try to include the smaller files below rather than SusyNt.h"

#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"
#include "SusyNtuple/Event.h"
#include "SusyNtuple/Electron.h"
#include "SusyNtuple/Muon.h"
#include "SusyNtuple/Photon.h"
#include "SusyNtuple/Jet.h"
#include "SusyNtuple/Tau.h"
#include "SusyNtuple/Met.h"
#include "SusyNtuple/TrackMet.h"
#include "SusyNtuple/TruthParticle.h"
#include "SusyNtuple/TruthJet.h"
#include "SusyNtuple/TruthMet.h"

#endif
