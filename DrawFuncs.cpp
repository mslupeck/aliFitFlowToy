/*
 * DrawFuncs.cpp
 *
 *  Created on: Feb 28, 2018
 *      Author: mss
 */

#include "DrawFuncs.h"

DrawFuncs::DrawFuncs() {}

void DrawFuncs::FileContents(TInputParams &inp){
	TFile* f = new TFile("outputs/V0A_1x8-V0C_1x8-TPC.root","READ");

	vector<TH1D*> vNch;
	vector<TH1D*> vR2Sim;
	vector<TH1D*> vR2Exp;
	const uint16_t nObjPerDet = 7;
	for(uint16_t i=0; i<f->GetListOfKeys()->GetSize(); i++){
		if(i<3*nObjPerDet){
			uint16_t idet = i%nObjPerDet;
			if(idet == 0){
				vNch.push_back((TH1D*)(f->GetListOfKeys()->At(i)));
			}
			else if(idet == 3){
				vR2Sim.push_back((TH1D*)(f->GetListOfKeys()->At(i)));
			}
			else if(idet == 4){
				vR2Exp.push_back((TH1D*)(f->GetListOfKeys()->At(i)));
			}
		}
	}


	string drawOpt[3] = {"","same","same"};
	Color_t col[3] = {kBlack, kRed, kBlue};

	TCanvas *c = new TCanvas("c", "c", 1800,850);
//	c->Divide(3);
	c->cd();
	for(uint16_t i=0; i<vNch.size(); i++){
		cout << vNch.at(i)->GetName() << endl;
		if(i==0){
			vNch.at(i)->Draw("");
		}
		else{
			vNch.at(i)->Draw("same");
		}
		vNch.at(i)->SetLineColor(kRed);
//		vNch.at(i)->SetLineWidth(2);
	}
/*	c->cd(2);
	for(uint16_t i=0; i<vR2Sim.size(); i++){
		vR2Sim.at(i)->Draw(vDrawOpt.at(i).c_str());
	}
	c->cd(3);
	for(uint16_t i=0; i<vR2Exp.size(); i++){
		vR2Exp.at(i)->SetLineColor(col[i]);
	}
*/
}
/*void DrawFuncs::Overlay(string inpath, TInputParams &inp){
	gStyle->SetOptStat(0);

	TFile* fra = new TFile("outputs/V0C_1x8-V0A_1x8_eff100.root","READ");
	TFile* frc = new TFile("outputs/V0A_1x8-V0C_1x8_eff100.root","READ");

	TH1D *hR2a = (TH1D*)fra->Get("hR2");
	TH1D *hR3a = (TH1D*)fra->Get("hR3");
	TH1D *hv2c = (TH1D*)fra->Get("hv2");
	TH1D *hv3c = (TH1D*)fra->Get("hv3");
	TH1D *hR2c = (TH1D*)frc->Get("hR2");
	TH1D *hR3c = (TH1D*)frc->Get("hR3");
	TH1D *hv2a = (TH1D*)frc->Get("hv2");
	TH1D *hv3a = (TH1D*)frc->Get("hv3");
	vector<TH1D*> vhNch;
	TH1D *hNchA0= (TH1D*)fra->Get("hNch0");
	TH1D *hNchC0= (TH1D*)fra->Get("hNch1");
	TH1D *hNchA1= (TH1D*)frc->Get("hNch1");
	TH1D *hNchC1= (TH1D*)frc->Get("hNch0");
	vhNch.push_back(hNchA0);
	vhNch.push_back(hNchC0);
	vhNch.push_back(hNchA1);
	vhNch.push_back(hNchC1);

	hR2a->SetLineColor(kRed);
	hv2a->SetLineColor(kRed);
	hR2c->SetLineColor(kBlue);
	hv2c->SetLineColor(kBlue);
	Color_t col[10]={kRed,kBlue,kRed,kBlue};
	for(uint16_t i=0; i<vhNch.size(); i++){
		vhNch.at(i)->SetLineColor(col[i]);
	}

	// Setup input values for referenece
	int nbins = hR2a->GetNbinsX();
	TH1D *hv2in = new TH1D("hv2in","hv2in",nbins,hR2a->GetBinLowEdge(1),hR2a->GetBinLowEdge(nbins+1));
	TH1D *hv3in = new TH1D("hv3in","hv3in",nbins,hR3a->GetBinLowEdge(1),hR3a->GetBinLowEdge(nbins+1));
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
	}

	// Set bin labels
	string centName[8] = {"0-5%","5-10%","10-20%","20-30%","30-40%","40-50%","50-60%","60-70%"};
	for(uint16_t ibin=1; ibin<=hR2a->GetNbinsX(); ibin++){
		hR2a->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hR3a->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hv2a->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hv3a->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hv2in->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		hv3in->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		for(uint16_t i=0; i<vhNch.size(); i++){
			vhNch.at(i)->GetXaxis()->SetBinLabel(ibin, centName[ibin-1].c_str());
		}
	}

	TLegend *leg = new TLegend(0.35,0.12, 0.57,0.4);
	leg->AddEntry(hR2a,"R_{2} V0A","lep");
//	leg->AddEntry(hR3ideal,"R_{3} V0A","lep");
	leg->AddEntry(hR2c,"R_{2} V0C","lep");
//	leg->AddEntry(hR3fit8,"R_{3} V0C","lep");

	TCanvas* cEpr = new TCanvas("cEpr","cEpr",1600,980);
	cEpr->Divide(2,2);
	cEpr->cd(1);
		hR2a->GetYaxis()->SetRangeUser(0,1.1);
		hR2a->SetTitle("R");
		hR2a->DrawCopy();
//		hR3ideal->DrawCopy("same");
		hR2c->DrawCopy("same");
//		hR3fit8->DrawCopy("same");
		leg->Draw();

	cEpr->cd(2);
		//hR2->GetYaxis()->SetRangeUser(0,1.1);
		hv2a->GetYaxis()->SetRangeUser(0,0.12);
		hv2a->SetTitle("v_{obs}");
		hv2a->DrawCopy();
//		hv3ideal->DrawCopy("same");
		hv2c->DrawCopy("same");
//		hv3fit8->DrawCopy("same");
	cEpr->cd(3);
		hv2a->Divide(hR2a);
		hv3a->Divide(hR3a);
		hv2c->Divide(hR2c);
		hv3c->Divide(hR3c);
		hv2a->SetYTitle("v = v_{obs}/R");
		hv2a->SetTitle("v = v_{obs}/R");
		hv2a->Draw();
//		hv3a->Draw("same");
		hv2c->Draw("same");
//		hv3c->Draw("same");
		hv2in->Draw("P same");
//		hv3in->Draw("P same");
	cEpr->cd(4);
		vhNch.at(0)->Draw();
		for(uint16_t i=1; i<vhNch.size(); i++){
			vhNch.at(i)->Draw("same");
		}

	for(uint16_t ibin=1; ibin<=hv2a->GetNbinsX(); ibin++){
		cout << ibin << " " << hv2a->GetBinContent(ibin) << "\t" << hv2c->GetBinContent(ibin) << endl;
	}
}

void DrawFuncs::CompareGeom(TInputParams &inp){
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
//		hnPartIn->Fill(icent, icc->nPart/2); // TODO: fix if needed
	}


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

void DrawFuncs::SetPadMargins(){
	gPad->SetRightMargin(0.03);
	gPad->SetLeftMargin(0.08);
	gPad->SetTopMargin(0.09);
	gPad->SetBottomMargin(0.08);
}

void DrawFuncs::CompareOthers(TInputParams &inp){
	gStyle->SetOptStat(0);
	gStyle->SetTitleFontSize(0.08);

	const uint16_t nFiles=7;
	string fileName[nFiles];
	fileName[0] = "outputs/V0C_1x8-V0A_1x8_eff90.root";
	fileName[1] = "outputs/V0A_1x8-V0C_1x8_eff90.root";
	fileName[2] = "outputs/V0C_1x8-V0A_1x8_eff100.root";
	fileName[3] = "outputs/V0A_1x8-V0C_1x8_eff100.root";
	fileName[4] = "other/Alexandru_v2_res_V0AC.root";
	fileName[5] = "other/Jasper-results-ideal.root";
	fileName[6] = "other/Jasper-results-8.root";

	TFile* f[nFiles];
	for(uint16_t ifile=0; ifile<nFiles; ifile++){
		f[ifile] = new TFile(fileName[ifile].c_str(),"READ");
		if(f[ifile] == NULL || !f[ifile]->IsOpen()){
			cout << "ComparePresent(): Error reading file: " << fileName[ifile] << endl;
			return;
		}
	}

	TH1D* v0A8eff90 = (TH1D*)(f[0]->Get("hR2"));
	TH1D* v0C8eff90 = (TH1D*)(f[1]->Get("hR2"));
	TH1D* v0A8eff100 = (TH1D*)(f[2]->Get("hR2"));
	TH1D* v0C8eff100 = (TH1D*)(f[3]->Get("hR2"));
	TH1D* v0Aalex = (TH1D*)f[4]->Get("hResV0A");
	TH1D* v0Calex = (TH1D*)f[4]->Get("hResV0C");
	TGraphErrors* v0A8jasp = (TGraphErrors*)f[5]->Get("gr_R2_V0A");
	TGraphErrors* v0C8jasp = (TGraphErrors*)f[5]->Get("gr_R2_V0C");
	TGraphErrors* v0Aidealjasp = (TGraphErrors*)f[6]->Get("gr_R2_V0A");
	TGraphErrors* v0Cidealjasp = (TGraphErrors*)f[6]->Get("gr_R2_V0C");

	const uint16_t nGraphs = 10;
	TObject* obj[nGraphs] = {v0A8eff100, v0C8eff100, v0A8eff90, v0C8eff90, v0A8jasp, v0C8jasp, v0Aidealjasp, v0Cidealjasp, v0Aalex, v0Calex};
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
	Color_t col[nGraphs] = {kRed, kRed, kPink+5, kPink+5, kBlue, kBlue, kBlue-2, kBlue-2, kBlack, kBlack};
//	float linewidth[nGraphs] = {2.5, 2.5, 1.5, 1.5, 3.5, 3.5, 3.5, 3.5, 2.5, 2.5};
	for(uint16_t igr=0; igr<nGraphs; igr++){
		gr[igr] = new TGraphErrors(8);
		if(igr<4){ // my histos
			TH1D* h = (TH1D*)obj[igr];
			for(uint16_t ibin=1; ibin<=h->GetNbinsX(); ibin++){
				gr[igr]->SetPoint(ibin-1, convBinToCent[ibin-1], h->GetBinContent(ibin));
				gr[igr]->SetPointError(ibin-1, convBinToErr[ibin-1], h->GetBinError(ibin));
			}
		}
		else if(igr<8){ // Jasper's
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
	string legLabel[nGraphs] = {"V0A-8-eff100-Maciej", "V0C-8-eff100-Maciej", "V0A-8-eff90-Maciej", "V0C-8-eff90-Maciej",
			                    "V0A-ideal-Jasper", "V0C-ideal-Jasper", "V0A-8-Jasper", "V0C-8-Jasper",
								"V0A-data-Alex",    "V0C-data-Alex"};
	TCanvas *c = new TCanvas("cCmp","cCmp",1800,850);
	c->Divide(2);
	c->cd(1);
		gr[0]->SetTitle("R_{2}, A-side");
		gr[0]->DrawClone("ap");
		legA->AddEntry(gr[0], legLabel[0].c_str(), "le");
		//gr[2]->DrawClone("p same");
		//legA->AddEntry(gr[2], legLabel[2].c_str(), "le");
		gr[4]->DrawClone("p same");
		legA->AddEntry(gr[4], legLabel[4].c_str(), "le");
		gr[6]->DrawClone("p same");
		legA->AddEntry(gr[6], legLabel[6].c_str(), "le");
		gr[8]->DrawClone("p same");
		legA->AddEntry(gr[8], legLabel[8].c_str(), "le");
		legA->DrawClone();
	c->cd(2);
		gr[1]->SetTitle("R_{2}, C-side");
		gr[1]->DrawClone("ap");
		legC->AddEntry(gr[1], legLabel[1].c_str(), "le");
		//gr[3]->DrawClone("p same");
		//legC->AddEntry(gr[3], legLabel[3].c_str(), "le");
		gr[5]->DrawClone("p same");
		legC->AddEntry(gr[5], legLabel[5].c_str(), "le");
		gr[7]->DrawClone("p same");
		legC->AddEntry(gr[7], legLabel[7].c_str(), "le");
		gr[9]->DrawClone("p same");
		legC->AddEntry(gr[9], legLabel[9].c_str(), "le");
		legC->DrawClone();

	TLegend* legAC = new TLegend(0.25, 0.13, 0.6, 0.35);
	TCanvas *c1 = new TCanvas("cCmpSamePerson","cCmpSamePerson",1830,950);
	c1->Divide(3,2);
	c1->cd(1);
		SetPadMargins();
		gr[0]->SetTitle("Maciej-8-eff100");
		gr[0]->SetLineColor(kBlack);
		gr[0]->Draw("ap");
		gr[1]->SetLineColor(kBlue);
		gr[1]->Draw("p same");
		legAC->AddEntry(gr[0], "A-side", "le");
		legAC->AddEntry(gr[1], "C-side", "le");
		legAC->Draw();
	c1->cd(2);
		SetPadMargins();
		gr[2]->SetTitle("Maciej-8-eff90");
		gr[2]->SetLineColor(kBlack);
		gr[2]->Draw("ap");
		gr[3]->SetLineColor(kBlue);
		gr[3]->Draw("p same");
		legAC->Draw();
	c1->cd(3);
		SetPadMargins();
		gr[4]->SetTitle("Jasper-ideal");
		gr[4]->SetLineColor(kBlack);
		gr[4]->Draw("ap");
		gr[5]->SetLineColor(kBlue);
		gr[5]->Draw("p same");
		legAC->Draw();
	c1->cd(4);
		SetPadMargins();
		gr[6]->SetTitle("Jasper-8");
		gr[6]->SetLineColor(kBlack);
		gr[6]->Draw("ap");
		gr[7]->SetLineColor(kBlue);
		gr[7]->Draw("p same");
		legAC->Draw();
	c1->cd(5);
		SetPadMargins();
		gr[8]->SetTitle("Alex-data");
		gr[8]->SetLineColor(kBlack);
		gr[8]->Draw("ap");
		gr[9]->SetLineColor(kBlue);
		gr[9]->Draw("p same");
		legAC->Draw();

}

void DrawFuncs::InvertedR(string inpath, TInputParams& inp){

	const uint16_t nFiles=3;
	string fileName[nFiles];
	fileName[0] = "outputs/V0C_1x8-V0A_1x8_eff100.root";
	fileName[1] = "outputs/V0A_1x8-V0C_1x8_eff100.root";
	fileName[2] = "other/Alexandru_v2_res_V0AC.root";

	TFile* f[nFiles];
	for(uint16_t ifile=0; ifile<nFiles; ifile++){
		f[ifile] = new TFile(fileName[ifile].c_str(),"READ");
		if(f[ifile] == NULL || !f[ifile]->IsOpen()){
			cout << "ComparePresent(): Error reading file: " << fileName[ifile] << endl;
			return;
		}
	}

	TH1D* v2a8 = (TH1D*)(f[0]->Get("hv2"));
	TH1D* v2c8 = (TH1D*)(f[1]->Get("hv2"));
	TH1D* r2a8 = (TH1D*)(f[1]->Get("hR2"));
	TH1D* r2c8 = (TH1D*)(f[0]->Get("hR2"));
	TH1D* r2aAlex = (TH1D*)f[2]->Get("hResV0A");
	TH1D* r2cAlex = (TH1D*)f[2]->Get("hResV0C");

	// Setup input (simulated) values
	int nbins = v2a8->GetNbinsX();
	TH1D *v2in = new TH1D("hv2in", "hv2in", nbins, v2a8->GetBinLowEdge(1), v2a8->GetBinLowEdge(nbins+1));
	v2in->SetMarkerColor(kGray+2);
	v2in->SetMarkerSize(2);
	v2in->SetMarkerStyle(33);
	for(uint16_t icent=0; icent<inp.vCentClass.size(); icent++){
		TInputCentralityClass* icc = inp.vCentClass.at(icent);
		v2in->SetBinContent(icent+1, icc->vv.at(2));
		v2in->SetBinError(icent+1, 0);
	}

	const uint16_t nGraphs = 6;
	TObject* objIn[nGraphs] = {r2a8, r2c8, v2a8, v2a8, r2aAlex, r2cAlex};
	for(uint16_t igr=0; igr<nGraphs; igr++){
		if(objIn[igr] == NULL){
			cout << "ComparePresent(): Error reading histo " << igr << " from file." << endl;
			return;
		}
	}

	TH1D* r2aInv = (TH1D*)v2a8->Clone();
	TH1D* r2cInv = (TH1D*)v2c8->Clone();
	r2aInv->Sumw2();
	r2cInv->Sumw2();
	r2aInv->Divide(v2in);
	r2cInv->Divide(v2in);
	TObject* obj[nGraphs] = {r2a8, r2c8, r2aInv, r2cInv, r2aAlex, r2cAlex};

	// Convert all plots to graphs
	const float convBinToCent[11] = {2.5, 7.5, 15, 25, 35, 45, 55, 65, 75, 85, 95};
	const float convBinToErr[11] = {2.5, 2.5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
	const float x0=0, x1=70, y0=0.3, y1=1;
	TGraphErrors* gr[nGraphs];
//	Color_t col[nGraphs] = {kRed, kRed, kPink+8, kPink+8, kGreen+3, kGreen+3, kBlue, kBlue, kCyan+2, kCyan+2, kBlack, kBlack};
	Color_t col[nGraphs] = {kRed, kBlue, kRed-2, kBlue-2, kRed-7, kBlue-9};
//	float linewidth[nGraphs] = {2.5, 2.5, 1.5, 1.5, 3.5, 3.5, 3.5, 3.5, 2.5, 2.5};
	for(uint16_t igr=0; igr<nGraphs; igr++){
		gr[igr] = new TGraphErrors(8);
		if(igr<4){ // my histos
			TH1D* h = (TH1D*)obj[igr];
			for(uint16_t ibin=1; ibin<=h->GetNbinsX(); ibin++){
				gr[igr]->SetPoint(ibin-1, convBinToCent[ibin-1], h->GetBinContent(ibin));
				gr[igr]->SetPointError(ibin-1, convBinToErr[ibin-1], h->GetBinError(ibin));
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

	TCanvas *cInv = new TCanvas("cInv","cInv",1830,600);
	cInv->Divide(2);
	cInv->cd(1);
		gr[0]->SetTitle("R_{2} comparison - A-side");
		gr[0]->GetYaxis()->SetRangeUser(0, 1);
		gr[0]->Draw("ap");
		gr[2]->Draw("p sames");
		gr[4]->Draw("p sames");
		TLegend *legA = new TLegend(0.2,0.12, 0.6,0.36);
		legA->AddEntry(gr[0],"R_{2} V0A8-std","lep");
		legA->AddEntry(gr[2],"R_{2} V0A8-inv","lep");
		legA->AddEntry(gr[4],"R_{2} V0A-data-Alex","lep");
		legA->Draw();
		gPad->SetGridx();
		gPad->SetGridy();
	cInv->cd(2);
		gr[1]->SetTitle("R_{2} comparison - C-side");
		gr[1]->GetYaxis()->SetRangeUser(0, 1);
		gr[1]->Draw("ap");
		gr[3]->Draw("p sames");
		gr[5]->Draw("p sames");
		TLegend *legC = new TLegend(0.2,0.12, 0.6,0.36);
		legC->AddEntry(gr[1],"R_{2} V0C8-std","lep");
		legC->AddEntry(gr[3],"R_{2} V0C8-inv","lep");
		legC->AddEntry(gr[5],"R_{2} V0C-data-Alex","lep");
		legC->Draw();
		gPad->SetGridx();
		gPad->SetGridy();
}
*/
