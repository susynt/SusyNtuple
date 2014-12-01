#include "SusyNtuple/MCWeighter.h"
#include "SusyNtuple/string_utils.h"
#include "SusyNtuple/vec_utils.h"

#include "TSystem.h"
#include "TFile.h"
#include "TKey.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TChainElement.h"
#include "TH1F.h"

#include <iostream>
#include <cstdlib> // atoi
#include <iterator> // distance
#include <sstream> // std::ostringstream

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
MCWeighter::MCWeighter() :
        m_useProcSumw(true),
        m_sumwMethod(Sumw_MAP),
        m_xsecMethod(Xsec_ST),
        m_xsecDB(gSystem->ExpandPathName(MCWeighter::defaultXsecDir().c_str())),
        m_warningCounter(0),
        m_allowInvalid(false),
        m_verbose(false)
{

}

/*--------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------*/
MCWeighter::~MCWeighter()
{
    cout<<"ProcessValidator summary: "<<m_procidValidator.summary()<<endl;
}

/*--------------------------------------------------------------------------------*/
// Build a map of MCID -> sumw.
// This method will loop over the input files associated with the TChain. The MCID
// in the first entry of the tree will be used, so one CANNOT use this if multiple
// datasets are combined into one SusyNt tree file! The generator weighted cutflow
// histograms will then be used to calculate the total sumw for each MCID. Each
// dataset used here must be complete, they CANNOT be spread out across multiple jobs.
// However, one can have more than one (complete) dataset in the chain, which is why
// we use the map.
/*--------------------------------------------------------------------------------*/
void MCWeighter::buildSumwMap(TTree* tree)
{
  if(tree->InheritsFrom("TChain")){
    buildSumwMapFromChain(dynamic_cast<TChain*>(tree));
  }
  else buildSumwMapFromTree(tree);

  // Dump the map values
  cout << endl << "On-the-fly sumw computation:" << endl;
  dumpSumwMap();
}
/*--------------------------------------------------------------------------------*/
void MCWeighter::buildSumwMapFromTree(TTree* tree)
{
  TFile* f = tree->GetCurrentFile();

  // Setup branch for accessing the MCID in the tree
  Event* evt = 0;
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("mcChannel", 1);
  tree->SetBranchAddress("event", &evt);
  tree->GetEntry(0);
  // General key, default process number (0)
  unsigned int mcid = evt->mcChannel;
  SumwMapKey genKey(mcid, 0);
  if(!sumwmapHasKey(genKey)) m_sumwMap[genKey] = 0;

  // Get the generator weighted histogram
  TH1F* hGenCF = (TH1F*) f->Get("genCutFlow");

  string labelCounter = (m_labelBinCounter.size()>0 ?
                         m_labelBinCounter :
                         defaultLabelBinCounter(static_cast<unsigned int>(mcid), m_verbose));
  int sumwBin = hGenCF->GetXaxis()->FindBin(labelCounter.c_str());
  m_sumwMap[genKey] += hGenCF->GetBinContent(sumwBin);

  // Find the histograms per process
  TIter next(f->GetListOfKeys());
  while(TKey* tkey = (TKey*) next()){
    // Test to see if object is a cutflow histogram
    TObject* obj = tkey->ReadObj();
    if(obj->InheritsFrom("TH1")){
      string histoName = obj->GetName();

      // Histo is named procCutFlowXYZ where XYZ is the process number
      string prefix = "procCutFlow";
      if(histoName.find(prefix) != string::npos){
        TH1F* hProcCF = (TH1F*) obj;

        // Extract the process ID (XYZ) from the histo name (procCutFlowXYZ)
        string procString = histoName.substr(prefix.size(), string::npos);
        // Make sure the string is an int
        if(!susy::utils::isInt(procString)){
          cerr << "MCWeighter::buildSumwMap - ERROR - proc string from procCutFlow "
               << "histo is not an integer! Histo name: " << histoName
               << " proc string: " << procString << endl;
          abort();
        }
        stringstream stream;
        stream << procString;
        int proc;
        stream >> proc;
        // Skip the default with proc = -1 or 0
        if(proc != -1 && proc != 0){
          SumwMapKey procKey(mcid, proc);
          bool keyNotThereYet(!sumwmapHasKey(procKey));
          if(keyNotThereYet) m_sumwMap[procKey] = 0;
          m_sumwMap[procKey] += hProcCF->GetBinContent(sumwBin);
        }
      } // Is a proc cutflow
    } // Object is a histo
  } // Loop over TKeys in TFile
}
/*--------------------------------------------------------------------------------*/
void MCWeighter::buildSumwMapFromChain(TChain* chain)
{
  // Loop over files in the chain
  TObjArray* fileElements = chain->GetListOfFiles();
  TIter next(fileElements);
  TChainElement* chainElement = 0;
  while((chainElement = (TChainElement*)next())){
    TString fileTitle = chainElement->GetTitle();
    TFile* f = TFile::Open(fileTitle.Data());

    // Get the tree, for extracting mcid
    TTree* tree = (TTree*) f->Get("susyNt");

    buildSumwMapFromTree(tree);

    f->Close();
    delete f;
  } // Loop over TChain elements

}

