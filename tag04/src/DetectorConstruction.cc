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

#include "G4GDMLParser.hh"


DetectorConstruction::DetectorConstruction() {
	
	fMessenger = new G4GenericMessenger(this, "/target/", "target control");
	auto& targetWidthCmd = fMessenger->DeclareMethod(
		"setWidth", &DetectorConstruction::ScaleTargetWidth,"Set target width.");
	
	G4GDMLParser* fParser;
	DefineParameters();
	DefineMaterials();
}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::DefineParameters() {

	fWorldSize = 1*m;
	distDetGe = 45.72*cm;
	distDetSi = 38*cm;
	TarLength = 10*cm;
	TarWidth = 106*nm;
	dDiameterGe = 10*cm;
	dDiameterSi = 8*cm;
	zLengthGe = 3*cm;
	zLengthSi = 3*cm;

	logicGeDet = nullptr;
	logicSiDet = nullptr;
	logicSiSph = nullptr;
}

void DetectorConstruction::DefineMaterials() {
	
	man = G4NistManager::Instance();
	Galactic = man->FindOrBuildMaterial("G4_Galactic");
	Ge = man->FindOrBuildMaterial("G4_Ge");
	Si = man->FindOrBuildMaterial("G4_Si");
	BC = man->FindOrBuildMaterial("G4_BORON_CARBIDE"); 
	Al = man->FindOrBuildMaterial("G4_Al");
	B10Iso = new G4Isotope("B10", 5, 10);
	elB10 = new G4Element("MatB10", "B", 1);
	elB10->AddIsotope(B10Iso, 100*perCent);
	B10 = new G4Material("B10Mat", 2.34*g/cm3, 1);
	B10->AddElement(elB10, 100*perCent);
}

void DetectorConstruction::ConstructTarget() {
	
	solidTarget = new G4Box("solidTarget", TarLength / 2, TarLength / 2, TarWidth / 2);
	logicTarget = new G4LogicalVolume(solidTarget, B10, "logicTarget");
	physTarget = new G4PVPlacement(
		0, G4ThreeVector(0, 0, 0), logicTarget, "physTarget", logicWorld, false, 0, false);
}

void DetectorConstruction::ConstructTarget(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, int& cpyNo) {

	G4Transform3D transformTarget = rotPhi * rotTheta;
	solidTargetCyl = new G4Tubs("solidTargetCyl", 0, TarLength / 2, TarWidth / 2, 0*deg, 360*deg);
	logicTargetCyl = new G4LogicalVolume(solidTargetCyl, B10, "logicTargetCyl");
	physTargetCyl = new G4PVPlacement(
		transformTarget, logicTargetCyl, "physTargetCyl", logicWorld, false, 100, false);
	cpyNo++;
}

void DetectorConstruction::ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, int& cpyNo) {

	G4Translate3D shiftZSiDet(0, 0, distDetSi + zLengthSi / 2);
	G4Transform3D transformSiDet = rotPhi * rotTheta * shiftZSiDet;
	solidSiDet = new G4Tubs("solidSiDet", 0, dDiameterSi / 2, zLengthSi / 2, 0*deg, 360*deg);
	logicSiDet = new G4LogicalVolume(solidSiDet, Si, "logicSiDet");
	physSiDet = new G4PVPlacement(
		transformSiDet, logicSiDet, "physSiDet", logicWorld, false, cpyNo, false);
	fSiScoringVolume = logicSiDet;
	// logicSiDet->GetSolid()->DumpInfo();
	cpyNo++;
}

void DetectorConstruction::ConstructSiSphDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {

	G4Transform3D transformSiSph = rotPhi * rotTheta;
	solidSiSph = new G4Sphere("solidSiSph", 2*distDetSi, 2*distDetSi + zLengthSi / 2, 0*deg, 90*deg, 0*deg, 90*deg);
	// solidSiSph = new G4Sphere("solidSiSph", 2*distDet, 2*distDet + zLengthSi / 2, 0*deg, 360*deg, 0*deg, 180*deg);
	// solidSiSph = new G4Sphere(
	// 	"solidSiSph", 2*distDet, 2*distDet + zLengthSi / 2, 0*deg, 360*deg, 0*deg, atan(dDiameterSi/(2*distDet))*rad);
	logicSiSph = new G4LogicalVolume(solidSiSph, Si, "logicSiSph");
	fSiScoringVolume = logicSiSph;
	physSiSph = new G4PVPlacement(
		transformSiSph, logicSiSph, "physSiSph", logicWorld, false, 7, false);
}

