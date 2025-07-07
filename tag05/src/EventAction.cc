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

EventAction::EventAction() : G4UserEventAction(), fEvisTot(0.) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
 
	fDecayChain = G4String(" ");
	fEvisTot = 0., fEdepGe = 0., fEdepSi = 0., fEdepGamma = 0., 
	fEdepAlpha = 0., fEdepLi7 = 0., fEdepSiElec = 0., fEdepGeElec = 0., fEdepSiBackground = 0.;
}

void EventAction::EndOfEventAction(const G4Event* evt) {
  
	G4int evtNb = evt->GetEventID(); 
	G4int printProgress = G4RunManager::GetRunManager()->GetPrintProgress();
	if (evtNb % printProgress == 0) {
		G4cout << "    End of event. Decay chain:" << fDecayChain 
		<< G4endl << G4endl;
	}

	Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
	run->EvisEvent(fEvisTot);

	G4double tol = 0.0001;
	G4double cats[8] = { fEdepGe, fEdepSi, fEdepGamma, fEdepAlpha, 
		fEdepLi7, fEdepSiElec, fEdepGeElec, fEdepSiBackground };
	auto man = G4AnalysisManager::Instance();

	for (size_t i = 0; i < 8; ++i) {
		if (cats[i] > tol) {
			man->FillNtupleDColumn(i, 0, cats[i]);
			man->AddNtupleRow(i);
			// if (i == 6) {
			// 	G4cout << "------Edep Ge Elec: " << fEdepGeElec << G4endl;
			// }
			// if (i == 4) {
			// 	G4cout << "------Edep Li7: " << fEdepLi7 << G4endl;
			// }
		}
		
	}
}




