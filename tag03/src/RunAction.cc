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
	
	auto man = G4AnalysisManager::Instance();
	man->SetDefaultFileType("root");

	man->CreateNtuple("GeScoring", "GeScoring");
	man->CreateNtupleDColumn("fEdepGe");
	man->FinishNtuple(0);

	man->CreateNtuple("SiScoring", "SiScoring");
	man->CreateNtupleDColumn("fEdepSi");
	man->FinishNtuple(1);

	man->CreateNtuple("Photons", "Photons");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->CreateNtupleDColumn("fEdep");
	man->FinishNtuple(2);

	man->CreateNtuple("Alphas", "Alphas");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(3);

	man->CreateNtuple("AlphaE", "AlphaE");
	man->CreateNtupleDColumn("fTotalE");
	man->FinishNtuple(4);

	man->CreateNtuple("Li7E", "Li7E");
	man->CreateNtupleDColumn("fTotalE");
	man->FinishNtuple(5);

	man->CreateNtuple("Si e-", "Si e-");
	man->CreateNtupleDColumn("fTotalE");
	man->FinishNtuple(6);

	man->CreateNtuple("Ge e-", "Ge e-");
	man->CreateNtupleDColumn("fTotalE");
	man->FinishNtuple(7);
}


RunAction::~RunAction() {}

G4Run* RunAction::GenerateRun() { 
  
	fRun = new Run();
	return fRun;
}

void RunAction::BeginOfRunAction(const G4Run*) { 
  	// Keep run condition
  	if (fPrimary) { 
		G4ParticleDefinition* particle = fPrimary->GetParticleGun()->GetParticleDefinition();
		G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
		fRun->SetPrimary(particle, energy);
  	}    
	
	
	// G4AnalysisManager* man = G4AnalysisManager::Instance();
	// man->SetDefaultFileType("root");
	// man->OpenFile("TestBKG.root");
	// man->OpenFile();

	G4AnalysisManager* man = G4AnalysisManager::Instance(); 
	if (uiMode) { // uiMode data capture // 
		G4cout << "===UI MODE===" << G4endl; 
		man->SetDefaultFileType("root");
		man->OpenFile("AGUI.root");
	} else { // sim.mac, genSim.mac data capture //
		G4cout << "===MACRO MODE===" << G4endl; 
		man->OpenFile();
	}
	
}

void RunAction::EndOfRunAction(const G4Run*) {
 
	if (isMaster) fRun->EndOfRun();
				
	auto man = G4AnalysisManager::Instance();
	man->Write();
	man->CloseFile();
}

