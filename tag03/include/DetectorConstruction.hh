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

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

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


class DetectorConstruction : public G4VUserDetectorConstruction {
  
	public:
  
		DetectorConstruction();
		~DetectorConstruction();

		G4LogicalVolume* GetGeScoringVolume() const { return fGeScoringVolume; }
		G4LogicalVolume* GetSiScoringVolume() const { return fSiScoringVolume; }
		G4LogicalVolume* GetTargetVolume() const { return logicTargetCyl; }
		G4double GetTargetWidth() { return TarWidth; }
		void ScaleTargetWidth(G4double scale) { 
			TarWidth *= scale; 
			G4RunManager::GetRunManager()->GeometryHasBeenModified();
		}

		virtual G4VPhysicalVolume* Construct();                 
		virtual G4double GetWorldSize() { return fWorldSize; } 
		virtual void ConstructSDandField();
		
  	private:
 
		G4NistManager *man;
		G4double fWorldSize, distDet, TarLength, TarWidth, dDiameterGe, dDiameterSi, zLengthGe, zLengthSi;
		G4Material *Galactic, *Ge, *BC, *Si, *Al, *B10;
		G4Element *elB10;
		G4Isotope *B10Iso;

		G4Box *solidWorld, *solidTarget;
		G4Tubs *solidGeDet, *solidSiDet, *solidTargetCyl, *solidCollimator;
		G4Sphere *solidSiSph;
		
		G4LogicalVolume *logicWorld, *logicGeDet, *fGeScoringVolume, *fSiScoringVolume, *logicTarget, 
		*logicTargetCyl, *logicSiDet, *logicCollimator, *logicSiSph;

		G4VPhysicalVolume *physWorld, *physGeDet, *physSiDet, *physSiSph, *physTarget, 
		*physTargetCyl, *physCollimator;

		G4GenericMessenger* fMessenger = nullptr;

		void DefineMaterials();
		void DefineParameters();
		void ConstructHPGeDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructAlShield(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructIsoSphere(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructShielding(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructTarget(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructSiSphDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructCollimator(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
		void ConstructTarget();
  
};

#endif
