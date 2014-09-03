#include "THnSparse.h"

#include "SusyNtuple/TrilTrigLogic.h"

using namespace std;
using namespace Susy;



/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
TrilTrigLogic::TrilTrigLogic()
{
  m_accOnly = false;
  m_dbg = 0;
}

/*--------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------*/
TrilTrigLogic::~TrilTrigLogic()
{
}

/*--------------------------------------------------------------------------------*/
// Trigger reweight maps
/*--------------------------------------------------------------------------------*/
void TrilTrigLogic::loadTriggerMaps()
{
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
// Event trigger requirement without object matching
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passEventTrigger(const Event* evt)
{
  // Take logical OR of the following triggers
 long long mask = TRIG_e24vhi_medium1 |
                      TRIG_2e12Tvh_loose1 |
                      TRIG_e24vh_medium1_e7_medium1 |
                      TRIG_mu24i_tight |
                      TRIG_2mu13 |
                      TRIG_mu18_tight_mu8_EFFS |
                      TRIG_e12Tvh_medium1_mu8 |
                      TRIG_mu18_tight_e7_medium1 |
		      TRIG_e18vh_medium1_2e7T_medium1 |
		      TRIG_3mu6 |
		      TRIG_mu18_tight_2mu4_EFFS |
		      TRIG_2e7T_medium1_mu6 |
		      TRIG_e7T_medium1_2mu6 |
		      TRIG_xe80_tclcw_loose |
		      TRIG_j110_a4tchad_xe90_tclcw_loose |
		      TRIG_j80_a4tchad_xe100_tclcw_loose |
		      TRIG_j80_a4tchad_xe70_tclcw_dphi2j45xe10 |
		      TRIG_mu4T_j65_a4tchad_xe70_tclcw_veryloose |
		      TRIG_2mu4T_xe60_tclcw |
		      TRIG_e24vh_medium1_EFxe35_tclcw |
	   	      TRIG_mu24_j65_a4tchad_EFxe40_tclcw;
  return evt->passTrig(mask, false);
}
/*--------------------------------------------------------------------------------*/
// Trigger matching for data
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passTriggerMatching(const LeptonVector& leptons, const TauVector& taus, const JetVector& jets, const Met* met,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs, bool useJetMetTrigs, bool useLepMetTrigs)
{
	DataStream stream = evt->stream;

	// Temporary 2012 prescription
	// Take the inclusive OR of all triggers for which the plateau is satisfied
	// For the single lepton triggers, only consider the leading lepton (safe for MM)

	// Need to keep track of how many leptons match to each trigger
	uint n1E = 0;         // number of electrons matching to single electron trigger
	uint n1M = 0;         // number of muons matching to single muon trigger

	// We have 2 di-electron triggers and 2 di-muon triggers...
	// For the symmetric dilepton triggers, it's easy
	uint nSym2E = 0;      // number of electrons matching to symm 2 electron trigger
	uint nSym2M = 0;      // number of muons matching to symm 2 muon trigger

	// For the asymmetric dilepton triggers, need to make sure to match both legs.
	// Or at least, match the best we can with the information available!
	uint nAsym2E     = 0; // number of electrons matching e24vh_medium1_e7_medium1
	uint nAsym2E_e24 = 0; // number of electrons matching e24 piece of e24vh_medium1_e7_medium1
	uint nAsym2M     = 0; // number of muons matching mu18_tight_mu8_EFFS
	uint nAsym2M_m18 = 0; // number of muons matching mu18 piece of mu18_tight_mu8_EFFS

	// For the e-mu and mu-e triggers, need to make sure to match both legs
	// Or at least, match the best we can with the information available!
	uint nEM_e = 0;       // number of electrons matching e12 piece of e12Tvh_medium1_mu8
	uint nEM_m = 0;       // number of muons matching mu8 piece of e12Tvh_medium1_mu8
	uint nME_e = 0;       // number of electrons matching e7 piece of mu18_tight_e7_medium1
	uint nME_m = 0;       // number of muons matching mu18 piece of mu18_tight_e7_medium1

	// For the trilep triggers, need to make sure to match three legs
	uint nEEM_e = 0;     // number of electrons matching e7T piece of 2e7T_medium1_mu6
	uint nEEM_m = 0;     // number of muons matching mu6 piece of 2e7T_medium1_mu6
	uint nEMM_e = 0;     // number of electrons matching e7T piece of e7T_medium1_2mu6
	uint nEMM_m = 0;     // number of muons matching mu6 piece of e7T_medium1_2mu6
	uint nMMM_m18 = 0;   // number of muons matching mu18 piece of mu18_tight_2mu4_EFFS
	uint nMMM_m4 = 0;    // number of muons matching mu4 piece of mu18_tight_2mu4_EFFS
	uint nEEE_e18 = 0;   // number of electrons matching e18vh piece of e18vh_medium1_2e7T_medium1
	uint nEEE_e7 = 0;    // number of electrons matching e7T piece of e18vh_medium1_2e7T_medium1
	uint nMMM_m6 = 0;    // number of muons matching mu6 piece of 3mu6

	// For the jet+MET triggers
	uint nJ80XE100=0;
	uint nJ80XE70DPhi=0;
	uint nXE80=0;
	uint nJ110XE90=0;
	// For the lep+MET triggers
	uint nM4X_m =0; // number of muons matching EF_2mu4T_xe60_tclcw
	uint nM8X_m =0; // number of muons matching EF_2mu8_EFxe40(_wMu)_tclcw
	uint nE24X_e = 0; // number of electrons matching EF_e24vh_medium1_EFxe35_tclcw
	uint nMJX70 = 0; // number of muons matching EF_mu4T_j65_a4tchad_xe70_tclcw_veryloose
	uint nMJX40 = 0; // number of muons matching EF_mu24_j65_a4tchad_EFxe40(_wMu)_tclcw
	// lepmet
	bool passMM4X = false;
	bool passMM8X = false;
	bool passE24X = false;
	bool passMJX70 = false;
	bool passMJX40 = false;
	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchEle(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.47, m_accOnly)
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)
#define matchLepMetEle(mask, ptMin, metMin) matchLepMetTrigger(lep, met, mask, ptMin, metMin, 2.47, m_accOnly)
#define matchLepMetMu(mask, ptMin, metMin) matchLepMetTrigger(lep, met, mask, ptMin, metMin, 2.4, m_accOnly)
#define matchLepJetMetEle(mask, ptMin, metMin, jetptMin) matchLepJetMetTrigger(lep, jets, met, mask,ptMin, metMin, jetptMin, 2.47, m_accOnly)
#define matchLepJetMetMu(mask, ptMin, metMin, jetptMin) matchLepJetMetTrigger(lep, jets, met, mask,ptMin, metMin, jetptMin, 2.4, m_accOnly)
	// Electrons
		if(lep->isEle()){
			// If leading lepton, check isolated trigger
			//if(i==0 && matchLepTrigger(lep, TRIG_e24vhi_medium1, 25, 2.47, m_accOnly)) n1E++;
			if(i==0 && matchEle(TRIG_e24vhi_medium1, 25)) n1E++;
			if(useDilepTrigs){
				// 2e symmetric trigger
				if(matchEle(TRIG_2e12Tvh_loose1, 14)) nSym2E++;
				// 2e asymmetric trigger
				if(matchEle(TRIG_e24vh_medium1_e7_medium1, 10)){
					nAsym2E++;
					if(matchEle(TRIG_e24vh_medium1, 25)) nAsym2E_e24++;
				}
				// e-mu trigger
				if(matchEle(TRIG_e12Tvh_medium1_mu8, 14)) nEM_e++;
				// mu-e trigger (this is the funny one)
				if(matchEle(TRIG_e7_medium1, 10)) nME_e++;
			}
			if(useTrilepTrigs){
				// EEM
				if(matchEle(TRIG_2e7T_medium1_mu6, 9)) nEEM_e++;
				// EMM
				if(matchEle(TRIG_e7T_medium1_2mu6, 9)) nEMM_e++;
				// EEE
				if(matchEle(TRIG_e18vh_medium1_2e7T_medium1, 20)) nEEE_e18++;
				if(matchEle(TRIG_e18vh_medium1_2e7T_medium1, 9)) nEEE_e7++;
			}
			if(useLepMetTrigs){
				//E24X
				if(matchLepMetEle(TRIG_e24vh_medium1,25,75 )) nE24X_e++;

			}
		}

		// Muons
		else{
			// If leading lepton, check isolated trigger
			//if(i==0 && matchLepTrigger(lep, TRIG_mu24i_tight, 25, m_accOnly)) n1M++;
			if(i==0 && matchMu(TRIG_mu24i_tight, 25)) n1M++;
			if(useDilepTrigs){
				// 2m symmetric trigger
				if(matchMu(TRIG_2mu13, 14)) nSym2M++;
				// 2m asymmetric trigger
				if(matchMu(TRIG_mu18_tight_mu8_EFFS, 8)){
					nAsym2M++;
					if(matchMu(TRIG_mu18_tight, 18)) nAsym2M_m18++;
				}
				// mu-e trigger
				if(matchMu(TRIG_mu18_tight_e7_medium1, 18)) nME_m++;
				// e-mu trigger
				if(matchMu(TRIG_mu8, 8)) nEM_m++;
			}
			if(useTrilepTrigs){
				// EEM
				if(matchMu(TRIG_mu6, 7)) nEEM_m++;
				// EMM
				if(matchMu(TRIG_mu6, 7)) nEMM_m++;
				// MMM
				if(matchMu(TRIG_mu18_tight_2mu4_EFFS, 19)) nMMM_m18++;
				if(matchMu(TRIG_mu18_tight_2mu4_EFFS, 5)) nMMM_m4++;
				if(matchMu(TRIG_mu6, 7)) nMMM_m6++;
			}
			if(useLepMetTrigs){
				//MMXE
				if(matchLepMetMu(TRIG_mu4T,5, 100)) nM4X_m++;
				if(matchLepMetMu(TRIG_mu8, 8, 80)) nM8X_m++;
				//MJXE
				if(matchLepJetMetMu(TRIG_mu4T, 5, 110, 105)) nMJX70++;
				if(matchLepJetMetMu(TRIG_mu24,25,80,105)) nMJX40++;

			}

		}

		// Cleanup preprocessor shorthand
#undef matchEle
#undef matchMu
#undef matchLepMetEle
#undef matchLepMetMu
#undef matchLepJetMetEle
#undef matchLepJetMetMu

	} // lepton loop

		//---------------Jet+MET Triggers---------------//
	  double njet120=0;
	  double njet150=0;
	  bool passDPhi = false;
	if(jets.size()>=2){
		if(jets[0]->Pt()>45 && jets[1]->Pt()>45){
			double dphij1met = fabs(jets[0]->DeltaPhi(met->lv()));
	 		double dphij2met = fabs(jets[1]->DeltaPhi(met->lv()));
			double minDPhi = dphij1met;
			if(dphij2met<minDPhi) minDPhi = dphij2met;
			if(minDPhi>1.0) {
				passDPhi = true;
			}
		}
	}
	for(uint i=0; i< jets.size(); i++) {
		const Jet* jet = jets[i];
		if(jet->Pt()>120) njet120++;
		if(jet->Pt()>150) njet150++;

}
	if(useJetMetTrigs){
		if((met->Et>120) && (evt->passTrig(TRIG_xe80_tclcw_loose))) nXE80++;
		if((njet120>0) && (met->Et>140) && (evt->passTrig(TRIG_j80_a4tchad_xe100_tclcw_loose))) nJ80XE100++;
		if((njet150>0) &&(met->Et>130) && (evt->passTrig(TRIG_j110_a4tchad_xe90_tclcw_loose))) nJ110XE90++;
		if((njet120>0) && (met->Et>110) && (passDPhi) && (evt->passTrig(TRIG_j80_a4tchad_xe70_tclcw_dphi2j45xe10))) nJ80XE70DPhi++;
}
	bool pass1E = n1E>0;
	bool passSym2E = nSym2E>1;
	bool passAsym2E = nAsym2E>1 && nAsym2E_e24>0;
	bool passEM = nEM_e>0 && nEM_m>0;
	bool pass1M = n1M>0;
	bool passSym2M = nSym2M>1;
	bool passAsym2M = nAsym2M>1 && nAsym2M_m18>0;
	bool passME = nME_e>0 && nME_m>0;
	// trilep
	bool passEEM = nEEM_e>1 && nEEM_m>0;
	bool passEMM = nEMM_e>0 && nEMM_m>1;
	bool passMMM_18_4 = nMMM_m18>0 && nMMM_m4>1;
	bool passEEE = nEEE_e18>0 && nEEE_e7>1;
	bool passMMM_6 = nMMM_m6>2;

	//jet met
	bool passJ80XE100 = nJ80XE100>0;
	bool passXE80 = nXE80>0;
	bool passJ110XE90 = nJ110XE90>0;
	bool passJ80XE70DPhi = nJ80XE70DPhi>0;
	//lep met
	if((nM4X_m>1) && (evt->passTrig(TRIG_2mu4T_xe60_tclcw))) passMM4X = true;
	if((nM8X_m>1) && (evt->passTrig(TRIG_2mu8_EFxe40_tclcw))) passMM8X = true;
        if((nMJX70>0) && (evt->passTrig(TRIG_mu4T_j65_a4tchad_xe70_tclcw_veryloose))) passMJX70 = true;
	if((nMJX40>0) && (evt->passTrig(TRIG_mu24_j65_a4tchad_EFxe40_tclcw))) passMJX40 = true;
	if((nE24X_e>0) && (evt->passTrig(TRIG_e24vh_medium1_EFxe35_tclcw))) passE24X=true;


	bool passEgamma = pass1E || passSym2E || passAsym2E || passEM || passEEM || passEMM || passEEE || passE24X;
	bool passMuons  = pass1M || passSym2M || passAsym2M || passME || passMMM_18_4 || passMMM_6|| passMM8X || passMM4X || passMJX70 ||  passMJX40;

	 bool passJetMet = passJ80XE100 || passXE80 || passJ110XE90 || passJ80XE70DPhi;
	// Stream dependence to avoid double counting
	if(stream==Stream_Egamma && !passEgamma) return false;
	if(stream==Stream_Muons && (!passMuons || passEgamma)) return false;
	if(stream==Stream_MC && !passEgamma && !passMuons && !passJetMet) return false;
	if(stream==Stream_JetTauEtmiss && (!passJetMet || passEgamma || passMuons)) return false;
	//cout << endl << "EVENT PASSES TRIGGER" << endl;

	return true;
}
/*--------------------------------------------------------------------------------*/
// Event Trigger requirement without object matching
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passEvent_xe80_tclcw_loose(const Event* evt, const Met* met)
{
	bool passjetTrig = false;
	long long metTrigMask = TRIG_xe80_tclcw_loose;
	if((met->Et>120) && (evt->passTrig(metTrigMask))) passjetTrig = true;
	return passjetTrig;

}
/*------------------------------------------------------------------------------------------------------------------------*/
bool TrilTrigLogic::passEvent_j110_a4tchad_xe90_tclcw_loose(const Event* evt, const Met* met, const JetVector& jets)
{
	bool passjetTrig = false;
	double njet150 = 0;
	long long jetTrigMask = TRIG_j110_a4tchad_xe90_tclcw_loose;
	for(uint i=0; i< jets.size(); i++) {
		const Jet* jet = jets[i];
		if(jet->Pt()>150) njet150++;
	}
	if((njet150>0) && (met->Et>130) && (evt->passTrig(jetTrigMask))) passjetTrig= true;
	return passjetTrig;
}
/*------------------------------------------------------------------------------------------------------------------------*/
bool TrilTrigLogic::passEvent_2e7T_medium1_mu6(const Event* evt, const LeptonVector& leptons)
{
	bool passEMTrig = false;
	double n_em = 0;
	double n_me = 0;
	long long emTrigMask = TRIG_2e7T_medium1_mu6;
	for(uint i=0; i < leptons.size(); i++){
		const Lepton* lep = leptons[i];
		if(lep->isEle()){
			if(lep->Pt()>9) n_em++;
		}
		else if(lep->isMu()){
			if(lep->Pt()>7) n_me++;
		}
	}
	if((n_em>1) && (n_me>0) && (evt->passTrig(emTrigMask))) passEMTrig = true;

	return passEMTrig;
}
/*------------------------------------------------------------------------------------------------------------------------*/
bool TrilTrigLogic::passEvent_e7T_medium1_2mu6(const Event* evt, const LeptonVector& leptons)
{
	bool passEMTrig = false;
	double n_em = 0;
	double n_me = 0;
	long long emTrigMask = TRIG_e7T_medium1_2mu6;
	for(uint i=0; i < leptons.size(); i++){
		const Lepton* lep = leptons[i];
		if(lep->isEle()){
			if(lep->Pt()>9) n_em++;
		}
		else if(lep->isMu()){
			if(lep->Pt()>7) n_me++;
		}
	}
	if((n_em>0) && (n_me>1) && (evt->passTrig(emTrigMask))) passEMTrig = true;
	return passEMTrig;
}

