#include "stepping.hh"
#include "G4RunManager.hh"
#include "G4Gamma.hh"
#include "G4ParticleTypes.hh"

MySteppingAction::MySteppingAction(EventAction *eventAction) {
	
	fEventAction = eventAction;
	runManager = G4RunManager::GetRunManager();
	detectorConstruction = static_cast<const DetectorConstruction*>(runManager->GetUserDetectorConstruction());
	fGeScoringVolumes = detectorConstruction->GetGeScoringVolumes();
	fSiScoringVolumes = detectorConstruction->GetSiScoringVolumes();
}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::RecordDeposition(const G4Step *step) {

	G4LogicalVolume* volumeHit = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4Track* track = step->GetTrack();
	G4ParticleDefinition* part = track->GetDefinition();
	G4double edep = step->GetTotalEnergyDeposit();
	G4cout << volumeHit->GetName() << G4endl;

	// 0: GeScoring
	// 1: SiScoring
	// 2: Gammas
	// 3: Alphas
	// 4: Li7E
	// 5: Si e-
	// 6: Ge e- (No Background)
	// 7: Si Background
	// 8: Ge Background

	if (fGeScoringVolumes.find(volumeHit) != fGeScoringVolumes.end()) {
		G4cout << "Hit Ge Det with " << part->GetParticleName() << " Edep: " << edep << G4endl;
		if (part == G4Gamma::Gamma()) {
			fEventAction->AddEdep(edep, 0);
			fEventAction->AddEdep(edep, 2);
		} else if (part == G4Electron::Definition()) {
			if (runManager->GetCurrentEvent()->GetEventID() % 2 == 0) {
				fEventAction->AddEdep(edep, 8);
			} else {
				fEventAction->AddEdep(edep, 6);
			}
			fEventAction->AddEdep(edep, 0);
		} else {
			track->SetTrackStatus(fKillTrackAndSecondaries);
		}
	} else if (fSiScoringVolumes.find(volumeHit) != fSiScoringVolumes.end()) {	
		G4cout << "Hit Si Det with " << part->GetParticleName() << " Edep: " << edep << G4endl;	
		if (part->GetParticleName() == "Li7") {
			fEventAction->AddEdep(edep, 4);
		} else if (part == G4Alpha::Alpha()) {
			fEventAction->AddEdep(edep, 3);
		} else if (part == G4Electron::Definition()) {
			fEventAction->AddEdep(edep, 5);
			fEventAction->AddEdep(edep, 7);
		} else {
			fEventAction->AddEdep(edep, 7);
		}
		fEventAction->AddEdep(edep, 1);
	}
}

void MySteppingAction::UserSteppingAction(const G4Step *step) {
	RecordDeposition(step);
}






// old method
	
	// if (fGeScoringVolumes.find(volumeHit) != fGeScoringVolumes.end()) {
	// 	if (part == G4Alpha::Alpha() || part->GetParticleName() == "Li7") {
	// 		step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
	// 	} else if (part == G4Gamma::Gamma()) {
	// 		fEventAction->AddEdepGe(edep);
	// 		fEventAction->AddEdepGamma(edep);
	// 	} else if (part == G4Electron::Definition()) {
	// 		fEventAction->AddEdepGe(edep);
	// 		fEventAction->AddEdepGeElec(edep);
	// 		// G4cout << "Step Edep with Parent: " << edep << ", " << step->GetTrack()->GetParentID() << G4endl;
	// 	} else {
	// 		// G4cout << part->GetParticleName() << " has hit a Ge det" << G4endl;
	// 		step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
	// 	}
	// } else if (fSiScoringVolumes.find(volumeHit) != fSiScoringVolumes.end()) {		
	// 	if (part == G4Gamma::Gamma()) {
	// 		// fEventAction->AddEdepSiBackground(edep);
	// 		// step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
	// 	} else if (part->GetParticleName() == "Li7") {
	// 		fEventAction->AddEdepLi7(edep);
	// 		// G4cout << "Li7 Edep at Event: " << edep << " " << 
	// 		// G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() << G4endl;
	// 	} else if (part == G4Alpha::Alpha()) {
	// 		fEventAction->AddEdepAlpha(edep);
	// 	} else if (part == G4Electron::Definition()) {
	// 		fEventAction->AddEdepSiElec(edep);
	// 		fEventAction->AddEdepSiBackground(edep);
	// 	} else {
	// 		// G4cout << part->GetParticleName() << " has hit a Si Det with E: " << edep << G4endl;
	// 		// step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
	// 		fEventAction->AddEdepSiBackground(edep);
	// 	}
	// 	fEventAction->AddEdepSi(edep);
	// }


