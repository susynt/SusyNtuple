#!/bin/env python

import glob
import os
import ROOT as r

r.gROOT.SetBatch(1)
r.PyConfig.IgnoreCommandLineOptions = True # don't let root steal your cmd-line options
rootcoredir = os.environ['ROOTCOREDIR']
r.gROOT.LoadMacro(rootcoredir+'/scripts/load_packages.C+')
r.load_packages()

from ROOT import SusyNtAna

def main():

    sample_name = 'Sherpa_CT10_lllnu_WZ'
    input_dir = '/var/tmp/susynt_dev/data/ntup_susy/'
    #input_dir = '/var/tmp/susynt_dev/data/ntup_common/'
    input_files = glob.glob(os.path.join(input_dir, '*.root*'))


    chain = r.TChain('susyNt')
    for f in input_files : chain.Add(f)
    num_entries = chain.GetEntries()
    num_entries_to_process = num_entries if num_entries<1e4 else int(1e4)
    print "About to loop on %d entries"%num_entries_to_process
    # aa = r.SusyNtAna()
    # aa.clearObjects()
    # aa.dumpEvent()
    # aa = Foo()
    # chain.Process(aa) #, sample_name, 100)
    chain.Process('TPySelector', 'Selector2Lep', 10)

    # cutflow = r.Susy2LepCutflow()
    # cutflow.setDebug(True)
    # cutflow.setSampleName(sample_name)

    # chain.Process(cutflow, sample_name, num_entries_to_process)

class Foo(r.TSelector):
    def __inif__(self):
        print "Foo ctor"
        # self.nttools = r.SusyNtTools()
        # self.nttools.setAnaType(0, True) # Ana_2Lep
    def Version(self):
        return 2
    def Begin(self, tree):
        print "being"
    def Terminate(self):
        print "terminate"
    def Process(self, entry):
        print "process ",entry

if __name__=='__main__':
    main()
