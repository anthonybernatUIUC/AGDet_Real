void BKGtest()
{
//Set File Parameters
	int Diameter = 8;
	int Length   = 3;

        int yHeight  = 20000;
	double xMin  = 0;
	double xMax  = 5;

	TCanvas *c1 = new TCanvas();

	c1->SetLogy();

        c1->SetTickx();
        c1->SetTicky();
        c1->SetGridx();
        c1->SetGridy();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Read Data from TFile
	TFile *input1 = new TFile("TestBKG_d8_target.root", "read");
        TTree *tree1 =  (TTree*)input1->Get("Edep");
        double fEdep_HPGe;
	tree1->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe);
        int entries1 = tree1->GetEntries();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Graph the data
	TString Title = "Simulated Background Spectrum with Boron-10 Neutron Capture";

	//Draw Spectra
        TH1F *hist1 = new TH1F("hist1", Title, 1000, xMin , xMax);
        for(int i = 0; i < entries1; i++)
        {
                tree1->GetEntry(i);
                hist1->Fill(fEdep_HPGe);
        }
        hist1->SetLineColorAlpha(kRed, 0.35);
        hist1->GetXaxis()->SetTitle("Energy (MeV)");
        hist1->GetYaxis()->SetTitle("Counts");
        hist1->GetYaxis()->SetRangeUser(1, yHeight);	
	hist1->SetStats(0);	
	hist1->Draw();

	//Draw Legend
	TLegend *leg = new TLegend(0.4, 0.67, 0.85, 0.87);
	leg->SetBorderSize(0);
	leg->AddEntry(hist1, "Background & B-10 Neutron Caputre", "l");
	leg->Draw();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Print the Graph
        TString GraphName = "BackgroundAndTarget_4_26.png";
                
	c1->Print(GraphName);
}
