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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4GenericMessenger.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4PVPlacement.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4GDMLParser.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4UImanager.hh"
#include "G4UserLimits.hh"
#include "G4TessellatedSolid.hh"
#include "G4TriangularFacet.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4VisManager.hh"

#include "detector.hh"

#include <set> 
#include <cmath>


class DetectorConstruction : public G4VUserDetectorConstruction {
  
	public:

		DetectorConstruction();
		~DetectorConstruction();

		std::set<G4LogicalVolume*> GetGeScoringVolumes() const { return GeDets; }
		std::set<G4LogicalVolume*> GetSiScoringVolumes() const { return SiDets; }
		G4LogicalVolume* GetTargetVolume() const { return logicTargetCyl; }
		G4int GetFirstSiDetIdx() const { return firstSiDetIdx; }
		G4double GetTargetWidth() { return zTarget; }

		void ScaleTargetThickness(G4String type) { // Sets Amplitude of Bessel

			if (type == "thin" || type == "Thin") {
				zTarget = 106*nm;
				targetMat = B10;
				G4cout << "Target thickness set to thin: " << zTarget / nm << " nm" << G4endl;
			} else if (type == "thick" || type == "Thick") {
				zTarget = 1*mm;
				targetMat = BC;
				G4cout << "Target thickness set to thick: " << zTarget / um << " um" << G4endl;
			} else if (type == "demo" || type == "Demo") {
				zTarget = 1*cm;
				targetMat = BC;
				G4cout << "Target thickness set to demo: " << zTarget / cm << " cm" << G4endl;
			} else {
				zTarget = 106*nm;
				targetMat = B10;
				G4cout << "Invalid target thickness type. Setting to thin." << G4endl;
			}
			
			G4GeometryManager::GetInstance()->OpenGeometry();
			if (solidBesselTarget) {
				G4SolidStore::GetInstance()->DeRegister(solidBesselTarget);
				delete solidBesselTarget;
			}
			if (logicBesselTarget) {
				G4LogicalVolumeStore::GetInstance()->DeRegister(logicBesselTarget);
				delete logicBesselTarget;
			}
			if (physBesselTarget) {
				G4PhysicalVolumeStore::GetInstance()->DeRegister(physBesselTarget);
				delete physBesselTarget;
			}
			ConstructBesselTarget(nu, radial_n);

			G4GeometryManager::GetInstance()->CloseGeometry(true, false);
			G4RunManager* runManager = G4RunManager::GetRunManager();
			runManager->GeometryHasBeenModified();
			
			if (G4VisManager::GetConcreteInstance() != nullptr) {
				G4VisManager* visManager = G4VisManager::GetInstance();
				if (visManager && visManager->GetCurrentViewer()) {
					G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/rebuild");
				}
			}
		}

		void SetBesselNu(G4int newNu, G4int newN) {
			nu = newNu;
			radial_n = newN;
			G4cout << "Updating Bessel function order to: (" << nu << ", " << radial_n << ")" << G4endl;

			G4GeometryManager::GetInstance()->OpenGeometry();
			if (solidBesselTarget) {
				G4SolidStore::GetInstance()->DeRegister(solidBesselTarget);
				delete solidBesselTarget;
			}
			ConstructBesselTarget(nu, radial_n);
			G4GeometryManager::GetInstance()->CloseGeometry(true, false);

			G4RunManager* runManager = G4RunManager::GetRunManager();
			runManager->GeometryHasBeenModified();
			if (G4VisManager::GetConcreteInstance() != nullptr) {
				G4VisManager* visManager = G4VisManager::GetInstance();
				if (visManager && visManager->GetCurrentViewer()) {
					G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/rebuild");
				}
			}

			// G4RunManager* runManager = G4RunManager::GetRunManager();
			// runManager->GeometryHasBeenModified();
			// runManager->ReinitializeGeometry(true);
			// runManager->Initialize();

			// G4UImanager* ui = G4UImanager::GetUIpointer(); 
			// ui->ApplyCommand("/vis/scene/clear");
			// ui->ApplyCommand("/vis/scene/add/volume worldVOL_PV"); 
			// ui->ApplyCommand("/vis/scene/rebuild");
			// ui->ApplyCommand("/vis/viewer/rebuild");
			// ui->ApplyCommand("/vis/viewer/flush");
		}

