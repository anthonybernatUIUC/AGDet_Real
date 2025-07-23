#include "detector.hh"

MySiDet::MySiDet(G4String name) : G4VSensitiveDetector(name) {}

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
        // G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() << std::endl;

        G4AnalysisManager* man = G4AnalysisManager::Instance();
        man->FillNtupleIColumn(3, 1, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        man->FillNtupleDColumn(3, 2, posAlpha[0]);
        man->FillNtupleDColumn(3, 3, posAlpha[1]);
        man->FillNtupleDColumn(3, 4, posAlpha[2]);
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