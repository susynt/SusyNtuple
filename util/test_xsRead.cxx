#include "SusyNtuple/SleptonXsecReader.h"

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

void printHelp(const char *exeName)
{
  cout<<"Usage :"<<endl
      <<exeName<<" -i inputFile.root"<<endl
      <<"\t -t treeName"<<endl
      <<endl
      <<"See https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSignalGridDirectSlepton"<<endl
      <<endl;
}

bool isFloatEqual(const float &lhs, const float &rhs, float fractionalAccuracy=1.e-5)
{
  if(rhs) return (std::abs(lhs-rhs)/rhs)<fractionalAccuracy;
  else return std::abs(lhs-rhs)<fractionalAccuracy;
}

int main(int argc, char **argv)
{

  // get this file from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/SUSYSignalGridDirectSlepton
  string defaultInputFile = "/tmp/SignalUncertainties-All.root";
  string defaultTreeName = "SignalUncertainties";
  string inputFilename = defaultInputFile;
  string inputTreename = defaultTreeName;
  int optind(1);
  while ((optind < argc)) {
    if(argv[optind][0]!='-'){optind++; continue;}
    string sw = argv[optind];
    if     (sw == "-h"){ printHelp(argv[0]); return 0; }
    else if(sw == "-i"){ optind++; inputFilename = argv[optind]; }
    else if(sw == "-t"){ optind++; inputTreename = argv[optind]; }
    else if(argv[optind][0]=='-') cout<<"Unknown switch "<<argv[optind]<<endl;
    optind++;
  } // end if(optind<argc)

  cout<<endl<<" --- test point comparison ---"<<endl;
  SleptonPoint p1(1,1,SleptonPoint::kLeftHanded);
  SleptonPoint p2(1,1,SleptonPoint::kLeftHanded);
  SleptonPoint p3(1,1,SleptonPoint::kRightHanded);
  cout<<"p1 : "<<p1<<endl
      <<"p2 : "<<p2<<endl
      <<"p3 : "<<p3<<endl;
  cout<<"p1==p2 : equality      " <<(p1==p2 ? "PASS" : "FAIL")<<endl;
  cout<<"p1!=p3 : inequality    " <<(p1!=p3 ? "PASS" : "FAIL")<<endl;

  cout<<endl<<" --- test reader           ---"<<endl;
  SleptonXsecReader reader(inputFilename.c_str(), inputTreename.c_str(),
			   SleptonXsecReader::kAverageMultipleEntries, // see SleptonXsecReader.h for possible values
			   true);
  //reader.printKnownPoints(); // --lots of lines-- 
  SleptonPoint p4(1, 2, SleptonPoint::kLeftHanded);
  SleptonPoint p5(150, 30, SleptonPoint::kRightHanded);
  float xsP4(reader.getCrossSection(p4)), expXsP4(0.);
  float xsP5(reader.getCrossSection(p5)), expXsP5(0.00925922);
  float xsErrP4(reader.getCrossSectionError(p4)), expXsErrP4(0.);
  float xsErrP5(reader.getCrossSectionError(p5)), expXsErrP5(0.0595336);
  cout<<"xsec for "<<p4<<" : "<<xsP4<<" \t "<<(isFloatEqual(xsP4, expXsP4) ? "PASS":"FAIL")<<endl;
  cout<<"xsec for "<<p5<<" : "<<xsP5<<" \t "<<(isFloatEqual(xsP5, expXsP5) ? "PASS":"FAIL")<<endl;
  cout<<"xsecErr for "<<p4<<" : "<<xsErrP4<<" \t "<<(isFloatEqual(xsErrP4, expXsErrP4) ? "PASS":"FAIL")<<endl;
  cout<<"xsecErr for "<<p5<<" : "<<xsErrP5<<" \t "<<(isFloatEqual(xsErrP5, expXsErrP5) ? "PASS":"FAIL")<<endl;


  cout<<endl<<" --- test parsing         ---"<<endl;
  int dsid(0), msl(0), m1(0);
  bool verbose(true);
  string dsetName("mc12_8TeV.166523.Herwigpp_pMSSM_DLiSlep_MSL_110_M1_040.merge.NTUP_SUSY.e1455_a159_a171_r3549_p1328/");
  int expDsid(166523), expMsl(110), expM1(40);
  cout<<"input string: "<<dsetName<<endl;
  SleptonXsecReader::extractDsetidMslM1FromString(dsetName.c_str(), dsid, msl, m1, verbose);
  cout<<"extracted parameters: dsid="<<dsid<<", msl="<<msl<<", m1="<<m1
      <<" \t "<<((dsid==expDsid && msl==expMsl && m1==expM1)? "PASS":"FAIL")
      <<endl;

  cout<<endl<<" --- test value from dsid ---"<<endl;
  // test value from dsid
  float xs166567 = reader.getCrossSection(166567, SleptonPoint::kUnknown);
  float expXs166567 = 0.; // because not R/L defined
  float xsErr166567 = reader.getCrossSectionError(166567, SleptonPoint::kUnknown);
  float expXsErr166567 = 0.;
  float xs175507 = reader.getCrossSection(175507, SleptonPoint::kLeftHanded);
  float expXs175507 = 0.00331967;
  float xsErr175507 = reader.getCrossSectionError(175507, SleptonPoint::kLeftHanded);
  float expXsErr175507 = 0.0711965;
  cout<<"xsec for 166567 : "<<xs166567<<" \t "<<(isFloatEqual(xs166567, expXs166567) ? "PASS":"FAIL")<<endl;
  cout<<"xsec for 175507 : "<<xs175507<<" \t "<<(isFloatEqual(xs175507, expXs175507) ? "PASS":"FAIL")<<endl;
  cout<<"xsecErr for 166567 : "<<xsErr166567<<" \t "<<(isFloatEqual(xsErr166567, expXsErr166567) ? "PASS":"FAIL")<<endl;
  cout<<"xsecErr for 175507 : "<<xsErr175507<<" \t "<<(isFloatEqual(xsErr175507, expXsErr175507) ? "PASS":"FAIL")<<endl;
  return 0;
}
