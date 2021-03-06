#include "HttStylesNew.cc"
void PlotLimits(char* fileList = "limits",
		bool blindData = false) {


  // char * filelist - List of files (output RooT files
  //                   produced by 'combine -M Asymptotic')
  // blindData -  true : blind observed limits
  //             false : show observed limits 
  // int benchmark - 0 : mh-max, mu = +200 GeV
  //                 1 : mh-mod +
  //                 2 : mh-mod -

  SetStyle();
  gStyle->SetOptFit(0000);
  gStyle->SetErrorX(0.5);

  const int nPoints = 12;

  // signal strength limits sigma*BR / sigma*BR (at tanb=30)
  double mA[nPoints];      
  double minus2R[nPoints]; 
  double minus1R[nPoints]; 
  double medianR[nPoints]; 
  double plus1R[nPoints];  
  double plus2R[nPoints];  
  double obsR[nPoints];    

  double obs[nPoints];
  double minus2[nPoints];
  double minus1[nPoints];
  double median[nPoints];
  double plus1[nPoints];
  double plus2[nPoints];

  std::ifstream inputList(fileList);

  TString FileList(fileList);

  TString fileName;

  double MH;
  double LIMIT;

  int counter = 0;

  while (inputList >> fileName) {

    //    std::cout << fileName << std::endl;

    TFile * file = new TFile(fileName);

    TTree * tree = file->Get("limit");

    //    std::cout << "file : " << file << std::endl;
    //    std::cout << "tree : " << tree << std::endl;

    tree->SetBranchAddress("limit",&LIMIT);
    tree->SetBranchAddress("mh",&MH);

    tree->GetEntry(0);
    mA[counter] = float(MH);
    minus2R[counter] = float(LIMIT);
        std::cout << minus2R[counter] << std::endl;
    
    tree->GetEntry(1);
    minus1R[counter] = float(LIMIT);
std::cout << minus1R[counter] << std::endl;
    tree->GetEntry(2);
    medianR[counter] = float(LIMIT);
std::cout << medianR[counter] << std::endl;
    tree->GetEntry(3);
    plus1R[counter] = float(LIMIT);
std::cout << plus1R[counter] << std::endl;
    tree->GetEntry(4);
    plus2R[counter] = float(LIMIT);
std::cout << plus2R[counter] << std::endl;
    tree->GetEntry(5);
    obsR[counter] = float(LIMIT);
std::cout << obsR[counter] << std::endl;
    if (blindData)
      obsR[counter] = medianR[counter];

    counter++; 
      
  }

  std::cout << " m(Phi1)  -2s   -1s   exp   +1s   +2s   obs " << std::endl; 
  //           "100  24.1  28.2  33.8  40.8  48.2  23.0


  for (int i=0; i<counter; ++i) {

    obs[i]    = obsR[i];
    minus2[i] = minus2R[i];
    minus1[i] = minus1R[i];
    median[i] = medianR[i];
    plus1[i]  = plus1R[i];
    plus2[i]  = plus2R[i];

    char strOut[200];
    sprintf(strOut,"%3i  %5.2f  %5.2f  %5.2f  %5.2f  %5.2f  %5.2f",
	    int(mA[i]),minus2[i],minus1[i],median[i],plus1[i],plus2[i],obs[i]);
    std::cout << strOut << std::endl;

  }

  double zeros[nPoints];
  double upper[nPoints];
  double lower[nPoints];
  double central[nPoints];
  for (int i=0; i<counter; ++i) {
    zeros[i] = 0;
    central[i] = 15; 
    minus2[i] = median[i] - minus2[i];
    minus1[i] = median[i] - minus1[i];
    plus1[i]  = plus1[i]  - median[i];
    plus2[i]  = plus2[i]  - median[i];
    upper[i] = 15 - central[i];
    lower[i] = central[i] - obs[i];
  }
  
  
  int nPointsX = counter;

  TGraph * obsG = new TGraph(nPointsX, mA, obs);
  obsG->SetLineWidth(3);
  obsG->SetLineColor(2);
  

  TGraph * expG = new TGraph(nPointsX, mA, median);
  expG->SetLineWidth(3);
  expG->SetLineColor(4);
  expG->SetLineStyle(2);

  
  TGraphAsymmErrors * observed = new TGraphAsymmErrors(nPointsX, mA, central, zeros, zeros, lower, upper);
  observed->SetFillColor(kCyan-4);
  observed->SetLineWidth(3);


  TGraphAsymmErrors * innerBand = new TGraphAsymmErrors(nPointsX, mA, median, zeros, zeros, minus1, plus1);
  innerBand->SetFillColor(kGreen);
  innerBand->SetLineWidth(3);
  innerBand->SetLineStyle(2);
  innerBand->SetLineColor(4);

  TGraphAsymmErrors * outerBand = new TGraphAsymmErrors(nPointsX, mA, median, zeros, zeros, minus2, plus2);
  outerBand->SetFillColor(kYellow);
  outerBand->SetLineWidth(3);
  outerBand->SetLineStyle(2);
  outerBand->SetLineColor(4);

  TH2F * frame = NULL;

  //  frame = new TH2F("frame","",2,100,500,2,0,70);
  frame = new TH2F("frame","",2,4,8,2,0,15);
  frame->GetXaxis()->SetTitle("#bf{m_{ #phi_{1}} [GeV/c]}");
  frame->GetYaxis()->SetTitle("Upper limit on #sigma x BR [pb]");
  frame->GetXaxis()->SetNdivisions(505);
  frame->GetYaxis()->SetTitleOffset(1.2);  
  frame->GetYaxis()->SetTitleSize(0.06);  
  

  TCanvas *canv = MakeCanvas("canv", "histograms", 600, 600);

  frame->Draw();

  outerBand->Draw("3same");
  innerBand->Draw("3same");
  expG->Draw("lsame");
  if (!blindData)
    obsG->Draw("lsame");

  canv->SetGridx();
  canv->SetGridy();
  canv->Update();

  float xLeg = 0.18;
  float yLeg = 0.83;
  float xLegend = 0.57;
  float yLegend = 0.41;
  float sizeLeg = 0.27;

  CMSPrelim(" Preliminary, L = 19.7 fb^{-1},  #sqrt{s} = 8 TeV",0.18,0.84);

  TLegend * leg = new TLegend(0.64,0.72,0.95,0.92);
  leg->SetFillColor(0);
  leg->SetTextSize(0.032);
  leg->AddEntry(innerBand,"expected (68%)","lf");
  leg->AddEntry(outerBand,"expected (95%)","lf");
  if (!blindData) {
    leg->AddEntry(obsG,"observed","l");
  }
  leg->Draw();

  TPad * pad = canv->GetPad(0);
  pad->RedrawAxis();
  leg->Draw();
  canv->Update();
  canv->Print("limits.pdf","Portrait pdf");
  canv->Print("limits.png");

}
