//============================================================================
// Name        : RootLinked.cpp
// Author      : Maciej Slupecki
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

#ifndef __CINT__
#include <TCanvas.h>
#include <TApplication.h>
#include <TFile.h>
#include <TMath.h>
#include <THStack.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TFormula.h>
#include <TFile.h>
#include <TRandom3.h>
#include <TColor.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TLegend.h>
#include <TGraphErrors.h>
#include <TPaletteAxis.h>
#include <TFrame.h>
#include <TPaveStats.h>
#include <TLatex.h>

#include <TTree.h>
#include <TBranch.h>
#endif

#include "cliParser.h"
#include "DrawFuncs.h"
#include "TInputParams.h"
#include "TEvent.h"
#include "TDetector.h"

using namespace std;

int InitVnsTest(TInputParams &inp, string &inDir){
	// Set test multiplicities and constant vns
	const uint16_t nN=4;
	const double vn[nN] = {0, 0, 0.06, 0.025};
	const double dvn[nN] = {0, 0, 0.0, 0.0};

	for(uint16_t icent=0; icent<inp.vCentClass.size(); icent++){
		TInputCentralityClass* icc = inp.vCentClass.at(icent);
		for(uint16_t in=0; in<nN; in++){ // fill first two harmonics with 0
			icc->vv.push_back(vn[in]);
			icc->vdv.push_back(dvn[in]);
		}
	}
	return 0;
}

