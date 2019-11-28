/*
 * TCell.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: mss
 */

#include "TCell.h"

TCell::TCell(float phi0, float phi1){
	nCh = 0;
	SetPhiRange(phi0, phi1);
}

void TCell::SetPhiRange(float phi0, float phi1){
	this->phi0 = phi0;
	this->phi1 = phi1;
}

float TCell::GetPhi0(){
	return phi0;
}

float TCell::GetPhi1(){
	return phi1;
}

void TCell::IncNch(){
	nCh++;
}

void TCell::ResetNch(){
	nCh=0;
}

int32_t TCell::GetNch(){
	return nCh;
}
