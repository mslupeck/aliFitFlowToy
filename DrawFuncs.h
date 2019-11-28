/*
 * DrawFuncs.h
 *
 *  Created on: Feb 28, 2018
 *      Author: mss
 */

#ifndef DRAWFUNCS_H_
#define DRAWFUNCS_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include <TFile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TStyle.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TLegend.h>

#include "TInputParams.h"

using namespace std;

class DrawFuncs {
private:
	static void SetPadMargins();
public:
	DrawFuncs();

	static void FileContents(TInputParams &inp);

//	static void InvertedR(string inpath, TInputParams &inp);
//	static void Overlay(string inpath, TInputParams &inp);
//	static void CompareGeom(TInputParams &inp);
//	static void CompareOthers(TInputParams &inp);
};

#endif /* DRAWFUNCS_H_ */
