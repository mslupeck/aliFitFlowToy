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
	double v;
	double dv;
	double psi;
	double qx;
	double qy;
	double r;
	TF1* f;

	THarm();
	~THarm();
	void Reset();
};

#endif /* THARM_H_ */
