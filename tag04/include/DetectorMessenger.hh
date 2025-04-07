#pragma once

#include "DetectorConstruction.hh"
#include "G4GenericMessenger.hh"
#include "G4UImessenger.hh"
#include "EventAction.hh"   
#include "G4UIparameter.hh"
#include "G4UIcommand.hh"

class DetectorMessenger {
    public:
        DetectorMessenger(DetectorConstruction* detector);
        ~DetectorMessenger();
        void SetTargetWidthMult(G4UIcommand* command, G4String newValue);

    private:
        DetectorConstruction* fDetector;
        G4UIcommand* fSetTargetWidthMult;
        G4GenericMessenger* fMessenger;

};
