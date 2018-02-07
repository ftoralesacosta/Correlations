#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TH2D.h>
#include <THStack.h>
#include <TProfile.h>
#include <iostream>
#include <fstream>

#define NTRACK_MAX (1U << 14)

#include <vector>
#include <math.h>

int main(int argc, char *argv[])
{
  if (argc < 2) {
    exit(EXIT_FAILURE);
  }
  int dummyc = 1;
  char **dummyv = new char *[1];

  dummyv[0] = strdup("main");

  for (int iarg = 1; iarg < argc; iarg++) {
    std::cout << "Opening: " << (TString)argv[iarg] << std::endl;
    TFile *file = TFile::Open((TString)argv[iarg]);

    if (file == NULL) {
      std::cout << " fail" << std::endl;
      exit(EXIT_FAILURE);
    }
    file->Print();

    TTree *_tree_event = dynamic_cast<TTree *>(file->Get("_tree_event"));

    if (_tree_event == NULL) {
      std::cout << " fail " << std::endl;
      exit(EXIT_FAILURE);
    }  
    //_tree_event->Print();

    TApplication application("", &dummyc, dummyv);
    
    UInt_t ntrack;
    Float_t track_pt[NTRACK_MAX];
    Float_t track_eta[NTRACK_MAX];
    Float_t track_phi[NTRACK_MAX];
    UChar_t track_quality[NTRACK_MAX];
    Long64_t Mix_Events[10];

    _tree_event->SetBranchAddress("ntrack", &ntrack);
    _tree_event->SetBranchAddress("track_pt", track_pt);
    _tree_event->SetBranchAddress("track_eta",track_eta);
    _tree_event->SetBranchAddress("track_phi", track_phi);
    _tree_event->SetBranchAddress("track_quality", track_quality);
    _tree_event->SetBranchAddress("Mix_Events", Mix_Events);

    TFile *file_mixed = TFile::Open((TString)argv[iarg]);
    //file_mixed->Print();
    TTree *_tree_mixevent = dynamic_cast<TTree *>(file_mixed->Get("_tree_event"));
   
    UInt_t mixntrack;
    Float_t mixtrack_pt[NTRACK_MAX];
    Float_t mixtrack_eta[NTRACK_MAX];
    Float_t mixtrack_phi[NTRACK_MAX];
    UChar_t mixtrack_quality[NTRACK_MAX];

    _tree_mixevent->SetBranchAddress("ntrack", &mixntrack);
    _tree_mixevent->SetBranchAddress("track_pt", mixtrack_pt);
    _tree_mixevent->SetBranchAddress("track_eta",mixtrack_eta);
    _tree_mixevent->SetBranchAddress("track_phi", mixtrack_phi);
    _tree_mixevent->SetBranchAddress("track_quality", mixtrack_quality);

    for (Long64_t ievent = 0; ievent < _tree_event->GetEntries(); ievent++){
      _tree_event->GetEntry(ievent);
      for(int imix=0; imix<10; imix++){
	_tree_mixevent->GetEntry(Mix_Events[imix]);
      }
      if(ievent % 10 == 0){
	std::cout<< "Event # " << ievent << " / " << _tree_event->GetEntries() << std::endl;
      }
    } 


 }//loop over args
}
