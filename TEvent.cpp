/*
 * TEvent.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#include "TEvent.h"

double TEvent::Round8Sectors(double phi){
	phi *= TMath::RadToDeg();
	const int nAngleLimits = 16;
	const double angleLimits[nAngleLimits+1] = {-360,-315,-270,-225,-180,-135,-90,-45,0,45,90,135,180,225,270,315,360};
	if(phi < angleLimits[0]){
		cout << "<E> Round8Sectors(): Too small phi: " << phi << endl;
		return 0;
	}
	else if(phi >= angleLimits[nAngleLimits]){
		cout << "<E> Round8Sectors(): Too large phi: " << phi << endl;
		return 0;
	}

	for(int i=0; i<nAngleLimits; i++){
		if(phi>=angleLimits[i] && phi<angleLimits[i+1]){
			return 0.5*(angleLimits[i]+angleLimits[i+1])*TMath::DegToRad();
		}
	}
	cout << "<E> Round8Sectors(): Wrong angle: " << phi << endl;
	return -1;
}

double TEvent::Round16Sectors(double phi){
	phi *= TMath::RadToDeg();
	const int nAngleLimits = 32;
	double angleLimits[nAngleLimits+1];
	for(int i=0; i<=nAngleLimits; i++){
		angleLimits[i] = -360. + 22.5*i;
	}

	if(phi < angleLimits[0]){
		cout << "<E> Round8Sectors(): Too small phi: " << phi << endl;
		return 0;
	}
	else if(phi >= angleLimits[nAngleLimits]){
		cout << "<E> Round8Sectors(): Too large phi: " << phi << endl;
		return 0;
	}

	for(int i=0; i<nAngleLimits; i++){
		if(phi>=angleLimits[i] && phi<angleLimits[i+1]){
			return 0.5*(angleLimits[i]+angleLimits[i+1])*TMath::DegToRad();
		}
	}
	cout << "<E> Round8Sectors(): Wrong angle: " << phi << endl;
	return -1;
}

double TEvent::RoundToSectors(int nSectors, double phi){
	phi *= TMath::RadToDeg();
	vector<double> vAngleLimits;
	for(int i=0; i<=2*nSectors; i++){
		vAngleLimits.push_back( -360.0 * ( 1 - ((double)i/nSectors) ) );
	}

	if(phi < vAngleLimits.at(0)){
		cout << "<E> Round8Sectors(): Too small phi: " << phi << endl;
		return 0;
	}
	else if(phi >= vAngleLimits.at(vAngleLimits.size()-1)){
		cout << "<E> Round8Sectors(): Too large phi: " << phi << endl;
		return 0;
	}

	for(int i=0; i<vAngleLimits.size()-1; i++){
		if(phi>=vAngleLimits.at(i) && phi<vAngleLimits.at(i+1)){
			return 0.5*(vAngleLimits.at(i)+vAngleLimits.at(i+1))*TMath::DegToRad();
		}
	}
	cout << "<E> Round8Sectors(): Wrong angle: " << phi << endl;
	return -1;
}

TEvent::TEvent(TInputParams *par, TRandom *rnd) {
	this->rnd = rnd;
	this->par = par;
	iCentrality = -1;
	fNSim = NULL;
	fNReco = NULL;
	InitHarmonics();
}

TEvent::~TEvent() {
	DestroyHarmonics();
	if(fNSim != NULL){
		delete fNSim;
	}
	if(fNReco != NULL){
		delete fNReco;
	}
	for(uint32_t ihit=0; ihit<vHit.size(); ihit++){
		delete vHit.at(ihit);
	}
}

void TEvent::InitHarmonics(){
	for(uint16_t iharm=0; iharm<par->vCentClass.at(0)->vv.size(); iharm++){
		vHarm.push_back(new THarm());
	}
}

void TEvent::DestroyHarmonics(){
	for(uint16_t iharm=0; iharm<vHarm.size(); iharm++){
		delete vHarm.at(iharm);
	}
}

void TEvent::SetCentrality(uint16_t icent){
	iCentrality = icent;
	if(icent < par->vCentClass.size()){
		TInputCentralityClass *icc = par->vCentClass.at(icent);
		for(uint16_t iharm=0; iharm<icc->vv.size(); iharm++){
			vHarm.at(iharm)->vSim = icc->vv.at(iharm);
			vHarm.at(iharm)->dvSim = icc->vdv.at(iharm);
		}
	}
	else{
		cout << "<E>TEvent::SetCentrality()::Index too large: " << icent << " >= " << par->vCentClass.size() << endl;
	}
}

double TEvent::GetR(uint16_t iharm) const {
	if(iharm < vHarm.size()){
		return vHarm.at(iharm)->r;
	}
	else{
		cout << "<E>TEvent::GetR()::Index too large: " << iharm << " >= " << vHarm.size() << endl;
		return 0;
	}
}

void TEvent::Simulate(){
	const uint16_t iharm0 = 2;
	TRandom3 rnd;
	rnd.SetSeed(0);
	// Build single-harmonic functions
	for(uint16_t iharm=iharm0; iharm<vHarm.size(); iharm++){
		THarm* h = vHarm.at(iharm);
		double psiRange = TMath::Pi()/iharm;
		h->psiSim = rnd.Uniform(-psiRange,psiRange);
		stringstream ssname;
		ssname << "fHarm" << iharm;
		h->fSim = new TF1(ssname.str().c_str(), "[0]*TMath::Cos([1]*(x-[2]))", -TMath::Pi(), TMath::Pi());
		h->fSim->SetParameter(0, rnd.Gaus(h->vSim, h->dvSim));
		h->fSim->SetParameter(1, iharm);
		h->fSim->SetParameter(2, h->psiSim);
	}
	// Build function fN composed of sum of single-harmonic functions
	stringstream ssfN;
	ssfN << "1+2*(";
	for(uint16_t iharm=iharm0; iharm<vHarm.size(); iharm++){
		ssfN << "fHarm" << iharm;
		if(iharm < vHarm.size()-1){
			ssfN << "+";
		}
	}
	ssfN << ")";
	fNSim = new TF1("fNSim",ssfN.str().c_str(), -TMath::Pi(), TMath::Pi());

	// Generate particles
	uint32_t nPart = par->vCentClass.at(iCentrality)->nPart;
	for(uint16_t ipart=0; ipart<nPart; ipart++){
		vHit.push_back(new THit(fNSim->GetRandom(-TMath::Pi(), TMath::Pi())));
	}
}

void TEvent::ApplyGeometry(uint16_t nSectors){
	for(uint16_t ipart=0; ipart<vHit.size(); ipart++){
		THit* hit = vHit.at(ipart);
		if(nSectors == 0){ // ideal
			hit->phiDet = hit->phiSim;
		}
		else if(nSectors > 0){ // FIT8
			hit->phiDet = RoundToSectors(nSectors, hit->phiSim);
		}
		else{
			cout << "<E>TEvent::ApplyGeometry(): Wrong number of nSectors = " << nSectors << endl;
			hit->phiDet = 0;
		}
	}
}

void TEvent::Reconstruct(){
	const float efficiency = 0.9;
	// Auto-correlations are removed by using half of the hits to reconstruct Q-vector, R and Psi_n
	// .. and another half is used to reconstruct Vn's

	// Calculate Q-vector
	uint32_t nPart = vHit.size();
	for(uint16_t ipart=0; ipart<nPart/2; ipart++){
		if(rnd->Uniform() < efficiency){
			THit* hit = vHit.at(ipart);
			for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
				THarm* h = vHarm.at(iharm);
				h->qx += TMath::Cos(hit->phiDet*iharm);
				h->qy += TMath::Sin(hit->phiDet*iharm);
			}
		}
	}
	// Reconstruct event plane and calculate it's resolution
	for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
		THarm* h = vHarm.at(iharm);
		h->psiReco = TMath::ATan2(h->qy, h->qx)/iharm;
		h->r = TMath::Cos((h->psiReco - h->psiSim) * iharm);
	}

	// Calculate vn_obs (sum across all particles)
	for(uint16_t ipart=nPart/2; ipart<nPart; ipart++){
		if(rnd->Uniform() < efficiency){
			THit* hit = vHit.at(ipart);
			for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
				THarm* h = vHarm.at(iharm);
				h->vReco += TMath::Cos((hit->phiDet - h->psiReco) * iharm);
			}
		}
	}
	// Finish calculating <vn_obs> (avg) and calculate resolution parameter and full event plane resolution
	for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
		THarm* h = vHarm.at(iharm);
		h->vReco /= nPart/2;
	}
}

void TEvent::Draw(uint16_t evn){
	gStyle->SetOptStat(0);
	gPad->GetCanvas()->cd((evn)+1);
	fNSim->SetLineColor(kBlack);
	fNSim->SetLineWidth(2.0);
	fNSim->GetYaxis()->SetRangeUser(-1.0, 3.0);
	fNSim->Draw("L");
	Color_t col[4] = {kYellow, kYellow, kRed, kBlue};
	for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
		THarm* h = vHarm.at(iharm);
		h->fSim->SetLineColor(col[iharm]);
		h->fSim->DrawCopy("L SAME");
	}

	for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
		THarm* h = vHarm.at(iharm);
		stringstream ssname;
		ssname << "fReco" << iharm;
		// TODO: This function is never deleted
		TF1* fReco = new TF1(ssname.str().c_str(), "[0]*TMath::Cos([1]*(x-[2]))", -TMath::Pi(), TMath::Pi());
		fReco->SetParameter(0, h->vReco);
		fReco->SetParameter(1, iharm);
		fReco->SetParameter(2, h->psiReco);
		fReco->SetLineColor(col[iharm]+2);
		fReco->SetLineWidth(2.0);
		fReco->Draw("L same");
	}

	stringstream ssfN;
	ssfN << "1+2*(";
	for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
		ssfN << "fReco" << iharm;
		if(iharm < vHarm.size()-1){
			ssfN << "+";
		}
	}
	ssfN << ")";
	fNReco = new TF1("fNReco",ssfN.str().c_str(), -TMath::Pi(), TMath::Pi());
	fNReco->SetLineColor(kGray);
	fNReco->SetLineWidth(2);
	fNReco->GetYaxis()->SetRangeUser(-0.5,2.0);
	fNReco->Draw("L same");

	stringstream ss;
	ss << "h" << evn << "_" << iCentrality;
	TH1D* h = new TH1D(ss.str().c_str(), ss.str().c_str(), 16, -TMath::Pi(), TMath::Pi());
	for(uint16_t ipart=0; ipart<vHit.size(); ipart++){
		h->Fill(vHit.at(ipart)->phiDet);
	}
	//gPad->GetCanvas()->cd((3*evn)+3);
	h->Sumw2();
	h->SetLineWidth(2);
	h->Scale(h->GetNbinsX()/h->Integral());
	h->GetYaxis()->SetRangeUser(0, 2*vHit.size()/16);
	h->Draw("same");
}

const vector<THit*>& TEvent::GetHit() const {
	return vHit;
}

const vector<THarm*>& TEvent::GetHarm() const {
	return vHarm;
}
