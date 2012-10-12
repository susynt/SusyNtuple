#include "SusyNtuple/TGuiUtils.h"

#include <iostream>
#include <math.h>


#include "TH1.h" 
#include "TF1.h" 
#include "TPave.h" 
#include "TLine.h" 
#include "TMarker.h" 
#include "TLatex.h"
#include "TFile.h"
#include "TPaveStats.h"
#include "TMath.h"

using namespace std;

TGuiUtils *TGuiUtils::rootUtils = 0;

//__________________________________________________________________________________________________
TGuiUtils::TGuiUtils()
{ 
  createAtlasStyle();
}

//__________________________________________________________________________________________________
void TGuiUtils::createAtlasStyle(){

  atlasStyle= new TStyle("ATLAS","Atlas style");

  Int_t icol=0;
  Int_t font=42;
  Double_t tsize=0.05;
  Double_t tlabelsize=0.04; //ATLAS 0.05
  Int_t markerStyle=20;
  Double_t msize =1.2; //AT 1.0
  Int_t hlinewidth =2;

  // Canvas settings
  atlasStyle->SetFrameBorderMode(icol);
  atlasStyle->SetFrameFillColor(icol);
  atlasStyle->SetCanvasBorderMode(icol);
  atlasStyle->SetPadBorderMode(icol);
  atlasStyle->SetPadColor(icol);
  atlasStyle->SetCanvasColor(icol);
  atlasStyle->SetStatColor(icol);
  
  // set the paper & margin sizes
  atlasStyle->SetPaperSize(20,26);
  atlasStyle->SetPadTopMargin(0.15);  //0.05
  atlasStyle->SetPadRightMargin(0.15); //0.05
  atlasStyle->SetPadBottomMargin(0.15); //0.16
  atlasStyle->SetPadLeftMargin(0.15); //0.16
  
  // use large fonts
  atlasStyle->SetTextFont(font);
  atlasStyle->SetTextSize(tsize);

  atlasStyle->SetTitleFont(font,"x");
  atlasStyle->SetTitleFont(font,"y");
  atlasStyle->SetTitleFont(font,"z");
  atlasStyle->SetTitleOffset(1.4,"x"); //1.4 //AT 0.9
  atlasStyle->SetTitleOffset(1.4,"y"); //1.4 //AT 1.5
  atlasStyle->SetTitleOffset(1.,"z");
  atlasStyle->SetTitleSize(tsize,"x");
  atlasStyle->SetTitleSize(tsize,"y");
  atlasStyle->SetTitleSize(tsize,"z");
  atlasStyle->SetTickLength (0.02,"x");
  atlasStyle->SetTickLength (0.02,"y");
  atlasStyle->SetTickLength (0.02,"z");


  atlasStyle->SetLabelFont(font,"x");
  atlasStyle->SetLabelFont(font,"y");
  atlasStyle->SetLabelFont(font,"z");
  //  atlasStyle->SetLabelOffset( 0.01, "x");
  //  atlasStyle->SetLabelOffset( 0.02, "y");
  //  atlasStyle->SetLabelOffset( 0.01, "z");
  atlasStyle->SetLabelSize(tlabelsize,"x");
  atlasStyle->SetLabelSize(tlabelsize,"y");
  atlasStyle->SetLabelSize(tlabelsize,"z");

  //palette settings
  atlasStyle->SetPalette(1,0);

  //use bold lines and markers
  atlasStyle->SetMarkerStyle(markerStyle);
  atlasStyle->SetMarkerSize(msize);
  atlasStyle->SetHistLineWidth(hlinewidth);
  atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  atlasStyle->SetEndErrorSize(0.); //remove erro bar caps
  
  //do not display any of the standard histogram decorations
  atlasStyle->SetStatX(0.99);
  atlasStyle->SetStatY(0.99);
  atlasStyle->SetStatH(0.01);
  atlasStyle->SetStatW(0.2);

  atlasStyle->SetStatStyle(0);
  atlasStyle->SetStatFont(font);
  atlasStyle->SetStatFontSize(0.03);
  atlasStyle->SetOptStat("nemruo");
  atlasStyle->SetStatBorderSize(1);
  atlasStyle->SetOptTitle(0);
  atlasStyle->SetOptFit(0);

  atlasStyle->SetTitleStyle(icol);
  atlasStyle->SetTitleH(0.08);

  // put tick marks on top and RHS of plots
  atlasStyle->SetPadTickX(1);
  atlasStyle->SetPadTickY(1);

}

