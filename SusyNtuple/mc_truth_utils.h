// Dear emacs, this is -*- c++ -*-
#ifndef SUSY_MC_TRUTH_UTILS_H
#define SUSY_MC_TRUTH_UTILS_H

/*
  MonteCarlo truth utilities
  

  davide.gerbaudo@gmail.com
  April 2014
*/

#include <functional>
#include <string>
#include <vector>

namespace Susy{
namespace mc{

typedef std::vector< int > vint_t;
typedef std::vector< vint_t > vvint_t;

// 2014-01-17: Order of enum changed so that kUnknown is zero
enum Hdecays {
    kUnknown = 0,
    kPwAw,
    kZZ,
    kPtauAtau,
    kPbAb,
    kPmuAmu,
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

//! convert Hdecay to a string
std::string decayToString(const Hdecays &d);
//! find the pdg of the parent
/*!
  Useful when there are intermediate duplicates and one needs to navigate up the decay chain
*/
int determineParentPdg(const vint_t *pdgs, const vvint_t *parentsIndices, const int &particleIndex);
//! functor that navigates up the chain when there are intermediate particles
struct IntermediateParentWalker {
    const vint_t &pdgs_;
    const vvint_t &parsIdxs_;
    size_t i_; //!< index current particle
    int pdg_; //!< pdg current particle
    vint_t parIndices_; //!< indices parents of current particle
    vint_t parPdgs_; //!< pdgs parents of current particle
    IntermediateParentWalker(const vint_t *pdgs, const vvint_t *parentsIndices, const int &particleIndex);
    int defaultParentPdg() const { return -999; }
    //! take the first one, usually fine
    int parentPdg() const { return parPdgs_.size()>0 ? parPdgs_[0] : defaultParentPdg(); }
    void setIndexCurrentParticle(const int &i);
    bool hasParent() { return parIndices_.size()>0; }
    bool hasSelfParent();
    size_t indexSelfParent();
    void walkUp();
};

//! match criterion to be used with for_each; store indices of higgs
struct IsHiggsIndices:
  public std::unary_function<const int&, void> {
  IsHiggsIndices(): counter_(0) { }
  void operator()(const int &p) { if(p==kPh) higgsIndices_.push_back(counter_); counter_++; }
  vint_t higgsIndices_;
  size_t counter_;
};

//! function object to map indices to pdg ids
struct IndexToPdg {
  const vint_t &pdgs_;
  IndexToPdg(const vint_t &pdgs) : pdgs_(pdgs) {}
  int operator()(const int &e) const { return pdgs_[e]; }
};

//! function object to filter b and bbar
struct IsBIndex : public std::unary_function<int, bool> {
  IsBIndex(const std::vector<int> *pdgs) : pdgs_(*pdgs) {}
  const std::vector<int> &pdgs_;
  bool operator() (int i) const { return (pdgs_[i]==kPb || pdgs_[i]==kAb); }
};

//! given an index, provide the pdg
struct PdgGetter{
  const std::vector<int> &pdgs_;
  PdgGetter(const std::vector<int> *pdgs) : pdgs_(*pdgs) {}
  int operator() (const int &i) const { return pdgs_[i]; }
};

//! determine whether the index correspond to a SM top that has pdg (-6,+6) and decays qqbar or lv
struct IsSmTopIndex {
    const vint_t &pdgs_;
    const vvint_t &chIdxs_;
    IsSmTopIndex(const vint_t *pdgs, const vvint_t *childrenIndices);
    bool isTop(const int &i);
    bool isTopChild(const int &i);
    bool areAllTopChildren(const int &itop);
    bool operator() (const int & i);
}; // end isSmTop

//! function to find Ws that decay, and avoid intermediate ones
struct IsDecayingWIndex {
    const vint_t &pdgs_;
    const vvint_t &chIdxs_;
    IsDecayingWIndex(const vint_t *pdgs, const vvint_t *childrenIndices);
    bool isW(const int &i);
    bool isWintermediate(const int &iW);
    bool isWChild(const int &i);
    bool areAllWChildren(const int &iW);
    bool operator() (const int & i);
}; // end isDecayingW


} // utils
} // susy
#endif