int InitDnDEtaReal(TInputParams &inp, string &inDir){
	// Set multiplicities according to the publication (https://aliceinfo.cern.ch/ArtSubmission/node/2227)
	const int nBins = 34;
	const float bin0 = -3.5;
	const float dN = 0.25;
	const int nCent = 8;
	float n[nBins];
	float *dnOverDeta[nCent];
	string centName[nCent] = {"0-5","5-10","10-20","20-30","30-40","40-50","50-60","60-70"};
	for(int ibin=0; ibin<nBins; ibin++){
		n[ibin] = bin0 + ibin*dN + dN/2; // setup x-axis array
	}
	// Init y-axis arrays (copy-pasted from the yaml file)
	float c0[nBins] = {1643,1670,1718,1787,1835,1912,1968,2001,2021,2017,1995,1970,1943,1929,1929,1943,1970,1995,2017,2021,2001,1968,1912,1835,1787,1718,1670,1643,1563,1474,1370,1324,1281,1244};
	float c1[nBins] = {1364,1391,1424,1474,1507,1569,1644,1679,1682,1672,1646,1621,1597,1583,1583,1597,1621,1646,1672,1682,1679,1644,1569,1507,1474,1424,1391,1364,1292,1218,1132,1093,1062,1032};
	float c2[nBins] = {1038,1061,1080,1114,1136,1178,1229,1253,1256,1247,1229,1210,1191,1181,1181,1191,1210,1229,1247,1256,1253,1229,1178,1136,1114,1080,1061,1038,977,921.3,857.7,829.6,807.4,787};
	float c3[nBins] = {714,726,738,759,772,797,827,842,844,838,826,811.9,799.2,792.4,792.4,799.2,811.9,826,838,844,842,827,797,772,759,738,726,714,665,625.4,582.6,565.5,551.4,538};
	float c4[nBins] = {475,482.7,489.7,502.6,510.6,522,539.9,549,549.3,545.5,537.5,527.6,519.3,514.7,514.7,519.3,527.6,537.5,545.5,549.3,549,539.9,522,510.6,502.6,489.7,482.7,475,440,413.6,386.7,375.6,368,359.9};
	float c5[nBins] = {302,306.3,310.1,317.9,322.3,327.6,335.1,340,340.2,337.7,332.5,326.3,320.7,317.5,317.5,320.7,326.3,332.5,337.7,340.2,340,335.1,327.6,322.3,317.9,310.1,306.3,302,277.5,261.3,244.7,238.4,233.8,229.4};
	float c6[nBins] = {178,179.9,181.7,186,188.2,189.8,193.5,196.4,196.5,194.8,191.4,187.5,184.3,182.5,182.5,184.3,187.5,191.4,194.8,196.5,196.4,193.5,189.8,188.2,186,181.7,179.9,178,163.2,153.4,143.8,140.3,138.7,136};
	float c7[nBins] = {94.9,96.1,96.8,98.3,98.8,99.1,101.2,102.7,103.1,102,100.3,98,96.1,95.2,95.2,96.1,98,100.3,102,103.1,102.7,101.2,99.1,98.8,98.3,96.8,96.1,94.9,86.8,81.9,77.3,75.8,75.1,73.8};
	dnOverDeta[0] = c0;
	dnOverDeta[1] = c1;
	dnOverDeta[2] = c2;
	dnOverDeta[3] = c3;
	dnOverDeta[4] = c4;
	dnOverDeta[5] = c5;
	dnOverDeta[6] = c6;
	dnOverDeta[7] = c7;

	// Create histos with published data
	gStyle->SetOptStat("nouie");
	TH1D* hpub[nCent];
	Color_t col[nCent] = {kViolet-6, kBlue+2, kBlue, kCyan+2, kSpring-1, kSpring+9, kYellow+1, kOrange+2};
	for(int icent=0; icent<nCent; icent++){
		stringstream ss;
		ss << "hPub-" << centName[icent];
		hpub[icent] = new TH1D(ss.str().c_str(), ss.str().c_str(), nBins, bin0, bin0+dN*nBins);
		for(int ibin=0; ibin<nBins; ibin++){
			hpub[icent]->SetBinContent(hpub[icent]->FindBin(n[ibin]), dnOverDeta[icent][ibin]);
		}
		hpub[icent]->GetXaxis()->SetTitle("Pseudorapidity (#eta)");
		hpub[icent]->GetYaxis()->SetTitle("dN_{ch}/d#eta");
		hpub[icent]->GetYaxis()->SetTitleOffset(1.5);
		hpub[icent]->SetTitle("");
		hpub[icent]->SetLineWidth(3);
		hpub[icent]->SetLineColor(col[icent]);
		hpub[icent]->GetYaxis()->SetRangeUser(0,2200);
	}

//	TCanvas *cvsMulti = new TCanvas("cvsMulti","cvsMulti",1800,800);
//	cvsMulti->cd();
	for(int icent=0; icent<nCent; icent++){

		// Fit function to the whole range of eta
		stringstream ssfname;
		ssfname << "fDnDEtaSim" << icent;
		TF1* fDnDEtaSim = new TF1(ssfname.str().c_str(), "[0]*TMath::Exp(-0.5*(x^2)/([1]^2)) - [2]*TMath::Exp(-0.5*(x^2)/([3]^2))" , -5.2, 5.2);
		double r0=0.23, r1=0.31;
		fDnDEtaSim->SetParLimits(0, 10, 2400);
		fDnDEtaSim->SetParameter(0, 1600);
		fDnDEtaSim->SetParLimits(1, 4.5, 5.0);
		fDnDEtaSim->SetParameter(1, 4.7);
		fDnDEtaSim->FixParameter(2, fDnDEtaSim->GetParameter(0)*r0);
		fDnDEtaSim->FixParameter(3, fDnDEtaSim->GetParameter(1)*r1);
		hpub[icent]->Fit(fDnDEtaSim,"Q0","",-3.5,5);
		fDnDEtaSim->FixParameter(0, fDnDEtaSim->GetParameter(0));
		fDnDEtaSim->FixParameter(1, fDnDEtaSim->GetParameter(1));
		fDnDEtaSim->ReleaseParameter(2);
		fDnDEtaSim->ReleaseParameter(3);
		hpub[icent]->Fit(fDnDEtaSim,"Q0","",-3.5,5);
		fDnDEtaSim->ReleaseParameter(0);
		fDnDEtaSim->ReleaseParameter(1);
		hpub[icent]->Fit(fDnDEtaSim,"Q0","",-3.5,5);

		inp.vCentClass.push_back(new TInputCentralityClass(fDnDEtaSim));

		// Drawing
		if(icent==0){
//			hpub[icent]->DrawCopy();
		}
		else{
//			hpub[icent]->DrawCopy("sames");
		}
//		fDnDEtaSim->Draw("sames");
	}

	return 0;
}

