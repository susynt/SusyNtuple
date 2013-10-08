#include "SusyNtuple/WhTruthExtractor.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>

using std::vector;
using std::cout;
using std::endl;
using std::find;
using std::setw;


//----------------------------------
// initialization of const vector is cumbersome; we need these ugly tmp arrays.
int tmpArr1[] = {WhTruthExtractor::kPb,   WhTruthExtractor::kAb};
int tmpArr2[] = {WhTruthExtractor::kPw,   WhTruthExtractor::kAw};
int tmpArr3[] = {WhTruthExtractor::kPmu,  WhTruthExtractor::kAmu};
int tmpArr4[] = {WhTruthExtractor::kPtau, WhTruthExtractor::kAtau};

WhTruthExtractor::WhTruthExtractor() :
  verbose_(false),
  pdgsPbAb_    (tmpArr1, tmpArr1 + sizeof(tmpArr1)/sizeof(int)),
  pdgsPwAw_    (tmpArr2, tmpArr2 + sizeof(tmpArr2)/sizeof(int)),
  pdgsPmuAmu_  (tmpArr3, tmpArr3 + sizeof(tmpArr3)/sizeof(int)),
  pdgsPtauAtau_(tmpArr4, tmpArr4 + sizeof(tmpArr4)/sizeof(int)),
  interestingHiggs_(-1),
  decay_(WhTruthExtractor::kUnknown)
{

}

