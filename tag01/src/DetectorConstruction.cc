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
	fWorldSize = 1*m;

	// Initialize custom messenger to adjust parameters
	fMessenger = new G4GenericMessenger(this, "/detcon/", "Detector Construction");

	fMessenger->DeclareProperty("isShielded", isShielded, "Load in Lead & Copper Shielding");
	fMessenger->DeclareProperty("rThickness", rThickness, "If isShielded is true, set Thickness of Lead Shield. Unit = cm.");
	fMessenger->DeclareProperty("dDiameter", dDiameter, "Set Diameter of HPGe. Unit = cm.");
	fMessenger->DeclareProperty("zLength", zLength, "Set Length of HPGe. Unit = cm.");
	fMessenger->DeclareProperty("outerShieldDiameter", outerShieldDiameter, "Set Outer Diameter of the Outer Shield (S3). Unit = in.");
	fMessenger->DeclareProperty("halfShell", halfShell, "Set True to study only half of the isotope shell");
	fMessenger->DeclareProperty("isHydrogen", isHydrogen, "Load Isotope Shell material as hydrogen instead of isotope mixture");

	// Set default values here (these can be changed here for debugging)
	isShielded = true;
	isHydrogen = false;
	halfShell = true;

	rThickness = 5;
	dDiameter = 8;
	zLength = 3;
	outerShieldDiameter = 8; // This will be changed

	DefineMaterials();
}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::DefineMaterials() {
	
	man = G4NistManager::Instance();

	// Vacuum using NIST Manager 
	// Galactic = new G4Material("Galactic", universe_mean_density, 1);
	// Galactic->AddElement(man->("H"), 1);
	Galactic = man->FindOrBuildMaterial("G4_Galactic");

	// Imported Materials
	// Detector
	Ge = man->FindOrBuildMaterial("G4_Ge");
	Si = man->FindOrBuildMaterial("G4_Si");

	// Shielding
	Cu = man->FindOrBuildMaterial("G4_Cu");
	Pb = man->FindOrBuildMaterial("G4_Pb");
	Al = man->FindOrBuildMaterial("G4_Al");

	// Target
	BC = man->FindOrBuildMaterial("G4_BORON_CARBIDE");

	// Isotope Shell - Source of background spectrum
	// Hydrogen for initial test
	H1 = new G4Isotope("H1", 1, 1);
	elH = new G4Element("isopure hydrogen", "H", 1);
	elH->AddIsotope(H1, 100*perCent);

	// Jin's List
	Cl35 = new G4Isotope("Cl35", 17, 35);
	Cr50 = new G4Isotope("Cr50", 24, 50);
	Cr52 = new G4Isotope("Cr50", 24, 52);
	Cr53 = new G4Isotope("Cr50", 24, 53);
	Fe56 = new G4Isotope("Cr50", 26, 56);
	Ni58 = new G4Isotope("Cr50", 28, 58);
	Ni60 = new G4Isotope("Cr50", 28, 60);
	Ni62 = new G4Isotope("Cr50", 28, 62);
	Ni63 = new G4Isotope("Cr50", 28, 63);
	Ni64 = new G4Isotope("Cr50", 28, 64);
	Na24 = new G4Isotope("Cr50", 11, 24);
	Mn56 = new G4Isotope("Cr50", 25, 56);

	elCl = new G4Element("MatCl", "Cl", 1);
	elCl->AddIsotope(Cl35, 100*perCent);

	elCr = new G4Element("MatCr", "Cr", 3);
	elCr->AddIsotope(Cr50, 33.33*perCent);
	elCr->AddIsotope(Cr52, 33.33*perCent);
	elCr->AddIsotope(Cr53, 33.34*perCent);

	elFe = new G4Element("MatFe", "Fe", 1);
	elFe->AddIsotope(Fe56, 100*perCent);

	elNi = new G4Element("MatNi", "Ni", 5);
	elNi->AddIsotope(Ni58, 20*perCent);
	elNi->AddIsotope(Ni60, 20*perCent);
	elNi->AddIsotope(Ni62, 20*perCent);
	elNi->AddIsotope(Ni63, 20*perCent);
	elNi->AddIsotope(Ni64, 20*perCent);

	elNa = new G4Element("MatNa", "Na", 1);
	elNa->AddIsotope(Na24, 100*perCent);

	elMn = new G4Element("MatMn", "Mn", 1);
	elMn->AddIsotope(Mn56, 100*perCent);

	// Construct Isotope Shell Material
	// Hydrogen Testing
	if (isHydrogen) {
		IsotopeShellMat = new G4Material("IsotopeShellMat", 0.07085 * g/cm3, 1);
		IsotopeShellMat->AddElement(elH, 100*perCent);
	} else {  
		IsotopeShellMat = new G4Material("IsotopeShellMat", 7.85 * g/cm3, 6);
		// I gave them all roughly the same abundance & sum to 100 percent
		IsotopeShellMat->AddElement(elCl, 8.34*perCent); 
		IsotopeShellMat->AddElement(elCr, 24.99*perCent);
		IsotopeShellMat->AddElement(elFe, 8.34*perCent);
		IsotopeShellMat->AddElement(elNi, 41.65*perCent);
		IsotopeShellMat->AddElement(elNa, 8.34*perCent);
		IsotopeShellMat->AddElement(elMn, 8.34*perCent);
	}
}

