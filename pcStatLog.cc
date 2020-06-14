/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Tue Apr 16 13:47:46 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//c, c++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <time.h>

//root
#include <TTree.h>
#include <TFile.h>

const int nDataLog = 10;

void pcStatLog (TString outRootFile, int utint, float data[nDataLog]) {

  Int_t _unixTime = utint;
  Float_t _data[nDataLog];
  for(Int_t i = 0;i<nDataLog;i++)
    _data[i] = data[i];
  //
  TFile *hfile = new TFile( outRootFile, "UPDATE", "pc stat log", 1);
  if (hfile->IsZombie()) {
    std::cout << "PROBLEM with the initialization of the output ROOT ntuple " 
         << outRootFile << ": check that the path is correct!!!"
         <<std::endl;
    assert(0);
  }
  //
  TString data_ss = "data["; data_ss+=nDataLog; data_ss+="]/F";
  TTree *tree;
  if(!(TTree*)hfile->Get("T")){
    //cout<<" FIRST TIME"<<endl;
    tree = new TTree("T", "pc stat log tree");
    tree->Branch("unixTime",&_unixTime, "unixTime/I");
    tree->Branch("data", _data, data_ss.Data());
  }
  else {
    //cout<<" NOT FIRST TIME"<<endl;
    tree = (TTree*)hfile->Get("T");
    tree->SetBranchAddress("unixTime", &_unixTime);
    tree->SetBranchAddress("data", &_data);
  }
  //
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);
  tree->Fill();
  hfile->Write();
  hfile->Close();
}

#ifdef PCSTATLOG_MAIN
# ifndef __CINT__
int main(int argc, char *argv[]) {
  if(argc == 13 && atoi(argv[1]) == 0 ){
    TString outRootFile = argv[2];
    //int utint  = std::time(0);
    int utint  = time(0);
    float data[nDataLog];
    for(Int_t i = 0;i<nDataLog;i++)
      data[i] =  atof(argv[i+3]);
    for(Int_t i = 0;i<nDataLog;i++)
      std::cout<<std::setw(5)<<i<<std::setw(15)<<data[i]<<std::endl;
    pcStatLog  ( outRootFile, utint, data);
  }
  else{
    std::cout<<" ---> ERROR in input arguments "<<std::endl;
    assert(0);
  }
  return 0;
}
# endif //ifndef __CINT__
#endif //ifdef PCSTATLOG_MAIN
