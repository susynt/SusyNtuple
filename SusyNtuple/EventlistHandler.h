//  -*- c++ -*-
#ifndef SUSY_EVENTLISTHANDLER_H
#define SUSY_EVENTLISTHANDLER_H

#include "TEventList.h"

#include <string>

class TFile;

namespace Susy {
/*
  A class to cache a TEventList for SusyNt

  Usage: set the cache filename. On the first run, the TEventList is
  created and filled. Then just check cacheDoesExists(), and call
  TChain::SetEventList(EventlistHandler::fecthEventList())

  Caveat:
  - if you are using a TChain, the trees must be added in the same
    order from one run to the next. Use a filelist to ensure this

  See test_EventlistHandler.cxx for an example.

  davide.gerbaudo@gmail.com, April 2014
 */
class EventlistHandler {

public:
    EventlistHandler();
    ~EventlistHandler();
    bool cacheDoesExists() const;
    EventlistHandler& setCacheFilename(const std::string value="./cache/Sample_eventList.root");
    EventlistHandler& setListName(const std::string value="EventList") { m_listName = value; return *this; }
    EventlistHandler& addEvent(Long64_t entry);
    EventlistHandler& setVerbose(bool value=true) { m_verbose = value; return *this; }
    TEventList* eventList() { return &m_eventlist; }
    TEventList* fetchEventList();
    std::string cacheFilename() const { return m_cacheFilename; }
    
private: // rule of three
//    EventlistHandler& (const EventlistHandler&);
//    EventlistHandler& operator=(const EventlistHandler&);
private:
    bool initOutputFile();
    bool outputFileExists();
    bool closeOutputFile();
    void setDefaultValues();
private:
    std::string m_cacheFilename;
    std::string m_listName;
    bool m_firstEventHasBeenProcessed;
    TEventList m_eventlist;
    TFile *m_cacheFile;
    bool m_verbose;
};
} // Susy

#endif
