#include  "G4VSensitiveDetector.hh"
#include "TimepixDetector.hh"
#include "TimepixHit.hh"
#include "G4THitsCollection.hh"
#include "G4VHitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "RootManager.hh"
#include <iostream>
#include <map>
#include <utility>
#include <algorithm>


TimepixDetector::TimepixDetector(G4String name) 
: G4VSensitiveDetector(name)
{ 
	collectionName.insert("TimepixHitCollection");
}

TimepixDetector::~TimepixDetector()
{ }

G4bool TimepixDetector::ProcessHits(G4Step* step, G4TouchableHistory* hist)
{ 
	

	TimepixHit* hit = new TimepixHit();

	G4StepPoint* point = step->GetPreStepPoint();
	G4ThreeVector pos = point->GetPosition();

	G4double edep = step -> GetTotalEnergyDeposit();
	G4double kin = step -> GetTrack() -> GetKineticEnergy();

    G4int px_x = (pos.x() + 7.04 * mm) / (55 * micrometer);
    G4int px_y = (pos.y() + 7.04 * mm) / (55 * micrometer);
    
	hit -> AddX(px_x);
	hit -> AddY(px_y);
	hit -> AddE(edep);
	hit -> AddK(kin);

	 
	THC -> insert(hit);
}

void TimepixDetector::Initialize(G4HCofThisEvent* HCE)
{ 
	
	//default hit storing system from examples

	THC = new TimepixHitCollection(GetName(), collectionName[0]);
	static G4int HCID = -1;
	if (HCID < 0 ) HCID = GetCollectionID(0);
	HCE -> AddHitsCollection(HCID, THC);
	//HCE -> AddHitsCollection(HCID, hitsCollection);
}

void TimepixDetector::EndOfEvent(G4HCofThisEvent* HCE)
{ 
	MyROOTManager* RM = MyROOTManager::GetPointer();
    
    G4int id = G4SDManager::GetSDMpointer() -> GetCollectionID("Timepix0/TimepixHitCollection");
	TimepixHitCollection* coll = (TimepixHitCollection*)HCE -> GetHC(id);	

	G4int number = coll -> entries();  //number of hits in this event


    /*
    const G4Event* current_event = 
        G4EventManager::GetEventManager() -> GetConstCurrentEvent();

    if (current_event) {
        event_id = current_event->GetEventID();
    }*/
	//after end of each event hit collection is reviewed to analyze hits

	
    //G4cout << number << G4endl;

    std::map<G4int, G4double> pixels;
    
	for (int i = 0; i < number; i++)
	{
		TimepixHit* hit = (*coll)[i];
        pixels[hit->GetX() + 256*hit->GetY()] += hit->GetE();
	}

	int nTrackCandidates = count_if(pixels.begin(), pixels.end(), 
                                    [](std::pair<G4int, G4double> p){return p.second > 70 * keV;});
    		

    if (nTrackCandidates != 1) return;
    if (pixels.size() < 2) return;
    
    G4int track_pixel = find_if(pixels.begin(), pixels.end(), 
                                    [](std::pair<G4int, G4double> p){return p.second > 70 * keV;}) -> first;
    
    G4int track_x = track_pixel % 256;
    G4int track_y = track_pixel / 256;
                                    
    for (auto it : pixels) {
        if (it.second > 70 * keV) continue;
        if (it.second == 0) continue;
        
        G4int photon_x = it.first % 256;
        G4int photon_y = it.first / 256;
        
        RM -> FillTree(photon_x - track_x, photon_y - track_y, it.second / keV);
    }

}

	