//__________________________________________________________________________________________________
void TGuiUtils::ATLASLabel(Double_t x,Double_t y,char* text,Color_t color) 
{
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);

  double delx = 0.115*696*gPad->GetWh()/(472*gPad->GetWw());

  l.DrawLatex(x,y,"ATLAS");
  if (text) {
    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.DrawLatex(x+delx,y,text);
    //    p.DrawLatex(x,y,"#sqrt{s}=900GeV");
  }
}
//__________________________________________________________________________________________________
void TGuiUtils::ATLASLabelOld(Double_t x,Double_t y,bool Preliminary,Color_t color) 
{
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);
  l.DrawLatex(x,y,"ATLAS");
  if (Preliminary) {
    TLatex p; 
    p.SetNDC();
    p.SetTextFont(42);
    p.SetTextColor(color);
    p.DrawLatex(x+0.115,y,"Preliminary");
  }
}
//__________________________________________________________________________________________________
void TGuiUtils::ATLASVersion(char* version,Double_t x,Double_t y,Color_t color) 
{

  if (version) {
    char versionString[100];
    sprintf(versionString,"Version %s",version);
    TLatex l;
    l.SetTextAlign(22); 
    l.SetTextSize(0.04); 
    l.SetNDC();
    l.SetTextFont(72);
    l.SetTextColor(color);
    l.DrawLatex(x,y,versionString);
  }
}


//__________________________________________________________________________________________________
TGraphErrors* TGuiUtils::myTGraphErrorsDivide(TGraphErrors* g1,TGraphErrors* g2)
{
  TGraphErrors* g3= new TGraphErrors();

  if (!g1) {
    printf("**myTGraphErrorsDivide: g1 does not exist !  \n"); 
    return g3;
  }
  if (!g2) {
    printf("**myTGraphErrorsDivide: g2 does not exist !  \n"); 
    return g3;
  }

  if (g1->GetN()!=g2->GetN()) {
   printf("**myTGraphErrorsDivide: vector do not have same number of entries !  \n"); 
  }

  Int_t iv=0;
  for (Int_t i1=0; i1<g1->GetN(); i1++) {
   for (Int_t i2=0; i2<g2->GetN(); i2++) {
     Double_t  x1=0., y1=0., x2=0., y2=0.;
     Double_t dx1=0.,dy1=0.,       dy2=0.;
     Double_t e=0.;
     
     g1->GetPoint(i1,x1,y1);
     g2->GetPoint(i2,x2,y2);
     if(x1==x2){
       dx1  = g1->GetErrorX(i1);
       if (y1!=0) dy1  = g1->GetErrorY(i1)/y1;
       if (y2!=0) dy2  = g2->GetErrorY(i2)/y2;
       
       if (y2!=0.) g3->SetPoint(iv, x1,y1/y2);
       else        g3->SetPoint(iv, x1,y2);
       
       if (y1!=0 && y2!=0) e=sqrt(dy1*dy1+dy2*dy2)*(y1/y2); 
       g3->SetPointError(iv,dx1,e);

       iv++;
     }
     else{
       printf("**myTGraphErrorsDivide: %d x1!=x2  %f %f  !  \n",iv,x1,x2);
     }
   }
  }
  return g3;


}