/*------------------------------------------------------------------------------------------------------------------------*/
bool TrilTrigLogic::passEvent_j80_a4tchad_xe100_tclcw_loose(const Event* evt, const Met* met, const JetVector& jets)
{
	bool passjetTrig = false;
	double njet120 = 0;
	long long jetTrigMask = TRIG_j80_a4tchad_xe100_tclcw_loose;
	for(uint i=0; i< jets.size(); i++) {
		const Jet* jet = jets[i];
		if(jet->Pt()>120) njet120++;
	}
	if((njet120>0) && (met->Et>140) && (evt->passTrig(jetTrigMask))) passjetTrig= true;
	return passjetTrig;
}

/*------------------------------------------------------------------------------------------------------------------------*/
bool TrilTrigLogic::passEvent_j80_a4tchad_xe70_tclcw_dphi2j45xe10(const Event* evt, const Met* met, const JetVector& jets)
{
	bool passjetTrig = false;
	double njet120 = 0;
	long long jetTrigMask = TRIG_j80_a4tchad_xe70_tclcw_dphi2j45xe10;
	bool passDPhi = false;

	if(jets.size()>=2){
		if(jets[0]->Pt()>45 && jets[1]->Pt()>45){
			double dphij1met = fabs(jets[0]->DeltaPhi(met->lv()));
			double dphij2met = fabs(jets[1]->DeltaPhi(met->lv()));
			double minDPhi = dphij1met;
			if(dphij2met<minDPhi) minDPhi = dphij2met;
			if(minDPhi>1.0) {
				passDPhi = true;
			}
		}
	}

	for(uint i=0; i< jets.size(); i++) {
		const Jet* jet = jets[i];
		if(jet->Pt()>120) njet120++;
	}
	if((njet120>0) && (met->Et>110) && (passDPhi) && (evt->passTrig(jetTrigMask))) passjetTrig= true;
	return passjetTrig;
}
/*--------------------------------------------------------------------------------*/
// Trigger matching for data
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passTriggerMatching_e24vhi_medium1(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;
	// Need to keep track of how many leptons match to each trigger
	uint n1E = 0;         // number of electrons matching to single electron trigger
	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchEle(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.47, m_accOnly)

		// Electrons
		if(lep->isEle()){
			if(i==0 && matchEle(TRIG_e24vhi_medium1, 25)) n1E++;
		}

		// Cleanup preprocessor shorthand
#undef matchEle

	} // lepton loop

	bool pass1E = n1E>0;

	bool passEgamma = pass1E;
	// Stream dependence to avoid double counting
	if(stream==Stream_Egamma && !passEgamma) return false;
	if(stream==Stream_MC && !passEgamma ) return false;
	return pass1E;
}

