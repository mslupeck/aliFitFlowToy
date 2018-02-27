/*
 * TInputParams.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: mss
 */

#include "TInputParams.h"

TInputParams::TInputParams() {
	nEvents = 0;
}

void TInputParams::Print(){
	cout << "  <I> TInputParams::Print()" << endl;
	cout << "  <I> nEvents = " << nEvents << endl;
	for(uint16_t ic=0; ic<vCentClass.size(); ic++){
		TInputCentralityClass* icc = vCentClass.at(ic);
		cout << "  <I>   " << ic << " " << icc->nPart << endl;
		for(uint16_t iv=0; iv<icc->vv.size(); iv++){
			cout << "  <I>     " << iv << " " << icc->vv.at(iv) << " +/- " << icc->vdv.at(iv) << endl;
		}
	}
}
