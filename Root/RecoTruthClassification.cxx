#include "SusyNtuple/RecoTruthClassification.h"

#include <cstdlib> // abs
#include <iostream>

//____________________________________________________________________________//
bool isRealLepton(int Origin, int Type, int mcMatchPdgId, bool isTau)
{
  if(isTau) return true;

  // when reco ele from conversion of photon radiated from a truth muon, 
  // conversions are conversions always, never prompt electrons
  if(abs(mcMatchPdgId) == 13 && Type==4 && Origin==5) return false;

  // avoid returning true if type is not electron or muon (ex: type 17 by chance close to prompt lep)
  return (Type==21||Type==1||Type==2||Type==3||Type==4||Type==5||Type==6||Type==7||Type==8); 
}
//____________________________________________________________________________//
bool isConvLepton(int Origin, int Type, int mcMatchPdgId, bool isTau)
{
  return isFakeLepton(Origin, Type, mcMatchPdgId,  isTau) 
    && (Origin == 5||Origin == 6||Origin == 7); 
}
//____________________________________________________________________________//
bool isFakeLepton(int Origin, int Type, int mcMatchPdgId, bool isTau)
{
  return !isRealLepton(Origin, Type, mcMatchPdgId, isTau); 
}
//____________________________________________________________________________//
bool isHFLepton(int Origin, int Type, int mcMatchPdgId, bool isTau)
{
  return isFakeLepton(Origin, Type, mcMatchPdgId, isTau) && 
  (Origin == 25 || Origin == 26 || Origin == 27 ||
   Origin == 28 || Origin == 29 || Origin == 32 || Origin == 33); 
}
//____________________________________________________________________________//
bool isLFLepton(int Origin, int Type, int mcMatchPdgId, bool isTau)
{
  return isFakeLepton(Origin, Type, mcMatchPdgId, isTau) &&
    !isConvLepton(Origin, Type, mcMatchPdgId, isTau) && 
    !isHFLepton(Origin, Type, mcMatchPdgId, isTau); 
}
//____________________________________________________________________________//
bool isJFLepton(int Origin, int Type, int mcMatchPdgId, bool isTau)
{
  return isFakeLepton(Origin, Type, mcMatchPdgId, isTau) && 
    !isConvLepton(Origin, Type, mcMatchPdgId, isTau); 
}
//____________________________________________________________________________//
bool isUKLepton(int Origin, int Type, int mcMatchPdgId, bool isTau)
{
  return isFakeLepton(Origin, Type, mcMatchPdgId, isTau) && 
    Type == 0 && Origin == 0; 
}
//____________________________________________________________________________//
int fakeType(int Origin, int Type, int mcMatchPdgId, bool isTau)
{
  if(isTau) {
    if (isRealLepton(Origin, Type, mcMatchPdgId, isTau) ) return PROMPT; // Prompt
    else return UK; // Unknown
  }
  if (isRealLepton(Origin, Type, mcMatchPdgId, isTau) ) return PROMPT; // Prompt
  else if (Origin == 5  || Origin == 6  || Origin == 7) return CONV; // Conversions
  else if (Origin == 25 || Origin == 26 || Origin == 27 ||
	   Origin == 28 || Origin == 29 || Origin == 32 || Origin == 33) return HF; // Heavy Flavour
  else if (!(Type == 0 && Origin == 0)) return LF; // Light Flavour
  else return UK; // Unknown
}

//____________________________________________________________________________//
bool isPromptElectron(int type, int origin)
{
    return ((type==21 || type==1 || type==2 || type==3) && 
            (origin==1 || origin==12 || origin==13 || 
             origin==10 || origin==14 || origin==15 || 
             origin==16 || origin==17 || origin==22 || 
             origin==43 || origin==44)); // should we add those from leptonic tau decay?
}
