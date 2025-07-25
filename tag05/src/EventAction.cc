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
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"


EventAction::EventAction() : G4UserEventAction(), fEvisTot(0.) {
	man = G4AnalysisManager::Instance();
}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
 
	fDecayChain = G4String(" ");

	runAction = static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction());
	run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

}

void EventAction::EndOfEventAction(const G4Event* evt) {
  
	G4int evtNb = evt->GetEventID(); 
	G4int printProgress = G4RunManager::GetRunManager()->GetPrintProgress();
	if (evtNb % printProgress == 0) {
		G4cout << "    End of event. Decay chain:" << fDecayChain 
		<< G4endl << G4endl;
	}

	run->EvisEvent(fEvisTot);

	G4double tol = 0.01;
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	for (size_t i = 0; i < man->GetNofNtuples(); ++i) {
		if (map[i] > tol) {
			man->FillNtupleDColumn(i, 0, map[i]);
			man->AddNtupleRow(i);
		}
	}
	map.clear();

}




