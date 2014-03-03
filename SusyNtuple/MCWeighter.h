#ifndef SusyNtuple_MCWeighter_h
#define SusyNtuple_MCWeighter_h

#include "TChain.h"
#include "TString.h"

#include "SUSYTools/SUSYCrossSection.h"
//#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNt.h"

/*

    MCWeighter - a class to handle the normalization of Monte Carlo

    There are options to control how to retrieve the sumw, xsec, etc.
    The defaults are recommended, as long as they work.

*/

class MCWeighter
{

  public:

    //
    // Enums to control weighting options
    //

    // Sumw method
    enum SumwMethod
    {
      Sumw_NT = 0,      // Use sumw stored in the ntuple
      Sumw_MAP          // Use SumwMap (recommended)
      //Sumw_FILE       // Use a text file for sumw (not supported)
    };

    // Xsec method
    enum XsecMethod
    {
      Xsec_NT = 0,      // Use xsec stored in the ntuple
      Xsec_ST           // Use xsec from SUSYTools (recommended)
    };

    // Systematic
    enum WeightSys
    {
      Sys_NOM = 0,
      Sys_XSEC_UP,
      Sys_XSEC_DN,
      Sys_PILEUP_UP,
      Sys_PILEUP_DN,
      Sys_N
    };

    //
    // Initialization and configuration
    //

    // Constructor and destructor
    MCWeighter(TChain* chain=0, 
               std::string xsecDir="$ROOTCOREDIR/data/SUSYTools/mc12_8TeV/");
    ~MCWeighter();

    // Build a map of MCID -> sumw.
    // This method will loop over the input files associated with the TChain. The MCID
    // in the first entry of the tree will be used, so one CANNOT use this if multiple
    // datasets are combined into one SusyNt tree file! The generator weighted cutflow
    // histograms will then be used to calculate the total sumw for each MCID. Each
    // dataset used here must be complete, they CANNOT be spread out across multiple jobs.
    // However, one can have more than one (complete) dataset in the chain, which is why
    // we use the map.
    void buildSumwMap(TChain* chain);

    // Specify methods to retrieve sumw and xsec
    void setUseProcSumw(bool useProcSumw=true) { m_useProcSumw = useProcSumw; }
    void setSumwMethod(SumwMethod opt=Sumw_MAP) { m_sumwMethod = opt; }
    void setXsecMethod(XsecMethod opt=Xsec_ST) { m_xsecMethod = opt; }

    // MC Weight includes generator, xsec, lumi, and pileup weights
    // Default weight uses 2012 A-D lumi. You can supply a different luminosity,
    // but the pileup weights will still correspond to A-D.
    float getMCWeight(const Susy::Event* evt, float lumi = LUMI_A_L, WeightSys sys=Sys_NOM);

    // Get sumw for this event
    float getSumw(const Susy::Event* evt);
    // Get cross section for this event
    SUSY::CrossSectionDB::Process getCrossSection(const Susy::Event* evt);
    float getXsecTimesEff(const Susy::Event* evt, WeightSys sys=Sys_NOM);
    // Get the pileup weight
    float getPileupWeight(const Susy::Event* evt, WeightSys sys=Sys_NOM);

  private:

    // Utils for checking that a string is an int. See
    // http://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
    std::string rmLeadingTrailingWhitespaces(const std::string& str);
    bool isInt(const std::string& s);

    bool m_useProcSumw;

    // typedefs, for convenience
    typedef std::pair<unsigned int, int> SumwMapKey;
    typedef std::map<SumwMapKey, float> SumwMap;
    typedef std::pair<int, int> intpair;
    typedef std::map<intpair, SUSY::CrossSectionDB::Process> XSecMap;

    SumwMethod m_sumwMethod;
    XsecMethod m_xsecMethod;

    // Map of (MCID, proc) -> sumw
    SumwMap m_sumwMap;

    // SUSYTools cross sections
    SUSY::CrossSectionDB m_xsecDB;
    XSecMap m_xsecCache;

};


#endif
