void DrawCompare(std::string var, int nbin, float min, float max, std::string cut = "1") {
 
 TTree* myTree_0 = (TTree*) _file0 -> Get("Analyzer/myTree");
 TH1F* h_0 = new TH1F("h_0","",nbin,min,max);
 TString toDraw_0 = Form ("%s >> h_0", var.c_str());
 myTree_0->Draw(toDraw_0.Data(),(std::string("weightNominalLHE *(") + cut + std::string(")")).c_str(),"");
 float normalization_0 = h_0 -> Integral(-1,-1);
 h_0->Scale(1./normalization_0);
 
 TTree* myTree_1 = (TTree*) _file1 -> Get("Analyzer/myTree");
 TH1F* h_1 = new TH1F("h_1","",nbin,min,max);
 TString toDraw_1 = Form ("%s >> h_1", var.c_str());
 myTree_1->Draw(toDraw_1.Data(),(std::string("weightNominalLHE *(") + cut + std::string(")")).c_str(),"");
 float normalization_1 = h_1 -> Integral(-1,-1);
 h_1->Scale(1./normalization_1);
  
 //---- plot
 h_0->SetLineColor(kRed);
 h_1->SetLineColor(kBlue);
 
 h_0->SetLineWidth(2);
 h_1->SetLineWidth(2);
 
 h_0->Draw();
 h_1->Draw("same"); 
  
 gPad->SetLogy();
 gPad->SetGrid();
 
}
