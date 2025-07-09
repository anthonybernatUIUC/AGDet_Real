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
#include <TLine.h>


// void GeHist(std::string file) {

//     int darkGray = TColor::GetColor(23, 23, 28);
//     gStyle->SetCanvasColor(darkGray);
//     // gStyle->SetPadColor(darkGray);
//     gStyle->SetStatColor(darkGray);
//     gStyle->SetTitleColor(1);
//     gStyle->SetTitleTextColor(kWhite);
//     gStyle->SetStatTextColor(kWhite);

//     char *cfile = new char[file.length() + 1];
//     std::strcpy(cfile, file.c_str());
//     TString title = "Simulated Energy Spectrum of ";
//     std::cout << "Opening File: " << file << std::endl;
//     TFile* input = new TFile(cfile, "read");

//     int binNo = 6000;
//     double xMin  = 0.0;
//     double xMax  = 3.0;

//     TTree* tree = (TTree*)input->Get("Ge e-");
//     TCanvas* canvas = new TCanvas();
//     canvas->SetLogy(1);
//     canvas->SetTicky();
//     canvas->SetTickx();

//     TString branchName = tree->GetListOfBranches()->At(0)->GetName();
//     double fEdep;
//     tree->SetBranchAddress(branchName, &fEdep);
//     int entries = tree->GetEntries();

//     double fEdepMax = INT_MIN;
//     double fEdepMin = INT_MAX;
//     TH1F* hist = new TH1F("GeHist", title + branchName, binNo, xMin, xMax);
//     for (int j = 0; j < entries; j++) {
//         tree->GetEntry(j);
//         hist->Fill(fEdep);
//         if (fEdep > fEdepMax) fEdepMax = fEdep;
//         if (fEdep < fEdepMin) fEdepMin = fEdep;
//     }
    
//     for (int i = 0; i < binNo; i++) {
//         int count = hist->GetBinContent(i);
//         if (count == 0) continue; // Skip bins with zero counts
//         std::cout << " | " << std::hex << i << ":" << std::dec << count;
//     }
//     std::cout << " |" << std::endl;

//     int totalCounts = hist->Integral(hist->FindBin(xMin), hist->FindBin(xMax));
//     int photoPeakCounts = hist->Integral(hist->FindBin(0.47), hist->FindBin(.49));
//     std::cout << "Total Counts: " << totalCounts << std::endl;
//     std::cout << "Photopeak Counts: " << photoPeakCounts << std::endl;
//     std::cout << "Photopeak Efficiency: " << (double)photoPeakCounts / totalCounts << std::endl;
//     std::cout << '\t' << "Max: " << fEdepMax << std::endl;
//     std::cout << '\t' << "Min: " << fEdepMin << std::endl;

//     hist->SetLineColorAlpha(kRed, 0.35);
//     hist->GetXaxis()->SetLabelColor(kWhite);
//     hist->GetYaxis()->SetLabelColor(kWhite);
//     hist->GetXaxis()->SetAxisColor(kWhite, 1);
//     hist->GetYaxis()->SetAxisColor(kWhite, 1);
//     hist->GetXaxis()->SetTitleColor(kWhite);
//     hist->GetYaxis()->SetTitleColor(kWhite);
//     hist->GetXaxis()->SetTitle("Energy / MeV");
//     hist->GetYaxis()->SetTitle("Counts");
//     hist->SetStats(0);	
//     hist->SetFillStyle(3002);
//     hist->SetFillColor(kRed);
//     hist->Draw();

//     // TLine* Cr53 = new TLine(2.239, 10, 2.239, hist->GetMaximum());
//     // Cr53->SetLineColor(kWhite);
//     // Cr53->SetLineStyle(4);
//     // Cr53->Draw("same");
//     // TLine* Mn56 = new TLine(1.81, 10, 1.81, hist->GetMaximum());
//     // Mn56->SetLineColor(kWhite);
//     // Mn56->SetLineStyle(4);
//     // Mn56->Draw();
//     // TLine* Cr50 = new TLine(1.899, 10, 1.899, hist->GetMaximum());
//     // Cr50->SetLineColor(kWhite);
//     // Cr50->SetLineStyle(4);
//     // Cr50->Draw();
//     // TLine* Na24 = new TLine(2.754, 10, 2.754, hist->GetMaximum());
//     // Na24->SetLineColor(kWhite);
//     // Na24->SetLineStyle(4);
//     // Na24->Draw();
//     // TLine* Fe56 = new TLine(.845, 10, .845, hist->GetMaximum()); // maybe?
//     // Fe56->SetLineColor(kWhite);
//     // Fe56->SetLineStyle(4);
//     // Fe56->Draw("same");
    
//     delete[] cfile;

// }

// Does the same as above but doesn't need to initialize its own histogram
void GeHist(std::string file) { 

    int darkGray = TColor::GetColor(23, 23, 28);
    gStyle->SetCanvasColor(darkGray);
    gStyle->SetStatColor(darkGray);
    gStyle->SetTitleColor(1);
    gStyle->SetTitleTextColor(kWhite);
    gStyle->SetStatTextColor(kWhite);

    char *cfile = new char[file.length() + 1];
    std::strcpy(cfile, file.c_str());
    std::cout << "Opening File: " << file << std::endl;
    TFile* tfile = new TFile(cfile, "read");

    TCanvas* canvas = new TCanvas();
    canvas->SetLogy(1);
    canvas->SetTicky();
    canvas->SetTickx();

    char const* treeName = "Ge e-";
    char branchName[] = "fEdepGeElec";
    char yo[] = ">>htemp(6000,0,3.0)";

    TTree* tree = (TTree*)tfile->Get(treeName);
    tree->Draw(strcat(branchName, yo));

    TH1 *hist = (TH1*)gPad->GetPrimitive("htemp");
    char title[] = "Simulated Gamma Energy Spectrum - ";
    hist->SetTitle(strcat(title, cfile));
    hist->GetXaxis()->SetLabelColor(kWhite);
    hist->GetYaxis()->SetLabelColor(kWhite);
    hist->GetXaxis()->SetAxisColor(kWhite, 1);
    hist->GetYaxis()->SetAxisColor(kWhite, 1);
    hist->GetXaxis()->SetTitleColor(kWhite);
    hist->GetYaxis()->SetTitleColor(kWhite);
    hist->GetXaxis()->SetTitle("Energy / MeV");
    hist->GetYaxis()->SetTitle("Counts");
    hist->SetLineColorAlpha(kRed, 0.35);
    hist->SetFillColor(kRed);
    hist->SetFillStyle(3002);
    hist->SetStats(0);

    int totalCounts = hist->Integral(hist->FindBin(0), hist->FindBin(10));
    int photoPeakCounts = hist->Integral(hist->FindBin(0.47), hist->FindBin(0.49));
    std::cout << "Total Counts: " << totalCounts << std::endl;
    std::cout << "Photopeak Counts: " << photoPeakCounts << std::endl;
    std::cout << "Photopeak Efficiency: " << (double)photoPeakCounts / totalCounts << std::endl;
}

