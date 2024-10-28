#pragma once

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

class MySiDet : public G4VSensitiveDetector {

    public:
        MySiDet(G4String);
        ~MySiDet();
    
    private:
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

};

class MyGeDet : public G4VSensitiveDetector {

    public:
        MyGeDet(G4String);
        ~MyGeDet();
    
    private:
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

};