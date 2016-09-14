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
# The output *.txt file can then be used as input
# to the CheckWeights executable
# (c.f. SusyNtuple/CheckWeights.h)
# to check the xsec information and whether or
# not it can be found in the SUSYTools xsec
# database.
#
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
# The "0001.susyNt.root" file would then be
# included in the output *.txt file.
#
# daniel.joseph.antrim@cern.ch
# September 2016
###################################################

from optparse import OptionParser
import os
import sys

import glob

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

    filelists = get_filelists(input_directory)
    first_files = get_first_files(filelists)

    ofilename = "xsec_check_filelist.txt"
    print "Creating output file: %s"%ofilename 
    ofile = open(ofilename, 'w')
    for f in first_files :
        line = f + "\n"
        ofile.write(line)
    ofile.close()
    
