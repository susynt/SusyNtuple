#include "SusyNtuple/SleptonXsecReader.h"

#include "TFile.h"
#include "TTree.h"

#include <algorithm>
#include <cassert>
#include <cstdlib> // atoi
#include <fstream>
#include <functional> // unary_function
#include <iostream>
#include <sstream>

using namespace std;


//--------------------------------------
SleptonPoint::SleptonPoint(int mSl, int m1, int finalState, bool verbose)
  : verbose_(verbose),
    mSl_(mSl),
    m1_(m1),
    finalState_(finalState)
{
  bool mslValid(mSlIsValid(mSl)), m1Valid(m1IsValid(m1)), fsValid(finalStateIsValid(finalState));
  if(verbose_) {
    if(!mslValid) cout<<"SleptonPoint: invalid msl "<<mSl<<endl;
    if(!m1Valid)  cout<<"SleptonPoint: invalid m1  "<<m1<<endl;
    if(!fsValid)  cout<<"SleptonPoint: invalid final state "<<finalState<<endl;
  } // end if(verbose)
  assert(mslValid && m1Valid && fsValid);
}
//--------------------------------------
bool SleptonPoint::mSlIsValid(int value)
{
  return (value >=0); // we might want to be more strict here
}
//--------------------------------------
bool SleptonPoint::m1IsValid(int value)
{
  return (value >=0); // we might want to be more strict here
}
//--------------------------------------
bool SleptonPoint::finalStateIsValid(int value)
{
  return ( (value >= SleptonPoint::kLeftHanded)
	   && (value <  SleptonPoint::kMaxFinalState) );
}
//--------------------------------------
// helper operators
bool operator== ( const SleptonPoint& a, const SleptonPoint& b )
{
  return ((a.mSl() == b.mSl())
	  && (a.m1() == b.m1())
	  && (a.finalState() == b.finalState()));
}
//--------------------------------------
string str ( const SleptonPoint& d )
{
  stringstream ss;
  ss << " mSl: " << d.mSl()
     << " m1: " << d.m1()
     << " finalState: "
     << (d.finalState()==SleptonPoint::kLeftHanded ? "kLeftHanded" 
	 : (d.finalState()==SleptonPoint::kRightHanded ? "kRightHanded"
	    : (d.finalState()==SleptonPoint::kLHRHCombined ? "kLHRHCombined"
	       : (d.finalState()==SleptonPoint::kUnknown ? "kUnknown"
	          : "???"))));
  return ss.str();
}
//--------------------------------------
ostream& operator<< ( ostream& m, const SleptonPoint& d )
{
  m << str(d);
  return m;
}
//--------------------------------------