//__________________________________________________________________________________________________
TGraphAsymmErrors* TGuiUtils::myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2)
{
 const Int_t debug=0; 

  TGraphAsymmErrors* g3= new TGraphAsymmErrors();
  Int_t n1=g1->GetN();
  Int_t n2=g2->GetN();

  if (n1!=n2) {
    printf(" vectors do not have same number of entries !  \n");
   return g3;
  }

  Double_t   x1=0.,   y1=0., x2=0., y2=0.;
  Double_t dx1h=0., dx1l=0.;
  Double_t dy1h=0., dy1l=0.;
  Double_t dy2h=0., dy2l=0.;

  //  Double_t* X1 = g1->GetX();
  //Double_t* Y1 = g1->GetY();
  Double_t* EXhigh1 = g1->GetEXhigh();
  Double_t* EXlow1 =  g1->GetEXlow();
  Double_t* EYhigh1 = g1->GetEYhigh();
  Double_t* EYlow1 =  g1->GetEYlow();

  //Double_t* X2 = g2->GetX();
  //Double_t* Y2 = g2->GetY();
  //Double_t* EXhigh2 = g2->GetEXhigh();
  //Double_t* EXlow2 =  g2->GetEXlow();
  Double_t* EYhigh2 = g2->GetEYhigh();
  Double_t* EYlow2 =  g2->GetEYlow();

  for (Int_t i=0; i<g1->GetN(); i++) {
    g1->GetPoint(i,x1,y1);
    g2->GetPoint(i,x2,y2);
    dx1h  = EXhigh1[i];
    dx1l  = EXlow1[i];
    if (y1!=0.) dy1h  = EYhigh1[i]/y1;
    else        dy1h  = 0.;
    if (y2!=0.) dy2h  = EYhigh2[i]/y2;
    else        dy2h  = 0.;
    if (y1!=0.) dy1l  = EYlow1 [i]/y1;
    else        dy1l  = 0.;
    if (y2!=0.) dy2l  = EYlow2 [i]/y2;
    else        dy2l  = 0.;
   
    if (debug)
      printf("%d dy1=%f %f dy2=%f %f sqrt= %f %f \n",i,dy1l,dy1h,dy2l,dy2h,
              sqrt(dy1l*dy1l+dy2l*dy2l),sqrt(dy1h*dy1h+dy2h*dy2h));

    if (y2!=0.) g3->SetPoint(i, x1,y1/y2);
    else       g3->SetPoint(i, x1,y2);
    Double_t el=0.; Double_t eh=0.;
    
    if (y1!=0. && y2!=0.) el=sqrt(dy1l*dy1l+dy2l*dy2l)*(y1/y2);
    if (y1!=0. && y2!=0.) eh=sqrt(dy1h*dy1h+dy2h*dy2h)*(y1/y2);
    
    if (debug) printf("dx1h=%f  dx1l=%f  el=%f  eh=%f \n",dx1h,dx1l,el,eh);
    g3->SetPointError(i,dx1h,dx1l,el,eh);
    
  }  
  return g3;
}


//__________________________________________________________________________________________________
TGraphAsymmErrors*  TGuiUtils::myMakeBand(TGraphErrors* g0, TGraphErrors* g1,TGraphErrors* g2)
{
  TGraphAsymmErrors* g3= new TGraphAsymmErrors();
  
  Double_t  x1=0., y1=0., x2=0., y2=0., y0=0, x3=0.;
  Double_t dum;
  for (Int_t i=0; i<g1->GetN(); i++) {
    g0->GetPoint(i, x1,y0);
    g1->GetPoint(i, x1,y1);
    g2->GetPoint(i, x1,y2);

    if (i==g1->GetN()-1) x2=x1;
    else                 g2->GetPoint(i+1,x2,dum);

    if (i==0)            x3=x1;
    else                 g2->GetPoint(i-1,x3,dum);
    
    Double_t tmp=y2;
    if (y1<y2) {y2=y1; y1=tmp;}
    Double_t y3=y0;
    g3->SetPoint(i,x1,y3);

    Double_t binwl=(x1-x3)/2.;
    Double_t binwh=(x2-x1)/2.;
    if (binwl==0.)  binwl= binwh;
    if (binwh==0.)  binwh= binwl;
    g3->SetPointError(i,binwl,binwh,(y3-y2),(y1-y3));

  }
  return g3;

}


//__________________________________________________________________________________________________
void TGuiUtils::myAddtoBand(TGraphErrors* g1, TGraphAsymmErrors* g2)
{
  Double_t  x1=0., y1=0.,  y2=0., y0=0;

  if (g1->GetN()!=g2->GetN())
   cout << " graphs have not the same # of elements " << endl;

  Double_t* EYhigh = g2-> GetEYhigh();
  Double_t* EYlow  = g2-> GetEYlow();

  for (Int_t i=0; i<g1->GetN(); i++) {
    g1->GetPoint(i, x1,y1);
    g2->GetPoint(i, x1,y2);

    if (y1==0) y1=1;
    if (y2==0) y2=1;

    Double_t eyh=0., eyl=0.;

    y0=y1-y2;
    if (y0!=0) {
      if (y0>0) {
	eyh=EYhigh[i];
	eyh=sqrt(eyh*eyh+y0*y0);
	g2->SetPointEYhigh(i,eyh);
      } else {
	eyl=EYlow[i];
	eyl=sqrt(eyl*eyl+y0*y0);
	g2->SetPointEYlow (i,eyl);
      }
    }
  }
  return;

}


//__________________________________________________________________________________________________
TGraphErrors* TGuiUtils::TH1ToTGraph(TH1 *h1)
{
  if (!h1) cout << "TH1TOTGraph: histogram not found !" << endl;
  
  TGraphErrors* g1= new TGraphErrors();
  
  Double_t x, y, ex, ey;
  for (Int_t i=0; i<h1->GetNbinsX(); i++) {
    y=h1->GetBinContent(i);
    ey=h1->GetBinError(i);
    x=h1->GetBinCenter(i);
    ex=h1->GetBinWidth(i);

    g1->SetPoint(i,x,y);
    g1->SetPointError(i,ex,ey);
  }
  
 return g1;
}


