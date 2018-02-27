/*
 * THarm.h
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#ifndef THARM_H_
#define THARM_H_

#include <TF1.h>

class THarm {
public:
	double vSim;
	double vReco;
	double dvSim;
	double psiSim;
	double psiReco;
	double qx;
	double qy;
	double r; // event plane resolution for subevent (half of hits)
	TF1* fSim;

	THarm();
	~THarm();
};

#endif /* THARM_H_ */
