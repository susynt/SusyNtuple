#include "SusyNtuple/WhTruthExtractor.h"

#include "SusyNtuple/vec_utils.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;
using std::find;
using std::setw;

namespace sut = susy::utils;
namespace smc =susy::mc;

//----------------------------------
// initialization of const vector is cumbersome; we need these ugly tmp arrays.
int tmpArr1[] = {smc::kPb,   smc::kAb};
int tmpArr2[] = {smc::kPw,   smc::kAw};
int tmpArr3[] = {smc::kPmu,  smc::kAmu};
int tmpArr4[] = {smc::kPtau, smc::kAtau};

WhTruthExtractor::WhTruthExtractor() :
  verbose_(false),
  pdgsPbAb_    (tmpArr1, tmpArr1 + sizeof(tmpArr1)/sizeof(int)),
  pdgsPwAw_    (tmpArr2, tmpArr2 + sizeof(tmpArr2)/sizeof(int)),
  pdgsPmuAmu_  (tmpArr3, tmpArr3 + sizeof(tmpArr3)/sizeof(int)),
  pdgsPtauAtau_(tmpArr4, tmpArr4 + sizeof(tmpArr4)/sizeof(int)),
  interestingHiggs_(-1),
  decay_(smc::kUnknown)
{
}
//----------------------------------
smc::Hdecays WhTruthExtractor::update(const vint_t* pdg, const vvint_t *childIndex, const vvint_t *parentIndex)
{
  hIndices_ = findHiggsIndices(*pdg);
  buildHiggsChildrenPgds(*pdg, *childIndex);
  buildHiggsParentsPgds(*pdg, *parentIndex);
  interestingHiggs_ = firstInterestingHiggs();
  decay_ = (interestingHiggs_ < 0 ?
            smc::kUnknown : decayType(static_cast<size_t>(interestingHiggs_)));
  if(verbose_) printStatus();
  return decay_;
}
//----------------------------------
void WhTruthExtractor::printStatus() const
{
  using sut::vecToString;
  cout<<" hIndices ["<<vecToString(hIndices_)<<"]"<<endl;
  for(size_t iH=0; iH<hIndices_.size(); iH++){
    cout<<"  higgs["<<hIndices_[iH]<<"]"
	<<" parents : "<<vecToString(hParPdgs_[iH])
	<<" children : "<<vecToString(hChiPdgs_[iH])
	<<endl;
  } // end for(iH)
  cout<<"interesting higgs : "<<hIndices_[interestingHiggs_]
      <<" with decay '"<<smc::decayToString(decay_)<<"'"<<endl;
}
//--------------------------------------
WhTruthExtractor::vint_t WhTruthExtractor::findHiggsIndices(const vint_t &pdg)
{
  smc::IsHiggsIndices ihi;
  ihi = for_each(pdg.begin(), pdg.end(), ihi);
  return ihi.higgsIndices_;
}
//----------------------------------
void WhTruthExtractor::buildHiggsChildrenPgds(const vint_t &pdgs, const vvint_t &childIndices)
{
  hChiPdgs_.resize(hIndices_.size());
  for(size_t iH=0; iH<hIndices_.size(); ++iH) {
    vint_t &chPdgs = hChiPdgs_[iH];
    const vint_t &chIdxs = childIndices[ hIndices_[iH] ];
    chPdgs.resize(chIdxs.size());
    std::transform(chIdxs.begin(), chIdxs.end(), chPdgs.begin(), smc::IndexToPdg(pdgs));
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
    std::transform(parIdxs.begin(), parIdxs.end(), parPdgs.begin(), smc::IndexToPdg(pdgs));
  } // end for(iH)
}
//----------------------------------
bool WhTruthExtractor::isBoringHiggs(size_t iHiggs) const
{
  using sut::containsAnyOf;
  if(iHiggs >= hIndices_.size()) return true;
  return (hChiPdgs_[iHiggs].size()<2
        ||
        containsAnyOf(hChiPdgs_[iHiggs], vint_t(1, smc::kPh)));
}
//----------------------------------
int WhTruthExtractor::firstInterestingHiggs() const
{
  for(size_t iH=0; iH<hIndices_.size(); ++iH) if(!isBoringHiggs(iH)) return iH;
  return -1;
}
//----------------------------------
smc::Hdecays WhTruthExtractor::decayType(size_t iHiggs) const
{
  using sut::containsAnyOf;
  if(iHiggs >= hIndices_.size())                  return smc::kUnknown;
  const vint_t &children = hChiPdgs_[iHiggs];
  if      (containsAnyOf(children, pdgsPwAw_))      return smc::kPwAw;
  else if (containsAnyOf(children, vint_t(1, smc::kPz))) return smc::kZZ;
  else if (containsAnyOf(children, pdgsPtauAtau_))  return smc::kPtauAtau;
  else if (containsAnyOf(children, pdgsPbAb_))      return smc::kPbAb;
  else if (containsAnyOf(children, pdgsPmuAmu_))    return smc::kPmuAmu;
  else                                              return smc::kUnknown;
}
//----------------------------------
void WhTruthExtractor::printEvent(const vint_t &pdg, const vint_t &status,
                                  const vvint_t &childIndex, const vvint_t &parentIndex)
{
  using std::left;
  using std::right;
  using sut::vecToString;
  int maxNpartToPrint=70;
  int colW=12;
  cout
    <<"--------------------------------"<<endl
    << left  << setw(colW)<<"i"
    << right << setw(colW)<<"id"
    << right << setw(colW)<<"status"
    << right << setw(colW)<<"par"
    << right << setw(colW)<<"chil"
    << endl
    <<"--------------------------------"<<endl;

  for(int iP=0;iP<maxNpartToPrint; ++iP){
    int id = pdg.at(iP);
    cout
      << left  << setw(colW)<<iP
      << right << setw(colW)<<id
      << right << setw(colW)<<status[iP]
      << right << setw(colW)<<vecToString(parentIndex.at(iP))
      << right << setw(colW)<<vecToString(childIndex.at(iP))
      << endl;
  } // end for(iP)
}
//----------------------------------
WhTruthExtractor::vint_t WhTruthExtractor::ttbarMcAtNloParticles(const vint_t *pdgs,
                                                                 const vvint_t *childrenIndices)

