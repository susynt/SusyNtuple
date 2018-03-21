//  -*- c++ -*-
#ifndef SusyCommon_TriggerTools_h
#define SusyCommon_TriggerTools_h

#include <iostream>
#include <vector>
#include <map>
#include "TBits.h"

namespace Susy {
    class Event;
    class Lepton;
}

class TH1;

//typedef std::tuple<int, int, int> DileptonTrigTuple;
typedef uint32_t DileptonTrigTuple; // give 24 bits for trigger idx, 4+4 bits for lepton idx

// pT cut of leptons to consider for dilepton trigger matching in SusyNt
const float DILEPTON_TRIG_MATCH_ELE_PT = 17.; // GeV
const float DILEPTON_TRIG_MATCH_MUO_PT = 17.; // GeV

class TriggerTools {
    public :
        
        ///> Constructor and destructor
        TriggerTools(bool dbg=false);
        virtual ~TriggerTools(){};

        /// initialize by reading the trigger histogram from file
        bool init(const std::string &filename);

        // Method to build the trigger map given the SusyNt object
        void buildTriggerMap(const TH1* trigHisto);

        int idx_of_trigger(std::string trigger);

        std::string trigger_at_idx(int idx);

        // Return a list of all the triggers we store
        static const std::vector<std::string> getTrigNames();

        // Return a list of all the single muon triggers we store
        static const std::vector<std::string> single_muo_triggers();

        // Return  a list of all dimuon triggers we storre
        static const std::vector<std::string> di_muo_triggers();

        // Return a list of all single electron triggers we store
        static const std::vector<std::string> single_ele_triggers();

        // Return a list of all di-electron triggers we store
        static const std::vector<std::string> di_ele_triggers();

        // Return a list of all dilepton ele+mu triggers we store
        static const std::vector<std::string> ele_muo_triggers();

        // Return a list of all MET triggers we store
        static const std::vector<std::string> met_triggers();
    
        // Method to test whether a given trigger is passed
        bool passTrigger(const TBits& triggerbits, const std::string &triggerName) const;

        // Dilepton Trigger Match
        static const float& ele_match_pt() { return DILEPTON_TRIG_MATCH_ELE_PT; }
        static const float& muo_match_pt() { return DILEPTON_TRIG_MATCH_MUO_PT; }

        bool lepton_trigger_match(Susy::Lepton* lep, std::string trigger);
        bool dilepton_trigger_match(Susy::Event* evt, Susy::Lepton* lep0, Susy::Lepton* lep1, std::string trigger="");
    
        // Method to dump the trigger information stored
        void dumpTriggerInfo() const;
    
        // dbg level
        bool m_dbg;
    
    private :
        // trigger histo
        std::map<std::string, int>  m_triggerMap;
        std::map<int, std::string> m_triggerMap_bit;
};
#endif
