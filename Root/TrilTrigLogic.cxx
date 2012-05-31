#include "THnSparse.h"

#include "SusyNtuple/TrilTrigLogic.h"

using namespace std;
using namespace Susy;


/*


              ,;MMMM..                                     
           ,;:MM"MMMMM.                                   
        ,;.MM::M.MMMMMM:                                   
        ""::.;'MMMMMMMMM                                   
               "'""MMMMM;                                 
                   ':MMMM.                                 
                    'MMMM;                                 
                     :MMMM;.                               
                      MMMMMM;...                           
                      MMMMMMMMMMMMM;.;..                   
                      MMMMMMMMMMMMMMMMMMM...               
                      MMMMMM:MMMMMMMMMMMMMMM;...       ..: 
                      MMMMMM;MMMMMMMMMMMMM:MMMMMMM:MMMM:M 
                      :MMMMMM:MMMMMMMMMMMMMMM.:::;:::;;:' 
                      ':MMMMMMM:MMMM:;MM:M;.MMM:';::M:'   
                       ':MMMMMM;M;;MM;::M;;::::;MM:""     
                         'MMMMMMMM;M;:::MMMMMMMMMM"       
                          ''MMMMMMMMMMMMMMMMMMMMM"         
                             ':MMMMMMMMMMMMMMMM"'         
                               '':MMMMMMMMMMM"'           
                                  ':MMMMMM""'             
                                     .                     
                                     :                     
                                    ::                     
                               ,..;.M'                     
                              ,;;MM:'                     
                                '"'

*/




/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
TrilTrigLogic::TrilTrigLogic(SusyNtObject* nt)
{
  m_nt = nt;

  // int lumi 
  //m_lumis.resize(11);
  m_lumis[Period_B] = 11.7377; // luminosity for period B = [177986, 178109]
  m_lumis[Period_D] = 166.65;  // luminosity for period D = [179710, 180481]
  m_lumis[Period_E] = 48.7803; // luminosity for period E = [180614, 180776]
  m_lumis[Period_F] = 132.498; // luminosity for period F = [182013, 182519]
  m_lumis[Period_G] = 507.84;  // luminosity for period G = [182726, 183462]
  m_lumis[Period_H] = 259.459; // luminosity for period H = [183544, 184169]
  m_lumis[Period_I] = 337.543; // luminosity for period I = [185353, 186493]
  m_lumis[Period_J] = 226.392; // luminosity for period J = [186516, 186755]
  m_lumis[Period_K] = 590.363; // luminosity for period K = [186873, 187815]
  m_lumis[Period_L] = 1401.87; // luminosity for period L = [188902, 190343]
  m_lumis[Period_M] = 1025.62; // luminosity for period M = [190503, 191933]

  m_lumiIK = m_lumis[Period_I] + m_lumis[Period_J] + m_lumis[Period_K];
  float lumiTot = 0;
  for(uint i=0; i<Period_Num; i++) lumiTot += m_lumis[i];

  m_dbg=0;
}

/*--------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------*/
TrilTrigLogic::~TrilTrigLogic()
{
  delete rew_e20;
  delete rew_e22;
  delete rew_e22vh;
  delete rew_e12;
  delete rew_e12T;
  delete rew_e12Tvh;
  delete rew_e10;
  delete rew_mu18;
  delete rew_mu18Med;
  delete rew_mu10L_not18;
  delete rew_mu10L_not18Med;
  delete rew_mu6_not18;
  delete rew_mu6_not18Med;
}

