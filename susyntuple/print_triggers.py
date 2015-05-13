#!/bin/env python
#
# Print a summary of the triggers stored in SusyNt
#
# TODO: clean up directory functionality
# TODO: once sample meta data is stored in SusyNt, print this out in table as well 
#       or potentially split up by sample
#
# daniel.joseph.antrim@cern.ch
# May 7 2015

import os
import sys
import utils
import glob
r = utils.import_root()
r.gROOT.SetBatch(True)
r.gErrorIgnoreLevel=3001 # getting dictiory and SusyNtuple streamer info errors that are not important

def main() :
    utils.load_packages()
    if len(sys.argv) < 2 :
        print("Usage:\n\tinput file: {} susyNt.root\n\tinput dir: {} /<path-to-susyNt-samples>/".format(sys.argv[0], sys.argv[0]))
        return
    input = sys.argv[1]
   
    if is_file(input) : trig_info_from_file(input, False)
    elif is_dir(input) : trig_info_from_dir(input)

def trig_info_from_file(input, dir) :
    # formatting
    col_width = 25
    lstr_col = ('{:<' + str(col_width) + 's}')
    rstr_col = ('{:>' + str(col_width) + 's}')
    num_col  = ('{:>' + str(col_width) + '.2f}')
    fields = ['trigger chain', 'events passing', '% of total']
    header_template = ' '.join([lstr_col] + [rstr_col for f in fields[1:]])
    line_template   = ' '.join([lstr_col] + [num_col for f in fields[1:]])
    header = header_template.format(*fields)
    line_break = '-'*(col_width * len(fields)+1*(len(fields)-1))
    
    input_file = r.TFile.Open(input)
    h_trig_name = 'trig'
    h_trig = input_file.Get(h_trig_name)
    total_number = h_trig.Integral()*1.0
    if not dir :
        print (line_break)
        print (header)
        print (line_break)
    n_trigs = h_trig.GetNbinsX()
    triggers = []
    for itrig in range(h_trig.GetNbinsX()) :
        this_trig = TrigNumbers()
        this_trig.name = h_trig.GetXaxis().GetBinLabel(itrig+1)
        this_trig.events = h_trig.GetBinContent(itrig+1)
        this_trig.percent = this_trig.events / total_number * 100.
        triggers.append(this_trig)
    if dir : return triggers, total_number
    else :
        trig_stats = [{'trigger chain' : t.name,
                       'events passing' : t.events,
                       '% of total' : t.percent
                      } for t in triggers] 
        trig_stats = sorted(trig_stats, key=lambda a: a['events passing'], reverse=True)
        lines = [line_template.format(*(a[k] for k in fields)) for a in trig_stats]
        print ('\n'.join(lines))

def trig_info_from_dir(indir) :
    # This function is pretty redundant, but works

    # formatting
    col_width = 25
    lstr_col = ('{:<' + str(col_width) + 's}')
    rstr_col = ('{:>' + str(col_width) + 's}')
    num_col  = ('{:>' + str(col_width) + '.2f}')
    fields = ['trigger chain', 'events passing', '% of total']
    header_template = ' '.join([lstr_col] + [rstr_col for f in fields[1:]])
    line_template   = ' '.join([lstr_col] + [num_col for f in fields[1:]])
    header = header_template.format(*fields)
    line_break = '-'*(col_width * len(fields)+1*(len(fields)-1))

    #reg_root = "*susyNt*.root"
    reg_root = "*.root"
    input_files = glob.glob(indir + reg_root)
    if len(input_files)==0 :
        print "No files matching {} in input directory. Exitting.".format(reg_root)
        sys.exit()
    # fill first to initialize the list
    triggers, total_number = trig_info_from_file(input_files[0], True)

    print (line_break)
    print (header)
    print (line_break)

    h_trig_name = 'trig'
    for file in input_files[1:] :
        rfile = r.TFile.Open(file)
        h_trig = None
        if rfile.GetListOfKeys().Contains("trig") : h_trig = rfile.Get(h_trig_name)
        if not h_trig : continue
        total_number += h_trig.Integral()*1.0
        
        for itrig in range(h_trig.GetNbinsX()) :
            trig_name = h_trig.GetXaxis().GetBinLabel(itrig+1)
            for t in triggers :
                if t.name == trig_name :
                    t.events += h_trig.GetBinContent(itrig+1)
    for t in triggers :
        t.percent = t.events / total_number * 100.
    trig_stats = [{'trigger chain' : t.name,
                   'events passing' : t.events,
                   '% of total' : t.percent
                  } for t in triggers] 
    trig_stats = sorted(trig_stats, key=lambda a: a['events passing'], reverse=True)
    lines = [line_template.format(*(a[k] for k in fields)) for a in trig_stats]
    print ('\n'.join(lines))
    

class TrigNumbers() :
    "Store some basic information about the triggers"
    def __init__(self) :
        name = ""
        events = 0
        percent = 0.0

def is_file(name) :
    if name.endswith(".root") : return True
    else : return False
def is_dir(name) :
    if name.endswith("/") : return True
    else : return False
  
#####################################  

if __name__=='__main__' :
    main()
