#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4GDMLParser.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "FTFP_BERT.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"

// Coerced gpt into making this file to test stuff quickly

class GDMLDetectorConstruction : public G4VUserDetectorConstruction {
    public:
        explicit GDMLDetectorConstruction(const G4String& gdmlFile) {
            parser = new G4GDMLParser();
            parser->Read(gdmlFile);
        }

        virtual G4VPhysicalVolume* Construct() override {
            G4LogicalVolumeStore* const store = G4LogicalVolumeStore::GetInstance();
            G4PhysicalVolumeStore* const pvStore = G4PhysicalVolumeStore::GetInstance();

            for (const auto& lv : *store) {
                std::cout << "Logical Volume: " << lv->GetName() << std::endl;
            } 

            for (const auto& pv : *pvStore) {
                std::cout << "Physical Volume: " << pv->GetName() << std::endl;
            }

            // G4LogicalVolume* AlCap = G4LogicalVolumeStore::GetInstance()->GetVolume("AlCap");
            // AlCap->SetVisAttributes(G4VisAttributes(G4Color::Gray())); // semi-transparent gray: G4Color(.5, .5, .5, .85)
            // AddLocalAxes(AlCap, 10.0*cm, 0.1*cm);

            // G4LogicalVolume* CuWrap = G4LogicalVolumeStore::GetInstance()->GetVolume("CuWrap");
            // CuWrap->SetVisAttributes(G4VisAttributes(G4Color::Brown()));

            G4LogicalVolume* SiDetAperture = G4LogicalVolumeStore::GetInstance()->GetVolume("SiDetAperture");
            SiDetAperture->SetVisAttributes(G4VisAttributes(G4Color::Red()));
            AddLocalAxes(SiDetAperture, 5*cm, 0.025*cm);

            G4LogicalVolume* SiCollimator = G4LogicalVolumeStore::GetInstance()->GetVolume("SiCollimator");
            SiCollimator->SetVisAttributes(G4VisAttributes(G4Color::Green()));

            G4LogicalVolume* GeMount = G4LogicalVolumeStore::GetInstance()->GetVolume("HPGeMount");

            G4LogicalVolume* CarbonWindow = G4LogicalVolumeStore::GetInstance()->GetVolume("CarbonWindow");
            CarbonWindow->SetVisAttributes(G4VisAttributes(G4Color(.663, .663, .663, .85)));

            auto nist = G4NistManager::Instance();
            auto air = nist->FindOrBuildMaterial("G4_AIR");

            auto point = new G4Sphere("Point", 0, 0.04*cm, 0., 360.*deg, 0., 180.*deg);
            auto pointLV1 = new G4LogicalVolume(point, air, "PointLV");
            pointLV1->SetVisAttributes(G4VisAttributes(G4Color::Cyan())); // MR
            new G4PVPlacement(nullptr, G4ThreeVector(9.025*mm, 0, 0), pointLV1, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);
            // new G4PVPlacement(nullptr, G4ThreeVector(9.351*mm, 0, 0.140*mm), pointLV1, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);
            
            auto pointLV2 = new G4LogicalVolume(point, air, "PointLV");
            pointLV2->SetVisAttributes(G4VisAttributes(G4Color::Green())); // TL
            new G4PVPlacement(nullptr, G4ThreeVector(-4.512*mm, 0, -7.815*mm), pointLV2, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);
            // new G4PVPlacement(nullptr, G4ThreeVector(-5.385*mm, 0, -6.070*mm), pointLV2, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);
            
            auto pointLV3 = new G4LogicalVolume(point, air, "PointLV");
            pointLV3->SetVisAttributes(G4VisAttributes(G4Color::Blue())); // BL
            new G4PVPlacement(nullptr, G4ThreeVector(-4.512*mm, 0, 7.815*mm), pointLV3, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);
            // new G4PVPlacement(nullptr, G4ThreeVector(-4.1*mm, 0, 7.939*mm), pointLV3, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);

            auto pointLV4 = new G4LogicalVolume(point, air, "PointLV");
            pointLV4->SetVisAttributes(G4VisAttributes(G4Color::Red())); // ML
            new G4PVPlacement(nullptr, G4ThreeVector(-14.464*mm, 0, 0), pointLV4, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);
            // new G4PVPlacement(nullptr, G4ThreeVector(-14.365*mm, 0, -0.203*mm), pointLV4, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);

            auto pointLV5 = new G4LogicalVolume(point, air, "PointLV");
            pointLV5->SetVisAttributes(G4VisAttributes(G4Color::Magenta())); // TR
            new G4PVPlacement(nullptr, G4ThreeVector(7.247*mm, 0, -12.552*mm), pointLV5, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);
            // new G4PVPlacement(nullptr, G4ThreeVector(3.343*mm, 0, -12.072*mm), pointLV5, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);

            auto pointLV6 = new G4LogicalVolume(point, air, "PointLV");
            pointLV6->SetVisAttributes(G4VisAttributes(G4Color::Yellow())); // BR
            new G4PVPlacement(nullptr, G4ThreeVector(7.247*mm, 0, 12.552*mm), pointLV6, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);
            // new G4PVPlacement(nullptr, G4ThreeVector(8.321*mm, 0, 12.449*mm), pointLV6, "PointPV", parser->GetVolume("worldVOL"), false, 0, false);

            G4RotationMatrix* rotX90 = new G4RotationMatrix();
            rotX90->rotateX(90.*deg);
            auto xzPlane = new G4Tubs("XZPlane", 0, 10*cm, 0.01*cm, 0., 360.*deg);
            auto xzLV = new G4LogicalVolume(xzPlane, G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR"), "XZPlaneLV");
            new G4PVPlacement(rotX90, G4ThreeVector(0, 0, 0), xzLV, "XZPlanePV", parser->GetVolume("worldVOL"), false, 0, false);

            return parser->GetWorldVolume();
        }

        void AddLocalAxes(G4LogicalVolume* parentLV, G4double length, G4double radius) {
            
            if (!parentLV) return;

            auto nist = G4NistManager::Instance();
            auto mat = nist->FindOrBuildMaterial("G4_AIR");

            // Cylinder aligned along Z by default → we'll rotate for X/Y
            auto axisSolid = new G4Tubs("AxisSolid", 0, radius, length/2., 0., 360.*deg);
            auto axisSolidZ = new G4Tubs("AxisSolid", 0, radius/2, 202*mm/2, 0., 360.*deg);

            // --- Z axis ---
            auto zLV = new G4LogicalVolume(axisSolidZ, mat, "ZAxisLV");
            auto zVis = new G4VisAttributes(G4Colour::Blue());
            zVis->SetForceSolid(true);
            zLV->SetVisAttributes(zVis);

            new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -210*mm/2), zLV, "ZAxisPV", parentLV, false, 0, false);

