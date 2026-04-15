#include "detector.hh"

MySiDet::MySiDet(G4String name) : G4VSensitiveDetector(name) {}
MySiDet::MySiDet(G4String name, G4int firstSiDetIdx) : G4VSensitiveDetector(name), firstSiDetIdx(firstSiDetIdx) {}

MyGeDet::MyGeDet(G4String name) : G4VSensitiveDetector(name) {}

MySiDet::~MySiDet() {}

MyGeDet::~MyGeDet() {}

G4bool MySiDet::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) {

    if (aStep->GetTrack()->GetDefinition() == G4Alpha::Alpha()) {
        
        // Alphas position/momentum
        G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
        G4ThreeVector posAlpha = preStepPoint->GetPosition();

        // Detector hit by alpha
        const G4VTouchable* touchable = preStepPoint->GetTouchable();
        G4ThreeVector posDetector = touchable->GetVolume()->GetTranslation();
        G4int copyNo = touchable->GetVolume()->GetCopyNo(); 
        // G4cout << touchable->GetVolume()->GetName() << G4endl;    
        // G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() << std::endl;

        G4AnalysisManager* man = G4AnalysisManager::Instance();
        man->FillNtupleIColumn(3, 1, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        man->FillNtupleDColumn(3, 2, posAlpha[0]);
        man->FillNtupleDColumn(3, 3, posAlpha[1]);
        man->FillNtupleDColumn(3, 4, posAlpha[2]);

        // SiDets are tuple numbers 9-15
        G4int tupleNum = copyNo - firstSiDetIdx + 9;
        man->FillNtupleIColumn(tupleNum, 1, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        man->FillNtupleDColumn(tupleNum, 2, posAlpha[0]);
        man->FillNtupleDColumn(tupleNum, 3, posAlpha[1]);
        man->FillNtupleDColumn(tupleNum, 4, posAlpha[2]);
    }

    return true;
}

G4bool MyGeDet::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) {

    if (aStep->GetTrack()->GetDefinition() == G4Gamma::Gamma()) {
        
        // Photon positions
        G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
        G4ThreeVector posPhoton = preStepPoint->GetPosition();

        // Detector hit by photon and its position
        const G4VTouchable* touchable = preStepPoint->GetTouchable();
        G4ThreeVector posDetector = touchable->GetVolume()->GetTranslation();

        G4AnalysisManager* man = G4AnalysisManager::Instance();
        man->FillNtupleIColumn(2, 1, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        man->FillNtupleDColumn(2, 2, posPhoton[0]);
        man->FillNtupleDColumn(2, 3, posPhoton[1]);
        man->FillNtupleDColumn(2, 4, posPhoton[2]);
    }
    
    return true;
}