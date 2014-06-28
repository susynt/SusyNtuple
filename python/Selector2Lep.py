from ROOT import TPySelector
import ROOT as r

class TauID:
    none = 0
    loose = 1
    medium = 2
    tight = 3

class PySelector2Lep(TPySelector):
    def Init(self, tree):
        self.nttool = r.SusyNtTools()
        self.m_entry = 0
        # self.m_entry = r.Long64_t(-1)
        self.ntevent = r.Susy.SusyNtObject(self.m_entry)
        self.ntevent.ReadFrom(tree)
    def Begin( self ):
        print 'py: beginning'

    def SlaveBegin( self, tree ):
        nttool = self.nttool
        isSimplifiedModel = False
        nttool.buildSumwMap(tree, isSimplifiedModel)
        # self.ntevent = r.Susy.SusyNtObject(0)
        # self.ntevent.ReadFrom(tree)
        #        nttool.configureBTagTool('')
        print 'py: slave beginning'
        print "Branches\n"+'\n'.join([b.GetName() for b in tree.GetListOfBranches()])

    def Process( self, entry ):
        if self.fChain.GetEntry( entry ) <= 0:
            return 0
        print 'py: processing ', entry
        ntevent = self.ntevent
        ntevent.m_entry = entry
        print 'IsAvailable ',ntevent.evt.IsAvailable()
        print 'run ', ntevent.evt().run,' event ',ntevent.evt().event
        print 'ele ',ntevent.ele()
        print 'ele.size() ',ntevent.ele().size()
        print [e for e in ntevent.ele()]
        sys = 0
        pre_electrons = self.nttool.getPreElectrons(ntevent, sys)
        pre_muons     = self.nttool.getPreMuons(ntevent, sys)
        pre_taus      = self.nttool.getPreTaus(ntevent, sys)
        pre_jets      = self.nttool.getPreJets(ntevent, sys)
        self.nttool.performOverlap(pre_electrons, pre_muons, pre_taus, pre_jets)

        # print pre_electrons
        # print type(pre_electrons)
        # print [e for e in pre_electrons]
        # event           = self.fChain.event
        # electrons       = self.get_pre_electrons(sys)
        # muons           = self.get_pre_muons(sys)
        # jets            = self.get_pre_jets(sys)
        # taus            = self.get_pre_taus(sys)

        # self.nttool.performOverlap(electrons, muons, taus, jets)
        # met             = self.fChain.met
        # truthParticles  = self.fChain.truthParticles
        # truthJets       = self.fChain.truthJets
        # truthMet        = self.fChain.truthMet

        return 1
    def get_pre_electrons(self, sys, min_pt=10.0):
        electrons = self.fChain.electrons
        [e.setState(sys) for e in electrons]
        return [e for e in electrons if e.Pt() > min_pt]
    def get_pre_muons(self, sys, min_pt=10.0, n0150BugFix=False):
        muons = self.fChain.muons
        [m.setState(sys, n0150BugFix) for m in muons]
        return [m for m in muons if m.Pt() > min_pt]
    def get_pre_jets(self, sys, min_pt=20):
        jets = self.fChain.jets
        print jets
        [j.setState(sys) for j in jets]
        return [j for j in jets if j.Pt() > min_pt]
    def get_pre_taus(self, sys, min_pt=20.0,
                     tauJetId=TauID.loose, tauEleId=TauID.medium, tauMuoId=TauID.medium):
        taus = self.fChain.taus
        [t.setState(sys) for t in taus]
        return [t for t in taus if t.Pt() > min_pt and self.nttool.isTauBDT(t, tauJetId, tauEleId, tauMuoId)]

    def SlaveTerminate( self ):
        print 'py: slave terminating'

    def Terminate( self ):
        print 'py: terminating'
