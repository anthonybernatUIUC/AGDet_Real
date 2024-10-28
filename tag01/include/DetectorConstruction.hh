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
#include "G4PVPlacement.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

    // Scoring Volume
    G4LogicalVolume *GetGeScoringVolume() const { return fGeScoringVolume; }
    G4LogicalVolume *GetSiScoringVolume() const { return fSiScoringVolume; }

    virtual G4VPhysicalVolume* Construct();                 
    virtual G4double GetWorldSize() { return fWorldSize; } 
    
  private:
 
    G4GenericMessenger *fMessenger;
    G4double fWorldSize, rThickness, zLength, dDiameter, outerShieldDiameter, ShellSweep;
    G4bool isShielded, isHydrogen, halfShell, useTarget;

    G4NistManager *man;
    G4Material *Galactic, *Ge, *Cu, *Pb, *Al, *IsotopeShellMat, *BC, *Si;
    G4Isotope *H1, *Cl35, *Cr50, *Cr52, *Cr53, *Fe56, *Ni58, *Ni60, *Ni62, *Ni63, *Ni64, *Na24, *Mn56;
    G4Element *elH, *elCl, *elCr, *elFe, *elNi, *elNa, *elMn;

    G4double zDistanceFromOrigin, Sidel, Capl, innerShellRadius, ShellThickness, 
    Di1, Do1, zL1, DistOrigin1, Di2, Do2, zL2, DistOrigin2, Di3, Do3, zL3, DistOrigin3, 
    TarLength, TarWidth, distDet;

    G4Box *solidWorld, *solidTarget;
    G4Tubs *solidGeDet, *solidAl1, *solidAl2, *solidShield1, *solidShield2, *solidShield3, 
    *solidTargetCyl, *solidSiDet;
    G4Sphere *solidIsoSphere;
    
    G4LogicalVolume *logicWorld, *logicGeDet, *fGeScoringVolume, *fSiScoringVolume, *logicAl1, *logicAl2, 
    *logicIsoSphere, *logicShield1, *logicShield2, *logicShield3, *logicTarget, *logicTargetCyl, *logicSiDet;

    G4VPhysicalVolume *physWorld, *physGeDet, *physAl1, *physAl2_1, *physAl2_2, *physShield1, 
    *physShield2, *physShield3, *physIsoSphere, *physTarget, *physTargetCyl, *physSiDet;

    void DefineMaterials();
    void ConstructHPGeDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
    void ConstructAlShield(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
    void ConstructIsoSphere(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
    void ConstructShielding(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
    void ConstructTarget(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
    void ConstructSiDetector(G4RotateY3D rotTheta, G4RotateZ3D rotPhi);
    void ConstructTarget();
  
};

#endif