/*--------------------------------------------------------------------------------*/
void MCWeighter::dumpSumwMap() const
{
  // Dump out the MCIDs and calculated sumw
  SumwMap::const_iterator sumwMapIter;
  cout.precision(8);
  for(sumwMapIter = m_sumwMap.begin(); sumwMapIter != m_sumwMap.end(); sumwMapIter++){
    cout << "mcid: " << sumwMapIter->first.first
         << " proc: " << sumwMapIter->first.second
         << " sumw: " << sumwMapIter->second
         << endl;
  }
  cout.precision(6);
}
/*--------------------------------------------------------------------------------*/
void MCWeighter::dumpXsecCache() const
{
    struct XSecEntry2str {
        string operator() (const XSecMap::const_iterator &entry) {
            std::ostringstream oss;
            oss<<" (first, second): "<<entry->first.first<<", "<<entry->first.second;
            return oss.str();
        }
    } entry2str;
    XSecMap::const_iterator it = m_xsecCache.begin();
    XSecMap::const_iterator end = m_xsecCache.end();
    cout<<"printing xsec cache ("<<std::distance(it, end)<<" lines)"<<endl;
    cout.precision(8);
    for( ; it!=end; ++it){
        cout<<entry2str(it);
    }
    cout.precision(6);
}
/*--------------------------------------------------------------------------------*/
void MCWeighter::dumpXsecDb() const
{
    struct Process2str { //should really be provided by CrossSectionDB...
        string header() const { return string("ID\tname\t(xsec*kfactor*efficiency)"); }
        string operator() (const SUSY::CrossSectionDB::Process &p) {
            std::ostringstream oss;
            oss<<" "<<p.ID()
               <<" "<<p.name()
               <<" "<<(p.xsect()*p.kfactor()*p.efficiency());
            return oss.str();
        }
    } process2str;
    SUSY::CrossSectionDB::iterator it = m_xsecDB.begin();
    SUSY::CrossSectionDB::iterator end = m_xsecDB.end();

    cout<<"printing xsec db ("<<std::distance(it, end)<<" lines)"<<endl;
    for(; it!=end; ++it)
        cout<<process2str(it->second)<<endl;
}
/*--------------------------------------------------------------------------------*/
float MCWeighter::getMCWeight(const Event* evt, float lumi, WeightSys sys)
{
    float weight = 1.0;
    size_t maxNwarnings=100;
    if(evt->isMC){
        float sumw = getSumw(evt);
        float xsec = getXsecTimesEff(evt, sys);
        float pupw = getPileupWeight(evt, sys);
        if(sumw) {
            weight = evt->w * pupw * xsec * lumi / sumw;
        } else {
            weight = 0.0;
            m_warningCounter++;
            if(m_warningCounter<maxNwarnings)
                cout<<"MCWeighter::getMCWeight: warning: trying to normalize an event with sumw=0"<<endl
                    <<"\tSomething must be wrong in your sumw map"
                    <<"\tPerhaps you need to call setLabelBinCounter with a non-default value"<<endl
                    <<"\tReturning a default weight of "<<weight<<" ("<<m_warningCounter<<"/"<<maxNwarnings<<")"
                    <<endl;
        }
    }
    return weight;
}
//------------------------------------------------
bool MCWeighter::sumwmapHasKey(SumwMapKey k)
{
    return(m_sumwMap.find(k) != m_sumwMap.end());
}
/*--------------------------------------------------------------------------------*/
// Get the sumw for this event
/*--------------------------------------------------------------------------------*/
float MCWeighter::getSumw(const Event* evt)
{
  float sumw = evt->sumw;
  if(m_sumwMethod==Sumw_MAP){
    // Map key is pair(mcid, proc)
    unsigned int mcid = evt->mcChannel;
    int procID = m_useProcSumw? evt->susyFinalState : 0;
    procID = ProcessValidator::convertDefaultSusyNt2DefaultSusyTools(procID);
    m_procidValidator.validate(procID);
    SumwMapKey key(mcid, procID);
    SumwMap::const_iterator sumwMapIter = m_sumwMap.find(key);
    if(sumwMapIter != m_sumwMap.end()) sumw = sumwMapIter->second;
    else{
      cerr << "MCWeighter::getEventWeight - ERROR - requesting to use sumw map but "
           << "mcid " << mcid << " proc " << procID << " not found!" << endl;
      cerr << "Here's what's currently in the map:" << endl;
      dumpSumwMap();
      abort();
    }
  }
  return sumw;
}

