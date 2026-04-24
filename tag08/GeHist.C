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
#include "TRandom.h"

void PlotAllGeDets(TFile* tfile, char* cfile) {

    for (int i = 1; i <= 6; ++i) {

        const char* treeName = Form("GammaDet%d", i);
        TTree* tree = (TTree*)tfile->Get(treeName);

        TString uniqueName = Form("Canvas_Det%d_Session%d", i, (int)gRandom->Integer(1000));
        TCanvas* canvas = new TCanvas(uniqueName, treeName, 800, 600);
        canvas->SetLogy(1);
        canvas->SetTicky();
        canvas->SetTickx();

        TString histName = Form("h_det%d", i); 
                                                //(bins, xMin, xMax)
        TString drawCommand = Form("fEdepGamma>>%s(1500,0,1.5)", histName.Data());
        tree->Draw(drawCommand);

        TH1 *hist = (TH1*)gPad->GetPrimitive(histName);
        hist->SetDirectory(0);
        TString titleStr = Form("Simulated Gamma Energy Spectrum - Det %d - %s", i, cfile);
        hist->SetTitle(titleStr.Data());
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
        
        canvas->Update();
    }
}

void GeHist(std::string file) { 

    char *cfile = new char[file.length() + 1];
    std::strcpy(cfile, file.c_str());
    std::cout << "Opening File: " << file << std::endl;
    TFile* tfile = new TFile(cfile, "read");

    int darkGray = TColor::GetColor(23, 23, 28);
    gStyle->SetCanvasColor(darkGray);
    gStyle->SetStatColor(darkGray);
    gStyle->SetTitleColor(1);
    gStyle->SetTitleTextColor(kWhite);
    gStyle->SetStatTextColor(kWhite);

    TCanvas* canvas = new TCanvas();
    canvas->SetLogy(1);
    canvas->SetTicky();
    canvas->SetTickx();

    // char const* treeName = "Ge e-";
    // char branchName[] = "fEdepGeElec";
    // char const* treeName = "Ge Background";
    // char branchName[] = "fEdepGeBackground";
    char const* treeName = "GeScoring";
    char branchName[] = "fEdepGe";
    char yo[] = ">>htemp(1500,0,3.0)"; // (bins, xMin, xMax)

    TTree* tree = (TTree*)tfile->Get(treeName);
    tree->Draw(strcat(branchName, yo));

    // Combined hist
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
    int photoPeakCounts = hist->Integral(hist->FindBin(0.47), hist->FindBin(0.48));
    std::cout << "Photopeak Counts: " << photoPeakCounts << std::endl;
    std::cout << "Total Counts:     " << totalCounts << std::endl;
    std::cout << "Photopeak Efficiency: " << (double)photoPeakCounts / totalCounts << std::endl;

    // TLine* Ge75 = new TLine(.1395, 10, .1395, hist->GetMaximum());
    // Ge75->SetLineColor(kWhite);
    // Ge75->SetLineStyle(4);
    // Ge75->Draw("same");

    // TLine* Hcapture = new TLine(2.223, 10, 2.223, hist->GetMaximum());
    // Hcapture->SetLineColor(kWhite);
    // Hcapture->SetLineStyle(4);
    // Hcapture->Draw("same");

    PlotAllGeDets(tfile, cfile);

    delete[] cfile;
}


