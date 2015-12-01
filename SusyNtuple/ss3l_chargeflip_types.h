//  -*- c++ -*-
#ifndef SUSYNTUPLE_SS3L_CHARGEFLIP_TYPES_H
#define SUSYNTUPLE_SS3L_CHARGEFLIP_TYPES_H

/// possible return values from the ss3l charge flip truth matching
/**
   from
   https://indico.cern.ch/event/448798/contribution/3/attachments/1161377/1672220/truthMatching.pdf
   and
   https://svnweb.cern.ch/trac/atlasphys-susy/browser/Physics/SUSY/Analyses/SameSignLeptonsJets/trunk/Ntuples_NoSyst/SS3LAnalysis/SS3LAnalysis/chargeflip.h

   version 1.1, 21/09/2015
 */
enum
{
	CHARGE_UNKNOWN = 0,
	CHARGE_CORRECT = 1,
	CHARGE_FLIPPED = 2,
	CHARGE_MAYBE_CORRECT = 3,
	CHARGE_MAYBE_FLIPPED = 4,
	CHARGE_AMBIGUOUS = 5,
	CHARGE_MCTRUTHCLASSIFIER_CORRECT = 6,
	CHARGE_MCTRUTHCLASSIFIER_FLIPPED = 7
};
#endif