/*--------------------------------------------------------------------------------*/
// Get the SUSYTools cross section for this sample
/*--------------------------------------------------------------------------------*/
SUSY::CrossSectionDB::Process MCWeighter::getCrossSection(const Event* evt)
{
  using namespace SUSY;
  CrossSectionDB::Process process;
  if(evt->isMC){
    // SUSYTools expects 0 as default value, but we have existing tags with default of -1
      int proc = evt->susyFinalState > 0? evt->susyFinalState : 0;
      unsigned int mcid = evt->mcChannel;
      if(m_procidValidator.validate(proc).valid){
          const intpair k(mcid, proc);
          XSecMap::const_iterator iter = m_xsecCache.find(k);
          bool isAlreadyCached(iter != m_xsecCache.end());
          if(isAlreadyCached){    process = iter->second; }
          else { m_xsecCache[k] = process = m_xsecDB.process(mcid, proc); }
      } else {
          if(m_allowInvalid){
              float invalidXsec=0.0; // default xsec from SUSYTools is -1; use 0.0 instead (no bias)
              process = CrossSectionDB::Process(process.ID(), process.name(), invalidXsec,
                                                process.kfactor(), process.efficiency(),
                                                process.relunc(), process.sumweight(), process.stat());
              if(m_procidValidator.counts_invalid<m_procidValidator.max_warnings)
                  cerr<<"MCWeighter::getCrossSection - WARNING - xsec not found in SUSYTools."
                      <<"(mcid "<<mcid<<", proc "<<proc<<"), returning xsec "<<invalidXsec
                      <<endl;

          } else {
              cout<<"For mcid "<<mcid<<" and proc "<<proc
                  <<" you need to either provide a xsec file (see test_mcWeighter),"
                  <<" or call MCWeighter::setAllowInvalid(true)"
                  <<endl;
              abort();
          } // if(!m_allowInvalid)
      } // if(!valid)
  } // if(isMC)
  return process;
}

/*--------------------------------------------------------------------------------*/
float MCWeighter::getXsecTimesEff(const Event* evt, MCWeighter::WeightSys sys)
{
  float xsec = evt->xsec;
  if(m_xsecMethod==Xsec_ST){
    SUSY::CrossSectionDB::Process p = getCrossSection(evt);
    xsec = p.xsect() * p.kfactor() * p.efficiency();
    if(sys==MCWeighter::Sys_XSEC_UP)
      xsec *= (1. + p.relunc());
    else if(sys==MCWeighter::Sys_XSEC_DN)
      xsec *= (1. - p.relunc());
  }
  return xsec;
}

