#ifndef BTAGCALIB2013_H
#define BTAGCALIB2013_H

#include <iostream>
#include <vector>
#include <string>
#include <math.h>

namespace Analysis{
  class CalibrationDataInterfaceROOT;
}

class BTagCalib2013
{
  public:
   BTagCalib2013( const std::string& , const std::string& , const std::string& , const std::string& , bool, double opval1=0.7892 , double opval2=-1);

   std::pair<std::vector<float>, std::vector<float> >* BTagCalibrationFunction(const std::vector<float>& pt,
									       const std::vector<float>& eta,
									       const std::vector<float>& bWeight,
									       const std::vector<int>& pdgid,
									       bool isSherpa = false);
   
   std::pair<std::vector<float>, std::vector<float> >* BTagCalibrationFunction(const std::vector<float>& pt,
									       const std::vector<float>& eta,
									       const std::vector<float>& bWeight1, 
									       const std::vector<float>& bWeight2,
									       const std::vector<int>& pdgid,
									       bool isSherpa = false);

 private:

  Analysis::CalibrationDataInterfaceROOT* m_BTagCalib;

  unsigned int m_sfIndex_C , m_sfIndex_B , m_sfIndex_T , m_sfIndex_L ;
  unsigned int m_insfIndex_C , m_insfIndex_B , m_insfIndex_T , m_insfIndex_L ;
  std::string m_jetAuthor;
  double m_opval1, m_opval2;
};

#endif
