#include "stepping.hh"
#include "G4RunManager.hh"
#include "G4Gamma.hh"
#include "G4ParticleTypes.hh"

MySteppingAction::MySteppingAction(EventAction *eventAction) {
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step *step) {

	const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*>(
		G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	G4LogicalVolume* fSiScoringVolume = detectorConstruction->GetSiScoringVolume();
	G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4ParticleDefinition* part = step->GetTrack()->GetDefinition();
	// G4double edep = step->GetTotalEnergyDeposit();
	G4double edep = step->GetTrack()->GetVertexKineticEnergy();
	
	if (volume == fSiScoringVolume && edep > 0) {
		// G4cout << edep << " ";
		fEventAction->AddEdepSi(edep);
	}
	

}


