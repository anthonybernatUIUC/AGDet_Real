#include <string> 
#include <iostream>
#include <vector>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include "TROOT.h"
#include "TStyle.h"
#include "TRandom.h"
#include "TGraph2D.h"
#include "TVector3.h"
#include "TColor.h"
#include "TArc.h"
#include "AlphaCounter.C"


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

void DrawThatThang(TTree* tree, TVector3 center, TVector3 normal, int n, const char* detOrientation) {
   
    normal = normal.Unit();

    // build orthonormal axes in plane
    TVector3 zAxis = normal;
    TVector3 tmp(0,0,1);
    if (fabs(zAxis.Dot(tmp)) > 0.99) tmp = TVector3(0,1,0);
    TVector3 xAxis = tmp.Cross(zAxis).Unit();
    TVector3 yAxis = zAxis.Cross(xAxis).Unit();

    int nBins = 250;
    double range = 34.9/2;

    char title[100];
    strcpy(title, "Det ");
    strcat(title, detOrientation);
    TH2F *h2 = new TH2F(
        detOrientation, title, 
        nBins, -range, range, nBins, -range, range);
    h2->SetStats(0);

    TString expr = Form(
                "(%f*(fX-%f) + %f*(fY-%f) + %f*(fZ-%f)) : "
                "(%f*(fX-%f) + %f*(fY-%f) + %f*(fZ-%f)) >> %s",
                xAxis.X(), center.X(), xAxis.Y(), center.Y(), xAxis.Z(), center.Z(),
                yAxis.X(), center.X(), yAxis.Y(), center.Y(), yAxis.Z(), center.Z(), detOrientation);

    TCanvas *c = new TCanvas(tree->GetName(), "Heatmap", 800, 800);
    
    h2->Draw();
    tree->Draw(expr, "", "COLZ");

    double radius = 12.70;
    TArc *circle = new TArc(0, 0, radius);
    circle->SetLineColor(kRed);
    circle->SetLineWidth(2);
    circle->SetFillStyle(0);
    circle->Draw("same");
}

void DetHitsPos(std::string file) {

    char *cfile = new char[file.length() + 1];
    std::strcpy(cfile, file.c_str());
    std::cout << "Opening File: " << file << std::endl;
    TFile* input = new TFile(cfile, "read");
    // PrintBranches(input);
    gStyle->SetPalette(kViridis);

    AlphaCounter(file);
    for (int i = 0; i < input->GetListOfKeys()->GetEntries(); ++i) {
        
        std::string name = input->GetListOfKeys()->At(i)->GetName();
        TTree* tree = (TTree*)input->Get(input->GetListOfKeys()->At(i)->GetName());
        int n = tree->GetEntries();
        if (name == "AlphaDet1") {
            DrawThatThang(tree, 210*TVector3(0.766, 0, 0.6428), TVector3(0.766, 0, 0.6428), n, "MR");
        }
        if (name == "AlphaDet2") {
            DrawThatThang(tree, 210*TVector3(-0.383, 0.6634, 0.6428), TVector3(-0.383, 0.6634, 0.6428), n, "TL");
        }
        if (name == "AlphaDet3") {
            DrawThatThang(tree, 210*TVector3(-0.383, -0.6634, 0.6428), TVector3(-0.383, -0.6634, 0.6428), n, "BL");
        }
        if (name == "AlphaDet4") {
            DrawThatThang(tree, 210*TVector3(-0.92215, 0, 0.38685), TVector3(-0.92215, 0, 0.38685), n, "ML");
        }
        if (name == "AlphaDet5") {
            DrawThatThang(tree, 210*TVector3(0.4611, -0.7986, 0.3869), TVector3(0.4611, -0.7986, 0.3869), n, "BR");
        }
        if (name == "AlphaDet6") {
            DrawThatThang(tree, 210*TVector3(0.4611, 0.7986, 0.3869), TVector3(0.4611, 0.7986, 0.3869), n, "TR");
        }
        // NOTE: 5 and 6 swapped in GDML! Potential swapping elsewhere?
    }

    delete[] cfile;
}
