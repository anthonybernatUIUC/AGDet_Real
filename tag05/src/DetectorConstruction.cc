//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "detector.hh"


DetectorConstruction::DetectorConstruction() {
	
	fMessenger = new G4GenericMessenger(this, "/target/", "target control");
	fMessengerShell = new G4GenericMessenger(this, "/shell/", "Isotope shell control");
	fMessenger->DeclareMethod("setWidth", &DetectorConstruction::ScaleTargetWidth,"Set target width");
	fMessengerShell->DeclareMethod("setShellType", &DetectorConstruction::SetShellType, "Set the type of shell");

	DefineParameters();
	DefineMaterials();
}

DetectorConstruction::~DetectorConstruction() {
	delete fMessenger;
	delete fMessengerShell;
	delete logicIsoSphere;
}

void DetectorConstruction::DefineParameters() {

	fWorldSize = 1.1*m;
	dTarget = 10*cm;
	zTarget = 106*nm;
	dGe = 8*cm;
	dSi = 2.8*cm;
	zGe = 3.14*cm;
	zSi = 3*mm;
	distDetGe = 22*cm;
	distDetSi = 21*cm;
	zPbBackShield = 10.6*cm;
	shellType = 0;
	// Ge Casing z = 15 cm, d = 10.2 cm irl

}

void DetectorConstruction::DefineMaterials() {
	
	man = G4NistManager::Instance();
	Galactic = man->FindOrBuildMaterial("G4_Galactic");
	BC = man->FindOrBuildMaterial("G4_BORON_CARBIDE"); 
	Ge = man->FindOrBuildMaterial("G4_Ge");
	Si = man->FindOrBuildMaterial("G4_Si");
	Al = man->FindOrBuildMaterial("G4_Al");
	Cu = man->FindOrBuildMaterial("G4_Cu");
  	Pb = man->FindOrBuildMaterial("G4_Pb");
	C = man->FindOrBuildMaterial("G4_C");

	B10Iso = new G4Isotope("B10", 5, 10);
	elB10 = new G4Element("MatB10", "B", 1);
	elB10->AddIsotope(B10Iso, 100*perCent);
	B10 = new G4Material("B10Mat", 2.34*g/cm3, 1);
	B10->AddElement(elB10, 100*perCent);

  	matSteel = new G4Material("Stainless Steel", 8.02*g/cm3, 5);
	elMn = new G4Element("Manganese", "Mn", 25, 54.94*g/mole);
	elCr = new G4Element("Chromium", "Cr", 24, 51.9961*g/mole);
	elSi = new G4Element("Silicon", "Si", 14, 28.09*g/mole);
	elNi = new G4Element("Nickel", "Ni", 28, 58.6934*g/mole);
	elFe = new G4Element("Iron", "Fe", 26, 55.845*g/mole);
  	matSteel->AddElement(elMn, 0.02);
	matSteel->AddElement(elCr, 0.19);
  	matSteel->AddElement(elSi, 0.01);
  	matSteel->AddElement(elNi, 0.10);
  	matSteel->AddElement(elFe, 0.68);

	Cl35 = new G4Isotope("Cl35", 17, 35);
	Cr50 = new G4Isotope("Cr50", 24, 50);
	Cr52 = new G4Isotope("Cr52", 24, 52);
	Cr53 = new G4Isotope("Cr53", 24, 53);
	Fe56 = new G4Isotope("Fe56", 26, 56);
	Ni58 = new G4Isotope("Ni58", 28, 58);
	Ni60 = new G4Isotope("Ni60", 28, 60);
	Ni62 = new G4Isotope("Ni62", 28, 62);
	Ni63 = new G4Isotope("Ni63", 28, 63);
	Ni64 = new G4Isotope("Ni64", 28, 64);
	Na24 = new G4Isotope("Na24", 11, 24);
	Mn56 = new G4Isotope("Mn56", 25, 56);

	elCl = new G4Element("ElCl", "Cl", 1);
	elCl->AddIsotope(Cl35, 1);

	elCr = new G4Element("ElCr", "Cr", 3);
	elCr->AddIsotope(Cr50, .3333);
	elCr->AddIsotope(Cr52, .3333);
	elCr->AddIsotope(Cr53, .3334);

	elFe = new G4Element("ElFe", "Fe", 1);
	elFe->AddIsotope(Fe56, 1);

	elNi = new G4Element("ElNi", "Ni", 5);
	elNi->AddIsotope(Ni58, .2);
	elNi->AddIsotope(Ni60, .2);
	elNi->AddIsotope(Ni62, .2);
	elNi->AddIsotope(Ni63, .2);
	elNi->AddIsotope(Ni64, .2);

	elNa = new G4Element("ElNa", "Na", 1);
	elNa->AddIsotope(Na24, 1);

	elMn = new G4Element("ElMn", "Mn", 1);
	elMn->AddIsotope(Mn56, 1);

	H1 = new G4Isotope("H1", 1, 1);
   	elH = new G4Element("isopure hydrogen", "H", 1);
  	elH->AddIsotope(H1, 1);

	IsotopeShellMatH = new G4Material("IsotopeShellMatH", 0.07085 * g/cm3, 1);
	IsotopeShellMatH->AddElement(elH, 1);

	// Pulled from Ian's code. Not necessarily accurate, since it uses isotopes pertinent to UCNtau
	IsotopeShellMat = new G4Material("IsotopeShellMat", 7.85 * g/cm3, 6); 
	IsotopeShellMat->AddElement(elCl, .0834);
	IsotopeShellMat->AddElement(elCr, .2499);
	IsotopeShellMat->AddElement(elFe, .0834);
	IsotopeShellMat->AddElement(elNi, .4165);
	IsotopeShellMat->AddElement(elNa, .0834);
	IsotopeShellMat->AddElement(elMn, .0834);

	// IsotopeShellMat = new G4Material("IsotopeShellMat", 7.85 * g/cm3, 1); 
	// IsotopeShellMat->AddElement(elMn, 1);

	// K40 = new G4Isotope("K40", 19, 40);
	// elK = new G4Element("Potassium", "K", 1);
	// elK->AddIsotope(K40, 1);

	// Co60 = new G4Isotope("Co60", 27, 60);
	// G4Element* elCo = new G4Element("Cobalt", "Co", 1);
	// elCo->AddIsotope(Co60, 1);

	// IsotopeShellMat = new G4Material("IsotopeShellMat", 8.9 * g/cm3, 1);
	// IsotopeShellMat->AddElement(elCo, 1); 
}

