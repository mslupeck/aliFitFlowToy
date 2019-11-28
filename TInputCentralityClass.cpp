/*
 * TInputCentralityClass.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#include "TInputCentralityClass.h"

TInputCentralityClass::TInputCentralityClass(TF1* fDnDEtaSim) {
	this->fDnDEtaSim = fDnDEtaSim;
}

TInputCentralityClass::~TInputCentralityClass() {
	if(fDnDEtaSim != 0){
		delete fDnDEtaSim;
	}
}
