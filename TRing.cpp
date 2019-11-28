/*
 * TRing.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: mss
 */

#include "TRing.h"

TRing::TRing(float r0, float r1, float z) {
	SetRadiusRange(r0, r1);
	SetZ(z);
	CalculateEtaRange();
}

void TRing::SetZ(float z){
	this->z = z;
}

void TRing::SetRadiusRange(float r0, float r1){
	this->r0 = r0;
	this->r1 = r1;
}

void TRing::CalculateEtaRange(){
	float theta0 = atan2(r0,z);
	float theta1 = atan2(r1,z);
	eta0 = -log(tan(0.5*theta0));
	eta1 = -log(tan(0.5*theta1));
	if(eta0 > eta1){ // make it so that eta0 is lower than eta1
		float tmp = eta0;
		eta0 = eta1;
		eta1 = tmp;
	}
}

float TRing::GetEta0() const{
	return eta0;
}

float TRing::GetEta1() const{
	return eta1;
}

void TRing::CreateCells(uint16_t nCells){
	double dphi = 2.0*M_PI/nCells;
	for(uint16_t icell=0; icell<nCells; icell++){
		TCell* c = new TCell(-M_PI + dphi*icell, -M_PI + dphi*(icell+1));
		vCells.push_back(c);
	}
}

const std::vector<TCell*>& TRing::GetCells() const{
	return vCells;
}

void TRing::IncCellValue(uint16_t nCell){
	if(nCell < vCells.size()){
		vCells.at(nCell)->IncNch();
	}
	else{
		std::cout << "<E> TSector::SetCell(): too large cell number: " << nCell << " >= " << vCells.size() << std::endl;
	}
}