//__________________________________________________________________________________________________
void TGuiUtils::myText(Double_t x,Double_t y,Color_t color,const char *text, Double_t tsize)
{
  TLatex l; 
  //l.SetTextAlign(12); 
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);

}


//__________________________________________________________________________________________________
void TGuiUtils::myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,const char *text)
{
  Double_t tsize=0.06;

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);

  Double_t y1=y-0.25*tsize;
  Double_t y2=y+0.25*tsize;
  Double_t x2=x-0.3*tsize;
  Double_t x1=x2-boxsize;

  printf("x1= %f x2= %f y1= %f y2= %f \n",x1,x2,y1,y2);

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetFillColor(mcolor);
  mbox->SetFillStyle(1001);
  mbox->Draw();

  TLine mline;
  mline.SetLineWidth(4);
  mline.SetLineColor(1);
  mline.SetLineStyle(1);
  Double_t y3=(y1+y2)/2.;
  mline.DrawLineNDC(x1,y3,x2,y3);
}


//__________________________________________________________________________________________________
void TGuiUtils::myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle,const char *text,Float_t msize) 
{
  Double_t tsize=0.06;
  TMarker *marker = new TMarker(x-(0.4*tsize),y,8);
  marker->SetMarkerColor(color);  marker->SetNDC();
  marker->SetMarkerStyle(mstyle);
  marker->SetMarkerSize(msize);
  marker->Draw();

  TLatex l; l.SetTextAlign(12); //l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);
}

//__________________________________________________________________________________________________
void TGuiUtils::myHatchBox(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
			   Option_t* option, Color_t color)
{
  TPave* box = new TPave(x1,y1,x2,y2,0,option);                                                
  box->SetFillStyle(3005);
  box->SetFillColor(color);    
  box->Draw();
}
//__________________________________________________________________________________________________
void TGuiUtils::myLine(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
		       Color_t color)
{
  TLine* _l = new TLine(x1, y1, x2, y2);
  _l->SetLineColor(color);
  _l->Draw();
}
//__________________________________________________________________________________________________
TCanvas* TGuiUtils::myCanvas(const char* name, int w, int h, int nx, int ny){
  TCanvas* slide = new TCanvas(name,name,10,10,w,h);
  slide->Divide(nx,ny);
  slide->cd(1);
  slide->Modified();
  return slide;
}
//__________________________________________________________________________________________________
TH1F* TGuiUtils::myTH1F(const char* name, const char* title, int nbin, float nlow, float nhigh, 
			const char* titleX, const char* titleY){
  TH1F* h = new TH1F(name,title,nbin, nlow, nhigh);
  h->GetXaxis()->SetTitle(titleX);
  h->GetYaxis()->SetTitle(titleY);
  h->Sumw2();
  return h;
}

//__________________________________________________________________________________________________
TH1F* TGuiUtils::myTH1F(const char* name, const char* title, int nbin, const Double_t* nX,  
			const char* titleX, const char* titleY){
  TH1F* h = new TH1F(name,title,nbin, nX);
  h->GetXaxis()->SetTitle(titleX);
  h->GetYaxis()->SetTitle(titleY);
  h->Sumw2();
  return h;
}

//__________________________________________________________________________________________________
TProfile* TGuiUtils::myTProfile(const char* name, const char* title, 
				int nbinX, float nlowX, float nhighX, 
				float nlowY, float nhighY, 
				const char* titleX, const char* titleY){
  TProfile*  h = new TProfile(name, title, nbinX, nlowY, nhighX, nlowY, nhighY);
  h->GetXaxis()->SetTitle(titleX);
  h->GetYaxis()->SetTitle(titleY);
  h->Sumw2();
  return h;
}
//__________________________________________________________________________________________________
TH2F* TGuiUtils::myTH2F(const char* name, const char* title, 
			int nbinX, float nlowX, float nhighX, 
			int nbinY, float nlowY, float nhighY,
			const char* titleX, const char* titleY){

  TH2F* h = new TH2F(name,title,nbinX,nlowX,nhighX, nbinY,nlowY,nhighY);
  h->GetXaxis()->SetTitle(titleX);
  h->GetYaxis()->SetTitle(titleY);
  h->Sumw2();
  return h;
}

