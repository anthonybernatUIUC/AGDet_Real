#include "stepping.hh"

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
	G4double edep = step->GetTotalEnergyDeposit();
	// G4cout << "HELLOOOOOOOOOOO" << G4endl;
	
	if (volume == fSiScoringVolume) {
		fEventAction->AddEdep(edep);
	}
	
	// if (volume == fGeScoringVolume) {
	// 	if (part == G4Alpha::Alpha()) {
	// 		step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
	// 	}
	// 	if (part == G4Gamma::Gamma()) {
	// 		fEventAction->AddEdepGe(edep);
	// 		fEventAction->AddEdepGamma(edep);
	// 	}
	// 	if (part == G4Electron::Definition()) {
	// 		fEventAction->AddEdepGe(edep);
	// 		fEventAction->AddEdepGeElec(edep);
	// 	}
	// }
	// if (volume == fSiScoringVolume) {
	// 	if (part == G4Gamma::Gamma()) {
	// 		step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
	// 	}
	// 	if (part == G4Alpha::Alpha()) {
	// 		fEventAction->AddEdepAlpha(edep);
	// 		fEventAction->AddEdepSi(edep);
	// 		// G4cout << "Alpha Energy: " << edep << G4endl;
	// 	}
	// 	if (part == G4Electron::Definition()) {
	// 		fEventAction->AddEdepSiElec(edep);
	// 		fEventAction->AddEdepSi(edep);
	// 	}
	// }
	// if (part->GetParticleName() == "Li7") {
	// 	G4double edep = step->GetTrack()->GetVertexKineticEnergy();
	// 	fEventAction->AddEdepLi7(edep);
	// }
	// if (part == G4Alpha::Alpha()) {
	// 	G4double edep = step->GetTrack()->GetVertexKineticEnergy();
	// 	fEventAction->AddEdepAlpha(edep);
	// }
}


