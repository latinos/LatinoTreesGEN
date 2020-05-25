void DrawVariable(std::string var, int nbin, float min, float max, std::string cut = "1", int applyLHEweight = 1) {
  
  std::cout << " var = " << var << std::endl;
  std::cout << "    nbin = " << nbin << std::endl;
  std::cout << "    min = " << min << std::endl;
  std::cout << "    max = " << max << std::endl;
  
  TTree* myTree_0 = (TTree*) _file0 -> Get("Analyzer/myTree");
  TH1F* h_0 = new TH1F("h_0","",nbin,min,max);
  TString toDraw_0 = Form ("%s >> h_0", var.c_str());
  if (applyLHEweight) myTree_0->Draw(toDraw_0.Data(),(std::string("weightNominalLHE *(") + cut + std::string(")")).c_str(),"");
  else                myTree_0->Draw(toDraw_0.Data(),(std::string("(") + cut + std::string(")")).c_str(),"");
  float normalization_0 = h_0 -> Integral(-1,-1);
  h_0->Scale(1./normalization_0);
   
  //---- plot
  h_0->SetLineColor(kRed);
  
  h_0->SetLineWidth(2);
  
  h_0->Draw();
  
//   gPad->SetLogy();
  gPad->SetGrid();
  
}
