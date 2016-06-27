#include <iostream>
#include "SusyNtuple/Event.h"

using namespace std;
using Susy::Event;

/*--------------------------------------------------------------------------------*/
// Event print
/*--------------------------------------------------------------------------------*/
void Event::print() const
{
  cout << "Run " << run;
  if(isMC) cout << " MCID " << mcChannel;
  if(isMC) cout << " treatAsYear " << treatAsYear;
  cout << " Event " << eventNumber;
  if(isMC) cout << " proc " << susyFinalState;
  cout << " Stream " << streamName(stream) 
       << " w " << w << endl;
  cout << " Initial (pre-derivation) number of events processed: " << initialNumberOfEvents;
  cout << "    sumw: " << sumOfEventWeights << "   sumw2: " << sumOfEventWeightsSquared << endl;
}
