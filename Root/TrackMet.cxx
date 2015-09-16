#include "SusyNtuple/TrackMet.h"

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::setw;
using std::fixed;

namespace Susy
{
TrackMet::TrackMet(const TrackMet &rhs) :
    TObject(rhs),
    Et(rhs.Et),
    phi(rhs.phi),
    sumet(rhs.sumet),
    // ele term
    refEle_et(rhs.refEle_et),
    refEle_phi(rhs.refEle_phi),
    refEle_sumet(rhs.refEle_sumet),
    // muo term
    refMuo_et(rhs.refMuo_et),
    refMuo_phi(rhs.refMuo_phi),
    refMuo_sumet(rhs.refMuo_sumet),
    // jet term
    refJet_et(rhs.refJet_et),
    refJet_phi(rhs.refJet_phi),
    refJet_sumet(rhs.refJet_sumet),
    // soft term
    softTerm_et(rhs.softTerm_et),
    softTerm_phi(rhs.softTerm_phi),
    softTerm_sumet(rhs.softTerm_sumet),
    sys(rhs.sys)
{}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
TrackMet& TrackMet::operator=(const TrackMet &rhs)
{
    if (this != &rhs){
        TObject::operator=(rhs);
        Et = rhs.Et;
        phi = rhs.phi;
        sumet = rhs.sumet;
        // ele term
        refEle_et = rhs.refEle_et;
        refEle_phi = rhs.refEle_phi;
        refEle_sumet = rhs.refEle_sumet;
        // muo term
        refMuo_et = rhs.refMuo_et;
        refMuo_phi = rhs.refMuo_phi;
        refMuo_sumet = rhs.refMuo_sumet;
        // jet term
        refJet_et = rhs.refJet_et;
        refJet_phi = rhs.refJet_phi;
        refJet_sumet = rhs.refJet_sumet;
        // soft term
        softTerm_et = rhs.softTerm_et;
        softTerm_phi = rhs.softTerm_phi;
        softTerm_sumet = rhs.softTerm_sumet;

        sys = rhs.sys;
    }
    return *this;
}
/*--------------------------------------------------------------------------------*/
// TrackMet print
/*--------------------------------------------------------------------------------*/
void TrackMet::print() const
{
    cout.precision(2);
    cout << fixed << "TrackMet : pt " << setw(6) << Et << " phi " << setw(4) << phi
         << endl;
    cout.precision(6);
    cout.unsetf(std::ios_base::fixed);
}
//----------------------------------------------------------
} // Susy