void DetectorConstruction::ConstructAlShield(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {
	// Shell
	G4double distAl1 = zLength / 2;
	Sidel = distAl1 + 1;
	G4Translate3D shiftZAl1(0, 0, (distDet + distAl1)*cm);
	G4Transform3D transformAl1 = rotPhi * rotTheta * shiftZAl1;
	solidAl1 = new G4Tubs(
		"solidAl1", (dDiameter / 2 + 1)*cm, (dDiameter / 2 + 1.1)*cm, Sidel*cm, 0*deg, 360*deg);
	logicAl1 = new G4LogicalVolume(solidAl1, Al, "logicAl1");
	physAl1 = new G4PVPlacement(transformAl1, logicAl1, "physAl1", logicWorld, false, 0, false);

	// Caps
	Capl = .1;
	G4double distAl2_1 = (zLength / 2) + (Sidel + Capl / 2);
	G4double distAl2_2 = (zLength / 2) - (Sidel + Capl / 2);
	G4Translate3D shiftZAl2_1(0, 0, (distDet + distAl2_1)*cm);
	G4Transform3D transformAl2_1 = rotPhi * rotTheta * shiftZAl2_1;
	G4Translate3D shiftZAl2_2(0, 0, (distDet + distAl2_2)*cm);
	G4Transform3D transformAl2_2 = rotPhi * rotTheta * shiftZAl2_2;
	solidAl2 = new G4Tubs("solidAl1", 0, (dDiameter / 2 + 1.1)*cm, Capl*mm / 2, 0*deg, 360*deg);
	logicAl2 = new G4LogicalVolume(solidAl2, Al, "logicAl2");
	physAl2_1 = new G4PVPlacement(transformAl2_1, logicAl2,"physAl2_1", logicWorld, false, 0, false);
	physAl2_2 = new G4PVPlacement(transformAl2_2, logicAl2, "physAl2_2", logicWorld, false, 0, false);
}

void DetectorConstruction::ConstructIsoSphere(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {
	innerShellRadius = 18.52;
	ShellThickness = 1;
	ShellSweep = halfShell ? 90*deg : 180*deg;

	G4Translate3D shiftZIsoSphere(0, 0, (distDet + zLength / 2)*cm);
	G4Transform3D transformIsoSphere = rotPhi * rotTheta * shiftZIsoSphere;
	solidIsoSphere = new G4Sphere(
		"solidIsoSphere", innerShellRadius*cm, (innerShellRadius + ShellThickness)*cm, 
		0*deg, 360*deg, 0*deg, ShellSweep);
	logicIsoSphere = new G4LogicalVolume(solidIsoSphere, IsotopeShellMat,"logicIsoSphere");
	physIsoSphere = new G4PVPlacement(
		transformIsoSphere, logicIsoSphere, "physIsoSphere",logicWorld, false, 0, false);
}

void DetectorConstruction::ConstructShielding(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {
	// Shield 1 (Lead)
	Di1 = 1; // Inches
	Do1 = 5;
	zL1 = 2;
	DistOrigin1 = (zL1 / 2) + zLength / 2.54;
	G4Translate3D shiftZShield1(0, 0, (distDet + DistOrigin1 * 2.54)*cm);
	G4Transform3D transformShield1 = rotPhi * rotTheta * shiftZShield1;
	solidShield1 = new G4Tubs(
		"solidShield1", (Di1 * 2.54 / 2)*cm, (Do1 * 2.54 / 2)*cm, (zL1 * 2.54 / 2)*cm, 0*deg, 360*deg);
	logicShield1 = new G4LogicalVolume(solidShield1, Pb, "logicShield1");
	physShield1 = new G4PVPlacement(
		transformShield1, logicShield1, "physShield1", logicWorld, false, 0, false);

	// Shield 2 (Copper)
	Di2 = 4.125; // Inches
	Do2 = 4.25;
	zL2 = zLength / 2.54 + 0.5;
	DistOrigin2 = (zL2 / 2);
	G4Translate3D shiftZShield2(0, 0, (distDet + DistOrigin2 * 2.54)*cm);
	G4Transform3D transformShield2 = rotPhi * rotTheta * shiftZShield2;
	solidShield2 = new G4Tubs(
		"solidShield2", (Di2 * 2.54 / 2)*cm, (Do2 * 2.54 / 2)*cm, (zL2 * 2.54 / 2)*cm, 0*deg, 360*deg);
	logicShield2 = new G4LogicalVolume(solidShield2, Cu, "logicShield2");
	physShield2 = new G4PVPlacement(
		transformShield2, logicShield2, "physShield2", logicWorld, false, 0, false);

	//Shield 3 (Lead)
	Di3 = 5.125; 
	Do3 = outerShieldDiameter; // Inches
	zL3 = zL1 + zLength / 2.54; 
	DistOrigin3 = (zL3 / 2);
	G4Translate3D shiftZShield3(0, 0, (distDet + DistOrigin3 * 2.54)*cm);
	G4Transform3D transformShield3 = rotPhi * rotTheta * shiftZShield3;
	solidShield3 = new G4Tubs(
		"solidShield3", (Di3 * 2.54 / 2)*cm, (Do3 * 2.54 / 2)*cm, (zL3 * 2.54 / 2)*cm, 0*deg, 360*deg);
	logicShield3 = new G4LogicalVolume(solidShield3, Pb, "logicShield3");
	physShield3 = new G4PVPlacement(
		transformShield3, logicShield3, "physShield3", logicWorld, false, 0, false);
}

void DetectorConstruction::ConstructTarget() {
	TarLength = 10*cm;
	TarWidth = 1*um;
	solidTarget = new G4Box("solidTarget", TarLength / 2, TarLength / 2, TarWidth / 2);
	logicTarget = new G4LogicalVolume(solidTarget, BC, "logicTarget");
	physTarget = new G4PVPlacement(
		0, G4ThreeVector(0, 0, 0), logicTarget, "physTarget", logicWorld, false, 0, false);
}

void DetectorConstruction::ConstructTarget(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {

	G4Transform3D transformTarget = rotPhi * rotTheta;
	TarLength = 10*cm;
	// TarWidth = 10*um;
	TarWidth = 1*mm;
	solidTargetCyl = new G4Tubs("solidTargetCyl", 0, TarLength / 2, TarWidth / 2, 0*deg, 360*deg);
	logicTargetCyl = new G4LogicalVolume(solidTargetCyl, BC, "logicTargetCyl");
	physTargetCyl = new G4PVPlacement(
		transformTarget, logicTargetCyl, "physTargetCyl", logicWorld, false, 0, false);
}

void DetectorConstruction::ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {

	G4Translate3D shiftZSiDet(0, 0, distDet*cm);
	G4Transform3D transformSiDet = rotPhi * rotTheta * shiftZSiDet;
	solidSiDet = new G4Tubs("solidSiDet", 0, 2.5*cm, 1*cm, 0*deg, 360*deg);
	logicSiDet = new G4LogicalVolume(solidSiDet, Si, "logicSiDet");
	fSiScoringVolume = logicSiDet;
	physSiDet = new G4PVPlacement(
		transformSiDet, logicSiDet, "physSiDet", logicWorld, false, 0, false);
}

void DetectorConstruction::ConstructHPGeDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi) {
	G4Translate3D shiftZGeDet(0, 0, (distDet + zLength / 2)*cm);
	G4Transform3D transformGeDet = rotPhi * rotTheta * shiftZGeDet;
	solidGeDet = new G4Tubs("solidGeDet", 0, (dDiameter / 2)*cm, (zLength / 2)*cm, 0*deg, 360*deg);
	logicGeDet = new G4LogicalVolume(solidGeDet, Ge, "logicGeDet");
	fGeScoringVolume = logicGeDet;
	physGeDet = new G4PVPlacement(transformGeDet, logicGeDet, "physGeDet", logicWorld, false, 0, false);
}

G4VPhysicalVolume* DetectorConstruction::Construct() {

	distDet = 10;

	solidWorld = new G4Box("World", fWorldSize / 2, fWorldSize / 2, fWorldSize / 2);
	logicWorld = new G4LogicalVolume(solidWorld, Galactic, "World");
	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);

	G4RotateY3D rotThetaSi(45*deg);
	G4RotateZ3D rotPhiSi(-45*deg);
	ConstructTarget(rotThetaSi, rotPhiSi);
	ConstructSiDetector(rotThetaSi, rotPhiSi);
	
	G4RotateY3D rotThetaHPGe1(90*deg);
	G4RotateZ3D rotPhiHPGe1(90*deg);
	G4RotateY3D rotThetaHPGe2(90*deg);
	G4RotateZ3D rotPhiHPGe2(-90*deg);
	ConstructHPGeDetector(rotThetaHPGe1, rotPhiHPGe1);
	ConstructHPGeDetector(rotThetaHPGe2, rotPhiHPGe2);

	return physWorld;

	// Failed first attempt at rotation
	/* G4double _50DegInRad = 50 * 3.14159265 / 180;
	G4double _120DegInRad = 120 * 3.14159265 / 180;

	for (G4int i = 0; i < 3; i++) { 
		if (i == 0) {
			G4double t = _50DegInRad;
			G4double p = 0;
			G4ThreeVector spherical(sin(t) * cos(p), sin(t) * sin(p), cos(t));
			G4RotationMatrix* Rotation = new G4RotationMatrix();
			Rotation->rotateY(-t*rad / 5 * 4);
			Rotation->rotateX(p*rad);

			// HPGe Detector
			G4double distDet = 20;
			solidDet = new G4Tubs("Det", 0, (dDiameter / 2)*cm, (zLength / 2)*cm, 0*deg, 360*deg);
			logicDet = new G4LogicalVolume(solidDet, Ge, "Det");
			fScoringVolume = logicDet;
			physDet = new G4PVPlacement(
				Rotation, (distDet + zLength / 2)*cm * spherical, 
				logicDet, "Det", logicWorld, false, 0, false);

			// Al Shield (for e+e- fix)
			// Shell
			G4double distAl1 = zLength / 2;
			Sidel = distAl1 + 1;
			solidAl1 = new G4Tubs(
				"solidAl1", (dDiameter / 2 + 1)*cm, (dDiameter / 2 + 1.1)*cm, Sidel*cm, 0*deg, 360*deg);
			logicAl1 = new G4LogicalVolume(solidAl1, Al, "logicAl1");
			physAl1 = new G4PVPlacement(
				Rotation, (distDet + distAl1)*cm * spherical,
				logicAl1, "physAl1", logicWorld, false, 0, false);

			// Caps
			Capl = .1;
			G4double distAl2_1 = (zLength / 2) + (Sidel + Capl / 2);
			G4double distAl2_2 = (zLength / 2) - (Sidel + Capl / 2);
			solidAl2 = new G4Tubs("solidAl1", 0, (dDiameter / 2 + 1.1)*cm, Capl*mm / 2, 0*deg, 360*deg);
			logicAl2 = new G4LogicalVolume(solidAl2, Al, "logicAl2");
			physAl2_1 = new G4PVPlacement(
				Rotation, (distDet + distAl2_1)*cm * spherical, 
				logicAl2,"physAl2_1", logicWorld, false, 0, false);
			physAl2_2 = new G4PVPlacement(
				Rotation, (distDet + distAl2_2)*cm * spherical, 
				logicAl2, "physAl2_2", logicWorld, false, 0, false);

			// Isotope Shell
			innerShellRadius = 18.52;
			ShellThickness = 1;
			ShellSweep = halfShell ? 90*deg : 180*deg;

			solidIsoSphere = new G4Sphere(
				"solidIsoSphere", innerShellRadius*cm, (innerShellRadius + ShellThickness)*cm, 
				0*deg, 360*deg, 0*deg, ShellSweep);
			logicIsoSphere = new G4LogicalVolume(solidIsoSphere, IsotopeShellMat,"logicIsoSphere");
			physIsoSphere = new G4PVPlacement(
				Rotation, (distDet + zLength / 2)*cm * spherical, 
				logicIsoSphere, "physIsoSphere",logicWorld, false, 0, false);

			//Shielding
			if (isShielded) {
				// Shield 1 (Lead)
				Di1 = 1; // Inches
				Do1 = 5;
				zL1 = 2;
				DistOrigin1 = (zL1 / 2) + zLength / 2.54 + 1; // +1 will be changed
				solidShield1 = new G4Tubs(
					"solidShield1", (Di1 * 2.54 / 2)*cm, (Do1 * 2.54 / 2)*cm, (zL1 * 2.54 / 2)*cm, 0*deg, 360*deg);
				logicShield1 = new G4LogicalVolume(solidShield1, Pb, "logicShield1");
				physShield1 = new G4PVPlacement(
					Rotation, (distDet + DistOrigin1 * 2.54)*cm * spherical, 
					logicShield1, "physShield1", logicWorld, false, 0, false);

				// Shield 2 (Copper)
				Di2 = 4.125; // Inches
				Do2 = 4.25;
				zL2 = zLength / 2.54 + 0.5; //This will be changed
				DistOrigin2 = (zL2 / 2);
				solidShield2 = new G4Tubs(
					"solidShield2", (Di2 * 2.54 / 2)*cm, (Do2 * 2.54 / 2)*cm, (zL2 * 2.54 / 2)*cm, 0*deg, 360*deg);
				logicShield2 = new G4LogicalVolume(solidShield2, Cu, "logicShield2");
				physShield2 = new G4PVPlacement(
					Rotation, (distDet + DistOrigin2 * 2.54)*cm * spherical,
					logicShield2, "physShield2", logicWorld, false, 0, false);

				//Shield 3 (Lead)
				Di3 = 5.125; //Inches This will be changed
				Do3 = outerShieldDiameter; //Inches
				zL3 = zL1 + zLength / 2.54 + 1; //This will be changed
				DistOrigin3 = (zL3 / 2); //This will be changed
				solidShield3 = new G4Tubs(
					"solidShield3", (Di3 * 2.54 / 2)*cm, (Do3 * 2.54 / 2)*cm, (zL3 * 2.54 / 2)*cm, 0*deg, 360*deg);
				logicShield3 = new G4LogicalVolume(solidShield3, Pb, "logicShield3");
				physShield3 = new G4PVPlacement(
					Rotation, (distDet + DistOrigin3 * 2.54)*cm * spherical, 
					logicShield3, "physShield3", logicWorld, false, 0, false);
			}
		}
		if (i == 1) {
			G4double t = _50DegInRad;
			G4double p = _120DegInRad;
			G4ThreeVector spherical(sin(t) * cos(p), sin(t) * sin(p), cos(t));
			// G4ThreeVector spherical(0, 0, 0);
			G4RotationMatrix* Rotation = new G4RotationMatrix();
			Rotation->rotateY(p*rad / 4);
			Rotation->rotateX(t*rad / 5 * 4);
			

			// HPGe Detector
			G4double distDet = 20;
			solidDet = new G4Tubs("Det", 0, (dDiameter / 2)*cm, (zLength / 2)*cm, 0*deg, 360*deg);
			logicDet = new G4LogicalVolume(solidDet, Ge, "Det");
			fScoringVolume = logicDet;
			physDet = new G4PVPlacement(
				Rotation, (distDet + zLength / 2)*cm * spherical, 
				logicDet, "Det", logicWorld, false, 0, false);

			// Al Shield (for e+e- fix)
			// Shell
			G4double distAl1 = zLength / 2;
			Sidel = distAl1 + 1;
			solidAl1 = new G4Tubs(
				"solidAl1", (dDiameter / 2 + 1)*cm, (dDiameter / 2 + 1.1)*cm, Sidel*cm, 0*deg, 360*deg);
			logicAl1 = new G4LogicalVolume(solidAl1, Al, "logicAl1");
			physAl1 = new G4PVPlacement(
				Rotation, (distDet + distAl1)*cm * spherical,
				logicAl1, "physAl1", logicWorld, false, 0, false);

			// Caps
			Capl = .1;
			G4double distAl2_1 = (zLength / 2) + (Sidel + Capl / 2);
			G4double distAl2_2 = (zLength / 2) - (Sidel + Capl / 2);
			solidAl2 = new G4Tubs("solidAl1", 0, (dDiameter / 2 + 1.1)*cm, Capl*mm / 2, 0*deg, 360*deg);
			logicAl2 = new G4LogicalVolume(solidAl2, Al, "logicAl2");
			physAl2_1 = new G4PVPlacement(
				Rotation, (distDet + distAl2_1)*cm * spherical, 
				logicAl2,"physAl2_1", logicWorld, false, 0, false);
			physAl2_2 = new G4PVPlacement(
				Rotation, (distDet + distAl2_2)*cm * spherical, 
				logicAl2, "physAl2_2", logicWorld, false, 0, false);

			// Isotope Shell
			innerShellRadius = 18.52;
			ShellThickness = 1;
			ShellSweep = halfShell ? 90*deg : 180*deg;

			solidIsoSphere = new G4Sphere(
				"solidIsoSphere", innerShellRadius*cm, (innerShellRadius + ShellThickness)*cm, 
				0*deg, 360*deg, 0*deg, ShellSweep);
			logicIsoSphere = new G4LogicalVolume(solidIsoSphere, IsotopeShellMat,"logicIsoSphere");
			physIsoSphere = new G4PVPlacement(
				Rotation, (distDet + zLength / 2)*cm * spherical, 
				logicIsoSphere, "physIsoSphere",logicWorld, false, 0, false);

			// Shielding
			if (isShielded) {
				// Shield 1 (Lead)
				Di1 = 1; // Inches
				Do1 = 5;
				zL1 = 2;
				DistOrigin1 = (zL1 / 2) + zLength / 2.54 + 1; // +1 will be changed
				solidShield1 = new G4Tubs(
					"solidShield1", (Di1 * 2.54 / 2)*cm, (Do1 * 2.54 / 2)*cm, (zL1 * 2.54 / 2)*cm, 0*deg, 360*deg);
				logicShield1 = new G4LogicalVolume(solidShield1, Pb, "logicShield1");
				physShield1 = new G4PVPlacement(
					Rotation, (distDet + DistOrigin1 * 2.54)*cm * spherical, 
					logicShield1, "physShield1", logicWorld, false, 0, false);

				// Shield 2 (Copper)
				Di2 = 4.125; // Inches
				Do2 = 4.25;
				zL2 = zLength / 2.54 + 0.5; //This will be changed
				DistOrigin2 = (zL2 / 2);
				solidShield2 = new G4Tubs(
					"solidShield2", (Di2 * 2.54 / 2)*cm, (Do2 * 2.54 / 2)*cm, (zL2 * 2.54 / 2)*cm, 0*deg, 360*deg);
				logicShield2 = new G4LogicalVolume(solidShield2, Cu, "logicShield2");
				physShield2 = new G4PVPlacement(
					Rotation, (distDet + DistOrigin2 * 2.54)*cm * spherical,
					logicShield2, "physShield2", logicWorld, false, 0, false);

				// Shield 3 (Lead)
				Di3 = 5.125; //Inches This will be changed
				Do3 = outerShieldDiameter; // Inches
				zL3 = zL1 + zLength / 2.54 + 1; // This will be changed
				DistOrigin3 = (zL3 / 2); // This will be changed
				solidShield3 = new G4Tubs(
					"solidShield3", (Di3 * 2.54 / 2)*cm, (Do3 * 2.54 / 2)*cm, (zL3 * 2.54 / 2)*cm, 0*deg, 360*deg);
				logicShield3 = new G4LogicalVolume(solidShield3, Pb, "logicShield3");
				physShield3 = new G4PVPlacement(
					Rotation, (distDet + DistOrigin3 * 2.54)*cm * spherical, 
					logicShield3, "physShield3", logicWorld, false, 0, false);
			}
		}
		if (i == 2) {
			G4double t = _50DegInRad;
			G4double p = 2 * _120DegInRad;
			G4ThreeVector spherical(sin(t) * cos(p), sin(t) * sin(p), cos(t));
			// G4ThreeVector spherical(0, 0, 0);
			G4RotationMatrix* Rotation = new G4RotationMatrix();
			Rotation->rotateY(t*rad / 5 * 4);
			Rotation->rotateX(-p*rad / 8);

			// HPGe Detector
			G4double distDet = 20;
			solidDet = new G4Tubs("Det", 0, (dDiameter / 2)*cm, (zLength / 2)*cm, 0*deg, 360*deg);
			logicDet = new G4LogicalVolume(solidDet, Ge, "Det");
			fScoringVolume = logicDet;
			physDet = new G4PVPlacement(
				Rotation, (distDet + zLength / 2)*cm * spherical, 
				logicDet, "Det", logicWorld, false, 0, false);

			// Al Shield (for e+e- fix)
			// Shell
			G4double distAl1 = zLength / 2;
			Sidel = distAl1 + 1;
			solidAl1 = new G4Tubs(
				"solidAl1", (dDiameter / 2 + 1)*cm, (dDiameter / 2 + 1.1)*cm, Sidel*cm, 0*deg, 360*deg);
			logicAl1 = new G4LogicalVolume(solidAl1, Al, "logicAl1");
			physAl1 = new G4PVPlacement(
				Rotation, (distDet + distAl1)*cm * spherical,
				logicAl1, "physAl1", logicWorld, false, 0, false);

			// Caps
			Capl = .1;
			G4double distAl2_1 = (zLength / 2) + (Sidel + Capl / 2);
			G4double distAl2_2 = (zLength / 2) - (Sidel + Capl / 2);
			solidAl2 = new G4Tubs("solidAl1", 0, (dDiameter / 2 + 1.1)*cm, Capl*mm / 2, 0*deg, 360*deg);
			logicAl2 = new G4LogicalVolume(solidAl2, Al, "logicAl2");
			physAl2_1 = new G4PVPlacement(
				Rotation, (distDet + distAl2_1)*cm * spherical, 
				logicAl2,"physAl2_1", logicWorld, false, 0, false);
			physAl2_2 = new G4PVPlacement(
				Rotation, (distDet + distAl2_2)*cm * spherical, 
				logicAl2, "physAl2_2", logicWorld, false, 0, false);

			// Isotope Shell
			// innerShellRadius = 18.52;
			// ShellThickness = 1;
			// ShellSweep = halfShell ? 90*deg : 180*deg;

			// solidIsoSphere = new G4Sphere(
			// 	"solidIsoSphere", innerShellRadius*cm, (innerShellRadius + ShellThickness)*cm, 
			// 	0*deg, 360*deg, 0*deg, ShellSweep);
			// logicIsoSphere = new G4LogicalVolume(solidIsoSphere, IsotopeShellMat,"logicIsoSphere");
			// physIsoSphere = new G4PVPlacement(
			// 	Rotation, (distDet + zLength / 2)*cm * spherical, 
			// 	logicIsoSphere, "physIsoSphere",logicWorld, false, 0, false);

			// Shielding
			if (isShielded) {
				// Shield 1 (Lead)
				Di1 = 1; // Inches
				Do1 = 5;
				zL1 = 2;
				DistOrigin1 = (zL1 / 2) + zLength / 2.54 + 1; // +1 will be changed
				solidShield1 = new G4Tubs(
					"solidShield1", (Di1 * 2.54 / 2)*cm, (Do1 * 2.54 / 2)*cm, (zL1 * 2.54 / 2)*cm, 0*deg, 360*deg);
				logicShield1 = new G4LogicalVolume(solidShield1, Pb, "logicShield1");
				physShield1 = new G4PVPlacement(
					Rotation, (distDet + DistOrigin1 * 2.54)*cm * spherical, 
					logicShield1, "physShield1", logicWorld, false, 0, false);

				// Shield 2 (Copper)
				Di2 = 4.125; // Inches
				Do2 = 4.25;
				zL2 = zLength / 2.54 + 0.5; //This will be changed
				DistOrigin2 = (zL2 / 2);
				solidShield2 = new G4Tubs(
					"solidShield2", (Di2 * 2.54 / 2)*cm, (Do2 * 2.54 / 2)*cm, (zL2 * 2.54 / 2)*cm, 0*deg, 360*deg);
				logicShield2 = new G4LogicalVolume(solidShield2, Cu, "logicShield2");
				physShield2 = new G4PVPlacement(
					Rotation, (distDet + DistOrigin2 * 2.54)*cm * spherical,
					logicShield2, "physShield2", logicWorld, false, 0, false);

				// Shield 3 (Lead)
				Di3 = 5.125; //Inches This will be changed
				Do3 = outerShieldDiameter; // Inches
				zL3 = zL1 + zLength / 2.54 + 1; // This will be changed
				DistOrigin3 = (zL3 / 2); // This will be changed
				solidShield3 = new G4Tubs(
					"solidShield3", (Di3 * 2.54 / 2)*cm, (Do3 * 2.54 / 2)*cm, (zL3 * 2.54 / 2)*cm, 0*deg, 360*deg);
				logicShield3 = new G4LogicalVolume(solidShield3, Pb, "logicShield3");
				physShield3 = new G4PVPlacement(
					Rotation, (distDet + DistOrigin3 * 2.54)*cm * spherical, 
					logicShield3, "physShield3", logicWorld, false, 0, false);
			}
		} 
	} */ 
}

