#include "SusyNtuple/SusyNtTools.h"

using namespace std;
using namespace Susy;


/*--------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------*/
SusyNtTools::SusyNtTools() :
        m_anaType(Ana_3Lep)
{}

/*--------------------------------------------------------------------------------*/
// 'Overview' functions to make it easy for user to grab all objects
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getBaselineObjects(SusyNtObject* susy_nt, ElectronVector &elecs,
				     MuonVector &muons, JetVector &jets, SusyNtSys sys){

  // Get all objects
  elecs = getPreElectrons(susy_nt, sys);
  muons = getPreMuons(susy_nt, sys);
  jets  = getPreJets(susy_nt, sys);

  // Now perform the overlap removals
  performOverlap(elecs, muons, jets);

  // Do SFOS removal for Mll < 20 
  if(m_anaType == Ana_3Lep)
  {
    RemoveSFOSPair(elecs, 20);
    RemoveSFOSPair(muons, 20);
  }
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(ElectronVector bsElecs, MuonVector bsMuons, JetVector bsJets,
				   ElectronVector &sigElecs, MuonVector &sigMuons, JetVector &sigJets){

  // Set signal objects
  sigElecs = getSignalElectrons(bsElecs);
  sigMuons = getSignalMuons(bsMuons);
  sigJets  = getSignalJets(bsJets);

}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::getSignalObjects(SusyNtObject* susy_nt, ElectronVector &sigElecs, 
				   MuonVector &sigMuons, JetVector &sigJets, SusyNtSys sys){

  // Temprorary vectors for baseline objects
  ElectronVector elecs = getPreElectrons(susy_nt, sys);
  MuonVector     muons = getPreMuons(susy_nt, sys);
  JetVector       jets = getPreJets(susy_nt, sys);

  // Perform overlap removal
  performOverlap(elecs, muons, jets);

  // Now set the signal objects
  sigElecs = getSignalElectrons(elecs);
  sigMuons = getSignalMuons(muons);
  sigJets  = getSignalJets(jets);

}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::performOverlap(ElectronVector &elecs, MuonVector &muons, JetVector &jets){

  // Remove electrons from electrons
  e_e_overlap(elecs, E_E_DR);

  // Remove jets from electrons
  e_j_overlap(elecs, jets, J_E_DR, true);
  
  // Remove electrons from jets
  e_j_overlap(elecs, jets, E_J_DR, false);

  // Remove muons from jets
  m_j_overlap(muons, jets, M_J_DR);

  // Remove electrons and muons that overlap
  e_m_overlap(elecs, muons, E_M_DR);

}

/*--------------------------------------------------------------------------------*/
// Methods to grab the Baseline objects
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getPreElectrons(SusyNtObject* susy_nt, SusyNtSys sys){

  // Not sure if I want to pass SusyNt object around or not... but just do it this way
  // for now for lack of a more creative idea.
  ElectronVector elecs;
  for(uint ie=0; ie<susy_nt->ele()->size(); ++ie){
    Electron* e = &(susy_nt->ele()->at(ie));
    //cout<<" electron "<<ie<<endl;
    //e->print();
    e->setState( sys );
    //cout<<"After sys: "<<endl;
    //e->print();
    
    // Apply any additional Selection
    if( e->Pt() < ELECTRON_PT_CUT ) continue;

    // Save
    elecs.push_back( e );
  }

  return elecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getPreMuons(SusyNtObject* susy_nt, SusyNtSys sys){

  MuonVector muons;
  for(uint im=0; im<susy_nt->muo()->size(); ++im){
    Muon* m = &(susy_nt->muo()->at(im));
    //cout<<" muon "<<im<<endl;
    //m->print();
    m->setState( sys );
    //cout<<"\tAfter sys:"<<endl;
    //m->print();

    // Apply any additional selection
    if( m->Pt() < MUON_PT_CUT ) continue;
    
    // Save
    muons.push_back( m );
  }

  return muons;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getPreJets(SusyNtObject* susy_nt, SusyNtSys sys){

  JetVector jets;
  for(uint ij=0; ij<susy_nt->jet()->size(); ++ij){
    Jet* j = &(susy_nt->jet()->at(ij));
    //cout<<"Before "<<ij<<endl;
    //j->print();
    j->setState( sys );
    //cout<<"After"<<endl;
    //j->print();

    // Apply any additional selection
    // At least jets of 20 GeV
    if( j->Pt() < JET_PT_CUT_3L )        continue;

    // Save
    jets.push_back( j );
  }
  
  return jets;

}

/*--------------------------------------------------------------------------------*/
// Get Signal objects
/*--------------------------------------------------------------------------------*/
ElectronVector SusyNtTools::getSignalElectrons(ElectronVector bsElecs){

  ElectronVector sigElecs;
  for(uint ie=0; ie<bsElecs.size(); ++ie){
    const Electron* e = bsElecs.at(ie);
    if( isSignalElectron( e ) )
      sigElecs.push_back( e );
  }// end loop over baseline electrons

  return sigElecs;
}
/*--------------------------------------------------------------------------------*/
MuonVector SusyNtTools::getSignalMuons(MuonVector bsMuons){
  
  MuonVector sigMuons;
  for(uint im=0; im<bsMuons.size(); ++im){
    const Muon* m = bsMuons.at(im);
    if( isSignalMuon( m ) )
      sigMuons.push_back( m );
  }// end loop over baseline muons

  return sigMuons;
}
/*--------------------------------------------------------------------------------*/
JetVector SusyNtTools::getSignalJets(JetVector bsJets){

  JetVector sigJets;
  for(uint ij=0; ij<bsJets.size(); ++ij){
    const Jet* j = bsJets.at(ij);
    if( isSignalJet( j ) )
      sigJets.push_back( j );
  }// end loop over baseline jets

  return sigJets;
}
/*--------------------------------------------------------------------------------*/
PhotonVector SusyNtTools::getSignalPhotons(SusyNtObject* susy_nt)
{
  // Currently only storing signal photons, so just a conv way to get them.

  PhotonVector sigPho;
  for(uint ip=0; ip<susy_nt->pho()->size(); ++ip)
    sigPho.push_back( &(susy_nt->pho()->at(ip)) );

  return sigPho;  
}
/*--------------------------------------------------------------------------------*/
// Get Met
/*--------------------------------------------------------------------------------*/
Susy::Met* SusyNtTools::getMet(SusyNtObject* susy_nt, SusyNtSys sys){

  // Right now now being clever. Could maybe make sys index correspond to 
  // index on the array.
  Met* met = NULL;
  vector<Met>* metTmp = susy_nt->met();
  for(uint i=0; i<metTmp->size(); ++i){
    if(metTmp->at(i).sys == sys){
      met = &(metTmp->at(i));
      return met;
    }
  }
  if( !met ) 
    cout<<"Error: Unable to find met for given systematic!\nReturning NULL!!"<<endl;
  
  return met;
}

/*--------------------------------------------------------------------------------*/
// Check if Lepton is Signal Lepton
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalElectron(const Electron* ele){
  
  if(!ele->tightPP)                                       return false;
  if(ele->ptcone20/ele->Pt() > ELECTRON_PTCONE20_ET_CUT ) return false;
  if( m_anaType == Ana_3Lep ) 
    if(fabs(ele->d0)/ele->errD0 > ELECTRON_D0SIG_CUT)             return false;
  return true;

}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalMuon(const Muon* mu){

  if(mu->ptcone20 > MUON_PTCONE20_CUT)        return false;
  if( m_anaType == Ana_3Lep )
    if(fabs(mu->d0)/mu->errD0 > MUON_D0SIG_CUT) return false;
  return true;
  
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::isSignalJet(const Jet* jet){

  // Two lep and three lep have different requirements
  const float JET_PT_CUT = (m_anaType == Ana_2Lep) ? 
    JET_PT_CUT_2L :
    JET_PT_CUT_3L;
  
  if( jet->Pt() < JET_PT_CUT )        return false;
  if( fabs(jet->Eta()) > JET_ETA_CUT) return false;
  if( jet->jvf < 0.75)                return false;
  return true;

}

/*--------------------------------------------------------------------------------*/
// Overlap Methods (Commandeered from Anyes)
/*--------------------------------------------------------------------------------*/
void SusyNtTools::e_e_overlap(ElectronVector &elecs, float mindR){

  if( elecs.size() < 2 ) return;

  for(int ie1=elecs.size()-1; ie1>=0; ie1--){
    const Electron* e1 = elecs.at(ie1);
    for(int ie2=ie1-1; ie2>=0; ie2--){
      const Electron* e2 = elecs.at(ie2);
      if( e1->DeltaR( *e2 ) > mindR ) continue;
      
      if( e1->Pt() < e2->Pt() ){
	elecs.erase( elecs.begin() + ie1 );
	break;
      }
      else
	elecs.erase( elecs.begin() + ie2 );
    }
  }
  
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::e_j_overlap(ElectronVector &elecs, JetVector &jets, float mindR, bool removeJets){

  if( elecs.size() == 0 || jets.size() == 0 ) return;

  for(int ie=elecs.size()-1; ie>=0; ie--){
    const Electron* e = elecs.at(ie);
    for(int ij=jets.size()-1; ij>=0; ij--){
      const Jet* j = jets.at(ij);
      if( e->DeltaR( *j ) > mindR ) continue;
      
      if( removeJets )
	jets.erase( jets.begin() + ij );
      else{
	elecs.erase( elecs.begin() + ie);
	break;
      }
    }// end loop over jets
  }// end loop over electrons	  

}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::m_j_overlap(MuonVector &muons, JetVector jets, float mindR){

  if( muons.size() == 0 || jets.size() == 0 ) return;

  for(int im=muons.size()-1; im>=0; im--){
    const Muon* m = muons.at(im);
    for(int ij=jets.size()-1; ij>=0; ij--){
      const Jet* j = jets.at(ij);
      if( m->DeltaR( *j ) > mindR ) continue;

      muons.erase( muons.begin() + im );
      break;

    }// end loop over jets
  }// end loop over muons
  
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::e_m_overlap(ElectronVector &elecs, MuonVector &muons, float mindR){

  if( elecs.size() == 0 || muons.size() == 0 ) return;

  //In this case we will want to remove both the electron and the muon
  for(int ie=elecs.size()-1; ie>=0; ie--){
    const Electron* e = elecs.at(ie);
    for(int im=muons.size()-1; im>=0; im--){
      const Muon* m = muons.at(im);
      if( e->DeltaR( *m ) > mindR ) continue;
      
      elecs.erase( elecs.begin() + ie );
      muons.erase( muons.begin() + im );
      break;

    }// end loop over muons
  }// end loop over jets

}
/*--------------------------------------------------------------------------------*/
// Methods to handle signal region cuts
/*--------------------------------------------------------------------------------*/
void SusyNtTools::RemoveSFOSPair(ElectronVector &elecs, float Mll){

  if( elecs.size() < 2 ) return;

  for(int ie1=elecs.size()-1; ie1>=0; ie1--){
    const Electron* e1 = elecs.at(ie1);
    for(int ie2=ie1-1; ie2>=0; ie2--){
      const Electron* e2 = elecs.at(ie2);
      if( !(e1->q * e2->q < 0 && (*e1 + *e2).M() < Mll) ) continue;
      elecs.erase( elecs.begin() + ie1 );
      ie1--; // removing another guy
      elecs.erase( elecs.begin() + ie2 );
      break;
    }
  }
  
}
/*--------------------------------------------------------------------------------*/
void SusyNtTools::RemoveSFOSPair(MuonVector &muons, float Mll){

  if( muons.size() < 2 ) return;

  for(int im1=muons.size()-1; im1>=0; im1--){
    const Muon* m1 = muons.at(im1);
    for(int im2=im1-1; im2>=0; im2--){
      const Muon* m2 = muons.at(im2);
      if( !(m1->q * m2->q < 0 && (*m1 + *m2).M() < Mll) ) continue;
      muons.erase( muons.begin() + im1 );
      im1--; // removing another guy
      muons.erase( muons.begin() + im2 );
      break;
    }
  }
  
}

/*--------------------------------------------------------------------------------*/
// Methods to grab useful quantities
/*--------------------------------------------------------------------------------*/
float SusyNtTools::getMetRel(const Met* met, const LeptonVector& leptons, const JetVector& jets){

  const TLorentzVector metLV = met->lv();
  float dPhi = TMath::Pi()/2.;
  
  for(uint il=0; il<leptons.size(); ++il)
    if( fabs(metLV.DeltaPhi( *leptons.at(il) )) < dPhi )
      dPhi = fabs(metLV.DeltaPhi( *leptons.at(il) ));
  for(uint ij=0; ij<jets.size(); ++ij){
    const Jet* jet = jets.at(ij);
    if( fabs(metLV.DeltaPhi( *jet )) < dPhi )
      dPhi = fabs(metLV.DeltaPhi( *jet ));    
  }// end loop over jets
  
  return metLV.Et() * sin(dPhi);
}
/*--------------------------------------------------------------------------------*/
// Top Tagging methods borrowed from SUSYTools. Yes this is duplication of code...
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::passTopTag(const LeptonVector& leptons, const JetVector& jets, const Met* met,
			     int opt, float ptJetCut, float mEffCut){

  // Defaults opt = 0, ptJetCut = 30 GeV, mEffCut = 100 GeV
  // Tagging is based on number of jets and at least two leptons
  if( leptons.size() < 2 ) return false;
  
  // Define some necessary vars
  uint njets         = jets.size();
  uint njetscan      = njets < 3 ? njets : 3; // Only consider up to 3 jets 
  TLorentzVector v1l = *leptons.at(0);
  TLorentzVector v2l = *leptons.at(1);
  double phi         = met->phi;
  double Et          = met->Et;
  TVector2 metv2 (Et * cos(phi), Et * sin(phi));

  // Check to see if our leptons and met is
  // consistent with a W candidate
  bool maybeW = toptag0j(v1l,v2l,metv2);
  if(!maybeW)            return true;
  if( jets.size() < 2 )  return true;
  
  // Check to see if we have top
  int npairs    = 0;
  for(uint i1=0; i1<njetscan-1; ++i1){
    if(i1==0 || opt==0){ // opt=0 implies compare all jets
      for(uint i2=i1+1; i2<njetscan; ++i2){
	TLorentzVector v1j = *jets.at(i1);
	TLorentzVector v2j = *jets.at(i2);
	bool maybeTop = toptag2j(ptJetCut,mEffCut,v1l,v2l,v1j,v2j,metv2,opt);
	if(maybeTop) npairs++;
      }// end loop over second jet
    }// end if options
  }// end loop over first jet

  if( npairs > 0 ) return false;
  return true;

}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::toptag0j(TLorentzVector v1l, TLorentzVector v2l, TVector2 met){

  // Taken from TopTag.cxx in SUSYTools

  static const Double_t wmass = 80.4;
  double metx = met.X();
  double mety = met.Y();

  // First mct(ll)
  //---------------
  double mctll=calcMCT(v1l,v2l);

  double pxus=v1l.Px()+v2l.Px()+metx;
  double pyus=v1l.Py()+v2l.Py()+mety;
  double rr=sqrt(pxus*pxus+pyus*pyus)/(2.*wmass);
  double fact=rr+sqrt(1+rr*rr);

  //cout << "mctll " << mctll << " cut value " << wmass*fact << std::endl;
  return mctll < wmass*fact ? true: false;
}
/*--------------------------------------------------------------------------------*/
bool SusyNtTools::toptag2j(double ptjcut, double meffcut, 
			   TLorentzVector v1l, TLorentzVector v2l,
			   TLorentzVector v1j, TLorentzVector v2j,
			   TVector2 met, int iopt ) {

  // Taken from TopTag.cxx in SUSYTools

  //
  double tmass = 172.5;
  double mljcut = 155.;
  double mctjcut = 137.;
  //
  TLorentzVector vjb[2];
  double ml1j[2];
  double ml2j[2];
  //
  double metx = met.X();  double mety = met.Y();
  double ptl1=v1l.Pt();   double ptl2=v2l.Pt();
  double ptj1=v1j.Pt();   double ptj2=v2j.Pt();
  double pxus=v1l.Px()+v2l.Px()+metx;
  double pyus=v1l.Py()+v2l.Py()+mety;
  double mefftop=ptj1+ptj2+ptl1+ptl2;

  if(ptj2 < ptjcut) return false;
  if(iopt == 0 && mefftop < meffcut) return false;

  // First part:
  //-------------
  vjb[0]=v1j;
  vjb[1]=v2j;
  double mctjj=calcMCT(vjb[0],vjb[1]);
  double pxusjl=vjb[0].Px()+vjb[1].Px()+pxus;
  double pyusjl=vjb[0].Py()+vjb[1].Py()+pyus;
  double rrj=sqrt(pxusjl*pxusjl+pyusjl*pyusjl)/(2.*tmass);
  double factj=rrj+sqrt(1+rrj*rrj);

  bool imct = mctjj < mctjcut*factj ? true : false;
  if(iopt==1) return imct;

  // Second part:
  //--------------
  for(int i=0;i<2;++i) {
    ml1j[i]=(v1l+vjb[i]).M();
    ml2j[i]=(v2l+vjb[i]).M();
  }
  int ncou=0;
  int icou1[2];
  int icou2[2];
  for(int i=0;i<2;++i) {
    for(int j=0;j<2;++j) {
      if(i != j) {
	if(ml1j[i]<mljcut && ml2j[j]<mljcut) {
	  icou1[ncou]=i;
	  icou2[ncou]=j;
	  ncou++;
	}
      }
    }
  }
  bool imjl = ncou>0 ? true : false;

  // Third part: finally mct(ql,ql) for each coupling passing the mjl cut
  //-------------exploit the dependence of mct((jl),(jl)) on the maximum
  //             mass^2 of the two jl combinations 
  int ngcou=0;
  for(int i=0;i<ncou;++i) {
    int ij1=icou1[i];
    int ij2=icou2[i];
    TLorentzVector vtot1=vjb[ij1]+v1l;
    TLorentzVector vtot2=vjb[ij2]+v2l;
    double mctjl=calcMCT(vtot1,vtot2);
    double mm1=(vjb[ij1]+v1l).M();
    double mm2=(vjb[ij2]+v2l).M();
    double mmax2 = mm1>mm2 ? (mm1*mm1) : (mm2*mm2);
    double upl=mmax2/(tmass)+(tmass);
    //cout << " i " << " mctjl " << mctjl << " mmax2 " << mmax2 << 
    //" upl " << upl << endl;
    if(mctjl < upl*factj )  ngcou++;
  }

  bool imctjl = ngcou>0 ? true : false;
  //cout << " ll " << imctll << " ct " << imct << " jl " << imjl <<
  //" ctlj " << imctjl << endl;

  return imct & imjl & imctjl;
}
/*--------------------------------------------------------------------------------*/
float SusyNtTools::calcMCT(TLorentzVector v1, TLorentzVector v2){
  float mct = (v1.Mt() + v2.Mt())*(v1.Mt() + v2.Mt()) - (v1-v2).Perp2();
  mct = (mct >= 0.) ? sqrt(mct) : sqrt(-mct);
  return mct;
}

