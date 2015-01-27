#ifndef WHTRUTHEXTRACTOR_H
#define WHTRUTHEXTRACTOR_H

#include "SusyNtuple/mc_truth_utils.h"

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
 public:
  WhTruthExtractor();
  susy::mc::Hdecays update(const vint_t* pdg, const vvint_t *childIndex, const vvint_t *parentIndex);
  susy::mc::Hdecays decay() const {return decay_;}
  void printStatus() const;
  //! indices of relevant particles (top, W, and their children) in a MC@NLO ttbar event
  /*! We might want to rename WhTruthExtractor to some generic truth extractor class.
    Discuss this with Steve.
    Davide, Aug 2013
   */
  static vint_t ttbarMcAtNloParticles(const vint_t *pdgs,
                                      const vvint_t *childrenIndices);
  /// indices of the relevant particle for a higgs event (H, its parents, its children)
  /**
     Internally calls @update()
  */
 vint_t higgsEventParticleIndices(const vint_t* pdg, const vvint_t *childIndex, const vvint_t *parentIndex);
 public:
  bool verbose_;
  const vint_t pdgsPbAb_;
  const vint_t pdgsPwAw_;
  const vint_t pdgsPmuAmu_;
  const vint_t pdgsPtauAtau_;
  static void printEvent(const vint_t &pdg, const vint_t &status,
                         const vvint_t &childIndex, const vvint_t &parentIndex);
 private:
  susy::mc::vint_t findHiggsIndices(const vint_t &pdg);
  void buildHiggsChildrenPgds(const vint_t &pdgs, const vvint_t &childIndices);
  void buildHiggsParentsPgds(const vint_t &pdgs, const vvint_t &parentIndices);
  bool isBoringHiggs(size_t iHiggs) const; //!< intermediate higgs have < 2 children or another higgs as child
  int firstInterestingHiggs() const; //!< internal index 1st interesting higgs; -1 if none
  susy::mc::Hdecays decayType(size_t iHiggs) const; //!< classify the decay of the i^th higgs
  //! print table with all particles, mostly for development/debugging
  vint_t hIndices_;
  vvint_t hParPdgs_;
  vvint_t hChiPdgs_;
  int interestingHiggs_;
  susy::mc::Hdecays decay_;
}; // end WhTruthExtractor
#endif
