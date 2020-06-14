#ifndef anapcstat_hh
#define anapcstat_hh

//My
#include "anapcstatbase.hh"

//c, c++
#include <vector>

//root
#include <TROOT.h>

class TChain;
class TFile;
class TTree;
class TString;
class TBranch;
class TGraph;

class anapcstat: public anapcstatbase {
public:
  anapcstat(TString fileList) : anapcstatbase(fileList)
  {
  }

  anapcstat(TString file, Int_t key) : anapcstatbase(file, key)
  {
  }

  void Loop(TString histOut);

  void set_host_name(TString host_n) {host_name = host_n;};
  void set_home_dir(TString home_d) {home_dir = home_d;};
  TString getLocalTimeStringFromUnixTime( double ut );
  void set_gr_time_synchronisation(TGraph *gr) {gr_time_synchronisation = gr;};
  void set_grv_diskOccupancy(std::vector<TGraph*> grv) {grv_diskOccupancy = grv;};

private:
  TString host_name;
  TString home_dir;
  TGraph *gr_time_synchronisation;
  Double_t getMaxElement(Int_t n, Double_t *arr);
  std::vector<TGraph*> grv_diskOccupancy;
};

#endif
