/*
 * TInputParams.h
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#ifndef TINPUTPARAMS_H_
#define TINPUTPARAMS_H_

using namespace std;

#include <iostream>
#include "TInputCentralityClass.h"

class TInputParams {
public:
	uint16_t iharm0;
	uint32_t nEvents;
	vector<TInputCentralityClass*> vCentClass;
	TInputParams();
	void Print();
};

#endif /* TINPUTPARAMS_H_ */
