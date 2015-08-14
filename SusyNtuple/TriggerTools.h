#ifndef SusyCommon_TriggerTools_h
#define SusyCommon_TriggerTools_h

#include <iostream>
#include <vector>
#include <map>
#include "TH1F.h"
#include "TBits.h"


////////////////////////////////////////////////////////
// Function to grab the trigger "container"           //
//      Used only at stage of writing the SusyNt      //
////////////////////////////////////////////////////////
std::vector<std::string> getTrigNames(std::string set);


////////////////////////////////////////////////////////
// TriggerTools                                       //
//      To be used at the analyis level of SusyNt     //
////////////////////////////////////////////////////////
class TriggerTools {
public :
    
    ///> Constructor and destructor
    TriggerTools(TChain* input_chain, const bool dbg);
    virtual ~TriggerTools()
    {
    };

    // Method to build the trigger map given the SusyNt object
    //void buildTriggerMap(TChain* susyNt, bool dbg);
    void buildTriggerMap();

    // Method to test whether a given trigger is passed
  //  bool passTrigger(std::map<std::string, int> trigMap, std::string triggerName);
    bool passTrigger(TBits& triggerbits, std::string triggerName);

    // Method to dump the trigger information stored
    void dumpTriggerInfo();
        

    // dbg level
    bool m_dbg;

private :
    // trigger histo
    TH1F* m_trigHisto;
    std::map<std::string, int>  m_triggerMap;

};


#endif