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
	
	fMessenger = new G4GenericMessenger(this, "/target/", "Target control");
	fMessengerShell = new G4GenericMessenger(this, "/shell/", "Isotope shell control");
	fMessengerAperture = new G4GenericMessenger(this, "/aperture/", "Aperture control");

	fMessenger->DeclareMethod("setThickness", &DetectorConstruction::ScaleTargetThickness,"Set target thickness");
	fMessenger->DeclareMethod("setBesselNu", &DetectorConstruction::SetBesselNu, "Set Bessel function order");
	fMessengerShell->DeclareMethod("setShellType", &DetectorConstruction::SetShellType, "Set the type of shell");
	fMessengerAperture->DeclareMethod("shiftAperture", &DetectorConstruction::ShiftAperture, "Shift the aperture in microns");
	
	DefineParameters();
	DefineMaterials();
}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::DefineParameters() {

	// Origin centered on sphere/target center

	fWorldSize = 5*m;
	dTarget = 10*cm;
	zTarget = 106*nm;
	// zTarget = 1*cm;
	dGe = 8*cm;
	// dSi = 2.8*cm; // based on irl specs (I think?)
	dSi = 3.49*cm;   // based on CAD model, not that it matters too much (I think?), both cover the collimator shadow
	zGe = 3.14*cm;
	zSi = 300*um;
	distDetGe = 22*cm;
	distDetSi = 21*cm;
	zPbBackShield = 10.6*cm;
	zPbBackShield = 8.5725*cm;
	shellType = 0;
	nu = 0;
	radial_n = 1;

	// Ge Casing z = 15 cm, d = 10.2 cm irl

	SiApertureNormals = { // with no tilts
		{"MR", G4ThreeVector(0.766, 0, 0.6428)}, 
		{"TL", G4ThreeVector(-0.383, 0.6634, 0.6428)}, 
		{"BL", G4ThreeVector(-0.383, -0.6634, 0.6428)},
		{"ML", G4ThreeVector(-0.92215, 0, 0.38685)}, 
		{"TR", G4ThreeVector(0.4611, 0.7986, 0.3869)}, 
		{"BR", G4ThreeVector(0.4611, -0.7986, 0.3869)} 
	};
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
   	elH = new G4Element("ElH", "H", 1);
  	elH->AddIsotope(H1, 1);

	H2 = new G4Material("Molecular Hydrogen", 0.071, 1);
	H2->AddElement(elH, 2);

    G4double density = 0.07085 * g/cm3; 
    G4double temperature = 20.28 * kelvin;
    G4double pressure = 1.0 * atmosphere;
    LH2 = new G4Material("LH2", density, 1, kStateLiquid, temperature, pressure);
    LH2->AddElement(elH, 2);

	IsotopeShellMatH = new G4Material("IsotopeShellMatH", 0.07085 * g/cm3, 1);
	IsotopeShellMatH->AddElement(elH, 1);

	// Pulled from Ian's code. Not necessarily accurate, since it uses isotopes pertinent to UCNtau background
	IsotopeShellMat = new G4Material("IsotopeShellMat", 7.85 * g/cm3, 6); 
	IsotopeShellMat->AddElement(elCl, .0834);
	IsotopeShellMat->AddElement(elCr, .2499);
	IsotopeShellMat->AddElement(elFe, .0834);
	IsotopeShellMat->AddElement(elNi, .4165);
	IsotopeShellMat->AddElement(elNa, .0834);
	IsotopeShellMat->AddElement(elMn, .0834);

	K40 = new G4Isotope("K40", 19, 40);
	elK = new G4Element("Potassium", "K", 1);
	elK->AddIsotope(K40, 1);

	// IsotopeShellMat = new G4Material("IsotopeShellMat", 0.86 * g/cm3, 1);
	// IsotopeShellMat->AddElement(elK, 1);

	Co60 = new G4Isotope("Co60", 27, 60);
	G4Element* elCo = new G4Element("Cobalt", "Co", 1);
	elCo->AddIsotope(Co60, 1);

	// IsotopeShellMat = new G4Material("IsotopeShellMat", 8.9 * g/cm3, 1);
	// IsotopeShellMat->AddElement(elCo, 1); 

	// IsotopeShellMat = new G4Material("IsotopeShellMat", 7.85 * g/cm3, 2);
	// IsotopeShellMat->AddElement(elCo, .5);
	// IsotopeShellMat->AddElement(elK, .5);

	targetMat = B10;

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
	logicTargetCyl->SetMaterial(B10);
	physTargetCyl = new G4PVPlacement(
		transformTarget, logicTargetCyl, "physTargetCyl" + std::to_string(cpyNo), logicWorld, false, 100, false);
	logicTargetCyl->SetVisAttributes(G4VisAttributes(G4Color(0.7, 0.4, 0.2, 1)));
	cpyNo++;
}

