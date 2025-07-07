#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4GDMLParser.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "FTFP_BERT.hh"
#include "G4LogicalVolumeStore.hh"

// Coerced gpt into making this file to test stuff quickly

class GDMLDetectorConstruction : public G4VUserDetectorConstruction {
public:
    explicit GDMLDetectorConstruction(const G4String& gdmlFile) {
        parser.Read(gdmlFile);
    }

    virtual G4VPhysicalVolume* Construct() override {
        G4LogicalVolumeStore* const store = G4LogicalVolumeStore::GetInstance();
        G4LogicalVolume* AlCap = G4LogicalVolumeStore::GetInstance()->GetVolume("AlCap");
        AlCap->SetVisAttributes(G4VisAttributes(G4Color::Gray()));

        G4LogicalVolume* CuWrap = G4LogicalVolumeStore::GetInstance()->GetVolume("CuWrap");
        CuWrap->SetVisAttributes(G4VisAttributes(G4Color::Brown()));

        G4LogicalVolume* SiDetAperture = G4LogicalVolumeStore::GetInstance()->GetVolume("SiDetAperture");
        SiDetAperture->SetVisAttributes(G4VisAttributes(G4Color::Green()));

        G4LogicalVolume* SiCollimator = G4LogicalVolumeStore::GetInstance()->GetVolume("SiCollimator");
        SiCollimator->SetVisAttributes(G4VisAttributes(G4Color::Green()));

        G4LogicalVolume* GeMount = G4LogicalVolumeStore::GetInstance()->GetVolume("HPGeMount");

        G4LogicalVolume* CarbonWindow = G4LogicalVolumeStore::GetInstance()->GetVolume("CarbonWindow");
        CarbonWindow->SetVisAttributes(G4VisAttributes(G4Color(.663, .663, .663, .85)));

        return parser.GetWorldVolume();
    }

private:
    G4GDMLParser parser;
};

int main(int argc, char** argv) {
    // Optional interactive UI
    G4UIExecutive* ui = nullptr;
    if (argc == 1 || argc == 2) {
        ui = new G4UIExecutive(argc, argv);
    }

    // Run manager
    auto runManager = new G4RunManager();

    // GDML detector construction
    G4String gdmlFile = "../TheRebirthGDMLMain.gdml";
    if (argc > 1) { 
        std::cout << "Using GDML file: " << argv[1] << std::endl;
        gdmlFile = argv[1];
    }
    runManager->SetUserInitialization(new GDMLDetectorConstruction(gdmlFile));
    runManager->SetUserInitialization(new FTFP_BERT(0));

    // Visualization manager
    auto visManager = new G4VisExecutive("quiet");
    visManager->Initialize();

    // Initialize kernel
    runManager->Initialize();

    // UI manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
    return 0;
}