bool TrilTrigLogic::passTriggerMatching_mu24i_tight(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;

	uint n1M = 0;         // number of muons matching to single muon trigger

	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)


		// Muons
		if(lep->isMu()){
			// If leading lepton, check isolated trigger
			//if(i==0 && matchLepTrigger(lep, TRIG_mu24i_tight, 25, m_accOnly)) n1M++;
			if(i==0 && matchMu(TRIG_mu24i_tight, 25)) n1M++;
		}

		// Cleanup preprocessor shorthand
#undef matchMu

	} // lepton loop

	bool pass1M = n1M>0;
	bool passMuons  = pass1M;
	if(stream==Stream_Muons && !passMuons ) return false;
	if(stream==Stream_MC && !passMuons ) return false;

	return pass1M;
}

bool TrilTrigLogic::passTriggerMatching_mu18_tight_e7_medium1(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;
	uint nME_e = 0;       // number of electrons matching e7 piece of mu18_tight_e7_medium1
	uint nME_m = 0;       // number of muons matching mu18 piece of mu18_tight_e7_medium1

	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchEle(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.47, m_accOnly)
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)

		// Electrons
		if(lep->isEle()){
			if(useDilepTrigs){
				if(matchEle(TRIG_e7_medium1, 10)) nME_e++;
			}
		}

		// Muons
		else{
			if(useDilepTrigs){
				if(matchMu(TRIG_mu18_tight_e7_medium1, 18)) nME_m++;
			}
		}

		// Cleanup preprocessor shorthand
