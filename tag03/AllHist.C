#include <string> 
#include <iostream>

void AllHist(std::string file) {

    char * cfile = new char [file.length()+1];
    std::strcpy (cfile, file.c_str());
    TString title = "Simulated Energy Spectrum of ";
    std::cout << "Opening File: " << file << std::endl;
    TFile* input = new TFile(cfile, "read");

    int yHeight = 500;
    int binNo = 500;
    double xMin  = 0;
    double xMax  = 2.5;

    for (int i = 0; i < input->GetListOfKeys()->GetEntries() - 3; ++i) { 
        
        TTree* tree = (TTree*)input->Get(input->GetListOfKeys()->At(i)->GetName());
        TCanvas* canvas = new TCanvas();
        canvas->SetLogy(1);
        canvas->SetTicky();
        canvas->SetTickx();
        cout << input->GetListOfKeys()->At(i)->GetName() << '\n'; 
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
        std::cout << '\t' << fEdepMax << std::endl;
        std::cout << '\t' << fEdepMin << std::endl;
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