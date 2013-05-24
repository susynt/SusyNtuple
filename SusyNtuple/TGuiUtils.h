#ifndef TGUIUTILS_H
#define TGUIUTILS_H

/*! \file TGuiUtils Declares the ROOT settings.
 * \author Anyes Taffard, Kevin Slagle
 * \version 1.0
 */

#include "TObject.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TH2F.h"
#include "TH3F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TVirtualPad.h"

#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"


/*! \class TGuiUtils
 This class contains the functionalities to format the ROOT canvas 
 and plots accroding to the ATLAS standard
 */
class TGuiUtils : public TObject
{
 public: 
  TGuiUtils();
  virtual ~TGuiUtils(){};
  
 public:
  static TGuiUtils *rootUtils;
  TStyle *atlasStyle;
  
  void ATLASLabel(Double_t x,Double_t y,char* text=NULL,Color_t color=1); 

  void ATLASLabelOld(Double_t x,Double_t y,bool Preliminary=false,Color_t color=1); 

  void ATLASVersion(char* version=NULL,Double_t x=0.88,Double_t y=0.975,Color_t color=1);   
  


  TGraphErrors* myTGraphErrorsDivide(TGraphErrors* g1,TGraphErrors* g2);
  TGraphAsymmErrors* myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2);
  TGraphAsymmErrors* myMakeBand(TGraphAsymmErrors* g0, TGraphAsymmErrors* g1,TGraphAsymmErrors* g2);
  
  TGraphErrors* TH1ToTGraph(TH1 *h1);

  TGraphAsymmErrors* TH1TOTGraphAsymErrors(TH1 *h1);
  
  TGraphAsymmErrors*  myRatioBand(TGraphAsymmErrors* _asymErrors);

  void myAddtoBand(TGraphAsymmErrors* g1, TGraphAsymmErrors* g2);
  

  
  void myText(Double_t x,Double_t y,Color_t color,const char *text, Double_t tsize=0.05);
  void myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,const char *text);
  
  void myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle,const char *text,Float_t msize=2.); 

  void myHatchBox(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
		  Option_t* option="NDC", Color_t color=-1);

  void myLine(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
	      Color_t color=kRed);
	      
  //Method for plotting

  TCanvas* myCanvas(const char* name, int w=500, int h=500, int nx=1, int ny=1);
  
  TH1F* myTH1F(const char* name, const char* title, int nbin, float nlow, float nhigh, 
	       const char* titleX, const char* titleY);

  TH1F* myTH1F(const char* name, const char* title, int nbin, const Double_t* nX, 
	       const char* titleX, const char* titleY);

  TProfile* myTProfile(const char* name, const char* title, int nbinX, float nlowX, float nhighX, 
		       float nlowY, float nhighY, 
		       const char* titleX, const char* titleY);
  TH2F* myTH2F(const char* name, const char* title, 
	       int nbinX, float nlowX, float nhighX, 
	       int nbinY, float nlowY, float nhighY, 
	       const char* titleX, const char* titleY);
  TH2F* myTH2F(const char* name, const char* title, 
	       int nbinX, const Double_t* nX,  
	       int nbinY, float nlowY, float nhighY, 
	       const char* titleX, const char* titleY);
  TH2F* myTH2F(const char* name, const char* title, 
	       int nbinX, const Double_t* nX,  
	       int nbinY, const Double_t* nY,  
	       const char* titleX, const char* titleY);

  TH3F* myTH3F(const char* name, const char* title, 
	       int nbinX, float nlowX, float nhighX, 
	       int nbinY, float nlowY, float nhighY, 
	       int nbinZ, float nlowZ, float nhighZ, 
	       const char* titleX, const char* titleY, const char* titleZ);
  
  TH3F* myTH3F(const char* name, const char* title, 
	       int nbinX, const Double_t* nX,  
	       int nbinY, const Double_t* nY,  
	       int nbinZ, const Double_t* nz,  
	       const char* titleX, const char* titleY, const char* titleZ);

  void myDraw1d(TH1* hist, TCanvas* c, int npad=1, Option_t* option="e", 
		bool logy=false, Color_t color=-1, bool showStat=true, int markerStyle=20);
  void myDraw1d(TH1* hist, TVirtualPad* c, int npad=1, Option_t* option="e", 
		bool logy=false, Color_t color=-1, bool showStat=true, int markerStyle=20);
  void myDraw2d(TH2* hist, TCanvas* c, int npad=1, Option_t* option="e", 
		Color_t color=-1, bool showStat=false);
  void myDraw2d(TH2* hist, TVirtualPad* p, int npad=1, Option_t* option="e", 
		Color_t color=-1, bool showStat=false);

  TVirtualPad* myDrawRatio(TCanvas* _c,TPad* _pTop, TPad* _pBot, 
			   THStack* _hStack, TH1F* _stackH, TH1F* _h, TLegend* _l,bool logy=false);

  void myDrawTHStack(TCanvas* _c, THStack* _hStack, TH1F* _stackH, 
		     TLegend* _l,bool logy=false);

  void addToTHStack(THStack* _hStack, TH1F* _h, int icol, const char* opt, 
		    TLegend* _l, const char* _txt);

  void getStackHisto(THStack* _hStack, TH1F* _stackH);

  void moveUnderOverFlow(TH1* h, int opt=0); //0 both under/over, 1: under 2:over

  double getMax(TH1* h1, TH1* h2);
  double getMin(TH1* h1, TH1* h2);
  double getMinBin(TH1* h);
  void legendSetting(TLegend* leg, float fontSize=0.02);


  double getHistMean(TH1* h);//under-over flow excluded

  /** Events / X [Unit]. Assume constant bin size */
  void yAxis(TH1* h1, const char* _unit);

  void normalize(TH1* h1, float val=1);

  /** Template dummy histo */ 
  void templateHisto();

  
  /**  Calculate poisson error on data using Neyman Procedure
       http://www.pp.rhul.ac.uk/~cowan/atlas/ErrorBars.pdf
       http://pdg.web.cern.ch/pdg/2011/reviews/rpp2011-rev-statistics.pdf     
   **/
  TGraphAsymmErrors* updateDataError(TH1* h, bool fillZero=false);



  ClassDef(TGuiUtils,1);


 private:
  void createAtlasStyle();
  
  
};

#endif // TGUIUTILS_H
