void BKGall()
{
//Set File Parameters
	int Diameter = 8;
	int Length = 3;

        int yHeight = 1000;
        double xMin = 0;
        double xMax = 5;
	int binNo   = 1000;

	TCanvas *c1 = new TCanvas();
        c1->SetTickx();
        c1->SetTicky();
        c1->SetGridx();
        c1->SetGridy();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Read Data from TFile
	//No Shield Variables
	TFile *input1 = new TFile("TestBKG_none.root", "read");
        TTree *tree1 =  (TTree*)input1->Get("Edep");
        double fEdep_HPGe;
	tree1->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe);
        int entries1 = tree1->GetEntries();

	//All Shield Variables
	//Shield3 Outer Diameter = 6.5 in
        TFile *input2 = new TFile("TestBKG_d65.root", "read");
        TTree *tree2 =  (TTree*)input2->Get("Edep");
        double fEdep_HPGe2;
        tree2->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe2);
        int entries2 = tree2->GetEntries();

	//Shield3 Outer Diameter = 8 in
        TFile *input3 = new TFile("TestBKG_d8.root", "read");
        TTree *tree3 =  (TTree*)input3->Get("Edep");
        double fEdep_HPGe3;
        tree3->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe3);
        int entries3 = tree3->GetEntries();

	//Shield3 Outer Diameter = 11 in
        TFile *input4 = new TFile("TestBKG_d11.root", "read");
        TTree *tree4 =  (TTree*)input4->Get("Edep");
        double fEdep_HPGe4;
        tree4->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe4);
        int entries4 = tree4->GetEntries();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Graph the data
	TString Title = "Simulated Background Spectrum with Increasing Shielding";

	//No Shield
        TH1F *hist1 = new TH1F("hist1", Title, binNo, xMin, xMax);
        for(int i = 0; i < entries1; i++)
        {
                tree1->GetEntry(i);
                hist1->Fill(fEdep_HPGe);
        }
        hist1->SetLineColorAlpha(kRed, 0.35);
        hist1->GetXaxis()->SetTitle("Energy (MeV)");
        hist1->GetYaxis()->SetTitle("Counts");
        hist1->GetYaxis()->SetRangeUser(0, yHeight);	
	hist1->SetStats(0);	
	hist1->Draw();

	//All Sheilds
	//Shield3 Outer Diameter = 6.5 cm
        TH1F *hist2 = new TH1F("hist2", Title, binNo, xMin, xMax);
        for(int i = 0; i < entries2; i++)
        {
                tree2->GetEntry(i);
                hist2->Fill(fEdep_HPGe2);
        }
        hist2->SetLineColorAlpha(kOrange, 0.35);
        hist2->GetXaxis()->SetTitle("Energy (MeV)");
        hist2->GetYaxis()->SetTitle("Counts");
        hist2->GetYaxis()->SetRangeUser(0, yHeight);
        hist2->SetStats(0);
        hist2->Draw("Same");

	//Shield3 Outer Diameter = 8 cm
        TH1F *hist3 = new TH1F("hist3", Title, binNo, xMin, xMax);
        for(int i = 0; i < entries3; i++)
        {
                tree3->GetEntry(i);
                hist3->Fill(fEdep_HPGe3);
        }
        hist3->SetLineColorAlpha(kGreen, 0.35);
        hist3->GetXaxis()->SetTitle("Energy (MeV)");
        hist3->GetYaxis()->SetTitle("Counts");
        hist3->GetYaxis()->SetRangeUser(0, yHeight);
        hist3->SetStats(0);
        hist3->Draw("Same");

	//Shield3 Outer Diameter = 11 cm
        TH1F *hist4 = new TH1F("hist4", Title, binNo, xMin, xMax);
        for(int i = 0; i < entries3; i++)
        {
                tree4->GetEntry(i);
                hist4->Fill(fEdep_HPGe4);
        }
        hist4->SetLineColorAlpha(kBlue, 0.35);
        hist4->GetXaxis()->SetTitle("Energy (MeV)");
        hist4->GetYaxis()->SetTitle("Counts");
        hist4->GetYaxis()->SetRangeUser(0, yHeight);
        hist4->SetStats(0);
        hist4->Draw("Same");

	//Construct Legend
	TLegend *leg = new TLegend(0.4, 0.67, 0.85, 0.87);
	leg->SetBorderSize(0);
	leg->AddEntry(hist1, "No Shielding", "l");
	leg->AddEntry(hist2, "S3 Thickness = 0.6875 in", "l");
        leg->AddEntry(hist3, "S3 Thickness = 1.4375 in", "l");
	leg->AddEntry(hist4, "S3 Thickness = 2.9375 in", "l");
	leg->Draw();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Print the Graph
        TString GraphName = "BackgroundAllShield.png";
                
	c1->Print(GraphName);
}