int InitVnsReal(TInputParams &inp, string &inDir){
	// Read vns from the published file
	string inPath = inDir + "/anizo-flow.root";
	TFile *fvn = new TFile(inPath.c_str(), "READ");
	const uint16_t nPlots = 3;
	TH1D* hv2[nPlots];
	TH1D* hv3[nPlots];
	if(fvn != NULL && fvn->IsOpen()){
		hv2[0] = (TH1D*)fvn->Get("Table 1/Hist1D_y1");
		hv2[1] = (TH1D*)fvn->Get("Table 1/Hist1D_y1_e1");
		hv2[2] = (TH1D*)fvn->Get("Table 1/Hist1D_y1_e2");
		hv3[0] = (TH1D*)fvn->Get("Table 2/Hist1D_y1");
		hv3[1] = (TH1D*)fvn->Get("Table 2/Hist1D_y1_e1");
		hv3[2] = (TH1D*)fvn->Get("Table 2/Hist1D_y1_e2");
		for(int i=0; i<nPlots; i++){
			if((hv2[i] == NULL) || (hv3[i] == NULL)){
				cout << "<E> InitVns(): Failed reading contents of file: " << i << ": (" << hv2[i] << " or " << hv3[i] << ")" << endl;
				return -1;
			}
		}
	}
	else{
		cout << "<E> InitVns(): Failed opening file: " << inPath << endl;
		return -1;
	}

	// Go over centralities
	for(uint16_t icent=0; icent<inp.vCentClass.size(); icent++){
		TInputCentralityClass* icc = inp.vCentClass.at(icent);
		for(uint16_t in=0; in<2; in++){ // fill first two harmonics with 0
			icc->vv.push_back(0);
			icc->vdv.push_back(0);
		}

		// Extract v2 from histo
		if(icent+1 > hv2[0]->GetNbinsX()){
			cout << "<E> InitVns(): Not enough bins in hv2 " << hv2[0]->GetName() << ": " << hv2[0]->GetTitle() << endl;
			return -1;
		}
		double v2 = hv2[0]->GetBinContent(icent+1);
		double dv21 = hv2[1]->GetBinContent(icent+1);
		double dv22 = hv2[2]->GetBinContent(icent+1);
		double dv2 = TMath::Sqrt(dv21*dv21 + dv22*dv22);
		icc->vv.push_back(v2);
		icc->vdv.push_back(dv2);

		// Extract v3 from histo
		if(icent+1 > hv3[0]->GetNbinsX()){
			cout << "<E> InitVns(): Not enough bins in hv3 " << hv3[0]->GetName() << ": " << hv3[0]->GetTitle() << endl;
			return -1;
		}
		double v3 = hv3[0]->GetBinContent(icent+1);
		double dv31 = hv3[1]->GetBinContent(icent+1);
		double dv32 = hv3[2]->GetBinContent(icent+1);
		double dv3 = TMath::Sqrt(dv31*dv31 + dv32*dv32);
		icc->vv.push_back(v3);
		icc->vdv.push_back(dv3);
	}
	return 0;
}

void InitHistos(const TInputParams &inp, const vector<TDetector*> &vDet, vector<TH1D*> &vh, vector<TH2D*> &vh2){
	// Init 1D histos
	uint16_t nBins = inp.vCentClass.size();
	uint16_t bin0 = 0;
	uint16_t bin1 = 8;
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		stringstream ssNch;
		ssNch << "hNch_" << idet << "_" << vDet.at(idet)->GetType();
		TH1D* hNch = new TH1D(ssNch.str().c_str(), ssNch.str().c_str(), nBins, bin0, bin1);
		hNch->SetXTitle("Centrality");
		hNch->SetYTitle("N_{ch}");
		hNch->SetLineWidth(3);
		hNch->SetLineColor(kBlack);
		hNch->Sumw2();
		vh.push_back(hNch);

		// Vn
		for(uint16_t iharm=inp.iharm0; iharm<vDet.at(idet)->GetHarmReco().size(); iharm++){
			stringstream ssvn, ssvdet;
			ssvn << "v_{" << iharm << "}";
			ssvdet << ssvn.str() << "_" << idet << "_" << vDet.at(idet)->GetType();
			TH1D* hv = new TH1D(ssvdet.str().c_str(), ssvdet.str().c_str(), nBins, bin0, bin1);
			hv->SetXTitle("Centrality");
			hv->SetYTitle(ssvn.str().c_str());
			hv->SetLineWidth(2);
			hv->SetLineColor(kBlack);
			hv->Sumw2();
			vh.push_back(hv);
		}

		// R - using simulated psi
		for(uint16_t iharm=inp.iharm0; iharm<vDet.at(idet)->GetHarmReco().size(); iharm++){
			stringstream ssrn, ssrdet;
			ssrn << "R_{" << iharm << "}^{sim}";
			ssrdet << ssrn.str() << "_" << idet << "_" << vDet.at(idet)->GetType();
			TH1D* hr = new TH1D(ssrdet.str().c_str(), ssrdet.str().c_str(), nBins, bin0, bin1);
			hr->SetXTitle("Centrality");
			hr->SetYTitle(ssrn.str().c_str());
			hr->SetLineWidth(2);
			hr->SetLineColor(kBlack);
			hr->Sumw2();
			vh.push_back(hr);
		}

		// R - in case mimicing experiment, where real psi is not known
		for(uint16_t iharm=inp.iharm0; iharm<vDet.at(idet)->GetHarmReco().size(); iharm++){
			stringstream ssrn, ssrdet;
			ssrn << "R_{" << iharm << "}^{exp}";
			ssrdet << ssrn.str() << "_" << idet << "_" << vDet.at(idet)->GetType();
			TH1D* hr = new TH1D(ssrdet.str().c_str(), ssrdet.str().c_str(), nBins, bin0, bin1);
			hr->SetXTitle("Centrality");
			hr->SetYTitle(ssrn.str().c_str());
			hr->SetLineWidth(2);
			hr->SetLineColor(kBlack);
			hr->Sumw2();
			vh.push_back(hr);
		}
	}

	// Print histo names
	for(uint16_t ih=0; ih<vh.size(); ih++){
		cout << "  <I> InitHistos(): " << ih << " - " << vh.at(ih)->GetName() << endl;
	}

	// Init 2D histos
	uint16_t nBinsAngle = 50;
	float binAngle0 = -TMath::Pi()/2;
	float binAngle1 = TMath::Pi()/2;
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		stringstream sscorr;
		sscorr << "corr_" << idet << "_" << vDet.at(idet)->GetType();
		TH2D* hcorr = new TH2D(sscorr.str().c_str(), sscorr.str().c_str(), nBinsAngle, binAngle0, binAngle1, nBinsAngle, binAngle0, binAngle1);
		hcorr->SetXTitle("Reconstructed #psi");
		hcorr->SetYTitle("Simulated #psi");
		vh2.push_back(hcorr);
	}
}