//----------------------------------
WhTruthExtractor::Hdecays WhTruthExtractor::update(const vint_t* pdg, const vvint_t *childIndex, const vvint_t *parentIndex)
{
  hIndices_ = findHiggsIndices(*pdg);
  buildHiggsChildrenPgds(*pdg, *childIndex);
  buildHiggsParentsPgds(*pdg, *parentIndex);
  interestingHiggs_ = firstInterestingHiggs();
  decay_ = (interestingHiggs_ < 0 ?
	    WhTruthExtractor::kUnknown : decayType(static_cast<size_t>(interestingHiggs_)));
  if(verbose_) printStatus();
  return decay_;
}
//----------------------------------
void WhTruthExtractor::printStatus() const
{
  printVector(hIndices_, "hIndices");
  for(size_t iH=0; iH<hIndices_.size(); iH++){
    cout<<"  higgs["<<hIndices_[iH]<<"]"
	<<" parents : "<<vecToString(hParPdgs_[iH])
	<<" children : "<<vecToString(hChiPdgs_[iH])
	<<endl;
  } // end for(iH)
  cout<<"interesting higgs : "<<hIndices_[interestingHiggs_]
      <<" with decay '"<<WhTruthExtractor::decayToString(decay_)<<"'"<<endl;
}
//----------------------------------
bool WhTruthExtractor::containsAnyOf(const vint_t &firstVec, const vint_t &subVec)
{
  typedef std::set< int > sint;
  sint a(firstVec.begin(), firstVec.end());
  sint b(subVec.begin(), subVec.end());
  vint_t intersection(std::min(a.size(), b.size()));
  vint_t::iterator it = set_intersection(a.begin(), a.end(),
					 b.begin(), b.end(),
					 intersection.begin());
  return it!=intersection.begin();
}
//--------------------------------------
// match criterion to be used with find_if; store indices of higgs
struct IsHiggsIndices:
  public std::unary_function<const int&, void> {
  IsHiggsIndices(): counter_(0) { }
  void operator()(const int &p) { if(p==WhTruthExtractor::kPh) higgsIndices_.push_back(counter_); counter_++; }
  WhTruthExtractor::vint_t higgsIndices_;
  size_t counter_;
};
//----------------------------------
WhTruthExtractor::vint_t WhTruthExtractor::findHiggsIndices(const vint_t &pdg)
{
  IsHiggsIndices ihi;
  ihi = for_each(pdg.begin(), pdg.end(), ihi);
  return ihi.higgsIndices_;
}
//----------------------------------
// function object to map indices to pdg ids
struct IndexToPdg {
  const WhTruthExtractor::vint_t &pdgs_;
  IndexToPdg(const WhTruthExtractor::vint_t &pdgs) : pdgs_(pdgs) {}
  int operator()(const int &e) const { return pdgs_[e]; }
};
//----------------------------------
void WhTruthExtractor::buildHiggsChildrenPgds(const vint_t &pdgs, const vvint_t &childIndices)
{
  hChiPdgs_.resize(hIndices_.size());
  for(size_t iH=0; iH<hIndices_.size(); ++iH) {
    vint_t &chPdgs = hChiPdgs_[iH];
    const vint_t &chIdxs = childIndices[ hIndices_[iH] ];
    chPdgs.resize(chIdxs.size());
    std::transform(chIdxs.begin(), chIdxs.end(), chPdgs.begin(), IndexToPdg(pdgs));
  } // end for(iH)
}
//----------------------------------
void WhTruthExtractor::buildHiggsParentsPgds(const vint_t &pdgs, const vvint_t &parentIndices)
{
  hParPdgs_.resize(hIndices_.size());
  for(size_t iH=0; iH<hIndices_.size(); ++iH) {
    vint_t &parPdgs = hParPdgs_[iH];
    const vint_t &parIdxs = parentIndices[ hIndices_[iH] ];
    parPdgs.resize(parIdxs.size());
    std::transform(parIdxs.begin(), parIdxs.end(), parPdgs.begin(), IndexToPdg(pdgs));
  } // end for(iH)
}
//----------------------------------
bool WhTruthExtractor::isBoringHiggs(size_t iHiggs) const
{
  if(iHiggs >= hIndices_.size()) return true;
  return (hChiPdgs_[iHiggs].size()<2
	  ||
	  containsAnyOf(hChiPdgs_[iHiggs], vint_t(1, WhTruthExtractor::kPh)));
}
//----------------------------------
int WhTruthExtractor::firstInterestingHiggs() const
{
  for(size_t iH=0; iH<hIndices_.size(); ++iH) if(!isBoringHiggs(iH)) return iH;
  return -1;
}
//----------------------------------
WhTruthExtractor::Hdecays WhTruthExtractor::decayType(size_t iHiggs) const
{
  if(iHiggs >= hIndices_.size())                  return WhTruthExtractor::kUnknown;
  const vint_t &children = hChiPdgs_[iHiggs];
  if      (containsAnyOf(children, pdgsPwAw_))      return WhTruthExtractor::kPwAw;
  else if (containsAnyOf(children, vint_t(1, kPz))) return WhTruthExtractor::kZZ;
  else if (containsAnyOf(children, pdgsPtauAtau_))  return WhTruthExtractor::kPtauAtau;
  else if (containsAnyOf(children, pdgsPbAb_))      return WhTruthExtractor::kPbAb;
  else if (containsAnyOf(children, pdgsPmuAmu_))    return WhTruthExtractor::kPmuAmu;
  else                                              return WhTruthExtractor::kUnknown;
}
//----------------------------------
void WhTruthExtractor::printEvent(const vint_t &pdg, const vvint_t &childIndex, const vvint_t &parentIndex)
{
  using std::left;
  using std::right;
  int maxNpartToPrint=70;
  int colW=20;
  cout
    <<"--------------------------------"<<endl
    << left  << setw(colW)<<"i"
    << right << setw(colW)<<"id"
    << right << setw(colW)<<"par"
    << right << setw(colW)<<"chil"
    << endl
    <<"--------------------------------"<<endl;

  for(int iP=0;iP<maxNpartToPrint; ++iP){
    int id = pdg.at(iP);
    cout
      << left  << setw(colW)<<iP
      << right << setw(colW)<<id
      << right << setw(colW)<<vecToString(parentIndex.at(iP))
      << right << setw(colW)<<vecToString(childIndex.at(iP))
      << endl;
  } // end for(iP)
}
//----------------------------------
std::string WhTruthExtractor::vecToString(const vint_t &vec)
{
  std::stringstream ss;
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(ss,", "));
  return ss.str();
}
//----------------------------------
std::string WhTruthExtractor::vecVecToString(const vvint_t &vvec)
{
  std::stringstream ss;
  for(size_t i=0; i<vvec.size(); i++) ss<<"["<<WhTruthExtractor::vecToString(vvec[i])<<"], ";
  return ss.str();
}
//----------------------------------
void WhTruthExtractor::printVector(const vint_t &vec, const char* label)
{
  cout<<label<<"[";
  std::ostream_iterator< int > out_it (cout,", ");
  std::copy(vec.begin(), vec.end(), out_it);
  cout<<"]"<<endl;
}
//----------------------------------
std::string WhTruthExtractor::decayToString(const WhTruthExtractor::Hdecays &d)
{
  switch(d) {
  case WhTruthExtractor::kPwAw :     return "WW";
  case WhTruthExtractor::kZZ :       return "ZZ";
  case WhTruthExtractor::kPtauAtau : return "tautau";
  case WhTruthExtractor::kPbAb :     return "bbar";
  case WhTruthExtractor::kPmuAmu :   return "mumu";
  case WhTruthExtractor::kUnknown :  return "unknown";
  default :                          return "undefined";
  }
}
//----------------------------------
// Util functions for ttbarMcAtNloParticles
//----------------------------------

// filter a vector (remove elements for which Predicate is true)
template < template <typename, typename> class Container,
           typename Predicate,
           typename Allocator,
           typename A
           >
