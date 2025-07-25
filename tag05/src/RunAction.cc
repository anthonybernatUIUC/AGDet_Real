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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class
//

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "Run.hh"

RunAction::RunAction(PrimaryGeneratorAction* kin, bool uiMode) : 
G4UserRunAction(), fPrimary(kin), fRun(0), uiMode(uiMode) {
	
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->SetDefaultFileType("root");

	man->CreateNtuple("GeScoring", "GeScoring");
	man->CreateNtupleDColumn("fEdepGe");
	man->FinishNtuple(0);
	
	man->CreateNtuple("SiScoring", "SiScoring");
	man->CreateNtupleDColumn("fEdepSi");
	man->FinishNtuple(1);
	
	man->CreateNtuple("Gammas", "Gammas");
	man->CreateNtupleDColumn("fEdepGammaOnly");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(2);
	
	man->CreateNtuple("Alphas", "Alphas");
	man->CreateNtupleDColumn("fEdepAlphaOnly");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(3);
	
	man->CreateNtuple("Li7E", "Li7E");
	man->CreateNtupleDColumn("fEdepLi7");
	man->FinishNtuple(4);
	
	man->CreateNtuple("Si e-", "Si e-");
	man->CreateNtupleDColumn("fEdepSiElec");
	man->FinishNtuple(5);
	
	man->CreateNtuple("Ge e-", "Ge e-");
	man->CreateNtupleDColumn("fEdepGeElec");
	man->FinishNtuple(6);
	
	man->CreateNtuple("Si Background", "Si Background");
	man->CreateNtupleDColumn("fEdepSiBackground");
	man->FinishNtuple(7);
	
	man->CreateNtuple("Ge Background", "Ge Background");
	man->CreateNtupleDColumn("fEdepGeBackground");
	man->FinishNtuple(8);
	
}


RunAction::~RunAction() {}

G4Run* RunAction::GenerateRun() { 
	fRun = new Run();
	return fRun;
}

void RunAction::BeginOfRunAction(const G4Run*) { 
  	
  	if (fPrimary) { 
		G4ParticleDefinition* particle = fPrimary->GetParticleGun()->GetParticleDefinition();
		G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
		fRun->SetPrimary(particle, energy);
  	}   

	G4AnalysisManager* man = G4AnalysisManager::Instance(); 
	if (uiMode) G4cout << "===UI MODE===" << G4endl; 
	else G4cout << "===MACRO MODE===" << G4endl; 
	
	man->SetNtupleMerging(true);
	man->OpenFile("");
	man->SetDefaultFileType("root");
}

void RunAction::EndOfRunAction(const G4Run*) {
 
	if (isMaster) fRun->EndOfRun();
				
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->Write();
	man->CloseFile();
	
}

