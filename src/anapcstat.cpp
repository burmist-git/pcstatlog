//my
#include "anapcstat.hh"

//root
#include <TROOT.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TRandom3.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TText.h>

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

void anapcstat::Loop(TString histOut){
  TString cpuuserStr;
  TString memStr;
  Double_t lineWidth = 2.0;
  Double_t timeAxisLabelOffset = 0.025;
  Int_t markerType = 1;
  Double_t markerSize = 1.0;
  Double_t dTtimeSynch = 0.1;
  Int_t i = 0;
  Double_t scaleArr[nChannels] = {1048576.0, 1048576.0, 1048576.0, 1048576.0, 1.0,
				        1.0,       1.0,       1.0,       1.0, 1.0};
  TGraph *gr_val_vs_t[nChannels];
  tGraphInit(gr_val_vs_t, "gr_val_vs_t", "val vs t");
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"nentries = "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  TGraph *gr_tot_disc_r = new TGraph(); 
  TGraph *gr_tot_disc_h = new TGraph(); 
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    for(i = 0;i<nChannels;i++)
      gr_val_vs_t[i]->SetPoint(gr_val_vs_t[i]->GetN(), unixTime, data[i]/scaleArr[i]);
    gr_tot_disc_r->SetPoint(gr_tot_disc_r->GetN(), unixTime, data[0]/scaleArr[0] + data[1]/scaleArr[1]);
    gr_tot_disc_h->SetPoint(gr_tot_disc_h->GetN(), unixTime, data[2]/scaleArr[2] + data[3]/scaleArr[3]);
  }
  //////////////////
  for( i = 0; i<nChannels; i++){
    gr_val_vs_t[i]->SetLineColor(kBlack);
    gr_val_vs_t[i]->SetLineWidth(lineWidth);
    gr_val_vs_t[i]->SetMarkerColor(kBlack);
    gr_val_vs_t[i]->SetMarkerStyle(markerType);
    gr_val_vs_t[i]->SetMarkerSize(markerSize);
    gr_val_vs_t[i]->GetXaxis()->SetTimeDisplay(1);
    gr_val_vs_t[i]->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}%F1970-01-01 00:00:00"); 
    gr_val_vs_t[i]->GetXaxis()->SetLabelOffset(timeAxisLabelOffset);
    //gr_val_vs_t[i]->GetXaxis()->SetTitle("Time, h");
  }
  //////////////////
  //gStyle->SetPalette(kRainBow);
  TCanvas *c1 = new TCanvas("c1","c1",10,10,1200,1200);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetStatColor(kWhite);
  TString c1Title = "pc statistic "; c1Title += host_name;
  c1->SetTitle(c1Title.Data());
  c1->SetName("c1_pc_statistic");
  c1->SetRightMargin(0.12);
  c1->SetLeftMargin(0.12);
  c1->SetTopMargin(0.1);
  c1->SetBottomMargin(0.15);
  c1->Divide(3,3);
  //$rootUsed         0
  //$rootAvail        1
  //$homeUsed         2
  //$homeAvail        3
  //$avgcpuuser       4
  //$avgcpuusersystem 5
  //$avgcpuuseridle   6
  //$memtotal         7
  //$memused          8
  //$memfree          9
  //
  c1->cd(1);
  c1->GetPad(1)->SetGrid();
  TMultiGraph *mg1 = new TMultiGraph("mg1","Disc space : /, GB");
  //Double_t dsMax = 1.1*(TMath::MaxElement(gr_val_vs_t[0]->GetN(),gr_val_vs_t[0]->GetY()) + TMath::MaxElement(gr_val_vs_t[1]->GetN(),gr_val_vs_t[1]->GetY()));
  Double_t dsMax = 1.1*(getMaxElement(gr_val_vs_t[0]->GetN(),gr_val_vs_t[0]->GetY()) + getMaxElement(gr_val_vs_t[1]->GetN(),gr_val_vs_t[1]->GetY()));
  mg1->SetMaximum(dsMax);
  mg1->SetMinimum(0.0);
  mg1->Add(gr_val_vs_t[0]);
  mg1->Add(gr_val_vs_t[1]);
  mg1->Add(gr_tot_disc_r);
  gr_tot_disc_r->SetLineColor(kRed);
  gr_tot_disc_r->SetMarkerColor(kRed);
  gr_tot_disc_r->SetLineStyle(kDashed);
  gr_val_vs_t[1]->SetLineColor(kGreen + 2);
  gr_val_vs_t[1]->SetMarkerColor(kGreen + 2);
  mg1->Draw("APL");
  //mg1->GetXaxis()->SetTitle("Time, h");
  mg1->GetYaxis()->SetTitle("Disc space, GB");  
  mg1->GetXaxis()->SetTimeDisplay(1);
  mg1->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}%F1970-01-01 00:00:00");
  mg1->GetXaxis()->SetLabelOffset(timeAxisLabelOffset);
  //
  c1->cd(2);
  c1->GetPad(2)->SetGrid();
  TString mg2Name = "Disc space : "; mg2Name += home_dir; mg2Name += ", GB";
  TMultiGraph *mg2 = new TMultiGraph("mg2",mg2Name.Data());
  //dsMax = 1.1*(TMath::MaxElement(gr_val_vs_t[2]->GetN(),gr_val_vs_t[2]->GetY()) + TMath::MaxElement(gr_val_vs_t[3]->GetN(),gr_val_vs_t[3]->GetY()));
  dsMax = 1.1*(getMaxElement(gr_val_vs_t[2]->GetN(),gr_val_vs_t[2]->GetY()) + getMaxElement(gr_val_vs_t[3]->GetN(),gr_val_vs_t[3]->GetY()));
  mg2->SetMaximum(dsMax);
  mg2->SetMinimum(0.0);
  mg2->Add(gr_val_vs_t[2]);
  mg2->Add(gr_val_vs_t[3]);
  mg2->Add(gr_tot_disc_h);
  gr_tot_disc_h->SetLineColor(kRed);
  gr_tot_disc_h->SetMarkerColor(kRed);
  gr_tot_disc_h->SetLineStyle(kDashed);
  gr_val_vs_t[3]->SetLineColor(kGreen + 2);
  gr_val_vs_t[3]->SetMarkerColor(kGreen + 2);
  mg2->Draw("APL");
  //mg2->GetXaxis()->SetTitle("Time, h");
  mg2->GetYaxis()->SetTitle("Disc space, GB");
  mg2->GetXaxis()->SetTimeDisplay(1);
  mg2->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}%F1970-01-01 00:00:00"); 
  mg2->GetXaxis()->SetLabelOffset(timeAxisLabelOffset);
  //
  c1->cd(3);
  TLegend *leg = new TLegend(0.1,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(gr_val_vs_t[0], "Used disc space","lp");
  leg->AddEntry(gr_val_vs_t[1], "Free disc space","lp");
  leg->Draw();
  double utstart;
  double utstop;
  double temper1;
  gr_val_vs_t[0]->GetPoint(1, utstart, temper1);
  TString timeStr_start = "Time start : ";
  timeStr_start += getLocalTimeStringFromUnixTime( utstart );
  gr_val_vs_t[0]->GetPoint(  gr_val_vs_t[0]->GetN()-1, utstop, temper1);
  TString timeStr_stop = "Time stop : ";
  timeStr_stop += getLocalTimeStringFromUnixTime( utstop );
  TText *text1_start = new TText(0.1,0.5,timeStr_start.Data());
  text1_start->SetTextSize(0.05);
  text1_start->Draw();
  TText *text1_stop = new TText(0.1,0.45,timeStr_stop.Data());
  text1_stop->SetTextSize(0.05);
  text1_stop->Draw();
  TString hostStr = "Host : "; hostStr += host_name;
  TText *text_hostn = new TText(0.1,0.40,hostStr.Data());
  text_hostn->SetTextSize(0.05);
  text_hostn->Draw();
  double tsynchstart;
  double tsynchstop;
  double valtmp;
  gr_time_synchronisation->GetPoint(1, tsynchstart, valtmp);
  gr_time_synchronisation->GetPoint(gr_time_synchronisation->GetN()-1, tsynchstop, valtmp);
  TString dtimeSynchroStr_start = "Time synchro mon start : ";
  dtimeSynchroStr_start += getLocalTimeStringFromUnixTime(tsynchstart);
  TString dtimeSynchroStr_stop = "Time synchro mon stop : ";
  dtimeSynchroStr_stop += getLocalTimeStringFromUnixTime(tsynchstop);
  TText *text_dtimeSynchro_start = new TText(0.1,0.35,dtimeSynchroStr_start.Data());
  text_dtimeSynchro_start->SetTextSize(0.035);
  text_dtimeSynchro_start->Draw();
  TText *text_dtimeSynchro_stop = new TText(0.1,0.30,dtimeSynchroStr_stop.Data());
  text_dtimeSynchro_stop->SetTextSize(0.035);
  text_dtimeSynchro_stop->Draw();
  if (grv_diskOccupancy.size()>0) {
    double tdiskOccstart;
    double tdiskOccstop;
    double valdocc;
    grv_diskOccupancy.at(0)->GetPoint(1, tdiskOccstart, valdocc);
    grv_diskOccupancy.at(0)->GetPoint(grv_diskOccupancy.at(0)->GetN()-1, tdiskOccstop, valdocc);
    TString ddiskOcctartStr_start = "Time disk occ. start : ";
    ddiskOcctartStr_start += getLocalTimeStringFromUnixTime(tdiskOccstart);
    TString ddiskOcctartStr_stop = "Time disk occ. stop : ";
    ddiskOcctartStr_stop += getLocalTimeStringFromUnixTime(tdiskOccstop);
    TText *text_diskOcctart_start = new TText(0.1,0.25,ddiskOcctartStr_start.Data());
    text_diskOcctart_start->SetTextSize(0.035);
    text_diskOcctart_start->Draw();
    TText *text_diskOcctart_stop = new TText(0.1,0.20,ddiskOcctartStr_stop.Data());
    text_diskOcctart_stop->SetTextSize(0.035);
    text_diskOcctart_stop->Draw();
  }
  //
  c1->cd(4);
  c1->GetPad(4)->SetGrid();
  TString mg_diskOcc_Name = "Disk occupancy, %";
  TMultiGraph *mg_diskOcc = new TMultiGraph("mg_diskOcc",mg_diskOcc_Name.Data());
  mg_diskOcc->SetMaximum(110.0);
  mg_diskOcc->SetMinimum(-10.0);
  for (UInt_t ui = 0; ui < grv_diskOccupancy.size(); ++ui){ 
    //grv_diskOccupancy.at(ui)->Draw("APL");
    if(grv_diskOccupancy.at(ui)->GetN() > 0)
      mg_diskOcc->Add(grv_diskOccupancy.at(ui));
    //grv_diskOccupancy.at(ui)->SetLineColor(kBlack);
    //grv_diskOccupancy.at(ui)->SetMarkerColor(kBlack);
    //grv_diskOccupancy.at(ui)->SetLineWidth(3.0);
  }
  //if (mg_diskOcc->GetListOfGraphs()->GetEntries() > 0)
  //cout<<"mg_diskOcc->GetListOfGraphs()->GetEntries() "<<mg_diskOcc->GetListOfGraphs()->GetEntries()<<endl;
  mg_diskOcc->Draw("APL");
  //mg_diskOcc->GetXaxis()->SetTitle("Time, h");
  mg_diskOcc->GetYaxis()->SetTitle(mg_diskOcc_Name.Data());
  mg_diskOcc->GetXaxis()->SetTimeDisplay(1);
  mg_diskOcc->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}%F1970-01-01 00:00:00"); 
  mg_diskOcc->GetXaxis()->SetLabelOffset(timeAxisLabelOffset);
  //
  c1->cd(5);
  c1->GetPad(5)->SetGrid();
  TString mg_cpu_Name = "CPU usage : average, system, IDLE, %";
  TMultiGraph *mg_cpu = new TMultiGraph("mg_cpu",mg_cpu_Name.Data());
  gr_val_vs_t[4]->SetLineColor(kBlue);
  gr_val_vs_t[4]->SetMarkerColor(kBlue);
  gr_val_vs_t[5]->SetLineColor(kRed);
  gr_val_vs_t[5]->SetMarkerColor(kRed);
  gr_val_vs_t[6]->SetLineColor(kBlack);
  gr_val_vs_t[6]->SetMarkerColor(kBlack);
  mg_cpu->SetMaximum(110.0);
  mg_cpu->SetMinimum(0.0);
  mg_cpu->Add(gr_val_vs_t[4]);
  mg_cpu->Add(gr_val_vs_t[5]);
  mg_cpu->Add(gr_val_vs_t[6]);
  mg_cpu->Draw("APL");
  //mg_cpu->GetXaxis()->SetTitle("Time, h");
  mg_cpu->GetYaxis()->SetTitle("CPU, %");
  mg_cpu->GetXaxis()->SetTimeDisplay(1);
  mg_cpu->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}%F1970-01-01 00:00:00"); 
  mg_cpu->GetXaxis()->SetLabelOffset(timeAxisLabelOffset);
  //
  c1->cd(6);
  TLegend *legcpu = new TLegend(0.1,0.6,0.9,0.9,"","brNDC");
  legcpu->AddEntry(gr_val_vs_t[4], "Average  usage, %","lp");
  legcpu->AddEntry(gr_val_vs_t[5], "System CPU usage, %","lp");
  legcpu->AddEntry(gr_val_vs_t[6], "IDLE, %","lp");
  legcpu->Draw();
  TLegend *leg_diskOcc = new TLegend(0.1,0.25,0.9,0.55,"","brNDC");
  for (UInt_t ui = 0; ui < grv_diskOccupancy.size(); ++ui)
    leg_diskOcc->AddEntry(grv_diskOccupancy.at(ui), grv_diskOccupancy.at(ui)->GetTitle(),"lp");
  leg_diskOcc->Draw();
  //
  //Double_t maxMem = 1.1*TMath::MaxElement(gr_val_vs_t[7]->GetN(),gr_val_vs_t[7]->GetY());
  Double_t maxMem = 1.1*getMaxElement(gr_val_vs_t[7]->GetN(),gr_val_vs_t[7]->GetY());
  c1->cd(7);
  c1->GetPad(7)->SetGrid();
  memStr = "Used memory, MB";
  TMultiGraph *mg3 = new TMultiGraph("mg3",memStr.Data());
  mg3->SetMaximum(maxMem);
  mg3->SetMinimum(0.0);
  mg3->Add(gr_val_vs_t[7]);
  mg3->Add(gr_val_vs_t[8]);
  gr_val_vs_t[7]->SetLineColor(kRed);
  gr_val_vs_t[7]->SetMarkerColor(kRed);
  gr_val_vs_t[7]->SetLineStyle(kDashed);
  mg3->Draw("APL");
  //mg3->GetXaxis()->SetTitle("Time, h");
  mg3->GetYaxis()->SetTitle("Memory, MB");
  mg3->GetXaxis()->SetTimeDisplay(1);
  mg3->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}%F1970-01-01 00:00:00");
  mg3->GetXaxis()->SetLabelOffset(timeAxisLabelOffset);
  gr_val_vs_t[8]->SetTitle(memStr.Data());
  gr_val_vs_t[8]->SetMaximum(maxMem);
  gr_val_vs_t[8]->SetMinimum(0.0);
  //gr_val_vs_t[8]->Draw("APL");
  gr_val_vs_t[8]->GetYaxis()->SetTitle("Memory, MB");
  //
  c1->cd(8);
  c1->GetPad(8)->SetGrid();
  memStr = "Free memory, MB";
  TMultiGraph *mg4 = new TMultiGraph("mg4",memStr.Data());
  mg4->SetMaximum(maxMem);
  mg4->SetMinimum(0.0);
  mg4->Add(gr_val_vs_t[7]);
  mg4->Add(gr_val_vs_t[9]);
  mg4->Draw("APL");
  //mg4->GetXaxis()->SetTitle("Time, h");
  mg4->GetYaxis()->SetTitle("Memory, MB");
  mg4->GetXaxis()->SetTimeDisplay(1);
  mg4->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}%F1970-01-01 00:00:00");
  mg4->GetXaxis()->SetLabelOffset(timeAxisLabelOffset);
  gr_val_vs_t[9]->SetTitle(memStr.Data());
  gr_val_vs_t[9]->SetMaximum(maxMem);
  gr_val_vs_t[9]->SetMinimum(0.0);
  //gr_val_vs_t[9]->Draw("APL");
  gr_val_vs_t[9]->GetYaxis()->SetTitle("Memory, MB");
  //
  c1->cd(9);
  c1->GetPad(9)->SetGrid();
  TString timesynchStr = "PC time synchronisation, s";
  gr_time_synchronisation->SetTitle(timesynchStr.Data());
  gr_time_synchronisation->SetMaximum(dTtimeSynch);
  gr_time_synchronisation->SetMinimum(-dTtimeSynch);
  gr_time_synchronisation->Draw("APL");
  gr_time_synchronisation->GetYaxis()->SetTitle("#Delta t, s");
  gr_time_synchronisation->GetXaxis()->SetTimeDisplay(1);
  gr_time_synchronisation->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}%F1970-01-01 00:00:00"); 
  gr_time_synchronisation->GetXaxis()->SetLabelOffset(timeAxisLabelOffset);
  //gr_time_synchronisation->GetXaxis()->SetTitle("Time, h");
  //////////////////
  TFile* rootFile = new TFile(histOut.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<histOut.Data()<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<histOut.Data()<<endl;
  mg1->Write();
  mg2->Write();
  c1->Write();
  for(i = 0;i<nChannels;i++)
    gr_val_vs_t[i]->Write();
  gr_time_synchronisation->Write();
  for (UInt_t ui = 0; ui < grv_diskOccupancy.size(); ++ui) 
    grv_diskOccupancy.at(ui)->Write();
  rootFile->Close();
  TString c1pdf = histOut; c1pdf += ".pdf";
  c1->SaveAs(c1pdf.Data());
}

TString anapcstat::getLocalTimeStringFromUnixTime( double ut){
  time_t t_ut = ut;
  struct tm * timeinfo = localtime(&t_ut);
  TString timeStr = asctime(timeinfo);
  //printf ("Current local time and date: %s", asctime(timeinfo));
  return timeStr;
}

Double_t anapcstat::getMaxElement(Int_t n, Double_t *arr){
  Double_t maxEl = -999.0;
  if(n>0){
    for(Int_t i = 0;i<n;i++){
      if(arr[i]>maxEl){
	maxEl = arr[i];
      }
    }
  }
  else
    assert(0);
  return maxEl;
}
