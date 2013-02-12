#include "CalibrationDataInterface/CalibrationDataInterfaceROOT.h"
#include "SusyNtuple/BTagCalib.h"


std::pair<vector<float>, vector<float> > BTagCalib::BTagCalibrationFunction(const vector<float>& pt,
									    const vector<float>& eta,
									    const vector<float>& bWeight,
									    const vector<int>& pdgid,
									    std::string taggerName,
									    std::string OP,
									    float opval,
									    bool isJVF,
									    std::string calibration,
									    std::string calibfolder) {

  return BTagCalibrationFunction(pt, eta, bWeight, bWeight, pdgid, taggerName, OP, opval, opval, isJVF, calibration, calibfolder);
  
}




std::pair<vector<float>, vector<float> > BTagCalib::BTagCalibrationFunction(const vector<float>& pt, 
									    const vector<float>& eta,
									    const vector<float>& bWeight1, 
									    const vector<float>& bWeight2,
									    const vector<int>& pdgid,
									    std::string taggerName,
									    std::string OP,
									    float opval1,
									    float opval2,
									    bool isJVF,
									    std::string calibration,
									    std::string calibfolder) {			  
  
  // ***************************************************************************************************************
  // ****                 Function for applying Scale Factors and associated Errors to Analyses                 ****
  // ***************************************************************************************************************
  //
  // - Input Jet information: Pt, Eta, SV0, PDG ID. First three are vectors of floats and last one is vector of ints
  // - Input option for applying shift due to errors. Takes integer value: 0 = Default, 1 = Shift Up, 2 = Shift Down
  // - Function then uses input values to feed into root file, which provides Scale Factors and associated errors
  //   as a function of Pt and Eta. These Scale Factors are then used to calculate the weights applied to individual
  //   jets and the event as a whole.
  // - Function then returns a pair of vector :
  //   first : vector of the weights in the following order : 
  // 0 : standard
  // 1 : B-jet efficiency down 
  // 2 : C-jet efficiency down 
  // 3 : mistag rate efficiency down 
  // 4 : B-jet efficiency up 
  // 5 : C-jet efficiency up 
  // 6 : mistag rate efficiency up
  // The uncertainties on the B, C and mistag rate efficiencies have to be added in quadrature at the end of the analysis
  // second : vector of floats containing the weights for the individual jets in the event.
  //
  // - IMPORTANT: SCALE FACTORS NOT DEFINED BELOW JET PT OF 20 GeV OR |ETA| ABOVE 2.5!!!
  //            
  // - The efficiency and inefficiency SFs for b and c-jets are valid from 20 to 200 GeV.
  // - The efficiency SFs for light-jets are valid from 20 to 750 GeV. 
  // - The inefficiency SFs for light-jets are valid from 20 to 200 GeV
  //   For jets with pT above these limits, values for either 200 GeV or 750 GeV will be used (handled internally now).
  //
  // ***************************************************************************************************************
  
  //! Create the BTagCalib object
  static Analysis::CalibrationDataInterfaceROOT* BTagCalib = new Analysis::CalibrationDataInterfaceROOT(taggerName,calibration,calibfolder);
  
  vector<float> EventWeight(7,1.);	
  vector<float> jetWeightVector;
  
  //! Set BTag Calibration variables
  std::string label;          //! Flavour, Light = "N/A", or "C", "B"
  
  
  //! Perform actions for each individual jet in an event
  for (unsigned int jitr = 0; jitr < pt.size(); jitr++) {
    // If a jet was out of acceptance, the return a weight of one
    if (pt[jitr]<0){
      jetWeightVector.push_back(1.);
      EventWeight.push_back(1.);
      continue;
    }
    
    
    //! Set quark flavour to be used
    if (pdgid[jitr] == 4) label = "C";
    else if (pdgid[jitr] == 5) label = "B";
    else if (pdgid[jitr] == 15) label = "T";	 	 
    else label = "Light"; // previously it was "N/A";

        //! Set necessary b-tag variables for retrieving SF + errors
       Analysis::CalibrationDataVariables BTagVars;
    BTagVars.jetAuthor = "AntiKt4TopoLCnoJVF";
    if(isJVF) BTagVars.jetAuthor = "AntiKt4TopoLCJVF";
    BTagVars.jetPt  = pt[jitr];
    BTagVars.jetEta = eta[jitr];
    
    Analysis::Uncertainty BTagUnc = Analysis::Total; //Systematic; //! None, Total, Statistical, Systematic
    
    Analysis::CalibResult BTagCalibResult;
    if(bWeight1[jitr]>opval1 && bWeight2[jitr]>opval2)
			BTagCalibResult = BTagCalib->getScaleFactor(BTagVars, label, OP, BTagUnc); //b-tagged jet
    else BTagCalibResult = BTagCalib->getInefficiencyScaleFactor(BTagVars, label, OP, BTagUnc); //not b-tagged jet

    float jetWeight = (float)BTagCalibResult.first;
    
    jetWeightVector.push_back(jetWeight);
    
    float jetWeight_B_down = jetWeight;
    float jetWeight_B_up = jetWeight;	
    float jetWeight_C_down = jetWeight;
    float jetWeight_C_up = jetWeight;	
    float jetWeight_L_down = jetWeight;
    float jetWeight_L_up = jetWeight;		
    
    //Treat efficiency and the inefficiency scale factor uncertainties as anti-correlated
    if(pdgid[jitr]==5) 
    {
      if(bWeight1[jitr]>opval1 && bWeight2[jitr]>opval2)
      {
	jetWeight_B_down -= (float)BTagCalibResult.second;
	jetWeight_B_up += (float)BTagCalibResult.second;	
      }
      else  
      {
	jetWeight_B_down += (float)BTagCalibResult.second;
	jetWeight_B_up -= (float)BTagCalibResult.second;
      }
    }
    else if(pdgid[jitr]==4 || pdgid[jitr]==15) 
    {
      if(bWeight1[jitr]>opval1 && bWeight2[jitr]>opval2)
      {
	jetWeight_C_down -= (float)BTagCalibResult.second;
	jetWeight_C_up += (float)BTagCalibResult.second;	
      }
      else
      {
	jetWeight_C_down += (float)BTagCalibResult.second;
	jetWeight_C_up -= (float)BTagCalibResult.second;		    	    
      }
    }	
    else if(bWeight1[jitr]>opval1 && bWeight2[jitr]>opval2) 
    {
      jetWeight_L_down -= (float)BTagCalibResult.second;
      jetWeight_L_up += (float)BTagCalibResult.second;	
    }
    else 
    {
      jetWeight_L_down += (float)BTagCalibResult.second;
      jetWeight_L_up -= (float)BTagCalibResult.second;		 
    } 
    
    
    
    EventWeight.at(0) *= jetWeight;
    EventWeight.at(1) *= jetWeight_B_down;
    EventWeight.at(2) *= jetWeight_C_down;
    EventWeight.at(3) *= jetWeight_L_down;
    EventWeight.at(4) *= jetWeight_B_up;
    EventWeight.at(5) *= jetWeight_C_up;
    EventWeight.at(6) *= jetWeight_L_up;	 
    
  }
  std::pair<vector<float>, vector<float> > weights_pair (EventWeight, jetWeightVector);
  
  return weights_pair;
}

