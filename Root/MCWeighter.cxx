#include "SusyNtuple/MCWeighter.h"

#include "SusyNtuple/Event.h"
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
#include <string> // string, stoi

using namespace std;
using namespace Susy;

/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
MCWeighter::MCWeighter() :
total_sumw(0),
m_useProcSumw(true),
m_sumwMethod(Sumw_MAP),
m_xsecMethod(Xsec_ST),
m_xsecDB(gSystem->ExpandPathName(MCWeighter::defaultXsecDir().c_str())),
m_warningCounter(0),
m_allowInvalid(false),
m_verbose(false)
{

}


MCWeighter::MCWeighter(TTree* tree, string xsecDir) :
total_sumw(0),
m_useProcSumw(true),
m_sumwMethod(Sumw_MAP),
m_xsecMethod(Xsec_ST),
m_xsecDB(gSystem->ExpandPathName(xsecDir.c_str())),
// m_labelBinCounter(MCWeighter::defaultLabelBinCounter()),
m_warningCounter(0),
m_allowInvalid(false),
m_verbose(false)
{
    if (tree) buildSumwMap(tree);
}


/*--------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------*/
MCWeighter::~MCWeighter()
{
    if(m_verbose)
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
    if (tree->InheritsFrom("TChain")) {
        buildSumwMapFromChain(dynamic_cast<TChain*>(tree));
    }
    else buildSumwMapFromTree(tree);

    if(m_verbose){
        cout<<endl<<"On-the-fly sumw computation:"<<endl;
        dumpSumwMap();
    }
}
/*--------------------------------------------------------------------------------*/
void MCWeighter::buildSumwMapFromTree(TTree* tree)
{
    const Event &evt = MCWeighter::readFirstEvent(tree);
    unsigned int mcid = evt.mcChannel;
    total_sumw += evt.sumOfEventWeights;
    if(m_verbose)
        cout<<"MCWeighter::buildSumwMapFromTree: mcid: "<<mcid<<"  partial sumw: "<<total_sumw<<endl;
}
/*--------------------------------------------------------------------------------*/
void MCWeighter::buildSumwMapFromChain(TChain* chain)
{

    // Loop over files in the chain
    TObjArray* fileElements = chain->GetListOfFiles();
    TIter next(fileElements);
    TChainElement* chainElement = 0;
    while ((chainElement = (TChainElement*)next())) {
        TString fileTitle = chainElement->GetTitle();
        TFile* f = TFile::Open(fileTitle.Data());
        // Get the tree
        TTree* tree = (TTree*)f->Get("susyNt");
        buildSumwMapFromTree(tree);

        f->Close();
        delete f;
    }
}
/*--------------------------------------------------------------------------------*/
void MCWeighter::dumpSumwMap() const
{
    // Dump out the MCIDs and calculated sumw
    SumwMap::const_iterator sumwMapIter;
    cout.precision(8);
    for (sumwMapIter = m_sumwMap.begin(); sumwMapIter != m_sumwMap.end(); sumwMapIter++) {
        cout<<"mcid: "<<sumwMapIter->first.dsid
            <<" proc: "<<sumwMapIter->first.proc
            <<" sumw: "<<sumwMapIter->second
            <<endl;
    }
    cout.precision(6);
}
/*--------------------------------------------------------------------------------*/
void MCWeighter::dumpXsecCache() const
{
    struct XSecEntry2str {
        string operator() (const XSecMap::const_iterator &entry)
        {
            std::ostringstream oss;
            oss<<" (first, second): "<<entry->first.first<<", "<<entry->first.second;
            return oss.str();
        }
    } entry2str;
    XSecMap::const_iterator it = m_xsecCache.begin();
    XSecMap::const_iterator end = m_xsecCache.end();
    cout<<"printing xsec cache ("<<std::distance(it, end)<<" lines)"<<endl;
    cout.precision(8);
    for (; it != end; ++it) {
        cout<<entry2str(it);
    }
    cout.precision(6);
}
/*--------------------------------------------------------------------------------*/
void MCWeighter::dumpXsecDb() const
{
    struct Process2str { //should really be provided by CrossSectionDB...
        string header() const { return string("ID\tname\t(xsec*kfactor*efficiency)"); }
        string operator() (const SUSY::CrossSectionDB::Process &p)
        {
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
    for (; it != end; ++it)
        cout<<process2str(it->second)<<endl;
}
/*--------------------------------------------------------------------------------*/
float MCWeighter::getMCWeight(const Event* evt, float lumi, WeightSys sys)
{
    float weight = 1.0;
    size_t maxNwarnings = 100;
    if (evt->isMC) {
        float sumw = total_sumw;
        //float sumw = getSumw(evt);
        //double sumw = evt->sumOfEventWeights; // now stored using CutBookKeeper
        float xsec = getXsecTimesEff(evt, sys);
        float pupw = getPileupWeight(evt, sys);
        if (sumw) {
            weight = evt->w * pupw * xsec * lumi / sumw;
        }
        else {
            weight = 0.0;
            m_warningCounter++;
            if (m_warningCounter < maxNwarnings)
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
    float sumw = total_sumw;
    return sumw;
}

/*--------------------------------------------------------------------------------*/
// Get the SUSYTools cross section for this sample
/*--------------------------------------------------------------------------------*/
SUSY::CrossSectionDB::Process MCWeighter::getCrossSection(const Event* evt)
{
    using namespace SUSY;
    CrossSectionDB::Process process;
    if (evt->isMC) {
        // SUSYTools expects 0 as default value, but we have existing tags with default of -1
        int proc = evt->susyFinalState > 0 ? evt->susyFinalState : 0;
        unsigned int mcid = evt->mcChannel;
        if (m_procidValidator.zero_hack(proc).valid) {
            // if(m_procidValidator.validate(proc).valid){
            const intpair k(mcid, proc);
            XSecMap::const_iterator iter = m_xsecCache.find(k);
            bool isAlreadyCached(iter != m_xsecCache.end());
            if (isAlreadyCached) { process = iter->second; }
            else { m_xsecCache[k] = process = m_xsecDB.process(mcid, proc); }
        }
        else {
            if (m_allowInvalid) {
                float invalidXsec = 0.0; // default xsec from SUSYTools is -1; use 0.0 instead (no bias)
                process = CrossSectionDB::Process(process.ID(), process.name(), invalidXsec,
                                                  process.kfactor(), process.efficiency(),
                                                  process.relunc(), process.sumweight(), process.stat());
                if (m_procidValidator.counts_invalid < m_procidValidator.max_warnings)
                    cerr<<"MCWeighter::getCrossSection - WARNING - xsec not found in SUSYTools."
                   <<"(mcid "<<mcid<<", proc "<<proc<<"), returning xsec "<<invalidXsec
                   <<endl;

            }
            else {
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
    if (m_xsecMethod == Xsec_ST) {
        SUSY::CrossSectionDB::Process p = getCrossSection(evt);
        xsec = p.xsect() * p.kfactor() * p.efficiency();
        if (sys == MCWeighter::Sys_XSEC_UP)
            xsec *= (1. + p.relunc());
        else if (sys == MCWeighter::Sys_XSEC_DN)
            xsec *= (1. - p.relunc());
    }
    return xsec;
}

/*--------------------------------------------------------------------------------*/
// Get the pileup weight for this event
/*--------------------------------------------------------------------------------*/
float MCWeighter::getPileupWeight(const Event* evt, MCWeighter::WeightSys sys)
{
    if (sys == MCWeighter::Sys_PILEUP_UP) return evt->wPileup_up;
    else if (sys == MCWeighter::Sys_PILEUP_DN) return evt->wPileup_dn;
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
    string bin_label = "Initial";
    if (MCWeighter::isSimplifiedModel(dsid, verbose))
        bin_label = "SusyProp Veto";
    return bin_label;
}
//----------------------------------------------------------
void MCWeighter::checkHistoHasBin(const TH1F &histo, const std::string &binLabel)
{
    int bin = histo.GetXaxis()->FindFixBin(binLabel.c_str());
    bool invalid_bin_label = bin==-1;
    if(invalid_bin_label){
        cerr<<"MCWeighter: cannot find bin '"<<binLabel<<"' from histo '"<<histo.GetName()<<"'"<<endl
            <<"Possible bins:"<<endl;
        for(int iBin=1; iBin<histo.GetNbinsX()+1; ++iBin)
            cerr<<"'"<<histo.GetXaxis()->GetBinLabel(iBin)<<"'"<<endl;
    }
}
//----------------------------------------------------------
size_t MCWeighter::parseAdditionalXsecFile(const std::string &input_filename, bool verbose)
{
    string filename = gSystem->ExpandPathName(input_filename.c_str());
    size_t nInitialElements(std::distance(m_xsecDB.begin(), m_xsecDB.end()));
    bool inputFileIsValid(MCWeighter::isFormattedAsSusyCrossSection(filename, verbose));
    if (inputFileIsValid) {
        SUSY::CrossSectionDB tmpXsecDb;
        tmpXsecDb.loadFile(filename.c_str());
        for (SUSY::CrossSectionDB::iterator p = tmpXsecDb.begin(); p != tmpXsecDb.end(); ++p) {
            int sample_id(p->second.ID());
            // this is an ugly conversion we inherit from SUSYCrossSection; drop when they provide Key::get_proc_id
            int proc_id(atoi(p->second.name().c_str()));
            bool alreadyThere(m_xsecDB.process(sample_id, proc_id).ID() != -1);
            if (alreadyThere)
                cout<<"MCWeighter::parseAdditionalXsecFile:"
                    <<" warning: the entry for (dsid="<<p->second.ID()<<" proc="<<p->second.name()<<")"
                    <<" will be overwritten"<<endl;
        } // for(p)
        m_xsecDB.loadFile(gSystem->ExpandPathName(filename.c_str()));
    }
    else {
        cout<<"MCWeighter::parseAdditionalXsecFile: invalid input file '"<<filename<<"'"<<endl;
    }
    size_t nFinalElements(std::distance(m_xsecDB.begin(), m_xsecDB.end()));
    if(verbose)
        cout<<"MCWeighter::parseAdditionalXsecFile:"
            <<" parsed "<<(nFinalElements - nInitialElements)<<" values"
            <<" from "<<filename
            <<endl;
    return nFinalElements - nInitialElements;
}
//----------------------------------------------------------
size_t MCWeighter::parseAdditionalXsecDirectory(const std::string &dir, bool verbose)
{
    size_t nInitialElements(std::distance(m_xsecDB.begin(), m_xsecDB.end()));
    vector<string> filenames = Susy::utils::filesFromDir(dir);
    for (vector<string>::const_iterator fname = filenames.begin(); fname != filenames.end(); ++fname)
        if (Susy::utils::contains(*fname, ".txt"))
            parseAdditionalXsecFile(*fname, verbose);
    size_t nFinalElements(std::distance(m_xsecDB.begin(), m_xsecDB.end()));
    return nFinalElements - nInitialElements;
}
//----------------------------------------------------------
bool MCWeighter::isFormattedAsSusyCrossSection(std::string filename, bool verbose)
{
    size_t nUsefulLines = readDsidsFromSusyCrossSectionFile(filename, verbose).size();
    return nUsefulLines > 0;
}
//----------------------------------------------------------
bool isEmptyLine(const std::string &line)
{
    return Susy::utils::rmLeadingTrailingWhitespaces(line).size() == 0;
}
bool isCommentLine(const std::string &line)
{
    string strippedLine(Susy::utils::rmLeadingTrailingWhitespaces(line));
    return strippedLine.size() > 0 && strippedLine[0] == '#';
}
std::vector<int> MCWeighter::readDsidsFromSusyCrossSectionFile(std::string filename, bool verbose)
{
    std::vector<int> dsids;
    ifstream input;
    input.open(filename.c_str(), ifstream::in);
    bool fileIsOpen(input.is_open());
    if (!fileIsOpen) {
        cerr<<"MCWeighter::readDsidsFromSusyCrossSectionFile: cannot open file "<<filename<<endl;
        return dsids;
    }
    size_t nEmptyOrCommentLines = 0;
    size_t nValidLines = 0;
    size_t nInvalidLines = 0;
    std::string line;
    if (verbose)
        cout<<"readDsidsFromSusyCrossSectionFile: parsing '"<<filename<<"'"<<endl;
    while (std::getline(input, line)) {
        bool skipThisLine(isEmptyLine(line) || isCommentLine(line));
        if (skipThisLine) {
            nEmptyOrCommentLines++;
            continue;
        }
        else {
            int dsid;
            if (MCWeighter::readDsidsFromSusyCrossSectionLine(line, dsid, verbose)) {
                dsids.push_back(dsid);
                nValidLines++;
            }
            else {
                nInvalidLines++;
            }
        } // if(!skipThisLine)
    } // while(getline)
    if (verbose)
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
    const size_t nExpectedTokens = 6;
    vector<string> tokens(Susy::utils::tokenizeString(line, ' '));
    bool hasExpectedTokens(tokens.size() == nExpectedTokens);
    bool firstTokenIsDsid(tokens.size() > 0 && Susy::utils::isInt(tokens[0]));
    bool isValidLine(hasExpectedTokens && firstTokenIsDsid);
    if (isValidLine) {
        dsid = atoi(tokens[0].c_str());
        valid_parse = true;
    }
    else {
        if (verbose)
            cout<<"invalid line"
                <<" ("<<tokens.size()<<" tokens, expected "<<nExpectedTokens<<","
                <<" firstTokenIsDsid "<<(firstTokenIsDsid ? "true" : "false")
                <<", "<<(tokens.size() ? tokens[0] : "")
                <<" ):"
                <<" '"<<line<<"'"<<endl;
    } // if(!isValidLine)
    return valid_parse;
}
//----------------------------------------------------------
bool MCWeighter::isSimplifiedModel(const unsigned int &dsid, bool verbose)
{
    bool is_known_dsid = false;
    // note: no need to propagate 'verbose' when parsing known files
    vector<int> know_dsids = MCWeighter::dsidsForKnownSimpliedModelSamples(false);
    is_known_dsid = Susy::utils::contains<int>(know_dsids, dsid);
    if (verbose)
        cout<<"isSimplifiedModel('"<<dsid<<"'):"<<" is_known_dsid "<<(is_known_dsid ? "true" : "false")<<endl;
    return is_known_dsid;
}
//----------------------------------------------------------
const Susy::Event& MCWeighter::readFirstEvent(TTree* tree)
{
    Susy::Event* evt = 0;
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus("event", 1);
    tree->SetBranchAddress("event", &evt);
    tree->GetEntry(0);
    return *evt;
}
//----------------------------------------------------------
int MCWeighter::extractProcessFromCutflowHistoname(const std::string &histoName, const std::string &prefix)
{
    cout<<"histoName '"<<histoName<<"', prefix '"<<prefix<<"'"<<endl;
    string procString = histoName.substr(prefix.size(), string::npos);
    cout<<"procString "<<procString<<endl;
    if (!Susy::utils::isInt(procString)) {
        cerr<<"MCWeighter::extractProcessFromCutflowHistoname - ERROR"
             <<" cannot extract integer process from '"<<histoName<<"' using prefix '"<<prefix<<"'"
             <<endl;
    }
    return std::stoi(procString);
}
//----------------------------------------------------------
std::vector<std::string> MCWeighter::xsecFilesForSimplifiedModels()
{
    std::vector<std::string> filenames;
    string basedir = gSystem->ExpandPathName(MCWeighter::defaultXsecDir().c_str());
    filenames.push_back(basedir + "/" + "Herwigpp_UEEE3_CTEQ6L1_simplifiedModel_wA.txt");
    filenames.push_back(basedir + "/" + "Herwigpp_UEEE3_CTEQ6L1_simplifiedModel_wC.txt");
    return filenames;
}
//----------------------------------------------------------
std::vector<int> MCWeighter::dsidsForKnownSimpliedModelSamples(bool verbose)
{
    vector<int> know_dsids;
    vector<string> known_simplified_lists = MCWeighter::xsecFilesForSimplifiedModels();
    vector<string>::const_iterator fname = known_simplified_lists.begin();
    for (; fname != known_simplified_lists.end(); ++fname) {
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
    return v == defaultSusyNt ? defaultSusyTools : v;
}
//----------------------------------------------------------
MCWeighter::ProcessValidator& MCWeighter::ProcessValidator::validate(int &value)
{
    bool isFirstEvent(counts_total == 0);
    if (isFirstEvent) {
        valid = true;
        last = value;
        value = convertDefaultSusyNt2DefaultSusyTools(value);
        counts_total++;
    }
    else {
        bool invalid = (value != last && (value == defaultSusyNt || value == defaultSusyTools));
        valid = !invalid;
        last = value;
        value = convertDefaultSusyNt2DefaultSusyTools(value);
        counts_total++;
        if (invalid) {
            counts_invalid++;
            if (counts_invalid < max_warnings)
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
