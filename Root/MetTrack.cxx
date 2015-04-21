#include "SusyNtuple/MetTrack.h"

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::setw;
using std::fixed;

namespace Susy
{
MetTrack::MetTrack(const MetTrack &rhs) :
    TObject(rhs),
    Et(rhs.Et),
    phi(rhs.phi),
    sumet(rhs.sumet),
    sys(rhs.sys)
{}
/*--------------------------------------------------------------------------------*/
// Assignment operator
/*--------------------------------------------------------------------------------*/
MetTrack& MetTrack::operator=(const MetTrack &rhs)
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
// MetTrack print
/*--------------------------------------------------------------------------------*/
void MetTrack::print() const
{
    cout.precision(2);
    cout << fixed << "MetTrack : pt " << setw(6) << Et << " phi " << setw(4) << phi
         << endl;
    cout.precision(6);
    cout.unsetf(std::ios_base::fixed);
}
//----------------------------------------------------------
} // Susy