/*--------------------------------------------------------------------------------*/
// Get the pileup weight for this event
/*--------------------------------------------------------------------------------*/
float MCWeighter::getPileupWeight(const Event* evt, MCWeighter::WeightSys sys)
{
  if(sys==MCWeighter::Sys_PILEUP_UP) return evt->wPileup_up;
  else if(sys==MCWeighter::Sys_PILEUP_DN) return evt->wPileup_dn;
  else return evt->wPileup;
}
//----------------------------------------------------------
MCWeighter& MCWeighter::setLabelBinCounter(const std::string &v)
{
    m_labelBinCounter = v;
    return *this;
}
//----------------------------------------------------------
std::string MCWeighter::defaultLabelBinCounter(const unsigned int &dsid, bool verbose)
{
    string bin_label="Initial";
    if(MCWeighter::isSimplifiedModel(dsid, verbose))
        bin_label = "SusyProp Veto";
    return bin_label;
}
//----------------------------------------------------------
size_t MCWeighter::parseAdditionalXsecFile(const std::string &input_filename, bool verbose)
{
    string filename = gSystem->ExpandPathName(input_filename.c_str());
    size_t nInitialElements(std::distance(m_xsecDB.begin(), m_xsecDB.end()));
    bool inputFileIsValid(MCWeighter::isFormattedAsSusyCrossSection(filename, verbose));
    if(inputFileIsValid) {
        SUSY::CrossSectionDB tmpXsecDb;
        tmpXsecDb.loadFile(filename.c_str());
        for(SUSY::CrossSectionDB::iterator p=tmpXsecDb.begin(); p!=tmpXsecDb.end(); ++p) {
            int sample_id(p->second.ID());
            // this is an ugly conversion we inherit from SUSYCrossSection; drop when they provide Key::get_proc_id
            int proc_id(atoi(p->second.name().c_str()));
            bool alreadyThere(m_xsecDB.process(sample_id, proc_id).ID()!=-1);
            if(alreadyThere)
                cout<<"MCWeighter::parseAdditionalXsecFile:"
                    <<" warning: the entry for (dsid="<<p->second.ID()<<" proc="<<p->second.name()<<")"
                    <<" will be overwritten"<<endl;
        } // for(p)
        m_xsecDB.loadFile(gSystem->ExpandPathName(filename.c_str()));
    } else {
        cout<<"MCWeighter::parseAdditionalXsecFile: invalid input file '"<<filename<<"'"<<endl;
    }
    size_t nFinalElements(std::distance(m_xsecDB.begin(), m_xsecDB.end()));
    if(verbose)
        cout<<"MCWeighter::parseAdditionalXsecFile: parsed "<<(nFinalElements - nInitialElements)<<" values from "<<filename<<endl;
    return nFinalElements - nInitialElements;
}
//----------------------------------------------------------
size_t MCWeighter::parseAdditionalXsecDirectory(const std::string &dir, bool verbose)
{
    size_t nInitialElements(std::distance(m_xsecDB.begin(), m_xsecDB.end()));
    vector<string> filenames = susy::utils::filesFromDir(dir);
    for(vector<string>::const_iterator fname = filenames.begin(); fname!=filenames.end(); ++fname)
        if(susy::utils::contains(*fname, ".txt"))
            parseAdditionalXsecFile(*fname, verbose);
    size_t nFinalElements(std::distance(m_xsecDB.begin(), m_xsecDB.end()));
    return nFinalElements - nInitialElements;
}
//----------------------------------------------------------
bool MCWeighter::isFormattedAsSusyCrossSection(std::string filename, bool verbose)
{
    size_t nUsefulLines = readDsidsFromSusyCrossSectionFile(filename, verbose).size();
    return nUsefulLines>0;
}
//----------------------------------------------------------
bool isEmptyLine(const std::string &line)
{
    return susy::utils::rmLeadingTrailingWhitespaces(line).size()==0;
}
bool isCommentLine(const std::string &line)
{
    string strippedLine(susy::utils::rmLeadingTrailingWhitespaces(line));
    return strippedLine.size()>0 && strippedLine[0]=='#';
}
std::vector<int> MCWeighter::readDsidsFromSusyCrossSectionFile(std::string filename, bool verbose)
{
    std::vector<int> dsids;
    ifstream input;
    input.open(filename.c_str(), ifstream::in);
    bool fileIsOpen(input.is_open());
    if(!fileIsOpen) {
        cout<<"MCWeighter::readDsidsFromSusyCrossSectionFile: cannot open file "<<filename<<endl;
        return dsids;
    }
    size_t nEmptyOrCommentLines=0;
    size_t nValidLines=0;
    size_t nInvalidLines=0;
    std::string line;
    if(verbose)
        cout<<"readDsidsFromSusyCrossSectionFile: parsing '"<<filename<<"'"<<endl;
    while (std::getline(input, line)) {
        bool skipThisLine(isEmptyLine(line) || isCommentLine(line));
        if(skipThisLine) {
            nEmptyOrCommentLines++;
            continue;
        } else {
            int dsid;
            if(MCWeighter::readDsidsFromSusyCrossSectionLine(line, dsid, verbose)) {
                dsids.push_back(dsid);
                nValidLines++;
            } else {
                nInvalidLines++;
            }
        } // if(!skipThisLine)
    } // while(getline)
    if(verbose)
        cout<<"readDsidsFromSusyCrossSectionFile('"<<filename<<"') : "
            <<" "<<nValidLines<<" valid"
            <<", "<<nInvalidLines<<" invalid"
            <<", "<<nEmptyOrCommentLines<<" empty/comment"
            <<" lines"<<endl;
    return dsids;
}
//----------------------------------------------------------
bool MCWeighter::readDsidsFromSusyCrossSectionLine(const std::string &line, int &dsid, bool verbose)
{
    bool valid_parse = false;
    const size_t nExpectedTokens=6;
    vector<string> tokens(susy::utils::tokenizeString(line, ' '));
    bool hasExpectedTokens(tokens.size()==nExpectedTokens);
    bool firstTokenIsDsid(tokens.size()>0 && susy::utils::isInt(tokens[0]));
    bool isValidLine(hasExpectedTokens && firstTokenIsDsid);
    if(isValidLine) {
        dsid = atoi(tokens[0].c_str());
        valid_parse = true;
    } else {
        if(verbose)
            cout<<"invalid line"
                <<" ("<<tokens.size()<<" tokens, expected "<<nExpectedTokens<<","
                <<" firstTokenIsDsid "<<(firstTokenIsDsid?"true":"false")
                <<", "<<(tokens.size() ? tokens[0] : "")
                <<" ):"
                <<" '"<<line<<"'"<<endl;
    } // if(!isValidLine)
    return valid_parse;
}
//----------------------------------------------------------
bool MCWeighter::isSimplifiedModel(const unsigned int &dsid, bool verbose)
{
    bool is_known_dsid=false;
    // note: no need to propagate 'verbose' when parsing known files
    vector<int> know_dsids = MCWeighter::dsidsForKnownSimpliedModelSamples(false);
    is_known_dsid = susy::utils::contains<int>(know_dsids, dsid);
    if(verbose)
        cout<<"isSimplifiedModel('"<<dsid<<"'):"<<" is_known_dsid "<<(is_known_dsid ? "true":"false")<<endl;
    return is_known_dsid;
}
//----------------------------------------------------------
std::vector<std::string> MCWeighter::xsecFilesForSimplifiedModels()
{
    std::vector<std::string> filenames;
    string basedir = gSystem->ExpandPathName(MCWeighter::defaultXsecDir().c_str());
    filenames.push_back(basedir+"/"+"Herwigpp_UEEE3_CTEQ6L1_simplifiedModel_wA.txt");
    filenames.push_back(basedir+"/"+"Herwigpp_UEEE3_CTEQ6L1_simplifiedModel_wC.txt");
    return filenames;
}
//----------------------------------------------------------
std::vector<int> MCWeighter::dsidsForKnownSimpliedModelSamples(bool verbose)
{
    vector<int> know_dsids;
    vector<string> known_simplified_lists = MCWeighter::xsecFilesForSimplifiedModels();
    vector<string>::const_iterator fname = known_simplified_lists.begin();
    for(; fname!=known_simplified_lists.end(); ++fname){
        vector<int> dsids(MCWeighter::readDsidsFromSusyCrossSectionFile(*fname, verbose));
        know_dsids.insert(know_dsids.end(), dsids.begin(), dsids.end());
    }
    return know_dsids;
}
//----------------------------------------------------------
MCWeighter& MCWeighter::setAllowInvalid(bool v)
{
    m_allowInvalid = v;
    return *this;
}
//----------------------------------------------------------
MCWeighter& MCWeighter::setVerbose(bool v)
{
    m_verbose = v;
    return *this;
}
//----------------------------------------------------------
int MCWeighter::ProcessValidator::convertDefaultSusyNt2DefaultSusyTools(const int &v)
{
    return v==defaultSusyNt ? defaultSusyTools : v;
}
//----------------------------------------------------------
MCWeighter::ProcessValidator& MCWeighter::ProcessValidator::validate(int &value)
{
    bool isFirstEvent(counts_total==0);
    if(isFirstEvent){
        valid = true;
        last = value;
        value = convertDefaultSusyNt2DefaultSusyTools(value);
        counts_total++;
    } else {
        bool invalid = (value!=last && (value==defaultSusyNt || value==defaultSusyTools));
        valid = !invalid;
        last = value;
        value = convertDefaultSusyNt2DefaultSusyTools(value);
        counts_total++;
        if(invalid){
            counts_invalid++;
            if(counts_invalid<max_warnings)
                cout<<"ProcessValidator.validate("<<last<<") is invalid, converting to "<<value<<endl;
        }
    }
    counts_total++;
    return *this;
}
//----------------------------------------------------------
std::string MCWeighter::ProcessValidator::summary() const
{
    std::ostringstream oss;
    oss<<" ProcessValidator:"
       <<" processed "<<counts_total<<" entries,"
       <<" "<<counts_invalid<<" invalid ones";
    return oss.str();

}
//----------------------------------------------------------
