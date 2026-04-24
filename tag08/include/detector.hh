#pragma once

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Gamma.hh"
#include "G4ParticleTypes.hh"

#include <set>

class DetectorConstruction; 
class EventAction;

class MySiDet : public G4VSensitiveDetector {

    public:
        MySiDet(G4String);
        MySiDet(G4String, G4int firstSiDetIdx);
        ~MySiDet();
    
    private:
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        G4int firstSiDetIdx;
        G4RunManager* runManager;
		const DetectorConstruction* detectorConstruction;
        EventAction* fEventAction;
		std::set<G4LogicalVolume*> fGeScoringVolumes;
		std::set<G4LogicalVolume*> fSiScoringVolumes;

};

class MyGeDet : public G4VSensitiveDetector {

    public:
        MyGeDet(G4String);
        ~MyGeDet();
    
    private:
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        G4RunManager* runManager;
		const DetectorConstruction* detectorConstruction;
        EventAction* fEventAction;
		std::set<G4LogicalVolume*> fGeScoringVolumes;
		std::set<G4LogicalVolume*> fSiScoringVolumes;

};