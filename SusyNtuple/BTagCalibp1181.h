#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

namespace BTagCalibp1181
{
   std::pair<vector<float>, vector<float> > BTagCalibrationFunction(const vector<float>& pt, const vector<float>& eta,
                                                            const vector<float>& bWeight, const vector<int>& pdgid, 
                                                            std::string taggerName = "SV0",
                                                            std::string OP = "5_85", float opval = 5.85,
                                                            std::string calibration = "SusyNtuple/data/BTagCalibration.env",
                                                            std::string calibfolder = "SusyNtuple/data/");
}