//__________________________________________________________________________________________________
TH2F* TGuiUtils::myTH2F(const char* name, const char* title, 
			int nbinX, const Double_t* nX,  
			int nbinY, float nlowY, float nhighY,
			const char* titleX, const char* titleY){

  TH2F* h = new TH2F(name,title,nbinX,nX,nbinY,nlowY,nhighY);
  h->GetXaxis()->SetTitle(titleX);
  h->GetYaxis()->SetTitle(titleY);
  h->Sumw2();
  return h;
}
//__________________________________________________________________________________________________
TH2F* TGuiUtils::myTH2F(const char* name, const char* title, 
			int nbinX, const Double_t* nX,  
			int nbinY, const Double_t* nY,  
			const char* titleX, const char* titleY){

  TH2F* h = new TH2F(name,title,nbinX,nX,nbinY,nY);
  h->GetXaxis()->SetTitle(titleX);
  h->GetYaxis()->SetTitle(titleY);
  h->Sumw2();
  return h;
}

//__________________________________________________________________________________________________
TH3F* TGuiUtils::myTH3F(const char* name, const char* title, 
			int nbinX, float nlowX, float nhighX, 
			int nbinY, float nlowY, float nhighY,
			int nbinZ, float nlowZ, float nhighZ,
			const char* titleX, const char* titleY, const char* titleZ)
{

  TH3F* h = new TH3F(name,title,nbinX,nlowX,nhighX, 
		     nbinY,nlowY,nhighY,
		     nbinZ,nlowZ,nhighZ);
  h->GetXaxis()->SetTitle(titleX);
  h->GetYaxis()->SetTitle(titleY);
  h->GetZaxis()->SetTitle(titleZ);
  h->Sumw2();
  return h;
}

//__________________________________________________________________________________________________
TH3F* TGuiUtils::myTH3F(const char* name, const char* title, 
			int nbinX, const Double_t* nX,  
			int nbinY, const Double_t* nY,  
			int nbinZ, const Double_t* nZ,  
			const char* titleX, const char* titleY, const char* titleZ)
{
  
  TH3F* h = new TH3F(name,title,nbinX,nX,nbinY,nY,nbinZ,nZ);
  h->GetXaxis()->SetTitle(titleX);
  h->GetYaxis()->SetTitle(titleY);
  h->GetZaxis()->SetTitle(titleZ);
  h->Sumw2();
  return h;
}
//__________________________________________________________________________________________________
void TGuiUtils::myDraw1d(TH1* hist, TCanvas* c, int npad, Option_t* option, bool logy,Color_t color, bool showStat, int markerStyle)
{
 TVirtualPad* p = c->cd(npad);
 myDraw1d(hist, p, npad, option, logy,color, showStat, markerStyle);
}
//__________________________________________________________________________________________________
void TGuiUtils::myDraw1d(TH1* hist, TVirtualPad* p, int npad, Option_t* option, bool logy,Color_t color, bool showStat, int markerStyle){
  if(color != -1){
    //    hist->SetFillColor(color);
    hist->SetMarkerColor(color);
    hist->SetLineColor(color);
  }
  hist->SetMarkerStyle(markerStyle);  

  float yMax=hist->GetMaximum();

  //find if the pad has already a histogram and rescale the Y-axis accordingly.
  TList *list = p->GetListOfPrimitives();
  TIter next(list); 
  TObject *obj; 
  TH1* h1=NULL;
  int nHisto=0;
  while ((obj=next())) { 
    if (obj->InheritsFrom("TH1")) { 
      TH1 *h = (TH1*)obj;
      nHisto++;
      if(h1==NULL) h1=h;
      if(h->GetMaximum()>yMax)	yMax=h->GetMaximum();
    } 
  }
  if(h1)  h1->SetMaximum(yMax*1.1);

  hist->Draw(option);
  if(logy) p->SetLogy();
  
  p->Update(); //this should force generation of stat box... doesn't seem to do it.

  //Get this histogram stat box (make sure to use same to get the stat box
  TPaveStats *st = (TPaveStats*)hist->FindObject("stats"); 
  if(st){
    //    if(showStat && nHisto==0){
    if(showStat){
      st->SetX1NDC(.79-nHisto*0.22); 
      st->SetX2NDC(.99-nHisto*0.22); 
    }
    else hist->SetStats(0);
  }
 
  p->Modified();
}
//__________________________________________________________________________________________________
void TGuiUtils::myDraw2d(TH2* hist, TCanvas* c, int npad, Option_t* option, Color_t color, bool showStat)
{
  TVirtualPad* p = c->cd(npad);
  myDraw2d(hist, p, npad, option, color, showStat);
}

