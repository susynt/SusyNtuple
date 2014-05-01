#include "SusyNtuple/WhTruthExtractor.h"

#include "SusyNtuple/vec_utils.h"

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
  using susy::utils::vecToString;
  cout<<" hIndices ["<<vecToString(hIndices_)<<"]"<<endl;
  for(size_t iH=0; iH<hIndices_.size(); iH++){
    cout<<"  higgs["<<hIndices_[iH]<<"]"
	<<" parents : "<<vecToString(hParPdgs_[iH])
	<<" children : "<<vecToString(hChiPdgs_[iH])
	<<endl;
  } // end for(iH)
  cout<<"interesting higgs : "<<hIndices_[interestingHiggs_]
      <<" with decay '"<<WhTruthExtractor::decayToString(decay_)<<"'"<<endl;
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
  using susy::utils::containsAnyOf;
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
  using susy::utils::containsAnyOf;
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
  using susy::utils::vecToString;
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

  // actually identify the particles we want to store
  vint_t particles;
  isSmTop topFilter(pdgs, childrenIndices);
  isDecayingW wFilter(pdgs, childrenIndices);
  int nParts(pdgs->size());
  for(int i=0; i<nParts; ++i){
    if(topFilter(i)){
      particles.push_back(i);
      vint_t bs(susy::utils::filter(childrenIndices->at(i), std::not1(isBfilter(pdgs))));
      particles.insert(particles.end(), bs.begin(), bs.end());
    } else if(wFilter(i)){
      particles.push_back(i);
      const vint_t &chIdxs = childrenIndices->at(i);
      particles.insert(particles.end(), chIdxs.begin(), chIdxs.end());
    }
  }
  susy::utils::removeDuplicates(particles);
  return particles;
}
//----------------------------------
int WhTruthExtractor::determineParentPdg(const vint_t *pdgs, const vvint_t *parentsIndices,
                                         const int &particleIndex)
{
    //! functor that navigates up the chain when there are intermediate particles
    struct IntermediateParentWalker {
        const vint_t &pdgs_;
        const vvint_t &parsIdxs_;
        size_t i_; //!< index current particle
        int pdg_; //!< pdg current particle
        vint_t parIndices_; //!< indices parents of current particle
        vint_t parPdgs_; //!< pdgs parents of current particle
        IntermediateParentWalker(const vint_t *pdgs, const vvint_t *parentsIndices, const int &particleIndex) :
            pdgs_(*pdgs), parsIdxs_(*parentsIndices), i_(0), pdg_(0) {
            setIndexCurrentParticle(particleIndex);
        }
        int defaultParentPdg() const { return -999; }
        //! take the first one, usually fine
        int parentPdg() const { return parPdgs_.size()>0 ? parPdgs_[0] : defaultParentPdg(); }
        void setIndexCurrentParticle(const int &i) {
            i_ = i;
            pdg_ = pdgs_[i];
            parIndices_ = parsIdxs_[ i ];
            parPdgs_.resize(parIndices_.size());
            std::transform(parIndices_.begin(), parIndices_.end(), parPdgs_.begin(), IndexToPdg(pdgs_));
        }
        bool hasParent() { return parIndices_.size()>0; }
        bool hasSelfParent() {
            return find(parPdgs_.begin(), parPdgs_.end(), pdg_) != parPdgs_.end();
        }
        size_t indexSelfParent() {
            vint_t::iterator it = find(parPdgs_.begin(), parPdgs_.end(), pdg_);
            return parIndices_[std::distance(parPdgs_.begin(), it)];
        }
        void walkUp() {
            while(hasParent() && hasSelfParent())
                setIndexCurrentParticle(indexSelfParent());
        }
    };
    IntermediateParentWalker ipw(pdgs, parentsIndices, particleIndex);
    int parentPdg = ipw.defaultParentPdg();
    if(ipw.hasParent()){
        if(!ipw.hasSelfParent())
            parentPdg = ipw.parentPdg();
        else  {
            ipw.walkUp();
            if(ipw.hasParent()) parentPdg = ipw.parentPdg();
        }
    }
    return parentPdg;
}
//----------------------------------