#undef matchEle
#undef matchMu

	} // lepton loop

	bool passME = nME_e>0 && nME_m>0;
	// trilep


	bool passMuons  =passME;

	// Stream dependence to avoid double counting
	if(stream==Stream_Muons && !passMuons ) return false;
	if(stream==Stream_MC && !passMuons ) return false;

	// Event passes trigger!
	//cout << endl << "EVENT PASSES TRIGGER" << endl;

	return passME;
}

bool TrilTrigLogic::passTriggerMatching_e12Tvh_medium1_mu8(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;

	// For the e-mu and mu-e triggers, need to make sure to match both legs
	// Or at least, match the best we can with the information available!
	uint nEM_e = 0;       // number of electrons matching e12 piece of e12Tvh_medium1_mu8
	uint nEM_m = 0;       // number of muons matching mu8 piece of e12Tvh_medium1_mu8

	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchEle(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.47, m_accOnly)
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)

		// Electrons
		if(lep->isEle()){
			if(useDilepTrigs){
				// e-mu trigger
				if(matchEle(TRIG_e12Tvh_medium1_mu8, 14)) nEM_e++;
			}
		}

		// Muons
		else{
			if(useDilepTrigs){
				if(matchMu(TRIG_mu8, 8)) nEM_m++;
			}
		}

		// Cleanup preprocessor shorthand
