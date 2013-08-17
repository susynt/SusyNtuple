#ifndef WHTRUTHEXTRACTOR_H
#define WHTRUTHEXTRACTOR_H
#include <vector>
#include <string>

//! A class to extract from d3pd the truth information for simplifiedModel_wA_noslep_WH samples

/*!
  This class goes through the pdg, parents, and children stored in
  a d3pd file, and extract pieces of information about the Higgs decay.
  For now, the main piece of information is the decay type of the Higgs.

  See this wiki for more information about the samples:
  https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSignalGridDirectSlepton

  davide.gerbaudo@gmail.com
  Feb 2013
 */

class WhTruthExtractor {
 public:
  typedef std::vector< int > vint_t;
  typedef std::vector< vint_t > vvint_t;
  enum Hdecays {
    kPwAw,
    kZZ,
    kPtauAtau,
    kPbAb,
    kPmuAmu,
    kUnknown,
  };
  enum PdgIds{
    kPd=+1, kAd=-1,
    kPu=+2, kAu=-2,
    kPs=+3, kAs=-3,
    kPc=+4, kAc=-4,
    kPb=+5, kAb=-5,
    kPt=+6, kAt=-6,
    kPel=+11, kAel=-11,
    kPve=+12, kAve=-12,
    kPmu=+13, kAmu=-13,
    kPvm=+14, kAvm=-14,
    kPtau=+15, kAtau=-15,
    kPvt=+16, kAvt=-16,
    kPglu=21,
    kPgam=22,
    kPz=+23,
    kPw=+24, kAw=-24,
    kPh=+25
  };
 public:
  WhTruthExtractor();
  Hdecays update(const vint_t* pdg, const vvint_t *childIndex, const vvint_t *parentIndex);
  Hdecays decay() const {return decay_;}
  void printStatus() const;
  //! returns true if the intersection of the two vectors is non-empty
  static bool containsAnyOf(const vint_t &firstVec, const vint_t &subVec);
  static void printVector(const vint_t &vec, const char* label="");
  static std::string vecToString(const vint_t &vec);
  static std::string vecVecToString(const vvint_t &vvec);
  static std::string decayToString(const WhTruthExtractor::Hdecays &d);
  //! indices of relevant particles (top, W, and their children) in a MC@NLO ttbar event
  /*! We might want to rename WhTruthExtractor to some generic truth extractor class.
    Discuss this with Steve.
    Davide, Aug 2013
   */
  static vint_t ttbarMcAtNloParticles(const vint_t *pdgs,
                                      const vvint_t *childrenIndices);
 public:
  bool verbose_;
  const vint_t pdgsPbAb_;
  const vint_t pdgsPwAw_;
  const vint_t pdgsPmuAmu_;
  const vint_t pdgsPtauAtau_;
 private:
  WhTruthExtractor::vint_t findHiggsIndices(const vint_t &pdg);
  void buildHiggsChildrenPgds(const vint_t &pdgs, const vvint_t &childIndices);
  void buildHiggsParentsPgds(const vint_t &pdgs, const vvint_t &parentIndices);
  bool isBoringHiggs(size_t iHiggs) const; //!< intermediate higgs have < 2 children or another higgs as child
  int firstInterestingHiggs() const; //!< internal index 1st interesting higgs; -1 if none
  WhTruthExtractor::Hdecays decayType(size_t iHiggs) const; //!< classify the decay of the i^th higgs
  //! print table with all particles, mostly for development/debugging
  void printEvent(const vint_t &pdg, const vvint_t &childIndex, const vvint_t &parentIndex);
  vint_t hIndices_;
  vvint_t hParPdgs_;
  vvint_t hChiPdgs_;
  int interestingHiggs_;
  Hdecays decay_;
}; // end WhTruthExtractor
#endif
