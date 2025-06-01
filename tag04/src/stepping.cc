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

	std::set<G4LogicalVolume*> fGeScoringVolumes = detectorConstruction->GetGeScoringVolumes();
	std::set<G4LogicalVolume*> fSiScoringVolumes = detectorConstruction->GetSiScoringVolumes();
	G4LogicalVolume* volumeHit = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4ParticleDefinition* part = step->GetTrack()->GetDefinition();
	G4double edep = step->GetTotalEnergyDeposit();
	
	if (fGeScoringVolumes.find(volumeHit) != fGeScoringVolumes.end()) {
		if (part == G4Alpha::Alpha() || part->GetParticleName() == "Li7") {
			step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
		} else if (part == G4Gamma::Gamma()) {
			fEventAction->AddEdepGe(edep);
			fEventAction->AddEdepGamma(edep);
		} else if (part == G4Electron::Definition()) {
			fEventAction->AddEdepGe(edep);
			fEventAction->AddEdepGeElec(edep);
		} else {
			std::cout << part->GetParticleName() << " has hit a Ge det" << std::endl;
		}
	} else if (fSiScoringVolumes.find(volumeHit) != fSiScoringVolumes.end()) {		
		if (part == G4Gamma::Gamma()) {
			step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
		} else if (part->GetParticleName() == "Li7") {
			fEventAction->AddEdepLi7(edep);
			fEventAction->AddEdepSi(edep);
		} else if (part == G4Alpha::Alpha()) {
			fEventAction->AddEdepAlpha(edep);
			fEventAction->AddEdepSi(edep);
		} else if (part == G4Electron::Definition()) {
			fEventAction->AddEdepSiElec(edep);
			fEventAction->AddEdepSi(edep);
		} else {
			std::cout << part->GetParticleName() << " has hit a Si Det" << std::endl;
		}
	}

	// if (part->GetParticleName() == "Li7") {
	// 	// G4cout << volume->GetName();
	// 	fEventAction->GetLi7Set().insert(volume->GetName());
	// 	G4double edep = step->GetTrack()->GetVertexKineticEnergy();
	// 	fEventAction->AddEdepLi7(edep);
	// }

}