void SaveHistos(vector<TH1D*> &vh, vector<TH2D*> &vh2, string filepath){
	TFile* f = new TFile(filepath.c_str(), "RECREATE");
	for(uint16_t ih=0; ih<vh.size(); ih++){
		vh.at(ih)->Write();
	}
	for(uint16_t ih=0; ih<vh2.size(); ih++){
		vh2.at(ih)->Write();
	}
	f->Close();
}

void ScaleHistos(vector<TH1D*> &vh, uint32_t nEvents){
	for(uint16_t ih=0; ih<vh.size(); ih++){
		vh.at(ih)->Scale(1./nEvents);
	}
}

void CalculateExpeimentalR(vector<TH1D*> &vh, vector<TDetector*> &vDet, TInputParams &par){
	const uint16_t nHarmPars = 3; // 3 = 1 and 1 and 1 (Rsim, Rexp and vn)
	const uint16_t nIndepPars = 1; // 1 for nCh
	const uint16_t iRexpOffset = 5;
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		TDetector* det = vDet.at(idet);
		size_t harmRecoSize = det->GetHarmReco().size() - par.iharm0;
		uint16_t iRexpBase     = idet                 * (harmRecoSize * nHarmPars + nIndepPars) + iRexpOffset;
		uint16_t iRexpNext1Det = (idet+1)%vDet.size() * (harmRecoSize * nHarmPars + nIndepPars) + iRexpOffset;
		uint16_t iRexpNext2Det = (idet+2)%vDet.size() * (harmRecoSize * nHarmPars + nIndepPars) + iRexpOffset;
		for(uint16_t iharm=0; iharm<harmRecoSize; iharm++){
			uint16_t irexp = iRexpBase + iharm;
			uint16_t irsim = irexp - harmRecoSize;
			uint16_t irsimNext1 = iRexpNext1Det + iharm - harmRecoSize;
			uint16_t irsimNext2 = iRexpNext2Det + iharm - harmRecoSize;
			TH1D* hExp      = vh.at(irexp);
			TH1D* hSim      = vh.at(irsim);
			TH1D* hSimNext1 = vh.at(irsimNext1);
			TH1D* hSimNext2 = vh.at(irsimNext2);
			for(uint16_t icentbin=1; icentbin<=hExp->GetNbinsX(); icentbin++){
				double a = hSim->GetBinContent(icentbin);
				double b = hSimNext1->GetBinContent(icentbin);
				double c = hSimNext2->GetBinContent(icentbin);
				double rexp = TMath::Sqrt( a * c / b );
				hExp->SetBinContent(icentbin-1, rexp);
				double da = hSim->GetBinError(icentbin);
				double db = hSimNext1->GetBinError(icentbin);
				double dc = hSimNext2->GetBinError(icentbin);
				double drexp = TMath::Sqrt( da*rexp/(2.0*a) + db*rexp/(2.0*b) + dc*rexp/(2.0*c) );
				hExp->SetBinError(icentbin-1, drexp);
			}
		}
	}
}

