#include "SusyNtuple/mc_truth_utils.h"

#include <algorithm>

using namespace Susy::mc;
using Susy::mc::IntermediateParentWalker;

//----------------------------------
std::string Susy::mc::decayToString(const Hdecays &d)
{
  switch(d) {
  case kPwAw :     return "WW";
  case kZZ :       return "ZZ";
  case kPtauAtau : return "tautau";
  case kPbAb :     return "bbar";
  case kPmuAmu :   return "mumu";
  case kUnknown :  return "unknown";
  default :        return "undefined";
  }
}
//----------------------------------
IntermediateParentWalker::IntermediateParentWalker(const vint_t *pdgs,
                                                   const vvint_t *parentsIndices,
                                                   const int &particleIndex) :
    pdgs_(*pdgs),
    parsIdxs_(*parentsIndices),
    i_(0),
    pdg_(0)
{
    setIndexCurrentParticle(particleIndex);
}
//----------------------------------
void IntermediateParentWalker::setIndexCurrentParticle(const int &i) {
    i_ = i;
    pdg_ = pdgs_[i];
    parIndices_ = parsIdxs_[ i ];
    parPdgs_.resize(parIndices_.size());
    std::transform(parIndices_.begin(), parIndices_.end(), parPdgs_.begin(), IndexToPdg(pdgs_));
}
//----------------------------------
bool IntermediateParentWalker::hasSelfParent()
{
    return find(parPdgs_.begin(), parPdgs_.end(), pdg_) != parPdgs_.end();
}
//----------------------------------
size_t IntermediateParentWalker::indexSelfParent()
{
    vint_t::iterator it = find(parPdgs_.begin(), parPdgs_.end(), pdg_);
    return parIndices_[std::distance(parPdgs_.begin(), it)];
}
//----------------------------------
void IntermediateParentWalker::walkUp()
{
    while(hasParent() && hasSelfParent())
        setIndexCurrentParticle(indexSelfParent());
}
//----------------------------------
int Susy::mc::determineParentPdg(const vint_t *pdgs, const vvint_t *parentsIndices,
                                 const int &particleIndex)
{
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
IsSmTopIndex::IsSmTopIndex(const vint_t *pdgs, const vvint_t *childrenIndices) :
    pdgs_(*pdgs),
    chIdxs_(*childrenIndices)
{
}
bool IsSmTopIndex::isTop(const int &i)
{
    const int &p=pdgs_[i];
    return(p==kPt || p==kAt);
}
bool IsSmTopIndex::isTopChild(const int &i)
{
    const int p=abs(pdgs_[i]);
    return(p==kPd || p==kPs || p==kPb || p==kPw || p==kPglu); // can radiate gluon
}
bool IsSmTopIndex::areAllTopChildren(const int &itop)
{
    size_t cnt(0);
    const vint_t &chIdxs = chIdxs_[itop];
    for(size_t i=0; i<chIdxs.size(); ++i)
        if(isTopChild(chIdxs[i]))
            cnt++;
    return cnt==chIdxs.size();
}
bool IsSmTopIndex::operator() (const int & i)
{
    return isTop(i) && areAllTopChildren(i);
}
//----------------------------------
IsDecayingWIndex::IsDecayingWIndex(const vint_t *pdgs, const vvint_t *childrenIndices) :
    pdgs_(*pdgs),
    chIdxs_(*childrenIndices)
{
}
bool IsDecayingWIndex::isW(const int &i)
{
    const int &p=pdgs_[i];
    return(p==kPw || p==kAw);
}
bool IsDecayingWIndex::isWintermediate(const int &iW)
{
    const vint_t &chIdxs = chIdxs_[iW];
    for(size_t i=0; i<chIdxs.size(); ++i)
        if(isW(chIdxs[i]))
            return true;
    return false;
}
bool IsDecayingWIndex::isWChild(const int &i)
{
    const int p=abs(pdgs_[i]);
    return(p==kPd || p==kPu || p==kPs || p==kPc || p==kPb
           || p==kPel || p==kPve || p==kPmu || p==kPvm || p==kPtau || p==kPvt
           || p==kPgam); // can radiate gamma
}
bool IsDecayingWIndex::areAllWChildren(const int &iW)
{
    size_t cnt(0);
    const vint_t &chIdxs = chIdxs_[iW];
    for(size_t i=0; i<chIdxs.size(); ++i)
        if(isWChild(chIdxs[i]))
            cnt++;
    return cnt==chIdxs.size();
}
bool IsDecayingWIndex::operator() (const int & i)
{
    return isW(i) && !isWintermediate(i) && areAllWChildren(i);
}
//----------------------------------
