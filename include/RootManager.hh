#include "TTree.h"
#include "TH2D.h"

//Singleton class which manages output to ROOT files


class MyROOTManager
{
	

	private:
	
	MyROOTManager(); 					
	virtual ~MyROOTManager();
	TTree* tree;
	TH2D* image;
	


	static MyROOTManager* theInstance;
	

	public:
	static MyROOTManager* GetPointer();	
	void Initialize();
	void Finalize();


	void FillHist(Double_t, Double_t);
	void FillTree(Int_t, Int_t, Double_t);

	
	
	
	
};
	

