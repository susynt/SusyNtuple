#!/bin/env python
#
# Example showing how to read a SusyNt ntuple and call the SusyNtTools functions
#
# davide.gerbaudo@gmail.com
# June 2014

import glob
import os
import utils
r = utils.import_root()
from cutflow import SkipEvent, Cutflow

def main():
    utils.load_packages()
    utils.generate_dicts()
    utils.import_SUSYDefs_enums()

    sample_name = 'mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad'
    # input_dir = '/var/tmp/susynt_dev/data/ntup_susy/'
    # #input_dir = '/var/tmp/susynt_dev/data/ntup_common/'
    # input_files = glob.glob(os.path.join(input_dir, '*.root*'))
    input_files = ['./susyNt.root']
    chain = r.TChain('susyNt')
    for f in input_files : chain.Add(f)
    num_entries = chain.GetEntries()
    num_entries_to_process = num_entries if num_entries<1e4 else int(1e4)
    print "About to loop on %d entries"%num_entries_to_process
    run_with_chain(chain, num_entries_to_process)

def run_with_chain(tree, n_max_entries=-1):
    nttool = r.SusyNtTools()
    m_entry = r.Long(-1)
    ntevent = r.Susy.SusyNtObject(m_entry)
    ntevent.ReadFrom(tree)
    isSimplifiedModel = False
    period, useRewUtils = 'Moriond', False
    trig_logic = r.DilTrigLogic(period, useRewUtils)
    n_entries_to_print = 4
    sys = utils.SusyNtSys.NOM
    tauId = utils.TauID
    tauJetId, tauEleId, tauMuoId = tauId.TauID_loose, tauId.TauID_medium, tauId.TauID_medium
    cutflow = Cutflow()
    for iEntry, entry in enumerate(tree):
        m_entry = iEntry
        if n_max_entries>0 and m_entry >= n_max_entries : break
        if iEntry < n_entries_to_print : print 'run ', ntevent.evt().run,' event ',ntevent.evt().event
        pre_elecs  = nttool.getPreElectrons(ntevent, sys)
        pre_muons  = nttool.getPreMuons(ntevent, sys)
        pre_taus   = nttool.getPreTaus(ntevent, sys)
        pre_jets   = nttool.getPreJets(ntevent, sys)
        nttool.performOverlap(pre_elecs, pre_muons, pre_taus, pre_jets)
        nttool.removeSFOSPair(pre_elecs, 12.0)
        nttool.removeSFOSPair(pre_muons, 12.0)
        rmLepsFromIso = False
        n_vertices = ntevent.evt().nVtx
        is_mc = ntevent.evt().isMC
        sig_elecs = nttool.getSignalElectrons(pre_elecs, pre_muons, n_vertices, is_mc, rmLepsFromIso)
        sig_muons = nttool.getSignalMuons(pre_muons, pre_elecs, n_vertices, is_mc, rmLepsFromIso)
        sig_taus = nttool.getSignalTaus(pre_taus, tauJetId, tauEleId, tauMuoId)
        sig_jets = nttool.getSignalJets(pre_jets, sys)
        sig_jets2l = nttool.getSignalJets2Lep(pre_jets, sys)
        met = nttool.getMet(ntevent, sys)
        pre_lep, sig_lep = r.LeptonVector(), r.LeptonVector()
        nttool.buildLeptons(pre_lep, pre_elecs, pre_muons)
        nttool.buildLeptons(sig_lep, sig_elecs, sig_muons)
        if iEntry<n_entries_to_print:
            print 'pre_lep:\n','\n'.join(["[%d] %s (eta,phi,pt) = (%.3f, %.3f, %.3f)"
                                          %
                                          (iL, "mu" if l.isMu() else "el", l.Eta(), l.Phi(), l.Pt())
                                          for iL, l in enumerate(pre_lep)])
        event_flag = ntevent.evt().cutFlags[0]
        def mll(leps):
            return (leps[0] + leps[1]).M()
        try:
            cutflow.cut_if(False, 'input')
            cutflow.cut_if(not nttool.passLAr(event_flag), 'lar')
            cutflow.cut_if(not nttool.passBadJet(event_flag), 'bad_jet')
            cutflow.cut_if(not nttool.passBadMuon(event_flag), 'bad_mu')
            cutflow.cut_if(not nttool.passCosmic(event_flag), 'cosmic')
            cutflow.cut_if(not pre_lep.size()==2, '2lep')
            cutflow.cut_if(not trig_logic.passDilTrig(pre_lep, met.Et, ntevent.evt()), 'trigger')
            cutflow.cut_if(not mll(pre_lep)>20.0, 'mll20')
        except SkipEvent:
            continue
    print '\n'+8*'-'+' cutflow '+8*'-'
    print cutflow

if __name__=='__main__':
    main()
