#pragma once

//root
#include <TROOT.h>
#include <TStyle.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TString.h>
#include <TFile.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TXMLEngine.h>
#include <TH2Poly.h>
#include <TLine.h>
#include <TObject.h>

//c, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <vector>

using namespace std;

void pcIOstat (TString outRootFile, int unixTimeInt, const std::vector<float>& val);
std::vector<TGraph*> getGraphsVectorOfDiskOccupancy (TString inRootFile, TString inDatFile);
void dumpVectorOfinDataValuesNames (std::vector<TString> dataValNamStr);
std::vector<TString>& getVectorOfinDataValuesNames (TString inDatFile);
