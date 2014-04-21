#include "SusyNtuple/EventlistHandler.h"

#include "TFile.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>


using Susy::EventlistHandler;

using std::cout;
using std::endl;
using std::string;

// A few util functions to check dirs and files
//----------------------------------------------------------
std::string basedir(const std::string &path)
{
  size_t found = path.find_last_of('/');
  return path.substr(0, found+1);
}
//----------------------------------------------------------
bool fileExists(const std::string &filename)
{
  std::ifstream file(filename.c_str());
  bool doesExists = file;
  file.close();
  return doesExists;
}
//----------------------------------------------------------
bool dirExists(const std::string &dirname)
{
  bool doesExist(false);
  if(dirname.length()<1) return doesExist;
  typedef struct stat Stat; Stat st;
  doesExist = (0==stat(dirname.c_str(), &st));
  bool isDir(S_ISDIR(st.st_mode));
  return doesExist && isDir;
}
//----------------------------------------------------------
// from : http://stackoverflow.com/questions/675039/how-can-i-create-directory-tree-in-c-linux
// return empty string if dir cannot be created; otherwise dir path
std::string mkdirIfNeeded(const std::string &dirname)
{
  std::string result;
  if(dirname.length()<1)      result = "";
  else if(dirExists(dirname)) result = dirname;
  else {
    int status(mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH));
    bool success(status==0);
    result = (success ? dirname : "");
  }
  return result;
}
//----------------------------------------------------------
EventlistHandler::EventlistHandler() :
    m_firstEventHasBeenProcessed(false),
    m_cacheFile(NULL),
    m_verbose(false)
{
    setDefaultValues();
}
//----------------------------------------------------------
EventlistHandler::~EventlistHandler()
{
    if(m_cacheFile) { closeOutputFile(); }
}
//----------------------------------------------------------
bool EventlistHandler::cacheDoesExists() const
{
    return fileExists(m_cacheFilename);
}
//----------------------------------------------------------
EventlistHandler& EventlistHandler::addEvent(Long64_t entry)
{
    if(!m_firstEventHasBeenProcessed){
        initOutputFile();
    }
    m_eventlist.Enter(entry);
    m_firstEventHasBeenProcessed = true;
    return *this;
}
//----------------------------------------------------------
EventlistHandler& EventlistHandler::setCacheFilename(const std::string value)
{
    string dir = basedir(value);
    if(!dirExists(dir)) {
        bool validDir(mkdirIfNeeded(dir).length()>0);
        if(!validDir) {
            cout<<"EventlistHandler::setCacheFilename: cannot set cache to '"<<value<<"', "
                " invalid directory '"<<dir<<"', using './'"<<endl;
            m_cacheFilename = "./"+value.substr(0, value.find_last_of('/'));
        }
    }
    if(m_verbose)
        cout<<"EventlistHandler: using cache file '"<<m_cacheFilename<<"'"<<endl;
    return *this;
}
//----------------------------------------------------------
TEventList* EventlistHandler::fetchEventList()
{
    m_cacheFile = TFile::Open(m_cacheFilename.c_str(), "read");
    if(m_cacheFile){
        TDirectory *pwd = gDirectory;
        m_cacheFile->cd();
        m_eventlist.Read(m_listName.c_str());
        m_cacheFile->Close();
        m_cacheFile->Delete();
        m_cacheFile = NULL;
        pwd->cd();
    } else {
        cout<<"EventlistHandler: cannot fetch event list '"<<m_listName<<"'"
            <<" from '"<<m_cacheFilename<<"' ("<<m_cacheFile<<")"
            <<endl;
    }
    return &m_eventlist;
}
//----------------------------------------------------------
bool EventlistHandler::initOutputFile()
{
    m_cacheFile = TFile::Open(m_cacheFilename.c_str(), "recreate");
    if(m_cacheFile){
        m_eventlist.SetDirectory(m_cacheFile);        
    } else {
        cout<<"EventlistHandler: cannot init output file '"<<m_cacheFilename<<"'"<<endl;
    }
    return m_cacheFile;
}
//----------------------------------------------------------
bool EventlistHandler::closeOutputFile()
{
    bool fileClosed = false;
    if(m_cacheFile){
        TDirectory *pwd = gDirectory;
        m_cacheFile->cd();
        m_eventlist.Write();
        m_cacheFile->Close();
        m_cacheFile->Delete();
        m_cacheFile = NULL;
        pwd->cd();
        fileClosed = true;
    }
    return fileClosed;
}
//----------------------------------------------------------
void EventlistHandler::setDefaultValues()
{
    setCacheFilename();
    setListName();
}
//----------------------------------------------------------
