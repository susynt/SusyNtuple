#include "SusyNtuple/VarHandle.h"
#include "SusyNtuple/SusyNt.h"
#include "SusyNtuple/DilTrigLogic.h"
#include <vector>
#include <utility> // std::pair

#ifdef __CINT__

#pragma link C++ class pair<unsigned int,int>+;
#pragma link C++ class D3PDReader::VarHandleBase+;
#pragma link C++ class D3PDReader::VarHandle<vector<Susy::TruthParticle>*>+;
#pragma link C++ class D3PDReader::VarHandle<vector<Susy::TruthJet>*>+;
#pragma link C++ class D3PDReader::VarHandle<vector<Susy::TruthMet>*>+;
#pragma link C++ class D3PDReader::VarHandle<Susy::Event*>+;
#pragma link C++ class D3PDReader::VarHandle<vector<Susy::Electron>*>+;
#pragma link C++ class D3PDReader::VarHandle<vector<Susy::Muon>*>+;
#pragma link C++ class D3PDReader::VarHandle<vector<Susy::Jet>*>+;
#pragma link C++ class D3PDReader::VarHandle<vector<Susy::Photon>*>+;
#pragma link C++ class D3PDReader::VarHandle<vector<Susy::Tau>*>+;
#pragma link C++ class D3PDReader::VarHandle<vector<Susy::Met>*>+;

#pragma link C++ class std::vector<Susy::Lepton*>+;
#pragma link C++ class DilTrigLogic+;
#endif
