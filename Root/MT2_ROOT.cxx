#include "SusyNtuple/MT2_ROOT.h"

ComputeMT2::ComputeMT2(TLorentzVector visa, TLorentzVector visb, TLorentzVector MET, double ma, double mb) {
  
  Init(visa, visb, MET, ma, mb);

}

ComputeMT2::ComputeMT2(TLorentzVector visa, TLorentzVector visb, TLorentzVector MET, TString name1, TString name2){
  if (particletype(name1) >= 0 && particletype(name2) >= 0){
    Init(visa, visb, MET, particletype(name1), particletype(name2));
  }
  else{
    std::cout << "MT2 Error: the particle names you give are not on the list (or have the wrong syntax)!" << std::endl;
    std::cout << "Choices are: " << std::endl;
    listparticles();
    exit(1);
  }
}    

void ComputeMT2::Init(TLorentzVector visa, TLorentzVector visb, TLorentzVector MET, double ma, double mb) {
  
  if (V1.M() < 0 || V2.M() < 0){
    std::cout << "MT2 Error: Your input particles have negative mass" << std::endl;
    exit(1);
  }

  V1 = visa;
  V2 = visb;
  ET = MET;
  Ma = ma;
  Mb = mb;
  myMT2 = -1;
  sol1 = -1;
  sol2 = -1;

}

ComputeMT2::~ComputeMT2() {
}

double ComputeMT2::Compute(bool useActualVisMass, double visaMass, double visbMass)  {

    if(useActualVisMass)
      myMT2 = asymm_mt2_lester_bisect::get_mT2(V1.M(),V1.Px(),V1.Py(),V2.M(),V2.Px(),V2.Py(),ET.Px(),ET.Py(),Ma,Mb);
    else 
      myMT2 = asymm_mt2_lester_bisect::get_mT2(visaMass,V1.Px(),V1.Py(),visbMass,V2.Px(),V2.Py(),ET.Px(),ET.Py(),Ma,Mb);
    sol1 = 0;
    sol2 = 0;
    return myMT2;

}

double ComputeMT2::particletype(TString part){

  if (part == "Wboson_PDG2014_GeV") return 80.385;
  if (part == "Neutrino") return 0;
  if (part == "Top_PDG2014_GeV") return 173.21;
  if (part == "Lepton") return 0.;
  return -1;

}

void ComputeMT2::listparticles(){
  std::cout << "Wboson_PDG2014_GeV, mass = 80.385 GeV" << std::endl;
  std::cout << "Neutrino, mass = 0 GeV" << std::endl;
  std::cout << "Top_PDG2014_GeV, mass = 173.21 GeV" << std::endl;
  std::cout << "Lepton, mass = 0 GeV" << std::endl;
}

std::pair <double,double> ComputeMT2::get_solutions(){
  if (myMT2 < 0){
    std::cout << "MT2 Error: You have not yet run MT2!" << std::endl;
    exit(1);   
  } 
  else{
    std::pair <double,double> out =  ben_findsols(myMT2, V1.Px(), V1.Py(), V1.M(), Ma, V2.Px(), V2.Py(), ET.Px(), ET.Py(), V2.M(),  Mb);
    return out;
  }
}
