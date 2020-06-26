 
void postprocess() {
    TFile* f_in = new TFile("./build/raw.root", "read");
    TFile* f_out = new TFile("./output.root", "recreate");
    
    TTree* tree = (TTree*)f_in->Get("MyTree");
    
    Double_t l = 2150; //radiator-detector dist, mm
    Double_t px = 0.055; //pixel size, mm
    
    
    int nentries = tree->GetEntries();
    Double_t edep;
    Int_t x;
    Int_t y;
    Double_t theta;
    tree->SetBranchAddress("EDEP", &edep);
    tree->SetBranchAddress("POSX", &x);
    tree->SetBranchAddress("POSY", &y);
    TH1D* edistr = new TH1D("edep", "edep", 50, 0, 70);
    TH1D* angle = new TH1D("theta", "theta", 50, 0, 4);
    TH2D* energy_angle = new TH2D("ea", "Energy vs angle", 50, 0, 4, 50, 0, 50);
    
    for(int i = 0; i < nentries; ++i) {
        tree->GetEntry(i);
        theta = px / l * sqrt(x*x + y*y);
        
        edistr->Fill(edep*1000);
        angle->Fill(theta*1000);
        energy_angle->Fill(theta*1000, edep*1000);
        
    }
    
    angle->GetXaxis()->SetTitle("#theta, mrad");
    angle->GetYaxis()->SetTitle("Photons per particle");
    angle->Scale(0.000001, "nosw2"); //1M electrons
    //angle->Draw();
    
    edistr->GetXaxis()->SetTitle("E, kev");
    edistr->GetYaxis()->SetTitle("Entries");
    edistr->Draw();
    
    
    f_out->cd();
    edistr->Write();
    angle->Write();
    energy_angle->Write();
}
