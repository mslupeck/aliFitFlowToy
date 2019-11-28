/*
 * TEvent.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#include "TEvent.h"

TEvent::TEvent(TInputParams *par, TRandom *rnd) {
	this->rnd = rnd;
	this->par = par;
	iCentrality = -1;
	fDnDPhiSim = NULL;
	fDnDPhiReco = NULL;
	InitHarmonics();
}

TEvent::~TEvent() {
	DestroyHarmonics();
	if(fDnDPhiSim != NULL){
		delete fDnDPhiSim;
	}
	if(fDnDPhiReco != NULL){
		delete fDnDPhiReco;
	}
	for(uint32_t ihit=0; ihit<vHit.size(); ihit++){
		delete vHit.at(ihit);
	}
}

void TEvent::InitHarmonics(){
	for(uint16_t iharm=0; iharm<par->vCentClass.at(0)->vv.size(); iharm++){
		vHarmSim.push_back(new THarm());
	}
}

void TEvent::DestroyHarmonics(){
	for(uint16_t iharm=0; iharm<vHarmSim.size(); iharm++){
		delete vHarmSim.at(iharm);
	}
}

void TEvent::SetCentrality(uint16_t icent){
	iCentrality = icent;
	if(icent < par->vCentClass.size()){
		TInputCentralityClass *icc = par->vCentClass.at(icent);
		for(uint16_t iharm=0; iharm<icc->vv.size(); iharm++){
			vHarmSim.at(iharm)->v = icc->vv.at(iharm);
			vHarmSim.at(iharm)->dv = icc->vdv.at(iharm);
		}
	}
	else{
		cout << "<E>TEvent::SetCentrality()::Index too large: " << icent << " >= " << par->vCentClass.size() << endl;
	}
}

//double TEvent::GetR(uint16_t iharm) const {
//	if(iharm < vHarm.size()){
//		return vHarm.at(iharm)->r;
//	}
//	else{
//		cout << "<E>TEvent::GetR()::Index too large: " << iharm << " >= " << vHarm.size() << endl;
//		return 0;
//	}
//}

void TEvent::Simulate(){
	// Build single-harmonic functions
	for(uint16_t iharm=par->iharm0; iharm<vHarmSim.size(); iharm++){
		THarm* hSim = vHarmSim.at(iharm);
		double psiRange = TMath::Pi()/iharm;
		hSim->psi = rnd->Uniform(-psiRange,psiRange);
		stringstream ssname;
		ssname << "fHarm" << iharm;
		hSim->f = new TF1(ssname.str().c_str(), "[0]*TMath::Cos([1]*(x-[2]))", -TMath::Pi(), TMath::Pi());
		hSim->f->SetParameter(0, rnd->Gaus(hSim->v, hSim->dv));
		hSim->f->SetParameter(1, iharm);
		hSim->f->SetParameter(2, hSim->psi);
	}
	// Build function fN composed of sum of single-harmonic functions
	stringstream ssfN;
	ssfN << "1+2*(";
	for(uint16_t iharm=par->iharm0; iharm<vHarmSim.size(); iharm++){
		ssfN << "fHarm" << iharm;
		if(iharm < vHarmSim.size()-1){
			ssfN << "+";
		}
	}
	ssfN << ")";
	fDnDPhiSim = new TF1("fDnDPhiSim", ssfN.str().c_str(), -TMath::Pi(), TMath::Pi());

	// Generate particles
	TF1* fDnDEtaSim = par->vCentClass.at(iCentrality)->fDnDEtaSim;
	float nPart = round(fDnDEtaSim->Integral(fDnDEtaSim->GetXmin(), fDnDEtaSim->GetXmax()));
	// TODO: nPart should be smeared according to a gaussian distr... with what sigma?
	for(uint32_t ipart=0; ipart<nPart; ipart++){
		float eta = fDnDEtaSim->GetRandom(fDnDEtaSim->GetXmin(), fDnDEtaSim->GetXmax());
		float phi = fDnDPhiSim->GetRandom(-TMath::Pi(), TMath::Pi());
		vHit.push_back(new THit(phi, eta));
	}
}

void TEvent::Transport(vector<TDetector*> &vDet){
	// Reset nCh registered by detectors
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		vDet.at(idet)->ResetAllCells();
	}

	// Assign number of hits to each detector cell
	uint32_t nPart = vHit.size();
	for(uint16_t ipart=0; ipart<nPart; ipart++){
		THit* h = vHit.at(ipart);
		for(uint16_t idet=0; idet<vDet.size(); idet++){
			TDetector* det = vDet.at(idet);
			for(uint16_t iring=0; iring<det->GetRings().size(); iring++){
				TRing* r = det->GetRings().at(iring);
				if((h->eta > r->GetEta0()) && (h->eta <= r->GetEta1())){ // check if eta of detector ring corresponds the eta of particle
					for(uint16_t icell=0; icell<r->GetCells().size(); icell++){
						TCell* c = r->GetCells().at(icell);
						if((h->phi > c->GetPhi0()) && (h->phi <= c->GetPhi1())){ // check if phi of cell corresponds to the phi of particle
							if(rnd->Uniform() < det->GetEff()){ // check efficiency
								c->IncNch();
							}
						}
					}
				}
			}
		}
	}
}

void TEvent::ReconstructR(vector<TDetector*> &vDet){
	//Calculate Q-vector for each detector separately
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		TDetector* det = vDet.at(idet);
		det->ResetHarm();
		for(uint16_t iring=0; iring<det->GetRings().size(); iring++){
			TRing* r = det->GetRings().at(iring);
			for(uint16_t icell=0; icell<r->GetCells().size(); icell++){
				TCell* c = r->GetCells().at(icell);
				float phiDet = 0.5 * (c->GetPhi0() + c->GetPhi1());
				for(uint16_t iharm=par->iharm0; iharm<det->GetHarmReco().size(); iharm++){
					double qx = TMath::Cos(phiDet*iharm) * c->GetNch();
					double qy = TMath::Sin(phiDet*iharm) * c->GetNch();
					det->AddQ(iharm, qx, qy);
				}
			}
		}

	}

	// Reconstruct event plane and calculate it's resolution
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		TDetector* det = vDet.at(idet);
		for(uint16_t iharm=par->iharm0; iharm<det->GetHarmReco().size(); iharm++){
			THarm* hReco = det->GetHarmReco().at(iharm);
			THarm* hSim = vHarmSim.at(iharm);
			det->SetPsiReco(iharm, TMath::ATan2(hReco->qy, hReco->qx)/iharm);
			det->SetRReco(iharm, TMath::Cos((hReco->psi - hSim->psi) * iharm));
		}
	}
}

void TEvent::ReconstructRSub3(vector<TDetector*> &vDet){
	//Calculate Q-vector for each detector separately
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		TDetector* det = vDet.at(idet);
		det->ResetHarm();
		for(uint16_t iring=0; iring<det->GetRings().size(); iring++){
			TRing* r = det->GetRings().at(iring);
			for(uint16_t icell=0; icell<r->GetCells().size(); icell++){
				TCell* c = r->GetCells().at(icell);
				float phiDet = 0.5 * (c->GetPhi0() + c->GetPhi1());
				for(uint16_t iharm=par->iharm0; iharm<det->GetHarmReco().size(); iharm++){
					double qx = TMath::Cos(phiDet*iharm) * c->GetNch();
					double qy = TMath::Sin(phiDet*iharm) * c->GetNch();
					det->AddQ(iharm, qx, qy);
				}
			}
		}
	}

	// Reconstruct event plane and calculate it's resolution
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		TDetector* det = vDet.at(idet);
		for(uint16_t iharm=par->iharm0; iharm<det->GetHarmReco().size(); iharm++){
			THarm* hReco = det->GetHarmReco().at(iharm);
			THarm* hSim = vHarmSim.at(iharm);
			det->SetPsiReco(iharm, TMath::ATan2(hReco->qy, hReco->qx)/iharm);
		}
	}

	for(uint16_t idet=0; idet<vDet.size(); idet++){
		uint16_t idetaux = (idet+1)%vDet.size();
		TDetector* det = vDet.at(idet);
		TDetector* detAux = vDet.at(idetaux);
		for(uint16_t iharm=par->iharm0; iharm<det->GetHarmReco().size(); iharm++){
			THarm* hReco1 = det   ->GetHarmReco().at(iharm);
			THarm* hReco2 = detAux->GetHarmReco().at(iharm);
			vDet.at(idet)->SetRReco(iharm, TMath::Cos((hReco1->psi - hReco2->psi) * iharm));
		}
	}
}

void TEvent::FillHistos(vector<TH1D*> &vh, vector<TH2D*> vh2, vector<TDetector*> &vDet){
	const uint16_t nHarmPars = 3; // 3 = 1 and 1 and 1 (Rsim, Rexp and vn)
	const uint16_t nIndepPars = 1; // 1 for nCh
	for(uint16_t idet=0; idet<vDet.size(); idet++){
		TDetector* det = vDet.at(idet);
		size_t harmRecoSize = det->GetHarmReco().size() - par->iharm0;
		uint16_t iTotNch = idet * (harmRecoSize * nHarmPars + nIndepPars);
		vh.at(iTotNch)->Fill(iCentrality, det->GetTotNch());
		for(uint16_t iharm=0; iharm<harmRecoSize; iharm++){
			THarm* hReco = det->GetHarmReco().at(iharm+par->iharm0);
			uint16_t iv = iTotNch + 1 + iharm;
			uint16_t ir = iv + harmRecoSize;
			vh.at(iv)->Fill(iCentrality, hReco->v);
			vh.at(ir)->Fill(iCentrality, hReco->r);
		}
		vh2.at(idet)->Fill(det->GetHarmReco().at(2)->psi, vHarmSim.at(2)->psi);
	}
}

/*void TEvent::Reconstruct_old(vector<TDetector*> vDet){
	// Auto-correlations are removed by using half of the hits to reconstruct Q-vector, R and Psi_n
	// .. and another half is used to reconstruct Vn's

	const uint16_t idet=0; // det=0 is used to get event plane and it's resolution
	//Calculate Q-vector
	for(uint16_t iring=0; iring<vDet.at(idet)->GetRings().size(); iring++){
		TRing* r = vDet.at(idet)->GetRings().at(iring);
		for(uint16_t icell=0; icell<r->GetCells().size(); icell++){
			TCell* c = r->GetCells().at(icell);
			float phiDet = 0.5 * (c->GetPhi0() + c->GetPhi1());
			for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
				THarm* h = vHarm.at(iharm);
				h->qx += TMath::Cos(phiDet*iharm) * c->GetNch();
				h->qy += TMath::Sin(phiDet*iharm) * c->GetNch();
			}
		}
	}

	// Reconstruct event plane and calculate it's resolution
	for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
		THarm* h = vHarm.at(iharm);
		h->psiReco = TMath::ATan2(h->qy, h->qx)/iharm;
		h->r = TMath::Cos((h->psiReco - h->psiSim) * iharm);
	}

	// Calculate vn_obs (sum across all particles) for detectos other than '0'
	uint32_t nChTot=0;
	for(uint16_t idet=1; idet<vDet.size(); idet++){
		for(uint16_t iring=0; iring<vDet.at(idet)->GetRings().size(); iring++){
			TRing* r = vDet.at(idet)->GetRings().at(iring);
			for(uint16_t icell=0; icell<r->GetCells().size(); icell++){
				TCell* c = r->GetCells().at(icell);
				nChTot += c->GetNch();
				float phiDet = 0.5 * (c->GetPhi0() + c->GetPhi1());
				for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
					THarm* h = vHarm.at(iharm);
					h->vReco += TMath::Cos((phiDet - h->psiReco) * iharm) * c->GetNch();
				}
			}
		}
	}

	// Finish calculating <vn_obs> (avg) and calculate resolution parameter and full event plane resolution
	for(uint16_t iharm=2; iharm<vHarm.size(); iharm++){
		THarm* h = vHarm.at(iharm);
		h->vReco /= nChTot;
	}
}
*/

