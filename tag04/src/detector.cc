#include "detector.hh"

MySiDet::MySiDet(G4String name) : G4VSensitiveDetector(name) {}

MyGeDet::MyGeDet(G4String name) : G4VSensitiveDetector(name) {}

MySiDet::~MySiDet() {}

MyGeDet::~MyGeDet() {}

G4bool MySiDet::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) {

    // std::cout << aStep->GetTrack()->GetDefinition()->GetParticleName();

    // Alphas position/momentum
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4ThreeVector posAlpha = preStepPoint->GetPosition();
    G4ThreeVector momAlpha = preStepPoint->GetMomentum();

    // Detector hit by alpha
    const G4VTouchable* touchable = preStepPoint->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();

    // Position of hit detector
    G4ThreeVector posDetector = touchable->GetVolume()->GetTranslation();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(3, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
    man->FillNtupleDColumn(3, 1, posAlpha[0]);
    man->FillNtupleDColumn(3, 2, posAlpha[1]);
    man->FillNtupleDColumn(3, 3, posAlpha[2]);
    man->AddNtupleRow(3);

    return true;
}

G4bool MyGeDet::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) {

    // Photon positions/momentum
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    // Detector hit by photon
    const G4VTouchable* touchable = preStepPoint->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();

    // Position of hit detector
    G4ThreeVector posDetector = touchable->GetVolume()->GetTranslation();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(2, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
    man->FillNtupleDColumn(2, 1, posPhoton[0]);
    man->FillNtupleDColumn(2, 2, posPhoton[1]);
    man->FillNtupleDColumn(2, 3, posPhoton[2]);
    // man->AddNtupleRow(2);

    return true;
}