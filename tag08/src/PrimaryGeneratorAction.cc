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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4IonTable.hh"
#include "G4Geantino.hh"
#include "G4ParticleDefinition.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include <G4Transform3D.hh>
#include <G4RotationMatrix.hh>


PrimaryGeneratorAction::PrimaryGeneratorAction() : 
G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
  
	generateBackground = false;
	generateCosmicRay = false;
	generateNBSR = true;

	particleTable = G4ParticleTable::GetParticleTable();

	fParticleGun = new G4ParticleGun(1);
	fParticleGun->SetParticleDefinition(particleTable->FindParticle("neutron"));
	fParticleGun->SetParticleEnergy(0.0001*eV);

	fParticleGun->SetParticleEnergy(1*MeV);
	fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));

	fMessengerGun = new G4GenericMessenger(this, "/gun/", "Gun control");
	fMessengerGun->DeclareMethod("setMode", &PrimaryGeneratorAction::SwitchGun, "Set gun mode");
	
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
	delete fParticleGun;
	delete fMessengerGun;
}

void PrimaryGeneratorAction::generateBeamlineHit(G4Event* anEvent) {
	
	double randAngle = G4UniformRand() * 2 * M_PI;
	double randRadius = G4UniformRand() * 4.8 * cm;

	fParticleGun->SetParticlePosition(
		G4ThreeVector(randRadius*std::cos(randAngle), randRadius*std::sin(randAngle), 40*cm));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1));
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGeneratorAction::generateBackgroundHit(G4Event* anEvent, G4int shellHits) {
	// Generate gammas in a spherical shell as a background source. 
	double shellRadius = 49.3*cm;
	for (int i = 0; i < shellHits; i++) {
		double randTheta = G4UniformRand() * (M_PI - 7.5*deg);
		double randPhi = G4UniformRand() * 2 * M_PI;
		double randX = std::cos(randPhi)*std::sin(randTheta);
		double randY = std::sin(randPhi)*std::sin(randTheta);
		double randZ = std::cos(randTheta);
		G4ThreeVector randAxis = G4ThreeVector(randX, randY, randZ);

		fParticleGun->SetParticlePosition(shellRadius*randAxis);
		fParticleGun->SetParticleMomentumDirection(-randAxis);
		fParticleGun->GeneratePrimaryVertex(anEvent);
	}
}

void PrimaryGeneratorAction::generateCosmicRayHit(G4Event* anEvent) {

	fParticleGun->SetParticlePosition(G4ThreeVector(0, 1*km / 2, 0));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, -1, 0));
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGeneratorAction::generateNBSRHit(G4Event* anEvent) {

	double randAngle = G4UniformRand() * 2 * M_PI;
	double randRadius = G4UniformRand() * 4.8 * cm;

	fParticleGun->SetParticlePosition(
		G4ThreeVector(randRadius*std::cos(randAngle), randRadius*std::sin(randAngle), .8*m));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGeneratorAction::generateAlphaSource() {

	// OG method
	// G4double cosTheta = G4UniformRand();
    // G4double sinTheta = std::sqrt(1.0 - cosTheta*cosTheta);
    // G4double phi = 2 * M_PI * G4UniformRand();

    // G4double ux = sinTheta * std::cos(phi);
    // G4double uy = sinTheta * std::sin(phi);
    // G4double uz = cosTheta;

    // G4ThreeVector direction(ux, uy, uz);

	// Generate cone around aperture method
	G4double cosTheta = 0.015*G4UniformRand() + 0.985;
    G4double sinTheta = std::sqrt(1.0 - cosTheta*cosTheta);
    G4double phi = 2 * M_PI * G4UniformRand();

    G4double ux = sinTheta * std::cos(phi);
    G4double uy = sinTheta * std::sin(phi);
    G4double uz = cosTheta;

	G4ThreeVector direction(ux, uy, uz);

	std::vector<G4ThreeVector> detDirs = {
		G4ThreeVector(0, 0, 1).rotateY(50*deg).rotateZ(0*deg),
		G4ThreeVector(0, 0, 1).rotateY(50*deg).rotateZ(120*deg),
		G4ThreeVector(0, 0, 1).rotateY(50*deg).rotateZ(-120*deg),
		G4ThreeVector(0, 0, 1).rotateY(-67.24*deg).rotateZ(0*deg),
		G4ThreeVector(0, 0, 1).rotateY(-67.24*deg).rotateZ(120*deg),
		G4ThreeVector(0, 0, 1).rotateY(-67.24*deg).rotateZ(-120*deg)
	};

	G4int detectorSelector = (G4int)(6.0 * G4UniformRand());
	G4ThreeVector detPos = 21*cm * detDirs[detectorSelector];

	// Compute new axis from shifted source
	G4ThreeVector newAxis = (detPos - fParticleGun->GetParticlePosition()).unit();

	// Rotate your cone to that axis
	direction.rotateUz(newAxis);

	fParticleGun->SetParticleMomentumDirection(direction);
}

void PrimaryGeneratorAction::generateDefaultSource() {
	
	fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));
}

void PrimaryGeneratorAction::generateAlphaInUniformDisk(G4double rad) {

	G4double r = 5*mm * std::sqrt(G4UniformRand());
    G4double phi2 = 2 * M_PI * G4UniformRand();
	fParticleGun->SetParticlePosition(r*G4ThreeVector(std::cos(phi2), std::sin(phi2), 0));
}

void PrimaryGeneratorAction::generateAlphaConstShift(G4ThreeVector shift) {
	
	fParticleGun->SetParticlePosition(shift);
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

	generateBeamlineHit(anEvent);
	fParticleGun->GeneratePrimaryVertex(anEvent);


	// if (generateBackground) {
	// 	if (anEvent->GetEventID() % 2 == 0) {
	// 		generateBackgroundHit(anEvent);
	// 	} else {
	// 		generateBeamlineHit(anEvent);
	// 	}
	// } else if (generateCosmicRay) {
	// 	if (anEvent->GetEventID() % 2 == 0) {
	// 		fParticleGun->SetParticleDefinition(particleTable->FindParticle("proton"));
	// 		fParticleGun->SetParticleEnergy(100*GeV);
	// 		generateCosmicRayHit(anEvent);
	// 	} else {
	// 		fParticleGun->SetParticleDefinition(particleTable->FindParticle("neutron"));
	// 		fParticleGun->SetParticleEnergy(0.0001*eV);
	// 		generateBeamlineHit(anEvent);
	// 	}
	// } else if (generateNBSR) {
	// 	if (anEvent->GetEventID() % 2 == 0) {
	// 		generateNBSRHit(anEvent);
	// 	} else {
	// 		generateBeamlineHit(anEvent);
	// 	}
	// } else {
	// 	generateBeamlineHit(anEvent);
	// }
}
