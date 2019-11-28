/*
 * THarm.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#include "THarm.h"

THarm::THarm() {
	Reset();
}

void THarm::Reset() {
	v = 0;
	dv = 0;
	psi = -1;
	qx = 0;
	qy = 0;
	r = 0;
	f = NULL;
}

THarm::~THarm() {
	if(f != NULL){
		delete f;
	}
}
