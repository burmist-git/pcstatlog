//my
#include "src/anapcstat.hh"
#include "libntpLoopstatsOne.h"
#include "libpcIOstat.h"

//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]){
  if(argc == 8 && atoi(argv[1])==0){
    TString rootFilesList = argv[2];
    TString outRootFileF = argv[3];
    TString host_n = argv[4];
    TString home_d = argv[5];
    TString ntpstatsFilePath = argv[6];
    TString ntpstatsFileName = argv[7];
    cout<<"--> Parameters : <--"<<endl
	<<"rootFilesList    : "<<rootFilesList<<endl
	<<"outRootFileF     : "<<outRootFileF<<endl
	<<"host_name        : "<<host_n<<endl
	<<"home_dir         : "<<home_d<<endl
	<<"ntpstatsFilePath : "<<ntpstatsFilePath<<endl
	<<"ntpstatsFileName : "<<ntpstatsFileName<<endl;
    anapcstat a(rootFilesList);
    a.set_gr_time_synchronisation(ntpLoopstatsOne(ntpstatsFilePath, ntpstatsFileName, "", host_n));
    a.set_host_name(host_n);
    a.set_home_dir(home_d);
    a.Loop(outRootFileF);
  }
  else if(argc == 10 && atoi(argv[1])==1){
    //cout<<"currentDateTime() "<<currentDateTime()<<endl;
    TString inRootFile = argv[2];
    TString outRootFileF = argv[3];
    TString host_n = argv[4];
    TString home_d = argv[5];
    TString ntpstatsFilePath = argv[6];
    TString ntpstatsFileName = argv[7];
    TString inRootFile_pcIO = argv[8];
    TString inDatFile_pcIO = argv[9];
    cout<<"--> Parameter : <--"<<endl
	<<"inRootFile       : "<<inRootFile<<endl
	<<"outRootFileF     : "<<outRootFileF<<endl
	<<"host_name        : "<<host_n<<endl
	<<"home_dir         : "<<home_d<<endl
	<<"ntpstatsFilePath : "<<ntpstatsFilePath<<endl
	<<"ntpstatsFileName : "<<ntpstatsFileName<<endl
	<<"inRootFile_pcIO  : "<<inRootFile_pcIO<<endl
	<<"inDatFile_pcIO   : "<<inDatFile_pcIO<<endl;
    anapcstat a( inRootFile, atoi(argv[1]));
    a.set_gr_time_synchronisation(ntpLoopstatsOne(ntpstatsFilePath, ntpstatsFileName, "", host_n));
    a.set_grv_diskOccupancy(getGraphsVectorOfDiskOccupancy(inRootFile_pcIO,inDatFile_pcIO));
    a.set_host_name(host_n);
    a.set_home_dir(home_d);
    a.Loop(outRootFileF);
  }
  else if(argc == 4 && atoi(argv[1])==2){
    // if else
  }
  else{
    cout<<" --> ERROR in input arguments "<<endl
	<<" runID [1] = 0 (execution ID number)"<<endl
      	<<"       [2] - file with list of the root files"<<endl
	<<"       [3] - name of root file with histograms"<<endl;
    cout<<" runID [1] = 1 (execution ID number)"<<endl
      	<<"       [2] - in root files"<<endl
	<<"       [3] - name of root file with histograms"<<endl;
  }
  return 0;
}
