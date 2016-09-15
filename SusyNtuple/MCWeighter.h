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
//
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

        double getMCWeight(const Susy::Event* evt, const float lumi = LUMI_A_A3,
                Susy::NtSys::SusyNtSys sys = Susy::NtSys::NOM, bool includePileup = true);

        double getSumw(const Susy::Event* evt);

        SUSY::CrossSectionDB::Process getCrossSection(const Susy::Event* evt);
        float getXsecTimesEff(const Susy::Event* evt, Susy::NtSys::SusyNtSys sys = Susy::NtSys::NOM);
        float getPileupWeight(const Susy::Event* evt, Susy::NtSys::SusyNtSys sys = Susy::NtSys::NOM);

        


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
        bool isCommentLine(const std::string& line);
        bool isEmptyLine(const std::string& line);

        SumwMap m_sumwMap;
        bool m_sumw_map_built;

        // SUSYCrossSection DB
        SUSY::CrossSectionDB m_xsecDB;
        std::string m_xsecDBdir;
        XSecMap m_xsecCache;

}; // class

      //  // needed for std::map lookup
      //  inline bool operator<(const MCWeighter::SumwMapKey &a, const MCWeighter::SumwMapKey &b)
      //      { return a.dsid<b.dsid && a.proc<b.proc; } 

#endif
