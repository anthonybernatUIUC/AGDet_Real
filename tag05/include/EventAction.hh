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
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#pragma once

#include "G4UserEventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "globals.hh"
#include "Run.hh"
#include "RunAction.hh"


class EventAction : public G4UserEventAction {
  
	public:
		EventAction();
		~EventAction();
		void AddDecayChain(G4String val) { fDecayChain += val; };
		void AddEvisible(G4double val) { fEvisTot += val; };

		void AddEdep(G4double edep, int catIdx) { 
			if (map.find(catIdx) == map.end()) {
				map[catIdx] = edep;
			} else {
				map[catIdx] += edep;
			}
		};
		
		virtual void BeginOfEventAction(const G4Event*);
      	virtual void EndOfEventAction(const G4Event*);
    
  	private:
		G4String fDecayChain;                   
		G4double fEvisTot;
		std::map<G4int, G4double> map;
		Run* run;
		const G4UserRunAction* runAction;
		G4AnalysisManager* man;
};
