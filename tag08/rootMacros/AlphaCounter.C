#include <string> 
#include <iostream>
#include <vector>
#include <TCanvas.h>
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include "TROOT.h"
#include "TStyle.h"

void AlphaCounter(std::string file) {

    char *cfile = new char[file.length() + 1];
    std::strcpy(cfile, file.c_str());
    std::cout << "Opening File: " << file << std::endl;
    TFile* input = new TFile(cfile, "read");

    char const* treeName = "AlphaDet";
    // TLeaf* l = (TLeaf*)tree->GetListOfLeaves()->At(0);

    int count = 0;
    std::cout << "[";
    for (int i = 0; i < input->GetListOfKeys()->GetEntries(); ++i) {

        TTree* tree = (TTree*)input->Get(input->GetListOfKeys()->At(i)->GetName());
        // std::cout << input->GetListOfKeys()->At(i)->GetName() << "--------------" << '\n'; 
        // std::cout << '\t' << tree->GetListOfBranches()->At(0)->GetName() << std::endl;
        int entries = tree->GetEntries();
        std::string ehlp = tree->GetName();
        if (ehlp.rfind(treeName, 0) == 0) {
            count++;
            if (count == 6) std::cout << entries << "]";
            else std::cout << entries << ", ";
            // std::cout << "Number of entries in " << tree->GetName() << ": " << entries << std::endl;
        }        
    }
    std::cout << std::endl;

    

}