/*--------------------------------------------------------------------------------*/
// Trigger reweight maps
/*--------------------------------------------------------------------------------*/
void TrilTrigLogic::loadTriggerMaps()
{
  // Using a modified muon map.  It would be better to have this in the common code
  TFile* eleTrigFile = new TFile("$ROOTCOREDIR/data/DGTriggerReweight/electron_maps.root");
  TFile* muoTrigFile = new TFile("$ROOTCOREDIR/data/SusyNtuple/muon_triggermaps_v2.0.root");

  rew_e20               = loadTrigWeighter(eleTrigFile, "e20_medium");
  rew_e22               = loadTrigWeighter(eleTrigFile, "e22_medium");
  rew_e22vh             = loadTrigWeighter(eleTrigFile, "e22vh_medium1");
  rew_e12               = loadTrigWeighter(eleTrigFile, "e12_medium");
  rew_e12T              = loadTrigWeighter(eleTrigFile, "e12T_medium");
  rew_e12Tvh            = loadTrigWeighter(eleTrigFile, "e12Tvh_medium");
  rew_e10               = loadTrigWeighter(eleTrigFile, "e10_medium");

  rew_mu18              = loadTrigWeighter(muoTrigFile, "mu18");
  rew_mu18Med           = loadTrigWeighter(muoTrigFile, "mu18_medium");
  rew_mu10L_not18       = loadTrigWeighter(muoTrigFile, "notmu18_mu10_loose");
  rew_mu10L_not18Med    = loadTrigWeighter(muoTrigFile, "notmu18_medium_mu10_loose");
  rew_mu6_not18         = loadTrigWeighter(muoTrigFile, "notmu18_mu6");
  rew_mu6_not18Med      = loadTrigWeighter(muoTrigFile, "notmu18_medium_mu6");

  eleTrigFile->Close();
  muoTrigFile->Close();
  delete eleTrigFile;
  delete muoTrigFile;
}

/*--------------------------------------------------------------------------------*/
APReweightND* TrilTrigLogic::loadTrigWeighter(TFile* f, TString name)
{
  TString numName = "ths_"+name+"_num";
  TString denName = "ths_"+name+"_den";
  // muon file currently contains a typo
  if (name.Contains("mu")) numName = "ths_"+name+"_nom";

  // Does this memory get cleaned up when the file closes?
  THnSparseD* num = (THnSparseD*) f->Get( numName );
  THnSparseD* den = (THnSparseD*) f->Get( denName );
  if(!num || !den){
    cout << "ERROR loading trig maps for chain " << name << endl;
    cout << "num " << num << endl;
    cout << "dun " << den << endl;
    abort();
  }
  return new APReweightND( den, num, true );
}

/*--------------------------------------------------------------------------------*/
// Get run dependent trigger chains, for data
/*--------------------------------------------------------------------------------*/
uint TrilTrigLogic::getTrigger1E(uint run)
{
  if(run < 186873) return TRIG_e20_medium;
  else if(run < 188902) return TRIG_e22_medium;
  else return TRIG_e22vh_medium1;
}
/*--------------------------------------------------------------------------------*/
uint TrilTrigLogic::getTrigger1M(uint run)
{
  if(run < 186516) return TRIG_mu18;
  else return TRIG_mu18_medium;
}
/*--------------------------------------------------------------------------------*/
uint TrilTrigLogic::getTrigger2E(uint run)
{
  if(run < 186873) return TRIG_2e12_medium;
  else if(run < 188902) return TRIG_2e12T_medium;
  else return TRIG_2e12Tvh_medium;
}
/*--------------------------------------------------------------------------------*/
uint TrilTrigLogic::getTrigger2M(uint run)
{ return TRIG_2mu10_loose; }
/*--------------------------------------------------------------------------------*/
uint TrilTrigLogic::getTriggerEM(uint run)
{ return TRIG_e10_medium_mu6; }

/*--------------------------------------------------------------------------------*/
// Trigger plateau flags
/*--------------------------------------------------------------------------------*/
void TrilTrigLogic::plateauFlags(const LeptonVector& leptons, bool& plat1E, bool& plat2E, 
                                bool& plat1M, bool& plat2M, bool& platEM)
{
  // Count objects above thresholds
  uint nE25 = 0;
  uint nE17 = 0;
  uint nE15 = 0;
  uint nM20 = 0;
  uint nM12 = 0;
  uint nM08 = 0;

  for(uint i=0; i<leptons.size(); i++){
    const Lepton* lep = leptons[i];
    float pt = lep->Pt();
    if(lep->isEle()){
      if(pt>25) nE25++;
      // Don't count 1e plateau electrons for the dilepton plateaus
      else{
        if(pt>17) nE17++;
        if(pt>15) nE15++;
      }
    }
    else{
      if(pt>20) nM20++;
      if(pt>12) nM12++;
      if(pt>8)  nM08++;
    }
  }
  plat1E = nE25 > 0;
  plat2E = nE17 > 1;
  plat1M = nM20 > 0;
  plat2M = nM12 > 1;
  platEM = nE15 > 0 && nM08 > 0;
}

