#ifndef SusyNtuple_Susy2LepCutflow_h
#define SusyNtuple_Susy2LepCutflow_h

//ROOT
#include "TTree.h"
#include "TChain.h"

//SusyNtuple
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/SusyNtTools.h"

//std/stl
#include <fstream>

/////////////////////////////////////////////////////////////
//
// Susy2LepCutflow
//
// Analysis (TSelector) looper with AnalysisType::Ana_2Lep
// base selection and several signal-region like selections
// applied
//
// daniel.joseph.antrim@cern.ch
// July 2017
//
/////////////////////////////////////////////////////////////

// for TSelector analysis loopers processing susyNt you MUST inherit from SusyNtAna
// in order to pick up the susyNt class objects
class Susy2LepCutflow : public SusyNtAna
{

    public :
        Susy2LepCutflow();
        virtual ~Susy2LepCutflow() {};

        void set_debug(int dbg) { m_dbg = dbg; }
        int dbg() { return m_dbg; }

        void set_chain(TChain* chain) { m_input_chain = chain; }
        TChain* chain() { return m_input_chain; }

        ////////////////////////////////////////////
        // analysis methods
        ////////////////////////////////////////////
        void initialize_counters();

        float w() { return m_mc_weight; }
        float sf() { return m_lep_sf; }
        float btagsf() { return m_btag_sf; }


        // standard ATLAS event cleaning
        bool passEventCleaning(const MuonVector& preMuons, const MuonVector& baseMuons,
                const JetVector& baseJets);

        // select dilepton events
        bool passDileptonEvent(const LeptonVector& baseLeptons, const LeptonVector& signalLeptons);

        // compute lepton scale factors for ID, Reco, and Isolation corrections
        float compute_lepton_scale_factors(const LeptonVector& leptons);

        // compute flavor tagging efficiency scale factor
        float compute_btagging_sf(const JetVector& jets);

        // signal regions
        void check_mt2_selections(const LeptonVector& leptons, const Met* met);
        void check_njet_selections(const JetVector& jets);
        void check_jetmet_selections(const JetVector& jets, const Met* met, const LeptonVector& leptons);


        ////////////////////////////////////////////
        // TSelector methods override
        ////////////////////////////////////////////
        virtual void Begin(TTree* tree); // Begin is called before looping on entries
        virtual Bool_t Process(Long64_t entry); // Main event loop function called on each event
        virtual void Terminate(); // Terminate is called after looping has finished

        ////////////////////////////////////////////
        // helper
        ////////////////////////////////////////////
        std::string weight_str(float weighted_counter, int raw_counter);
        void print_counters();
        std::string event_counters();
        std::string dilepton_counts();
        std::string dil_cut(float* counter_w, uint* counter, int weight=20);

        struct EventCleaning {
            EventCleaning() :
                grl(0),
                lar(0),
                tile(0),
                ttc(0),
                sct(0),
                vtx(0),
                bad_mu(0),
                cos_mu(0),
                bad_jet(0),
                grl_w(0.0),
                lar_w(0.0),
                tile_w(0.0),
                ttc_w(0.0),
                sct_w(0.0),
                vtx_w(0.0),
                bad_mu_w(0.0),
                cos_mu_w(0.0),
                bad_jet_w(0.0)
            {}

            uint grl;       // event count pass GRL
            uint lar;       // event count pass LAr error flags
            uint tile;      // event count pass TILE error flags
            uint ttc;       // event count pass TTC veto
            uint sct;       // event count pass SCT error flags
            uint vtx;       // event count event has primary vertex
            uint bad_mu;    // event count pass bad muon check
            uint cos_mu;    // event count pass cosmic muon check
            uint bad_jet;   // event count pass bad jet (jet cleaning)

            float grl_w;
            float lar_w;
            float tile_w;
            float ttc_w;
            float sct_w;
            float vtx_w;
            float bad_mu_w;
            float cos_mu_w;
            float bad_jet_w;
        };

        struct DileptonCounts {

            DileptonCounts() :
                n_baseline(0),
                n_baseline_w(0) { clear(); }

            // selecting dilepton events
            uint n_baseline;
            uint n_signal[DiLepEvtType::ET_N];
            uint n_os[DiLepEvtType::ET_N];
            uint n_vetoZ[DiLepEvtType::ET_N];
            uint n_mll[DiLepEvtType::ET_N];
            uint n_pt[DiLepEvtType::ET_N];

