#!/bin/env python
#
# Compare a few distributions for two sets of input files.
# Originally written to compare ntup_susy and ntup_common
#
# davide.gerbaudo@gmail.com
# June 2014

import glob
import math
import optparse
import os
import sys
import utils
r = utils.import_root()
from cutflow import SkipEvent, Cutflow

usage="""
    %prog input1 input2 output_dir [options]
    where
    input1, input2 are either directories or root files or filelist.txt
    output_dir is the directory where the plots will be made
    Example:

    plot_comparison.py \
    /var/tmp/susynt_dev/data/ntup_susy/ \
    /var/tmp/susynt_dev/data/ntup_common/ \
    out_plot/ \
    -v \
    --fill \
    --labels susy,common
    """

def main():
    utils.load_packages()
    utils.generate_dicts()
    utils.import_SUSYDefs_enums()
    parser = optparse.OptionParser(usage=usage)
    parser.add_option('-f', '--fill', action='store_true', default=False, help='force filling histos')
    parser.add_option('-l', '--labels', help='labels for plots (comma separated)')
    parser.add_option('-n', '--normalize', action='store_true', help='normalize histograms to unity')
    parser.add_option('-N', '--num-entries', help='loop only over N entries')
    parser.add_option('-s', '--sample', help='sample name')
    parser.add_option('-v', '--verbose', action='store_true', default=False)
    (opts, args) = parser.parse_args()
    print 'labels ',opts.labels
    if not args_are_valid(args) : parser.error('invalid arguments')
    verbose = opts.verbose

    input1 = args[0]
    input2 = args[1]
    out_dir = args[2]
    histo_cache_filename = os.path.join(out_dir, 'plot_comparison.root')
    fill_histos = opts.fill or not os.path.exists(histo_cache_filename)
    allOpts = [x.dest for x in parser._get_all_options()[1:]]
    if opts.verbose :
        print '\nUsing the following opts:\n'
        print '\n'.join("%s : %s"%(o, str(getattr(opts, o))) for o in allOpts)
        print '\n'.join(["%s : %s"%(o, str(eval(o))) for o in ['histo_cache_filename', 'fill_histos']])
    if fill_histos :
        run_fill(input1, input2, histo_cache_filename, opts)
    run_plot(histo_cache_filename, out_dir, opts)

def args_are_valid(args):
    input1 = args[0] if len(args)>0 else None
    input2 = args[1] if len(args)>1 else None
    out_dir = args[2] if len(args)>2 else None
    return (len(args)==3 and
            os.path.exists(input1) and
            os.path.exists(input2) and
            utils.is_valid_output_dir(out_dir))

def parse_input(name, opts):
    verbose = opts.verbose
    result = []
    if os.path.isfile(name) and '.root' in name:
        result = [name]
    elif os.path.isfile(name) and '.txt' in name:
        result = [l.strip() for l in open(name).readlines() if '.root' in l]
    elif os.path.isdir(name):
        result = glob.glob(os.path.join(name, '*.root*'))
    if verbose : print "parsed {0} files from input {1}".format(len(result), name)
    return result

def run_fill(input1, input2, histos_filename, opts):
    sample_name = opts.sample
    num_entries = int(opts.num_entries) if opts.num_entries else None
    histos1 = book_histos(suffix='_1')
    histos2 = book_histos(suffix='_2')
    fill_histos(parse_input(input1, opts), histos1, max_num_entries=num_entries, verbose=opts.verbose)
    fill_histos(parse_input(input2, opts), histos2, max_num_entries=num_entries, verbose=opts.verbose)
    write_dict_or_obj(histos_filename, {'h1':histos1, 'h2':histos2})

