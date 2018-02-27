/*
 * THarm.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#include "THarm.h"

THarm::THarm() {
	vSim = 0;
	vReco = 0;
	dvSim = 0;
	psiSim = -1;
	psiReco = -1;
	qx = 0;
	qy = 0;
	r = 0;
	fSim = NULL;
}

THarm::~THarm() {
	if(fSim != NULL){
		delete fSim;
	}
}
