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
/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class
//
//

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"

#include "globals.hh"

class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  	public:
  		PrimaryGeneratorAction();    
   		~PrimaryGeneratorAction();
    	virtual void GeneratePrimaries(G4Event*);
    	G4ParticleGun* GetParticleGun() { return fParticleGun; };
		void generateBackgroundHit(G4Event* anEvent, G4int shellHits = 1);
		void generateBeamlineHit();
		void generateCosmicRayHit(G4Event* anEvent);
		void generateNBSRHit(G4Event* anEvent);
		void generateAlphaSource();
		void generateDefaultSource();
		void generateInUniformDisk(G4double rad);
		void generateConstDrift(G4ThreeVector drift);

		void SwitchGun(G4String mode) { 
			gunMode = mode; 
			if (gunMode == "default" || gunMode == "Default") {
				generateDefaultSource();
			} else if (gunMode == "uniformArea") {
				generateInUniformDisk(5*mm);
			} else if (gunMode == "cone") {
				generateBeamlineHit();
			} else if (gunMode == "reset") {
				fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 40*cm));
			} else {
				gunMode = "reset";
				fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 40*cm));
				G4cout << "ts jawn NOT a gun mode!" << G4endl;
			}
		};

		void SetDrift(G4ThreeVector drift) {
			G4cout << "Gun drift set to: " << drift*cm << G4endl;
			gunMode = "drift";
			generateConstDrift(G4ThreeVector(drift[0], drift[1], 40)*cm);
		};

		G4String GetGunMode() { return gunMode; };
    	G4int Z, A;
            
  	private:
		G4GenericMessenger* fMessengerGun;
    	G4ParticleGun* fParticleGun;
		G4ParticleTable* particleTable;
		G4String gunMode = "default";
		bool generateBackground;
		bool generateCosmicRay;
		bool generateNBSR;
};

#endif
