#!/usr/bin/env python

####################################################
# Provide a path to a directory that contains
# *.txt files each of which is a filelist for
# specific MC sample, this script will make a
# text file in the current directory that contains
# on each line a single root file corresponding
# to the first file in each of the *.txt files
# in the user-provided input directory.
#
# Each line of a *.txt file filelist is a single
# susyNt.root file.
#
# For each input file, the xsec*eff*kfactor will be
# checked using MCWeighter::getXsecTimesEff
# which uses the SUSYTools database to get these
# values. The script assumes that the relevant
# information was not found properly in the 
# SUSYTools database if the value returned is
# negative.
#
# example directory structure:
#
#  mc15_13TeV.410000.files.txt
#  mc15_13TeV.387941.files.txt
#  mc15_13TeV.368742.files.txt
#
# example filelist in user-provided directory:
#
#  mc15_13TeV.410000.*.0001.susyNt.root
#  mc15_13TeV.410000.*.0002.susyNt.root
#  mc15_13TeV.410000.*.0003.susyNt.root
#
# daniel.joseph.antrim@cern.ch
# September 2016
###################################################

from optparse import OptionParser
import os
import sys

import glob

import utils
r = utils.import_root()

def get_filelists(inputdir) :
    """
    Gather all of the *.txt files located in the
    directory provided by the user and return them
    as a list
    """

    print "Gathering filelists from %s"%inputdir
    if not inputdir.endswith("/") :
        inputdir = inputdir + "/"
    files = glob.glob(inputdir + "*.txt")
    if len(files)==0 :
        print "get_first_files    ERROR No *.txt filelists found in %s"%inputdir
        sys.exit()
    return files

def get_first_files(filelists = []) :
    """
    For each of the filelists located in the user-provided
    directory, grab the first susyNt.root file. Return the
    list of such files for each filelist.
    """

    first_files = []
    for filelist in filelists :
        lines = open(filelist).readlines()
        found_first_file = False
        for line in lines :
            if found_first_file : break
            if line.startswith("#") : continue
            if not line : continue
            line = line.strip()
            if line.endswith("susyNt.root") :
                found_first_file = True
                first_files.append(line)
        if not found_first_file :
            print "get_first_files    WARNING Could not find first susyNt file in list %s"%filelist
    print 35*"-"
    print "Number of first files expected: %d"%len(filelists)
    print "Number of first files found   : %d"%len(first_files)
    print 18*"- "
    print "N(expected) - N(found)        = %d"%(len(filelists)-len(first_files))
    print 35*"-"

    return first_files

def check_weights(infile, n_checked, n_total) :

    check_string = "check_weights [%d/%d]"%(n_checked, n_total)

    rfile = r.TFile.Open(infile)
    tree = rfile.Get("susyNt")
    m_entry = r.Long(0)
    ntevent = r.Susy.SusyNtObject(m_entry)
    ntevent.ReadFrom(tree)
    sys = utils.SusyNtSys.NOM
    # initialize MCWeighter defaultly, will pick up default xsec ST dir
    weighter = r.MCWeighter()
    weighter.getXsecTimesEff(ntevent.evt(), sys)


    is_bad = False
    for iEntry, entry in enumerate(tree) :
        m_entry = iEntry
        tree.GetEntry(m_entry)
        # for some reason python doesn't like to read in the first event correclty
        # (?) so wait until we have a proper mcid
        if ntevent.evt().mcChannel > 0 :
            check_string += "    checking DSID %d"%ntevent.evt().mcChannel
            xsec_eff = weighter.getXsecTimesEff(ntevent.evt(), sys)

            if xsec_eff < 0 :
                is_bad = True
                check_string += " --> WARNING Missing xsec info for sample!"
            else :
                check_string += " --> OK"
                

            break

    print check_string

    bad_dsid = ""
    if is_bad :
        bad_dsid = "%d"%ntevent.evt().mcChannel

    return bad_dsid

if __name__ == "__main__" :
    parser = OptionParser()
    parser.add_option("-i", "--input", help="Input directory containing text files", default = "")
    (options,args) = parser.parse_args()
    input_directory = options.input

    if input_directory == "" :
        print "ERROR Provided directory is \"\"" 
        sys.exit()

    if not os.path.isdir(input_directory) :
        print "ERROR Provided directory does not exist"
        sys.exit()

    # load in the C++/RootCore packages
    utils.load_packages()

    filelists = get_filelists(input_directory)
    first_files = get_first_files(filelists)

    n_total = len(first_files)
    n_checked = 0

    bad_datasets = []
    for f in first_files :
        n_checked+=1
        check = check_weights(f, n_checked, n_total)
        if not (check=="") :
            bad_datasets.append(check) 

    print 60*"-"
    print "Summary of checking xsec*eff*kfactor for list of files"
    
    if len(bad_datasets)==0 :
        print " > [%d/%d] samples OK"%(n_checked, n_total)
    else :
        n_bad = n_checked - len(bad_datasets)
        print " > [%d/%d] samples OK"%(n_bad, n_total)
        print " -> List of datasets missing proper xsec information:"
        for ds in bad_datasets :
            print "      %s"%ds
    print 60*"-"
