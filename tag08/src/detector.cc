#include "detector.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "G4EventManager.hh"


MySiDet::MySiDet(G4String name) : G4VSensitiveDetector(name) {}
MySiDet::MySiDet(G4String name, G4int firstSiDetIdx) : G4VSensitiveDetector(name), firstSiDetIdx(firstSiDetIdx) {}
MyGeDet::MyGeDet(G4String name) : G4VSensitiveDetector(name) {}

MySiDet::~MySiDet() {}
MyGeDet::~MyGeDet() {}

    // AddEdep indices to populate ROOT file
    // 0: GeScoring
    // 1: SiScoring
    // 2: Gammas
    // 3: Alphas
    // 4: Li7E
    // 5: Si e-
    // 6: Ge e- (No Background)
    // 7: Si Background
    // 8: Ge Background

    // 9:  SiDet1
    // 10: SiDet2
    // 11: SiDet3
    // 12: SiDet4
    // 13: SiDet5
    // 14: SiDet6

    // 15: GeDet1
    // 16: GeDet2
    // 17: GeDet3
    // 18: GeDet4
    // 19: GeDet5
    // 20: GeDet6

G4bool MySiDet::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) {

    auto* runManager = G4RunManager::GetRunManager();
	auto* detectorConstruction = static_cast<const DetectorConstruction*>(runManager->GetUserDetectorConstruction());
    auto *fEventAction = static_cast<EventAction*>(G4EventManager::GetEventManager()->GetUserEventAction());
    
    G4Track* track = aStep->GetTrack();
    G4ParticleDefinition* part = track->GetDefinition();
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4int copyNo = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();
    G4int tupleNum = copyNo + 9;

    if (part == G4Gamma::Gamma()) {
		aStep->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
	} else if (part->GetParticleName() == "Li7") {
			fEventAction->AddEdep(edep, 4); // Li7E
            fEventAction->AddEdep(edep, 1); // SiScoring
            fEventAction->AddEdep(edep, tupleNum); // SiDet[1-6]
	} else if (part == G4Alpha::Alpha()) {
        // Alphas position/momentum
        G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
        G4ThreeVector posAlpha = preStepPoint->GetPosition();

        // Alphas hits and positions
        const G4VTouchable* touchable = preStepPoint->GetTouchable();
        G4ThreeVector posDetector = touchable->GetVolume()->GetTranslation();
        G4int copyNo = touchable->GetVolume()->GetCopyNo(); 
        G4AnalysisManager* man = G4AnalysisManager::Instance();

        fEventAction->AddEdep(edep, 1); // SiScoring
        fEventAction->AddEdep(edep, 3); // Alphas

        // SiDets are tuple numbers 9-15
        fEventAction->AddEdep(edep, tupleNum); // SiDet[1-6]
        // man->FillNtupleIColumn(tupleNum, 1, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        // man->FillNtupleDColumn(tupleNum, 2, posAlpha[0]);
        // man->FillNtupleDColumn(tupleNum, 3, posAlpha[1]);
        // man->FillNtupleDColumn(tupleNum, 4, posAlpha[2]); have to fill these in EventAction

    } else if (aStep->GetTrack()->GetDefinition() == G4Electron::Definition()) {
        fEventAction->AddEdep(edep, 1); // SiScoring
        fEventAction->AddEdep(edep, 5); // Si e-
		fEventAction->AddEdep(edep, 7); // Si Background
        fEventAction->AddEdep(edep, tupleNum); // SiDet[1-6]
    } else {
        fEventAction->AddEdep(edep, 1); // SiScoring
        fEventAction->AddEdep(edep, 7); // Si Background
        fEventAction->AddEdep(edep, tupleNum); // SiDet[1-6]
    }

    return true;
}

G4bool MyGeDet::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) {

    auto* runManager = G4RunManager::GetRunManager();
	auto* detectorConstruction = static_cast<const DetectorConstruction*>(runManager->GetUserDetectorConstruction());
    auto *fEventAction = static_cast<EventAction*>(G4EventManager::GetEventManager()->GetUserEventAction());

    G4Track* track = aStep->GetTrack();
	G4ParticleDefinition* part = track->GetDefinition();
	G4double edep = aStep->GetTotalEnergyDeposit();
    G4int cpyNo = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetCopyNo();

    if (part == G4Alpha::Alpha() || part->GetParticleName() == "Li7") {
        aStep->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
        return true;
    }

    if (part == G4Gamma::Gamma()) {
    
        // Photon positions
        G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
        G4ThreeVector posPhoton = preStepPoint->GetPosition();
        G4AnalysisManager* man = G4AnalysisManager::Instance();
		fEventAction->AddEdep(edep, 2); // Gammas
        
    } else if (part == G4Electron::Definition()) {
        fEventAction->AddEdep(edep, 6); // Ge e- (No Background)
    } 

    fEventAction->AddEdep(edep, 0); // GeScoring
    fEventAction->AddEdep(edep, cpyNo + 15); // GeDet[1-6]
    
    return true;
}