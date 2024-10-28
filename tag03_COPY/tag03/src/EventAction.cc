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
#include "Run.hh"
#include "G4AnalysisManager.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include <iomanip>

EventAction::EventAction() : G4UserEventAction(), fEvisTot(0.) {
  G4RunManager::GetRunManager()->SetPrintProgress(10000);
}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {
 
  fDecayChain = G4String(" ");
  fEvisTot = 0.;
  fEdepGe = 0.;
  fEdepSi = 0.;
  fIncident = false;
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

  auto man = G4AnalysisManager::Instance();
  if (fEdepGe > 0.) { 
    man->FillNtupleDColumn(0, 0, fEdepGe);
    man->AddNtupleRow(0);
  } 
  if (fEdepSi > 0.) {
    man->FillNtupleDColumn(1, 0, fEdepSi);
    man->AddNtupleRow(1);
  }
  
}