//--------------------------------------
SleptonXsecReader::SleptonXsecReader(WorkingMode mode, bool verbose)
  : verbose_(verbose),
    mode_(mode)
{
  populateDsidList();
}
//--------------------------------------
SleptonXsecReader::SleptonXsecReader(const char* txtDsidFileName,
                                     const char* rootFileName, const char* treeName,
                                     WorkingMode mode, bool verbose)
  : verbose_(verbose),
    mode_(mode)
{
  populateDsidList(txtDsidFileName);
  readFile(rootFileName, treeName);
}
//--------------------------------------
bool isMultiple(const SleptonXsecReader::PointWithXsecCounts &p) { return p.counts_>1; }
//--------------------------------------
int SleptonXsecReader::readFile(const char* fileName, const char* treeName)
{
  TFile *inFile = TFile::Open(fileName);
  if(!inFile || !inFile->IsOpen()){
    if(verbose_) cout<<"SleptonXsecReader::readFile : cannot open '"<<fileName<<"'"<<endl;
    return 0;
  }
  TTree *tree = 0;
  inFile->GetObject(treeName, tree);
  if(!tree){
    if(verbose_) cout<<"SleptonXsecReader::readFile : cannot find '"<<treeName<<"'"
		     <<" in '"<<fileName<<"'"<<endl;
    return 0;
  }
  Float_t mSl=0, mN=0, xsec=0, xsecErr=0;
  Int_t finalState=0;
  tree->SetBranchAddress("mSL", &mSl);
  tree->SetBranchAddress("mN", &mN);
  tree->SetBranchAddress("crossSection", &xsec);
  tree->SetBranchAddress("Tot_error", &xsecErr);
  tree->SetBranchAddress("finalState", &finalState);
  int nEntries(tree->GetEntries());
  if(verbose_) cout<<"SleptonXsecReader::readFile: "
		   <<" starting with "<<points_.size()<<" points"
		   <<" and about to read "<<nEntries<<" entries"<<endl;
  for(int iEntry=0; iEntry<nEntries; iEntry++) {
    if(!tree->GetEntry(iEntry)) continue;
    else processEntry(mSl, mN, finalState, xsec, xsecErr);
  } // end for(iEntry)
  inFile->Close();
  inFile->Delete();
  if(mode_==kDropMultipleEntries)
    points_.erase(std::remove_if(points_.begin(), points_.end(), isMultiple), points_.end());
  if(verbose_) cout<<"SleptonXsecReader::readFile : now with "<<points_.size()<<" points"<<endl;
  return points_.size();
}
//--------------------------------------
// match criterion to be used with find_if
struct CriterionSamePoint:
  public std::unary_function<const SleptonXsecReader::PointWithXsecCounts&, bool> {
  CriterionSamePoint(const SleptonPoint &p): p_(p) { }
  bool operator()(const SleptonXsecReader::PointWithXsecCounts &p) { return p.point_ == p_; }
  const SleptonPoint p_;
};
//--------------------------------------
float SleptonXsecReader::getCrossSection(const SleptonPoint &point) const
{
  Vpxs::const_iterator match = std::find_if(points_.begin(), points_.end(),
					    CriterionSamePoint(point));
  if(match==points_.end()) {
    if(verbose_) cout<<"SleptonXsecReader::getCrossSection : no match for "<<point<<endl;
    return 0.;
  }
  else return match->xsec_;
}
//--------------------------------------
float SleptonXsecReader::getCrossSectionError(const SleptonPoint &point) const
{
  Vpxs::const_iterator match = std::find_if(points_.begin(), points_.end(),
					    CriterionSamePoint(point));
  if(match==points_.end()) {
    if(verbose_) cout<<"SleptonXsecReader::getCrossSectionError : no match for "<<point<<endl;
    return 0.;
  }
  else return match->xsecErr_;
}
//--------------------------------------
// match criterion to be used with find_if
struct CriterionSameId:
  public std::unary_function<const SleptonXsecReader::PointWithDsid&, bool> {
  CriterionSameId(int id): id_(id) { }
  bool operator()(const SleptonXsecReader::PointWithDsid &p) { return p.dsid_ == id_; }
  const int id_;
};
//--------------------------------------
float SleptonXsecReader::getCrossSection(const int &datasetId, const int &finalState) const
{
  Vpd::const_iterator ds = find_if(knownDsids_.begin(), knownDsids_.end(),
				   CriterionSameId(datasetId));
  if(ds==knownDsids_.end()) return 0.;
  else if(finalState==SleptonPoint::kLeftHanded || finalState==SleptonPoint::kRightHanded) 
    return getCrossSection(SleptonPoint(ds->point_.mSl(), ds->point_.m1(), finalState));
  else 
    return ( 
             getCrossSection(SleptonPoint(ds->point_.mSl(), ds->point_.m1(), SleptonPoint::kLeftHanded )) +
             getCrossSection(SleptonPoint(ds->point_.mSl(), ds->point_.m1(), SleptonPoint::kRightHanded)) 
           );
}
//--------------------------------------
float SleptonXsecReader::getCrossSectionError(const int &datasetId, const int &finalState) const
{
  // Error for kLHRHCombined assumes full correlation between SleptonPoint::kLeftHanded && SleptonPoint::kRightHanded
  Vpd::const_iterator ds = find_if(knownDsids_.begin(), knownDsids_.end(),
				   CriterionSameId(datasetId));
  if(ds==knownDsids_.end()) return 0.;
  else if(finalState==SleptonPoint::kLeftHanded || finalState==SleptonPoint::kRightHanded) 
    return getCrossSectionError(SleptonPoint(ds->point_.mSl(), ds->point_.m1(), finalState));
  else 
    return ( 
             getCrossSectionError(SleptonPoint(ds->point_.mSl(), ds->point_.m1(), SleptonPoint::kLeftHanded )) +
             getCrossSectionError(SleptonPoint(ds->point_.mSl(), ds->point_.m1(), SleptonPoint::kRightHanded)) 
           );
}
//--------------------------------------
void SleptonXsecReader::printKnownPoints() const
{
  cout<<"SleptonXsecReader::printKnownPoints"<<endl;
  for(Vpxs::const_iterator it=points_.begin(); it!=points_.end(); ++it)
    cout<<it->point_<<" : "<<it->xsec_<<endl;
  cout<<endl;
}
//--------------------------------------
bool SleptonXsecReader::extractDsetidMslM1FromString(const char* dsetName, int &dsid, int &msl, int &m1, bool verbose){
  // when boost can be used easily within rootcore, switch to boost/regex
  // (already implemented in this file@315918)
  // For now parse manually something like .*TeV\.<dsid>_MSL_<msl>_M1_<m1>\.
  string name(dsetName);
  string sTev("TeV."), sMsl("_MSL_"), sM1("_M1_");
  size_t posTev = name.find(sTev);
  if(posTev==string::npos){
    if(verbose) cout<<"cannot parse '"<<sTev<<"' in "<<name<<endl;
    return false;
  }
  size_t posDsidEnd = name.find(".", posTev+sTev.length());
  size_t posMsl = name.find(sMsl);
  if(posMsl==string::npos) {
    if(verbose) cout<<"cannot parse '"<<sMsl<<"' in "<<name<<endl;
    return false;
  }
  size_t posM1 = name.find(sM1, posMsl);
  if(posM1==string::npos) {
    if(verbose) cout<<"cannot parse '"<<sM1<<"' in "<<name<<endl;
    return false;
  }
  size_t posM1end = name.find(".", posM1);

  size_t dsidB(posTev + sTev.length()), dsidE(posDsidEnd);
  size_t mslB (posMsl + sMsl.length()),  mslE(posM1);
  size_t m1B  (posM1 +sM1.length()),      m1E(posM1end);
  if(false)//verbose)
    cout<<" dsid ["<<dsidB<<", "<<dsidE<<"]: '"<<name.substr(dsidB, dsidE-dsidB)<<"'"
        <<" msl  ["<<mslB <<", "<<mslE <<"]: '"<<name.substr(mslB, mslE-mslB)   <<"'"
        <<" m1   ["<<m1B  <<", "<<m1E  <<"]: '"<<name.substr(m1B, m1E-m1B)      <<"'"
        <<endl;
  dsid = atoi(name.substr(dsidB, dsidE-dsidB).c_str());
  msl  = atoi(name.substr(mslB, mslE-mslB).c_str());
  m1   = atoi(name.substr(m1B, m1E-m1B).c_str());
  return true;
}
//--------------------------------------
std::string SleptonXsecReader::rmLeadingTrailingWhitespaces(const std::string &str)
{
  size_t startpos = str.find_first_not_of(" \t");
  size_t endpos = str.find_last_not_of(" \t");
  if(( string::npos == startpos ) || ( string::npos == endpos)) return string("");
  else return str.substr(startpos, endpos-startpos+1); 
}
//--------------------------------------
struct CriterionSamePointWithXsecCounts:
  public std::unary_function<const SleptonXsecReader::PointWithXsecCounts&, bool> {
  CriterionSamePointWithXsecCounts(const SleptonXsecReader::PointWithXsecCounts &p): p_(p) { }
  bool operator()(const SleptonXsecReader::PointWithXsecCounts &p) { return p.point_ == p_.point_; }
  const SleptonXsecReader::PointWithXsecCounts p_;
};
//--------------------------------------
bool SleptonXsecReader::processEntry(int mSl, int mN, int finalState, float xs, float xsErr)
{
  PointWithXsecCounts newPoint(SleptonPoint(mSl, mN, finalState), xs, xsErr);
  Vpxs::iterator existingPoint = std::find_if(points_.begin(), points_.end(),
					      CriterionSamePointWithXsecCounts(newPoint));
  if(existingPoint == points_.end()) {
    points_.push_back(newPoint);
    return true;
  } else {
    PointWithXsecCounts &p = *existingPoint;
    p.counts_++;
    if(mode_==kAverageMultipleEntries) {
      float denom(1.0/p.counts_);
      p.xsec_ = (p.counts_-1.0)*denom*p.xsec_ + 1.0*denom*newPoint.xsec_;
      p.xsecErr_ = (p.counts_-1.0)*denom*p.xsecErr_ + 1.0*denom*newPoint.xsecErr_;
    } else if(mode_==kLastOfMultipleEntries) {
      p.xsec_ = newPoint.xsec_;
      p.xsecErr_ = newPoint.xsecErr_;
    } else if(mode_==kDropMultipleEntries) {
      // nothing to do, will remove elements after all entries have been processed
    }
    return false;
  } // end if(existingPoint)
}
//--------------------------------------
int SleptonXsecReader::populateDsidList(const char* filename)
{
  string line, fileToParse(filename);
  if(!fileToParse.length()) fileToParse = SleptonXsecReader::getDefaultDsidFilename();
  ifstream inputFile(fileToParse.c_str());
  if(!inputFile){
    if(verbose_) 
      cout<<"SleptonXsecReader::populateDsidList(): inputFile not open,"
	  <<" cannot parse "<<fileToParse<<endl;
    return 0;
  }
  int initialKnownPoints(knownDsids_.size());
  while(getline(inputFile,line)){
    line = SleptonXsecReader::rmLeadingTrailingWhitespaces(line);
    bool boringLine(line.size()<2 || (line.size()>0 && line[0] == '#'));
    if(boringLine) continue;
    int dsid(0), msl(0), m1(0);
    if(extractDsetidMslM1FromString(line.c_str(), dsid, msl, m1, verbose_))
      knownDsids_.push_back( PointWithDsid( SleptonPoint( msl, m1, SleptonPoint::kUnknown ),
					    dsid ) );
  } // end while(getline)
  inputFile.close();
  return knownDsids_.size() - initialKnownPoints;
}
//--------------------------------------
bool SleptonXsecReader::getRootcoreDir(std::string &dir)
{
  char* rootcoredir = getenv("ROOTCOREDIR");
  bool envvarDefined(rootcoredir!=0);
  if (!envvarDefined) {
    dir = "";
    cout<<"SleptonXsecReader::getRootcoreDir: ROOTCOREDIR not defined"<<endl;
  } else {
    dir = rootcoredir;
  }
  return envvarDefined;
}
//--------------------------------------
std::string SleptonXsecReader::getDefaultDsidFilename()
{
  string rootcoredir, filename;
  bool rcDirFound(SleptonXsecReader::getRootcoreDir(rootcoredir));
  if(rcDirFound) filename = rootcoredir + "/data/SusyNtuple/samplesList_pMSSM_DLiSlep.txt";
  else           cout<<"invalid ROOTCOREDIR, cannot determine the default dsid file"<<endl;
  return filename;
}
//--------------------------------------
std::string SleptonXsecReader::getDefaultRootFilename()
{
  string rootcoredir, filename;
  bool rcDirFound(SleptonXsecReader::getRootcoreDir(rootcoredir));
  if(rcDirFound) filename = rootcoredir + "/data/SusyNtuple/DLiSlep_SignalUncertainties_All.root";
  else           cout<<"invalid ROOTCOREDIR, cannot determine the default root file"<<endl;
  return filename;
}
//--------------------------------------
std::string SleptonXsecReader::getDefaultTreeName()
{
  return string("SignalUncertainties");
}
//--------------------------------------
// helper operators
std::string str( const SleptonXsecReader& d )
{
  stringstream ss;
  ss<<"Not implemented yet";
  return ss.str();
}
//--------------------------------------
std::ostream& operator<< ( std::ostream& m, const SleptonXsecReader& d )
{
  m << str(d);
  return m;
}
//--------------------------------------