/*--------------------------------------------------------------------------------*/
// Trigger matching for data
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passTriggerMatching(const LeptonVector& leptons)
{
  //cout << "check trigger" << endl;
  uint run = m_nt->evt()->run;
  DataStream stream = m_nt->evt()->stream;
  if(stream==Stream_MC) return true;

  //DataPeriod period = getDataPeriod(run);

  // Trigger chains, e.g. Trig_e20_medium
  uint trig1E = getTrigger1E(run);
  uint trig2E = getTrigger2E(run);
  uint trig1M = getTrigger1M(run);
  uint trig2M = getTrigger2M(run);
  uint trigEM = getTriggerEM(run);

  // match flags
  uint n1El   = 0;      // num ele matching to 1e trigger
  uint n2El   = 0;      // num ele matching to 2e trigger
  uint n1Mu   = 0;      // num muo matching to 1m trigger
  uint n2Mu   = 0;      // num muo matching to 2m trigger
  uint nEMu_e = 0;      // num ele matching to em trigger
  uint nEMu_m = 0;      // num muo matching to em trigger

  // plateau flags determine which chains to use
  bool plat1E, plat2E, plat1M, plat2M, platEM;
  plateauFlags(leptons, plat1E, plat2E, plat1M, plat2M, platEM);

  // loop over leptons
  for(uint i=0; i < leptons.size(); i++){
    const Lepton* lep = leptons[i];
    float pt = lep->Pt();

    // Electrons can only participate in one trigger each, 
    // because of the non-conditional efficiency maps
    if(lep->isEle()){

      // Electrons in the 1e plateau are only used for 1e trigger
      if(pt > 25){
        if(lep->matchTrig(trig1E)) n1El++;
      }

      // 2e trigger
      else if(pt>17 && plat2E){
        if(lep->matchTrig(trig2E)) n2El++;
      }

      // e-mu trigger
      else if(pt>15 && platEM){
        if(lep->matchTrig(trigEM)) nEMu_e++;
      }
    }

    // Muons can participate in more than one trigger,
    // although we must use e-mu or 2mu, not both. Not sure why, exactly
    else{

      // Muons in the 1m plateau also participate in dilepton triggers
      if(pt>20 && lep->matchTrig(trig1M)){
        n1Mu = true;
      }

      // Prioritize e-mu trigger if in plateau
      if(pt>8 && platEM){
        if(lep->matchTrig(trigEM)) nEMu_m++;
      }

      // 2mu trigger
      else if(pt>12 && plat2M){
        if(lep->matchTrig(trig2M)) n2Mu++;
      }
    }
  }

  bool pass1E = n1El>0;
  bool pass2E = n2El>1;
  bool pass1M = n1Mu>0;
  bool pass2M = n2Mu>1;
  bool passEMu = nEMu_e>0 && nEMu_m>0;

  bool passEleTrigs = pass1E || pass2E || passEMu;
  bool passMuoTrigs = pass1M || pass2M;

  if(m_dbg){
    cout << "plat1E: " << plat1E << endl;
    cout << "plat2E: " << plat2E << endl;
    cout << "plat1M: " << plat1M << endl;
    cout << "plat2M: " << plat2M << endl;
    cout << "platEM: " << platEM << endl;
    cout << "n1El:   " << n1El << endl;
    cout << "n2El:   " << n2El << endl;
    cout << "n1Mu:   " << n1Mu << endl;
    cout << "n2Mu:   " << n2Mu << endl;
    cout << "nEMu_e: " << nEMu_e << endl;
    cout << "nEMu_m: " << nEMu_m << endl;
    cout << "pass1E: " << pass1E << endl;
    cout << "pass2E: " << pass2E << endl;
    cout << "pass1M: " << pass1M << endl;
    cout << "pass2M: " << pass2M << endl;
    cout << "passEM: " << passEMu << endl;
    cout << "passEl: " << passEleTrigs << endl;
    cout << "passMu: " << passMuoTrigs << endl;
  }

  // stream dependence, avoid double counting
  if(stream==Stream_Egamma && !passEleTrigs) return false;
  if(stream==Stream_Muons && (!passMuoTrigs || passEleTrigs)) return false;

  // TEST
  //cout << "EVENT PASSES TRIGGER" << endl;

  return true;
}