{
  // actually identify the particles we want to store
  vint_t particles;
  smc::IsSmTopIndex   topFilter(pdgs, childrenIndices);
  smc::IsDecayingWIndex wFilter(pdgs, childrenIndices);
  int nParts(pdgs->size());
  for(int i=0; i<nParts; ++i){
    if(topFilter(i)){
      particles.push_back(i);
      vint_t bs(sut::filter(childrenIndices->at(i), std::not1(smc::IsBIndex(pdgs))));
      particles.insert(particles.end(), bs.begin(), bs.end());
    } else if(wFilter(i)){
      particles.push_back(i);
      const vint_t &chIdxs = childrenIndices->at(i);
      particles.insert(particles.end(), chIdxs.begin(), chIdxs.end());
    }
  }
  sut::removeDuplicates(particles);
  return particles;
}
//----------------------------------
WhTruthExtractor::vint_t WhTruthExtractor::higgsEventParticleIndices(const vint_t* pdg,
                                                                     const vvint_t *childIndex,
                                                                     const vvint_t *parentIndex)
{
    vint_t indices;
    update(pdg, childIndex, parentIndex);
    bool interesting_H_was_found = (interestingHiggs_ >=0);
    if(interesting_H_was_found){
        const int hIdx = hIndices_[interestingHiggs_];
        const vint_t &chIdxs  = childIndex ->at(hIdx);
        indices.push_back(hIdx);
        indices.insert(indices.end(), chIdxs.begin(), chIdxs.end());
        // note to self: the h parent is usually another h; no need to store parents
    }
    return indices;
}
//----------------------------------