            float n_baseline_w;
            float n_signal_w[DiLepEvtType::ET_N];
            float n_os_w[DiLepEvtType::ET_N];
            float n_vetoZ_w[DiLepEvtType::ET_N];
            float n_mll_w[DiLepEvtType::ET_N];
            float n_pt_w[DiLepEvtType::ET_N];

            // mt2 selections
            uint n_mt290[DiLepEvtType::ET_N];
            uint n_mt2120[DiLepEvtType::ET_N];
            uint n_mt2150[DiLepEvtType::ET_N];

            float n_mt290_w[DiLepEvtType::ET_N];
            float n_mt2120_w[DiLepEvtType::ET_N];
            float n_mt2150_w[DiLepEvtType::ET_N];

            // jets
            uint n_e2bjets[DiLepEvtType::ET_N];
            uint n_e2bjets0sjets[DiLepEvtType::ET_N];
            uint n_ge2bjets[DiLepEvtType::ET_N];

            float n_e2bjets_w[DiLepEvtType::ET_N];
            float n_e2bjets0sjets_w[DiLepEvtType::ET_N];
            float n_ge2bjets_w[DiLepEvtType::ET_N];

            // jets + met
            uint n_ge2jets[DiLepEvtType::ET_N];
            uint n_forwardJetVeto[DiLepEvtType::ET_N];
            uint n_bveto[DiLepEvtType::ET_N];
            uint n_dphill[DiLepEvtType::ET_N];
            uint n_met100[DiLepEvtType::ET_N];
            uint n_ht500[DiLepEvtType::ET_N];
            uint n_meff500[DiLepEvtType::ET_N];
            
            float n_ge2jets_w[DiLepEvtType::ET_N];
            float n_forwardJetVeto_w[DiLepEvtType::ET_N];
            float n_bveto_w[DiLepEvtType::ET_N];
            float n_dphill_w[DiLepEvtType::ET_N];
            float n_met100_w[DiLepEvtType::ET_N];
            float n_ht500_w[DiLepEvtType::ET_N];
            float n_meff500_w[DiLepEvtType::ET_N];
            
            


            void clear() {
                for(int i = 0; i < DiLepEvtType::ET_N; i++) {
                    // raw
                    n_signal[i] = 0;
                    n_os[i] = 0;
                    n_mll[i] = 0;
                    n_vetoZ[i] = 0;
                    n_pt[i] = 0;
                    n_mt290[i] = 0;
                    n_mt2120[i] = 0;
                    n_mt2150[i] = 0;

                    n_e2bjets[i] = 0;
                    n_e2bjets0sjets[i] = 0;
                    n_ge2bjets[i] = 0;

                    n_ge2jets[i] = 0;
                    n_forwardJetVeto[i] = 0;
                    n_bveto[i] = 0;
                    n_dphill[i] = 0;
                    n_met100[i] = 0;
                    n_ht500[i] = 0;
                    n_meff500[i] = 0;

                    // weighted
                    n_signal_w[i] = 0.0;
                    n_os_w[i] = 0.0;
                    n_mll_w[i] = 0.0;
                    n_vetoZ_w[i] = 0.0;
                    n_pt_w[i] = 0.0;
                    n_mt290_w[i] = 0.0;
                    n_mt2120_w[i] = 0.0;
                    n_mt2150_w[i] = 0.0;

                    n_e2bjets_w[i] = 0.0;
                    n_e2bjets0sjets_w[i] = 0.0;
                    n_ge2bjets_w[i] = 0.0;

                    n_ge2jets_w[i] = 0.0;
                    n_forwardJetVeto_w[i] = 0.0;
                    n_bveto_w[i] = 0.0;
                    n_dphill_w[i] = 0.0;
                    n_met100_w[i] = 0.0;
                    n_ht500_w[i] = 0.0;
                    n_meff500_w[i] = 0.0;
                } // i
            }
            

        };

    private :
        int m_dbg;
        TChain* m_input_chain; // the TChain object we are processing
        DiLepEvtType m_lep_type;
        float m_mc_weight;
        float m_lep_sf;
        float m_btag_sf;

        ////////////////////////////////////////////
        // counters
        ////////////////////////////////////////////
        uint          n_readin; // total events processed
        EventCleaning event_cleaning_counters;
        DileptonCounts dilepton_counters;
        


}; //class


#endif
