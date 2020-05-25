// 
// Show scale variation
// 


void setupHisto(TH1F* histo, int icolor) {
  
  Color_t* color = new Color_t [200];
  color[0] = kRed ;
  color[1] = kAzure + 10 ;
  color[2] = kYellow + 2 ;
  color[3] = kGreen ;
  color[4] = kGreen + 4 ;
  color[5] = kBlue ;
  color[6] = kCyan ;
  color[7] = kPink + 1 ;
  color[8] = kBlack ;
  color[9] = kYellow + 4 ;
  for (int i=0; i<30; i++) {
    color[i+10] = kBlue + i;
  }
  
  
  
  histo->SetLineWidth(2);
  histo->SetLineColor(color[icolor]);
  histo->SetMarkerColor(color[icolor]);
  histo->SetMarkerSize(1);
  histo->SetMarkerStyle(20+icolor);
}


// 
// https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideDataFormatGeneratorInterface
// https://github.com/amassiro/HIGSTXS/blob/master/CalculateNormalizations.cxx
// 

void DrawVariableWeights(std::string var, int nbin, float min, float max, std::string cut = "1") {
  
  std::cout << " var = " << var << std::endl;
  std::cout << "    nbin = " << nbin << std::endl;
  std::cout << "    min = " << min << std::endl;
  std::cout << "    max = " << max << std::endl;
  
  TTree* myTree_0 = (TTree*) _file0 -> Get("Analyzer/myTree");

  TH1F* h_0 = new TH1F("h_0","",nbin,min,max);
  TString toDraw_0 = Form ("%s >> h_0", var.c_str());
//   myTree_0->Draw(toDraw_0.Data(),(std::string("weightNominal *(") + cut + std::string(")")).c_str(),"goff");
  myTree_0->Draw(toDraw_0.Data(),(std::string("weightsLHE[" + std::to_string(4) + "] *(") + cut + std::string(")")).c_str(),"goff");
  float normalization = h_0 -> Integral(-1,-1);
//   h_0->Scale(1./normalization);

  TH1F* h_w[200];
  TH1F* h_w_ratio[200];
  
  for (int i=0; i<20; i++) {
    TString nameHisto = Form ("hw_%d", i);
    h_w[i] = new TH1F(nameHisto.Data(),"",nbin,min,max);
    TString toDraw_1 = Form ("%s >> %s", var.c_str(),nameHisto.Data());
    myTree_0->Draw(toDraw_1.Data(),(std::string("weightsLHE[" + std::to_string(i) + "] *(") + cut + std::string(")")).c_str(),"goff");
    float normalization_1 = h_w[i] -> Integral(-1,-1);
    std::cout << " normalization_1/normalization = " << normalization_1/normalization << std::endl;
//     h_w[i]->Scale(normalization_1/normalization);
    
    setupHisto( h_w[i], i);
    
    TString nameHistoRatio = Form ("hw_ratio_%d", i);
    h_w_ratio[i] = (TH1F*) (h_w[i]->Clone(nameHistoRatio.Data()));
    h_w_ratio[i] -> Divide (h_0);
  }
  

  //---- plot
  h_0->SetLineColor(kRed);  
  h_0->SetLineWidth(4);
  
  TCanvas* cc = new TCanvas ("cc", "", 800, 600);
  
  h_0->Draw();
  
  for (int i=0; i<9; i++) {
    if (i==2 || i==6) continue;
    h_w[i]->Draw("histo same");
  }
  
//   gPad->SetLogy();
  gPad->SetGrid();
  
  
  TCanvas* ccRatio = new TCanvas ("ccRatio", "", 800, 600);
  
  for (int i=0; i<9; i++) {
    if (i==2 || i==6) continue;
    if (i==0) h_w_ratio[i]->Draw("histo");
    else      h_w_ratio[i]->Draw("histo same");
  }
  
  h_w_ratio[0]->GetYaxis()->SetRangeUser(0.5,1.5);
  
  gPad->SetGrid();
  
}