#undef matchEle
#undef matchMu

	} // lepton loop

	bool passEM = nEM_e>0 && nEM_m>0;


	bool passEgamma = passEM;
	// Stream dependence to avoid double counting
	if(stream==Stream_Egamma && !passEgamma) return false;
	if(stream==Stream_MC && !passEgamma ) return false;

	return passEM;
}

bool TrilTrigLogic::passTriggerMatching_mu18_tight_mu8_EFFS(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;
	uint nAsym2M     = 0; // number of muons matching mu18_tight_mu8_EFFS
	uint nAsym2M_m18 = 0; // number of muons matching mu18 piece of mu18_tight_mu8_EFFS

	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)
		// Muons
		if(lep->isMu()){
			if(useDilepTrigs){
				// 2m asymmetric trigger
				if(matchMu(TRIG_mu18_tight_mu8_EFFS, 8)){
					nAsym2M++;
					if(matchMu(TRIG_mu18_tight, 18)) nAsym2M_m18++;
				}
			}
		}

		// Cleanup preprocessor shorthand
#undef matchMu

	} // lepton loop

	bool passAsym2M = nAsym2M>1 && nAsym2M_m18>0;

	bool passMuons  = passAsym2M;
	// Stream dependence to avoid double counting
	if(stream==Stream_Muons && !passMuons) return false;
	if(stream==Stream_MC && !passMuons ) return false;

	return passAsym2M;
}

bool TrilTrigLogic::passTriggerMatching_e24vh_medium1_e7_medium1(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;

	// For the asymmetric dilepton triggers, need to make sure to match both legs.
	// Or at least, match the best we can with the information available!
	uint nAsym2E     = 0; // number of electrons matching e24vh_medium1_e7_medium1
	uint nAsym2E_e24 = 0; // number of electrons matching e24 piece of e24vh_medium1_e7_medium1

	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchEle(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.47, m_accOnly)

		// Electrons
		if(lep->isEle()){
			if(useDilepTrigs){
				if(matchEle(TRIG_e24vh_medium1_e7_medium1, 10)){
					nAsym2E++;
					if(matchEle(TRIG_e24vh_medium1, 25)) nAsym2E_e24++;
				}
			}
		}

		// Cleanup preprocessor shorthand
#undef matchEle

	} // lepton loop

	bool passAsym2E = nAsym2E>1 && nAsym2E_e24>0;

	bool passEgamma =passAsym2E;


	// Stream dependence to avoid double counting
	if(stream==Stream_Egamma && !passEgamma) return false;
	if(stream==Stream_MC && !passEgamma ) return false;

	return passAsym2E;
}

