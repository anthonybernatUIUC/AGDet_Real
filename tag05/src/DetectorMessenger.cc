
// #include "G4UIcommand.hh"
// #include "DetectorMessenger.hh"

// DetectorMessenger::DetectorMessenger(DetectorConstruction* detector) : fDetector(detector) {
    
//     // fSetTargetWidthMult = new G4UIcommand("/detector/setWidth", this);
//     // fSetTargetWidthMult->SetGuidance("Set the width multiplier of the Si detector.");
    
//     // G4UIparameter* w = new G4UIparameter("Target Width", 'd', false);
//     // w->SetDefaultValue(1.0);
//     // fSetTargetWidthMult->SetParameter(w);

//     fMessenger = new G4GenericMessenger(fDetector, "/target/", "Commands for the target");
//     fMessenger->DeclareMethod("setWidth", &DetectorConstruction::ScaleTargetWidth, "Set the width of target");
//     fMessenger->DeclareMethod("setShellType", &DetectorConstruction::SetShellType, "Set the type of shell");
    
// }

// DetectorMessenger::~DetectorMessenger() {
//     delete fMessenger;
// }

// void DetectorMessenger::SetTargetWidthMult(G4UIcommand* command, G4String newValue) {
    
//     if (command == fSetTargetWidthMult) {
//         G4double w = std::stod(newValue);
//         fDetector->ScaleTargetWidth(w);
//     }
// }






