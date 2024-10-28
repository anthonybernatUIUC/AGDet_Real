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
//
// 

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

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle("alpha");
  fParticleGun->SetParticleDefinition(particle);	
  fParticleGun->SetParticleEnergy(5*MeV); 
  fParticleGun->SetParticlePosition(G4ThreeVector());
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  
  CLHEP::HepRandom::setTheSeed(time(NULL));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(GenerateRandAngle()));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

G4ThreeVector PrimaryGeneratorAction::GenerateRandAngle() {

  G4double dRadSi = (10 / 2)*mm;
  G4double distDet = 1*cm;
  G4double tantheta = dRadSi / distDet;

  G4double ubT = atan(tantheta);
  G4double lbT = -ubT;
	G4double theta = (lbT + G4UniformRand() * (ubT - lbT));

  G4double ubP = 2*CLHEP::pi;
  G4double lbP = 0;
  G4double phi = (lbP + G4UniformRand() * (ubP - lbP));
  
  return G4ThreeVector(sin(theta)*cos(phi), sin(theta)*sin(phi), 1);
}
