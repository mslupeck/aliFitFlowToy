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
#include <TF1.h>
#include <TStyle.h>
#include "THarm.h"
#include "THit.h"
#include "TInputParams.h"

using namespace std;

class TEvent {
private:
	TRandom *rnd;
	TInputParams *par;
	int16_t iCentrality;
	vector<THarm*> vHarm;
	vector<THit*> vHit;
	TF1* fNSim;
	TF1* fNReco;
	void InitHarmonics();
	void DestroyHarmonics();
	void InitCentNames();

public:

	static double Round8Sectors(double phi);
	static double Round16Sectors(double phi);
	static double RoundToSectors(int nSectors, double phi);

	TEvent(TInputParams *par, TRandom *rnd);
	~TEvent();
	void Simulate();
	void Draw(uint16_t evn);
	void ApplyGeometry(uint16_t nSectors);
	void Reconstruct();
	void SetCentrality(uint16_t i);

	double GetR(uint16_t iharm) const;
	const vector<THit*>& GetHit() const;
	const vector<THarm*>& GetHarm() const;
};

#endif /* TEVENT_H_ */
