void BKGall()
{
//Set File Parameters
	int Diameter = 8;
	int Length = 3;

        int yHeight = 1000;
        double xMin  = 0;
        double xMax  = 5;

	TCanvas *c1 = new TCanvas();
        c1->SetTickx();
        c1->SetTicky();
        c1->SetGridx();
        c1->SetGridy();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Read Data from TFile
	//No Shield Variables
	TFile *input1 = new TFile("TestBKG_All_Half.root", "read");
        TTree *tree1 =  (TTree*)input1->Get("Edep");
        double fEdep_HPGe;
	tree1->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe);
        int entries1 = tree1->GetEntries();

	//All Shield Variables
        TFile *input3 = new TFile("TestBKG_All.root", "read");
        TTree *tree3 =  (TTree*)input3->Get("Edep");
        double fEdep_HPGe3;
        tree3->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe3);
        int entries3 = tree3->GetEntries();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Graph the data
	TString Title = "Simulated Background Spectrum Half vs Full Shell, Shielded";

	//No Shield
        TH1F *hist1 = new TH1F("hist1", Title, 1000, xMin, xMax);
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
        TH1F *hist3 = new TH1F("hist3", Title, 1000, xMin, xMax);
        for(int i = 0; i < entries3; i++)
        {
                tree3->GetEntry(i);
                hist3->Fill(fEdep_HPGe3);
        }
        hist3->SetLineColorAlpha(kBlue, 0.35);
        hist3->GetXaxis()->SetTitle("Energy (MeV)");
        hist3->GetYaxis()->SetTitle("Counts");
        hist3->GetYaxis()->SetRangeUser(0, yHeight);
        hist3->SetStats(0);
        hist3->Draw("Same");

	//Construct Legend
	TLegend *leg = new TLegend(0.4, 0.77, 0.65, 0.87);
	leg->SetBorderSize(0);
	leg->AddEntry(hist1, "Half Shell, Shielded", "l");
        leg->AddEntry(hist3, "Full Shell, Shielded", "l");
	leg->Draw();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Print the Graph
        TString GraphName = "BackgroundAllShield_HalfvsFullShell.pdf";
                
	c1->Print(GraphName);
}
