#ifndef SLEPTONXSECREADER_H
#define SLEPTONXSECREADER_H
#include <vector>
#include <string>
#include <ostream>

//! A slepton point in the model parameter space.
/*!
  A slepton point is defined in the model parameter space by tree values:
  (1) the slepton mass
  (2) the m1 mass
  (3) the final state (left-handed or right-handed)

  See this wiki for more information
  https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSignalGridDirectSlepton

  davide.gerbaudo@gmail.com
  Feb 2013
 */

class SleptonPoint {
 public:
  enum FinalStates {  //!< see https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSignalUncertainties#Details_on_the_content_of_the_Si
    kLeftHanded = 201,  //!< left-handed selectron pair
    kRightHanded = 202, //!< right-handed selectron pair
    kLHRHCombined,
    kUnknown,
    kMaxFinalState,
  };
 public:
  static bool mSlIsValid(int value);
  static bool m1IsValid(int value);
  static bool finalStateIsValid(int value);
 public:
  SleptonPoint():verbose_(true),mSl_(0),m1_(0),finalState_(0) {};  //!<  avoid grauitous default c'tor, see item 4 "More Effective C++", S. Meyer
  SleptonPoint(int mSl, int m1, int finalState, bool verbose=false);
  //SleptonPoint( const SleptonPoint& rhs );
  int mSl()        const { return mSl_; };
  int m1()         const { return m1_;  };
  int finalState() const { return finalState_; };
 public:
  bool verbose_;   //!< toggle on/off messages
 private:
  int mSl_;        //!< mass slepton
  int m1_;		   //!< mass lsp (?)
  int finalState_; //!< final state (left-handed or right-handed)
}; // end SleptonPoint

bool operator== ( const SleptonPoint& a, const SleptonPoint& b );
inline bool operator!= ( const SleptonPoint& a, const SleptonPoint& b ) { return !(a==b); }
std::string str( const SleptonPoint& d );                            //<! printing helper
std::ostream& operator<< ( std::ostream& m, const SleptonPoint& d ); //<! printing helper (wraps above)

/*-----------------------------------------------------*/

//! A class to read the slepton xsections from a root file.
/*!
  Read the cross section values from a tree in a root file.
  The tree should have the following leaves;
  - mSL
  - mN
  - finalState
  - crossSection
  If there are multiple entries in the tree corresponding to the same
  SleptonPoint, you can either average them, or take the last one, or
  drop them all.

  If one wants to retrieve the values by dataset id, then one should
  also provide a xsec text file, used to map dsid<-->SleptonPoint.
  See constructor for default filenames.

  davide.gerbaudo@gmail.com
  Feb 2013
 */
class SleptonXsecReader {

 public:
  struct PointWithXsecCounts {
    PointWithXsecCounts() : xsec_(0.), xsecErr_(0.), counts_(0.) {};
    PointWithXsecCounts(SleptonPoint p, float xs, float xsE, unsigned int cnt=1) : point_(p), xsec_(xs), xsecErr_(xsE), counts_(cnt) {};
    SleptonPoint point_;
    float xsec_;
    float xsecErr_;
    unsigned int counts_;
  };
  typedef std::vector< PointWithXsecCounts > Vpxs;
 public:
  struct PointWithDsid {
    PointWithDsid() : dsid_(0.) {};
    PointWithDsid(SleptonPoint p, int dsid) : point_(p), dsid_(dsid) {};
    SleptonPoint point_;
    int dsid_;
  };
  typedef std::vector< PointWithDsid > Vpd;
 public:
  enum WorkingMode {
	kAverageMultipleEntries = 0,
	kLastOfMultipleEntries,
	kDropMultipleEntries,
  };

 public:
  SleptonXsecReader(WorkingMode mode=kAverageMultipleEntries, bool verbose=false);
  SleptonXsecReader(const char* txtDsidFileName,
                    const char* rootFileName, const char* treeName,
					WorkingMode=kAverageMultipleEntries, bool verbose=false);
  int readFile(const char* fileName, const char* treeName); //!< return number of valid entries found
  float getCrossSection(const SleptonPoint &point) const;
  float getCrossSection(const int &datasetId, const int &finalState) const;
  float getCrossSectionError(const SleptonPoint &point) const;
  float getCrossSectionError(const int &datasetId, const int &finalState) const;
  void printKnownPoints() const;
  static bool extractDsetidMslM1FromString(const char* dsetName, int &dsid, int &msl, int &m1, bool verbose=false);
  static bool getRootcoreBin(std::string &dir); //!< safe getenv; return false if ROOTCOREBIN not defined
  static std::string getDefaultDsidFilename(); //!< file with the dsid and mass values
  static std::string getDefaultRootFilename(); //!< file with the tree containing the xsec values 
  static std::string getDefaultTreeName();     //!< tree containing the xsec values
  public:
  bool verbose_;     //!< toggle on/off messages
  WorkingMode mode_; //!< defines how multiple entries for the same point are handled
 private:
  bool processEntry(int mSl, int mN, int finalState, float xs, float xsErr); //!< true if it leads to a new el. in points_
  int populateDsidList(const char* filename="");
 private:
  Vpxs points_;    //!< points with xsec, extracted from the root file
  Vpd knownDsids_; //!< known list of dataset IDs (used to map to (mSl,m1) and retrieve the xsec
}; //end SleptonXsecReader

std::string str( const SleptonXsecReader& d );                            //<! printing helper
std::ostream& operator<< ( std::ostream& m, const SleptonXsecReader& d ); //<! printing helper (wraps above)



#endif // SLEPTONXSECREADER_H