bool TrilTrigLogic::passTriggerMatching_2e12Tvh_loose1(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;

	// We have 2 di-electron triggers and 2 di-muon triggers...
	// For the symmetric dilepton triggers, it's easy
	uint nSym2E = 0;      // number of electrons matching to symm 2 electron trigger
	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchEle(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.47, m_accOnly)

		// Electrons
		if(lep->isEle()){
			if(useDilepTrigs){
				// 2e symmetric trigger
				if(matchEle(TRIG_2e12Tvh_loose1, 14)) nSym2E++;
			}
		}
		// Cleanup preprocessor shorthand
#undef matchEle

	} // lepton loop

	bool passSym2E = nSym2E>1;
	bool passEgamma = passSym2E;

	// Stream dependence to avoid double counting
	if(stream==Stream_Egamma && !passEgamma) return false;
	if(stream==Stream_MC && !passEgamma ) return false;

	return passSym2E;
}

bool TrilTrigLogic::passTriggerMatching_2mu13(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;
	// We have 2 di-electron triggers and 2 di-muon triggers...
	// For the symmetric dilepton triggers, it's easy
	uint nSym2M = 0;      // number of muons matching to symm 2 muon trigger

	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)

		if(lep->isMu()){
			if(useDilepTrigs){
				// 2m symmetric trigger
				if(matchMu(TRIG_2mu13, 14)) nSym2M++;
			}
		}

		// Cleanup preprocessor shorthand
#undef matchMu

	} // lepton loop

	bool passSym2M = nSym2M>1;
	bool passMuons  = passSym2M;
	// Stream dependence to avoid double counting
	if(stream==Stream_Muons && !passMuons) return false;
	if(stream==Stream_MC && !passMuons ) return false;

	return passSym2M;
}

bool TrilTrigLogic::passTriggerMatching_e18vh_medium1_2e7T_medium1(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;
	// For the trilep triggers, need to make sure to match three legs
	uint nEEE_e18 = 0;   // number of electrons matching e18vh piece of e18vh_medium1_2e7T_medium1
	uint nEEE_e7 = 0;    // number of electrons matching e7T piece of e18vh_medium1_2e7T_medium1


	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchEle(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.47, m_accOnly)

		// Electrons
		if(lep->isEle()){
			if(useTrilepTrigs){
				// EEE
				if(matchEle(TRIG_e18vh_medium1_2e7T_medium1, 20)) nEEE_e18++;
				if(matchEle(TRIG_e18vh_medium1_2e7T_medium1, 9)) nEEE_e7++;
			}
		}


		// Cleanup preprocessor shorthand
#undef matchEle
	} // lepton loop

	bool passEEE = nEEE_e18>0 && nEEE_e7>1;


	bool passEgamma = passEEE;
	// Stream dependence to avoid double counting
	if(stream==Stream_Egamma && !passEgamma) return false;
	if(stream==Stream_MC && !passEgamma ) return false;

	return passEEE;
}

bool TrilTrigLogic::passTriggerMatching_3mu6(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;

	uint nMMM_m6 = 0;    // number of muons matching mu6 piece of 3mu6


	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)

		// Muons
		if(lep->isMu()){
			// If leading lepton, check isolated trigger
			//if(i==0 && matchLepTrigger(lep, TRIG_mu24i_tight, 25, m_accOnly)) n1M++;
			if(useTrilepTrigs){
				if(matchMu(TRIG_mu6, 7)) nMMM_m6++;
			}
		}

		// Cleanup preprocessor shorthand
#undef matchMu

	} // lepton loop

	bool passMMM_6 = nMMM_m6>2;


	bool passMuons  = passMMM_6;
	if(stream==Stream_Muons && !passMuons) return false;
	if(stream==Stream_MC && !passMuons ) return false;

	return passMMM_6;
}

bool TrilTrigLogic::passTriggerMatching_mu18tight_2mu4_EFFS(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;
	uint nMMM_m18 = 0;   // number of muons matching mu18 piece of mu18_tight_2mu4_EFFS
	uint nMMM_m4 = 0;    // number of muons matching mu4 piece of mu18_tight_2mu4_EFFS


	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)

		// Muons
		if(lep->isMu()){
			if(useTrilepTrigs){
				// MMM
				if(matchMu(TRIG_mu18_tight_2mu4_EFFS, 19)) nMMM_m18++;
				if(matchMu(TRIG_mu18_tight_2mu4_EFFS, 5)) nMMM_m4++;
			}
		}

		// Cleanup preprocessor shorthand
#undef matchMu

	} // lepton loop

	bool passMMM_18_4 = nMMM_m18>0 && nMMM_m4>1;
	bool passMuons  = passMMM_18_4;
	if(stream==Stream_Muons && !passMuons) return false;
	if(stream==Stream_MC && !passMuons ) return false;

	return passMMM_18_4;
}