		void SetShellType(G4int type) {
			if (!logicIsoSphere) return;
			shellType = type;
			if (shellType == 0) {
				logicIsoSphere->SetMaterial(IsotopeShellMatH);
				logicIsoSphere->SetVisAttributes(G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.1)));
				G4cout << "Isotope Shell Type: Hydrogen" << G4endl;
			} else {
				logicIsoSphere->SetMaterial(IsotopeShellMat);
				logicIsoSphere->SetVisAttributes(G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.1)));
				G4cout << "Isotope Shell Type: Mixed Isotopes" << G4endl;
			} 
			G4RunManager::GetRunManager()->GeometryHasBeenModified();
			G4RunManager::GetRunManager()->ReinitializeGeometry(); 
		}

		void ShiftAperture(G4double shift, G4String det) {
			
			G4PhysicalVolumeStore* const pvStore = G4PhysicalVolumeStore::GetInstance();			
			G4VPhysicalVolume* vol = pvStore->GetVolume(det);
			G4cout << "Old " << det << " position: " << vol->GetTranslation() << G4endl;
			vol->SetTranslation(vol->GetTranslation() + shift*um*SiApertureNormals[det]);
			G4cout << "New " << det << " position: " << vol->GetTranslation() << G4endl;

			G4RunManager::GetRunManager()->GeometryHasBeenModified();
			G4RunManager::GetRunManager()->ReinitializeGeometry();
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

		virtual G4VPhysicalVolume* Construct();                 
		virtual G4double GetWorldSize() { return fWorldSize; } 
		virtual void ConstructSDandField();
		
  	private:
 
		G4int shellType, nu, radial_n, SiDetCpyNo, GeDetCpyNo;
		G4double fWorldSize, distDetGe, distDetSi, dTarget, zTarget, dGe, dSi, zGe, zSi, zPbBackShield, firstSiDetIdx;
		G4Material *Galactic, *Ge, *BC, *Si, *Al, *B10, *matSteel, *Pb, *Cu, *C, *IsotopeShellMatH, *IsotopeShellMat, *H2, *LH2, *targetMat;
		G4Element *elB10, *elMn, *elSi, *elCr, *elNi, *elFe, *elH, *elCl, *elNa, *elK, *elCo;
		G4Isotope *B10Iso, *Cl35, *Cr50, *Cr52, *Cr53, *Fe56, *Ni58, *Ni60, *K40, *Co60, *H1,
		*Ni62, *Ni63, *Ni64, *Na24, *Mn56;

		G4Box *solidWorld, *solidTarget;
		G4Tubs *solidGeDet, *solidSiDet, *solidTargetCyl, *solidGeMount, 
		*solidSiAperture, *solidPbBackShield, *solidNBSR;
		G4Sphere *solidIsoSphere;	
		G4TessellatedSolid* solidBesselTarget;	
		
		G4LogicalVolume *logicWorld, *logicGeDet, *logicTarget, *logicSiAperture, *logicTargetCyl, 
		*logicSiDet, *logicGeMount, *logicPbBackShield, *logicNBSR, *logicIsoSphere = nullptr, *logicBesselTarget;

		G4VPhysicalVolume *physWorld, *physGeDet, *physSiDet, *physTarget, *physTargetCyl, 
		*physGeMount, *physSiAperture, *physPbBackShield, *physIsoSphere, *physNBSR, *physBesselTarget;

		G4GenericMessenger *fMessenger = nullptr, *fMessengerShell = nullptr, *fMessengerAperture = nullptr;
		G4NistManager *man;
		G4GDMLParser* fParser;
		MySiDet* sensDetSi;
		MyGeDet* sensDetGe;

		std::set<G4LogicalVolume*> GeDets, SiDets;
		std::vector<G4Box*> solidAtmosphere;
		std::vector<G4LogicalVolume*> logicAtmosphere;
		std::vector<G4VPhysicalVolume*> physAtmosphere;
		std::map<G4String, G4ThreeVector> SiApertureNormals;		

		void DefineMaterials();
		void DefineParameters();
		void ConstructHPGeDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, G4int& cpyNo);
		void ConstructHPGeDetectorXYZ(G4RotateX3D rotX, G4RotateY3D rotY, G4RotateZ3D rotZ, G4int& cpyNo);
		void ConstructAlShield(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructIsoSphere(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructShielding(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructTarget(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, G4int& cpyNo);
		void ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, G4int& cpyNo);
		void ConstructPbBackShield(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, G4int& cpyNo);
		void ConstructIsotopeShell(G4int& cpyno);
		void ConstructAtmosphere(G4int numLayers, G4int& cpyNo);
		void ConstructNBSR(G4RotateY3D rotTheta, G4RotateZ3D rotPhi, G4int& cpyNo);
		void AddLocalAxes(G4LogicalVolume* parentLV, G4double length, G4double radius);
		void ConstructTarget();
		void ConstructTriangle(G4GDMLParser* fParser);
		void ConstructBesselTarget(G4int nu, G4int n);
};
