/*
 * TInputCentralityClass.h
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#ifndef TINPUTCENTRALITYCLASS_H_
#define TINPUTCENTRALITYCLASS_H_

#include <stdint.h>
#include <vector>

#include <TF1.h>

using namespace std;

class TInputCentralityClass {
public:
	TF1* fDnDEtaSim;
	vector<double> vv;
	vector<double> vdv;

	TInputCentralityClass(TF1* fDnDEtaSim);
	~TInputCentralityClass();
};

#endif /* TINPUTCENTRALITYCLASS_H_ */
