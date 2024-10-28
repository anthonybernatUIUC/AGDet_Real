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
#include "G4UnionSolid.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4GenericMessenger.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "detector.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
  
  public:
  
    DetectorConstruction();
    ~DetectorConstruction();

    G4LogicalVolume* GetGeScoringVolume() const { return fGeScoringVolume; }
    G4LogicalVolume* GetSiScoringVolume() const { return fSiScoringVolume; }

    virtual G4VPhysicalVolume* Construct();                 
    virtual G4double GetWorldSize() { return fWorldSize; } 
    virtual void ConstructSDandField();
    
  private:
 
    G4double fWorldSize, zLengthGe, zLengthSi, dDiameterGe, dDiameterSi, distDet;

    G4Material *Galactic, *Ge, *Si, *air, *Al, *Au, *mylar, *B, *BC;

    G4Box *solidWorld;
    G4Tubs *solidGeDet, *solidSiDet;
    
    G4LogicalVolume *logicWorld, *logicGeDet, *logicSiDet, *fGeScoringVolume, *fSiScoringVolume;

    G4VPhysicalVolume *physWorld, *physGeDet, *physSiDet;

    void DefineMaterials();
    void ConstructHPGeDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
    void ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
    void ConstructPIPSDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
};

#endif

