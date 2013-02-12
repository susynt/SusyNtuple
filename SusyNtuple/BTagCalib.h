#ifndef BTAGCALIB_H
#define BTAGCALIB_H



#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

namespace BTagCalib
{
   std::pair<vector<float>, vector<float> > BTagCalibrationFunction(const vector<float>& pt,
								    const vector<float>& eta,
								    const vector<float>& bWeight,
								    const vector<int>& pdgid,
								    std::string taggerName = "MV1",
								    std::string OP = "0_7892",
								    float opval = 0.7892,
								    bool isJVF = false,
								    std::string calibration = "SUSYTools/data/BTagCalibration.env",
								    std::string calibfolder = "SUSYTools/data/");
   
   std::pair<vector<float>, vector<float> > BTagCalibrationFunction(const vector<float>& pt,
								    const vector<float>& eta,
								    const vector<float>& bWeight1, 
								    const vector<float>& bWeight2,
								    const vector<int>& pdgid, 
								    std::string taggerName = "MV1",
								    std::string OP = "0_7892",
								    float opval1 = 0.7892,
								    float opval2 = 0.7892,
								    bool isJVF = false,
								    std::string calibration = "SUSYTools/data/BTagCalibration.env",
								    std::string calibfolder = "SUSYTools/data/");
}

#endif
