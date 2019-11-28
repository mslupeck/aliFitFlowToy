/*
 * TRing.h
 *
 *  Created on: Mar 14, 2018
 *      Author: mss
 */

#ifndef TRING_H_
#define TRING_H_

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>

#include "TCell.h"

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

#endif /* TRING_H_ */
