# utility functions for SusyNt python access

# davide.gerbaudo@gmail.com
# June 2014

import operator
import os
import re

def rootcoredir():
    return os.environ['ROOTCOREDIR']

def import_root():
    import ROOT as r
    r.gROOT.SetBatch(1)
    r.PyConfig.IgnoreCommandLineOptions = True # don't let root steal your cmd-line options
    return r

r = import_root()
def load_packages():
    "Equivalent to RootCore's load_packages.C"
    rootcoredir = os.environ['ROOTCOREDIR']
    for l in open(os.path.join(rootcoredir, 'preload')):
        r.gSystem.Load(l.strip())
    for l in open(os.path.join(rootcoredir, 'load')):
        r.gSystem.Load('lib%s'%l.strip())
    #r.gROOT.LoadMacro(rootcoredir+'/scripts/load_packages.C+')
    #r.load_packages()

def generate_dicts():
    'generate missing dicts to access SusyNtuple objects'
    wd = os.getcwd()
    cpp_dir = 'tmp_cpp'
    r.gSystem.ChangeDirectory(cpp_dir)
    dict_macro = 'linkdef.cxx'
    r.gROOT.LoadMacro(dict_macro+'+')
    r.gSystem.ChangeDirectory(wd)

class Memoize :
    """A class to cache cpu-intensive functions.
    Arguments must be hashable.
    See for example
    http://stackoverflow.com/questions/1988804/what-is-memoization-and-how-can-i-use-it-in-python
    """
    def __init__(self, f) :
        self.f = f
        self.memo = {}
    def __call__(self, *args) :
        if not args in self.memo : self.memo[args] = self.f(*args)
        return self.memo[args]

def dict_keys_sorted_by_value(aDict={}) :
    "Given a dict, return its keys sorted by their values"
    return [x[0] for x in sorted(aDict.iteritems(), key=operator.itemgetter(1))]

def enum_from_header(filename, enumName) :
    """
    Given a c header file, extract the enum as a dict of key:values.
    From:
    https://mail.python.org/pipermail/python-list/2009-August/548422.html
    Modified to also get the enum name.
    """
    verbose = False
    file_data = open(filename).read()
    # Remove comments and preprocessor directives
    file_data = ' '.join(line.split('//')[0].split('#')[0] for line in file_data.splitlines())
    file_data = ' '.join(re.split(r'\/\*.*?\*\/', file_data))
    # Look for enums: In the first { } block after the keyword "enum"
    enums = [(text.split('{')[0].replace('enum','').strip(), text.split('{')[1].split('}')[0])
             for text in re.split(r'\benum\b', file_data)[1:]]
    enum = dict()
    for enum_name, enum_keyvals in enums:
        last_value = -1
        for key_name in enum_keyvals.split(','):
            if '=' in key_name:
                key_name, key_value = key_name.split('=')
                key_value = int(str(eval(key_value)), 0) # int(str()) to catch shift << as well
            else:
                key_value = last_value + 1
            last_value = key_value
            key_name = key_name.strip()
            if enum_name == enumName : enum[key_name] = key_value
            if verbose : print '%s = %d' % (key_name, key_value)
        if verbose : print
    return enum

def import_SUSYDefs_enums():
    """
    Parse the header SusyDefs.h; then with a metaclass make the enums
    available within this module. In this way I don't need to add all
    the lines to linkdef. The values will be available as:

    utils.TauID.TauID_loose
    """
    enums = ['TauID', 'AnalysisType', 'SusyNtSys', 'BTagSys']
    for e_name in enums:
        e_vals = enum_from_header(os.path.join(rootcoredir(), 'include/SusyNtuple/SusyDefs.h'), e_name)
        enum_expr = "class %s:\n\t%s\n"%(e_name, '\n\t'.join(["%s = %d"%(k, e_vals[k])
                                                              for k in dict_keys_sorted_by_value(e_vals)]))
        exec enum_expr in globals()
import_SUSYDefs_enums()
