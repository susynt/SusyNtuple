#!/bin/env python
#
# Given a list of susynt files, report the ones where events were flagged as having 'susypropagator' events.
#
# 
#
# davide.gerbaudo@gmail.com
# Oct 2014

import glob
import math
import optparse
import os
import sys
import utils
r = utils.import_root()
from plot_comparison import parse_input

usage="""
    %prog input1 [input2 ...] [options]
    where
    input1, input2, ... are SusyNt files or dirs or filelist.txt
    Example:

    %prog \
    /gdata/atlas/ucintprod/SusyNt/susy_n0155/*/*root \
    -v \
    """
# general settings
histoname='rawCutFlow'
initial_label = 'Initial' # see SusyNtMaker::makeCutFlow()
susyprop_label = 'SusyProp Veto'

def main():
    utils.load_packages()
    parser = optparse.OptionParser(usage=usage)
    parser.add_option('-v', '--verbose', action='store_true', default=False)
    (opts, args) = parser.parse_args()
    if not args_are_valid(args) : parser.error('invalid arguments')
    verbose = opts.verbose

    input_files = [i for ia in args for i in parse_input(ia)]
    allOpts = [x.dest for x in parser._get_all_options()[1:]]
    if opts.verbose :
        print '\nUsing the following opts:\n'
        print '\n'.join("%s : %s"%(o, str(getattr(opts, o))) for o in allOpts)
    file_counters = {'processed':0, 'flagged':0}
    event_counters = {'processed':0, 'flagged':0}
    for input_file in input_files:
        input_counts, filtered_counts = initial_counts_differ_from_susyprop_counts(input_file, verbose=opts.verbose)
        flagged = input_counts!=filtered_counts
        if flagged:
            file_counters['flagged'] += 1
            print "{0}!={1}, {2}".format(input_counts, filtered_counts, input_file)
        file_counters['processed'] += 1
        event_counters['processed'] += input_counts
        event_counters['flagged'] += (input_counts-filtered_counts)
    print "Processed {0} inputs, {1} have events flagged with {2}".format(file_counters['processed'],
                                                                          file_counters['flagged'],
                                                                          susyprop_label)
    print "Fraction of events flagged: {0:.2%}".format(event_counters['flagged']
                                                       /
                                                       event_counters['processed'])

def args_are_valid(args):
    return (len(args)>=1 and
            all(os.path.exists(a) for a in args))

                      
def initial_counts_differ_from_susyprop_counts(filename, verbose=False):
    initial_counts, susyprop_counts = 0, 0
    input_file = r.TFile.Open(filename)
    if input_file and input_file.IsOpen():
        histo = input_file.Get(histoname)
        if histo:
            xax = histo.GetXaxis()
            initial_counts = histo.GetBinContent(xax.FindBin(initial_label))
            susyprop_counts = histo.GetBinContent(xax.FindBin(susyprop_label))
            counts_are_different = susyprop_counts!=initial_counts
        elif verbose : print "cannot get histo '{0}' from '{1}'".format(histoname, filename)
    elif verbose : print "cannot open '{0}'".forma(filename)
    return initial_counts, susyprop_counts

if __name__=='__main__':
    main()
