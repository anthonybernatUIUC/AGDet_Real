#pragma once

#include "G4Step.hh"
#include "G4UserSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Gamma.hh"
#include "G4ParticleTypes.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"


class MySteppingAction : public G4UserSteppingAction {

	public:
		MySteppingAction(EventAction *eventAction);
		~MySteppingAction();

		virtual void UserSteppingAction(const G4Step*);
		void RecordDeposition(const G4Step *step);

	private:
		EventAction *fEventAction;
		G4RunManager* runManager;
		const DetectorConstruction* detectorConstruction;
		std::set<G4LogicalVolume*> fGeScoringVolumes;
		std::set<G4LogicalVolume*> fSiScoringVolumes;
};