Container<A, Allocator> filter(Container<A, Allocator> const & container, Predicate const & pred) {
  Container<A, Allocator> filtered(container);
  filtered.erase(remove_if(filtered.begin(), filtered.end(), pred), filtered.end());
  return filtered;
}

struct isBfilter : public std::unary_function<int, bool> {
  isBfilter(const std::vector<int> *pdgs) : pdgs_(*pdgs) {}
  const std::vector<int> &pdgs_;
  bool operator() (int i) const { return (pdgs_[i]==WhTruthExtractor::kPb || pdgs_[i]==WhTruthExtractor::kAb); }
};

struct PdgGetter{
  const std::vector<int> &pdgs_;
  PdgGetter(const std::vector<int> *pdgs) : pdgs_(*pdgs) {}
  int operator() (const int &i) const { return pdgs_[i]; }
};

struct RemoveDuplicatesFunctor {
  void operator() (std::vector<int> &vec) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }
} removeDuplicates;

WhTruthExtractor::vint_t WhTruthExtractor::ttbarMcAtNloParticles(const vint_t *pdgs,
                                                                 const vvint_t *childrenIndices)

{
  //! a SM top has pdg (-6,+6) and decays qqbar or lv
  struct isSmTop {
    const vint_t &pdgs_;
    const vvint_t &chIdxs_;
    isSmTop(const vint_t *pdgs, const vvint_t *childrenIndices) :
      pdgs_(*pdgs), chIdxs_(*childrenIndices) {}
    bool isTop(const int &i) { const int &p=pdgs_[i]; return(p==kPt || p==kAt); }
    bool isTopChild(const int &i) {
      const int p=abs(pdgs_[i]);
      return(p==kPd || p==kPs || p==kPb || p==kPw || p==kPglu); // can radiate gluon
    }
    bool areAllTopChildren(const int &itop) {
      size_t cnt(0);
      const vint_t &chIdxs = chIdxs_[itop];
      for(size_t i=0; i<chIdxs.size(); ++i) if(isTopChild(chIdxs[i])) cnt++;
      return cnt==chIdxs.size();
    }
    bool operator() (const int & i) { return isTop(i) && areAllTopChildren(i); }
  }; // end isSmTop

  //! function to find Ws that decay, and avoid intermediate ones
  struct isDecayingW {
    const vint_t &pdgs_;
    const vvint_t &chIdxs_;
    isDecayingW(const vint_t *pdgs, const vvint_t *childrenIndices) :
      pdgs_(*pdgs), chIdxs_(*childrenIndices) {}
    bool isW(const int &i) { const int &p=pdgs_[i]; return(p==kPw || p==kAw); }
    bool isWintermediate(const int &iW) {
      const vint_t &chIdxs = chIdxs_[iW];
      for(size_t i=0; i<chIdxs.size(); ++i) if(isW(chIdxs[i])) return true;
      return false;
    }
    bool isWChild(const int &i) {
      const int p=abs(pdgs_[i]);
      return(p==kPd || p==kPu || p==kPs || p==kPc || p==kPb
             || p==kPel || p==kPve || p==kPmu || p==kPvm || p==kPtau || p==kPvt
             || p==kPgam); // can radiate gamma
    }
    bool areAllWChildren(const int &iW) {
      size_t cnt(0);
      const vint_t &chIdxs = chIdxs_[iW];
      for(size_t i=0; i<chIdxs.size(); ++i) if(isWChild(chIdxs[i])) cnt++;
      return cnt==chIdxs.size();
    }
    bool operator() (const int & i) { return isW(i) && !isWintermediate(i) && areAllWChildren(i); }
  }; // end isDecayingW

  vint_t particles;
  isSmTop topFilter(pdgs, childrenIndices);
  isDecayingW wFilter(pdgs, childrenIndices);
  int nParts(pdgs->size());
  for(int i=0; i<nParts; ++i){
    if(topFilter(i)){
      particles.push_back(i);
      vint_t bs(filter(childrenIndices->at(i), std::not1(isBfilter(pdgs))));
      particles.insert(particles.end(), bs.begin(), bs.end());
    } else if(wFilter(i)){
      particles.push_back(i);
      const vint_t &chIdxs = childrenIndices->at(i);
      particles.insert(particles.end(), chIdxs.begin(), chIdxs.end());
    }
  }
  removeDuplicates(particles);

  std::vector<int> foo; foo.resize(particles.size());
  std::transform (particles.begin(), particles.end(), foo.begin(), PdgGetter(pdgs));
  cout<<"indices : "<<vecToString(particles)
      <<" pdgs : "<<vecToString(foo)
      <<endl;
  return particles;
}
//----------------------------------
