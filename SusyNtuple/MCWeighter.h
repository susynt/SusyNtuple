#ifndef SusyNtuple_MCWeighter_h
#define SusyNtuple_MCWeighter_h

//ROOT
#include "TChain.h"
#include "TString.h"

//SUSY
#include "SUSYTools/SUSYCrossSection.h"
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/SusyNtSys.h"


//std/stl
#include <string>
#include <map>

//forward
namespace Susy { class Event; }

/// A class to handle the normalization of Monte-Carlo
/*
*/

class MCWeighter
{
    public :
        struct SumwMapKey {
            unsigned int dsid;
            int proc;
            SumwMapKey() : dsid(0), proc(0) {}
            SumwMapKey(unsigned int d, int p) : dsid(d), proc(p) {}

            bool operator<(const SumwMapKey& rhs) const {
                if(dsid==rhs.dsid) return proc<rhs.proc;
                return (dsid<rhs.dsid);
            }
            bool operator==(const SumwMapKey& rhs) const {
                return (dsid==rhs.dsid && proc==rhs.proc); }

        }; // struct

        typedef std::map<SumwMapKey, float> SumwMap;

        typedef std::pair<int, int> intpair;
        typedef std::map<intpair, SUSY::CrossSectionDB::Process> XSecMap;

        //
        //  Enums to control weighting options
        //
        // sumw method
        enum SumwMethod
        {
            Sumw_NT = 0, // use the sumw information as stored in susyNt (from CutBookKeepers) [default]
            Sumw_FILE    // look up sumw from a user-specified file
        };

        // xsec method
        enum XsecMethod
        {
            Xsec_ST = 0 // use SUSYTools' xsec database [default]
        };


        //
        // Initialization and configuration
        //
        MCWeighter();
        MCWeighter(TTree* tree,
                std::string xsecDir = "$ROOTCOREDIR/data/SUSYTools/mc15_13TeV/");
        virtual ~MCWeighter() {};

        void setVerbose(bool doit) { m_dbg = doit; }
        bool dbg() { return m_dbg; }

        static std::string defaultXsecDir() {
            return std::string("$ROOTCOREBIN/data/SUSYTools/mc15_13TeV/");
        }

        void setSumwFromNT() { m_sumw_method = Sumw_NT; }
        void setSumwFromFILE(std::string file); // { m_sumw_method = Sumw_FILE; }

        SumwMethod& sumwMethod() { return m_sumw_method; }
        XsecMethod& xsecMethod() { return m_xsec_method; }

        void buildSumwMap(TTree* tree);
        static const Susy::Event& readFirstEvent(TTree* tree);

        bool mapHasKey(SumwMapKey& k);

        void printSumwMap() const;

        double getMCWeight(const Susy::Event* evt, const float lumi = 1000,
                Susy::NtSys::SusyNtSys sys = Susy::NtSys::NOM, bool includePileup = true);

        double getSumw(const Susy::Event* evt);

        SUSY::CrossSectionDB::Process getCrossSection(const Susy::Event* evt);
        float getXsecTimesEff(const Susy::Event* evt, Susy::NtSys::SusyNtSys sys = Susy::NtSys::NOM);
        float getPileupWeight(const Susy::Event* evt, Susy::NtSys::SusyNtSys sys = Susy::NtSys::NOM);

        // methods for parsing additional xsec files and checking files        
        size_t parseAdditionalXsecFile(const std::string& filename, bool verbose=false);


        /**
            The format is 6 words:
                dsid    name(or process id)   xsec    kfactor   efficiency   relative-uncertainty
            Empty lines and comments ('#') are skipped.
            c.f. SUSYTools/SUSYCrossSection.h

            If 'is_sumw_file' set to true, will assume a 3 word format:
                dsid     process-id     sumw
            where process-id is 0 for background (non-signal) samples
        */
        static bool isFormattedAsSusyCrossSection(std::string filename, bool verbose=false, bool is_sumw_file=false);
        /// given a cross-section/sumw text file (CrossSectionDB format), return the dsids
        static std::vector<int> readDsidsFromSusyCrossSectionFile(std::string filename, bool verbose=false, bool is_sumw_file=false);
        /// given a line from a cross-section/sumw file, parse the dsid; return false if not found
        static bool readDsidsFromSusyCrossSectionLine(const std::string& line, int& dsid, bool verbose=false, bool is_sumw_file=false);


    private :
        bool m_dbg;
        SumwMethod m_sumw_method;
        XsecMethod m_xsec_method;

        double m_sumw;
        double m_default_sumw;
        std::string m_sumw_file;

        // get sumw
        void buildSumwMapFromTree(TTree* tree);
        void buildSumwMapFromChain(TChain* chain);
        void getSumwFromFile(unsigned int mcid);

        SumwMap m_sumwMap;
        bool m_sumw_map_built;

        // SUSYCrossSection DB
        SUSY::CrossSectionDB m_xsecDB;
        std::string m_xsecDBdir;
        XSecMap m_xsecCache;

}; // class

#endif
