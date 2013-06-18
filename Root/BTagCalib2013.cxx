#include "CalibrationDataInterface/CalibrationDataInterfaceROOT.h"
#include "SusyNtuple/BTagCalib2013.h"

BTagCalib2013::BTagCalib2013( const std::string& taggerName , const std::string& calibration , const std::string& calibfolder , const std::string& OP , bool isJVF , double opval1 , double opval2 ){
  m_BTagCalib = new Analysis::CalibrationDataInterfaceROOT(taggerName,calibration,calibfolder);

  m_jetAuthor = "AntiKt4TopoLCnoJVF";
  if(isJVF) m_jetAuthor = "AntiKt4TopoLCJVF";

  m_sfIndex_C = m_sfIndex_B = m_sfIndex_T = m_sfIndex_L = 0;
  bool worked = m_BTagCalib->retrieveCalibrationIndex( "C" , OP , m_jetAuthor , true , m_sfIndex_C );
  worked = m_BTagCalib->retrieveCalibrationIndex( "B" , OP , m_jetAuthor , true , m_sfIndex_B );
  worked = m_BTagCalib->retrieveCalibrationIndex( "T" , OP , m_jetAuthor , true , m_sfIndex_T );
  worked = m_BTagCalib->retrieveCalibrationIndex( "Light" , OP , m_jetAuthor , true , m_sfIndex_L );

  m_insfIndex_C = m_insfIndex_B = m_insfIndex_T = m_insfIndex_L = -1;
  worked = m_BTagCalib->retrieveCalibrationIndex( "C" , OP , m_jetAuthor , false , m_insfIndex_C );
  worked = m_BTagCalib->retrieveCalibrationIndex( "B" , OP , m_jetAuthor , false , m_insfIndex_B );
  worked = m_BTagCalib->retrieveCalibrationIndex( "T" , OP , m_jetAuthor , false , m_insfIndex_T );
  worked = m_BTagCalib->retrieveCalibrationIndex( "Light" , OP , m_jetAuthor , false , m_insfIndex_L );

  m_opval1 = opval1;
  if (opval2>0) m_opval2 = opval2;
  else          m_opval2 = opval1;
}

std::pair<std::vector<float>, std::vector<float> >* BTagCalib2013::BTagCalibrationFunction(const std::vector<float>& pt,
										       const std::vector<float>& eta,
										       const std::vector<float>& bWeight,
										       const std::vector<int>& pdgid,
										       bool isSherpa){
  return BTagCalibrationFunction(pt, eta, bWeight, bWeight, pdgid, isSherpa);
}




