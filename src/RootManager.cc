#include "RootManager.hh"
#include "TFile.h"

MyROOTManager* MyROOTManager::theInstance = 0;		//initialize "global" variables

MyROOTManager::MyROOTManager()
{}

MyROOTManager::~MyROOTManager()
{}

MyROOTManager* MyROOTManager::GetPointer()	
{
	if (!theInstance) theInstance = new  MyROOTManager(); 
	return theInstance;
}

void MyROOTManager::Initialize()
{
	
	//create output ROOT data objects

    Int_t a, b;
	Double_t c,d;
    Int_t e;
	tree = new TTree("MyTree", "Timepix hits");
	tree -> Branch("POSX", &a);
	tree -> Branch("POSY", &b);
	tree -> Branch("EDEP", &c);

    
    
	image = new TH2D("image", "Pixels", 256, -7, 7, 256, -7, 7);

}



	// fill methods to call after each event

void MyROOTManager::FillHist(Double_t x, Double_t y)
{
	image -> Fill(x, y);	
}	

void MyROOTManager::FillTree(Int_t x, Int_t y, Double_t e)
{
	tree -> SetBranchAddress("POSX", &x);
	tree -> SetBranchAddress("POSY", &y);
	tree -> SetBranchAddress("EDEP", &e);

	tree -> Fill();
}	




void MyROOTManager::Finalize()
{
	TFile* f;

    f = new TFile("./raw.root", "recreate");
    printf("Results saved to raw.root\n");


	f->cd();
	tree->Write();
	image->Write();
	f -> Close();
}
	


