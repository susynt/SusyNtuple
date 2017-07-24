#include "SusyNtuple/SusyLepTrigExample.h"

//std/stl
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//ROOT
#include "TROOT.h"
#include "TCanvas.h"
#include "TLegend.h"

//////////////////////////////////////////////////////////////////////////////
SusyLepTrigExample::SusyLepTrigExample() :
    m_input_chain(nullptr),
    m_dbg(false),
    single_mu_thresholds_printed(false),
    di_mu_thresholds_printed(false),
    m_lead_pt(25),
    m_sublead_pt(20),
    outfile_initialized(false),
    m_out_file(nullptr),
    h_single_mu_fired(nullptr),
    h_single_mu_match(nullptr),
    h_dimu_fired(nullptr),
    h_dimu_match(nullptr),
    h_single_ele_fired(nullptr),
    h_single_ele_match(nullptr),
    h_diel_fired(nullptr),
    h_diel_match(nullptr),
    h_mixed_fired(nullptr),
    h_mixed_match(nullptr)
{
    cout << "SusyLepTrigExample" << endl;
    gROOT->SetBatch(true);
}
//////////////////////////////////////////////////////////////////////////////
SusyLepTrigExample::~SusyLepTrigExample()
{
    
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::Begin(TTree* /*tree*/)
{
    SusyNtAna::Begin(0);
    initialize_output();

}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::initialize_output()
{
    initialize_histos();
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::initialize_histos()
{

    // single muon fire/matching histo
    size_t  n_bins = nttools().triggerTool().single_muo_triggers().size();
    vector<string> sm_trigs = nttools().triggerTool().single_muo_triggers();
    h_single_mu_fired = new TH1F("h_single_mu_fired", "Single Mu;;Entries", n_bins, 0, n_bins);
    h_single_mu_fired->SetLineColor(kBlack);
    h_single_mu_match = new TH1F("h_single_mu_match", "Single Mu;;Entries", n_bins, 0, n_bins);
    h_single_mu_match->SetFillStyle(3354);
    h_single_mu_match->SetFillColor(30);
    h_single_mu_match->SetLineColor(kBlue);

    for(int itrig = 0; itrig < (int)sm_trigs.size(); itrig++) {
        h_single_mu_fired->GetXaxis()->SetBinLabel(itrig+1, sm_trigs.at(itrig).c_str());
        h_single_mu_match->GetXaxis()->SetBinLabel(itrig+1, sm_trigs.at(itrig).c_str());
    }

    // single muon scale factor histos
    for(int itrig = 0; itrig < (int)sm_trigs.size(); itrig++) {
        stringstream sx; 
        sx << "Single Muon Trigger SF: " << sm_trigs.at(itrig);
        stringstream id;
        id << "h_single_muon_sf_" << sm_trigs.at(itrig);
        TH1F* h = new TH1F(id.str().c_str(), sx.str().c_str(), 120, 0, 1.20);
        h->SetLineColor(kBlack);
        h->SetFillColor(30);
        h_single_mu_sf.push_back(h);
    }

    // dimuon fire/matching histo
    n_bins = nttools().triggerTool().di_muo_triggers().size();
    vector<string> dm_trigs = nttools().triggerTool().di_muo_triggers();
    h_dimu_fired = new TH1F("h_di_mu_fired", "Di Mu;;Entries", n_bins, 0, n_bins);
    h_dimu_fired->SetLineColor(kBlack);
    h_dimu_match = new TH1F("h_di_mu_match", "Di Mu;;Entries", n_bins, 0, n_bins);
    h_dimu_match->SetFillStyle(3354);
    h_dimu_match->SetFillColor(38);
    h_dimu_match->SetLineColor(kBlue);

    for(int itrig = 0; itrig < (int)dm_trigs.size(); itrig++) {
        h_dimu_fired->GetXaxis()->SetBinLabel(itrig+1, dm_trigs.at(itrig).c_str());
        h_dimu_match->GetXaxis()->SetBinLabel(itrig+1, dm_trigs.at(itrig).c_str());
    }

    // dimuon scale factor histos
    for(int itrig = 0; itrig < (int)dm_trigs.size(); itrig++) {
        stringstream sx;
        sx << "Dimuon Trigger SF: " << dm_trigs.at(itrig);
        stringstream id;
        id << "h_dimuon_sf_" << dm_trigs.at(itrig);
        TH1F* h = new TH1F(id.str().c_str(), sx.str().c_str(), 120, 0, 1.20);
        h->SetLineColor(kBlack);
        h->SetFillColor(38);
        h_di_mu_sf.push_back(h);
    }

    // single electron fire/matching histo
    n_bins = nttools().triggerTool().single_ele_triggers().size();
    vector<string> se_trigs = nttools().triggerTool().single_ele_triggers();
    h_single_ele_fired = new TH1F("h_single_ele_fired", "Single Ele;;Entries", n_bins, 0, n_bins);
    h_single_ele_fired->SetLineColor(kBlack);
    h_single_ele_match = new TH1F("h_single_ele_match", "Single Ele;;Entries", n_bins, 0, n_bins);
    h_single_ele_match->SetFillStyle(3354);
    h_single_ele_match->SetFillColor(30);
    h_single_ele_match->SetLineColor(kBlue);
    for(int itrig = 0; itrig < (int)se_trigs.size(); itrig++) {
        h_single_ele_fired->GetXaxis()->SetBinLabel(itrig+1, se_trigs.at(itrig).c_str());
        h_single_ele_match->GetXaxis()->SetBinLabel(itrig+1, se_trigs.at(itrig).c_str());
    }

    // single electron scale factor histos
    for(int itrig = 0; itrig < (int)se_trigs.size(); itrig++) {
        stringstream sx;
        sx << "Single Electron Trigger SF: " << se_trigs.at(itrig);
        stringstream id;
        id << "h_single_ele_sf_" << se_trigs.at(itrig);
        TH1F* h = new TH1F(id.str().c_str(), sx.str().c_str(), 120, 0, 1.2);
        h->SetLineColor(kBlack);
        h->SetFillColor(30);
        h_single_ele_sf.push_back(h);
    }

    // dielectron fire/matching histo
    n_bins = nttools().triggerTool().di_ele_triggers().size();
    vector<string> de_trigs = nttools().triggerTool().di_ele_triggers();
    h_diel_fired = new TH1F("h_dielectron_fired", "Dielectron;;Entries", n_bins, 0, n_bins);
    h_diel_fired->SetLineColor(kBlack);
    h_diel_match = new TH1F("h_dielectron_match", "Dielectron;;Entries", n_bins, 0, n_bins);
    h_diel_match->SetFillStyle(3354);
    h_diel_match->SetFillColor(38);
    h_diel_match->SetLineColor(kBlue);
    for(int itrig = 0; itrig < (int)de_trigs.size(); itrig++) {
        h_diel_fired->GetXaxis()->SetBinLabel(itrig+1, de_trigs.at(itrig).c_str());
        h_diel_match->GetXaxis()->SetBinLabel(itrig+1, de_trigs.at(itrig).c_str());
    }

    // dielectron scale factor histos
    for(int itrig = 0; itrig < (int)de_trigs.size(); itrig++) {
        stringstream sx;
        sx << "Dielectron Trigger SF: " << de_trigs.at(itrig);
        stringstream id;
        id << "h_dielectron_sf_" << de_trigs.at(itrig);
        TH1F* h = new TH1F(id.str().c_str(), sx.str().c_str(), 120, 0, 1.2);
        h->SetLineColor(kBlack);
        h->SetFillColor(30);
        h_di_el_sf.push_back(h);
    }

    // mixed fire/matching histos
    n_bins = nttools().triggerTool().ele_muo_triggers().size();
    vector<string> mix_trigs = nttools().triggerTool().ele_muo_triggers();
    h_mixed_fired = new TH1F("h_mixed_fired", "Mixed Lepton;;Entries", n_bins, 0, n_bins);
    h_mixed_fired->SetLineColor(kBlack);
    h_mixed_match = new TH1F("h_mixed_match", "Mixed Lepton;;Entries", n_bins, 0, n_bins);
    h_mixed_match->SetFillStyle(3354);
    h_mixed_match->SetFillColor(38);
    h_mixed_match->SetLineColor(kBlue);
    for(int itrig = 0; itrig < (int)mix_trigs.size(); itrig++) {
        h_mixed_fired->GetXaxis()->SetBinLabel(itrig+1, mix_trigs.at(itrig).c_str());
        h_mixed_match->GetXaxis()->SetBinLabel(itrig+1, mix_trigs.at(itrig).c_str());
    }

}
//////////////////////////////////////////////////////////////////////////////
Bool_t SusyLepTrigExample::Process(Long64_t entry)
{
    GetEntry(entry);
    SusyNtAna::clearObjects();

    // do this here so that we can add the DSID to the file
    if(!outfile_initialized) {
        outfile_initialized = true;
        stringstream sx;
        sx << "susy_lep_trig_" << nt.evt()->mcChannel << ".root";
        m_out_file = new TFile(sx.str().c_str(), "RECREATE");
    }

    m_chainEntry++;
    if(m_chainEntry%500==0) {
        cout << "SusyLepTrigExample::Process    ++++ Processing entry " << setw(6) << m_chainEntry
                << " event " << setw(7) << nt.evt()->eventNumber << " ++++ " << endl;
    }

    SusyNtAna::selectObjects();
    int flags = nt.evt()->cutFlags[NtSys::NOM];
    if(!pass_event_cleaning(flags, m_preMuons, m_baseMuons, m_baseJets)) return false;

    // single lepton
    if(m_signalLeptons.size() == 1) {
        bool lead_pt = (m_signalLeptons.at(0)->Pt() >= lead_pt_threshold());
        if(lead_pt)
            test_single_lepton();
    }
    // dilepton
    else if(m_signalLeptons.size() == 2) { 
        bool lead_pt = (m_signalLeptons.at(0)->Pt() >= lead_pt_threshold());
        bool sublead_pt = (m_signalLeptons.at(1)->Pt() >= sublead_pt_threshold());
        if(lead_pt && sublead_pt)
            test_dilepton();
    }

    return true;
}
//////////////////////////////////////////////////////////////////////////////
bool SusyLepTrigExample::pass_event_cleaning(int flags, const MuonVector& pre_muons,
        const MuonVector& base_muons, const JetVector& base_jets)
{
    if( !nttools().passGRL(flags) )             return false;
    if( !nttools().passLarErr(flags) )          return false;
    if( !nttools().passTileErr(flags) )         return false;
    if( !nttools().passTTC(flags) )             return false;
    if( !nttools().passSCTErr(flags) )          return false;
    if( !nttools().passGoodVtx(flags) )         return false;
    if( !nttools().passBadMuon(pre_muons) )     return false;
    if( !nttools().passCosmicMuon(base_muons) ) return false;
    if( !nttools().passJetCleaning(base_jets) ) return false;
    return true;
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::test_single_lepton()
{
    Lepton* l = m_signalLeptons.at(0);

    if(!l->isEle()) {
        Susy::Muon* muon = dynamic_cast<Susy::Muon*>(l);

        map<string, float> thresholds = single_muon_trigger_thresholds();
        vector<string> triggers = nttools().triggerTool().single_muo_triggers();
        for(int itrig = 0; itrig < (int)triggers.size(); itrig++) {
            test_single_mu_trigger(muon, triggers.at(itrig), thresholds[triggers.at(itrig)], itrig);
        }
    }
    else {
        Susy::Electron* electron = dynamic_cast<Susy::Electron*>(l);
        vector<string> triggers = nttools().triggerTool().single_ele_triggers();
        for(int itrig = 0; itrig < (int)triggers.size(); itrig++) {
            test_single_ele_trigger(electron, triggers.at(itrig), itrig);
        }
    }

}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::test_dilepton()
{
    Lepton* l0 = m_signalLeptons.at(0);
    Lepton* l1 = m_signalLeptons.at(1);

    bool is_mm = ( !l0->isEle() && !l1->isEle() );
    bool is_ee = ( l0->isEle() && l1->isEle() );
    bool is_em = ( l0->isEle() != l1->isEle() );

    if(is_mm) {
        Susy::Muon* mu0 = dynamic_cast<Susy::Muon*>(l0);
        Susy::Muon* mu1 = dynamic_cast<Susy::Muon*>(l1);

        map<string, pair<float, float> > thresholds = dimuon_trigger_thresholds();

        vector<string> triggers = nttools().triggerTool().di_muo_triggers();
        for(int itrig = 0; itrig < (int)triggers.size(); itrig++) {
            test_dimuon_trigger(mu0, mu1, triggers.at(itrig), thresholds[triggers.at(itrig)], itrig);
        }
    }
    else if(is_ee) {
        Susy::Electron* el0 = dynamic_cast<Susy::Electron*>(l0);
        Susy::Electron* el1 = dynamic_cast<Susy::Electron*>(l1);
        vector<string> triggers = nttools().triggerTool().di_ele_triggers();
        for(int itrig = 0; itrig < (int)triggers.size(); itrig++) {
            test_dielectron_trigger(el0, el1, triggers.at(itrig), itrig);
        }
    }
    else if(is_em) {
        Susy::Muon* mu;
        Susy::Electron* el;
        if(l0->isEle()) {
            el = dynamic_cast<Susy::Electron*>(l0);
            mu = dynamic_cast<Susy::Muon*>(l1);
        }
        else {
            el = dynamic_cast<Susy::Electron*>(l1);
            mu = dynamic_cast<Susy::Muon*>(l0);
        }
        vector<string> triggers = nttools().triggerTool().ele_muo_triggers();

        // n.b. we don't store scale factors (currently) for E+M triggers
        for(int itrig = 0; itrig < (int)triggers.size(); itrig++) {
            test_mixed_trigger(el, mu, triggers.at(itrig), itrig);
        }
    }


    return;
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::test_single_mu_trigger(Susy::Muon* muon, string trigger, float threshold, int sm_idx)
{
    //int idx = nttools().triggerTool().idx_of_trigger(trigger);

    // did the trigger even fire?
    bool trig_has_fired = nttools().triggerTool().passTrigger(nt.evt()->trigBits, trigger);
    if(!trig_has_fired) return;
    int content = h_single_mu_fired->GetBinContent(sm_idx+1);
    h_single_mu_fired->SetBinContent(sm_idx+1, content+1);

    
    bool mu_is_matched = (muon->Pt() >= threshold);
    mu_is_matched = (mu_is_matched && nttools().triggerTool().lepton_trigger_match(muon, trigger));
    if(mu_is_matched) {
        int content = h_single_mu_match->GetBinContent(sm_idx+1);
        h_single_mu_match->SetBinContent(sm_idx+1, content+1);
        float sf = nttools().get_muon_trigger_scale_factor(*muon, trigger);
        h_single_mu_sf.at(sm_idx)->Fill(sf);
    }
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::test_single_ele_trigger(Susy::Electron* electron, string trigger, int se_idx)
{
    // did the trigger even fire?
    bool trig_has_fired = nttools().triggerTool().passTrigger(nt.evt()->trigBits, trigger);
    if(!trig_has_fired) return;
    int content = h_single_ele_fired->GetBinContent(se_idx+1);
    h_single_ele_fired->SetBinContent(se_idx+1, content+1);

    bool ele_is_matched = nttools().triggerTool().lepton_trigger_match(electron, trigger);
    if(ele_is_matched) {
        int content = h_single_ele_match->GetBinContent(se_idx+1);
        h_single_ele_match->SetBinContent(se_idx+1, content+1);
        float sf = nttools().get_electron_trigger_scale_factor(*electron, trigger);
        h_single_ele_sf.at(se_idx)->Fill(sf);
    }
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::test_dimuon_trigger(Susy::Muon* mu0, Susy::Muon* mu1, string trigger,
            pair<float, float> thresholds, int dimu_idx)
{
    //int idx = nttools().triggerTool().idx_of_trigger(trigger);

    // did the trigger even fire?
    bool trig_has_fired = nttools().triggerTool().passTrigger(nt.evt()->trigBits, trigger);
    if(!trig_has_fired) return;
    int content = h_dimu_fired->GetBinContent(dimu_idx+1);
    h_dimu_fired->SetBinContent(dimu_idx+1, content+1);

    bool muons_are_matched = (mu0->Pt() >= 24); //std::get<0>(thresholds));
    muons_are_matched = (muons_are_matched && (mu1->Pt() >= 20));//std::get<1>(thresholds)));
    muons_are_matched = (muons_are_matched &&
                nttools().triggerTool().dilepton_trigger_match(nt.evt(), mu0, mu1, trigger)); 

    if(muons_are_matched) {
        int content = h_dimu_match->GetBinContent(dimu_idx+1);
        h_dimu_match->SetBinContent(dimu_idx+1, content+1);
        float sf = nttools().get_muon_trigger_scale_factor(*mu0, *mu1, trigger);
        h_di_mu_sf.at(dimu_idx)->Fill(sf);
    }

}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::test_dielectron_trigger(Susy::Electron* el0, Susy::Electron* el1, string trigger,
            int diel_idx)
{
    // did the trigger even fire?
    bool trig_has_fired = nttools().triggerTool().passTrigger(nt.evt()->trigBits, trigger);
    if(!trig_has_fired) return;
    int content = h_diel_fired->GetBinContent(diel_idx+1);
    h_diel_fired->SetBinContent(diel_idx+1, content+1);

    bool electrons_are_matched = nttools().triggerTool().dilepton_trigger_match(nt.evt(), el0, el1, trigger);
    if(electrons_are_matched) {
        int content = h_diel_match->GetBinContent(diel_idx+1);
        h_diel_match->SetBinContent(diel_idx+1, content+1);
        float sf = nttools().get_electron_trigger_scale_factor(*el0, *el1, trigger);
        h_di_el_sf.at(diel_idx)->Fill(sf);
    }

}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::test_mixed_trigger(Susy::Electron* el, Susy::Muon* mu, string trigger, int mix_idx)
{
    // did the trigger even fire?
    bool trig_has_fired = nttools().triggerTool().passTrigger(nt.evt()->trigBits, trigger);
    if(!trig_has_fired) return;
    int content = h_mixed_fired->GetBinContent(mix_idx+1);
    h_mixed_fired->SetBinContent(mix_idx+1, content+1);

    bool leptons_are_matched = nttools().triggerTool().dilepton_trigger_match(nt.evt(), el, mu, trigger);
    if(leptons_are_matched) {
        int content = h_mixed_match->GetBinContent(mix_idx+1);
        h_mixed_match->SetBinContent(mix_idx+1, content+1);
        // don't have SF for mixed triggers yet
    }
}
//////////////////////////////////////////////////////////////////////////////
map<string, float> SusyLepTrigExample::single_muon_trigger_thresholds()
{

    map<string, float> threshold_map;

    vector<string> triggers = nttools().triggerTool().single_muo_triggers();
    string hlt = "HLT_";
    string del = "_";
    string mu = "mu";

    for(auto t : triggers) {
        string trig = t;
        trig.erase(0, hlt.length());
        size_t pos = 0;
        while( (pos = trig.find(del)) != std::string::npos) {
            trig.erase(pos, trig.length());
        } // while
        pos = trig.find(mu);
        trig.erase(pos, mu.length());
        float threshold = std::stoi(trig);
        threshold_map[t] = 1.05*threshold; 
    } // t

    if(dbg() && !single_mu_thresholds_printed) {
        single_mu_thresholds_printed = true;
        cout << "------------------------------------------------------------------------------------" << endl;
        cout << "SusyLepTrigExample::single_muon_trigger_thresholds    Single muon trigger thresholds" << endl; 
        cout << endl;
        for(auto threshold : threshold_map) {
        cout << "SusyLepTrigExample::single_muon_trigger_thresholds    " << threshold.first << " = " << threshold.second << endl;
        }
        cout << "------------------------------------------------------------------------------------" << endl;

    }

    return threshold_map;
}
//////////////////////////////////////////////////////////////////////////////
map<string, pair<float, float> > SusyLepTrigExample::dimuon_trigger_thresholds()
{
    map<string, pair<float, float> > threshold_map;

    vector<string> triggers = nttools().triggerTool().di_muo_triggers();
    string hlt = "HLT_";
    string del = "_";
    string mu = "mu";
    string second_leg = "_mu8noL1";

    for(auto t : triggers) {
        string trig = t;
        size_t pos = trig.find(second_leg);
        trig.erase(pos, second_leg.length());
        trig.erase(0, hlt.length());
        trig.erase(0, mu.length());
        float threshold_leg1 = std::stoi(trig) * 1.05;
        float threshold_leg2 = 8;
        pair<float, float> thmap(threshold_leg1, threshold_leg2);
        threshold_map[t] = thmap;
    } // t

    if(dbg() && !di_mu_thresholds_printed) {
        di_mu_thresholds_printed = true;
        cout << "------------------------------------------------------------------------------------" << endl;
        cout << "SusyLepTrigExample::dimuon_trigger_thresholds    Dimuon trigger thresholds" << endl;
        cout << endl;
        for(auto threshold : threshold_map) {
        cout << "SusyLepTrigExample::dimuon_trigger_thresholds    " << threshold.first << " = (" << std::get<0>(threshold.second) << ", " << std::get<1>(threshold.second) << ")" << endl; 
        }
        cout << "------------------------------------------------------------------------------------" << endl;
    }

    return threshold_map;
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::write_single_mu_histos()
{
    m_out_file->cd();

    TCanvas* c0 = new TCanvas("c_single_mu_fired_matched", "", 800, 600);
    c0->SetGrid(1,1);
    c0->cd();

    float maxy = h_single_mu_fired->GetMaximum();
    if(h_single_mu_match->GetMaximum() > maxy) maxy = h_single_mu_match->GetMaximum();
    maxy = 1.4*maxy;
    h_single_mu_fired->SetMaximum(maxy);
    h_single_mu_match->SetMaximum(maxy);
    h_single_mu_fired->SetMinimum(0);
    h_single_mu_match->SetMinimum(0);

    h_single_mu_fired->Draw("hist");
    h_single_mu_match->Draw("hist same");
    c0->Write();

    for(auto & h : h_single_mu_sf) h->Write();
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::write_single_ele_histos()
{
    m_out_file->cd();

    TCanvas* c0 = new TCanvas("c_single_ele_fired_matched", "", 800, 600);
    c0->SetGrid(1,1);
    c0->cd();

    float maxy = h_single_ele_fired->GetMaximum();
    if(h_single_ele_match->GetMaximum() > maxy) maxy = h_single_ele_match->GetMaximum();
    maxy = 1.4*maxy;
    h_single_ele_fired->SetMaximum(maxy);
    h_single_ele_match->SetMaximum(maxy);
    h_single_ele_fired->SetMinimum(0);
    h_single_ele_match->SetMinimum(0);

    h_single_ele_fired->Draw("hist");
    h_single_ele_match->Draw("hist same");
    c0->Write();

    for(auto & h : h_single_ele_sf) h->Write();
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::write_di_mu_histos()
{
    m_out_file->cd();

    TCanvas* c0 = new TCanvas("c_di_mu_fired_matched", "", 800, 600);
    c0->SetGrid(1,1);
    c0->cd();

    float maxy = h_dimu_fired->GetMaximum();
    if(h_dimu_match->GetMaximum() > maxy) maxy = h_dimu_match->GetMaximum();
    maxy = 1.4*maxy;
    h_dimu_fired->SetMaximum(maxy);
    h_dimu_match->SetMaximum(maxy);
    h_dimu_fired->SetMinimum(0);
    h_dimu_match->SetMinimum(0);

    h_dimu_fired->Draw("hist");
    h_dimu_match->Draw("hist same");
    c0->Write();

    for(auto & h : h_di_mu_sf) h->Write();
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::write_di_el_histos()
{
    m_out_file->cd();
    TCanvas* c0 = new TCanvas("c_di_el_fired_matched", "", 800, 600);
    c0->SetGrid(1,1);
    c0->cd();

    float maxy = h_diel_fired->GetMaximum();
    if(h_diel_match->GetMaximum() > maxy) maxy = h_diel_match->GetMaximum();
    maxy = 1.4*maxy;
    h_diel_fired->SetMaximum(maxy);
    h_diel_match->SetMaximum(maxy);
    h_diel_fired->SetMinimum(0);
    h_diel_match->SetMinimum(0);

    h_diel_fired->Draw("hist");
    h_diel_match->Draw("hist same");
    c0->Write();
    for(auto & h : h_di_el_sf) h->Write();
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::write_mix_histos()
{
    m_out_file->cd();
    TCanvas* c0 = new TCanvas("c_mixed_fired_match", "", 800, 600);
    c0->SetGrid(1,1);
    c0->cd();

    float maxy = h_mixed_fired->GetMaximum();
    if(h_mixed_match->GetMaximum() > maxy) maxy = h_mixed_match->GetMaximum();
    maxy = 1.4*maxy;
    h_mixed_fired->SetMaximum(maxy);
    h_mixed_match->SetMaximum(maxy);
    h_mixed_fired->SetMinimum(0);
    h_mixed_match->SetMinimum(0);

    h_mixed_fired->Draw("hist");
    h_mixed_match->Draw("hist same");
    c0->Write();
}
//////////////////////////////////////////////////////////////////////////////
void SusyLepTrigExample::Terminate()
{
    m_out_file->cd();

    write_single_mu_histos();
    write_di_mu_histos();

    write_single_ele_histos();
    write_di_el_histos();

    write_mix_histos();

    m_out_file->Write();

    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "SusyLepTrigExample::Terminate    Output histograms stored in file: " << m_out_file->GetName() << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
}
