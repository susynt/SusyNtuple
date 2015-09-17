//  -*- c++ -*-
#ifndef SusyCommon_TriggerTools_h
#define SusyCommon_TriggerTools_h

#include <iostream>
#include <vector>
#include <map>
#include "TBits.h"

class TH1;

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
    TriggerTools(bool dbg=false);
    virtual ~TriggerTools()
    {
    };
    /// initialize by reading the trigger histogram from file
    bool init(const std::string &filename);
    // Method to build the trigger map given the SusyNt object
    //void buildTriggerMap(TChain* susyNt, bool dbg);
    void buildTriggerMap(const TH1* trigHisto);

    // Method to test whether a given trigger is passed
  //  bool passTrigger(std::map<std::string, int> trigMap, std::string triggerName);
    bool passTrigger(const TBits& triggerbits, const std::string &triggerName) const;

    // Method to dump the trigger information stored
    void dumpTriggerInfo() const;

    /* /// name of the first file in the  */
    /* static std::string firstFilename(const TChain* chain); */

    // dbg level
    bool m_dbg;

private :
    // trigger histo
    std::map<std::string, int>  m_triggerMap;

};


#endif
