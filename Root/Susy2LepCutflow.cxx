#include "SusyNtuple/Susy2LepCutflow.h"

// SusyNtuple
#include "SusyNtuple/KinematicTools.h"
#include "SusyNtuple/SusyDefs.h"
using namespace Susy; // everything in SusyNtuple is in this namespace

//ROOT

// std/stl
#include <iomanip> // setw
#include <iostream>
#include <string>
#include <sstream> // stringstream, ostringstream
using namespace std;

//////////////////////////////////////////////////////////////////////////////
Susy2LepCutflow::Susy2LepCutflow() :
    m_dbg(0),
    m_input_chain(nullptr),
    m_lep_type(DiLepEvtType::ET_Unknown),
    m_mc_weight(1.0)
{
    initialize_counters();
}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::initialize_counters()
{
    n_readin = 0;
}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::Begin(TTree* /*tree*/)
{
    // call base class' Begin method
    SusyNtAna::Begin(0);
    if(dbg()) cout << "Susy2LepCutflow::Begin" << endl;

    return;
}
//////////////////////////////////////////////////////////////////////////////
Bool_t Susy2LepCutflow::Process(Long64_t entry)
{

    // calling "GetEntry" loads into memory the susyNt class objects for this event
    GetEntry(entry);
    SusyNtAna::clearObjects(); // clear the previous event's objects
    // m_ET = ET_Unknown

    // increment for each processed event
    n_readin++;

    // increment the chain entry (c.f. SusyNtuple/SusyNtAna.h)
    m_chainEntry++;

    // evt() provides pointer to the SusyNt::Event class object for this event
    int run_number = nt.evt()->run;
    int event_number = nt.evt()->eventNumber;

    if(dbg() || m_chainEntry%1000==0) {
        cout << "Susy2LepCutflow::Process    **** Processing entry " << setw(6) << m_chainEntry
                << "  run " << run_number << "  event " << event_number << " **** " << endl;
    }

    // SusyNtAna::selectObject fills the baseline and signal objects
    // for the given AnalysisType
    // m_preX    = objects before any selection (as they are in susyNt)
    // m_baseX   = objects with the Analysis' baseline selection AND overlap removal applied
    // m_signalX = objects with the Analysis' signal selection applied (and baseline AND overlap removal)
    SusyNtAna::selectObjects();

    // get the MC weight using the inherited MCWeighter object
    // (c.f. SusyNtuple/MCWeighter.h)
    if(nt.evt()->isMC) {
        float lumi = 100000; // normalize the MC to 100 fb-1
        m_mc_weight = SusyNtAna::mcWeighter().getMCWeight(nt.evt(), lumi, NtSys::NOM);
    }
    else {
        m_mc_weight = 1.; // don't re-weight data
    }

    // check that the event passes the standard ATLAS event cleaning cuts
    if(!passEventCleaning(m_preMuons, m_baseMuons, m_baseJets)) return false;

    // check that the event passes dilepton selection
    if(!passDileptonEvent(m_baseLeptons, m_signalLeptons)) return false;

    // pass mt2 selections
    check_mt2_selections(m_signalLeptons, m_met);

    // pass n-jet selections
    check_njet_selections(m_signalJets);

    // pass some jet and met selections
    check_jetmet_selections(m_signalJets, m_met, m_signalLeptons);


    return kTRUE;
}
//////////////////////////////////////////////////////////////////////////////
bool Susy2LepCutflow::passEventCleaning(const MuonVector& preMuons, const MuonVector& baseMuons,
            const JetVector& baseJets)
{
    int flags = nt.evt()->cutFlags[NtSys::NOM];

    if(!nttools().passGRL(flags))           return false;
    event_cleaning_counters.grl++;
    event_cleaning_counters.grl_w += w();

    if(!nttools().passLarErr(flags))        return false;
    event_cleaning_counters.lar++;
    event_cleaning_counters.lar_w += w();

    if(!nttools().passTileErr(flags))       return false;
    event_cleaning_counters.tile++;
    event_cleaning_counters.tile_w += w();

    if(!nttools().passTTC(flags))           return false;
    event_cleaning_counters.ttc++;
    event_cleaning_counters.ttc_w += w();

    if(!nttools().passSCTErr(flags))        return false;
    event_cleaning_counters.sct++;
    event_cleaning_counters.sct_w += w();

    if(!nttools().passGoodVtx(flags))       return false;
    event_cleaning_counters.vtx++;
    event_cleaning_counters.vtx_w += w();


    ///////////////////////////////////////////////////////
    // for bad muon, cosmic moun, and jet cleaning the
    // cuts depend on the baseline object defintion
    // (and in thec ase of the cosmic muon cut, it also
    // depends on the analysis' overlap removal
    // procedure) -- so we do not use the cutFlags but
    // rather use the objects that have passed the various
    // analysis selections to do the checks
    ///////////////////////////////////////////////////////
    if(!nttools().passBadMuon(preMuons))    return false;
    event_cleaning_counters.bad_mu++;
    event_cleaning_counters.bad_mu_w += w();

    if(!nttools().passCosmicMuon(baseMuons)) return false;
    event_cleaning_counters.cos_mu++;
    event_cleaning_counters.cos_mu_w += w();

    if(!nttools().passJetCleaning(baseJets)) return false;
    event_cleaning_counters.bad_jet++;
    event_cleaning_counters.bad_jet_w += w();

    return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Susy2LepCutflow::passDileptonEvent(const LeptonVector& baseLeptons, const LeptonVector& signalLeptons)
{
    ///////////////////////////////////////////////////////
    // at least two baseline leptons
    ///////////////////////////////////////////////////////
    size_t n_base_leptons = (baseLeptons.size());

    if(!n_base_leptons>=2) return false;
    dilepton_counters.n_baseline++;
    dilepton_counters.n_baseline_w += w();

    ///////////////////////////////////////////////////////
    // exactly two signal leptons
    ///////////////////////////////////////////////////////
    size_t n_signal_leptons = (signalLeptons.size());
    if(n_signal_leptons!=2) return false;

    m_lep_type = getDiLepEvtType(signalLeptons); // c.f. SusyNtuple/SusyDefs.h
    if(m_lep_type == ET_me) m_lep_type = ET_em; // group together all different-flavor
    dilepton_counters.n_signal[m_lep_type]++;
    dilepton_counters.n_signal_w[m_lep_type] += w();

    Susy::Lepton* l0 = signalLeptons.at(0);
    Susy::Lepton* l1 = signalLeptons.at(1);

    ///////////////////////////////////////////////////////
    // only look at opposite sign dilepton events
    ///////////////////////////////////////////////////////
    float is_os = ((l0->q * l1->q) < 0);
    if(!is_os) return false;
    dilepton_counters.n_os[m_lep_type]++;
    dilepton_counters.n_os_w[m_lep_type] += w();

    ///////////////////////////////////////////////////////
    // dilepton invariant mass > 20 GeV
    ///////////////////////////////////////////////////////
    float mll = (*l0 + *l1).M(); // Susy::Lepton inherits from TLorentzVector
    if(! (mll>20. /*GeV*/)) return false;
    dilepton_counters.n_mll[m_lep_type]++;
    dilepton_counters.n_mll_w[m_lep_type] += w();

    ///////////////////////////////////////////////////////
    // veto Z decays
    ///////////////////////////////////////////////////////
    if(m_lep_type == ET_ee || m_lep_type == ET_mm) {
        bool pass_z = (fabs(mll-91.2)>10.);
        if(!pass_z) return false;
    }
    dilepton_counters.n_vetoZ[m_lep_type]++;
    dilepton_counters.n_vetoZ_w[m_lep_type] += w();

    ///////////////////////////////////////////////////////
    // lepton pT > (25, 20) for (lead, sublead)
    ///////////////////////////////////////////////////////
    float lead_lepton_pt = l0->Pt(); // GeV
    float sublead_lepton_pt = l1->Pt(); // GeV

    bool lead_pt_ok = (lead_lepton_pt > 25.);
    bool sublead_pt_ok = (sublead_lepton_pt > 20.);

    if(!(lead_pt_ok && sublead_pt_ok)) return false;
    dilepton_counters.n_pt[m_lep_type]++;
    dilepton_counters.n_pt_w[m_lep_type] += w();

    return true;
}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::check_mt2_selections(const LeptonVector& leptons, const Met* met)
{

    // mt2 > 90 GeV
    float mt2 = kin::getMT2(leptons, met);
    if(mt2>90) {
        dilepton_counters.n_mt290[m_lep_type]++;
        dilepton_counters.n_mt290_w[m_lep_type] += w();
    }

    // mt2 > 120 GeV
    if(mt2>120) {
        dilepton_counters.n_mt2120[m_lep_type]++;
        dilepton_counters.n_mt2120_w[m_lep_type] += w();
    }

    // mt2 > 150 GeV
    if(mt2>150) {
        dilepton_counters.n_mt2150[m_lep_type]++;
        dilepton_counters.n_mt2150_w[m_lep_type] += w();
    }

}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::check_njet_selections(const JetVector& jets)
{
    JetVector bjets;
    JetVector non_bjets;

    // we can use the SusyNtTools object to grab our AnalysisType's
    // JetSelector object to perform the b-tagging selection
    // (c.f. SusyNtuple/JetSelector)
    for(auto & j : jets) {
        if(nttools().jetSelector().isB(j)) bjets.push_back(j);
        else { non_bjets.push_back(j); }
    }

    size_t n_bjets = bjets.size();
    size_t n_nonbjets = non_bjets.size();

    if(n_bjets>=2) {
        dilepton_counters.n_ge2bjets[m_lep_type]++;
        dilepton_counters.n_ge2bjets_w[m_lep_type] += w();
        if(n_bjets==2) {
            dilepton_counters.n_e2bjets[m_lep_type]++;
            dilepton_counters.n_e2bjets_w[m_lep_type] += w();
            if(n_nonbjets==0) {
                dilepton_counters.n_e2bjets0sjets[m_lep_type]++;
                dilepton_counters.n_e2bjets0sjets_w[m_lep_type] += w();
            }
        }
    }

}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::check_jetmet_selections(const JetVector& jets, const Met* met,
            const LeptonVector& leptons)
{
    size_t n_jets = jets.size();
    JetVector bjets;
    JetVector fjets;

    for(auto & j : jets) {
        if(nttools().jetSelector().isB(j)) bjets.push_back(j);
        if(j->Pt() > 30. && fabs(j->Eta()) > 2.4 && fabs(j->Eta() < 4.5)) fjets.push_back(j);
    }

    // require at least 2 jets
    if(n_jets>=2) {
        dilepton_counters.n_ge2jets[m_lep_type]++;
        dilepton_counters.n_ge2jets_w[m_lep_type] += w();
    }
    else { return; }

    // veto forward jets
    if(fjets.size()==0) {
        dilepton_counters.n_forwardJetVeto[m_lep_type]++;
        dilepton_counters.n_forwardJetVeto_w[m_lep_type] += w();
    }
    else { return; }

    // veto b-tagged jets
    if(bjets.size()==0) {
        dilepton_counters.n_bveto[m_lep_type]++;
        dilepton_counters.n_bveto_w[m_lep_type] += w();
    }
    else { return; }

    // require leptons to be close in azimuth 
    Susy::Lepton* l0 = leptons.at(0);
    Susy::Lepton* l1 = leptons.at(1);
    float delta_phi = l0->DeltaPhi(*l1);
    if(fabs(delta_phi) < (M_PI/2.)) {
        dilepton_counters.n_dphill[m_lep_type]++;
        dilepton_counters.n_dphill_w[m_lep_type] += w();
    }
    else { return; }

    // require met > 100 GeV
    float metvalue = m_met->Et;
    if(metvalue>100.) {
        dilepton_counters.n_met100[m_lep_type]++;
        dilepton_counters.n_met100_w[m_lep_type] += w();
    }
    else { return; }

    // require ht > 500
    float ht = 0.0;
    // we can loop over jets, we are downstream from the b- and f-jet vetos
    for(auto & j : jets) ht += j->Pt();
    if(ht>500.) {
        dilepton_counters.n_ht500[m_lep_type]++;
        dilepton_counters.n_ht500_w[m_lep_type] += w();
    }
    else { return; }

    // meff (ht + met + leptons)
    ht += l0->Pt();
    ht += l1->Pt();
    ht += m_met->Et;
    if(ht>500.) {
        dilepton_counters.n_meff500[m_lep_type]++;
        dilepton_counters.n_meff500_w[m_lep_type] += w();
    }
    else { return; }
}
//////////////////////////////////////////////////////////////////////////////
string Susy2LepCutflow::weight_str(float weighted, int counter)
{
    stringstream sx;
    sx << weighted << " [" << counter << "]";
    return sx.str();
}
//////////////////////////////////////////////////////////////////////////////
string Susy2LepCutflow::event_counters()
{
    ostringstream oss;
    oss << "------------------------------------------" << endl;
    oss << " Event Counters " << endl;
    oss << endl;
    oss << "  pass GRL              : " << weight_str(event_cleaning_counters.grl_w, event_cleaning_counters.grl) << endl;
    oss << "  pass LAr error        : " << weight_str(event_cleaning_counters.lar_w, event_cleaning_counters.lar) << endl;
    oss << "  pass Tile error       : " << weight_str(event_cleaning_counters.tile_w, event_cleaning_counters.tile) << endl;
    oss << "  pass TTC veto         : " << weight_str(event_cleaning_counters.ttc_w, event_cleaning_counters.ttc) << endl;
    oss << "  pass SCT error        : " << weight_str(event_cleaning_counters.sct_w, event_cleaning_counters.sct) << endl;
    oss << "  pass good vertex      : " << weight_str(event_cleaning_counters.vtx_w, event_cleaning_counters.vtx) << endl;
    oss << "  pass bad muon         : " << weight_str(event_cleaning_counters.bad_mu_w, event_cleaning_counters.bad_mu) << endl;
    oss << "  pass cosmic muon veto : " << weight_str(event_cleaning_counters.cos_mu_w, event_cleaning_counters.cos_mu) << endl;
    oss << "  pass jet cleaning     : " << weight_str(event_cleaning_counters.bad_jet_w, event_cleaning_counters.bad_jet) << endl;
    oss << "- - - - - - - - - - - - - - - - - - - - - " << endl;
    return oss.str();


}
//////////////////////////////////////////////////////////////////////////////
string Susy2LepCutflow::dil_cut(float* counter_w, uint* counter, int weight)
{
    stringstream sx;
    sx << std::right << setw(weight) << (weight_str(counter_w[0], counter[0]))
       << std::right << setw(weight) << (weight_str(counter_w[1], counter[1]))
       << std::right << setw(weight) << (weight_str(counter_w[2], counter[2]));
    return sx.str();
}
//////////////////////////////////////////////////////////////////////////////
string Susy2LepCutflow::dilepton_counts()
{
    int weight = 20;
    ostringstream oss;
    oss << " Dilepton Counters " << endl;
    oss << endl;
    oss << ">=2 baseline leptons  " << setw(weight) << weight_str(dilepton_counters.n_baseline_w, dilepton_counters.n_baseline) << endl; 
    oss << endl;
    oss << "Selection / Flavor    " << setw(weight) << "EE" << setw(weight) << "MM" << setw(weight) << "EM" << endl;
    oss << "==2 signal leptons    " << setw(weight) << dil_cut(dilepton_counters.n_signal_w, dilepton_counters.n_signal) << endl;
    oss << "opposite sign         " << setw(weight) << dil_cut(dilepton_counters.n_os_w, dilepton_counters.n_os) << endl;
    oss << "mll > 20 GeV          " << setw(weight) << dil_cut(dilepton_counters.n_mll_w, dilepton_counters.n_mll) << endl;
    oss << "Z-veto                " << setw(weight) << dil_cut(dilepton_counters.n_vetoZ_w, dilepton_counters.n_vetoZ) << endl;
    oss << "lep pT > (25, 20) GeV " << setw(weight) << dil_cut(dilepton_counters.n_pt_w, dilepton_counters.n_pt) << endl;
    oss << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    oss << " * SR mT2 90 * " << endl;
    oss << "mt2 > 90 GeV          " << setw(weight) << dil_cut(dilepton_counters.n_mt290_w, dilepton_counters.n_mt290) << endl;
    oss << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    oss << " * SR mT2 120 * " << endl;
    oss << "mt2 > 120 GeV         " << setw(weight) << dil_cut(dilepton_counters.n_mt2120_w, dilepton_counters.n_mt2120) << endl;
    oss << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    oss << " * SR mT2 150 * " << endl;
    oss << "mt2 > 150 GeV         " << setw(weight) << dil_cut(dilepton_counters.n_mt2150_w, dilepton_counters.n_mt2150) << endl;
    oss << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    oss << " * SR b-jets * " << endl; 
    oss << ">=2 b-tagged jets     " << setw(weight) << dil_cut(dilepton_counters.n_ge2bjets_w, dilepton_counters.n_ge2bjets) << endl;
    oss << "==2 b-tagged jets     " << setw(weight) << dil_cut(dilepton_counters.n_e2bjets_w, dilepton_counters.n_e2bjets) << endl;
    oss << "==0 non-b-tagged jets " << setw(weight) << dil_cut(dilepton_counters.n_e2bjets0sjets_w, dilepton_counters.n_e2bjets0sjets) << endl;
    oss << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    oss << " * SR Jets + MET * " << endl;
    oss << ">=2 jets              " << setw(weight) << dil_cut(dilepton_counters.n_ge2jets_w, dilepton_counters.n_ge2jets) << endl;
    oss << "forward jet veto      " << setw(weight) << dil_cut(dilepton_counters.n_forwardJetVeto_w, dilepton_counters.n_forwardJetVeto) << endl;
    oss << "b-jet veto            " << setw(weight) << dil_cut(dilepton_counters.n_bveto_w, dilepton_counters.n_bveto) << endl;
    oss << "dphi_ll < pi/2        " << setw(weight) << dil_cut(dilepton_counters.n_dphill_w, dilepton_counters.n_dphill) << endl;
    oss << "MET > 100 GeV         " << setw(weight) << dil_cut(dilepton_counters.n_met100_w, dilepton_counters.n_met100) << endl;
    oss << "HT > 500 GeV          " << setw(weight) << dil_cut(dilepton_counters.n_ht500_w, dilepton_counters.n_ht500) << endl;
    oss << "MEFF > 500 GeV        " << setw(weight) << dil_cut(dilepton_counters.n_meff500_w, dilepton_counters.n_meff500) << endl;
    oss << "-------------------------------------------------------------" << endl;
    
    

    return oss.str();
}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::print_counters()
{
    cout << event_counters() << endl;
    cout << dilepton_counts() << endl;

}
//////////////////////////////////////////////////////////////////////////////
void Susy2LepCutflow::Terminate()
{
    // print the cutflows
    print_counters();

    // close SusyNtAna and print timers
    SusyNtAna::Terminate();

    return;
}
//////////////////////////////////////////////////////////////////////////////
