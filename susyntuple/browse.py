#!/bin/env python
#
# Load the necessary dictionaries/enums and start a TBrowser
#
# davide.gerbaudo@gmail.com
# Dec 2014

import sys
import utils
r = utils.import_root()
r.gROOT.SetBatch(False)

def main():
    utils.load_packages()
    utils.generate_dicts()
    utils.import_SUSYDefs_enums()

    input1 = sys.argv[1]
    input_file = r.TFile.Open(input1)
    tree = input_file.Get('susyNt')
    tree.StartViewer()
    answ = ''
    while answ != 'y':
        answ = raw_input('Quit?')

if __name__=='__main__':
    main()
