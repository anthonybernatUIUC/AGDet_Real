#include "detector.hh"

MySiDet::MySiDet(G4String name) : G4VSensitiveDetector(name) {}

MySiDet::~MySiDet() {}

G4bool MySiDet::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) {
    // G4Track* track = aStep->GetTrack();
    // track->SetTrackStatus(fStopAndKill);

    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4ThreeVector posAlpha = preStepPoint->GetPosition();
    G4ThreeVector momAlpha = preStepPoint->GetMomentum();

    const G4VTouchable* touchable = preStepPoint->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();

    // Position of hit detector
    G4ThreeVector posDetector = touchable->GetVolume()->GetTranslation();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(1, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
    man->FillNtupleDColumn(1, 1, posAlpha[0]);
    man->FillNtupleDColumn(1, 2, posAlpha[1]);
    man->FillNtupleDColumn(1, 3, posAlpha[2]);
    man->FillNtupleDColumn(1, 4, aStep->GetTotalEnergyDeposit());
    man->AddNtupleRow(1);

    return true;
}