def book_histos(suffix='', out_file=None):
    if out_file : out_file.cd()
    histos = {}
    suffix = suffix if suffix else ''
    suffix = ('_'+suffix) if suffix and not suffix.startswith('_') else suffix
    histos['el_n_base' ] = r.TH1F('el_n_base'+suffix,  'N base electrons / event; N_{el}', 21, -0.5, 20.5)
    histos['el_n_sign' ] = r.TH1F('el_n_sign'+suffix,  'N signal electrons / event; N_{el}', 21, -0.5, 20.5)
    histos['el_pt' ] = r.TH1F('el_pt'+suffix,  'el p_{T};p_{T} [GeV]', 50, 0.0, 250.0)
    histos['el_eta'] = r.TH1F('el_eta'+suffix, 'el #eta; #eta', 50, -3.0, +3.0)
    histos['el_phi'] = r.TH1F('el_phi'+suffix, 'el #phi; #phi [rad]', 50, -math.pi, +math.pi)
    histos['mu_n_base' ] = r.TH1F('mu_n_base'+suffix,  'N base muons / event; N_{mu}', 21, -0.5, 20.5)
    histos['mu_n_sign' ] = r.TH1F('mu_n_sign'+suffix,  'N signal muons / event; N_{mu}', 21, -0.5, 20.5)
    histos['mu_pt' ] = r.TH1F('mu_pt'+suffix,  '#mu p_{T};p_{T} [GeV]', 50, 0.0, 250.0)
    histos['mu_eta'] = r.TH1F('mu_eta'+suffix, '#mu #eta; #eta', 50, -3.0, +3.0)
    histos['mu_phi'] = r.TH1F('mu_phi'+suffix, '#mu #phi; #phi [rad]', 50, -math.pi, +math.pi)
    for k, v in histos.iteritems() : v.SetDirectory(out_file)
    return histos

def write_dict_or_obj(output_filename, objects={}):
    'write to file a dict (or nested dicts) containing TObjects; move to utils?'
    output_file = r.TFile.Open(output_filename, 'recreate')
    output_file.cd()
    def write(dict_or_obj):
        is_dict = type(dict_or_obj) is dict
        is_writable = hasattr(dict_or_obj, 'Write')
        if is_dict:
            for v in dict_or_obj.values():
                write(v)
        elif is_writable:
            dict_or_obj.Write()
        else:
            print "write_dict_or_obj : warning %s is neither a dict nor writable"%str(dict_or_obj)
    write(objects)
    output_file.Close()

def histo_names(suffix='') :
    def extract_name(dict_or_hist):
        "input must be either a dict or something with 'GetName'"
        isDict = type(dict_or_hist) is dict
        return dict([(k, extract_name(v))
                     for k,v in dict_or_hist.iteritems()]) if isDict else dict_or_hist.GetName()
    return extract_name(book_histos(suffix=suffix, out_file=None))

def read_dict_or_name(input_file, object_names={}):
    """
    Read from a file a dict (or nested dicts) containing names of
    TObjects. Return a dict with the same structure and keys, but with
    objecs instead of names.
    """
    def read(dict_or_name):
        is_dict = type(dict_or_name) is dict
        if is_dict:
            for k, v in dict_or_name.iteritems():
                dict_or_name[k] = read(v)
            return dict_or_name
        else:
            obj = input_file.Get(dict_or_name)
            if not obj : print "cannot get %s from %s"%(dict_or_name, input_file.GetName())
            return obj
    return read(object_names)

def fill_histos(input_files, histos, tree_name='susyNt', max_num_entries=None, verbose=False):
    "fill histograms (for now unweighted, just to do a per-lepton comparison)"
    chain = r.TChain(tree_name)
    for f in input_files : chain.Add(f)
    num_entries = chain.GetEntries()
    if verbose : print "About to loop on %d entries from %d files"%(num_entries, len(input_files))
    nttool = r.SusyNtTools()
    m_entry = r.Long(-1)
    ntevent = r.Susy.SusyNtObject(m_entry)
    ntevent.ReadFrom(chain)
    isSimplifiedModel = False
    period, useRewUtils = 'Moriond', False
    trig_logic = r.DilTrigLogic(period, useRewUtils)
    n_entries_to_print = 4
    sys = utils.SusyNtSys.NtSys_NOM
    tauId = utils.TauID
    tauJetId, tauEleId, tauMuoId = tauId.TauID_loose, tauId.TauID_medium, tauId.TauID_medium
    cutflow = Cutflow()
    for iEntry, entry in enumerate(chain):
        m_entry = iEntry
        if max_num_entries and iEntry > max_num_entries : break
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
        if verbose and iEntry<n_entries_to_print:
            print "pre_elecs[{0}], pre_muons[{1}] pre_taus[{2}] pre_jets[{3}]".format(len(pre_elecs), len(pre_muons), len(pre_taus), len(pre_jets))
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
            histos['el_n_base'].Fill(len(pre_elecs)) # todo: fix, use base rather than pre
            histos['el_n_sign'].Fill(len(sig_elecs))
            histos['mu_n_base'].Fill(len(pre_muons)) # todo: fix, use base rather than pre
            histos['mu_n_sign'].Fill(len(sig_muons))
            for el in sig_elecs:
                histos['el_pt'].Fill(el.Pt())
                histos['el_eta'].Fill(el.Eta())
                histos['el_phi'].Fill(el.Phi())
            for mu in sig_muons:
                histos['mu_pt'].Fill(mu.Pt())
                histos['mu_eta'].Fill(mu.Eta())
                histos['mu_phi'].Fill(mu.Phi())
        except SkipEvent:
            continue
    print '\n'+8*'-'+' cutflow '+8*'-'
    print cutflow

