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

	man->CreateNtuple("AlphaDet1", "AlphaDet1");
	man->CreateNtupleDColumn("fEdepAlpha");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(9);

	man->CreateNtuple("AlphaDet2", "AlphaDet2");
	man->CreateNtupleDColumn("fEdepAlpha");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(10);

	man->CreateNtuple("AlphaDet3", "AlphaDet3");
	man->CreateNtupleDColumn("fEdepAlpha");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(11);

	man->CreateNtuple("AlphaDet4", "AlphaDet4");
	man->CreateNtupleDColumn("fEdepAlpha");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(12);

	man->CreateNtuple("AlphaDet5", "AlphaDet5");
	man->CreateNtupleDColumn("fEdepAlpha");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(13);

	man->CreateNtuple("AlphaDet6", "AlphaDet6");
	man->CreateNtupleDColumn("fEdepAlpha");
	man->CreateNtupleIColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(14);

}

RunAction::~RunAction() {}

G4Run* RunAction::GenerateRun() { 
	fRun = new Run();
	return fRun;
}

void RunAction::BeginOfRunAction(const G4Run*) { 
  	
  	if (fPrimary) { 
		G4ParticleGun* gun = fPrimary->GetParticleGun();
		G4String mode = fPrimary->GetGunMode();
		if (mode == "default" || mode == "Default") {
			G4cout << "Default gun mode: alpha source at center" << G4endl;
		} else if (mode == "uniformArea") {
			G4cout << "uniformArea gun mode: alpha source in uniform disk" << G4endl;
		} else if (mode == "left") {
			G4cout << "left gun mode: alpha source on left side" << G4endl;
		} else if (mode == "right") {
			G4cout << "right gun mode: alpha source on right side" << G4endl;
		} else {
			G4cout << "ts jawn NOT a gun mode" << G4endl;
		}
		G4ParticleDefinition* particle = gun->GetParticleDefinition();
		G4double energy = gun->GetParticleEnergy();
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

