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

#include "detector.hh"

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
#include "G4PVPlacement.hh"
#include "G4PVPlacement.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
  public:
  
    DetectorConstruction();
    ~DetectorConstruction() override = default;
    
    G4VPhysicalVolume* Construct() override;              
    G4double GetWorldSize() { return fWorldSize; }; 
    G4LogicalVolume* GetSiScoringVolume() const { return fSiScoringVolume; }

  private:
  
    G4double fWorldSize, distDet, zLengthSi, dDiameterSi;
    G4NistManager *man;
    G4Material *Galactic, *Ge, *BC, *Si, *Al, *Air;
    G4Box *solidWorld;
    G4Tubs *solidSiDet;
    G4LogicalVolume *logicWorld, *logicSiDet, *fSiScoringVolume;
    G4PVPlacement *physWorld, *physSiDet;

    void DefineMaterials();
    void ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
    void ConstructSDandField();

};

