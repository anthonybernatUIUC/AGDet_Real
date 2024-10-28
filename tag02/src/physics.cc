#include "physics.hh"

MyPL::MyPL() {

    G4int verb = 0;
    // RegisterPhysics(new G4EmStandardPhysics_option4(verb));
    RegisterPhysics(new G4RadioactiveDecayPhysics(verb));
    // RegisterPhysics(new G4HadronElasticPhysicsXS(verb));  
    // RegisterPhysics(new G4HadronInelasticQBBC(verb + 2));
    // RegisterPhysics(new G4IonElasticPhysics(verb));
    // RegisterPhysics(new G4IonPhysicsXS(verb));
    // RegisterPhysics(new G4EmLivermorePhysics(verb));
    RegisterPhysics(new G4EmPenelopePhysics(verb));
    
    RegisterPhysics(new G4DecayPhysics(verb));
    // RegisterPhysics(new G4EmStandardPhysics(verb));

    // RegisterPhysics( new NeutronHPphysics("neutronHP"));  
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(4*eV, 1*GeV);
}   

MyPL::~MyPL() {}