//__________________________________________________________________________________________________
void TGuiUtils::myDraw2d(TH2* hist, TVirtualPad* p, int npad, Option_t* option, Color_t color, bool showStat){
  if(color != -1){
    //    hist->SetFillColor(color);
    hist->SetMarkerColor(color);
    hist->SetLineColor(color);
  }
  
  hist->Draw(option);
    
  p->Update(); //this should force generation of stat box... doesn't seem to do it.

  //Get this histogram stat box (make sure to use sames to get the sta box
  TPaveStats *st = (TPaveStats*)hist->FindObject("stats"); 
  if(st){
    if(showStat){
      st->SetX1NDC(.79); 
      st->SetX2NDC(.99); 
    }
    else hist->SetStats(0);
  }
 
  p->Modified();
}
//__________________________________________________________________________________________________
void TGuiUtils::addToTHStack(THStack* _hStack, TH1F* _h, int icol, const char* opt, 
			     TLegend* _l, const char* _txt)
{
  _h->SetFillColor(icol);
  _hStack->Add(_h,opt);
  
  char ss[20];
  TString _opt(opt);
  if(_opt.CompareTo("HIST")==0) sprintf(ss,"f");
  if(_opt.CompareTo("e")==0) sprintf(ss,"p");
  _l->AddEntry(_h,_txt,ss);


}
//_____________________________________________________________________________
void TGuiUtils::getStackHisto(THStack* _hStack, TH1F* _stackH){

  //TH1F* _stackH = (TH1F*) _h->Clone();
  _stackH->Reset();
  _stackH->SetTitle(string("Stack_"+string(_stackH->GetTitle())).c_str());
  _stackH->SetName(string("Stack_"+string(_stackH->GetName())).c_str());
  //  _stackH = (TH1F*) _hStack->GetHistogram();
  TIter next(_hStack->GetHists());
  TObject* _obj;
  while ((_obj = next()))
    _stackH->Add((TH1F*)_obj,1);
}
//_____________________________________________________________________________
void TGuiUtils::myDrawTHStack(TCanvas* _c, THStack* _hStack, 
			      TH1F* _stackH,  TLegend* _l, bool logy){
  const float maxScaleLin=1.2;
  const float maxScaleLog=10;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;

  if(!_stackH) {
    std::cout << "histSTack empty" <<std::endl;
    return;
  }

  Double_t max=(_stackH->GetMaximum()+_stackH->GetBinError(_stackH->GetMaximumBin()))*(1+0.1);

  _c->cd(1);
  _hStack->Draw("hist");
  _hStack->SetMaximum(max*scale);
  _hStack->GetYaxis()->SetTitleSize(0.06);
  _hStack->GetYaxis()->SetTitleOffset(1.2);
  _hStack->GetYaxis()->SetLabelSize(0.06);
  _hStack->GetXaxis()->SetTitle(_stackH->GetXaxis()->GetTitle());
  _hStack->GetYaxis()->SetTitle(_stackH->GetYaxis()->GetTitle());
  _hStack->Draw("hist");
  if(logy) _c->SetLogy();

  if(_l){
    _l->SetTextSize(0.05);
    _l->Draw();
  }
  _c->RedrawAxis();
  _c->Update();
  
}

