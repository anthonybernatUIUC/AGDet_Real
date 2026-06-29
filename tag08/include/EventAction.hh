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
#include "G4THitsMap.hh"


class EventAction : public G4UserEventAction {
  
	public:
		EventAction();
		~EventAction();
		void AddDecayChain(G4String val) { fDecayChain += val; };
		void AddEdep(G4double edep, int catIdx) { detMap[catIdx] += edep; };
		void AddPos(int catIdx, G4ThreeVector pos) {
			if (posMap[catIdx].size() == 0) {
				posMap[catIdx] = { pos };
			} else {
				posMap[catIdx].push_back(pos);
			}
		}

		void AddSiEdep(G4int catIdx, G4double edep) { fSiHitsMap.add(catIdx, edep); }
    	void AddGeEdep(G4int catIdx, G4double edep) { fGeHitsMap.add(catIdx, edep); }
		
		virtual void BeginOfEventAction(const G4Event*);
      	virtual void EndOfEventAction(const G4Event*);
    
  	private:
		G4String fDecayChain;                   
		std::map<G4int, G4double> detMap;
		std::map< G4int, std::vector<G4ThreeVector> > posMap;
		G4THitsMap<G4double> fSiHitsMap;
    	G4THitsMap<G4double> fGeHitsMap;
};