void RunToyMC(TInputParams &inp, const string &sGeo, const string &dirPath){
	// Initialize detector geometry
	vector<TDetector*> vDet;
	string sGeoTmp = sGeo;
	while(true){
		size_t sepPos = sGeoTmp.find_last_of('-');
		string lastGeo = sGeoTmp.substr(sepPos+1);
		vDet.push_back(new TDetector(lastGeo, &inp));
		if(sepPos == string::npos){
			break;
		}
		sGeoTmp.erase(sepPos);
	}

	// Print which geometry has been initialized
	cout << "  <I> RunToyMC(): Initialized detectors: " << endl;
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		cout << "  <I>   " << idet;
		if(idet==0){
			cout << "-R ";
		}
		else{
			cout << "-vn";
		}
		cout << " " << vDet.at(idet)->GetType();
		cout << "\t| EtaRange = " << vDet.at(idet)->GetMinEta() << " .. " << vDet.at(idet)->GetMaxEta() << endl;
	}

	// Init histos
	vector<TH1D*> vh;
	vector<TH2D*> vh2;
	InitHistos(inp, vDet, vh, vh2);

	// Main centrality and event loops
	TRandom3* rnd = new TRandom3(0);
//	for(uint16_t icent=0; icent<1; icent++){
	for(uint16_t icent=0; icent<inp.vCentClass.size(); icent++){
		cout << "  <I> RunToyMC(): icent=" << icent << endl;
		for(uint32_t evn=0; evn<inp.nEvents; evn++){
			TEvent* event = new TEvent(&inp, rnd);
			event->SetCentrality(icent);
			event->Simulate();
			event->Transport(vDet);
			event->ReconstructR(vDet);   // based on simulated psi
			//event->ReconstructRSub3(vDet); // mimics experimental case, where real psi is not known
			event->FillHistos(vh, vh2, vDet);

//			if(evn<4){
//				event->Draw(evn);
//			}
//			else{
				delete event;
//			}
		}
	}

	TCanvas *c = new TCanvas("c","c",1800,850);
	c->Divide(3);
	for(uint16_t i=0; i<vh2.size(); i++){
		c->cd(i+1);
		vh2.at(i)->Draw("colz");
	}

	// Finish averaging (assumes the same number of events is simulated for all centralities)
	ScaleHistos(vh, inp.nEvents);
	CalculateExpeimentalR(vh, vDet, inp);

	// Save to file
	stringstream ssfilename;
	ssfilename << dirPath << sGeo << ".root";
	SaveHistos(vh, vh2, ssfilename.str());
}

int main(int argc, char* argv[])
{
	const string dirPath = "outputs/";
	// Command line input handling
	cliParser cli;
	cli.AddPar("m", "Mode of operation", true);
	cli.GetModeList()->AddPar("sim", "Run main MC simulation");
	cli.GetModeList()->AddPar("drawFileContents", "Read sim output histos and overlay them");
	cli.AddPar("g", "Geometry mode: V0A_1x8-V0C_1x8 - two detectors are initialized, the last one is used to get R, others to get Vn. See detector string options in TDetector::InitGeo()");
	cli.AddPar("i", "Input directory");
	cli.AddPar("n", "Number of events to be simulated");
	if(cli.Parse(argc, argv)!=0){
		cli.PrintUsage(cout, "aliFitSimAnalysis");
		return -1;
	}

	if(cli.GetParString("m").compare("drawFileContents")==0){
		TApplication theApp("App", &argc, argv);
			string inputDir = cli.GetParString("i");
			if(inputDir.size()<1){ // set default value
				inputDir = "inputs/";
			}
			TInputParams inp;
			InitDnDEtaReal(inp, inputDir);
			InitVnsReal(inp, inputDir);
			DrawFuncs::FileContents(inp);
			cout << "Done." << endl;
		theApp.Run();
	}
	else if(cli.GetParString("m").compare("sim")==0){
		TApplication theApp("App", &argc, argv);
			// Init input parameters from CLI
			string sGeo = cli.GetParString("g");
			if(sGeo.size()<1){
				cout << "<E> No -g parameter provided. Aborting. " << endl;
				return -1;
			}

			TInputParams inp;
			if(cli.GetParString("n").size()>0){
				inp.nEvents = cli.GetParInt("n");
			}
			else{
				inp.nEvents = 1e3;
			}
			cout << "  <I> NEvents = " << inp.nEvents << endl;

			string inputDir = cli.GetParString("i");
			if(inputDir.size()<1){ // set default value
				inputDir = "inputs/";
			}

			InitDnDEtaReal(inp, inputDir);
//			InitVnsTest(inp, inputDir);
			InitVnsReal(inp, inputDir);
//			inp.Print();
			RunToyMC(inp, sGeo, dirPath);
			cout << "Done." << endl;
		theApp.Run();
	}
	return 0;
}

