/*
 * TDetector.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: mss
 */

#include "TDetector.h"

TCell::TCell(float phi0, float phi1){
	nch = 0;
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
	nch++;
}

int32_t TCell::GetNch(){
	return nch;
}




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
		TCell* c = new TCell(dphi*icell, dphi*(icell+1));
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




TDetector::TDetector(std::string type) {
	if(type.compare("V0A_1x8") == 0){
		AddRing(4.3, 41.2, 329, 8);
	}
	else if(type.compare("V0C_1x8") == 0){
		AddRing(4.5, 32.0, -87, 8);
	}
	else if(type.compare("V0A_4x8") == 0){
		AddRing( 4.3,  7.5, 329, 8);
		AddRing( 7.7, 13.7, 329, 8);
		AddRing(13.9, 22.6, 329, 8);
		AddRing(22.8, 41.2, 329, 8);
	}
	else if(type.compare("V0C_4x8") == 0){
		AddRing( 4.5,  7.1, -86, 8);
		AddRing( 7.3, 11.7, -87, 8);
		AddRing(11.9, 19.3, -88, 8);
		AddRing(19.5, 32.0, -88, 8);
	}
	else{
		std::cout << "<E> TDetector::TDetector(): No geometry initialized." << std::endl;
	}
}

void TDetector::AddRing(float r0, float r1, float z, uint16_t nCells){
	TRing* r = new TRing(r0, r1, z);
	r->CreateCells(nCells);
	vRings.push_back(r);
}

const std::vector<TRing*>& TDetector::GetRings() const{
	return vRings;
}

void TDetector::IncCellValue(uint16_t nRing, uint16_t nCell){
	if(nRing < vRings.size()){
		vRings.at(nRing)->IncCellValue(nCell);
	}
	else{
		std::cout << "<E> TDetector::SetCell(): too large sector number: " << nRing << " >= " << vRings.size() << std::endl;
	}
}
