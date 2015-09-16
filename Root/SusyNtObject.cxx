#include "SusyNtuple/SusyNtObject.h"

using namespace std;
using namespace Susy;

#include <iostream>


/*--------------------------------------------------------------------------------*/
// SusyNtObject constructor for writing only.
/*--------------------------------------------------------------------------------*/
SusyNtObject::SusyNtObject():
        evt(this, "event", 0),
        ele(this, "electrons", 0),
        muo(this, "muons", 0),
        jet(this, "jets", 0),
	    pho(this, "photons", 0),
	    tau(this, "taus", 0),
        met(this, "met", 0),
        tkm(this, "trackMet", 0),
        tpr(this, "truthParticles", 0),
        tjt(this, "truthJets", 0),
        tmt(this, "truthMet", 0)
{
}

/*--------------------------------------------------------------------------------*/
// SusyNtObject constructor for reading and writing
/*--------------------------------------------------------------------------------*/
SusyNtObject::SusyNtObject(const Long64_t& entry):
        evt(this, "event", &entry),
        ele(this, "electrons", &entry),
        muo(this, "muons", &entry),
        jet(this, "jets", &entry),
	    pho(this, "photons", &entry),
	    tau(this, "taus", &entry),
        met(this, "met", &entry),
        tkm(this, "trackMet", &entry),
        tpr(this, "truthParticles", &entry),
        tjt(this, "truthJets", &entry),
        tmt(this, "truthMet", &entry)
{
}

/*--------------------------------------------------------------------------------*/
// Set branches active for writing.  Only active branches are written
/*--------------------------------------------------------------------------------*/
void SusyNtObject::SetActive()
{
  evt.SetActive(true);
  ele.SetActive(true);
  muo.SetActive(true);
  jet.SetActive(true);
  pho.SetActive(true);
  tau.SetActive(true);
  met.SetActive(true);
  tkm.SetActive(true);
  tpr.SetActive(true); 
  tjt.SetActive(true); 
  tmt.SetActive(true);
}

/*--------------------------------------------------------------------------------*/
// Connect the objects to an output tree
/*--------------------------------------------------------------------------------*/
void SusyNtObject::WriteTo(TTree* tree)
{
  evt.WriteTo(tree);
  ele.WriteTo(tree);
  muo.WriteTo(tree);
  jet.WriteTo(tree);
  pho.WriteTo(tree);
  tau.WriteTo(tree);
  met.WriteTo(tree);
  tkm.WriteTo(tree);
  tpr.WriteTo(tree);
  tjt.WriteTo(tree);
  tmt.WriteTo(tree);
}

/*--------------------------------------------------------------------------------*/
// Connect the objects to an input tree
/*--------------------------------------------------------------------------------*/
void SusyNtObject::ReadFrom(TTree* tree)
{
  evt.ReadFrom(tree);
  ele.ReadFrom(tree);
  muo.ReadFrom(tree);
  jet.ReadFrom(tree);
  pho.ReadFrom(tree);
  tau.ReadFrom(tree);
  met.ReadFrom(tree);
  tkm.ReadFrom(tree);
  tpr.ReadFrom(tree);
  tjt.ReadFrom(tree);
  tmt.ReadFrom(tree);
}

/*--------------------------------------------------------------------------------*/
// Clear the variables when in read mode
/*--------------------------------------------------------------------------------*/
void SusyNtObject::clear()
{
  evt()->clear();
  ele()->clear();
  muo()->clear();
  jet()->clear();
  pho()->clear();
  tau()->clear();
  met()->clear();
  tkm()->clear();
  tpr()->clear();
  tjt()->clear();
  tmt()->clear();
}