bool TrilTrigLogic::passTriggerMatching_2e7T_medium1_mu6(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;

	// For the trilep triggers, need to make sure to match three legs
	uint nEEM_e = 0;     // number of electrons matching e7T piece of 2e7T_medium1_mu6
	uint nEEM_m = 0;     // number of muons matching mu6 piece of 2e7T_medium1_mu6
	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchEle(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.47, m_accOnly)
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)

		// Electrons
		if(lep->isEle()){
			if(useTrilepTrigs){
				// EEM
				if(matchEle(TRIG_2e7T_medium1_mu6, 9)) nEEM_e++;
			}
		}

		// Muons
		if(lep->isMu()){
			if(useTrilepTrigs){
				// EEM
				if(matchMu(TRIG_mu6, 7)) nEEM_m++;
			}
		}

		// Cleanup preprocessor shorthand
#undef matchEle
#undef matchMu

	} // lepton loop

	// trilep
	bool passEEM = nEEM_e>1 && nEEM_m>0;

	bool passEgamma = passEEM;

	// Stream dependence to avoid double counting
	if(stream==Stream_Egamma && !passEgamma) return false;
	if(stream==Stream_MC && !passEgamma ) return false;

	return passEEM;
//	return true;
}


bool TrilTrigLogic::passTriggerMatching_e7T_medium1_2mu6(const LeptonVector& leptons, const TauVector& taus,
		const Event* evt, bool useDilepTrigs, bool useTrilepTrigs)
{
	DataStream stream = evt->stream;

	// For the trilep triggers, need to make sure to match three legs
	uint nEMM_e = 0;     // number of electrons matching e7T piece of e7T_medium1_2mu6
	uint nEMM_m = 0;     // number of muons matching mu6 piece of e7T_medium1_2mu6


	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];

		// Preprocessor shorthand for feature matching with eta cuts folded in
#define matchEle(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.47, m_accOnly)
#define matchMu(mask, ptMin) matchLepTrigger(lep, mask, ptMin, 2.4, m_accOnly)

		// Electrons
		if(lep->isEle()){
			if(useTrilepTrigs){
				// EMM
				if(matchEle(TRIG_e7T_medium1_2mu6, 9)) nEMM_e++;
			}
		}

		// Muons
		if(lep->isMu()){
			if(useTrilepTrigs){
				// EMM
				if(matchMu(TRIG_mu6, 7)) nEMM_m++;
			}
		}

		// Cleanup preprocessor shorthand
#undef matchEle
#undef matchMu

	} // lepton loop
	// trilep
	bool passEMM = nEMM_e>0 && nEMM_m>1;

	bool passEgamma =passEMM ;
	// Stream dependence to avoid double counting
	if(stream==Stream_Egamma && !passEgamma) return false;
	if(stream==Stream_MC && !passEgamma ) return false;

	return passEMM;
	//return true;
}
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passTriggerMatching_2mu4T_xe60_tclcw(const LeptonVector& leptons,const TauVector& taus, const JetVector& jets, const Met* met , const Event* evt){
	DataStream stream = evt->stream;
	bool passMM4X=false;
	uint nM4X_m =0; // number of muons matching EF_2mu4T_xe60_tclcw
	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];
#define matchLepMetMu(mask, ptMin, metMin) matchLepMetTrigger(lep, met, mask, ptMin, metMin, 2.4, m_accOnly)
		if(lep->isMu()){
			if(matchLepMetMu(TRIG_mu4T,5,100)) nM4X_m++;
		}
#undef matchLepMetMu
	} // lepton loop
// lepmet
if((nM4X_m>1) && (evt->passTrig(TRIG_2mu4T_xe60_tclcw))) passMM4X = true;
bool passMuons = passMM4X;
	if(stream==Stream_Muons && !passMuons) return false;
	if(stream==Stream_MC && !passMuons) return false;

	return passMM4X;
}
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passTriggerMatching_2mu8_EFxe40_tclcw(const LeptonVector& leptons,const TauVector& taus,const JetVector& jets, const Met* met , const Event* evt){
	DataStream stream = evt->stream;
	bool passMM8X = false;
	uint nM8X_m =0; // number of muons matching EF_2mu8_EFxe40_tclcw
	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];
#define matchLepMetMu(mask, ptMin, metMin) matchLepMetTrigger(lep, met, mask, ptMin, metMin, 2.4, m_accOnly)
		if(lep->isMu()){
			if(matchLepMetMu(TRIG_mu8,8,80)) nM8X_m++;
		}
#undef matchLepMetMu
	} // lepton loop
// lepmet
if((nM8X_m>1) && (evt->passTrig(TRIG_2mu8_EFxe40_tclcw))) passMM8X = true;
bool passMuons = passMM8X;
	if(stream==Stream_Muons && !passMuons) return false;
	if(stream==Stream_MC && !passMuons) return false;

	return passMM8X;
}
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passTriggerMatching_mu4T_j65_a4tchad_xe70_tclcw_veryloose(const LeptonVector& leptons,const TauVector& taus,const JetVector& jets, const Met* met , const Event* evt){
	DataStream stream = evt->stream;
	bool passMJX70 = false;
	uint nMJX70 =0; // number of muons matching EF_mu4T_j65_a4tchad_xe70_tclcw_veryloose
	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];
