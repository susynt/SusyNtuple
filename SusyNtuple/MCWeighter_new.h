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

//forward
namespace Susy { class Event; }

/// A class to handle the normalization of Monte-Carlo
/*
*/

class MCWeighter
{
    public :

        //
        // Initialization and configuration
        //
        MCWeighter();
        MCWeighter(TTree* tree,
                std::string xsecDir = "$ROOTCOREDIR/data/SUSYTools/mc15_13TeV/");
        virtual ~MCWeighter() {};

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

        


        struct SumwMapKey {
            unsigned int dsid;
            int proc;
            SumwMapKey() : dsid(0), proc(0) {}
            SumwMapKey(unsigned int d, int p) : dsid(d), proc(p) {}
        }; // struct

        typedef std::map<SumwMapKey, float> SumwMap;

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
            Xsec_ST = 0, // use SUSYTools' xsec database [default]
        }

    private :
        int m_dbg;
        SumwMethod m_sumw_method;
        XsecMethod m_xsec_method;

        double m_sumw;
        double m_default_sumw;
        std::string m_sum_file;

        // get sumw
        void buildSumwMapFromTree(TTree* tree);
        void buildSumwMapFromChain(TChain* chain);

        SumwMap m_sumwMap;
        bool m_sumw_map_built;

        // SUSYCrossSection DB
        SUSY::CrossSectionDB m_xsecDB;

}; // class


#endif
