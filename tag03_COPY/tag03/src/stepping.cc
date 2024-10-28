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

	G4LogicalVolume* fGeScoringVolume = detectorConstruction->GetGeScoringVolume();
	G4LogicalVolume* fSiScoringVolume = detectorConstruction->GetSiScoringVolume();
	G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4ParticleDefinition* part = step->GetTrack()->GetDefinition();
	G4double edep = step->GetTotalEnergyDeposit();
	
	if (volume == fGeScoringVolume) {
		if (part == G4Alpha::Alpha()) {
			step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
		}
		if (part == G4Gamma::Gamma()) {
			fEventAction->AddEdepGe(edep);
			fEventAction->AddEdepGamma(edep);
		}
		if (part == G4Electron::Definition()) {
			fEventAction->AddEdepGe(edep);
			fEventAction->AddEdepGeElec(edep);
		}
	}
	if (volume == fSiScoringVolume) {
		if (part == G4Gamma::Gamma()) {
			step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
		}
		if (part == G4Alpha::Alpha()) {
			fEventAction->AddEdepAlpha(edep);
			fEventAction->AddEdepSi(edep);
		}
		if (part == G4Electron::Definition()) {
			fEventAction->AddEdepSiElec(edep);
			fEventAction->AddEdepSi(edep);
		}
	}
	if (part->GetParticleName() == "Li7") {
		G4double edep = step->GetTrack()->GetVertexKineticEnergy();
		fEventAction->AddEdepLi7(edep);
	}
}


