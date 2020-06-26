#ifndef TimepixHit_h
#define TimepixHit_h 1

#include "G4THitsCollection.hh"
#include "globals.hh"
#include  "G4VHit.hh"


//A class inherited from default to make G4SensitiveDetector work

class TimepixHit : public G4VHit
{
	public:
	TimepixHit();
	~TimepixHit();

	void Print();
	void Draw();

	void AddX (G4int);
	void AddY (G4int);
	void AddE (G4double);
	void AddK (G4double);
    void AddId (G4int);
	G4int GetX();
	G4int GetY();
	G4double GetE();
	G4double GetK();
    G4double GetId();
    


	private:
	G4double edep, ekin;	
    G4int event_id, pos_x, pos_y;
};



#endif

typedef G4THitsCollection <TimepixHit> TimepixHitCollection;
