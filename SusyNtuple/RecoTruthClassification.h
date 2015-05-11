#ifndef RecoTruthClassification_h
#define RecoTruthClassification_h

//------------------------------------------
// Classify reco lepton based on truth 
//
// author: Anyes Taffard 
//------------------------------------------

 enum type{
   PROMPT,
   CONV,
   HF,
   LF,
   ELEC,
   MUON,
   QJET,
   GJET,
   UK
 };

///** @brief Checks if is a prompt lepton from W, Z, tau. */
bool isRealLepton(int Origin, int Type, int mcMatchPdgId, bool isTau = false);

///** @brief Checks if it is not prompt and has origin 5,6 or 7: Conversion, Dalitz decay or Electromagnetic process. */
bool isConvLepton(int Origin, int Type, int mcMatchPdgId, bool isTau = false);

///** @brief Checks if it is not prompt. */
bool isFakeLepton(int Origin, int Type, int mcMatchPdgId, bool isTau = false);

///** @brief Checks if it is not prompt and has origin heavy flavour. */
bool isHFLepton(int Origin, int Type, int mcMatchPdgId, bool isTau = false);

///** @brief Checks if it is not prompt and has not origin heavy flavour or conversion. */
bool isLFLepton(int Origin, int Type, int mcMatchPdgId, bool isTau = false);

///** @brief Checks if it is not prompt and not conversion. */
bool isJFLepton(int Origin, int Type, int mcMatchPdgId, bool isTau = false);

 ///** @brief Checks if it is not prompt and has unknown type and origin. */
bool isUKLepton(int Origin, int Type, int mcMatchPdgId, bool isTau = false);

///** @brief Returns the type of fake: 0=prompt 1=conversion 2=HF 3=LF 4=unknown. */
int fakeType(int Origin, int Type, int mcMatchPdgId, bool isTau = false);

///** @brief Check if the electron is prompt */
bool isPromptElectron(int type, int origin);


#endif
