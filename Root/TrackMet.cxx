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
