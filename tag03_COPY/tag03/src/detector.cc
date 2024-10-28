#include "detector.hh"

MySiDet::MySiDet(G4String name) : G4VSensitiveDetector(name) {}

MyGeDet::MyGeDet(G4String name) : G4VSensitiveDetector(name) {}

MySiDet::~MySiDet() {}

MyGeDet::~MyGeDet() {}

G4bool MySiDet::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) {
    G4Track* track = aStep->GetTrack();
    // track->SetTrackStatus(fStopAndKill);

    // Alphas position/momentum
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
    G4ThreeVector posAlpha = preStepPoint->GetPosition();
    G4ThreeVector momAlpha = preStepPoint->GetMomentum();

    // Detector hit by alpha
    const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();

    // Position of hit detector
    G4VPhysicalVolume* physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(3, 0, evt);
    man->FillNtupleDColumn(3, 1, posAlpha[0]);
    man->FillNtupleDColumn(3, 2, posAlpha[1]);
    man->FillNtupleDColumn(3, 3, posAlpha[2]);
    man->AddNtupleRow(3);

    return true;
}

G4bool MyGeDet::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) {
    // Kill photons immediately upon reaching front of detector
    G4Track* track = aStep->GetTrack();
    // track->SetTrackStatus(fStopAndKill);

    // Photon positions/momentum
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    // Detector hit by photon
    const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();

    // Position of hit detector
    G4VPhysicalVolume* physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(2, 0, evt);
    man->FillNtupleDColumn(2, 1, posPhoton[0]);
    man->FillNtupleDColumn(2, 2, posPhoton[1]);
    man->FillNtupleDColumn(2, 3, posPhoton[2]);
    man->AddNtupleRow(2);

    return true;
}