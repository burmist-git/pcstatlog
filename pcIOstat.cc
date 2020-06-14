/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Sun Apr 28 16:50:39 JST 2019                                         *
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
#include <vector>

//root
#include <TTree.h>
#include <TFile.h>
#include <TGraph.h>

void pcIOstat (TString outRootFile, int unixTimeInt, const std::vector<float>& val) {
  //
  Int_t ut = unixTimeInt;
  std::vector<float> *v = new std::vector<float>;
  for(unsigned i = 0; i < val.size(); i++)
    v->push_back(val.at(i));
  //
  TFile *hfile = new TFile( outRootFile, "UPDATE", "Data vector with unix timestamp", 1);
  if (hfile->IsZombie()) {
    std::cout << "PROBLEM with the initialization of the output ROOT ntuple " 
         << outRootFile << ": check that the path is correct!!!"
         <<std::endl;
    assert(0);
  }
  //
  TTree *tree;
  if(!(TTree*)hfile->Get("T")){
    //std::cout<<" FIRST TIME"<<std::endl;
    tree = new TTree("T", "Data tree");
    tree->Branch("ut",&ut, "ut/I");
    tree->Branch("v",v);
  }
  else {
    //std::cout<<" NOT FIRST TIME"<<std::endl;
    tree = (TTree*)hfile->Get("T");
    //tree->Branch("ut",&ut, "ut/I");
    //tree->Branch("v",&v);
    tree->SetBranchAddress("ut", &ut);
    tree->SetBranchAddress("v", &v);
    /*
    TBranch *bv = 0;
    int* pos = v.data();
    tree->SetBranchAddress("v", pos, &bv);
    */
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

std::vector<TString> getVectorOfinDataValuesNames (TString inDatFile) {
  std::vector<TString> dataValNamStr;
  std::ifstream indata;
  TString valName;
  indata.open(inDatFile.Data());
  assert(indata.is_open());  
  while (indata  >>  valName){
    if(indata.eof()){
      std::cout<<"EOF"<<std::endl;
      break;
    }
    dataValNamStr.push_back(valName);
  }
  indata.close();
  return dataValNamStr;
}

void dumpVectorOfinDataValuesNames (std::vector<TString> dataValNamStr) {
  for(unsigned i = 0; i < dataValNamStr.size(); i++)
    std::cout<<"dataValNamStr.at(i) = "<<dataValNamStr.at(i)<<std::endl;
}

std::vector<TGraph*> getGraphsVectorOfDiskOccupancy (TString inRootFile, TString inDatFile) {
  Double_t lineWidth = 2.0;
  Double_t markerSize = 1.0;
  const Int_t nDiffVal = 6;
  const Int_t colorArr[nDiffVal] = { kBlack, kRed, kBlue, kGreen + 3, kMagenta, kGray+1};
  const Int_t markerArr[nDiffVal] = { 20, 20, 20, 21, 21, 21};
  std::vector<TString> dataValNamStr = getVectorOfinDataValuesNames(inDatFile);
  dumpVectorOfinDataValuesNames(dataValNamStr);
  //
  std::vector<TGraph*> grv;
  for (UInt_t i = 0; i < dataValNamStr.size(); ++i) {
    TGraph* gr = new TGraph();
    TString grName = "gr_"; grName += dataValNamStr.at(i); grName += "_vs_ut";
    TString grTitle = " disck occ. : "; grTitle += dataValNamStr.at(i);
    std::cout<<"grName  "<<grName<<std::endl;
    //std::cout<<"grName  "<<grName<<std::endl
    //	     <<"grTitle "<<grTitle<<std::endl;
    gr->SetName(grName.Data());
    gr->SetTitle(grTitle.Data());
    grv.push_back(gr);
  }
  //
  TFile *f = TFile::Open(inRootFile.Data(),"READ");
  if (!f) { 
    assert(0);
  }
  TTree *t; 
  f->GetObject("T",t);
  Int_t ut;
  std::vector<float> *v;
  TBranch *b_ut;
  TBranch *b_v;
  t->SetBranchAddress("ut", &ut, &b_ut);
  t->SetBranchAddress("v", &v, &b_v);
  //Loop
  Long64_t nentries = t->GetEntriesFast();
  std::cout<<"nentries = "<<nentries<<std::endl;
  v = 0;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //Long64_t ientry = t->LoadTree(jentry);
    t->LoadTree(jentry);
    nb = t->GetEntry(jentry);
    nbytes += nb;
    //std::cout<<ientry<<" "<<ut<<std::endl;
    //////////////////////////////////////
    if(v->size() != grv.size()){
      std::cout<<" ERROR ---> v->size() != grv.size()"<<std::endl
	       <<"            v->size()  = "<<v->size()<<std::endl
	       <<"           grv.size()  = "<<grv.size()<<std::endl;
      assert(0);
    }
    for (UInt_t i = 0; i < v->size(); ++i) {
      //std::cout<<"i = "<<i<<std::endl;
      grv.at(i)->SetPoint(grv.at(i)->GetN(), ut, v->at(i));
    }
  }
  //////////////////////////////////////
  for (UInt_t i = 0; i < grv.size(); ++i) {
    grv.at(i)->SetLineColor(colorArr[i - (i/nDiffVal)*nDiffVal]);
    grv.at(i)->SetLineWidth(lineWidth);
    grv.at(i)->SetMarkerColor(colorArr[i - (i/nDiffVal)*nDiffVal]);
    grv.at(i)->SetMarkerStyle(markerArr[i - (i/nDiffVal)*nDiffVal]);
    grv.at(i)->SetMarkerSize(markerSize);
  }
  //////////////////////////////////////
  TString histOut = inRootFile; histOut += "_hist.root";
  TFile* outRootFile = new TFile(histOut.Data(), "RECREATE", " Histograms", 1);
  outRootFile->cd();
  if (outRootFile->IsZombie()){
    std::cout<<"  ERROR ---> file "<<histOut.Data()<<" is zombi"<<std::endl;
    assert(0);
  }
  else
    std::cout<<"  Output Histos file ---> "<<histOut.Data()<<std::endl;
  for (UInt_t i = 0; i < grv.size(); ++i) 
    grv.at(i)->Write();
  outRootFile->Close();
  //////////////////////////////////////
  return grv;
}

#ifdef PCIOSTAT_MAIN
# ifndef __CINT__
int main(int argc, char *argv[]) {
  if(argc > 5  && atoi(argv[1]) == 0 ){
    TString outRootFile = argv[2];
    float dT_of_measuremtns = atof(argv[3]);
    int nv = atoi(argv[4]);
    std::vector<float> v;
    for(int i = 0;i < nv; i++)
      v.push_back(atof(argv[5+i]));
    //int utint  = std::time(0);
    int ut = time(0);
    ut = ut - dT_of_measuremtns/2.0;
    pcIOstat ( outRootFile, ut, v);
  }
  else{
    std::cout<<" ---> ERROR in input arguments "<<std::endl;
    assert(0);
  }
  return 0;
}
# endif //ifndef __CINT__
#endif //ifdef PCIOSTAT_MAIN
