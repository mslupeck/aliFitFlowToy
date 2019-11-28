/*
 * TCell.h
 *
 *  Created on: Mar 14, 2018
 *      Author: mss
 */

#ifndef TCELL_H_
#define TCELL_H_

#include <stdint.h>

class TCell {
private:
	int32_t nCh; // number of charged particles hitting this cell
	float phi0;
	float phi1;
public:
	TCell(float phi0, float phi1);

	void SetPhiRange(float phi0, float phi1);
	float GetPhi0();
	float GetPhi1();
	void IncNch();
	void ResetNch();
	int32_t GetNch();
};

#endif /* TCELL_H_ */