void TEvent::Draw(uint16_t evn){
	gStyle->SetOptStat(0);
	gPad->GetCanvas()->cd((evn)+1);
	fDnDPhiSim->SetLineColor(kBlack);
	fDnDPhiSim->SetLineWidth(2.0);
	fDnDPhiSim->GetYaxis()->SetRangeUser(-1.0, 3.0);
	fDnDPhiSim->Draw("L");
	Color_t col[4] = {kYellow, kYellow, kRed, kBlue};
	for(uint16_t iharm=par->iharm0; iharm<vHarmSim.size(); iharm++){
		THarm* h = vHarmSim.at(iharm);
		h->f->SetLineColor(col[iharm]);
		h->f->DrawCopy("L SAME");
	}

/*	for(uint16_t iharm=2; iharm<vHarmSim.size(); iharm++){
		THarm* h = vHarmSim.at(iharm);
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
	fDnDPhiReco = new TF1("fNReco",ssfN.str().c_str(), -TMath::Pi(), TMath::Pi());
	fDnDPhiReco->SetLineColor(kGray);
	fDnDPhiReco->SetLineWidth(2);
	fDnDPhiReco->GetYaxis()->SetRangeUser(-0.5,2.0);
	fDnDPhiReco->Draw("L same");
*/
	stringstream ss;
	ss << "h" << evn << "_" << iCentrality;
	TH1D* h = new TH1D(ss.str().c_str(), ss.str().c_str(), 16, -TMath::Pi(), TMath::Pi());
	for(uint16_t ipart=0; ipart<vHit.size(); ipart++){
//		h->Fill(vHit.at(ipart)->phiDet);
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

const vector<THarm*>& TEvent::GetHarmSim() const {
	return vHarmSim;
}

TInputParams* TEvent::GetInputParams(){
	return par;
}
