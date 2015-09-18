#include "SusyNtuple/MT2.h"
#include "TLorentzVector.h"
#include <stdlib.h>
#include <stdio.h>

/*
===========================================================================
This is a class for computing (asymmetric) MT2 using the bisection method. 
Created: October 17, 2014
Author: Christopher Lester
Maintainers: Christopher Lester (lester@hep.phy.cam.ac.uk) and Benjamin Nachman (bnachman@cern.ch)
===========================================================================

===============
Instructions:
===============

For TLorentzVectors visa, visb, metp (the visible momenta and the vector sum of the missed 
particles) an for trial masses ma and mb, 

ComputeMT2 mt2class = ComputeMT2(visa,visb,metp,0.,80.);
double mt2 = mt2class.Compute();

you can also do something like

ComputeMT2 mt2class = ComputeMT2(visa,visb,metp,"Wboson","Neutrino");
double mt2 = mt2class.Compute();

to use the W boson and neutrino masses as trial masses.  If you use a name that
is not in the database, it will list them for you and quit the program.

If you want to know the actual values of the missing particle momenta that minimize MT2, you can do
std::pair <double,double> sols = mt2class.get_solutions();
double solution1 = sols.first;
double solution2 = sols.first;

*/

class ComputeMT2 {
 public:
  double particletype(TString part);
  void listparticles();
  ComputeMT2(TLorentzVector visa, TLorentzVector visb, TLorentzVector MET, double ma=0., double mb=0.);
  ComputeMT2(TLorentzVector visa, TLorentzVector visb, TLorentzVector MET, TString name1, TString name2);
  ~ComputeMT2();
  double Compute(bool useActualVisMass = true, double visaMass = 0., double visbMass = 0.) ;
  std::pair <double,double> get_solutions();
 private:
  void Init(TLorentzVector visa, TLorentzVector visb, TLorentzVector MET, double ma, double mb);
  TLorentzVector V1;
  TLorentzVector V2;
  TLorentzVector ET;
  double Ma;
  double Mb;
  double myMT2;
  double sol1;
  double sol2; 
};