void DetectorConstruction::ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, int& cpyNo) {

	G4Translate3D shiftZSiDet(0, 0, distDetSi + zSi / 2);
	G4Transform3D transformSiDet = rotPhi * rotTheta * shiftZSiDet;
	auto* solidSiDet = new G4Tubs("solidSiDet" + std::to_string(cpyNo), 0, dSi / 2, zSi / 2, 0*deg, 360*deg);
	auto* logicSiDet = new G4LogicalVolume(solidSiDet, Si, "logicSiDet" + std::to_string(cpyNo));
	auto* physSiDet = new G4PVPlacement(
		transformSiDet, logicSiDet, "physSiDet" + std::to_string(cpyNo), logicWorld, false, SiDetCpyNo, false);
	SiDets.insert(logicSiDet);
	logicSiDet->SetVisAttributes(G4VisAttributes(G4Color::Cyan()));
	cpyNo++;
	SiDetCpyNo++;
}

void DetectorConstruction::ConstructHPGeDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, int& cpyNo) {
	
	G4Translate3D shiftZGeDet(0, 0, distDetGe + zGe / 2);
	G4Transform3D transformGeDet = rotPhi * rotTheta * shiftZGeDet;
	solidGeDet = new G4Tubs("solidGeDet" + std::to_string(cpyNo), 0, dGe / 2, zGe / 2, 0*deg, 360*deg);
	logicGeDet = new G4LogicalVolume(solidGeDet, Ge, "logicGeDet" + std::to_string(cpyNo));
	physGeDet = new G4PVPlacement(
		transformGeDet, logicGeDet, "physGeDet" + std::to_string(cpyNo), logicWorld, false, GeDetCpyNo, false);
	GeDets.insert(logicGeDet);
	logicGeDet->SetVisAttributes(G4VisAttributes(G4Color::Blue()));
	cpyNo++;
	GeDetCpyNo++;
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
		"solidPbBackShield" + std::to_string(cpyNo), 2.1*cm, 4.5*cm, zPbBackShield / 2, 0*deg, 360*deg);
	logicPbBackShield = new G4LogicalVolume(solidPbBackShield, Pb, "logicPbBackShield" + std::to_string(cpyNo));
	physPbBackShield = new G4PVPlacement(
		transformPbBackShield, logicPbBackShield, "physPbBackShield" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	logicPbBackShield->SetVisAttributes(G4VisAttributes(G4Colour(.36, .38, .45, 1.)));
	cpyNo++;
}

void DetectorConstruction::ConstructIsotopeShell(int& cpyNo) {
    	
	solidIsoSphere = new G4Sphere(
		"solidIsoSphere" + std::to_string(cpyNo), 49*cm, 50*cm, 0*deg, 360*deg, 0*deg, 172.5*deg);
	if (shellType == 0) {
		logicIsoSphere = new G4LogicalVolume(solidIsoSphere, IsotopeShellMatH, "logicIsoSphere" + std::to_string(cpyNo));
		logicIsoSphere->SetVisAttributes(G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.1)));
	} else if (shellType == 1) {
		logicIsoSphere = new G4LogicalVolume(solidIsoSphere, IsotopeShellMat, "logicIsoSphere" + std::to_string(cpyNo));
		logicIsoSphere->SetVisAttributes(G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1)));
	} else {
		G4Exception("DetectorConstruction::ConstructIsotopeShell", "InvalidShellType", FatalException,
			"Invalid shell type. Use 0 for hydrogen or 1 for whatever.");
	}
	physIsoSphere = new G4PVPlacement(
		0, G4ThreeVector(0, 0, 0), logicIsoSphere, "physIsoSphere" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	cpyNo++;
}