//_____________________________________________________________________________
TVirtualPad* TGuiUtils::myDrawRatio(TCanvas* _c, TPad* _pTop, TPad* _pBot, 
				    THStack* _hStack, TH1F* _stackH,  TH1F* _h, TLegend* _l, bool logy){
  const float maxScaleLin=1.4;
  const float maxScaleLog=10;
  float scale=maxScaleLin;
  if(logy)  scale=maxScaleLog;

  TVirtualPad* _tv = _c->cd();

  if(!_stackH) {
    std::cout << "histSTack empty" <<std::endl;
    return _tv;
  }

  Double_t max=(_stackH->GetMaximum()+_stackH->GetBinError(_stackH->GetMaximumBin()))*(1+0.1);
  if(_h) max = getMax(_h,_stackH);

  //(Pred-Data)/Data ratio
  //==>> Data/MC ratio
  TH1F*  _ratioH=NULL;
  float avg =0;
  if(_h){
    /*
    TH1F* _hDiff = (TH1F*) _h->Clone();
    _hDiff->Reset();
    _hDiff->Add(_stackH,_h,1,-1);
    */
    TH1F* _hMC = (TH1F*) _stackH->Clone();

    _ratioH= (TH1F*) _h->Clone();
    _ratioH->Reset();
    _ratioH->GetXaxis()->SetTitle(_h->GetXaxis()->GetTitle());
    _ratioH->GetXaxis()->SetTitleSize(0.05);
    _ratioH->SetLabelSize(0.12,"X");
    _ratioH->SetLabelSize(0.12,"Y");
    //_ratioH->GetYaxis()->SetTitle("(Pred-Data)/Data");
    _ratioH->GetYaxis()->SetTitle("Data/SM");
    _ratioH->GetYaxis()->SetTitleSize(0.05);
    _ratioH->GetYaxis()->SetNdivisions(205);
    //_ratioH->SetAxisRange(-0.3,0.3,"Y");
    
    //_ratioH->Divide(_hDiff,_h,1,1);
    _ratioH->Divide(_h,_hMC,1,1);
    avg = _h->Integral(0,-1) / _hMC->Integral(0,-1);
  }

  _pTop->SetTopMargin(0.05);
  _pTop->SetBottomMargin(0.4);
  _pTop->SetRightMargin(0.05);
  _pTop->SetLeftMargin(0.15);
  _pTop->SetNumber(1);
  if(_h){
    _pTop->SetBottomMargin(0.05);
    _pBot->SetTopMargin(0);
    _pBot->SetBottomMargin(0.4);
    _pBot->SetRightMargin(0.05);
    _pBot->SetLeftMargin(0.15);
    _pBot->SetNumber(2);
  }
  _tv->cd();

  //Top plot
  _pTop->Draw();
  _pTop->cd();

  float min = getMinBin(_stackH);
  if(min<0) min=fabs(min); //protection
  if(min<0.1) min= 0.1;
  else if(min<1) min=1;
  if(max==0)  max=1;
  //  if(min<=0 && logy) min=1;

  if(_h){
    _h->SetMaximum(max*scale);
    _h->SetMinimum(min);
    _h->GetYaxis()->SetRangeUser(min,max*scale);
    myDraw1d(_h,_pTop,kBlack,"p",logy, -1, false,20);
  }


  _hStack->Draw("histsame");
  _hStack->SetMaximum(max*scale);
  _hStack->SetMinimum(min);
  _hStack->GetYaxis()->SetTitleSize(0.06);
  _hStack->GetYaxis()->SetTitleOffset(1.2);
  _hStack->GetYaxis()->SetLabelSize(0.06);
  _hStack->GetXaxis()->SetTitle(_stackH->GetXaxis()->GetTitle());
  _hStack->GetYaxis()->SetTitle(_stackH->GetYaxis()->GetTitle());
  _hStack->GetYaxis()->SetRangeUser(min,max*scale);
  //  _hStack->Draw("hist");

  if(_h){
    myDraw1d(_h,_pTop,kBlack,"psame",logy, -1, false,20);
  }
  else{
    if(logy) _pTop->SetLogy();
  }
  if(_l){
    _l->SetTextSize(0.05);
    _l->Draw();
  }

  std::cout << "min " << min << " max " << max << std::endl;

  _pTop->RedrawAxis();
  _pTop->Update();
  
  //Bottom Ratio
  _tv->cd();
  if(_ratioH){
    _pBot->Draw();
    _pBot->cd();
    _ratioH->GetXaxis()->SetTitleSize(0.15);
    _ratioH->GetXaxis()->SetTitleOffset(1.2);
    _ratioH->GetYaxis()->SetTitleSize(0.08);
    _ratioH->GetYaxis()->SetTitleOffset(0.8);
    myDraw1d(_ratioH,_pBot,1,"p",false, kBlack, false,20);
    _ratioH->SetMarkerSize(0.3);

    TLine* _line = new TLine(_ratioH->GetBinCenter(1)-_ratioH->GetBinWidth(1)/2,1,
			     _ratioH->GetBinCenter(_ratioH->GetNbinsX())+
			     _ratioH->GetBinWidth(_ratioH->GetNbinsX())/2,1);

    float min = _ratioH->GetMinimum()*1.2;
    float max = _ratioH->GetMaximum()*1.2;

    _line->SetLineStyle(7);
    _line->SetLineWidth(1);
    _line->SetLineColor(kRed);
    _line->Draw();
    _pBot->RedrawAxis();
    
    if(min<0) min=-1;
    if(max>1) max=2;
    min=0;
    max=2;
    _ratioH->GetYaxis()->SetRangeUser(min,max);
    _pBot->Update();
  }

  return _tv;

}
//_____________________________________________________________________________
void TGuiUtils::moveUnderOverFlow(TH1* h, int opt){

  if(opt==0 || opt==1){ //undeflow   
    double cunder = h->GetBinContent(0);
    double eunder = h->GetBinError(0);
    double efirst = h->GetBinError(1);
    double efirstnew = sqrt(efirst*efirst+eunder*eunder);
    h->AddBinContent(1,cunder);
    h->SetBinError(1,efirstnew); 
    h->SetBinContent(0,0);
    h->SetBinError(0,0);
  }
  if(opt==0 || opt==2){//overflow
    int nbins=  h->GetNbinsX();
    double cover = h->GetBinContent(nbins+1);
    double eover = h->GetBinError(nbins+1);
    double elast = h->GetBinError(nbins);
    double elastnew = sqrt(elast*elast+eover*eover);
    h->AddBinContent(nbins,cover);
    h->SetBinError(nbins,elastnew); 
    h->SetBinContent(nbins+1,0); 
    h->SetBinError(nbins+1,0); 
  }
}

