#ifndef SusyNtuple_Susy2LepCutflow_h
#define SusyNtuple_Susy2LepCutflow_h

//ROOT
#include "TTree.h"
#include "TChain.h"

//SusyNtuple
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/SusyNtTools.h"

//std/stl
#include <fstream>

/////////////////////////////////////////////////////////////
//
// Susy2LepCutflow
//
// Analysis (TSelector) looper with AnalysisType::Ana_2Lep
// base selection and several signal-region like selections
// applied
//
// daniel.joseph.antrim@cern.ch
// July 2017
//
/////////////////////////////////////////////////////////////

// for TSelector analysis loopers processing susyNt you MUST inherit from SusyNtAna
// in order to pick up the susyNt class objects
class Susy2LepCutflow : public SusyNtAna
{

    public :
        Susy2LepCutflow();
        virtual ~Susy2LepCutflow() {};

        void set_debug(int dbg) { m_dbg = dbg; }
        int dbg() { return m_dbg; }

        void set_chain(TChain* chain) { m_input_chain = chain; }
        TChain* chain() { return m_input_chain; }

        ////////////////////////////////////////////
        // analysis methods
        ////////////////////////////////////////////
        void initialize_counters();

        // standard ATLAS event cleaning
        bool passEventCleaning(const MuonVector& preMuons, const MuonVector& baseMuons,
                const JetVector& baseJets);


        ////////////////////////////////////////////
        // TSelector methods override
        ////////////////////////////////////////////
        virtual void Begin(TTree* tree); // Begin is called before looping on entries
        virtual Bool_t Process(Long64_t entry); // Main event loop function called on each event
        virtual void Terminate(); // Terminate is called after looping has finished

        ////////////////////////////////////////////
        // helper
        ////////////////////////////////////////////

        struct EventCleaning {
            EventCleaning() :
                grl(0),
                lar(0),
                tile(0)
            {}

            uint grl;
            uint lar;
            uint tile;
        };




    private :
        int m_dbg;
        TChain* m_input_chain; // the TChain object we are processing

        ////////////////////////////////////////////
        // counters
        ////////////////////////////////////////////
        uint          n_readin; // total events processed
        EventCleaning event_cleaning_counters;
        


}; //class


#endif
