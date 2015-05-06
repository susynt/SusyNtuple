#!/bin/env python
#
# Print a summary of the branch size.
#
# Used to monitor how we're doing on the SusyNt size at each production
# See also the AOD equivalent PyUtils/PoolFile.py used by AtlasCore/checkFile.py 
# Note to self: root provides three ways to read the size
# - GetTotalSize (uncompressed, including overhead)
# - GetTotBytes(uncompressed)
# - GetZipBytes(compressed)
# All we care about is how many kb/event it takes on disk (compressed).
#
# davide.gerbaudo@gmail.com
# May 2015

import os
import sys
import utils
r = utils.import_root()
r.gROOT.SetBatch(True)

def main():
    utils.load_packages()
    if len(sys.argv)<2:
        print("Usage:\n{} susyNt1.root [susyNt2.root ...]".format(sys.argv[0]))
        return
    input_file_names = sys.argv[1:]
    tree_name = 'susyNt'
    col_width = 20
    lstr_col = ('{:<'+str(col_width)+'s}')
    rstr_col = ('{:>'+str(col_width)+'s}')
    num_col  = ('{:>'+str(col_width)+'.2f}')
    fields = ['branch_name', 'kb/event']
    # probably we do not need the in-memory footprintl; uncomment if you do
    # fields = ['branch_name', 'kb/event', 'memory kb/event']
    header_template = ' '.join([lstr_col]+[rstr_col for f in fields[1:]])
    line_template   = ' '.join([lstr_col]+[num_col for f in fields[1:]])
    header = header_template.format(*fields)
    line_break = '-'*(col_width*len(fields)+1*(len(fields)-1))

    for input_file_name in input_file_names:
        input_file = r.TFile.Open(input_file_name)
        tree = input_file.Get(tree_name)
        stat_info = os.stat(input_file_name)
        num_entries = tree.GetEntries()
        per_event = 1.0 / (Units.kb *num_entries)
        branches = tree.GetListOfBranches()
        print ("{} size on disk {:.2f} kb ({} entries)".format(
                input_file_name,
                os.path.getsize(input_file_name)/Units.kb,
                num_entries))
        print (line_break)
        print (header)
        print (line_break)
        branch_attributes = [{'branch_name':     b.GetName(),
                              'kb/event':        b.GetZipBytes()*per_event,
                              'memory kb/event': b.GetTotalSize()*per_event
                              }
                             for b in branches]
        branch_attributes = sorted(branch_attributes, key=lambda a: a['kb/event'])
        lines = [line_template.format(*(a[k] for k in fields)) for a in branch_attributes]
        print ('\n'.join(lines))
        branch_attributes.append({'branch_name':     'total',
                                  'kb/event':        sum(a['kb/event'] for a in branch_attributes),
                                  'memory kb/event': sum(a['memory kb/event'] for a in branch_attributes)})
        print(line_break)
        print (line_template.format(*[branch_attributes[-1][k] for k in fields]))
        input_file.Close()


class Units(object):
    "Both os and root use bytes. Convert to something more human."
    kb = 1024.
    Mb = 1024.*1024.

if __name__=='__main__':
    main()
