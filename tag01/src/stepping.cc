#include "stepping.hh"
#include "G4RunManager.hh"
#include "G4Gamma.hh"
#include "G4ParticleTypes.hh"
#include "G4IonTable.hh"

MySteppingAction::MySteppingAction(EventAction *eventAction) {
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step *step) {
	
	G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

	G4ParticleDefinition *part = step->GetTrack()->GetDefinition();
	G4int pID = step->GetTrack()->GetParentID();

	const DetectorConstruction *detectorConstruction = static_cast<const DetectorConstruction*>(
		G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	G4LogicalVolume *fGeScoringVolume = detectorConstruction->GetGeScoringVolume();
	G4LogicalVolume *fSiScoringVolume = detectorConstruction->GetSiScoringVolume();

	G4cout << volume << G4endl;

	// Add deposited energy at each step within the detector
	if (volume == fGeScoringVolume && part == G4Gamma::Gamma()) {	
		fEventAction->IncidentIndicator();
		G4double edep = step->GetTotalEnergyDeposit();
		G4cout << "Gamma edep: " << edep << G4endl;
		fEventAction->AddEdepGe(edep);
	}
	if (volume == fSiScoringVolume && part == G4Alpha::Alpha()) {
		G4double edep = step->GetTotalEnergyDeposit();
		G4cout << "Alpha edep: " << edep << G4endl;
		fEventAction->AddEdepSi(edep);
	}
}