void DetectorConstruction::ConstructTarget() {
	
	solidTarget = new G4Box("solidTarget", dTarget / 2, dTarget / 2, zTarget / 2);
	logicTarget = new G4LogicalVolume(solidTarget, B10, "logicTarget");
	physTarget = new G4PVPlacement(
		0, G4ThreeVector(0, 0, 0), logicTarget, "physTarget", logicWorld, false, 0, false);
}

void DetectorConstruction::ConstructTarget(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, int& cpyNo) {

	G4Transform3D transformTarget = rotPhi * rotTheta;
	solidTargetCyl = new G4Tubs("solidTargetCyl" + std::to_string(cpyNo), 0, dTarget / 2, zTarget / 2, 0*deg, 360*deg);
	logicTargetCyl = new G4LogicalVolume(solidTargetCyl, B10, "logicTargetCyl" + std::to_string(cpyNo));
	physTargetCyl = new G4PVPlacement(
		transformTarget, logicTargetCyl, "physTargetCyl" + std::to_string(cpyNo), logicWorld, false, 100, false);
	logicTargetCyl->SetVisAttributes(G4VisAttributes(G4Color(0.7, 0.4, 0.2, 1)));
	cpyNo++;
}

void DetectorConstruction::ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, int& cpyNo) {

	G4Translate3D shiftZSiDet(0, 0, distDetSi + zSi / 2);
	G4Transform3D transformSiDet = rotPhi * rotTheta * shiftZSiDet;
	solidSiDet = new G4Tubs("solidSiDet" + std::to_string(cpyNo), 0, dSi / 2, zSi / 2, 0*deg, 360*deg);
	logicSiDet = new G4LogicalVolume(solidSiDet, Si, "logicSiDet" + std::to_string(cpyNo));
	physSiDet = new G4PVPlacement(
		transformSiDet, logicSiDet, "physSiDet" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	SiDets.insert(logicSiDet);
	logicSiDet->SetVisAttributes(G4VisAttributes(G4Color::Cyan()));
	cpyNo++;
}

void DetectorConstruction::ConstructHPGeDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, int& cpyNo) {
	
	G4Translate3D shiftZGeDet(0, 0, distDetGe + zGe / 2);
	G4Transform3D transformGeDet = rotPhi * rotTheta * shiftZGeDet;
	solidGeDet = new G4Tubs("solidGeDet" + std::to_string(cpyNo), 0, dGe / 2, zGe / 2, 0*deg, 360*deg);
	logicGeDet = new G4LogicalVolume(solidGeDet, Ge, "logicGeDet" + std::to_string(cpyNo));
	physGeDet = new G4PVPlacement(
		transformGeDet, logicGeDet, "physGeDet" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	GeDets.insert(logicGeDet);
	logicGeDet->SetVisAttributes(G4VisAttributes(G4Color::Blue()));
	cpyNo++;

	// solidGeMount = new G4Tubs(
	// 	"solidGeMount" + std::to_string(cpyNo), (dGe + .5*cm) / 2, (dGe + 1*cm) / 2, zGe / 2, 0*deg, 360*deg);
	// logicGeMount = new G4LogicalVolume(solidGeMount, matSteel, "logicGeMount" + std::to_string(cpyNo));
	// physGeMount = new G4PVPlacement(
	// 	transformGeDet, logicGeMount, "physGeMount" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	// cpyNo++;
}

void DetectorConstruction::ConstructHPGeDetectorXYZ(G4RotateX3D rotX, G4RotateY3D rotY, G4RotateZ3D rotZ, int& cpyNo) {
	
	G4Translate3D shiftZGeDet(0, 0, distDetGe + zGe / 2);
	G4Transform3D transformGeDet = rotX * rotY * rotZ * shiftZGeDet;
	solidGeDet = new G4Tubs("solidGeDet" + std::to_string(cpyNo), 0, dGe / 2, zGe / 2, 0*deg, 360*deg);
	logicGeDet = new G4LogicalVolume(solidGeDet, Ge, "logicGeDet" + std::to_string(cpyNo));
	physGeDet = new G4PVPlacement(
		transformGeDet, logicGeDet, "physGeDet" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	GeDets.insert(logicGeDet);
	logicGeDet->SetVisAttributes(G4VisAttributes(G4Color::Blue()));
	cpyNo++;

	solidGeMount = new G4Tubs(
		"solidGeMount" + std::to_string(cpyNo), dGe / 2, (dGe + 1*cm) / 2, zGe / 2, 0*deg, 360*deg);
	logicGeMount = new G4LogicalVolume(solidGeMount, matSteel, "logicGeMount" + std::to_string(cpyNo));
	physGeMount = new G4PVPlacement(
		transformGeDet, logicGeMount, "physGeMount" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	logicGeMount->SetVisAttributes(G4VisAttributes(G4Color::White()));
	cpyNo++;
}

void DetectorConstruction::ConstructPbBackShield(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, int& cpyNo) {

	G4Translate3D shiftZPbBackShield(0, 0, distDetGe + zPbBackShield / 2 + 21.1*cm / 2 + 1*cm);
	G4Transform3D transformPbBackShield = rotPhi * rotTheta * shiftZPbBackShield;
	solidPbBackShield = new G4Tubs(
		"solidPbBackShield" + std::to_string(cpyNo), 2.1*cm, 5.3*cm, zPbBackShield / 2, 0*deg, 360*deg);
	logicPbBackShield = new G4LogicalVolume(solidPbBackShield, Pb, "logicPbBackShield" + std::to_string(cpyNo));
	physPbBackShield = new G4PVPlacement(
		transformPbBackShield, logicPbBackShield, "physPbBackShield" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	logicPbBackShield->SetVisAttributes(G4VisAttributes(G4Colour(.36, .38, .45, 1.)));
	cpyNo++;
}

void DetectorConstruction::ConstructIsotopeShell(int& cpyNo) {
    	
	solidIsoSphere = new G4Sphere(
		"solidIsoSphere" + std::to_string(cpyNo), 49*cm, 50*cm, 0*deg, 360*deg, 7.5*deg, 180*deg);
	if (shellType == 0) {
		logicIsoSphere = new G4LogicalVolume(solidIsoSphere, IsotopeShellMatH, "logicIsoSphere" + std::to_string(cpyNo));
		logicIsoSphere->SetVisAttributes(G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.1)));
	} else if (shellType == 1) {
		logicIsoSphere = new G4LogicalVolume(solidIsoSphere, IsotopeShellMat, "logicIsoSphere" + std::to_string(cpyNo));
		logicIsoSphere->SetVisAttributes(G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1)));
	} else {
		G4Exception("DetectorConstruction::ConstructIsotopeShell", "InvalidShellType", FatalException,
			"Invalid shell type specified. Use 0 for hydrogen or 1 for whatever.");
	}
	physIsoSphere = new G4PVPlacement(
		0, G4ThreeVector(0, 0, 0), logicIsoSphere, "physIsoSphere" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	cpyNo++;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {

	G4cout << "Constructing Geometry..." << G4endl;
	int cpyNo = 10;

	G4LogicalVolumeStore* const store = G4LogicalVolumeStore::GetInstance();
	// clear store to avoid dupes and annoying warnings since reintializing geometry likely calls Construct() again
	if (store->size() > 0) store->clear();

	G4GDMLParser* fParser = new G4GDMLParser();
	// fParser->Read("../thevoicesaregettinglouder2.gdml");
	fParser->Read("../TheRebirthGDMLMain.gdml", true);
	G4cout << "-------------------------------" << G4endl;
	physWorld = fParser->GetWorldVolume();
	logicWorld = physWorld->GetLogicalVolume();

	G4LogicalVolume* AGDevice = G4LogicalVolumeStore::GetInstance()->GetVolume("AGDevice");
	AGDevice->SetMaterial(matSteel);
	// AGDevice->SetVisAttributes(G4VisAttributes(G4Color(1, 1, 1, .5)));
	
	G4LogicalVolume* AlCap = G4LogicalVolumeStore::GetInstance()->GetVolume("AlCap");
	AlCap->SetMaterial(Al);
	AlCap->SetVisAttributes(G4VisAttributes(G4Color::Gray()));

	G4LogicalVolume* CuWrap = G4LogicalVolumeStore::GetInstance()->GetVolume("CuWrap");
	CuWrap->SetMaterial(Cu);
	CuWrap->SetVisAttributes(G4VisAttributes(G4Color::Brown()));

	G4LogicalVolume* SiDetAperture = G4LogicalVolumeStore::GetInstance()->GetVolume("SiDetAperture");
	SiDetAperture->SetMaterial(matSteel);
	SiDetAperture->SetVisAttributes(G4VisAttributes(G4Color::Green()));

	G4LogicalVolume* SiCollimator = G4LogicalVolumeStore::GetInstance()->GetVolume("SiCollimator");
	SiCollimator->SetMaterial(matSteel);
	SiCollimator->SetVisAttributes(G4VisAttributes(G4Color::Green()));

	G4LogicalVolume* GeMount = G4LogicalVolumeStore::GetInstance()->GetVolume("HPGeMount");
	GeMount->SetMaterial(matSteel);

	G4LogicalVolume* CarbonWindow = G4LogicalVolumeStore::GetInstance()->GetVolume("CarbonWindow");
	CarbonWindow->SetMaterial(C);
	CarbonWindow->SetVisAttributes(G4VisAttributes(G4Color(.663, .663, .663, .2)));

	// ---Front Ge Det locations--- // 
	G4RotateY3D rotThetaHPGe1(40*deg);
	G4RotateZ3D rotPhiHPGe1(90*deg);
	ConstructHPGeDetector(rotThetaHPGe1, rotPhiHPGe1, cpyNo);
	ConstructPbBackShield(rotThetaHPGe1, rotPhiHPGe1, cpyNo);

	G4RotateY3D rotThetaHPGe2(40*deg);
	G4RotateZ3D rotPhiHPGe2(-30*deg);
	ConstructHPGeDetector(rotThetaHPGe2, rotPhiHPGe2, cpyNo);
	ConstructPbBackShield(rotThetaHPGe2, rotPhiHPGe2, cpyNo);

	G4RotateY3D rotThetaHPGe3(40*deg);
	G4RotateZ3D rotPhiHPGe3(-150*deg);
	ConstructHPGeDetector(rotThetaHPGe3, rotPhiHPGe3, cpyNo);
	ConstructPbBackShield(rotThetaHPGe3, rotPhiHPGe3, cpyNo);

	// ---Rear Ge Det locations--- //
	G4RotateY3D rotThetaHPGe4(220*deg);
	G4RotateZ3D rotPhiHPGe4(90*deg);
	// ConstructHPGeDetector(rotThetaHPGe4, rotPhiHPGe4, cpyNo);
	ConstructPbBackShield(rotThetaHPGe4, rotPhiHPGe4, cpyNo);

	G4RotateY3D rotThetaHPGe5(140*deg);
	G4RotateZ3D rotPhiHPGe5(30*deg);
	// ConstructHPGeDetector(rotThetaHPGe3, rotPhiHPGe3, cpyNo);
	ConstructPbBackShield(rotThetaHPGe5, rotPhiHPGe5, cpyNo);

	G4RotateY3D rotThetaHPGe6(140*deg);
	G4RotateZ3D rotPhiHPGe6(150*deg);
	// ConstructHPGeDetector(rotThetaHPGe4, rotPhiHPGe4, cpyNo);
	ConstructPbBackShield(rotThetaHPGe6, rotPhiHPGe6, cpyNo);

	// ---Si Dets--- //
	G4RotateY3D rotThetaSi1(50*deg);
	G4RotateZ3D rotPhiSi1(-90*deg);
	ConstructSiDetector(rotThetaSi1, rotPhiSi1, cpyNo);

	G4RotateY3D rotThetaSi2(50*deg);
	G4RotateZ3D rotPhiSi2(30*deg);
	ConstructSiDetector(rotThetaSi2, rotPhiSi2, cpyNo);

	G4RotateY3D rotThetaSi3(50*deg);
	G4RotateZ3D rotPhiSi3(150*deg);
	ConstructSiDetector(rotThetaSi3, rotPhiSi3, cpyNo);

	G4RotateY3D rotThetaSi4(67.24*deg);
	G4RotateZ3D rotPhiSi4(90*deg);
	ConstructSiDetector(rotThetaSi4, rotPhiSi4, cpyNo);

	G4RotateY3D rotThetaSi5(67.24*deg);
	G4RotateZ3D rotPhiSi5(210*deg);
	ConstructSiDetector(rotThetaSi5, rotPhiSi5, cpyNo);

	G4RotateY3D rotThetaSi6(67.24*deg);
	G4RotateZ3D rotPhiSi6(-30*deg);
	ConstructSiDetector(rotThetaSi6, rotPhiSi6, cpyNo);

	G4RotateY3D rotThetaTar(0*deg);
	G4RotateZ3D rotPhiTar(0*deg);
	ConstructTarget(rotThetaTar, rotPhiTar, cpyNo);

	ConstructIsotopeShell(cpyNo);

	// for (auto i = store->cbegin(); i != store->cend(); ++i) {
	// 	G4cout << "Logical Volume: " << (*i)->GetName() << G4endl;
	// }

	delete fParser;
	return physWorld; 
}

void DetectorConstruction::ConstructSDandField() {
	
	MySiDet* sensDetSi = new MySiDet("SiSensDet");
	MyGeDet* sensDetGe = new MyGeDet("GeSensDet");
	if (!GeDets.empty()) {
		G4cout << "dawn in the adan" << G4endl;
		for (auto& logicGeDet : GeDets) {
			logicGeDet->SetSensitiveDetector(sensDetGe);
		}
	}
	if (!SiDets.empty()) {
		G4cout << "viva la vida" << G4endl;
		for (auto& logicSiDet : SiDets) {
			logicSiDet->SetSensitiveDetector(sensDetSi);
		}
	}
}

