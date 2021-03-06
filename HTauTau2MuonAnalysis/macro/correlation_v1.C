#include "HttStylesNew.cc"
#include <string>
#include <vector>
#include <iostream>
#include "Rtypes.h"
#include "TH1.h"
#include "TH2.h"
#include <TROOT.h>
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TMath.h"
#include "HtoH.h"
void correlation_v1(TString fileName = "Data_RegionC",
		 //		 TString fileS1Name,
		 //		 TString fileS2Name,
		 bool bkgSideBand = true,
		 TString legName = "Region C") {

  float lumi = 19700;
  float sigma = 3;
 
  // bins for rebinning
   int newNBinsX = 4;
   float newBins[5] = {0.,1.,2.,3.,10.};

   TFile * f = new TFile (fileName+".root");
   //   TFile * fs1 = new TFile (fileS1Name+".root");
   //   TFile * fs2 = new TFile (fileS2Name+".root"); 
//    gROOT->Macro("Style_Imported_Style_1.C");
//    gROOT->SetStyle("Imported_Style_1") ;
   SetStyle();
   gStyle->SetOptStat(0000);
   
   TH1F * h1  = (TH1F*)f->Get("InvMassTrackPlusPos_2QH");
   TH1F * h2  = (TH1F*)f->Get("InvMassTrackPlusPos_3QH");
   TH1F * h3  = (TH1F*)f->Get("InvMassTrackPlusNeg_2QH");
   TH1F * h4  = (TH1F*)f->Get("InvMassTrackPlusNeg_3QH");
   
   if (bkgSideBand) {
     h1  = (TH1F*)f->Get("InvMassTrackPlusPos_2Q_ControlH");
     h2  = (TH1F*)f->Get("InvMassTrackPlusPos_3Q_ControlH");
     h3  = (TH1F*)f->Get("InvMassTrackPlusNeg_2Q_ControlH");
     h4  = (TH1F*)f->Get("InvMassTrackPlusNeg_3Q_ControlH");
   }

   TH1F * a1 = new TH1F("add1","",100,0.,10.);
   TH1F * a2 = new TH1F("add2","",100,0.,10.);
   //   TH1F * N1D_control_i_H= new TH1F("N1D_control_i_H","",100,0.,10.);
    
   //   a1->Add(h1,h2);
   //   a2->Add(h3,h4);
   //   N1D_control_i_H->Add(a1,a2);

   TH1F * N1D_control_i_H = (TH1F*)f->Get("InvMassTrackPlusMuon1D_ControlH");

   N1D_control_i_H->Scale(1./N1D_control_i_H->GetSumOfWeights());   
   // rebinning --->
   TH1F * N1Drebin = (TH1F*)TH1toTH1(N1D_control_i_H,newNBinsX,newBins,true,"_rebin");

//    float xBin;
//    float yBin;
//    float x1D = N1D_control_i_H->GetBinContent(xBin);
//    float y1D = N1D_control_i_H->GetBinContent(yBin);

   

   TH2F * True2DH = NULL;
   TH2F * True2Dsignal1H = NULL;
   TH2F * True2Dsignal2H = NULL;

   if (bkgSideBand) {

     True2DH = new TH2F("True2DH","",100,0.,10.,100,0.,10.);

     TH2F * H1  = (TH2F*)f->Get("InvMassTrackPlusMuon2D_ControlPosH");
     TH2F * H2  = (TH2F*)f->Get("InvMassTrackPlusMuon2D_ControlNegH");
     TH2F * H3  = (TH2F*)f->Get("InvMassTrackPlusMuon2D_ControlBothH");   
     
     float entriesBkg = H1->GetEntries() + H2->GetEntries() + H3->GetEntries();
     TH1F * counterFinalH = (TH1F*)f->Get("counterFinalH");
     float entriesSig = counterFinalH->GetEntries();
     float ratio = entriesBkg / entriesSig;     
     std::cout << std::endl;
     std::cout << "ratio(Bkg/Sig) = " << entriesBkg << "/" << entriesSig << " = " << ratio << std::endl; 

     TH2F * A1 = new TH2F("A1","",100,0.,10.,100,0.,10.);
     A1->Add(H1,H2);
     True2DH->Add(A1,H3);

     // TH1F * inputs1 = (TH1F*)fs1->Get("inputEventsH");
     // TH1F * inputs2 = (TH1F*)fs2->Get("inputEventsH");

     // float ev1 = inputs1->GetSumOfWeights();
     // float ev2 = inputs2->GetSumOfWeights();

     // TH2F * S1H1  = (TH2F*)fs1->Get("InvMassTrackPlusMuon2D_ControlPosH");
     // TH2F * S1H2  = (TH2F*)fs1->Get("InvMassTrackPlusMuon2D_ControlNegH");
     // TH2F * S1H3  = (TH2F*)fs1->Get("InvMassTrackPlusMuon2D_ControlBothH");   

     // TH2F * S2H1  = (TH2F*)fs2->Get("InvMassTrackPlusMuon2D_ControlPosH");
     // TH2F * S2H2  = (TH2F*)fs2->Get("InvMassTrackPlusMuon2D_ControlNegH");
     // TH2F * S2H3  = (TH2F*)fs2->Get("InvMassTrackPlusMuon2D_ControlBothH");   

   }
   else {

     True2DH = (TH2F*)f->Get("InvMassTrackPlusMuon2D_H");
     

   }

   std::cout << "Entries (True2D) = " << True2DH->GetEntries() << std::endl;
   float Norm = True2DH->GetSumOfWeights();
   

   TH2F * True2Drebin = (TH2F*)TH2toTH2(True2DH,newNBinsX,newBins,newNBinsX,newBins,"_rebin");
   TH2F * True2DrebinTmp = (TH2F*)True2Drebin->Clone("Tmp2D");
   for (int iB=1; iB<=newNBinsX; ++iB) {
     for (int jB=1; jB<=newNBinsX; ++jB) {
       if (iB!=jB) {
	 float c1 = True2DrebinTmp->GetBinContent(iB,jB);
	 float c2 = True2DrebinTmp->GetBinContent(jB,iB);
	 float e1 = True2DrebinTmp->GetBinError(iB,jB);
	 float e2 = True2DrebinTmp->GetBinError(jB,iB);
	 float c = c1 + c2;
	 float e = TMath::Sqrt(e1*e1+e2*e2);
	 True2Drebin->SetBinContent(iB,jB,c);
	 True2Drebin->SetBinError(iB,jB,e);
       }
       True2Drebin->SetBinContent(iB,jB,True2Drebin->GetBinContent(iB,jB)/Norm);
       True2Drebin->SetBinError(iB,jB,True2Drebin->GetBinError(iB,jB)/Norm);
     }
   }

   TH2F * prod2DH= new TH2F("InvMass2DTrackPlusProbe_2H","",newNBinsX,newBins,newNBinsX,newBins);
   for(int ix=1; ix<=newNBinsX; ix++){
    for(int iy=1; iy<=newNBinsX; iy++){
      float x = N1Drebin->GetBinContent(ix);
      float y = N1Drebin->GetBinContent(iy);
      float prod = x * y;
      if (ix==iy)
	prod2DH->SetBinContent(ix,iy,prod);
      else
	prod2DH->SetBinContent(ix,iy,2*prod);
    }
   }


   TH2F * Corr2DH = new TH2F("Corr2DH","",newNBinsX,newBins,newNBinsX,newBins);
   
   int nBins1D = (newNBinsX+1) * newNBinsX / 2;
   TH1F * Corr1DH = new TH1F("Corr1DH","",nBins1D,0.,float(nBins1D));

   std::cout << std::endl;
   int counter = 0;
   for(int ix=1; ix<=newNBinsX; ix++){
    for(int iy=ix; iy<=newNBinsX; iy++){
      float XC = prod2DH->GetBinContent(ix,iy);
      float true2DC = True2Drebin->GetBinContent(ix,iy);
      float err2DC  = True2Drebin->GetBinError(ix,iy);
      float corrC = 1;
      float errC = 0;
      if (XC>0) 
	corrC = true2DC / XC ;
      if (true2DC>0)
	errC = corrC * err2DC / true2DC;
      
      corrC = floor(100*corrC+0.5)/100;
      errC  = floor(100*errC+0.5)/100;

      Corr2DH->SetBinContent(ix,iy,corrC);
      Corr2DH->SetBinError(ix,iy,errC);
      Corr2DH->SetBinContent(iy,ix,corrC);
      Corr2DH->SetBinError(iy,ix,errC);
      //Corr2DH->Draw("box");
      char strVal[20];
      sprintf(strVal,"%4.2f +/- %4.2f",corrC,errC); 
      char strLab[20];
      sprintf(strLab,"(%1i,%1i)",ix,iy);
      std::cout << strLab << " : " << strVal << std::endl;
      counter++;
      Corr1DH->SetBinContent(counter,corrC);
      Corr1DH->SetBinError(counter,errC);
      Corr1DH->GetXaxis()->SetBinLabel(counter,strLab);
    }
    std::cout << std::endl;
   }   

   Corr1DH->GetXaxis()->SetLabelSize(0.07);
   Corr1DH->SetMarkerSize(1.4);
   Corr1DH->GetYaxis()->SetTitle("Correlation coefficient");
   Corr1DH->GetXaxis()->SetTitle("Bin");
   Corr1DH->GetYaxis()->SetTitleOffset(1);
   Corr1DH->GetXaxis()->SetTitleOffset(1);
   Corr1DH->SetLineWidth(2);
   TCanvas * canv = new TCanvas("canv","Canvas",1000,700); 
   Corr1DH->GetYaxis()->SetRangeUser(0.,2.);
   Corr1DH->Draw("e1");
   canv->SetGridx();
   canv->SetGridy();
   canv->Update();
   canv->Print(fileName+"_1Dcorr.png");

   TCanvas * canv2 = new TCanvas("canv1","Canvas",900,700);
   Corr2DH->GetXaxis()->SetTitle("m_{1} [GeV]");
   Corr2DH->GetYaxis()->SetTitle("m_{2} [GeV]");
   Corr2DH->GetXaxis()->SetTitleOffset(1.1);
   Corr2DH->GetYaxis()->SetTitleOffset(1.1);
   // Corr2DH->GetXaxis()->SetTickSize(1);
   // Corr2DH->GetYaxis()->SetTickSize(1);
   
   Corr2DH->GetXaxis()->SetNdivisions(110);
   Corr2DH->GetYaxis()->SetNdivisions(110);
   Corr2DH->Draw("texte");
   Corr2DH->SetMarkerSize(1.8);
   int nMax = 3;
   for (int i=1; i<=nMax; ++i) {
     TLine * lineX = new TLine(0., newBins[i], 10., newBins[i]); 
     lineX->SetLineStyle(1);
     lineX->SetLineWidth(2);
     lineX->Draw();
     TLine * lineY = new TLine(newBins[i], 0., newBins[i], 10.);  
     lineY->SetLineStyle(1);
     lineY->SetLineWidth(2);
     lineY->Draw();
   }
   TLatex * cms = new TLatex(0.25,0.94,"CMS Preliminary   L = 19.7 fb^{-1} at #sqrt{s} = 8 TeV");
   
   cms->SetNDC();
   cms->SetTextSize(0.04);
   cms->Draw();

   TLegend * leg = new TLegend(0.75,0.82,0.95,0.92);
   leg->SetFillColor(0);
   leg->SetTextSize(0.044);
   leg->SetHeader(legName);
   TPad * pad2 = canv2->GetPad(0);
   pad2->RedrawAxis();
   leg->Draw();
   canv2->Update();
   canv2->Print(fileName+"_2Dcorr_v1.png");
   canv2->Print(fileName+"_2Dcorr_v1.pdf","Portrait pdf");


   TFile * outputFile = outputFile = new TFile(fileName+"_corr.root","recreate");
   Corr1DH->Clone("Corr_1D");
   Corr2DH->Clone("Corr_2D");

   outputFile->Write();
   outputFile->Close();

}
   




