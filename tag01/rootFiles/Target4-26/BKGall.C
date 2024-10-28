void BKGall()
{
//Set File Parameters
	int Diameter = 8;
	int Length = 3;

        int yHeight = 20000;
        double xMin = 0.35;
        double xMax = 0.6;
	int binNo   = 1000;

	TCanvas *c1 = new TCanvas();
        c1->SetTickx();
        c1->SetTicky();
        c1->SetGridx();
        c1->SetGridy();
	c1->SetLogy();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Read Data from TFile
	//Background + 478 keV
	TFile *input1 = new TFile("TestBKG_d8_target.root", "read");
        TTree *tree1 =  (TTree*)input1->Get("Edep");
        double fEdep_HPGe;
	tree1->SetBranchAddress("fEdep_HPGe", &fEdep_HPGe);
        int entries1 = tree1->GetEntries();

	//Background Only
        TTree *tree3 =  (TTree*)input1->Get("EdepBKG");
        double fEdep_HPGe3;
        tree3->SetBranchAddress("fEdep_HPGe_BKGonly", &fEdep_HPGe3);
        int entries3 = tree3->GetEntries();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Graph the data
	TString Title = "Simulated Background Spectrum with Boron-10 Neutron Capture";

	//BKG + 478 keV
        TH1F *hist1 = new TH1F("hist1", Title, binNo, xMin, xMax);
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

	//BKG only
        TH1F *hist3 = new TH1F("hist3", Title, binNo, xMin, xMax);
        for(int i = 0; i < entries3; i++)
        {
                tree3->GetEntry(i);
                hist3->Fill(fEdep_HPGe3);
        }
        hist3->SetLineColorAlpha(kGreen, 0.35);
        hist3->GetXaxis()->SetTitle("Energy (MeV)");
        hist3->GetYaxis()->SetTitle("Counts");
        hist3->GetYaxis()->SetRangeUser(1, yHeight);
        hist3->SetStats(0);
        hist3->Draw("Same");

	//Construct Legend
	TLegend *leg = new TLegend(0.4, 0.67, 0.85, 0.87);
	leg->SetBorderSize(0);
	leg->AddEntry(hist1, "Background & B-10 Neutron Caputre", "l");
        leg->AddEntry(hist3, "Background w/o B-10 Neutron Caputre", "l");
//	leg->Draw();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Calculate Singal-to-Noise Ratio
	double LowerBound = 0.455;
	double UpperBound = 0.490;
	
	//Draw Integration region
	TLine *l1 = new TLine(LowerBound,1,LowerBound,yHeight);
	l1->SetLineWidth(1);
	l1->SetLineStyle(9);
	l1->SetLineColor(kBlue);
//	leg->AddEntry(l1, "Integration Width", "l");
        l1->Draw();

        TLine *l2 = new TLine(UpperBound,1,UpperBound,yHeight);
	l2->SetLineWidth(1);
	l2->SetLineStyle(9);
	l2->SetLineColor(kBlue);
        l2->Draw();

	float Signal = hist1->Integral(hist1->FindFixBin(LowerBound), hist1->FindFixBin(UpperBound));

	float Noise = hist3->Integral(hist3->FindFixBin(LowerBound), hist3->FindFixBin(UpperBound));

	float SNR = Signal/Noise;

	cout << "Signal-to-Noise Ratio: " << SNR << endl;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//Print the Graph
        TString GraphName = "BackgroundAndTargetHighlight_4_26.png";
                
	c1->Print(GraphName);
}
