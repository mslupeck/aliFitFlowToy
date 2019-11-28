/*
 * TDetector.h
 *
 *  Created on: Feb 27, 2018
 *      Author: mss
 */

#ifndef TDETECTOR_H_
#define TDETECTOR_H_

#include "TInputParams.h"
#include "THarm.h"
#include "TRing.h"

class TDetector {
private:
	TInputParams *par;			// structure with input parameters
	std::string type;			// name and type of the detector
	std::vector<TRing*> vRings; // container with rings covering some eta range
	float maxEta;				// globally for the whole detector
	float minEta;				// globally for the whole detector
	std::vector<THarm*> vHarmReco;	// container for flow parameters that depend on harmonic number, reconstructed using this detector
	float efficiency;

	int InitGeo();
	void FindMaxMinEta();
	void InitHarmonics();
	void DestroyHarmonics();

public:
	TDetector(std::string type, TInputParams *par, float eff=1.0);
	virtual ~TDetector();

	float GetMaxEta() const;
	float GetMinEta() const;
	float GetEff() const;
	std::string& GetType();

	uint32_t GetTotNch();
	void AddRing(float r0, float r1, float z, uint16_t nCells);
	const std::vector<TRing*>& GetRings() const;
	void IncCellValue(uint16_t nRing, uint16_t nCell);
	void ResetAllCells();

	const std::vector<THarm*>& GetHarmReco() const;
	void AddQ(uint16_t iharm, double addx, double addy);
	void DivQ(uint16_t iharm, double divx, double divy);
	void SetPsiReco(uint16_t iharm, double psi);
	void SetRReco(uint16_t iharm, double r);
	void ResetHarm();

	void Print();
};

#endif /* TDETECTOR_H_ */
