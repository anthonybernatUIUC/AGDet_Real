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

#include "detector.hh"

#include <set> 


class DetectorConstruction : public G4VUserDetectorConstruction {
  
	public:

		DetectorConstruction();
		~DetectorConstruction();

		std::set<G4LogicalVolume*> GetGeScoringVolumes() const { return GeDets; }
		std::set<G4LogicalVolume*> GetSiScoringVolumes() const { return SiDets; }
		G4LogicalVolume* GetTargetVolume() const { return logicTargetCyl; }
		G4int GetFirstSiDetIdx() const { return firstSiDetIdx; }
		G4double GetTargetWidth() { return zTarget; }

		void ScaleTargetWidth(G4double scale) { 
			zTarget *= scale; 
			G4RunManager::GetRunManager()->GeometryHasBeenModified();
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
		}

		void ShiftAperture(G4double shift, G4String det) {
			
			G4PhysicalVolumeStore* const pvStore = G4PhysicalVolumeStore::GetInstance();			

			G4VPhysicalVolume* vol = pvStore->GetVolume(det);
			G4cout << "Old " << det << " position: " << vol->GetTranslation() << G4endl;
			vol->SetTranslation(vol->GetTranslation() + shift*um*SiApertureNormals[det]);
			G4cout << "New " << det << " position: " << vol->GetTranslation() << G4endl;

			G4RunManager::GetRunManager()->GeometryHasBeenModified();
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
 
		G4int shellType;
		G4double fWorldSize, distDetGe, distDetSi, dTarget, zTarget, dGe, dSi, zGe, zSi, zPbBackShield, firstSiDetIdx;
		G4Material *Galactic, *Ge, *BC, *Si, *Al, *B10, *matSteel, *Pb, *Cu, *C, *IsotopeShellMatH, *IsotopeShellMat, *H2, *LH2;
		G4Element *elB10, *elMn, *elSi, *elCr, *elNi, *elFe, *elH, *elCl, *elNa, *elK, *elCo;
		G4Isotope *B10Iso, *Cl35, *Cr50, *Cr52, *Cr53, *Fe56, *Ni58, *Ni60, *K40, *Co60, *H1,
		*Ni62, *Ni63, *Ni64, *Na24, *Mn56;

		G4Box *solidWorld, *solidTarget;
		G4Tubs *solidGeDet, *solidSiDet, *solidTargetCyl, *solidGeMount, 
		*solidSiAperture, *solidPbBackShield, *solidNBSR;
		G4Sphere *solidIsoSphere;		
		
		G4LogicalVolume *logicWorld, *logicGeDet, *logicTarget, *logicSiAperture, *logicTargetCyl, 
		*logicSiDet, *logicGeMount, *logicPbBackShield, *logicNBSR, *logicIsoSphere = nullptr;

		G4VPhysicalVolume *physWorld, *physGeDet, *physSiDet, *physTarget, *physTargetCyl, 
		*physGeMount, *physSiAperture, *physPbBackShield, *physIsoSphere, *physNBSR;

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

};
