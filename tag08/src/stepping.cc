#include "stepping.hh"


MySteppingAction::MySteppingAction(EventAction *eventAction) {
	
	fEventAction = eventAction;
	runManager = G4RunManager::GetRunManager();
	detectorConstruction = static_cast<const DetectorConstruction*>(runManager->GetUserDetectorConstruction());
}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::RecordDeposition(const G4Step *step) {

	fGeScoringVolumes = detectorConstruction->GetGeScoringVolumes(); // do not put these in constructor. reason for breaking code: unknown. well i mean i kinda know insofar as it makes the if statements not work but i dont know why the memory locations would be different
	fSiScoringVolumes = detectorConstruction->GetSiScoringVolumes();

	G4LogicalVolume* volumeHit = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	G4Track* track = step->GetTrack();
	G4ParticleDefinition* part = track->GetDefinition();
	G4double edep = step->GetTotalEnergyDeposit();
	G4int copyNo = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();
	// if (part != G4Neutron::Neutron()) G4cout << volumeHit->GetName() << " | " << part->GetParticleName() << " | " << edep << G4endl;

	// 0: GeScoring
	// 1: SiScoring
	// 2: Gammas
	// 3: Alphas
	// 4: Li7E
	// 5: Si e-
	// 6: Ge e- (No Background)
	// 7: Si Background
	// 8: Ge Background

	// 9: Si Det 1
	// 10: Si Det 2
	// 11: Si Det 3
	// 12: Si Det 4
	// 13: Si Det 5
	// 14: Si Det 6

	// 15: Ge Det 1
	// 16: Ge Det 2
	// 17: Ge Det 3
	// 18: Ge Det 4
	// 19: Ge Det 5
	// 20: Ge Det 6

	if (fGeScoringVolumes.find(volumeHit) != fGeScoringVolumes.end()) {
		if (part == G4Gamma::Gamma()) {
			// G4cout << "Ge Scoring Volume Hit: " << volumeHit->GetName() << " | " << part->GetParticleName() << " | " << edep << G4endl;
			fEventAction->AddEdep(edep, 0);
			fEventAction->AddEdep(edep, 2);
		} else if (part == G4Electron::Definition()) {
			fEventAction->AddEdep(edep, 0);
			fEventAction->AddEdep(edep, 6);
		} else {
			track->SetTrackStatus(fKillTrackAndSecondaries);
		}
	} 

	if (fSiScoringVolumes.find(volumeHit) != fSiScoringVolumes.end()) {		
		if (part == G4Gamma::Gamma()) {
			step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
		} else if (part->GetParticleName() == "Li7") {
			fEventAction->AddEdep(edep, 4); // Li7E
		} else if (part == G4Alpha::Alpha()) {
			G4cout << "Si Scoring Volume Hit: " << volumeHit->GetName() << " | " << part->GetParticleName() << " | " << edep << G4endl;
			fEventAction->AddEdep(edep, 3);
			fEventAction->AddEdep(edep, copyNo - detectorConstruction->GetFirstSiDetIdx() + 9);
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
	// RecordDeposition(step);
}

