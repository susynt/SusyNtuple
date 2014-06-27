from ROOT import TPySelector
import ROOT as r

class PySelector2Lep(TPySelector):
    def Begin( self ):
        print 'py: beginning'

    def SlaveBegin( self, tree ):
        nttool = r.SusyNtTools()
        isSimplifiedModel = False
        nttool.buildSumwMap(tree, isSimplifiedModel)
        #        nttool.configureBTagTool('')
        self.nttool = nttool
        self.ntevent = r.Susy.SusyNtObject(0)
        self.ntevent.ReadFrom(tree)
        print 'py: slave beginning'
        print "Branches\n"+'\n'.join([b.GetName() for b in tree.GetListOfBranches()])

    def Process( self, entry ):
        if self.fChain.GetEntry( entry ) <= 0:
            return 0
        print 'py: processing ', entry#self.fChain.MyVar
        ntevent = r.Susy.SusyNtObject(entry)
        ntevent.ReadFrom(self.fChain)
        print 'run ', ntevent.evt().run
        sys = 0
        event           = self.fChain.event
        electrons       = self.get_pre_electrons(sys)
        muons           = self.fChain.muons
        jets            = self.fChain.jets
        photons         = self.fChain.photons
        taus            = self.fChain.taus
        met             = self.fChain.met
        truthParticles  = self.fChain.truthParticles
        truthJets       = self.fChain.truthJets
        truthMet        = self.fChain.truthMet
        
        return 1
    def get_pre_electrons(self, sys, min_pt=10.0):
        electrons = self.fChain.electrons
        [e.setState(sys) for e in electrons]
        return [e for e in electrons if e.Pt() > min_pt]
    # def get_pre_muons(self, sys)
    def SlaveTerminate( self ):
        print 'py: slave terminating'

    def Terminate( self ):
        print 'py: terminating'
