#ifndef SusyNtuple_TrilTrigLogic_h
#define SusyNtuple_TrilTrigLogic_h

#include "TFile.h"

#include "ReweightUtils/APWeightEntry.h"
#include "ReweightUtils/APReweightND.h"
#include "ReweightUtils/APEvtWeight.h"

#include "SusyNt.h"
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtObject.h"


/*

   TrilTrigLogic - class to implement the multilepton trigger logic

*/

//using namespace Susy;

class TrilTrigLogic
{

	public:

		TrilTrigLogic();
		~TrilTrigLogic();

		// Load the reweighters with maps from input files
		void loadTriggerMaps();
		APReweightND* loadTrigWeighter(TFile* f, TString chain);

		// Trigger cut without matching
		bool passEventTrigger(const Susy::Event* evt);

		// Trigger matching in data (and MC at the moment)
		//bool passTriggerMatching(const LeptonVector& leptons, Event* evt);

		bool passEvent_xe80_tclcw_loose(const Susy::Event* evt, const Susy::Met* met);

		bool passEvent_j110_a4tchad_xe90_tclcw_loose(const Susy::Event* evt, const Susy::Met* met, const JetVector& jets);

		bool passEvent_2e7T_medium1_mu6(const Susy::Event* evt, const LeptonVector& leptons);
		bool passEvent_e7T_medium1_2mu6(const Susy::Event* evt, const LeptonVector& leptons);
		bool passEvent_j80_a4tchad_xe100_tclcw_loose(const Susy::Event* evt, const Susy::Met* met, const JetVector& jets);

		bool passEvent_j80_a4tchad_xe70_tclcw_dphi2j45xe10(const Susy::Event* evt, const Susy::Met* met, const JetVector& jets);
		// Trigger matching in data (and MC at the moment)
		//bool passTriggerMatching(const LeptonVector& leptons, Event* evt);
		bool passTriggerMatching(const LeptonVector& leptons, const TauVector& taus, const JetVector& jets, const Susy::Met* met, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true, bool useJetMetTrigs=true, bool useLepMetTrigs=true);

		bool passTriggerMatching_e24vhi_medium1(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_mu24i_tight(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_mu18_tight_e7_medium1(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_e12Tvh_medium1_mu8(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_mu18_tight_mu8_EFFS(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_e24vh_medium1_e7_medium1(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_2e12Tvh_loose1(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_2mu13(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_e18vh_medium1_2e7T_medium1(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_3mu6(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_mu18tight_2mu4_EFFS(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);

		bool passTriggerMatching_2e7T_medium1_mu6(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);
		bool passTriggerMatching_e7T_medium1_2mu6(const LeptonVector& leptons, const TauVector& taus, const Susy::Event* evt, bool useDilepTrigs=true, bool useTrilepTrigs=true);
		bool passTriggerMatching_2mu4T_xe60_tclcw(const LeptonVector& leptons,const TauVector& taus,const JetVector& jets, const Susy::Met* met , const Susy::Event* evt);

		bool passTriggerMatching_2mu8_EFxe40_tclcw(const LeptonVector& leptons,const TauVector& taus,const JetVector& jets, const Susy::Met* met , const Susy::Event* evt);

		bool passTriggerMatching_mu4T_j65_a4tchad_xe70_tclcw_veryloose(const LeptonVector& leptons,const TauVector& taus,const JetVector& jets, const Susy::Met* met , const Susy::Event* evt);

		bool passTriggerMatching_mu24_j65_a4tchad_EFxe40_tclcw(const LeptonVector& leptons,const TauVector& taus,const JetVector& jets, const Susy::Met* met , const Susy::Event* evt);

		bool passTriggerMatching_e24vh_medium1_EFxe35_tclcw(const LeptonVector& leptons, const TauVector& taus, const JetVector& jets, const Susy::Met* met , const Susy::Event* evt);


		bool matchLepTrigger(const Susy::Lepton* lep, long long trigMask, float ptMin, float etaMax,
					bool accOnly=false);
		bool matchTauTrigger(const Susy::Tau* tau, long long trigMask, float ptMin, bool accOnly=false);
		bool matchLepMetTrigger(const Susy::Lepton* lep, const Susy::Met* met, long long trigMask, float ptMin, float metMin, float etaMax, bool accOnly);
		bool matchLepJetMetTrigger(const Susy::Lepton* lep, const JetVector& jets, const Susy::Met* met, long long trigMask, float ptMin, float metMin, float jetptMin, float etaMax, bool accOnly);

		//bool matchLepTrigger(const Susy::Lepton* lep, int trigMask, float ptThreshold=0,
		//                     bool accOnly=false) {
		//  return lep->Pt() > ptThreshold && (accOnly || lep->matchTrig(trigMask));
		//}
		//bool matchTauTrigger(const Susy::Tau* tau, int trigMask, float ptThreshold=0,
		//                     bool accOnly=false){
		//  return tau->Pt() > ptThreshold && (accOnly || tau->matchTrig(trigMask));
		//}

		// Trigger reweighting in MC - not currently setup
		float getTriggerWeight(const LeptonVector& leptons, Susy::Event* evt);

		// Only apply pt threshold cuts for trigger acceptance
		void setAccOnly(bool doIt=true) { m_accOnly = doIt; }

		// Debug info
		void setDebug(bool doIt=true) { m_dbg = doIt; }

	protected:

		bool                m_accOnly;      // Only check trigger kinematic acceptance

		int                 m_dbg;          // Debug flag

};

#endif
