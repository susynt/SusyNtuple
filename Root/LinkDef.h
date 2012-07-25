#include <vector>

#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/SusyNtObject.h"

#include "SusyNtuple/ChainHelper.h"
#include "SusyNtuple/SusyNtTools.h"
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/Susy2LepCutflow.h"
#include "SusyNtuple/Susy3LepCutflow.h"
#include "SusyNtuple/TGuiUtils.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

#pragma link C++ class TGuiUtils+;
#pragma link C++ class SusyNtAna;
#pragma link C++ class Susy2LepCutflow;
#pragma link C++ class Susy3LepCutflow;

#pragma link C++ namespace Susy+;

#pragma link C++ class Susy::SusyNtObject;
#pragma link C++ class Susy::Particle+;
#pragma link C++ class Susy::Lepton+;
#pragma link C++ class Susy::Electron+;
#pragma link C++ class Susy::Muon+;
#pragma link C++ class Susy::Jet+;
#pragma link C++ class Susy::Met+;
#pragma link C++ class Susy::Event+;
#pragma link C++ class Susy::Photon+;
#pragma link C++ class Susy::Tau+;

// STL
#pragma link C++ class std::vector< Susy::Particle >+;
#pragma link C++ class std::vector< Susy::Lepton >+;
#pragma link C++ class std::vector< Susy::Electron >+;
#pragma link C++ class std::vector< Susy::Muon >+;
#pragma link C++ class std::vector< Susy::Jet >+;
#pragma link C++ class std::vector< Susy::Met >+;
#pragma link C++ class std::vector< Susy::Photon >+;

//Tools //optional to have this here ?
#pragma link C++ class SusyNtTools+;
#pragma link C++ class ChainHelper+;

#endif