def run_plot(histo_cache_filename, out_dir, opts):
    if opts.verbose : print 'plotting histograms from ',histo_cache_filename
    histo_names1 = histo_names(suffix='_1')
    histo_names2 = histo_names(suffix='_2')
    input_file = r.TFile.Open(histo_cache_filename)
    histos1 = read_dict_or_name(input_file, histo_names1)
    histos2 = read_dict_or_name(input_file, histo_names2)
    print 'labels ',opts.labels
    two_labels = ('a', 'b') if not opts.labels else (opts.labels.split(',')[0], opts.labels.split(',')[1])
    print 'two_labels', two_labels
    plot_dict_of_histos(histos1, histos2, out_dir, normalize=opts.normalize, two_labels=two_labels)

def book_canvas():
    can = r.TCanvas('can', '')
    can.SetBorderSize(1)
    can.SetBorderMode(0)
    return can

def plot_dict_of_histos(histos1={}, histos2={}, out_dir='./', canvas=None,
                        graphical_attrs={}, normalize=False, key=None, two_labels=('a', 'b')):
    graphical_attrs = (graphical_attrs if graphical_attrs
                       else {'h1': {'line_color':r.kRed,
                                    'line_style':1},
                             'h2': {'line_color':r.kBlue,
                                    'line_style':2,
                                    'line_width':2}
                                    })
    def normalize_histo(h):
        integral = h.Integral()
        if integral: h.Scale(1.0/integral)
    def set_histo_attrs(h, attrs):
        if 'line_color' in attrs : h.SetLineColor(attrs['line_color'])
        if 'line_style' in attrs : h.SetLineStyle(attrs['line_style'])
        if 'line_width' in attrs : h.SetLineWidth(attrs['line_width'])
    def build_legend(histos=[], labels=[], pad=None, opt='lp'):
        legWidth, legHeight = 0.35, 0.25
        rMarg, lMarg, tMarg = pad.GetRightMargin(), pad.GetLeftMargin(), pad.GetTopMargin()
        leg = r.TLegend(1.0 - rMarg - legWidth, 1.0 - tMarg - legHeight, 1.0 - rMarg, 1.0 - tMarg)
        leg.SetBorderSize(1)
        leg.SetFillColor(0)
        leg.SetFillStyle(0)
        for h, l in zip(histos, labels) : leg.AddEntry(h, l, opt)
        return leg
    def plot(h1, h2, can, filename, graphical_attrs, normalize):
        can = book_canvas() if not can else can
        can.cd()
        can.Clear()
        set_histo_attrs(h1, graphical_attrs['h1'])
        set_histo_attrs(h2, graphical_attrs['h2'])
        if normalize: [normalize_histo(h) for h in [h1, h2]]
        h1.SetMinimum(0.0)
        h1.SetMaximum(1.1*max([h.GetMaximum() for h in [h1, h2]]))
        h1.SetStats(0)
        h1.Draw()
        h2.Draw('same')
        labels = ["%s: entries %d, mean %.2f"%(l, h.GetEntries(), h.GetMean()) for l,h in zip(two_labels, [h1, h2])]
        leg = build_legend([h1, h2], labels, can)
        leg.Draw()
        can.Update()
        can.SaveAs(filename)
    h1_is_dict, h2_is_dict = type(histos1) is dict, type(histos1) is dict
    h1_is_hist, h2_is_hist = hasattr(histos1, 'Draw'), hasattr(histos2, 'Draw')
    if h1_is_hist and h2_is_hist:
        plot_name = os.path.join(out_dir, key+'.png')
        plot(histos1, histos2, canvas, plot_name, graphical_attrs, normalize)
    elif h1_is_dict and h2_is_dict:
        keys = histos1.keys()
        for k in keys:
            plot_dict_of_histos(histos1[k], histos2[k], out_dir=out_dir, canvas=canvas,
                                graphical_attrs=graphical_attrs, normalize=normalize,
                                key='_'.join([key, k] if key else [k]), two_labels=two_labels)
    else:
        print "wrong input types (dict or hist): (1) %s (2) %s"%(str(histos1), str(histos2))

if __name__=='__main__':
    main()
