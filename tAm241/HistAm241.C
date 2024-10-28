void HistAm241() {

    TString title1 = "Simulated Energy Spectrum of Am241 Decay";
    TCanvas* canvas = new TCanvas();
    canvas->SetLogy(1);
    canvas->SetTicky();
    canvas->SetTickx();

    TFile* input = new TFile("Am241.root", "read");
    
    TTree* tree1 = (TTree*)input->Get("DetScoring");
    double fEdep;
	tree1->SetBranchAddress("fEdep", &fEdep);
    int entries1 = tree1->GetEntries();

    int yHeight = 500;
    int binNo = 500;
    double xMin  = 0;
    double xMax  = 6;
    
    double fEdepMax = INT_MIN;
    double fEdepMin = INT_MAX;
    TH1F *hist1 = new TH1F("hist1", title1, binNo, xMin, xMax);
    for (int i = 0; i < entries1; i++) {
        tree1->GetEntry(i);
        hist1->Fill(fEdep);
        if (fEdep > fEdepMax) fEdepMax = fEdep;
        if (fEdep < fEdepMin) fEdepMin = fEdep;
    }
    for (int i = 0; i < binNo; i++) {
        std:cout << " | " << hist1->GetBinContent(i);
    }
    std::cout << " |" << std::endl;
    std::cout << fEdepMax << std::endl;
    std::cout << fEdepMin << std::endl;

    hist1->SetLineColorAlpha(kRed, 0.35);
    hist1->GetXaxis()->SetTitle("Energy / MeV");
    hist1->GetYaxis()->SetTitle("Counts");	
	hist1->SetStats(0);	
	hist1->Draw();
    // hist1->SaveAs("output.png");

    // TLine *l1 = new TLine(1.77, 0, 1.77, 100);
    // l1->SetLineWidth(1);
    // l1->SetLineStyle(kDashed);
    // l1->Draw();
    // TLine *l2 = new TLine(1.47, 0, 1.47, 400);
    // l2->SetLineWidth(1);
    // l2->SetLineStyle(kDashed);
    // l2->Draw();


}