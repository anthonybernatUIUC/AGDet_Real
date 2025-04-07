#include <string> 
#include <iostream>
#include <vector>
#include <TCanvas.h>
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include "TROOT.h"


void AllHist(std::string file) {

    char *cfile = new char[file.length()+1];
    std::strcpy(cfile, file.c_str());
    TString title = "Simulated Energy Spectrum of ";
    std::cout << "Opening File: " << file << std::endl;
    TFile* input = new TFile(cfile, "read");

    int yHeight = 500;
    int binNo = 500;
    double xMin  = 0;
    double xMax  = 2.5;

    for (int i = 0; i < input->GetListOfKeys()->GetEntries() - 2; ++i) { 
        
        TTree* tree = (TTree*)input->Get(input->GetListOfKeys()->At(i)->GetName());
        TCanvas* canvas = new TCanvas();
        canvas->SetLogy(1);
        canvas->SetTicky();
        canvas->SetTickx();
        std::cout << input->GetListOfKeys()->At(i)->GetName() << "--------------" << '\n'; 
        // std::cout << '\t' << tree->GetListOfBranches()->At(0)->GetName() << std::endl;

        TLeaf* l = (TLeaf*)tree->GetListOfLeaves()->At(0);
        std::string type = l->GetTypeName();
        // std::cout << l->GetTypeName() << std::endl;    
        TString branchName = tree->GetListOfBranches()->At(0)->GetName();
        double fEdep;
        if (type != "Double_t") {
            delete canvas;
            continue;
        }
        tree->SetBranchAddress(branchName, &fEdep);
        int entries = tree->GetEntries();
        TString str = std::to_string(i);

        double fEdepMax = INT_MIN;
        double fEdepMin = INT_MAX;
        TH1F* hist = new TH1F("hist" + str, title + branchName, binNo, xMin, xMax);
        
        for (int j = 0; j < entries; j++) {
            tree->GetEntry(j);
            hist->Fill(fEdep);
            if (fEdep > fEdepMax) fEdepMax = fEdep;
            if (fEdep < fEdepMin) fEdepMin = fEdep;
        }
        std::cout << '\t' << "Max: " << fEdepMax << std::endl;
        std::cout << '\t' << "Min: " << fEdepMin << std::endl;
        int prevCount = 0;
        std::vector<int> bro;
        for (int i = 0; i < binNo; i++) {
            int count = hist->GetBinContent(i);
            std::cout << " | " << count;
            if (prevCount > 10 && count == 0) {
                std::cout << "bro: " << i;
                bro.push_back(i);
            }
            prevCount = count;
        }
        std::cout << " |" << std::endl;

        if (!bro.empty()) {
            // bin_centerx = h1->GetXaxis()->GetBinCenter(h1->GetXaxis()->FindFixBin(somex)); 
            double bin_centerx = hist->GetXaxis()->GetBinCenter(bro.at(0)); 
            std::cout << "Bro Bin Center: " << bin_centerx << std::endl;
        }

        hist->SetLineColorAlpha(kRed, 0.35);
        hist->GetXaxis()->SetTitle("Energy / MeV");
        hist->GetYaxis()->SetTitle("Counts");	
        hist->SetStats(0);	
        hist->SetFillStyle(3002);
        hist->SetFillColor(kRed);
        hist->Draw();
        
    }
    delete[] cfile;
}