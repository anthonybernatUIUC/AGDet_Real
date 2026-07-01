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
#include <numeric>

void PrintBranches(TFile* input) {
    for (int i = 0; i < input->GetListOfKeys()->GetEntries(); ++i) {

        TTree* tree = (TTree*)input->Get(input->GetListOfKeys()->At(i)->GetName());
        std::cout << 
            input->GetListOfKeys()->At(i)->GetName() << " =============================== " << 
            "Counts: " << tree->GetEntries() << '\n'; 

        for (auto branchIter = tree->GetListOfBranches()->begin(); 
             branchIter != tree->GetListOfBranches()->end(); ++branchIter) {

            TBranch* branch = (TBranch*)(*branchIter);
            std::cout << '\t' << "Branch: " << branch->GetName() << std::endl;
        }      
    }
}

void Coincidence(std::string file_thin, std::string file_thick, int n) {
    
    char *cfile_thin = new char[file_thin.length() + 1];
    std::strcpy(cfile_thin, file_thin.c_str());
    std::cout << "Opening File: " << file_thin << std::endl;
    TFile* input_thin = new TFile(cfile_thin, "read");

    char *cfile_thick = new char[file_thick.length() + 1];
    std::strcpy(cfile_thick, file_thick.c_str());
    std::cout << "Opening File: " << file_thick << std::endl;
    TFile* input_thick = new TFile(cfile_thick, "read");
    std::cout << std::endl;

    std::vector<int> gammaCountsThin(6, 0);
    std::vector<int> gammaCountsThick(6, 0);
    std::vector<int> gammaPhotopeakCountsThin(6, 0);
    std::vector<int> gammaPhotopeakCountsThick(6, 0);
    std::vector<int> alphaCountsThin(6, 0);
    
    for (int i = 1; i < 7; ++i) {
        TTree* treeThin = (TTree*)input_thin->Get(Form("GammaDet%d", i));
        TTree* treeThick = (TTree*)input_thick->Get(Form("GammaDet%d", i));
        TTree* treeAlphaThin = (TTree*)input_thin->Get(Form("AlphaDet%d", i));

        gammaCountsThin[i-1] = treeThin->GetEntries();
        gammaCountsThick[i-1] = treeThick->GetEntries();
        alphaCountsThin[i-1] = treeAlphaThin->GetEntries();
        gammaPhotopeakCountsThin[i-1] = treeThin->Draw("fEdepGamma", "fEdepGamma >= 0.47 && fEdepGamma <= 0.48", "goff");
        gammaPhotopeakCountsThick[i-1] = treeThick->Draw("fEdepGamma", "fEdepGamma >= 0.47 && fEdepGamma <= 0.48", "goff");
    }
    std::vector<std::string> SiDetLocations = { "MR", "TL", "BL", "ML", "TR", "BR" };
    std::cout << "Alpha Counts (Thin) ============================================ " << std::endl;
    for (int i = 0; i < 6; ++i) {
        std::cout << "\tAlpha Counts (" << SiDetLocations[i] << "): " << alphaCountsThin[i] << std::endl;
    }
    int r_alphathin = std::accumulate(alphaCountsThin.begin(), alphaCountsThin.end(), 0);
    std::cout << "Total Alpha Counts: " << r_alphathin << std::endl << std::endl;
    for (int i = 0; i < 6; ++i) {
        if (i == 0) {
            std::cout << "Front Ge Dets ============================================ " << std::endl;
        }
        if (i == 3) {
            std::cout << "Back Ge Dets  ============================================ " << std::endl;
        }
        std::cout << "\tGamma | Photopeak Counts  (Thin, " << i+1 << "): " << gammaCountsThin[i] << " | " << gammaPhotopeakCountsThin[i] << std::endl;
        std::cout << "\tGamma | Photopeak Counts (Thick, " << i+1 << "): " << gammaCountsThick[i] << " | " << gammaPhotopeakCountsThick[i] << std::endl;
    }
    int r_gammathin = std::accumulate(gammaCountsThin.begin(), gammaCountsThin.end(), 0);
    int r_gammathick = std::accumulate(gammaCountsThick.begin(), gammaCountsThick.end(), 0);
    int r_gammathin_photopeak = std::accumulate(gammaPhotopeakCountsThin.begin(), gammaPhotopeakCountsThin.end(), 0);
    int r_gammathick_photopeak = std::accumulate(gammaPhotopeakCountsThick.begin(), gammaPhotopeakCountsThick.end(), 0);
    std::cout << "Total Gamma | Photopeak Counts   (Thin): " << r_gammathin << " | " << r_gammathin_photopeak << std::endl;
    std::cout << "Total Gamma | Photopeak  Counts (Thick): " << r_gammathick << " | " << r_gammathick_photopeak << std::endl << std::endl;

    int AG_SolidAngle = 1;

    // std::cout << "Absolute Neutron Flux: " << r_alphathin * r_gammathick / (r_gammathin * AG_SolidAngle) 
    //     << "*(the time r_gammathick needs to integrate to " << r_gammathick << " counts)" << std::endl;
    std::cout << "Absolute Neutron Flux R_n = " << n << "/t, \n\twhere t is time r_gammathick needs to integrate " 
    << r_gammathick << " counts with 6 detectors" << std::endl << std::endl;    

    delete[] cfile_thin;
    delete[] cfile_thick;
}