void DetectorConstruction::ConstructAtmosphere(G4int numLayers, G4int& cpyNo) {

	G4double initHeight = 45*cm;
	G4double layerHeight = ((fWorldSize / 2) - initHeight) / numLayers;

	for (G4int i = 0; i < numLayers; i++) {
		G4Box* solidAir = new G4Box("test", 100*cm + 100*(numLayers - i - 1)*cm, layerHeight / 2, 100*cm + 100*(numLayers - i - 1)*cm);
		G4LogicalVolume* logicAir = new G4LogicalVolume(solidAir, man->FindOrBuildMaterial("G4_AIR"), "logicAir" + std::to_string(cpyNo));
		logicAir->SetVisAttributes(G4VisAttributes(G4Color(1, 1, 1, .2)));
		G4PVPlacement* physAir = new G4PVPlacement(
			0, G4ThreeVector(0, initHeight + layerHeight*(i + .5), 0), logicAir, "physAtmosphere" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
		solidAtmosphere.push_back(solidAir);
		logicAtmosphere.push_back(logicAir);
		physAtmosphere.push_back(physAir);
		cpyNo++;
	}
}

void DetectorConstruction::ConstructNBSR(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, G4int& cpyNo) {

	G4Translate3D shiftZNBSR(0, 0, 1*m);
	G4Transform3D transformNBSR = rotPhi * rotTheta * shiftZNBSR;
	solidNBSR = new G4Tubs("solidLH2" + std::to_string(cpyNo), 0*cm, 5*cm, 5*cm, 0*deg, 360*deg);
	logicNBSR = new G4LogicalVolume(solidNBSR, LH2, "logicLH2" + std::to_string(cpyNo));
	physNBSR = new G4PVPlacement(
		transformNBSR, logicNBSR, "physLH2" + std::to_string(cpyNo), logicWorld, false, cpyNo, false);
	logicNBSR->SetVisAttributes(G4VisAttributes(G4Color::Cyan()));
	cpyNo++;
}

void DetectorConstruction::AddLocalAxes(G4LogicalVolume* parentLV, G4double length, G4double radius) {
            
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
	// auto xLV = new G4LogicalVolume(axisSolid, mat, "XAxisLV");
	// auto xVis = new G4VisAttributes(G4Colour::Red());
	// xVis->SetForceSolid(true);
	// xLV->SetVisAttributes(xVis);

	// auto rotX = new G4RotationMatrix();
	// rotX->rotateY(90.*deg);  // Z -> X

	// new G4PVPlacement(rotX, G4ThreeVector(length/2., 0, 0), xLV, "XAxisPV", parentLV, false, 0, false);

	// --- Y axis ---
	// auto yLV = new G4LogicalVolume(axisSolid, mat, "YAxisLV");
	// auto yVis = new G4VisAttributes(G4Colour::Green());
	// yVis->SetForceSolid(true);
	// yLV->SetVisAttributes(yVis);

	// auto rotY = new G4RotationMatrix();
	// rotY->rotateX(-90.*deg); // Z -> Y

	// new G4PVPlacement(rotY, G4ThreeVector(0, length/2., 0), yLV, "YAxisPV", parentLV, false, 0, false);
}

void DetectorConstruction::ConstructTriangle(G4GDMLParser* fParser) {

	// std::vector<G4ThreeVector> points = { // ideal
	// 	G4ThreeVector(9.025, 0, 0)*mm, 
	// 	G4ThreeVector(-4.512, 0, -7.815)*mm, 
	// 	G4ThreeVector(-4.512, 0, 7.815)*mm, 
	// 	G4ThreeVector(-14.464, 0, 0)*mm, 
	// 	G4ThreeVector(7.247, 0, -12.552)*mm,
	// 	G4ThreeVector(7.247, 0, 12.552)*mm 
	// };
	std::vector<G4ThreeVector> points = { // as measured
		G4ThreeVector(9.351, 0, 0.140)*mm, 
		G4ThreeVector(-5.385, 0, -6.070)*mm, 
		G4ThreeVector(-4.1, 0, 7.939)*mm, 
		G4ThreeVector(-14.365, 0, -0.203)*mm, 
		G4ThreeVector(3.343, 0, -12.072)*mm,
		G4ThreeVector(8.321, 0, 12.449)*mm
	};

	G4Sphere* point = new G4Sphere("Point", 0, 0.04*cm, 0., 360.*deg, 0., 180.*deg);
	G4LogicalVolume* motherVol = fParser->GetVolume("worldVOL");
	
	// ========== MR ========== //
	G4LogicalVolume* pointLV1 = new G4LogicalVolume(point, Galactic, "PointLV");
	pointLV1->SetVisAttributes(G4VisAttributes(G4Color::Cyan())); 
	new G4PVPlacement(nullptr, points[0], pointLV1, "PointPV", motherVol, false, 0, false);
	// ========== TL ========== //
	G4LogicalVolume* pointLV2 = new G4LogicalVolume(point, Galactic, "PointLV");
	pointLV2->SetVisAttributes(G4VisAttributes(G4Color::Green())); 
	new G4PVPlacement(nullptr, points[1], pointLV2, "PointPV", motherVol, false, 0, false);
	// ========== BL ========== //
	G4LogicalVolume* pointLV3 = new G4LogicalVolume(point, Galactic, "PointLV");
	pointLV3->SetVisAttributes(G4VisAttributes(G4Color::Blue())); 
	new G4PVPlacement(nullptr, points[2], pointLV3, "PointPV", motherVol, false, 0, false);
	// ========== ML ========== //
	G4LogicalVolume* pointLV4 = new G4LogicalVolume(point, Galactic, "PointLV");
	pointLV4->SetVisAttributes(G4VisAttributes(G4Color::Red())); 
	new G4PVPlacement(nullptr, points[3], pointLV4, "PointPV", motherVol, false, 0, false);
	// ========== TR ========== //
	G4LogicalVolume* pointLV5 = new G4LogicalVolume(point, Galactic, "PointLV");
	pointLV5->SetVisAttributes(G4VisAttributes(G4Color::Magenta()));
	new G4PVPlacement(nullptr, points[4], pointLV5, "PointPV", motherVol, false, 0, false);
	// ========== BR ========== // 
	G4LogicalVolume* pointLV6 = new G4LogicalVolume(point, Galactic, "PointLV");
	pointLV6->SetVisAttributes(G4VisAttributes(G4Color::Yellow())); 
	new G4PVPlacement(nullptr, points[5], pointLV6, "PointPV", motherVol, false, 0, false);

	G4RotationMatrix* rotX90 = new G4RotationMatrix();
	rotX90->rotateX(90*deg);
	G4Tubs* xzPlane = new G4Tubs("XZPlane", 0, 10*cm, 0.01*cm, 0., 360.*deg);
	G4LogicalVolume* xzLV = new G4LogicalVolume(xzPlane, Galactic, "XZPlaneLV");
	new G4PVPlacement(
		rotX90, G4ThreeVector(0, 0, 0), xzLV, "XZPlanePV", fParser->GetVolume("worldVOL"), false, 0, false);
}

void DetectorConstruction::ConstructBesselTarget(G4int nu, G4int radial_n) {

	nu = std::min(std::max(nu, 0), 19);
	radial_n = std::min(std::max(radial_n, 1), 3);
	G4cout << "Setting Bessel function order to: (" << nu << ", " << radial_n << ")" << G4endl;

	std::vector<std::vector<G4double>> alpha = 
	{	// n-th (n = 1, 2, 3) zero of J_nu for nu = 0, 1, 2, ... 19
		{ 2.40482556, 3.83170597, 5.1356223, 6.3801619, 7.58834243, 
		8.77148382, 9.93610952, 11.08637002, 12.22509226, 13.35430048, 
		14.47550069, 15.58984788, 16.69824993, 17.80143515, 18.89999795, 
		19.99443063, 21.08514611, 22.17249462, 23.25677609, 24.33824962 },
		{ 5.52007811, 7.01558667, 8.41724414, 9.76102313, 11.06470949, 
		12.3386042, 13.58929017, 14.82126873, 16.03777419, 17.24122038, 
		18.43346367, 19.6159669, 20.78990636, 21.95624407, 23.11577835, 
		24.26918003, 25.41701901, 26.55978414, 27.69789835, 28.83173035 },
		{ 8.65372791, 10.17346814, 11.61984117, 13.01520072, 14.37253667, 
		15.70017408, 17.00381967, 18.28758283, 19.55453643, 20.80704779, 
		22.04698536, 23.27585373, 24.49488504, 25.70510305, 26.90736898, 
		28.10241523, 29.2908707, 30.47327995, 31.65011815, 32.82180276 }
	};

	solidBesselTarget = new G4TessellatedSolid("BesselTarget");

	// set-up grid
	int Nr = 128;
	int Ntheta = 128;

	std::vector<G4double> rs(Nr);
	for (int i = 1; i <= Nr; i++) {
		G4double deltar = (double)i / Nr;
		rs[i-1] = deltar * dTarget / 2;
	}
	
	std::vector<G4double> thetas(Ntheta);
	for (int j = 0; j < Ntheta; j++) {
		thetas[j] = (2.0 * M_PI * j) / Ntheta;
	}

	// compute wave heights at each (r, theta)
	std::vector<std::vector<G4double>> zs = 
		std::vector<std::vector<G4double>>(Nr, std::vector<G4double>(Ntheta));
	
	G4double zmin = INFINITY;
	G4double zmax = -INFINITY;
	std::pair<int, int> minIndex;
	G4double A = zTarget;
	G4double phi0 = 0;
	for (int i = 0; i < Nr; i++) {
		for (int j = 0; j < Ntheta; j++) {
			zs[i][j] = A * std::cyl_bessel_j(nu, alpha[radial_n - 1][nu] * rs[i] / (dTarget / 2)) * 
				std::cos(nu * thetas[j] - phi0);
			if (zs[i][j] < zmin) { 
				zmin = zs[i][j];
				minIndex = {i, j};
			}
			if (zs[i][j] > zmax) {
				zmax = zs[i][j];
			}
		}
	}
	G4cout << "(zmin, zmax)\n\t|(" << zmin/nm << ", " << zmax/nm << ") nm\n\t|(" 
		   << zmin/A << ", " << zmax/A << ") amplitudes; A = " << A/nm << " nm" << G4endl;
	G4cout << "Bessel z range = " << (zmax - zmin)/nm << " nm" << G4endl;

	// convert to xyz and store in grid
	std::vector<std::vector<G4ThreeVector>> xyzgrid;
	for (int i = 0; i < Nr; i++) {
		std::vector<G4ThreeVector> ring;
		for (int j = 0; j < Ntheta; j++) {
			G4double x = rs[i] * std::cos(thetas[j]);
			G4double y = rs[i] * std::sin(thetas[j]);
			G4double z = zs[i][j];
			ring.push_back(G4ThreeVector(x, y, z));
		}
		xyzgrid.push_back(ring);
	}

	// special case for connecting to center'

	// original method: connect each point on inner ring to center with triangle
	// This works normally, but for Bessel 2 1, you get consistent errors, so another approach is needed
	G4ThreeVector targetCenter(0, 0, A*std::cyl_bessel_j(nu, 0));
	for (int j = 0; j < Ntheta; j++) {
		G4ThreeVector p1 = xyzgrid[0][j];
		G4ThreeVector p2 = xyzgrid[0][(j+1) % Ntheta];
		solidBesselTarget->AddFacet(new G4TriangularFacet(p1, p2, targetCenter, ABSOLUTE));
	}

	// new method: center patch
	// Step 1: make polygon center
	// G4int core_sides = 8; 
	// G4double core_radius = 10*um;
	// std::vector<G4double> core_theta(core_sides);
	// for (int j = 0; j < core_sides; j++) {
	// 	core_theta[j] = (2.0 * M_PI * j) / core_sides;
	// 	G4cout << core_theta[j] << " ";
	// }

	G4double buffer = 1*nm;
	G4ThreeVector closurePoint = G4ThreeVector(0, 0, -abs(xyzgrid[minIndex.first][minIndex.second].z()) - buffer);
	G4cout << "Closure point: " << closurePoint / nm << " nm" << G4endl;
	G4cout << "Edge point:    " << xyzgrid[Nr-1][0] / nm << " nm" << G4endl;
	// create triangles between adjacent points in the grid and add to tessellated solid
	for (int i = 0; i < Nr - 1; i++) {
		for (int j = 0; j < Ntheta; j++) {
			G4ThreeVector p1 = xyzgrid[i][j];
			G4ThreeVector p2 = xyzgrid[i+1][j];
			G4ThreeVector p3 = xyzgrid[i][(j+1) % Ntheta];
			G4ThreeVector p4 = xyzgrid[i+1][(j+1) % Ntheta];
			solidBesselTarget->AddFacet(new G4TriangularFacet(p1, p2, p3, ABSOLUTE));
			solidBesselTarget->AddFacet(new G4TriangularFacet(p2, p4, p3, ABSOLUTE));

			// check for outer ring points, extend down to closure and close the solid
			if (i == Nr - 2) { 
				G4ThreeVector p_base1 = p2 + closurePoint;
    			G4ThreeVector p_base2 = p4 + closurePoint;
				solidBesselTarget->AddFacet(new G4TriangularFacet(p_base1, p4, p2, ABSOLUTE));
				solidBesselTarget->AddFacet(new G4TriangularFacet(p4, p_base1, p_base2, ABSOLUTE));
				solidBesselTarget->AddFacet(new G4TriangularFacet(p_base2, p_base1, closurePoint, ABSOLUTE));
			}
		}
	}

	solidBesselTarget->SetSolidClosed(true);

	// if (!logicBesselTarget) {
	// 	logicBesselTarget = new G4LogicalVolume(solidBesselTarget, targetMat, "logicBesselTarget");
	// 	logicBesselTarget->SetVisAttributes(G4VisAttributes(G4Color(0.7, 0.4, 0.2, 1)));  // Bronze
	// } else {
	// 	logicBesselTarget->SetSolid(solidBesselTarget);
	// }
	auto help = new G4LogicalVolume(solidBesselTarget, targetMat, "logicBesselTarget");
	help->SetVisAttributes(G4VisAttributes(G4Color(0.7, 0.4, 0.2, 1)));  // Bronze
	if (!physBesselTarget) {
		physBesselTarget = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), help, "physBesselTarget", logicWorld, false, 0, false);
	} else {
		physBesselTarget->SetLogicalVolume(help);
	}
	// new G4PVPlacement(0, G4ThreeVector(0, 0, 0), help, "physBesselTarget", logicWorld, false, 0, false);
	
}

