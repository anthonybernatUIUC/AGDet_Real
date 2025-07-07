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
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4Geantino.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() : 
G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
  
	G4int n_particle = 1;
	fParticleGun = new G4ParticleGun(n_particle);
	fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("neutron"));
	fParticleGun->SetParticleEnergy(0.0001*eV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
	delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
	
	double randAngle = G4UniformRand() * 2 * M_PI;
	double randRadius = G4UniformRand() * 4.8 * cm;
	fParticleGun->SetParticlePosition(G4ThreeVector(
		randRadius*std::cos(randAngle), randRadius*std::sin(randAngle), -25*cm));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
	fParticleGun->GeneratePrimaryVertex(anEvent);

	// Generate gammas in a spherical shell as a background source
	bool generateBackground = true;
	if (generateBackground) {
		int shellHits = 1;
		double shellRadius = 49.1*cm;
		for (int i = 0; i < shellHits; i++) {
			double randTheta = G4UniformRand() * (M_PI - 7.5*deg) + 7.5*deg;
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
}