void DetectorConstruction::ConstructCollimator(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {

	G4int zLengthCollimator = 1*cm;
	G4Translate3D shiftZCollimator(0, 0, distDetSi - zLengthSi / 2 - zLengthCollimator / 2 - .5*cm);
	G4Transform3D transformCollimator = rotPhi * rotTheta * shiftZCollimator;
	solidCollimator = new G4Tubs("solidCollimator", 1*cm, dDiameterSi / 2, zLengthCollimator / 2, 0*deg, 360*deg);
	logicCollimator = new G4LogicalVolume(solidCollimator, Al, "logicCollimator");
	physCollimator = new G4PVPlacement(
		transformCollimator, logicCollimator, "physCollimator", logicWorld, false, 6, false);
}

void DetectorConstruction::ConstructHPGeDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, int& cpyNo) {
	
	G4Translate3D shiftZGeDet(0, 0, distDetGe + zLengthGe / 2);
	G4Transform3D transformGeDet1 = rotPhi * rotTheta * shiftZGeDet;
	solidGeDet = new G4Tubs("solidGeDet", 0, dDiameterGe / 2, zLengthGe / 2, 0*deg, 360*deg);
	logicGeDet = new G4LogicalVolume(solidGeDet, Ge, "logicGeDet");
	fGeScoringVolume = logicGeDet;
	physGeDet = new G4PVPlacement(transformGeDet1, logicGeDet, "physGeDet", logicWorld, false, cpyNo, false);
	cpyNo++;
}

void DetectorConstruction::ConstructHPGeDetectorXYZ(G4RotateX3D rotX, G4RotateY3D rotY, G4RotateZ3D rotZ, int& cpyNo) {
	
	G4Translate3D shiftZGeDet(0, 0, distDetGe + zLengthGe / 2);
	G4Transform3D transformGeDet1 = rotX * rotY * rotZ * shiftZGeDet;
	solidGeDet = new G4Tubs("solidGeDet", 0, dDiameterGe / 2, zLengthGe / 2, 0*deg, 360*deg);
	logicGeDet = new G4LogicalVolume(solidGeDet, Ge, "logicGeDet");
	fGeScoringVolume = logicGeDet;
	physGeDet = new G4PVPlacement(transformGeDet1, logicGeDet, "physGeDet", logicWorld, false, cpyNo, false);
	cpyNo++;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {

	// solidWorld = new G4Box("World", fWorldSize / 2, fWorldSize / 2, fWorldSize / 2);
	// logicWorld = new G4LogicalVolume(solidWorld, Galactic, "World");
	// physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 5);

	std::cout << "Loading GDML file..." << std::endl;

	G4GDMLParser* fParser = new G4GDMLParser();
	// fParser->Read("/home/anthony/software/AGDet_Real/tag04/thevoicesaregettinglouder2.gdml");
	// fParser->Read("/home/anthony/software/AGDet_Real/tag04/SpacedOut.gdml");
	fParser->Read("/home/anthony/software/AGDet_Real/tag04/TheRebirthGDMLMain.gdml");
	physWorld = fParser->GetWorldVolume();  
	logicWorld = physWorld->GetLogicalVolume();
	
	std::cout << "gurt tingle " << physWorld << std::endl;

	int cpyNo = 10;

	G4RotateX3D rotX1(-40*deg);
	G4RotateY3D rotY1(0*deg);
	G4RotateZ3D rotZ1(0*deg);
	ConstructHPGeDetectorXYZ(rotX1, rotY1, rotZ1, cpyNo);

	G4RotateX3D rotX2(-220*deg);
	G4RotateY3D rotY2(0*deg);
	G4RotateZ3D rotZ2(0*deg);
	ConstructHPGeDetectorXYZ(rotX2, rotY2, rotZ2, cpyNo);

	G4RotateY3D rotThetaHPGe1(40*deg);
	G4RotateZ3D rotPhiHPGe1(-30*deg);
	ConstructHPGeDetector(rotThetaHPGe1, rotPhiHPGe1, cpyNo);

	G4RotateY3D rotThetaHPGe2(40*deg);
	G4RotateZ3D rotPhiHPGe2(-150*deg);
	ConstructHPGeDetector(rotThetaHPGe2, rotPhiHPGe2, cpyNo);

	G4RotateY3D rotThetaHPGe3(140*deg);
	G4RotateZ3D rotPhiHPGe3(30*deg);
	ConstructHPGeDetector(rotThetaHPGe3, rotPhiHPGe3, cpyNo);

	G4RotateY3D rotThetaHPGe4(140*deg);
	G4RotateZ3D rotPhiHPGe4(150*deg);
	ConstructHPGeDetector(rotThetaHPGe4, rotPhiHPGe4, cpyNo);

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

	delete fParser;
	return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
	
	MySiDet* sensDetSi = new MySiDet("SiSensDet");
	MyGeDet* sensDetGe = new MyGeDet("GeSensDet");
	if (logicGeDet) {
		G4cout << "dawn in the adan" << G4endl;
		logicGeDet->SetSensitiveDetector(sensDetGe);
	}
	if (logicSiSph) {
		G4cout << "kitsune maison" << G4endl;
		logicSiSph->SetSensitiveDetector(sensDetSi);
	}
	if (logicSiDet) {
		G4cout << "viva la vida" << G4endl;
		logicSiDet->SetSensitiveDetector(sensDetSi);
	}
}