G4VPhysicalVolume* DetectorConstruction::Construct() {

	G4cout << "-------------------------------" << G4endl;
	G4cout << "Constructing Geometry . . ." << G4endl;

	if (solidBesselTarget) {
        G4SolidStore::GetInstance()->DeRegister(solidBesselTarget);
        delete solidBesselTarget; 
		solidBesselTarget = nullptr;
    }

	auto fParser = new G4GDMLParser();
	fParser->Read("../Rebirth3.gdml", false);

	// --- World --- //
	physWorld = fParser->GetWorldVolume();
	logicWorld = physWorld->GetLogicalVolume();
	logicWorld->SetVisAttributes(G4VisAttributes(G4Color(1, 0, 0, .05))); // transparent red

	// --- AG Chamber --- //
	G4LogicalVolume* AGDevice = fParser->GetVolume("AGDevice");
	AGDevice->SetMaterial(matSteel);

	// --- Al Cap --- //
	G4LogicalVolume* AlCap = fParser->GetVolume("AlCap");
	AlCap->SetMaterial(Al);
	AlCap->SetVisAttributes(G4VisAttributes(G4Color::Gray()));

	// --- Cu Wrap --- //
	G4LogicalVolume* CuWrap = fParser->GetVolume("CuWrap");
	CuWrap->SetMaterial(Cu);
	CuWrap->SetVisAttributes(G4VisAttributes(G4Color::Brown()));

	// --- Si Det Aperture --- //
	G4LogicalVolume* SiDetAperture = fParser->GetVolume("SiDetAperture");
	SiDetAperture->SetMaterial(matSteel);
	SiDetAperture->SetVisAttributes(G4VisAttributes(G4Color::Red()));
	// AddLocalAxes(SiDetAperture, 5*cm, 0.025*cm);

	// --- Si Collimator --- //
	G4LogicalVolume* SiTurret = fParser->GetVolume("SiTurret");
	SiTurret->SetMaterial(matSteel);
	SiTurret->SetVisAttributes(G4VisAttributes(G4Color::Green()));

	// --- Ge Mount --- //
	G4LogicalVolume* GeMount = fParser->GetVolume("HPGeMount");
	GeMount->SetMaterial(matSteel);

	// --- Carbon Window --- //
	G4LogicalVolume* CarbonWindow = fParser->GetVolume("CarbonWindow");
	CarbonWindow->SetMaterial(C);
	CarbonWindow->SetVisAttributes(G4VisAttributes(G4Color(.663, .663, .663, .2)));

	// --- Aperture Cover --- //
	G4LogicalVolume* ApertureCover = fParser->GetVolume("SiDetApertureCover");
	ApertureCover->SetMaterial(matSteel);

	G4int cpyNo = 10;
	GeDetCpyNo = 0;
	SiDetCpyNo = 0;

	// --- Front Ge Dets --- // 
	G4RotateY3D rotThetaHPGe1(-40*deg);
	G4RotateZ3D rotPhiHPGe1(0*deg);
	ConstructHPGeDetector(rotThetaHPGe1, rotPhiHPGe1, cpyNo);
	ConstructPbBackShield(rotThetaHPGe1, rotPhiHPGe1, cpyNo);

	G4RotateY3D rotThetaHPGe2(-40*deg);
	G4RotateZ3D rotPhiHPGe2(120*deg);
	ConstructHPGeDetector(rotThetaHPGe2, rotPhiHPGe2, cpyNo);
	ConstructPbBackShield(rotThetaHPGe2, rotPhiHPGe2, cpyNo);

	G4RotateY3D rotThetaHPGe3(-40*deg);
	G4RotateZ3D rotPhiHPGe3(-120*deg);
	ConstructHPGeDetector(rotThetaHPGe3, rotPhiHPGe3, cpyNo);
	ConstructPbBackShield(rotThetaHPGe3, rotPhiHPGe3, cpyNo);

	// --- Back Ge Dets --- //
	G4RotateY3D rotThetaHPGe4(140*deg);
	G4RotateZ3D rotPhiHPGe4(0*deg);
	ConstructHPGeDetector(rotThetaHPGe4, rotPhiHPGe4, cpyNo);
	ConstructPbBackShield(rotThetaHPGe4, rotPhiHPGe4, cpyNo);

	G4RotateY3D rotThetaHPGe5(140*deg);
	G4RotateZ3D rotPhiHPGe5(120*deg);
	ConstructHPGeDetector(rotThetaHPGe5, rotPhiHPGe5, cpyNo);
	ConstructPbBackShield(rotThetaHPGe5, rotPhiHPGe5, cpyNo);

	G4RotateY3D rotThetaHPGe6(140*deg);
	G4RotateZ3D rotPhiHPGe6(-120*deg);
	ConstructHPGeDetector(rotThetaHPGe6, rotPhiHPGe6, cpyNo);
	ConstructPbBackShield(rotThetaHPGe6, rotPhiHPGe6, cpyNo);

	// --- Si Dets --- //
	firstSiDetIdx = cpyNo; // for use in MySiDet to determine tuple numbers
	G4RotateY3D rotThetaSi1(50*deg);
	G4RotateZ3D rotPhiSi1(0*deg);
	ConstructSiDetector(rotThetaSi1, rotPhiSi1, cpyNo);

	G4RotateY3D rotThetaSi2(50*deg);
	G4RotateZ3D rotPhiSi2(120*deg);
	ConstructSiDetector(rotThetaSi2, rotPhiSi2, cpyNo);

	G4RotateY3D rotThetaSi3(50*deg);
	G4RotateZ3D rotPhiSi3(-120*deg);
	ConstructSiDetector(rotThetaSi3, rotPhiSi3, cpyNo);

	G4RotateY3D rotThetaSi4(-67.24*deg);
	G4RotateZ3D rotPhiSi4(0*deg);
	ConstructSiDetector(rotThetaSi4, rotPhiSi4, cpyNo);

	G4RotateY3D rotThetaSi5(-67.24*deg);
	G4RotateZ3D rotPhiSi5(120*deg);
	ConstructSiDetector(rotThetaSi5, rotPhiSi5, cpyNo);

	G4RotateY3D rotThetaSi6(-67.24*deg);
	G4RotateZ3D rotPhiSi6(-120*deg);
	ConstructSiDetector(rotThetaSi6, rotPhiSi6, cpyNo);

	// --- Background Contributors --- //
	// ConstructIsotopeShell(cpyNo);
	// ConstructAtmosphere(10, cpyNo);
	
	// G4RotateY3D rotThetaNBSR(0*deg);
	// G4RotateZ3D rotPhiNBSR(0*deg);
	// ConstructNBSR(rotThetaNBSR, rotPhiNBSR, cpyNo);

	ConstructBesselTarget(nu, radial_n);
	// ConstructTarget(0, 0, cpyNo);

	// Something to keep in mind to make visualization faster
	// // High-res mesh for tracking
	// G4TessellatedSolid* highResSolid = BuildBesselMesh(nu, 100); // 100 segments
	// G4LogicalVolume* logicBessel = new G4LogicalVolume(highResSolid, mat, "BesselLogic");

	// // Low-res mesh for looking at
	// G4TessellatedSolid* lowResSolid = BuildBesselMesh(nu, 20);   // 20 segments
	// G4LogicalVolume* logicBesselVis = new G4LogicalVolume(lowResSolid, mat, "BesselVis");

	// // Logic: Hide the heavy one, show the light one
	// logicBessel->SetVisAttributes(G4VisAttributes::GetInvisible());
	// logicBesselVis->SetVisAttributes(new G4VisAttributes(G4Colour(1,1,1)));

	// if (G4UImanager::GetUIpointer()->GetSession() != nullptr) {
	// 	G4Tubs* xzPlane = new G4Tubs("XZPlane", 0, dTarget/2, 1*nm, 0., 360.*deg);
	// 	G4LogicalVolume* xzLV = new G4LogicalVolume(xzPlane, Galactic, "XZPlaneLV");
	// 	new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), xzLV, "XZPlanePV", logicWorld, false, 0, false);
	// }
	

	G4cout << "Geometry Construction Complete . . ." << G4endl;
	G4cout << "-------------------------------" << G4endl;

	// printPhysicalVolumeStore(G4PhysicalVolumeStore::GetInstance());
	// printVolumeStore(G4LogicalVolumeStore::GetInstance());

	return physWorld; 
}

void DetectorConstruction::ConstructSDandField() {
	

	if (!GeDets.empty()) {
		int GeIdx = 0;
		for (auto& GeDet : GeDets) {
			MyGeDet* sensDetGe = new MyGeDet("GeSensDet" + std::to_string(GeIdx));
			GeDet->SetSensitiveDetector(sensDetGe);
			GeIdx++;
		}
	}
	if (!SiDets.empty()) {
		int SiIdx = 0;
		for (auto& SiDet : SiDets) {
			MySiDet* sensDetSi = new MySiDet("SiSensDet" + std::to_string(SiIdx));
			SiDet->SetSensitiveDetector(sensDetSi);
			SiIdx++;
		}
	}
}

