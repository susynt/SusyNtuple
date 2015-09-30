#include "SusyNtuple/EventlistHandler.h"
#include "SusyNtuple/Susy2LepCutflow.h"
#include "SusyNtuple/ChainHelper.h"
#include "SusyNtuple/string_utils.h"

#include "TChain.h"

#include <iostream>
#include <string>

using namespace std;

namespace Susy
{
///  Example class showing how to use an EventlistHandler
/**
  davide.gerbaudo@gmail.com, April 2014
*/
class DileptonCutflowWithList : public Susy2LepCutflow
{
public:
    DileptonCutflowWithList() : m_useExistingList(false), m_nProcessedEntries(0) {}
    virtual void    Begin(TTree *tree)
        {        
            Susy2LepCutflow::Begin(tree);
            // check if the event list is there; if so, fetch it and loop only on those events
            m_useExistingList = m_eventList.cacheDoesExists();
            if(m_useExistingList) {
                tree->SetEventList(m_eventList.fetchEventList());
                cout<<"using existing event list from "<<m_eventList.cacheFilename()<<endl;
            }
        }
    virtual void    Terminate()
        {
            // nothing special to be done here; the EventlistHandler destructor will take care of saving the list
            if(!m_useExistingList)
                cout<<"event list for this run will be saved in "<<m_eventList.cacheFilename()<<endl;
            Susy2LepCutflow::Terminate();
        }
    virtual Bool_t  Process(Long64_t entry)
        {
            // here the only additional step is to call 'addEvent' for the entries you want to process
            bool isSelectedEvent = Susy2LepCutflow::Process(entry);
            if(!m_useExistingList && isSelectedEvent)
                m_eventList.addEvent(entry);
            m_nProcessedEntries++;
            return isSelectedEvent;
        }
public:
    bool m_useExistingList;        //!< to keep track of whether there is already an event list
    EventlistHandler m_eventList;  //!< the actual event list
    Long64_t m_nProcessedEntries;  //!< a counter to check that we are processing less events when using the list
};
}

//----------------------------------------------------------
//
// main executable, where we loop and use DileptonCutflowWithList
//
//----------------------------------------------------------
void printHelp(const char *exeName)
{
  cout<<"Usage :"<<endl
      <<exeName<<endl
      <<"\t -i inputFile.root"<<endl
      <<"\t -s samplename"<<endl
      <<endl;
}

int main(int argc, char **argv)
{

  string sampleName;
  string inputRootFname;
  bool verbose(false);

  int optind(1);
  while ((optind < argc)) {
    if(argv[optind][0]!='-'){optind++; continue;}
    string sw = argv[optind];
    if     (sw == "-h"){ printHelp(argv[0]); return 0; }
    else if(sw == "-i"){ optind++; inputRootFname = argv[optind]; }
    else if(sw == "-s"){ optind++; sampleName     = argv[optind]; }
    else if(sw == "-v"){ verbose = true; }
    else if(argv[optind][0]=='-') cout<<"Unknown switch "<<sw<<endl;
    optind++;
  } // end if(optind<argc)
  
  if(verbose)
      cout<<"Being called as: "<<Susy::utils::commandLineArguments(argc, argv)<<endl;
  cout<<"Using the following options:"<<endl
      <<"inputRootFname : "<<inputRootFname<<endl
      <<"sample         : "<<sampleName<<endl
      <<endl;

  DileptonCutflowWithList analysis;
  if(verbose) analysis.setDebug(1);
  analysis.setSampleName(ChainHelper::sampleName(inputRootFname, verbose));
  analysis.m_eventList.setListName(sampleName); // can be anything; used to save the TEventList in the file
  analysis.m_eventList.setCacheFilename("./cache/"+sampleName+"_list.root"); // can be any (root) path where you have r/w permission

  TChain chain("susyNt");
  ChainHelper::addFile(&chain, inputRootFname);
  Long64_t nEntries = chain.GetEntries();

  chain.Process(&analysis, sampleName.c_str()); 
  cout<<"Total entries:   "<<nEntries<<endl;
  cout<<"Number of processed entries "<<analysis.m_nProcessedEntries<<endl;

  return 0;
}
