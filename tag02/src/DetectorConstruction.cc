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

DetectorConstruction::DetectorConstruction() {
	
	fWorldSize = 20*cm;
	zLengthGe = 3*cm;
	zLengthSi = 100*um;
	dDiameterGe = 8*mm;
	dDiameterSi = 10*mm;
	distDet = 1*cm;

	DefineMaterials();
}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::DefineMaterials() {
	
	G4NistManager* man = G4NistManager::Instance();

	Galactic = new G4Material("Galactic", universe_mean_density, 1);
	Galactic->AddElement(man->FindOrBuildElement("H"), 1);

	Ge = man->FindOrBuildMaterial("G4_Ge");
	Si = man->FindOrBuildMaterial("G4_Si");
	BC = man->FindOrBuildMaterial("G4_BORON_CARBIDE");
	// air = man->FindOrBuildMaterial("G4_AIR");
	// Al = man->FindOrBuildMaterial("G4_Al");
	// Au = man->FindOrBuildMaterial("G4_Au");
	// mylar = man->FindOrBuildMaterial("G4_MYLAR");
	// B = man->FindOrBuildMaterial("G4_B");

}

void DetectorConstruction::ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {

	G4Translate3D shiftZSiDet(0, 0, distDet + zLengthSi / 2);
	G4Transform3D transformSiDet = rotPhi * rotTheta * shiftZSiDet;
	solidSiDet = new G4Tubs("solidSiDet", 0, dDiameterSi / 2, zLengthSi / 2, 0*deg, 360*deg);
	logicSiDet = new G4LogicalVolume(solidSiDet, Si, "logicSiDet");
	physSiDet = new G4PVPlacement(transformSiDet, logicSiDet, "physSiDet", logicWorld, false, 0, false);
	fSiScoringVolume = logicSiDet;
}

void DetectorConstruction::ConstructHPGeDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {
	
	G4Translate3D shiftZGeDet(0, 0, distDet + zLengthGe / 2);
	G4Transform3D transformGeDet = rotPhi * rotTheta * shiftZGeDet;
	solidGeDet = new G4Tubs("solidGeDet", 0, dDiameterGe / 2, zLengthGe / 2, 0*deg, 360*deg);
	logicGeDet = new G4LogicalVolume(solidGeDet, Ge, "logicGeDet");
	physGeDet = new G4PVPlacement(transformGeDet, logicGeDet, "physGeDet", logicWorld, false, 0, false);
	fGeScoringVolume = logicGeDet;
}

void DetectorConstruction::ConstructPIPSDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {

	G4Translate3D shiftZCasing(0, 0, 0);
	G4Transform3D transformCasing = rotPhi * rotTheta * shiftZCasing;
	//Casing
	G4Tubs* pipe = new G4Tubs("pipe", 5.642*mm , 7.4*mm, 3.8*mm, 0, 360*deg);
	G4Box* box = new G4Box("box", 2*mm , 3*mm, 3*mm);
	auto pipe1 = new G4UnionSolid("Pipe1", pipe, box, 0, G4ThreeVector(0., 8.9*mm, 0));
	auto pipeLV = new G4LogicalVolume(pipe1, Au, "Pipe1");
	auto pipePV = new G4PVPlacement(transformCasing, pipeLV, "Pipe1", logicWorld, false, 0, false);
	// Source Covering-----------MYLAR
	G4Translate3D shiftZCover(0.0*mm, 0.0*mm, 2.380326*cm);
	G4Transform3D transformCover = rotPhi * rotTheta * shiftZCover;
	auto sourceCoverS = new G4Tubs("mylar", 0*mm, 12*mm, 0.00181*mm, 0*deg, 360*deg);
	auto sourceCoverLV = new G4LogicalVolume(sourceCoverS, mylar, "mylar");
	auto sourceCoverPV = new G4PVPlacement(transformCover, sourceCoverLV, "mylar", logicWorld, false, 0, false);
	//----------Boron Layer-----------------
	G4Translate3D shiftZB(0.0*mm, 0.0*mm, 2.800130*mm);
	G4Transform3D transformB = rotPhi * rotTheta * shiftZB;
	auto scintillatorS = new G4Tubs("boron", 0*mm, 5.642*mm, 0.000050*mm, 0*deg, 360*deg); 
	auto scintillatorLV = new G4LogicalVolume(scintillatorS, B, "Boron");
	auto fScintillatorPV = new G4PVPlacement(transformB, scintillatorLV, "Boron", logicWorld, false, 0, false);
	//--------------------------------------
	G4Translate3D shiftZSi(0.0*mm, 0.0*mm, 2.05*mm);
	G4Transform3D transformSi = rotPhi * rotTheta * shiftZSi;
	auto siliconS = new G4Tubs("Silicon", 0.0*mm, 5.642*mm, 1*mm, 0*deg, 360*deg);   
	auto siliconLV = new G4LogicalVolume(siliconS, Si, "silicon");
	auto fSiliconPV = new G4PVPlacement(transformSi, siliconLV, "silicon", logicWorld, false, 0, false);
}

G4VPhysicalVolume* DetectorConstruction::Construct() {

	solidWorld = new G4Box("World", fWorldSize / 2, fWorldSize / 2, fWorldSize / 2);
	logicWorld = new G4LogicalVolume(solidWorld, Galactic, "World");
	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);

	G4RotateY3D rotThetaSi(0*deg);
	G4RotateZ3D rotPhiSi(0*deg);
	ConstructSiDetector(rotThetaSi, rotPhiSi);

	return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
	
	MySiDet* sensDetSi = new MySiDet("SiSensitiveDetector");
	MyGeDet* sensDetGe = new MyGeDet("GeSensitiveDetector");
	if (logicSiDet) {
		G4cout << "HJGNKMBGDDJGNVB" << G4endl;
		logicSiDet->SetSensitiveDetector(sensDetSi);
	}
	if (logicGeDet) {
		G4cout << "hello vro" << G4endl;
		logicGeDet->SetSensitiveDetector(sensDetGe);
	}
}


