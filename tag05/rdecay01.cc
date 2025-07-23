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
/// \file rdecay01.cc
/// \brief Main program of the radioactivedecay/rdecay01 example

#include "G4Types.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4SteppingVerbose.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "QGSP_BIC_HP.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

int main(int argc, char** argv) {

	// detect interactive mode (if no arguments) and define UI session
	G4UIExecutive* ui = nullptr;

	bool uiMode = false;
	if (argc == 1) { 
		ui = new G4UIExecutive(argc, argv);
		uiMode = true;
	}

	G4int precision = 4;
	G4SteppingVerbose::UseBestUnit(precision);
	CLHEP::HepRandom::setTheSeed((unsigned)clock());

	// construct the run manager
	#ifdef G4MULTITHREADED
		G4MTRunManager* runManager = new G4MTRunManager;
		G4int maxCores = G4Threading::G4GetNumberOfCores();
		G4int coresToUse = maxCores;
		if (argc == 3) coresToUse = G4UIcommand::ConvertToInt(argv[2]);

		G4cout << "Using " << coresToUse << "/" << maxCores << " available cores." << G4endl;

		runManager->SetNumberOfThreads(coresToUse);
	#else
		// should be grayed out when in multithreaded mode (possibly Intellisense dependent)
		G4RunManager* runManager = new G4RunManager;
		G4cout << "Number of Cores: 1 (single-threaded mode)" << G4endl;
	#endif

	// storing this for future reference:
	// "cmake -DCMAKE_PREFIX_PATH=/home/anthony/software/geant4/geant4-v11.2.0-mt-install/bin/geant4-config .."  

	// set mandatory initialization classes
	runManager->SetUserInitialization(new DetectorConstruction());
	runManager->SetUserInitialization(new QGSP_BIC_HP(0));
	runManager->SetUserInitialization(new ActionInitialization(uiMode));
	runManager->Initialize();

	G4VisManager* visManager = nullptr;
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
  	if (ui)  {
		// interactive mode
		visManager = new G4VisExecutive("quiet");
		visManager->Initialize();
		UImanager->ApplyCommand("/control/execute vis.mac");
		UImanager->ApplyCommand("/vis/scene/add/scale 10 cm");
		UImanager->ApplyCommand("/vis/scene/add/axes 0 0 0 10 cm");
		ui->SessionStart();
		delete ui;
  	} else  {
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
  	}

  	delete visManager;
  	delete runManager;
}