#define matchLepJetMetMu(mask, ptMin, metMin, jetptMin) matchLepJetMetTrigger(lep, jets, met, mask,ptMin, metMin, jetptMin, 2.4, m_accOnly)
		if(lep->isMu()){
				if(matchLepJetMetMu(TRIG_mu4T, 5, 110, 105)) nMJX70++;
		}
#undef matchLepJetMetMu
	} // lepton loop
// lepmet
if((nMJX70>0) && (evt->passTrig(TRIG_mu4T_j65_a4tchad_xe70_tclcw_veryloose))) passMJX70=true;

bool passMuons = passMJX70;

	if(stream==Stream_Muons && !passMuons) return false;
	if(stream==Stream_MC && !passMuons) return false;

	return passMJX70;
}
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passTriggerMatching_mu24_j65_a4tchad_EFxe40_tclcw(const LeptonVector& leptons,const TauVector& taus,const JetVector& jets, const Met* met , const Event* evt){
	DataStream stream = evt->stream;
	bool passMJX40 = false;
	uint nMJX40 =0; // number of muons matching EF_mu24_j65_a4tchad_EFxe40_wMu_tclcw
	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];
#define matchLepJetMetMu(mask, ptMin, metMin, jetptMin) matchLepJetMetTrigger(lep, jets, met, mask,ptMin, metMin, jetptMin, 2.4, m_accOnly)
		if(lep->isMu()){
				//if(matchLepJetMetMu(TRIG_mu24_j65_a4tchad_EFxe40wMu_tclcw,25,80,105)) nMJX40++;
				if(matchLepJetMetMu(TRIG_mu24,25,80,105)) nMJX40++;
		}
#undef matchLepJetMetMu
	} // lepton loop
// lepmet
if((nMJX40>0) && (evt->passTrig(TRIG_mu24_j65_a4tchad_EFxe40_tclcw))) passMJX40 =true;
bool passMuons = passMJX40;
	if(stream==Stream_Muons && !passMuons) return false;
	if(stream==Stream_MC && !passMuons) return false;

	return passMJX40;
}
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::passTriggerMatching_e24vh_medium1_EFxe35_tclcw(const LeptonVector& leptons,const TauVector& taus,const JetVector& jets, const Met* met , const Event* evt){
	DataStream stream = evt->stream;
	bool passE24X=false;
	uint nE24X_e =0; // number of muons matching EF_e24vh_medium1_EFxe35_tclcw
	// loop over leptons
	for(uint i=0; i < leptons.size(); i++){

		const Lepton* lep = leptons[i];
#define matchLepMetEle(mask, ptMin, metMin) matchLepMetTrigger(lep, met, mask, ptMin, metMin, 2.47, m_accOnly)
		if(lep->isEle()){
				if(matchLepMetEle(TRIG_e24vh_medium1,25,75 )) nE24X_e++;
		}
#undef matchLepMetEle
	} // lepton loop
// lepmet
if((nE24X_e>0) && (evt->passTrig(TRIG_e24vh_medium1_EFxe35_tclcw))) passE24X = true;
bool passEgamma = passE24X;
	if(stream==Stream_Egamma && !passEgamma) return false;
	if(stream==Stream_MC && !passEgamma) return false;

	return passE24X;
}

/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::matchLepTrigger(const Lepton* lep, long long trigMask,
                                    float ptMin, float etaMax, bool accOnly)
{
  if(lep->Pt() < ptMin) return false;
  if(etaMax > 0 && fabs(lep->Eta()) > etaMax) return false;
  return accOnly || lep->matchTrig(trigMask);
}
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::matchTauTrigger(const Tau* tau, long long trigMask, float ptMin, bool accOnly)
{
  if(tau->Pt() < ptMin) return false;
  return accOnly || tau->matchTrig(trigMask);
}
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::matchLepMetTrigger(const Lepton* lep, const Met* met, long long trigMask, float ptMin, float metMin, float etaMax, bool accOnly)
{
 if(lep->Pt() < ptMin) return false;
 if(met->Et < metMin) return false;
 if(etaMax > 0 && fabs(lep->Eta()) > etaMax) return false;
 return accOnly || lep->matchTrig(trigMask);
}
/*--------------------------------------------------------------------------------*/
bool TrilTrigLogic::matchLepJetMetTrigger(const Lepton* lep, const JetVector& jets, const Met* met, long long trigMask, float ptMin, float metMin, float jetptMin, float etaMax, bool accOnly)
{
 double njet=0;
 for(uint i =0; i<jets.size(); i++){
	const Jet* jet = jets[i];
 if(jet->Pt() >jetptMin) njet++;
}
 if(njet==0) return false;
 if(lep->Pt() < ptMin) return false;
 if(met->Et < metMin) return false;
 if(etaMax > 0 && fabs(lep->Eta()) > etaMax) return false;
 return accOnly || lep->matchTrig(trigMask);
}

/*--------------------------------------------------------------------------------*/
// Trigger reweighting for MC - not currently used
/*--------------------------------------------------------------------------------*/
float TrilTrigLogic::getTriggerWeight(const LeptonVector& leptons, Event* evt)
{
  DataStream stream = evt->stream;
  if(stream!=Stream_MC) return 1;

  // for now...
  return 1;
}

