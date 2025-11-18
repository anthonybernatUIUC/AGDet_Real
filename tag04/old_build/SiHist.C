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

void SiHist(std::string file) {

    int darkGray = TColor::GetColor(23, 23, 28);
    gStyle->SetCanvasColor(darkGray);
    // gStyle->SetPadColor(darkGray);
    gStyle->SetStatColor(darkGray);
    gStyle->SetTitleColor(1);
    gStyle->SetTitleTextColor(kWhite);
    gStyle->SetStatTextColor(kWhite);

    char *cfile = new char[file.length() + 1];
    std::strcpy(cfile, file.c_str());
    TString title = "Simulated Energy Spectrum of ";
    std::cout << "Opening File: " << file << std::endl;
    TFile* input = new TFile(cfile, "read");

    int binNo = 1500;
    double xMin  = 0.0;
    double xMax  = 2.0;

    TTree* tree = (TTree*)input->Get("SiScoring");
    TCanvas* canvas = new TCanvas();
    canvas->SetLogy(1);
    canvas->SetTicky();
    canvas->SetTickx();

    TString branchName = tree->GetListOfBranches()->At(0)->GetName();
    double fEdep;
    tree->SetBranchAddress(branchName, &fEdep);
    int entries = tree->GetEntries();

    TH1F* hist = new TH1F("SiHist", title + branchName, binNo, xMin, xMax);
    for (int j = 0; j < entries; j++) {
        tree->GetEntry(j);
        hist->Fill(fEdep);
    }

    for (int i = 0; i < binNo; i++) {
        int count = hist->GetBinContent(i);
        std::cout << " | " << count;
    }
    std::cout << " |" << std::endl;

    hist->SetLineColorAlpha(kRed, 0.35);
    hist->GetXaxis()->SetLabelColor(kWhite);
    hist->GetYaxis()->SetLabelColor(kWhite);
    hist->GetXaxis()->SetAxisColor(kWhite, 1);
    hist->GetYaxis()->SetAxisColor(kWhite, 1);
    hist->GetXaxis()->SetTitleColor(kWhite);
    hist->GetYaxis()->SetTitleColor(kWhite);
    hist->GetXaxis()->SetTitle("Energy / MeV");
    hist->GetYaxis()->SetTitle("Counts");
    hist->SetStats(0);	
    hist->SetFillStyle(3002);
    hist->SetFillColor(kRed);
    hist->Draw();

    delete[] cfile;

}