//_____________________________________________________________________________
double TGuiUtils::getMax(TH1* h1, TH1* h2){
  double max1=(h1->GetMaximum()+h1->GetBinError(h1->GetMaximumBin()))*(1+0.1);
  double max2=(h2->GetMaximum()+h2->GetBinError(h2->GetMaximumBin()))*(1+0.1);
  double max=max1;
  if(max1<max2) max=max2;
  return max;
}
//_____________________________________________________________________________
double TGuiUtils::getMin(TH1* h1, TH1* h2){
  double min1=(h1->GetMinimum()-h1->GetBinError(h1->GetMinimumBin()))*(1+0.1);
  double min2=(h2->GetMinimum()-h2->GetBinError(h2->GetMinimumBin()))*(1+0.1);
  double min=min1;
  if(min2<min1) min=min2;
  return min;
}
//_____________________________________________________________________________
double TGuiUtils::getMinBin(TH1* h){
  double minBin=h->GetMaximum();
  for(int ibin=1; ibin<=h->GetNbinsX(); ibin++){
    if(h->GetBinContent(ibin)<minBin && h->GetBinContent(ibin)!=0) minBin = h->GetBinContent(ibin);
  }
  return minBin;
}
//_____________________________________________________________________________
void TGuiUtils::legendSetting(TLegend* leg,float fontSize){
  leg->SetFillColor(10);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetTextSize(fontSize);
}

//_____________________________________________________________________________
double TGuiUtils::getHistMean(TH1* h)
{
  int numBins = h->GetXaxis()->GetNbins(); 
  Double_t *x = new Double_t[numBins]; 
  Double_t* y = new Double_t[numBins]; 
  for (int i = 1; i <= numBins; i++) { 
    x[i-1] = h->GetBinCenter(i); 
    y[i-1] = h->GetBinContent(i); 
  } 
  double median = TMath::Median(numBins, x, y);
  delete x;
  delete y;
  return median;
}
//_____________________________________________________________________________
void TGuiUtils::yAxis(TH1* h1, const char* _unit){

  float x1 = h1->GetXaxis()->GetXmin();
  float x2 = h1->GetXaxis()->GetXmax();
  float nbins= h1->GetXaxis()->GetNbins() ;
  float range = x2-x1;
  float R = range/nbins;
  char _width[200];
  string _label;
  
  if(int(range)%int(nbins)==0) sprintf(_width,"%d",int(R));
  else sprintf(_width,"%3.2f",R);
  _label = "Events/";
  _label.append(_width);
  _label.append(_unit);

  h1->GetYaxis()->SetTitle(_label.c_str());
  return;

}
//_____________________________________________________________________________
void TGuiUtils::normalize(TH1* h1, float val){
  
  float integral=h1->GetEntries();
  if(integral==0) return;
  
  integral = h1->Integral(0,h1->GetXaxis()->GetNbins());

  if(integral!=0) h1->Scale(val/integral);
}


//_____________________________________________________________________________                                                                                                                            
void TGuiUtils::templateHisto(){
  
  TF1* _f = new TF1("f1","TMath::Landau(x,[0],[1],0)",-5,1000);
  _f->SetParameters(80,25); 

  TCanvas* _c = myCanvas("TEST"); 
  TH1F* _h = myTH1F("test Hist" , "test", 250, 0, 500, "p_{T} [GeV]", "Events");
  _h->FillRandom("f1", 10000);
  _h->GetXaxis()->SetNdivisions(205);
  myDraw1d(_h, _c, 1, "e", false, kBlack, false);
    
}
