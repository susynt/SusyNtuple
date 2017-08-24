#ifndef SUSYNTUPLE_SUSYLEPTRIGEXAMPLE_H
#define SUSYNTUPLE_SUSYLEPTRIGEXAMPLE_H

//ROOT
#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"
#include "TCanvas.h"

//SusyNtuple
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/SusyNtTools.h"

//std/stl
#include <fstream>
#include <string>
#include <map>
//#include <pair>
#include <vector>

class SusyLepTrigExample : public SusyNtAna
{
    public :

        SusyLepTrigExample();
        virtual ~SusyLepTrigExample();

        void set_chain(TChain* chain) { m_input_chain = chain; }
        TChain* chain() { return m_input_chain; }
        void set_debug(bool dbg) { m_dbg = dbg; }
        bool dbg() { return m_dbg; }
        void set_pt_thresholds(int pt0 = 25, int pt1 = 20) { m_lead_pt = pt0; m_sublead_pt = pt1; }
        int lead_pt_threshold() { return m_lead_pt; }
        int sublead_pt_threshold() { return m_sublead_pt; }

        void initialize_output();
        void initialize_histos();

        bool pass_event_cleaning(int flags, const MuonVector& pre_muons,
            const MuonVector& base_muons, const JetVector& base_jets);

        void test_single_lepton();
        void test_single_mu_trigger(Susy::Muon* muon, std::string trigger, float threshold, int single_idx);
        void test_single_ele_trigger(Susy::Electron* electron, std::string trigger, int single_idx);
        void write_single_mu_histos();
        void write_single_ele_histos();
        


        void test_dilepton();
        void test_dimuon_trigger(Susy::Muon* mu0, Susy::Muon* mu1, std::string trigger,
                std::pair<float, float> thresholds, int dimu_idx);
        void test_dielectron_trigger(Susy::Electron* el0, Susy::Electron* el1, std::string trigger,
                int diel_idx);
        void test_mixed_trigger(Susy::Electron* el, Susy::Muon* mu, std::string trigger, int mix_idx);
        void write_di_mu_histos();
        void write_di_el_histos();
        void write_mix_histos();

        std::map<std::string, float> single_muon_trigger_thresholds();
        std::map<std::string, std::pair<float, float> > dimuon_trigger_thresholds();

        ////////////////////////////////
        // TSelector methods
        ////////////////////////////////
        virtual void Begin(TTree *tree);
        virtual Bool_t Process(Long64_t entry);
        virtual void Terminate();

    private :

        TChain* m_input_chain;
        bool m_dbg;
        bool single_mu_thresholds_printed;
        bool di_mu_thresholds_printed;
        int m_lead_pt;
        int m_sublead_pt;

        bool outfile_initialized;
        TFile* m_out_file;
        TH1F* h_single_mu_fired;
        TH1F* h_single_mu_match;
        std::vector<TH1F*> h_single_mu_sf;

        TH1F* h_dimu_fired;
        TH1F* h_dimu_match;
        std::vector<TH1F*> h_di_mu_sf;

        TH1F* h_single_ele_fired;
        TH1F* h_single_ele_match; 
        std::vector<TH1F*> h_single_ele_sf;

        TH1F* h_diel_fired;
        TH1F* h_diel_match;
        std::vector<TH1F*> h_di_el_sf;

        TH1F* h_mixed_fired;
        TH1F* h_mixed_match;


}; // class


#endif