std::pair<std::vector<float>, std::vector<float> >* BTagCalib2013::BTagCalibrationFunction(const std::vector<float>& pt, 
										       const std::vector<float>& eta,
										       const std::vector<float>& bWeight1, 
										       const std::vector<float>& bWeight2,
										       const std::vector<int>& pdgid,
										       bool isSherpa) {
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
  
  std::vector<float> EventWeight(7,1.);	
  std::vector<float> jetWeightVector(pt.size(),1.);
  
  //! Set BTag Calibration variables
  //std::string label;          //! Flavour, Light = "N/A", or "C", "B"
  int sfIndex = -1;
  int insfIndex = -1;

  //! Perform actions for each individual jet in an event
  for (unsigned int jitr = 0; jitr < pt.size(); jitr++) {
    // If a jet was out of acceptance, the return a weight of one
    if (pt[jitr]<0){
      jetWeightVector[jitr] = 1.;
      //.push_back(1.);
      //EventWeight.push_back(1.);
      continue;
    }

/*    //! Set quark flavour to be used
    if (pdgid[jitr] == 4) label = "C";
    else if (pdgid[jitr] == 5) label = "B";
    else if (pdgid[jitr] == 15) label = "T";
    else label = "Light"; // previously it was "N/A";
*/
    
    //! Set quark flavour to be used
    if (pdgid[jitr] == 4){
      sfIndex = m_sfIndex_C;
      insfIndex = m_insfIndex_C;
    } else if (pdgid[jitr] == 5){
      sfIndex = m_sfIndex_B;
      insfIndex = m_insfIndex_B;
    } else if (pdgid[jitr] == 15){
      sfIndex = m_sfIndex_T;
      insfIndex = m_insfIndex_T;
    } else {
      sfIndex = m_sfIndex_L;
      insfIndex = m_insfIndex_L;
    }

   //! Sherpa additional scale factors 
    float Sherpa_sf = 1.;
    if(isSherpa)
    {
      float pTbinEdges[11]={20.0,30.0,40.0,50.0,60.0,75.0,90.0,110.0,140.0,200.0,14000.0};
      
      float SherpaBEta12Eff[10]={0.503149,0.587658,0.640698,0.670935,0.698329,0.71546,0.728238,0.733176,0.732302,0.703178};
      float McAtNloBEta12Eff[10]={0.601933,0.669075,0.709769,0.734397,0.755355,0.769026,0.779684,0.784209,0.781448,0.754663};
      float SherpaBEtaGT12Eff[10]={0.414948,0.498414,0.558093,0.592192,0.624685,0.644518,0.657145,0.665846,0.66005,0.617568};
      float McAtNloBEtaGT12Eff[10]={0.487291,0.56725,0.616989,0.648154,0.676655,0.695549,0.709394,0.710949,0.703369,0.661734};
      
      float SherpaCEta12Eff[10]={0.131813,0.15916,0.182347,0.196998,0.211756,0.221406,0.22644,0.229084,0.224435,0.211912};
      float McAtNloCEta12Eff[10]={0.166871,0.189851,0.21086,0.221371,0.235092,0.245027,0.25199,0.254255,0.255771,0.238571};
      float SherpaCEtaGT12Eff[10]={0.0899759,0.113708,0.134007,0.145204,0.158004,0.166745,0.169704,0.171959,0.169002,0.15506};
      float McAtNloCEtaGT12Eff[10]={0.114779,0.138703,0.159295,0.169321,0.186907,0.197272,0.204476,0.206179,0.202801,0.185518};
      
      float SherpaTEta12Eff[10]={0.0545276,0.0622382,0.0731923,0.0817876,0.0897496,0.0986202,0.106865,0.111345,0.112738,0.0969457};
      float McAtNloTEta12Eff[10]={0.0565259,0.0632809,0.0761104,0.0830634,0.0932884,0.0993728,0.10682,0.1114,0.117661,0.101395};
      //Due to lacking statistics the same efficiencies for |eta|<1.2 are used for |eta|>1.2
      float SherpaTEtaGT12Eff[10]={0.0545276,0.0622382,0.0731923,0.0817876,0.0897496,0.0986202,0.106865,0.111345,0.112738,0.0969457};
      float McAtNloTEtaGT12Eff[10]={0.0565259,0.0632809,0.0761104,0.0830634,0.0932884,0.0993728,0.10682,0.1114,0.117661,0.101395};
      
      float SherpaLightEta12Eff[10]={0.00891998,0.011756,0.0138216,0.0147977,0.0162266,0.0167734,0.0169798,0.0171518,0.0170799,0.0166981};
      float McAtNloLightEta12Eff[10]={0.00567163,0.00608742,0.00619453,0.00640946,0.006543,0.00672126,0.00724751,0.0073207,0.00783033,0.00936298};
      float SherpaLightEtaGT12Eff[10]={0.00796589,0.0100371,0.0120175,0.0129341,0.0145191,0.0153309,0.0157342,0.0156472,0.01601,0.0159805};
      float McAtNloLightEtaGT12Eff[10]={0.00531424,0.00548512,0.00573528,0.00608869,0.00669183,0.0071079,0.00763569,0.00847156,0.0101617,0.0123613};
      
      for (int i=0; i<10; i++){
	if (pt[jitr]/1000.>=pTbinEdges[i] && pt[jitr]/1000.<pTbinEdges[i+1]){
	  if (pdgid[jitr]==5 && fabs(eta[jitr])<1.2 && bWeight1[jitr]>m_opval1) Sherpa_sf=McAtNloBEta12Eff[i]/SherpaBEta12Eff[i];
	  else if (pdgid[jitr]==5 && fabs(eta[jitr])>=1.2 && bWeight1[jitr]>m_opval1) Sherpa_sf=McAtNloBEtaGT12Eff[i]/SherpaBEtaGT12Eff[i];
	  else if (pdgid[jitr]==5 && fabs(eta[jitr])<1.2 && bWeight1[jitr]<=m_opval1) Sherpa_sf=(1.0-McAtNloBEta12Eff[i])/(1.0-SherpaBEta12Eff[i]);
	  else if (pdgid[jitr]==5 && fabs(eta[jitr])>=1.2 && bWeight1[jitr]<=m_opval1) Sherpa_sf=(1.0-McAtNloBEtaGT12Eff[i])/(1.0-SherpaBEtaGT12Eff[i]);
	  
	  else if (pdgid[jitr]==4 && fabs(eta[jitr])<1.2 && bWeight1[jitr]>m_opval1) Sherpa_sf=McAtNloCEta12Eff[i]/SherpaCEta12Eff[i];
	  else if (pdgid[jitr]==4 && fabs(eta[jitr])>=1.2 && bWeight1[jitr]>m_opval1) Sherpa_sf=McAtNloCEtaGT12Eff[i]/SherpaCEtaGT12Eff[i];
	  else if (pdgid[jitr]==4 && fabs(eta[jitr])<1.2 && bWeight1[jitr]<=m_opval1) Sherpa_sf=(1.0-McAtNloCEta12Eff[i])/(1.0-SherpaCEta12Eff[i]);
	  else if (pdgid[jitr]==4 && fabs(eta[jitr])>=1.2 && bWeight1[jitr]<=m_opval1) Sherpa_sf=(1.0-McAtNloCEtaGT12Eff[i])/(1.0-SherpaCEtaGT12Eff[i]);
	  
	  else if (pdgid[jitr]==15 && fabs(eta[jitr])<1.2 && bWeight1[jitr]>m_opval1) Sherpa_sf=McAtNloTEta12Eff[i]/SherpaTEta12Eff[i];
	  else if (pdgid[jitr]==15 && fabs(eta[jitr])>=1.2 && bWeight1[jitr]>m_opval1) Sherpa_sf=McAtNloTEtaGT12Eff[i]/SherpaTEtaGT12Eff[i];
	  else if (pdgid[jitr]==15 && fabs(eta[jitr])<1.2 && bWeight1[jitr]<=m_opval1) Sherpa_sf=(1.0-McAtNloTEta12Eff[i])/(1.0-SherpaTEta12Eff[i]);
	  else if (pdgid[jitr]==15 && fabs(eta[jitr])>=1.2 && bWeight1[jitr]<=m_opval1) Sherpa_sf=(1.0-McAtNloTEtaGT12Eff[i])/(1.0-SherpaTEtaGT12Eff[i]);
	  
	  else if (pdgid[jitr]==0 && fabs(eta[jitr])<1.2 && bWeight1[jitr]>m_opval1) Sherpa_sf=McAtNloLightEta12Eff[i]/SherpaLightEta12Eff[i];
	  else if (pdgid[jitr]==0 && fabs(eta[jitr])>=1.2 && bWeight1[jitr]>m_opval1) Sherpa_sf=McAtNloLightEtaGT12Eff[i]/SherpaLightEtaGT12Eff[i];
	  else if (pdgid[jitr]==0 && fabs(eta[jitr])<1.2 && bWeight1[jitr]<=m_opval1) Sherpa_sf=(1.0-McAtNloLightEta12Eff[i])/(1.0-SherpaLightEta12Eff[i]);
	  else if (pdgid[jitr]==0 && fabs(eta[jitr])>=1.2 && bWeight1[jitr]<=m_opval1) Sherpa_sf=(1.0-McAtNloLightEtaGT12Eff[i])/(1.0-SherpaLightEtaGT12Eff[i]);
	}
      } 
    }


    //! Set necessary b-tag variables for retrieving SF + errors
    Analysis::CalibrationDataVariables BTagVars;
    BTagVars.jetAuthor = m_jetAuthor;
    BTagVars.jetPt  = pt[jitr];
    BTagVars.jetEta = eta[jitr];
    
    Analysis::Uncertainty BTagUnc = Analysis::Total; //Systematic; //! None, Total, Statistical, Systematic
    
    Analysis::CalibResult BTagCalibResult;
    if(bWeight1[jitr]>m_opval1 && bWeight2[jitr]>m_opval2)
            BTagCalibResult = m_BTagCalib->getScaleFactor( BTagVars , sfIndex , BTagUnc );
    else BTagCalibResult = m_BTagCalib->getInefficiencyScaleFactor(BTagVars, sfIndex, insfIndex, BTagUnc); //not b-tagged jet
//			BTagCalibResult = m_BTagCalib->getScaleFactor(BTagVars, label, OP, BTagUnc); //b-tagged jet
//    else BTagCalibResult = m_BTagCalib->getInefficiencyScaleFactor(BTagVars, label, OP, BTagUnc); //not b-tagged jet

    float jetWeight = (float)BTagCalibResult.first*Sherpa_sf;

    jetWeightVector[jitr] = jetWeight; //.push_back(jetWeight);

    float jetWeight_B_down = jetWeight;
    float jetWeight_B_up = jetWeight;	
    float jetWeight_C_down = jetWeight;
    float jetWeight_C_up = jetWeight;	
    float jetWeight_L_down = jetWeight;
    float jetWeight_L_up = jetWeight;		
    
    //Treat efficiency and the inefficiency scale factor uncertainties as anti-correlated
    if(pdgid[jitr]==5) 
    {
      if(bWeight1[jitr]>m_opval1 && bWeight2[jitr]>m_opval2)
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
      if(bWeight1[jitr]>m_opval1 && bWeight2[jitr]>m_opval2)
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
    else if(bWeight1[jitr]>m_opval1 && bWeight2[jitr]>m_opval2) 
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
//  std::pair<std::vector<float>, std::vector<float> >* weights_pair = new std::pair<std::vector<float>, std::vector<float> >(EventWeight, jetWeightVector);
  
  return new std::pair< std::vector<float>, std::vector<float> >(EventWeight, jetWeightVector); // weights_pair;
}