            // --- X axis ---
            auto xLV = new G4LogicalVolume(axisSolid, mat, "XAxisLV");
            auto xVis = new G4VisAttributes(G4Colour::Red());
            xVis->SetForceSolid(true);
            xLV->SetVisAttributes(xVis);

            auto rotX = new G4RotationMatrix();
            rotX->rotateY(90.*deg);  // Z -> X

            new G4PVPlacement(rotX, G4ThreeVector(length/2., 0, 0), xLV, "XAxisPV", parentLV, false, 0, false);

            // --- Y axis ---
            auto yLV = new G4LogicalVolume(axisSolid, mat, "YAxisLV");
            auto yVis = new G4VisAttributes(G4Colour::Green());
            yVis->SetForceSolid(true);
            yLV->SetVisAttributes(yVis);

            auto rotY = new G4RotationMatrix();
            rotY->rotateX(-90.*deg); // Z -> Y

            new G4PVPlacement(rotY, G4ThreeVector(0, length/2., 0), yLV, "YAxisPV", parentLV, false, 0, false);
        }

    private:
        G4GDMLParser* parser;
};

int main(int argc, char** argv) {
    G4UIExecutive* ui = nullptr;
    if (argc == 1 || argc == 2) {
        ui = new G4UIExecutive(argc, argv);
    }

    auto runManager = new G4RunManager();

    G4String gdmlFile = "../TheRebirthGDMLMain.gdml";
    // G4String gdmlFile = "../Rebirth3.gdml";
    if (argc > 1) { 
        std::cout << "Using GDML file: " << argv[1] << std::endl;
        gdmlFile = argv[1];
    }
    runManager->SetUserInitialization(new GDMLDetectorConstruction(gdmlFile));
    runManager->SetUserInitialization(new FTFP_BERT(0));

    auto visManager = new G4VisExecutive();
    visManager->Initialize();
    runManager->Initialize();

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
