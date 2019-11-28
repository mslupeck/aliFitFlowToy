/*
 * TEvent.h
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#ifndef TEVENT_H_
#define TEVENT_H_

#include <stdint.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <TRandom3.h>
#include <TMath.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TStyle.h>

#include "THarm.h"
#include "THit.h"
#include "TInputParams.h"
#include "TDetector.h"

using namespace std;

class TEvent {
private:
	TRandom *rnd;
	TInputParams *par;
	int16_t iCentrality;
	vector<THit*> vHit;
	TF1* fDnDPhiSim;
	TF1* fDnDPhiReco;
	std::vector<THarm*> vHarmSim; // container for simulated flow parameters

	void InitHarmonics();
	void DestroyHarmonics();

public:
	TEvent(TInputParams *par, TRandom *rnd);
	~TEvent();
	void Draw(uint16_t evn);
	void Simulate();
	void Transport(vector<TDetector*> &vDet);
	void ReconstructR(vector<TDetector*> &vDet);     // Use basic method
	void ReconstructRSub3(vector<TDetector*> &vDet); // Use 3-sub event method
	void FillHistos(vector<TH1D*>& vh, vector<TH2D*> vh2, vector<TDetector*> &vDet);
	void Reconstruct_old(vector<TDetector*> &vDet);
	void SetCentrality(uint16_t i);

//	double GetR(uint16_t iharm) const;
	const vector<THit*>& GetHit() const;
	const vector<THarm*>& GetHarmSim() const;
	TInputParams* GetInputParams();
};

#endif /* TEVENT_H_ */
