#ifndef MCTRUTHCLASSIFIER_MCTRUTHCLASSIFIERDEFS_H
#define MCTRUTHCLASSIFIER_MCTRUTHCLASSIFIERDEFS_H
/********************************************************************

NAME:     MCTruthClassifierDefs.h 
PACKAGE:  atlasoff/PhysicsAnalysis/MCTruthClassifier

AUTHORS:  O. Fedin
CREATED:  Oct 2007

PURPOSE:  
          
Updated:  
********************************************************************/
#include <vector>
#include <string>
//
#define PARTICLETYPES   23
#define PARTICLEORIGIN  45
#define PARTICLEOUTCOME 12

namespace MCTruthPartClassifier {

  enum ParticleType {

   Unknown         =  0,  
   UnknownElectron =  1,  
   IsoElectron     =  2,
   NonIsoElectron  =  3,
   BkgElectron     =  4,
   UnknownMuon     =  5,  
   IsoMuon         =  6,
   NonIsoMuon      =  7,
   BkgMuon         =  8,
   UnknownTau      =  9,  
   IsoTau          =  10,
   NonIsoTau       =  11,
   BkgTau          =  12,
   UnknownPhoton   =  13,  
   IsoPhoton       =  14,
   NonIsoPhoton    =  15,
   BkgPhoton       =  16,
   Hadron          =  17,
   Neutrino        =  18,
   NuclFrag        =  19,
   NonPrimary      =  20,
   GenParticle     =  21,
   SUSYParticle    =  22 
  };

  enum ParticleOrigin {

    NonDefined    = 0,

    SingleElec    = 1,
    SingleMuon    = 2,
    SinglePhot    = 3,
    SingleTau     = 4,


    PhotonConv    = 5,
    DalitzDec     = 6,
    ElMagProc     = 7,
    Mu            = 8,
    TauLep        = 9,
    top           = 10,
    QuarkWeakDec  = 11,
    WBoson        = 12,
    ZBoson        = 13,
    Higgs         = 14,
    HiggsMSSM     = 15,
    HeavyBoson    = 16,
    WBosonLRSM    = 17,
    NuREle        = 18,
    NuRMu         = 19,
    NuRTau        = 20,
    LQ            = 21,

    SUSY          = 22,
    LightMeson    = 23,
    StrangeMeson  = 24,
    CharmedMeson  = 25,
    BottomMeson   = 26,
    CCbarMeson    = 27,
    JPsi          = 28,
    BBbarMeson    = 29,  
    LightBaryon   = 30,
    StrangeBaryon = 31,
    CharmedBaryon = 32, 
    BottomBaryon  = 33,
    PionDecay     = 34,
    KaonDecay     = 35,
    BremPhot      = 36,
    PromptPhot    = 37,
    UndrPhot      = 38,
    ISRPhot       = 39,
    FSRPhot       = 40, 
    NucReact      = 41,
    PiZero        = 42,
    DiBoson       = 43,
    ZorHeavyBoson = 44
  };

  enum ParticleOutCome {
    NonDefinedOutCome  =  0,  
    UnknownOutCome     =  1,  
    UnConverted        =  2,
    Converted          =  3,
    NonInteract        =  4,
    NuclInteraction    =  5,
    ElectrMagInter     =  6,
    DecaytoElectron    =  7,
    DecaytoMuon        =  8,
    OneProng           =  9,
    ThreeProng         = 10,
    FiveProng          = 11
   };



 struct ParticleDef {
  
  /** the vector of patricle types */
  std::vector<std::string> sParticleType;   
  /** the vector of patricle origin */
  std::vector<std::string> sParticleOrigin;   
  /** the vector of patricle out come */
  std::vector<std::string> sParticleOutCome;   
   
