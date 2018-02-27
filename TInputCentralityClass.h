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

using namespace std;

class TInputCentralityClass {
public:
	uint32_t nPart;
	vector<double> vv;
	vector<double> vdv;

	TInputCentralityClass(uint32_t nPart);
};

#endif /* TINPUTCENTRALITYCLASS_H_ */
