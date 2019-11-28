/*
 * TDetector.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: mss
 */

#include "TDetector.h"

TDetector::TDetector(std::string type, TInputParams *par, float eff) {
	this->type = type;
	this->par = par;
	this->efficiency = eff;
	InitGeo();
	FindMaxMinEta();
	InitHarmonics();
}

TDetector::~TDetector() {
	DestroyHarmonics();
}

int TDetector::InitGeo(){
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
	else if(type.compare("TPC") == 0){
		AddRing(2466,1e8,2497,180);
		AddRing(2466,1e8,-2497,180);
	}
	else{
		std::cout << "<E> TDetector::TDetector(): Empty geometry initialized: " << type << std::endl;
		return -1;
	}
	return 0;
}

void TDetector::FindMaxMinEta(){
	minEta = 1e30;
	maxEta = -1e30;
	for(uint16_t iring=0; iring<vRings.size(); iring++){
		TRing* r = vRings.at(iring);
		float eta0 = r->GetEta0();
		float eta1 = r->GetEta1();
		if(eta0 < minEta){
			minEta = eta0;
		}
		if(eta1 > maxEta){
			maxEta = eta1;
		}
		//std::cout << "  <I> TDetector::FindMaxMinEta(): " << iring << " " << eta0 << " " << eta1 << " | " << minEta << " " << maxEta << std::endl;
	}
}

void TDetector::InitHarmonics(){
	for(uint16_t iharm=0; iharm<par->vCentClass.at(0)->vv.size(); iharm++){
		vHarmReco.push_back(new THarm());
	}
}

void TDetector::DestroyHarmonics(){
	for(uint16_t iharm=0; iharm<vHarmReco.size(); iharm++){
		delete vHarmReco.at(iharm);
	}
}

float TDetector::GetMaxEta() const{
	return maxEta;
}

float TDetector::GetMinEta() const{
	return minEta;
}

float TDetector::GetEff() const{
	return efficiency;
}

std::string& TDetector::GetType() {
	return type;
}

uint32_t TDetector::GetTotNch(){
	uint32_t nch=0;
	for(uint16_t iring=0; iring<vRings.size(); iring++){
		for(uint16_t icell=0; icell<vRings.at(iring)->GetCells().size(); icell++){
			nch += vRings.at(iring)->GetCells().at(icell)->GetNch();
		}
	}
	return nch;
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

void TDetector::ResetAllCells(){
	for(uint16_t iring=0; iring<vRings.size(); iring++){
		for(uint16_t icell=0; icell<vRings.at(iring)->GetCells().size(); icell++){
			vRings.at(iring)->GetCells().at(icell)->ResetNch();
		}
	}
}

const std::vector<THarm*>& TDetector::GetHarmReco() const{
	return vHarmReco;
}

void TDetector::AddQ(uint16_t iharm, double addx, double addy){
	if(iharm < vHarmReco.size()){
		vHarmReco.at(iharm)->qx += addx;
		vHarmReco.at(iharm)->qy += addy;
	}
	else{
		std::cout << "<E> TDetector::AddQx(): too large harmonic number: " << iharm << " >= " << vHarmReco.size() << std::endl;
	}
}

void TDetector::DivQ(uint16_t iharm, double divx, double divy){
	if(iharm < vHarmReco.size()){
		vHarmReco.at(iharm)->qx /= divx;
		vHarmReco.at(iharm)->qy /= divy;
	}
	else{
		std::cout << "<E> TDetector::DivQx(): too large harmonic number: " << iharm << " >= " << vHarmReco.size() << std::endl;
	}
}

void TDetector::SetPsiReco(uint16_t iharm, double psi){
	if(iharm < vHarmReco.size()){
		vHarmReco.at(iharm)->psi = psi;
	}
	else{
		std::cout << "<E> TDetector::SetPsi(): too large harmonic number: " << iharm << " >= " << vHarmReco.size() << std::endl;
	}
}

void TDetector::SetRReco(uint16_t iharm, double r){
	if(iharm < vHarmReco.size()){
		vHarmReco.at(iharm)->r = r;
	}
	else{
		std::cout << "<E> TDetector::DivR(): too large harmonic number: " << iharm << " >= " << vHarmReco.size() << std::endl;
	}
}

void TDetector::ResetHarm(){
	for(uint16_t iharm=0; iharm<vHarmReco.size(); iharm++){
		vHarmReco.at(iharm)->Reset();
	}
}

void TDetector::Print(){
	std::cout << "  <I> TDetector::Print(): Base:   " << type << " (" << minEta << ", " << maxEta << ") " << GetTotNch();
}