/*--------------------------------------------------------------------------------*/
// Trigger reweighting for MC
/*--------------------------------------------------------------------------------*/
float TrilTrigLogic::getTriggerWeight(const LeptonVector& leptons)
{
  uint run = m_nt->evt()->run;
  DataStream stream = m_nt->evt()->stream;
  if(stream!=Stream_MC) return 1;
  McPeriod period = getMcPeriod(run);

  // plateau flags determine which chains to use
  bool plat1E, plat2E, plat1M, plat2M, platEM;
  plateauFlags(leptons, plat1E, plat2E, plat1M, plat2M, platEM);

  // Trigger weights, one per chain
  APEvtWeight w_e20(APEvtWeight::kElectron);
  APEvtWeight w_e22(APEvtWeight::kElectron);
  APEvtWeight w_e22vh(APEvtWeight::kElectron);
  APEvtWeight w_2e12(APEvtWeight::kDiElectron);
  APEvtWeight w_2e12T(APEvtWeight::kDiElectron);
  APEvtWeight w_2e12Tvh(APEvtWeight::kDiElectron);
  APEvtWeight w_e10(APEvtWeight::kElectronMO);
  APEvtWeight w_mu18(APEvtWeight::kMuon);
  APEvtWeight w_mu18Med(APEvtWeight::kMuon);
  APEvtWeight w_2mu10L_not18(APEvtWeight::kDiMuon);
  APEvtWeight w_2mu10L_not18Med(APEvtWeight::kDiMuon);
  APEvtWeight w_mu6_not18(APEvtWeight::kMuonMO);
  APEvtWeight w_mu6_not18Med(APEvtWeight::kMuonMO);

  // Loop over leptons
  for(uint iL = 0; iL < leptons.size(); iL++){

    const Lepton* lep = leptons[iL];

    // Electrons can only participate in one trigger each, 
    // because of the non-conditional efficiency maps
    if(lep->isEle()){

      const Electron* ele = (const Electron*) lep;

      // Construct electron properties
      double etaBin = getEleEtaBin(ele->clusEta);
      double pt = ele->Pt();
      if(pt >= 100.) pt = 99.5;
      double properties[] = { etaBin, pt };


      // Electrons in the 1e plateau are only used for 1e trigger
      if(pt > 25){

        w_e20.AddWeightToEvt( rew_e20->GetWeight(properties) );
        w_e22.AddWeightToEvt( rew_e22->GetWeight(properties) );
        w_e22vh.AddWeightToEvt( rew_e22vh->GetWeight(properties) );

      }

      // 2e trigger
      else if(pt>17 && plat2E){

        w_2e12.AddWeightToEvt( rew_e12->GetWeight(properties) );
        w_2e12T.AddWeightToEvt( rew_e12T->GetWeight(properties) );
        w_2e12Tvh.AddWeightToEvt( rew_e12Tvh->GetWeight(properties) );

      }

      // e-mu trigger
      else if(pt>15 && platEM){

        w_e10.AddWeightToEvt( rew_e10->GetWeight(properties) );

      }


    } // end electron

    // Muons can participate in more than one trigger,
    // although we must use e-mu or 2mu, not both. Not sure why, exactly
    else{

      const Muon* mu = (const Muon*) lep;
      float pt = mu->Pt();

      // Build muon properties
      double properties[] = { pt*1000., mu->Eta(), mu->Phi(), mu->isCombined, mu->ptcone20*1000. };

      // Muons in the 1m plateau also participate in dilepton triggers
      if(pt>20){

        w_mu18.AddWeightToEvt( rew_mu18->GetWeight(properties) );
        w_mu18Med.AddWeightToEvt( rew_mu18Med->GetWeight(properties) );

      }

      // Prioritize e-mu trigger if in plateau
      if(pt>8 && platEM){

        w_mu6_not18.AddWeightToEvt( rew_mu6_not18->GetWeight(properties) );
        w_mu6_not18Med.AddWeightToEvt( rew_mu6_not18Med->GetWeight(properties) );

      }

      // Otherwise, it contributes to the 2mu trigger
      else if(pt>12 && plat2M){

        w_2mu10L_not18.AddWeightToEvt( rew_mu10L_not18->GetWeight(properties) );
        w_2mu10L_not18Med.AddWeightToEvt( rew_mu10L_not18Med->GetWeight(properties) );

      }

    } // end muon
  } // end lepton loop

  // e-mu trigger weights
  APEvtWeight w_e10_mu6_not18    = w_e10 && w_mu6_not18;
  APEvtWeight w_e10_mu6_not18Med = w_e10 && w_mu6_not18Med;

  // Periods B-H are easy
  if(period == McPeriod_BD || period == McPeriod_EH){

    APEvtWeight w_tot = w_e20 || w_mu18 || w_2e12 || w_2mu10L_not18;
    return w_tot.GetWeight();

  }

  // Periods I-K have different triggers, so need to combine
  // them according to the lumi fractions for each period.
  else if(period == McPeriod_IK){

    APEvtWeight w_tot_I = w_e20 || w_mu18    || w_2e12  || w_2mu10L_not18    || w_e10_mu6_not18;
    APEvtWeight w_tot_J = w_e20 || w_mu18Med || w_2e12  || w_2mu10L_not18Med || w_e10_mu6_not18Med;
    APEvtWeight w_tot_K = w_e22 || w_mu18Med || w_2e12T || w_2mu10L_not18Med || w_e10_mu6_not18Med;

    double weightI = w_tot_I.GetWeight();
    double weightJ = w_tot_J.GetWeight();
    double weightK = w_tot_K.GetWeight();

    return weightI*m_lumis[Period_I]/m_lumiIK + 
           weightJ*m_lumis[Period_J]/m_lumiIK + 
           weightK*m_lumis[Period_K]/m_lumiIK;

  }

  else if(period == McPeriod_LM){

    APEvtWeight w_tot = w_e22vh || w_mu18Med || w_2e12Tvh || w_2mu10L_not18Med || w_e10_mu6_not18Med;
    return w_tot.GetWeight();

  }

  cout << "ERROR - mc period " << period << " not supported" << endl;
  abort();
  return 0;

} // getTriggerWeight

/*--------------------------------------------------------------------------------*/
// Electron eta bin, for trigger efficiency parametrization
/*--------------------------------------------------------------------------------*/
double TrilTrigLogic::getEleEtaBin(float clusEta)
{
  float eta = fabs(clusEta);
  if     (eta>0    && eta<=0.6)  {return 1.0;}
  else if(eta>0.6  && eta<=0.8)  {return 2.0;}
  else if(eta>0.8  && eta<=1.15) {return 3.0;}
  else if(eta>1.15 && eta<=1.37) {return 4.0;}
  else if(eta>1.37 && eta<=1.52) {return 5.0;}
  else if(eta>1.52 && eta<=1.81) {return 6.0;}
  else if(eta>1.81 && eta<=2.01) {return 7.0;}
  else if(eta>2.01 && eta<=2.37) {return 8.0;}
  else if(eta>2.37 && eta<=2.47) {return 9.0;}
  else {return -1;}
}
