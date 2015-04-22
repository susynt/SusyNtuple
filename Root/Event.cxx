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
  cout << " Event " << event;
  if(isMC) cout << " proc " << susyFinalState;
  cout << " Stream " << streamName(stream) 
       << " w " << w << endl;
}
