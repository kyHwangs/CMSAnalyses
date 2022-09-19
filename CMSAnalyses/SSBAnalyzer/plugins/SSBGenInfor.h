// -*- C++ -*-
//
// Package:    SSBGenInfor
// Class:      SSBGenInfor
// 
/**\class SSBGenInfor SSBGenInfor.cc CMSAnalyses/SSBGenInfor/plugins/SSBGenInfor.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author: Seungkyu Ha, Sehwook Lee, Suyoung Choi, Jaehoon Lim,  Korea University. SSB developers. 
//         Created:  Mon Dec 29 10:42:18 KST 2014
// $Id$
//
//

#ifndef SSBGenInfor_h
#define SSBGenInfor_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

// GenJet
#include "DataFormats/JetReco/interface/GenJet.h"

// GenMET
//#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/PatCandidates/interface/MET.h"

// TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h" 
#include "CommonTools/UtilAlgos/interface/TFileService.h"   

// TreeManager
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBTreeManager.h"

// pythia Event History
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/Ref.h"

// root
#include "TTree.h"
//
// class declaration
//
using namespace std;
using namespace reco;

class SSBTreeManager;

class SSBGenInfor : public edm::EDAnalyzer {

   typedef std::vector<reco::GenParticle> GenParticleCollection;
   typedef std::vector<reco::GenJet>      GenJetCollection;
   typedef std::vector<pat::MET>          GenMETCollection;

   typedef std::vector<int>               vec_i;
   typedef std::map<int, vec_i>           map_i;
   typedef std::map<int, vec_i>::iterator map_i_it;
   typedef std::map<int, std::string>     map_s;
   typedef std::map<int, int>             map_ii;
   typedef std::map<int, int>::iterator   map_ii_it;

   public:
      explicit SSBGenInfor(const edm::ParameterSet&);
      ~SSBGenInfor();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

      void TestCout(const edm::Event&, SSBTreeManager*);
      void GenPar(const edm::Event&, SSBTreeManager*);
      void GenJet(const edm::Event&, SSBTreeManager*);
      void GenMET(const edm::Event&, SSBTreeManager*);

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // User Function 
      void InitializeGenPar();
      int IndexLinker ( map_i, int, int target_depth = 0, int target_index = -999, int target_pdgid = 0, int target_status = 0, bool PrintError = false, int LoopDepth = 0);
      void FillGenPar(int, int, int, int, int, GenParticleCollection::const_iterator, SSBTreeManager*);

      TTree* ssbtree;
      SSBTreeManager* ssbtreeManager;
      edm::Service<TFileService> ssbfs;

      bool isMC;

      edm::EDGetTokenT<GenEventInfoProduct>         genEvnInfoTag;
      edm::EDGetTokenT<reco::GenParticleCollection> genParInfoTag;
      edm::EDGetTokenT<reco::GenJetCollection>      genJetInfoTag;
      edm::EDGetTokenT<pat::METCollection>          genMETInfoTag;

      // variables for Event info. 
      int Event;
      int Run;
      int Lumi; 
      bool isData;    

      int genPar_index;
      int genJet_index;
      int genMET_index;

      map_i AllParMom;
      vec_i OriginalMom;
      map_i AllParDau;
      vec_i OriginalDau;
      map_i AllParInfo;
      vec_i pdgId_status;
      map_i SelParDau;
      vec_i SelectedDau;

      vec_i TreePar;
      vec_i FinalPar;
      vec_i SelectedPar;

      map_ii SelectedpdgId;

      map_s ParName;

//      int PythiaVersion;
      bool isSignal;
//      bool isMINIAOD;
      // ----------member data ---------------------------

};

#endif
