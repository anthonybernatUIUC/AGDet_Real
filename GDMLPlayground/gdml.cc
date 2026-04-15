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

class GDMLDetectorConstruction : public G4VUserDetectorConstruction {
    private:
        G4GDMLParser* parser;
    
    public:
        explicit GDMLDetectorConstruction(const G4String& gdmlFile) {
            parser = new G4GDMLParser();
            parser->Read(gdmlFile);
        }

        virtual G4VPhysicalVolume* Construct() override {
            G4LogicalVolumeStore* const store = G4LogicalVolumeStore::GetInstance();
            G4PhysicalVolumeStore* const pvStore = G4PhysicalVolumeStore::GetInstance();

            printVolumeStore(store);
            printPhysicalVolumeStore(pvStore);

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

            // G4LogicalVolume* GeMount = G4LogicalVolumeStore::GetInstance()->GetVolume("HPGeMount");

            // G4LogicalVolume* CarbonWindow = G4LogicalVolumeStore::GetInstance()->GetVolume("CarbonWindow");
            // CarbonWindow->SetVisAttributes(G4VisAttributes(G4Color(.663, .663, .663, .85)));

            ConstructTriangle(parser);
            return parser->GetWorldVolume();
        }

        void printVolumeStore(G4LogicalVolumeStore* const store) {
			for (auto i = store->cbegin(); i != store->cend(); ++i) {
				G4cout << "Logical Volume: " << (*i)->GetName() << G4endl;
			}
		}

		void printPhysicalVolumeStore(G4PhysicalVolumeStore* const store) {
			for (auto i = store->cbegin(); i != store->cend(); ++i) {
				G4cout << "Physical Volume: " << (*i)->GetName() << G4endl;
			}
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

        void ConstructTriangle(G4GDMLParser* fParser) {

            auto man = G4NistManager::Instance();
            auto Galactic = man->FindOrBuildMaterial("G4_Galactic");    
            G4Sphere* point = new G4Sphere("Point", 0, 0.04*cm, 0., 360.*deg, 0., 180.*deg);
            G4LogicalVolume* motherVol = fParser->GetVolume("worldVOL");
            // std::vector<G4ThreeVector> points = {
            // 	G4ThreeVector(9.025, 0, 0)*mm, 
            // 	G4ThreeVector(-4.512, 0, -7.815)*mm, 
            // 	G4ThreeVector(-4.512, 0, 7.815)*mm, 
            // 	G4ThreeVector(-14.464, 0, 0)*mm, 
            // 	G4ThreeVector(7.247, 0, -12.552)*mm,
            // 	G4ThreeVector(7.247, 0, 12.552)*mm 
            // };
            std::vector<G4ThreeVector> points = {
                G4ThreeVector(9.351, 0, 0.140)*mm, 
                G4ThreeVector(-5.385, 0, -6.070)*mm, 
                G4ThreeVector(-4.1, 0, 7.939)*mm, 
                G4ThreeVector(-14.365, 0, -0.203)*mm, 
                G4ThreeVector(3.343, 0, -12.072)*mm,
                G4ThreeVector(8.321, 0, 12.449)*mm 
            };
            // ========== MR ==========
            G4LogicalVolume* pointLV1 = new G4LogicalVolume(point, Galactic, "PointLV");
            pointLV1->SetVisAttributes(G4VisAttributes(G4Color::Cyan())); 
            new G4PVPlacement(nullptr, points[0], pointLV1, "PointPV", motherVol, false, 0, false);
            // ========== TL ==========
            G4LogicalVolume* pointLV2 = new G4LogicalVolume(point, Galactic, "PointLV");
            pointLV2->SetVisAttributes(G4VisAttributes(G4Color::Green())); 
            new G4PVPlacement(nullptr, points[1], pointLV2, "PointPV", motherVol, false, 0, false);
            // ========== BL ==========
            G4LogicalVolume* pointLV3 = new G4LogicalVolume(point, Galactic, "PointLV");
            pointLV3->SetVisAttributes(G4VisAttributes(G4Color::Blue())); 
            new G4PVPlacement(nullptr, points[2], pointLV3, "PointPV", motherVol, false, 0, false);
            // ========== ML ==========
            G4LogicalVolume* pointLV4 = new G4LogicalVolume(point, Galactic, "PointLV");
            pointLV4->SetVisAttributes(G4VisAttributes(G4Color::Red())); 
            new G4PVPlacement(nullptr, points[3], pointLV4, "PointPV", motherVol, false, 0, false);
            // ========== TR ==========
            G4LogicalVolume* pointLV5 = new G4LogicalVolume(point, Galactic, "PointLV");
            pointLV5->SetVisAttributes(G4VisAttributes(G4Color::Magenta()));
            new G4PVPlacement(nullptr, points[4], pointLV5, "PointPV", motherVol, false, 0, false);
            // ========== BR ==========
            G4LogicalVolume* pointLV6 = new G4LogicalVolume(point, Galactic, "PointLV");
            pointLV6->SetVisAttributes(G4VisAttributes(G4Color::Yellow())); 
            new G4PVPlacement(nullptr, points[5], pointLV6, "PointPV", motherVol, false, 0, false);

            G4RotationMatrix* rotX90 = new G4RotationMatrix();
            rotX90->rotateX(90*deg);
            G4Tubs* xzPlane = new G4Tubs("XZPlane", 0, 10*cm, 0.01*cm, 0., 360.*deg);
            G4LogicalVolume* xzLV = new G4LogicalVolume(xzPlane, Galactic, "XZPlaneLV");
            new G4PVPlacement(rotX90, G4ThreeVector(0, 0, 0), xzLV, "XZPlanePV", fParser->GetVolume("worldVOL"), false, 0, false);
        
        };

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
