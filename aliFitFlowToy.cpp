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
#include "TInputParams.h"
#include "TEvent.h"
#include "TDetector.h"

using namespace std;

int InitMultiTest(TInputParams &inp, string &inDir){
	for(int icent=0; icent<8; icent++){
		inp.vCentClass.push_back(new TInputCentralityClass(1875-250*(icent)));
	}
	return 0;
}

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

int InitMultiReal(TInputParams &inp, string &inDir){
	// TODO::Manual mode switch
	bool initV0 = true; // if false then V0+ is initialized

	// TODO: set arbitrary multiplicities

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
	// Select eta range
	// TODO: in order to move geometry into several rings the structure needs to be changed and eta selection needs to be done in the TEvent
	double etaA0=0, etaA1=0, etaC0=0, etaC1=0;
	if(initV0){ // current V0
		etaA0 = 2.8;
		etaA1 = 5.1;
		etaC0 = -3.7;
		etaC1 = -1.7;
	}
	else{ // V0+
		etaA0 = 2.19;
		etaA1 = 5.08;
	}
	TCanvas *cvsMulti = new TCanvas("cvsMulti","cvsMulti",1800,800);
	cvsMulti->cd();
//	cvsMulti->Divide(2);
	for(int icent=0; icent<nCent; icent++){
		stringstream ssfnameA, ssfnameC;
		ssfnameA << "fpolA" << icent;
		ssfnameC << "fpolC" << icent;
		// Fit functions for A- and C-side separately - to obtain integral within selected range (not to be affected by histo binning)
		TF1* fpolA = new TF1(ssfnameA.str().c_str(),"pol3", etaA0, etaA1);
		TF1* fpolC = new TF1(ssfnameC.str().c_str(),"pol3", etaC0, etaC1);
		hpub[icent]->Fit(ssfnameA.str().c_str(),"0","",etaA0,etaA1);
		hpub[icent]->Fit(ssfnameC.str().c_str(),"0","",etaC0,etaC1);
		float nPartA = fpolA->Integral(etaA0, etaA1)/(etaA1-etaA0);
		float nPartC = fpolC->Integral(etaC0, etaC1)/(etaC1-etaC0);

		//TODO::Make a proper removal of the correlation. For now use 2x more particles per event (half to calc R and Psi_n and another half for vn)
		cout << "nPartA,C: " << nPartA << " " << nPartC << endl;
		float nPart=0;
		if(initV0){ // current V0
			//nPart = (nPartA) * 2;
			nPart = (nPartC) * 2;
			//nPart = (nPartA + nPartC) * 2;
		}
		else{ // V0+
			nPart = nPartC * 2;
		}
		inp.vCentClass.push_back(new TInputCentralityClass(round(nPart)));
		// !!!!!!!!!!!!!!! ^

		// Drawing
//		cvsMulti->cd(1);
			if(icent==0){
//				hpub[icent]->GetXaxis()->SetRangeUser(-4,5.5);
				hpub[icent]->GetXaxis()->SetRangeUser(etaA0,etaA1);
				hpub[icent]->DrawCopy();
			}
			else{
				hpub[icent]->DrawCopy("sames");
			}
			fpolA->Draw("same");
//		cvsMulti->cd(2);
//			if(icent==0){
//				hpub[icent]->GetXaxis()->SetRangeUser(etaC0,etaC1);
//				hpub[icent]->DrawCopy();
//			}
//			else{
//				hpub[icent]->DrawCopy("sames");
//			}
			fpolC->Draw("same");
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

void Overlay(string inpath, TInputParams &inp){
	gStyle->SetOptStat(0);

	//TFile* fideal = new TFile("outputs/ideal0.root","READ");
	//TFile* ffit8 = new TFile("outputs/FIT8.root","READ");

	TFile* fideal = new TFile("outputs/V0A8.root","READ");
	TFile* ffit8 = new TFile("outputs/V0C8.root","READ");

	TH1D *hR2ideal = (TH1D*)fideal->Get("hR2");
	TH1D *hR3ideal = (TH1D*)fideal->Get("hR3");
	TH1D *hv2ideal = (TH1D*)fideal->Get("hv2");
	TH1D *hv3ideal = (TH1D*)fideal->Get("hv3");
	TH1D *hR2fit8 = (TH1D*)ffit8->Get("hR2");
	TH1D *hR3fit8 = (TH1D*)ffit8->Get("hR3");
	TH1D *hv2fit8 = (TH1D*)ffit8->Get("hv2");
	TH1D *hv3fit8 = (TH1D*)ffit8->Get("hv3");

	hR2ideal->SetLineColor(kRed);
	hR3ideal->SetLineColor(kBlue);
	hv2ideal->SetLineColor(kRed);
	hv3ideal->SetLineColor(kBlue);
	hR2fit8->SetLineColor(kRed+2);
	hR3fit8->SetLineColor(kBlue+2);
	hv2fit8->SetLineColor(kRed+2);
	hv3fit8->SetLineColor(kBlue+2);

	// Setup input values for referenece
	int nbins = hR2ideal->GetNbinsX();
	TH1D *hv2in = new TH1D("hv2in","hv2in",nbins,hR2ideal->GetBinLowEdge(1),hR2ideal->GetBinLowEdge(nbins+1));
	TH1D *hv3in = new TH1D("hv3in","hv3in",nbins,hR2ideal->GetBinLowEdge(1),hR2ideal->GetBinLowEdge(nbins+1));
	TH1D *hnPartIn = new TH1D("hnPartIn","nParticles",nbins,hR2ideal->GetBinLowEdge(1),hR2ideal->GetBinLowEdge(nbins+1));
	hnPartIn->SetLineWidth(2);
	hv2in->SetMarkerColor(kGray+2);
	hv2in->SetMarkerSize(2);
	hv2in->SetMarkerStyle(33);
	hv3in->SetMarkerColor(kBlue-2);
	hv3in->SetMarkerSize(2);
	hv3in->SetMarkerStyle(33);
	for(uint16_t icent=0; icent<inp.vCentClass.size(); icent++){
		TInputCentralityClass* icc = inp.vCentClass.at(icent);
		hv2in->Fill(icent, icc->vv.at(2));
		hv3in->Fill(icent, icc->vv.at(3));
		hnPartIn->Fill(icent, icc->nPart/2);
	}

	// Set bin labels
	string centName[8] = {"0-5%","5-10%","10-20%","20-30%","30-40%","40-50%","50-60%","60-70%"};
	for(uint16_t ibin=1; ibin<=hR2ideal->GetNbinsX(); ibin++){
		hR2ideal->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hR3ideal->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hv2ideal->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hv3ideal->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hv2in->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hv3in->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hnPartIn->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
	}

	TLegend *leg = new TLegend(0.35,0.12, 0.57,0.4);
	leg->AddEntry(hR2ideal,"R_{2} V0A","lep");
//	leg->AddEntry(hR3ideal,"R_{3} V0A","lep");
	leg->AddEntry(hR2fit8,"R_{2} V0C","lep");
//	leg->AddEntry(hR3fit8,"R_{3} V0C","lep");

	TCanvas* cEpr = new TCanvas("cEpr","cEpr",1600,980);
	cEpr->Divide(2,2);
	cEpr->cd(1);
		hR2ideal->GetYaxis()->SetRangeUser(0,1.1);
		hR2ideal->SetTitle("R");
		hR2ideal->DrawCopy();
//		hR3ideal->DrawCopy("same");
		hR2fit8->DrawCopy("same");
//		hR3fit8->DrawCopy("same");
		leg->Draw();

	cEpr->cd(2);
		//hR2->GetYaxis()->SetRangeUser(0,1.1);
		hv2ideal->GetYaxis()->SetRangeUser(0,0.12);
		hv2ideal->SetTitle("v_{obs}");
		hv2ideal->DrawCopy();
//		hv3ideal->DrawCopy("same");
		hv2fit8->DrawCopy("same");
//		hv3fit8->DrawCopy("same");
	cEpr->cd(3);
		hv2ideal->Divide(hR2ideal);
		hv3ideal->Divide(hR3ideal);
		hv2fit8->Divide(hR2fit8);
		hv3fit8->Divide(hR3fit8);
		hv2ideal->SetYTitle("v = v_{obs}/R");
		hv2ideal->SetTitle("v = v_{obs}/R");
		hv2ideal->Draw();
//		hv3ideal->Draw("same");
		hv2fit8->Draw("same");
//		hv3fit8->Draw("same");
		hv2in->Draw("P same");
//		hv3in->Draw("P same");
	cEpr->cd(4);
		hnPartIn->Draw();

	for(uint16_t ibin=1; ibin<=hv2ideal->GetNbinsX(); ibin++){
		cout << ibin << " " << hv2ideal->GetBinContent(ibin) << "\t" << hv2fit8->GetBinContent(ibin) << endl;
	}
}

void CompareGeom(TInputParams &inp){
	gStyle->SetOptStat(0);

	// Setup file names
	vector<string> vFileName;
	vFileName.push_back("ideal0");
	for(uint16_t nSectors=4; nSectors<=24; nSectors++){
		stringstream ss;
		ss << "FIT" << nSectors;
		vFileName.push_back(ss.str());
	}

	// Open files
	vector<TFile*> vFileList;
	for(uint16_t ifile=0; ifile<vFileName.size(); ifile++){
		string path = "outputs/" + vFileName.at(ifile) + ".root";
		TFile* f = new TFile(path.c_str(),"READ");
		if(f==NULL || !f->IsOpen()){
			cout << "<E> CompareGeom(): File not opened: " << ifile << " " << path << endl;
			return;
		}
		vFileList.push_back(f);
	}

	// Read histos from files
	vector<TH1D*> vhr2, vhv2Obs;
	for(uint16_t ifile=0; ifile<vFileList.size(); ifile++){
		vhr2.push_back( (TH1D*)vFileList.at(ifile)->Get("hR2") );
		vhv2Obs.push_back( (TH1D*)vFileList.at(ifile)->Get("hv2") );
	}

	// Name histos and make the division
	vector<TH1D*> vhv2;
	string xtitle = "Centrality";
	for(uint16_t ifile=0; ifile<vFileList.size(); ifile++){
		stringstream ss1;
		ss1 << "R2_" << vFileName.at(ifile);
		vhr2.at(ifile)->SetName(ss1.str().c_str());
		vhr2.at(ifile)->SetTitle("R");
		vhr2.at(ifile)->SetYTitle("R");
		vhr2.at(ifile)->SetXTitle(xtitle.c_str());
		stringstream ss2;
		ss1 << "v2Obs_" << vFileName.at(ifile);
		vhv2Obs.at(ifile)->SetName(ss2.str().c_str());
		vhv2Obs.at(ifile)->SetTitle("v_{obs}");
		vhv2Obs.at(ifile)->SetYTitle("v_{obs}");
		vhv2Obs.at(ifile)->SetXTitle(xtitle.c_str());
		stringstream ss3;
		ss3 << "v2_" << vFileName.at(ifile);
		vhv2.push_back((TH1D*)vhv2Obs.at(ifile)->Clone(ss3.str().c_str()));
		vhv2.at(ifile)->Divide(vhr2.at(ifile));
		vhv2.at(ifile)->SetTitle("v = v_{obs}/R");
		vhv2.at(ifile)->SetYTitle("v = v_{obs}/R");
		vhv2.at(ifile)->SetXTitle(xtitle.c_str());
	}

	// Setup histo colors, labels, legend, etc.
	Color_t col[] = {kBlack, kGray+3, kGray+2, kGray, kViolet-6, kBlue+2, kBlue, kCyan+2, kSpring-1, kSpring+9, kYellow+1, kOrange+2, kRed+1, kMagenta-6};
	TLegend *leg = new TLegend(0.35,0.11, 0.52,0.7);
	for(uint16_t ifile=0; ifile<vFileList.size(); ifile++){
		vhr2.at(ifile)->SetLineColor(col[ifile]);
		vhr2.at(ifile)->SetLineWidth(2);
		vhv2Obs.at(ifile)->SetLineColor(col[ifile]);
		vhv2Obs.at(ifile)->SetLineWidth(2);
		vhv2.at(ifile)->SetLineColor(col[ifile]);
		vhv2.at(ifile)->SetLineWidth(2);
		string centName[8] = {"0-5%","5-10%","10-20%","20-30%","30-40%","40-50%","50-60%","60-70%"};
		for(uint16_t ibin=1; ibin<=vhr2.at(ifile)->GetNbinsX(); ibin++){
			vhr2.at(ifile)->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
			vhv2Obs.at(ifile)->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
			vhv2.at(ifile)->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		}
		leg->AddEntry(vhr2.at(ifile),vhr2.at(ifile)->GetName(),"lep");
	}

	// Setup input values for referenece
	int nbins = vhr2.at(0)->GetNbinsX();
	TH1D *hv2in = new TH1D("hv2in","hv2in",nbins,vhr2.at(0)->GetBinLowEdge(1),vhr2.at(0)->GetBinLowEdge(nbins+1));
	TH1D *hnPartIn = new TH1D("hnPartIn","nParticles",nbins,vhr2.at(0)->GetBinLowEdge(1),vhr2.at(0)->GetBinLowEdge(nbins+1));
	hnPartIn->SetLineWidth(2);
	hv2in->SetMarkerColor(kGray+2);
	hv2in->SetMarkerSize(2);
	hv2in->SetMarkerStyle(33);
	for(uint16_t icent=0; icent<inp.vCentClass.size(); icent++){
		TInputCentralityClass* icc = inp.vCentClass.at(icent);
		hv2in->Fill(icent, icc->vv.at(2));
		hnPartIn->Fill(icent, icc->nPart/2);
	}

	// Draw
/*	TCanvas* cEpr = new TCanvas("cEpr","cEpr", 1600,980);
	cEpr->Divide(2,2);
	cEpr->cd(1);
		vhr2.at(0)->GetYaxis()->SetRangeUser(0, 1.1);
		vhr2.at(0)->SetLineWidth(3);
		vhr2.at(0)->Draw();
		for(uint16_t ifile=1; ifile<vFileList.size(); ifile++){
			vhr2.at(ifile)->Draw("same");
		}
		vhr2.at(0)->Draw("same");
		leg->Draw();

	cEpr->cd(2);
		vhv2Obs.at(0)->GetYaxis()->SetRangeUser(0, 0.12);
		vhv2Obs.at(0)->SetLineWidth(3);
		vhv2Obs.at(0)->Draw();
		for(uint16_t ifile=1; ifile<vFileList.size(); ifile++){
			vhv2Obs.at(ifile)->Draw("same");
		}
		vhv2Obs.at(0)->Draw("same");
	cEpr->cd(3);
		vhv2.at(0)->GetYaxis()->SetRangeUser(0, 0.12);
		vhv2.at(0)->SetLineWidth(3);
		vhv2.at(0)->Draw();
		for(uint16_t ifile=1; ifile<vFileList.size(); ifile++){
			vhv2.at(ifile)->Draw("same");
		}
		vhv2.at(0)->Draw("same");
		hv2in->Draw("P same");
	cEpr->cd(4);
		hnPartIn->Draw();
*/

	uint16_t nGeometries = vFileList.size();
	TH1D* hGeomR2 = new TH1D("hGeomR2", "hGeomR2", nGeometries, 0, nGeometries);
	TH1D* hGeomV2Obs = new TH1D("hGeomV2Obs", "hGeomV2Obs", nGeometries, 0, nGeometries);
	TH1D* hGeomV2 = new TH1D("hGeomV2", "hGeomV2", nGeometries, 0, nGeometries);
	TF1* fInputV2 = new TF1("fInputV1","[0]",0,100);
	hGeomR2->SetXTitle("Number of sectors");
	hGeomV2Obs->SetXTitle("Number of sectors");
	hGeomV2->SetXTitle("Number of sectors");

	const uint16_t centBin = 4; // 20-30%
	for(uint16_t igeom = 0; igeom<nGeometries; igeom++){
		hGeomR2->SetBinContent(igeom+1, vhr2.at(igeom)->GetBinContent(centBin));
		hGeomV2Obs->SetBinContent(igeom+1, vhv2Obs.at(igeom)->GetBinContent(centBin));
		hGeomV2->SetBinContent(igeom+1, vhv2.at(igeom)->GetBinContent(centBin));
		hGeomR2->SetBinError(igeom+1, vhr2.at(igeom)->GetBinError(centBin));
		hGeomV2Obs->SetBinError(igeom+1, vhv2Obs.at(igeom)->GetBinError(centBin));
		hGeomV2->SetBinError(igeom+1, vhv2.at(igeom)->GetBinError(centBin));
		stringstream ssbinlabel;
		if(igeom == 0){
			ssbinlabel << igeom;
		}
		else{
			ssbinlabel << igeom+3;
		}
		hGeomR2->GetXaxis()->SetBinLabel(igeom+1, ssbinlabel.str().c_str());
		hGeomV2Obs->GetXaxis()->SetBinLabel(igeom+1, ssbinlabel.str().c_str());
		hGeomV2->GetXaxis()->SetBinLabel(igeom+1, ssbinlabel.str().c_str());
		fInputV2->SetParameter(0, hv2in->GetBinContent(centBin));
	}

	TCanvas* cGeom = new TCanvas("cGeom","cGeom", 1700,980);
	cGeom->Divide(2);
	cGeom->cd(1);
		hGeomR2->SetTitle("R_{2}");
		hGeomR2->GetYaxis()->SetRangeUser(0, 1.0);
		hGeomR2->SetLineWidth(2);
		hGeomR2->Draw();
	cGeom->cd(2);
		hGeomV2Obs->SetTitle("v_{2 obs} (blue), v_{2} (red)");
		hGeomV2Obs->GetYaxis()->SetRangeUser(0, 0.12);
		hGeomV2Obs->SetLineWidth(2);
		hGeomV2Obs->Draw();
		hGeomV2->SetLineColor(kRed);
		hGeomV2->SetLineWidth(2);
		hGeomV2->Draw("same");
		fInputV2->SetLineWidth(2);
		fInputV2->SetLineColor(kBlack);
		fInputV2->SetLineStyle(2);
		fInputV2->Draw("same");
}

void SetPadMargins(){
	gPad->SetRightMargin(0.03);
	gPad->SetLeftMargin(0.08);
	gPad->SetTopMargin(0.09);
	gPad->SetBottomMargin(0.08);
}

void CompareOthers(TInputParams &inp){
	gStyle->SetOptStat(0);
	gStyle->SetTitleFontSize(0.08);

	const uint16_t nFiles=9;
	string fileName[nFiles];
	fileName[0] = "outputs/V0A-ideal.root";
	fileName[1] = "outputs/V0C-ideal.root";
	fileName[2] = "outputs/V0A8.root";
	fileName[3] = "outputs/V0C8.root";
	fileName[4] = "outputs/V0A8-eff90.root";
	fileName[5] = "outputs/V0C8-eff90.root";
	fileName[6] = "other/Alexandru_v2_res_V0AC.root";
	fileName[7] = "other/Jasper-results-ideal.root";
	fileName[8] = "other/Jasper-results-8.root";

	TFile* f[nFiles];
	for(uint16_t ifile=0; ifile<nFiles; ifile++){
		f[ifile] = new TFile(fileName[ifile].c_str(),"READ");
		if(f[ifile] == NULL || !f[ifile]->IsOpen()){
			cout << "ComparePresent(): Error reading file: " << fileName[ifile] << endl;
			return;
		}
	}

	TH1D* v0Aideal = (TH1D*)(f[0]->Get("hR2"));
	TH1D* v0Cideal = (TH1D*)f[1]->Get("hR2");
	TH1D* v0A8 = (TH1D*)(f[2]->Get("hR2"));
	TH1D* v0C8 = (TH1D*)f[3]->Get("hR2");
	TH1D* v0A8eff90 = (TH1D*)(f[4]->Get("hR2"));
	TH1D* v0C8eff90 = (TH1D*)f[5]->Get("hR2");
	TH1D* v0Aalex = (TH1D*)f[6]->Get("hResV0A");
	TH1D* v0Calex = (TH1D*)f[6]->Get("hResV0C");
	TGraphErrors* v0A8jasp = (TGraphErrors*)f[7]->Get("gr_R2_V0A");
	TGraphErrors* v0C8jasp = (TGraphErrors*)f[7]->Get("gr_R2_V0C");
	TGraphErrors* v0Aidealjasp = (TGraphErrors*)f[8]->Get("gr_R2_V0A");
	TGraphErrors* v0Cidealjasp = (TGraphErrors*)f[8]->Get("gr_R2_V0C");

	const uint16_t nGraphs = 12;
	TObject* obj[nGraphs] = {v0Aideal, v0Cideal, v0A8, v0C8, v0A8eff90, v0C8eff90, v0A8jasp, v0C8jasp, v0Aidealjasp, v0Cidealjasp, v0Aalex, v0Calex};
	for(uint16_t igr=0; igr<nGraphs; igr++){
		if(obj[igr] == NULL){
			cout << "ComparePresent(): Error reading histo " << igr << " from file." << endl;
			return;
		}
	}

	// Convert all plots to graphs
	const float convBinToCent[11] = {2.5, 7.5, 15, 25, 35, 45, 55, 65, 75, 85, 95};
	const float convBinToErr[11] = {2.5, 2.5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	const float x0=0, x1=70, y0=0.3, y1=1;
	TGraphErrors* gr[nGraphs];
//	Color_t col[nGraphs] = {kRed, kRed, kPink+8, kPink+8, kGreen+3, kGreen+3, kBlue, kBlue, kCyan+2, kCyan+2, kBlack, kBlack};
	Color_t col[nGraphs] = {kRed, kRed, kRed, kRed, kPink+5, kPink+5, kBlue, kBlue, kBlue, kBlue, kBlack, kBlack};
	float linewidth[nGraphs] = {2.5, 2.5, 1.5, 1.5, 3.5, 3.5, 3.5, 3.5, 2.5, 2.5};
	for(uint16_t igr=0; igr<nGraphs; igr++){
		gr[igr] = new TGraphErrors(8);
		if(igr<6){ // my histos
			TH1D* h = (TH1D*)obj[igr];
			for(uint16_t ibin=1; ibin<=h->GetNbinsX(); ibin++){
				gr[igr]->SetPoint(ibin-1, convBinToCent[ibin-1], h->GetBinContent(ibin));
				gr[igr]->SetPointError(ibin-1, convBinToErr[ibin-1], h->GetBinError(ibin));
			}
		}
		else if(igr<10){ // Jasper's
			gr[igr] = (TGraphErrors*)obj[igr];
			for(uint16_t i=0; i<gr[igr]->GetN(); i++){
				gr[igr]->SetPointError(i, convBinToErr[i], gr[igr]->GetErrorY(i));
			}
		}
		else{ // Alexandru's
			TH1D* h = (TH1D*)obj[igr];
			for(uint16_t ibin=1; ibin<=h->GetNbinsX(); ibin++){
				gr[igr]->SetPoint(ibin-1, convBinToCent[ibin-1], h->GetBinContent(ibin));
				gr[igr]->SetPointError(ibin-1, convBinToErr[ibin-1], h->GetBinError(ibin));
			}
		}
		gr[igr]->SetLineColor(col[igr]);
		//gr[igr]->SetLineWidth(linewidth[igr]);
		gr[igr]->SetLineWidth(2.5);
		gr[igr]->GetXaxis()->SetRangeUser(x0, x1);
		gr[igr]->GetYaxis()->SetRangeUser(y0, y1);
		gr[igr]->GetXaxis()->SetLabelSize(0.07);
		gr[igr]->GetYaxis()->SetLabelSize(0.07);
	}

	TLegend* legA = new TLegend(0.22, 0.12, 0.64, 0.38);
	TLegend* legC = new TLegend(0.22, 0.12, 0.64, 0.38);
	TLegend* legA8 = new TLegend(0.22, 0.12, 0.64, 0.38);
	TLegend* legC8 = new TLegend(0.22, 0.12, 0.64, 0.38);
	string legLabel[nGraphs] = {"V0A-ideal-Maciej",   "V0C-ideal-Maciej",   "V0A-8-Maciej",     "V0C-8-Maciej",
								"V0A-8-eff90-Maciej", "V0C-8-eff90-Maciej", "V0A-ideal-Jasper", "V0C-ideal-Jasper",
								"V0A-8-Jasper",       "V0C-8-Jasper",       "V0A-data-Alex",    "V0C-data-Alex"};
	TCanvas *c = new TCanvas("cCmpIdeal","cCmpIdeal",1800,850);
	c->Divide(2);
	c->cd(1);
		gr[0]->SetTitle("R_{2}, A-side");
		gr[0]->DrawClone("ap");
		legA->AddEntry(gr[0], legLabel[0].c_str(), "le");
		gr[6]->DrawClone("p same");
		legA->AddEntry(gr[6], legLabel[6].c_str(), "le");
		gr[10]->DrawClone("p same");
		legA->AddEntry(gr[10], legLabel[10].c_str(), "le");
		legA->DrawClone();
	c->cd(2);
		gr[1]->SetTitle("R_{2}, C-side");
		gr[1]->DrawClone("ap");
		legC->AddEntry(gr[1], legLabel[1].c_str(), "le");
		gr[7]->DrawClone("p same");
		legC->AddEntry(gr[7], legLabel[7].c_str(), "le");
		gr[11]->DrawClone("p same");
		legC->AddEntry(gr[11], legLabel[11].c_str(), "le");
		legC->DrawClone();

	TCanvas *c8 = new TCanvas("cCmp8","cCmp8",1800,850);
	c8->Divide(2);
	c8->cd(1);
		gr[2]->SetTitle("R_{2}, A-side");
		gr[2]->DrawClone("ap");
		legA8->AddEntry(gr[2], legLabel[2].c_str(), "le");
		gr[4]->DrawClone("p same");
		legA8->AddEntry(gr[4], legLabel[4].c_str(), "le");
		gr[8]->DrawClone("p same");
		legA8->AddEntry(gr[8], legLabel[8].c_str(), "le");
		gr[10]->DrawClone("p same");
		legA8->AddEntry(gr[10], legLabel[10].c_str(), "le");
		legA8->DrawClone();
	c8->cd(2);
		gr[3]->SetTitle("R_{2}, C-side");
		gr[3]->DrawClone("ap");
		legC8->AddEntry(gr[3], legLabel[3].c_str(), "le");
		gr[5]->DrawClone("p same");
		legC8->AddEntry(gr[5], legLabel[5].c_str(), "le");
		gr[9]->DrawClone("p same");
		legC8->AddEntry(gr[9], legLabel[9].c_str(), "le");
		gr[11]->DrawClone("p same");
		legC8->AddEntry(gr[11], legLabel[11].c_str(), "le");
		legC8->DrawClone();

	TLegend* legAC = new TLegend(0.25, 0.13, 0.6, 0.35);
	TCanvas *c1 = new TCanvas("cCmpSamePerson","cCmpSamePerson",1830,950);
	c1->Divide(3,2);
	c1->cd(1);
		SetPadMargins();
		gr[0]->SetTitle("Maciej-ideal");
		gr[0]->SetLineColor(kBlack);
		gr[0]->Draw("ap");
		gr[1]->SetLineColor(kBlue);
		gr[1]->Draw("p same");
		legAC->AddEntry(gr[0], "A-side", "le");
		legAC->AddEntry(gr[1], "C-side", "le");
		legAC->Draw();
	c1->cd(2);
		SetPadMargins();
		gr[2]->SetTitle("Maciej-8");
		gr[2]->SetLineColor(kBlack);
		gr[2]->Draw("ap");
		gr[3]->SetLineColor(kBlue);
		gr[3]->Draw("p same");
		legAC->Draw();
	c1->cd(3);
		SetPadMargins();
		gr[4]->SetTitle("Maciej-8-eff90");
		gr[4]->SetLineColor(kBlack);
		gr[4]->Draw("ap");
		gr[5]->SetLineColor(kBlue);
		gr[5]->Draw("p same");
		legAC->Draw();
	c1->cd(4);
		SetPadMargins();
		gr[6]->SetTitle("Jasper-ideal");
		gr[6]->SetLineColor(kBlack);
		gr[6]->Draw("ap");
		gr[7]->SetLineColor(kBlue);
		gr[7]->Draw("p same");
		legAC->Draw();
	c1->cd(5);
		SetPadMargins();
		gr[8]->SetTitle("Jasper-8");
		gr[8]->SetLineColor(kBlack);
		gr[8]->Draw("ap");
		gr[9]->SetLineColor(kBlue);
		gr[9]->Draw("p same");
		legAC->Draw();
	c1->cd(6);
		SetPadMargins();
		gr[10]->SetTitle("Alex-data");
		gr[10]->SetLineColor(kBlack);
		gr[10]->Draw("ap");
		gr[11]->SetLineColor(kBlue);
		gr[11]->Draw("p same");
		legAC->Draw();

}

void RunToyMC(TInputParams &inp, uint16_t nSectors, const string &cliModeName, const string &dirPath){
	// Init histos
	uint16_t nBins = inp.vCentClass.size();
	//uint16_t bin0 = inp.vCentClass.at(0)->nPart-0.5*(inp.vCentClass.at(1)->nPart-inp.vCentClass.at(0)->nPart);
	//uint16_t bin1 = inp.vCentClass.size()*(inp.vCentClass.at(1)->nPart-inp.vCentClass.at(0)->nPart);
	uint16_t bin0 = 0;
	uint16_t bin1 = 8;
	TH1D* hv2 = new TH1D("hv2","hv2", nBins, bin0, bin1);
	TH1D* hv3 = new TH1D("hv3","hv3", nBins, bin0, bin1);
	hv2->SetXTitle("Centrality");
	hv2->SetYTitle("v");
	hv2->SetLineWidth(2);
	hv2->SetLineColor(kBlack);
	hv2->Sumw2();
	hv3->SetXTitle("Centrality");
	hv3->SetYTitle("v");
	hv3->SetLineWidth(2);
	hv3->SetLineColor(kBlue);
	hv3->Sumw2();
	TH1D* hR2 = new TH1D("hR2","hR2", nBins, bin0, bin1);
	TH1D* hR3 = new TH1D("hR3","hR3", nBins, bin0, bin1);
	hR2->SetXTitle("Centrality");
	hR2->SetYTitle("R");
	hR2->SetLineWidth(2);
	hR2->SetLineColor(kBlack);
	hR2->Sumw2();
	hR3->SetXTitle("Centrality");
	hR3->SetYTitle("R");
	hR3->SetLineColor(kBlue);
	hR3->SetLineWidth(2);
	hR3->Sumw2();

//	TCanvas* cvs = new TCanvas("cvs","cvs",1800,980);
//	cvs->Divide(2,2);


	// Initialize detector geometry
	TDetector *v0a = new TDetector("V0A_1x8");
	TDetector *v0c = new TDetector("V0C_1x8");

	// Main centrality and event loops
	TRandom3* rnd = new TRandom3(0);
	for(uint16_t icent=0; icent<inp.vCentClass.size(); icent++){
		cout << icent << endl;
		for(uint32_t evn=0; evn<inp.nEvents; evn++){
			TEvent* event = new TEvent(&inp, rnd);
			event->SetCentrality(icent);
			event->Simulate();
			event->ApplyGeometry(nSectors);
			event->Reconstruct();
			//uint32_t nPart = event->GetHit().size();
			//hR2->Fill(nPart, event->GetR(2));
			//hR3->Fill(nPart, event->GetR(3));
			//hv2->Fill(nPart, event->GetHarm().at(2)->vReco);
			//hv3->Fill(nPart, event->GetHarm().at(3)->vReco);

			hR2->Fill(icent, event->GetR(2));
			hR3->Fill(icent, event->GetR(3));
			hv2->Fill(icent, event->GetHarm().at(2)->vReco);
			hv3->Fill(icent, event->GetHarm().at(3)->vReco);

//			if(evn<4){
//				event->Draw(evn);
//			}
//			else{
				delete event;
//			}
		}
	}
	// Finish averaging (assumes the same number of events is simulated for all centralities)
	hR2->Scale(1./inp.nEvents);
	hR3->Scale(1./inp.nEvents);
	hv2->Scale(1./inp.nEvents);
	hv3->Scale(1./inp.nEvents);

	// Save to file
	stringstream ssfilename;
	ssfilename << dirPath << cliModeName << nSectors << ".root";
	TFile* f = new TFile(ssfilename.str().c_str(), "RECREATE");
	hR2->Write();
	hR3->Write();
	hv2->Write();
	hv3->Write();
	f->Close();
}

int main(int argc, char* argv[])
{
	const string dirPath = "outputs/";
	// Command line input handling
	cliParser cli;
	cli.AddPar("m", "Mode of operation", true);
	cli.GetModeList()->AddPar("ideal", "Run main MC simulation");
	cli.GetModeList()->AddPar("FIT", "Read sim output histos and overlay them");
	cli.GetModeList()->AddPar("overlay", "Read sim output histos and overlay them");
	cli.GetModeList()->AddPar("compareGeom", "Read sim output files from different geometries and plot them together");
	cli.GetModeList()->AddPar("compareOthers", "Compare present V0 geom from this toyMC with Alexandru");
	cli.AddPar("i", "Input directory");
	cli.AddPar("n", "Number of events to be simulated");
	cli.AddPar("s", "Number of sectors, required for '-m FIT'");
	if(cli.Parse(argc, argv)!=0){
		cli.PrintUsage(cout, "aliFitSimAnalysis");
		return -1;
	}

	if(cli.GetParString("m").compare("overlay")==0){
		TApplication theApp("App", &argc, argv);
			string inputDir = cli.GetParString("i");
			if(inputDir.size()<1){ // set default value
				inputDir = "inputs/";
			}
			TInputParams inp;
			InitMultiReal(inp, inputDir);
			InitVnsReal(inp, inputDir);
			Overlay(dirPath, inp);
			cout << "Done." << endl;
		theApp.Run();
	}
	else if(cli.GetParString("m").compare("compareGeom")==0){
		TApplication theApp("App", &argc, argv);
			string inputDir = cli.GetParString("i");
			if(inputDir.size()<1){ // set default value
				inputDir = "inputs/";
			}
			TInputParams inp;
			InitMultiReal(inp, inputDir);
			InitVnsReal(inp, inputDir);
			CompareGeom(inp);
			cout << "Done." << endl;
		theApp.Run();
	}
	else if(cli.GetParString("m").compare("compareOthers")==0){
		TApplication theApp("App", &argc, argv);
			string inputDir = cli.GetParString("i");
			if(inputDir.size()<1){ // set default value
				inputDir = "inputs/";
			}
			TInputParams inp;
			//InitMultiReal(inp, inputDir);
			//InitVnsReal(inp, inputDir);
			CompareOthers(inp);
			cout << "Done." << endl;
		theApp.Run();
	}
	else if(cli.GetParString("m").compare("ideal")==0 || cli.GetParString("m").compare("FIT")==0){
//		TApplication theApp("App", &argc, argv);
			uint16_t nSectors=0;
			if(cli.GetParString("m").compare("ideal")==0){
				nSectors = 0;
			}
			else if(cli.GetParString("m").compare("FIT")==0){
				if(cli.GetParString("s").size() < 1){
					cout << "<E> Number of sectors not provided for FIT geometry." << endl;
					return -1;
				}
				nSectors = cli.GetParInt("s");
			}

			// Init input parameters from CLI
			TInputParams inp;
			if(cli.GetParString("n").size()>0){
				inp.nEvents = cli.GetParInt("n");
			}
			else{
				inp.nEvents = 1e3;
			}
			cout << "<I> NEvents = " << inp.nEvents << endl;

			string inputDir = cli.GetParString("i");
			if(inputDir.size()<1){ // set default value
				inputDir = "inputs/";
			}

//			InitMultiTest(inp, inputDir);
			InitMultiReal(inp, inputDir);
//			InitVnsTest(inp, inputDir);
			InitVnsReal(inp, inputDir);
			inp.Print();
			RunToyMC(inp, nSectors, cli.GetParString("m"), dirPath);
			cout << "Done." << endl;
//		theApp.Run();
	}
	return 0;
}
