#ifndef SusyNtuple_MCWeighter_h
#define SusyNtuple_MCWeighter_h

#include "TChain.h"
#include "TString.h"

#include "SUSYTools/SUSYCrossSection.h"
#include "SusyNtuple/SusyNt.h"

#include <string>
/// A class to handle the normalization of Monte Carlo
/**
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

    /// Helper to keep track of events with  invalid process id
    struct ProcessValidator {
    ProcessValidator() : counts_total(0), counts_invalid(0), max_warnings(4), valid(false), last(0) {}
      size_t counts_total;
      size_t counts_invalid;
      size_t max_warnings;
      /// if necessary, convert our 'unknown' value (-1) to the SUSYTools 'unknown' value (0)
      /**
         Also flag as invalid the suspicious events (i.e. when proc==-1 and proc!=previous_proc)
       */
      ProcessValidator& validate(int &value);
      bool valid; ///< status from the current call to validate()
      int last; ///< procid from the last call to validate()
      std::string summary() const;
    };
    //
    // Initialization and configuration
    //

    // Constructor and destructor
    MCWeighter(TTree* tree,
               std::string xsecDir="$ROOTCOREDIR/data/SUSYTools/mc12_8TeV/");
    ~MCWeighter();

    /// Build a map of MCID -> sumw.
    /**
     This method will loop over the input files associated with the TChain. The MCID
     in the first entry of the tree will be used, so one CANNOT use this if multiple
     datasets are combined into one SusyNt tree file! The generator weighted cutflow
     histograms will then be used to calculate the total sumw for each MCID. Each
     dataset used here must be complete, they CANNOT be spread out across multiple jobs.
     However, one can have more than one (complete) dataset in the chain, which is why
     we use the map.
    */
    void buildSumwMap(TTree* tree);
    void clearAndRebuildSumwMap(TTree* tree) { m_sumwMap.clear(); buildSumwMap(tree); }
    void dumpSumwMap() const;
    void dumpXsecCache() const;
    void dumpXsecDb() const;

    /// Specify methods to retrieve sumw and xsec
    void setUseProcSumw(bool useProcSumw=true) { m_useProcSumw = useProcSumw; }
    void setSumwMethod(SumwMethod opt=Sumw_MAP) { m_sumwMethod = opt; }
    void setXsecMethod(XsecMethod opt=Xsec_ST) { m_xsecMethod = opt; }

    /// MC Weight includes generator, xsec, lumi, and pileup weights
    /** Default weight uses 2012 A-D lumi. You can supply a different luminosity,
        but the pileup weights will still correspond to A-D. */
    float getMCWeight(const Susy::Event* evt, float lumi = LUMI_A_L, WeightSys sys=Sys_NOM);
    bool sumwmapHasKey(SumwMapKey k);

    /// Get sumw for this event
    float getSumw(const Susy::Event* evt);
    /// Get cross section for this event
    SUSY::CrossSectionDB::Process getCrossSection(const Susy::Event* evt);
    float getXsecTimesEff(const Susy::Event* evt, WeightSys sys=Sys_NOM);
    /// Get the pileup weight
    float getPileupWeight(const Susy::Event* evt, WeightSys sys=Sys_NOM);
    /// specify the bin used to compute sumw; requires you to call clearAndRebuildSumwMap.
    /**
       For a list of available bin labels (counters), see SusyNtMaker::makeCutFlow()
    */
    MCWeighter& setLabelBinCounter(const std::string &v);
    /// read additional files containing more cross section values
    /**
       This is to account for samples that are not in the SUSYTools lists.
       Returns the number of cross section values read from the file.
     */
    size_t parseAdditionalXsecFile(const std::string &filename, bool verbose);
    /// same as parseAdditionalXsecFile, but get any *.txt in a given directory
    size_t parseAdditionalXsecDirectory(const std::string &dir, bool verbose);
    /// default counter used to compute the normalization
    /**
       Use "Initial" for all samples, other than Simplified Model for which use "SusyProp Veto"
     */
    static std::string defaultLabelBinCounter() { return "Initial"; }
    /// determine whether a given file is formatted following the CrossSectionDB format
    /**
       The format is essentially 6 words:

       id       name(or final_state)   xsect    kfactor    efficiency    rel.uncertainty

       Empty lines and comments ('#') are skipped.
       For more details, see SUSYTools/SUSYCrossSection.h
    */
    static bool isFormattedAsSusyCrossSection(std::string filename, bool verbose);
    /// given a text file containing cross sections for CrossSectionDB, return the dsids
    static std::vector<int> readDsidsFromSusyCrossSectionFile(std::string filename, bool verbose);
 private:
    void buildSumwMapFromTree(TTree* tree);
    void buildSumwMapFromChain(TChain* chain);

    bool m_useProcSumw;

    // typedefs, for convenience
    typedef std::pair<int, int> intpair;
    typedef std::map<intpair, SUSY::CrossSectionDB::Process> XSecMap;

    SumwMethod m_sumwMethod;
    XsecMethod m_xsecMethod;

    // Map of (MCID, proc) -> sumw
    SumwMap m_sumwMap;

    /// SUSYTools cross sections
    SUSY::CrossSectionDB m_xsecDB;
    XSecMap m_xsecCache;

    std::string m_labelBinCounter; ///< label of the bin (from the SusyNt histos) used to determine sumw
    size_t m_warningCounter;
    ProcessValidator m_procidValidator; ///< validate susy process id
};


#endif
