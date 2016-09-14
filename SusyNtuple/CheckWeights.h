//  -*- c++ -*-
#ifndef SusyAna_CheckWeights_h
#define SusyAna_CheckWeights_h


// Root Packages
#include "TTree.h"

// Susy Common
#include "SusyNtuple/SusyNtAna.h"

#include <fstream>

/// Three lepton cutflow
/**
General script to implement basic selection with all signal region cut
methods.
*/

class CheckWeights : public SusyNtAna
{

  public:

    CheckWeights();
    virtual ~CheckWeights(){};

    void setCheckNumber(int number) { m_check_number = number; }
    void setTotal(int number) { m_total = number; }

    // Init is called when TTree (or TChain) is attached
    virtual void    Init(TTree* tree);
    // Begin is called before looping on entries
    virtual void    Begin(TTree* tree);
    // Terminate is called after looping is finished
    virtual void    Terminate();

    // Main event loop function
    virtual Bool_t  Process(Long64_t entry);

    ClassDef(CheckWeights, 1);

  protected:
    int m_check_number;
    int m_total;

};

#endif
