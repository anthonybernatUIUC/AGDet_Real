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
	fWorldSize = 20*mm;
	zLengthSi = 300*um;
	distDet = 7*mm;
	dDiameterSi = 7.5*mm;
	DefineMaterials();
}

void DetectorConstruction::DefineMaterials() {
	
	man = G4NistManager::Instance();
	Galactic = man->FindOrBuildMaterial("G4_Galactic");
	Air = man->FindOrBuildMaterial("G4_AIR");
	Ge = man->FindOrBuildMaterial("G4_Ge");
	Si = man->FindOrBuildMaterial("G4_Si");
	BC = man->FindOrBuildMaterial("G4_BORON_CARBIDE");
	Al = man->FindOrBuildMaterial("G4_Al");
}

void DetectorConstruction::ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {

	G4Translate3D shiftZSiDet(0, 0, distDet + zLengthSi / 2);
	G4Transform3D transformSiDet = rotPhi * rotTheta * shiftZSiDet;
	solidSiDet = new G4Tubs("solidSiDet", 0, dDiameterSi / 2, zLengthSi / 2, 0*deg, 360*deg);
	logicSiDet = new G4LogicalVolume(solidSiDet, Si, "logicSiDet");
	physSiDet = new G4PVPlacement(
		transformSiDet, logicSiDet, "physSiDet", logicWorld, false, 1, false);
	fSiScoringVolume = logicSiDet;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  
	solidWorld = new G4Box("solidWorld", fWorldSize / 2, fWorldSize / 2, fWorldSize / 2); 
	logicWorld = new G4LogicalVolume(solidWorld, Galactic, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0);

	G4RotateY3D rotThetaSi(0*deg);
	G4RotateZ3D rotPhiSi(0*deg);
	ConstructSiDetector(rotThetaSi, rotPhiSi);

	return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
	
	MySiDet* sensDetSi = new MySiDet("SiSensDet");
	if (logicSiDet) {
		G4cout << "viva la vida" << G4endl;
		logicSiDet->SetSensitiveDetector(sensDetSi);
	}
}