  /**Default constructor*/
  ParticleDef()
  {
   sParticleType.reserve(PARTICLETYPES);

   sParticleType.push_back("Unknown"); 
   sParticleType.push_back("UnknownElectron"); 
   sParticleType.push_back("IsoElectron"); 
   sParticleType.push_back("NonIsoElectron");
   sParticleType.push_back("BkgElectron");
   sParticleType.push_back("UnknownMuon");
   sParticleType.push_back("IsoMuon");
   sParticleType.push_back("NonIsoMuon");
   sParticleType.push_back("BkgMuon");
   sParticleType.push_back("UnknownTau");
   sParticleType.push_back("IsoTau");
   sParticleType.push_back("NonIsoTau");
   sParticleType.push_back("BkgTau");
   sParticleType.push_back("UnknownPhoton");
   sParticleType.push_back("IsoPhoton");
   sParticleType.push_back("NonIsoPhoton");
   sParticleType.push_back("BkgPhoton");
   sParticleType.push_back("Hadron");
   sParticleType.push_back("Neutrino");
   sParticleType.push_back("NuclFrag");
   sParticleType.push_back("NonPrimary");
   sParticleType.push_back("GenParticle"); 
   sParticleType.push_back("SUSYParticle"); 

   sParticleOrigin.reserve(PARTICLEORIGIN);
   sParticleOrigin.push_back("NonDefined");
   sParticleOrigin.push_back("SingleElec");
   sParticleOrigin.push_back("SingleMuon");
   sParticleOrigin.push_back("SinglePhot");
   sParticleOrigin.push_back("SingleTau");
   sParticleOrigin.push_back("PhotonConv");
   sParticleOrigin.push_back("DalitzDec");
   sParticleOrigin.push_back("ElMagProc");
   sParticleOrigin.push_back("Mu");
   sParticleOrigin.push_back("TauLep");
   sParticleOrigin.push_back("top");
   sParticleOrigin.push_back("QuarkWeakDec");
   sParticleOrigin.push_back("WBoson");
   sParticleOrigin.push_back("ZBoson");
   sParticleOrigin.push_back("Higgs");
   sParticleOrigin.push_back("HiggsMSSM");
   sParticleOrigin.push_back("WZMSSM");
   sParticleOrigin.push_back("WBosonLRSM");
   sParticleOrigin.push_back("NuREle");
   sParticleOrigin.push_back("NuRMu ");
   sParticleOrigin.push_back("NuRTau");
   sParticleOrigin.push_back("LQ");
   sParticleOrigin.push_back("SUSY");
   sParticleOrigin.push_back("LightMeson");
   sParticleOrigin.push_back("StrangeMeson");
   sParticleOrigin.push_back("CharmedMeson");
   sParticleOrigin.push_back("BottomMeson");
   sParticleOrigin.push_back("CCbarMeson");
   sParticleOrigin.push_back("JPsi");
   sParticleOrigin.push_back("BBbarMeson");
   sParticleOrigin.push_back("LightBaryon");
   sParticleOrigin.push_back("StrangeBaryon");
   sParticleOrigin.push_back("CharmedBaryon");
   sParticleOrigin.push_back("BottomBaryon");
   sParticleOrigin.push_back("PionDecay");
   sParticleOrigin.push_back("KaonDecay");
   sParticleOrigin.push_back("BremPhot");
   sParticleOrigin.push_back("PromptPhot");
   sParticleOrigin.push_back("UndrPhot");
   sParticleOrigin.push_back("ISRPhot");
   sParticleOrigin.push_back("FSRPhot");
   sParticleOrigin.push_back("NucReact");
   sParticleOrigin.push_back("PiZero");
   sParticleOrigin.push_back("DiBoson");
   sParticleOrigin.push_back("ZorHeavyBoson");

   sParticleOutCome.reserve(PARTICLEOUTCOME);
   sParticleOutCome.push_back("NonDefinedOutCome");
   sParticleOutCome.push_back("UnknownOutCome");
   sParticleOutCome.push_back("UnConverted");
   sParticleOutCome.push_back("Converted");
   sParticleOutCome.push_back("NonInteract");
   sParticleOutCome.push_back("NuclInteraction");
   sParticleOutCome.push_back("ElectrMagInter");
   sParticleOutCome.push_back("DecaytoElectron");
   sParticleOutCome.push_back("DecaytoMuon");
   sParticleOutCome.push_back("OneProng");
   sParticleOutCome.push_back("ThreeProng");
   sParticleOutCome.push_back("FiveProng");


  }
       
 };
    
}

#endif // MCTRUTHCLASSIFIER_MCTRUTHCLASSIFIERDEFS_H 
