/*
 * TDetector.h
 *
 *  Created on: Feb 27, 2018
 *      Author: mss
 */

#ifndef TDETECTOR_H_
#define TDETECTOR_H_

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>

class TCell {
private:
	int32_t nch;
	float phi0;
	float phi1;
public:
	TCell(float phi0, float phi1);

	void SetPhiRange(float phi0, float phi1);
	float GetPhi0();
	float GetPhi1();
	void IncNch();
	int32_t GetNch();
};

class TRing {
private:
	std::vector<TCell*> vCells;
	float z;
	float r0;
	float r1;
	float eta0;
	float eta1;

public:
	TRing(float r0, float r1, float z);

	void SetZ(float z);
	void SetRadiusRange(float r0, float r1);
	void CalculateEtaRange();
	float GetEta0() const;
	float GetEta1() const;

	void CreateCells(uint16_t nCells);
	const std::vector<TCell*>& GetCells() const;
	void IncCellValue(uint16_t nCell);
};

class TDetector {
private:
	std::vector<TRing*> vRings;

public:
	TDetector(std::string type);

	void AddRing(float r0, float r1, float z, uint16_t nCells);
	const std::vector<TRing*>& GetRings() const;
	void IncCellValue(uint16_t nRing, uint16_t nCell);
};

#endif /* TDETECTOR_H_ */
