#ifndef SSBAnalyzer_h
#define SSBAnalyzer_h

//////////////////////////////////////
////// This code is for MiniAOD //////
//////////////////////////////////////

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"

// needed for TFileService
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

/// needed for pat candidate
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/Candidate/interface/Candidate.h"

// needed for PDF
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include <LHAPDF/LHAPDF.h>

/// needed for trigger
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

/// needed for pileup
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

/// needed for JetID and Jet Energy Uncertainty
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "DataFormats/Common/interface/Ptr.h"

/// needed for pat::Muon
#include "DataFormats/PatCandidates/interface/Muon.h"

/// needed for pat::Electron
#include "DataFormats/PatCandidates/interface/Electron.h"

/// electron conversion veto
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

/// electron Iso.
#include "DataFormats/RecoCandidate/interface/IsoDepositDirection.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h"

/// needed for MET
#include "DataFormats/PatCandidates/interface/MET.h"

/// Managing Tree
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBTreeManager.h"
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBPDFWeight.h"
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBIsoCal.h"

/// root
#include "TTree.h"

/// Generated Level Information
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBGenInfor.h"

using namespace std;

class TTree;

class SSBTreeManager;
class SSBGenInfor;
class SSBPDFWeight;
class SSBIsoCal;

class SSBAnalyzer : public edm::EDAnalyzer {
  typedef std::vector<PileupSummaryInfo> PileUpCollection;
  typedef std::vector<reco::Vertex> VertexCollection;
  typedef std::vector<pat::TriggerPath> TriggerPathCollection;
  typedef std::vector<reco::GenParticle> GenParticleCollection;
  typedef std::vector<reco::GenJet> GenJetCollection;
  typedef std::vector<reco::GenMET> GenMETCollection;
  typedef std::vector<pat::Muon> MuonCollection;
  typedef edm::View<pat::Electron> ElectronCollection;
  typedef std::vector<reco::Conversion> ConversionCollection;

  typedef std::vector<int> vec_i;
  typedef std::map<int, vec_i> map_i;
  typedef std::map<int, vec_i>::iterator map_i_it;
  typedef std::map<int, std::string> map_s;
  typedef std::map<int, int> map_ii;
  typedef std::map<int, int>::iterator map_ii_it;

public:
  explicit SSBAnalyzer(const edm::ParameterSet&);
  ~SSBAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  void GenPar(const edm::Event&, SSBTreeManager*);
  void GenJet(const edm::Event&, SSBTreeManager*);
  void GenMET(const edm::Event&, SSBTreeManager*);

private:
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  // User Function
  void InitializeGenPar();
  int IndexLinker(map_i,
                  int,
                  int target_depth = 0,
                  int target_index = -999,
                  int target_pdgid = 0,
                  int target_status = 0,
                  bool PrintError = false,
                  int LoopDepth = 0);
  void FillGenPar(int, int, int, int, int, GenParticleCollection::const_iterator, SSBTreeManager*);

  // ----------member data ---------------------------

  //
  TTree* ssbtree;
  SSBTreeManager* ssbtreeManager;
  SSBGenInfor* ssbgeninfor;
  SSBPDFWeight* pdfWeight;
  SSBIsoCal* isolation;

  // Input tags
  edm::Service<TFileService> ssbfs;

  bool isMC;

  // PDF
  edm::InputTag pdfTag;
  std::vector<std::string> pdfSets;
  bool pdfCent;
  bool pdfSys;

  edm::LumiReWeighting LumiWeights_;
  //      edm::InputTag vertexTag;
  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;

  edm::InputTag rhoTag;
  edm::InputTag pileupTag;

  edm::InputTag genParInfoTag;
  edm::InputTag genJetInfoTag;
  edm::InputTag genMETInfoTag;

  std::vector<std::string> triggerList;

  edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
  edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;

  //      edm::InputTag muonTag;
  /// Muon
  edm::EDGetTokenT<pat::MuonCollection> muonToken_;

  /// Electron
  edm::InputTag elecmvaIdNoTrTag;
  edm::InputTag elecmvaIdTrNoIPTag;
  edm::InputTag elecmvaIdTrTag;
  edm::InputTag beamSpotInputTag;
  edm::InputTag conversionsInputTag;
  //      edm::EDGetTokenT<pat::ElectronCollection> electronToken_;
  edm::EDGetTokenT<edm::View<pat::Electron> > electronToken_;
  // For Electron VID
  edm::EDGetTokenT<edm::ValueMap<bool> > eleVetoIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > eleLooseIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > eleMediumIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > eleTightIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > eleHEEPIdMapToken_;

  // ID decisions objects
  edm::EDGetTokenT<edm::ValueMap<bool> > mva_eleNonTrigMediumIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > mva_eleNonTrigTightIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > mva_eleTrigMediumIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > mva_eleTrigTightIdMapToken_;
  // MVA values and categories (optional)
  edm::EDGetTokenT<edm::ValueMap<float> > mvaValuesMapToken_;
  edm::EDGetTokenT<edm::ValueMap<int> > mvaCategoriesMapToken_;

  // Jet
  edm::EDGetTokenT<pat::JetCollection> jetToken_;
  edm::EDGetTokenT<pat::JetCollection> jetpuppiToken_;
  edm::EDGetTokenT<pat::JetCollection> jetTokenUp_;
  std::string jetUncTag;

  edm::ParameterSet pfLooseJetIDparam;
  edm::ParameterSet pfTightJetIDparam;
  std::string csvBJetTag;

  // Met
  edm::EDGetTokenT<pat::METCollection> metToken_;
  edm::EDGetTokenT<pat::METCollection> metnoHFToken_;
  edm::EDGetTokenT<pat::METCollection> metJetEnUpToken_;
  edm::EDGetTokenT<pat::METCollection> metJetEnDownToken_;

  /// index for TClonesArray
  int genPar_index;
  int genJet_index;
  int genMET_index;

  int muon_index;
  int jet_index;
  int MET_index;
  int ele_index;
  int photon_index;

  // variables for Event info.
  int Event;
  int Run;
  int Lumi;
  bool isData;

  // variables for Event Filter
  bool incons_mu_pt_flt_;
  bool greedy_mu_flt_;
  bool pfreco_mu_flt_;

  // variables for vertex
  double sumtrackptsquare;
  bool PV_Filter_;
  double vtx_x_;
  double vtx_y_;
  double vtx_z_;
  double vtx_x_e;
  double vtx_y_e;
  double vtx_z_e;

  double BVX;
  double BVY;
  double BVZ;

  // Rho
  double rho;

  // variables for Muons
  bool isLoose;
  bool isSoft;
  bool isTight;
  bool isHighPt;
  double trkIso03;
  double relIso03;
  //      double trkIso04;
  //      double relIso04;
  double PFIsodbeta03;
  double PFIsodbeta04;

  // variables for patJet
  bool isLoosejetid_pass;
  bool isTightjetid_pass;
  double jets_pt_;
  double jets_UncEnUp_;
  double jets_UncEnDown_;
  double jets_eta_;
  double jets_phi_;
  double jets_energy_;
  double jets_pdgid_;
  bool jets_isJet_;
  int jets_charge_;
  float jets_bDisc_;
  int jets_pfjetid_;
  int jets_mvapujetid_;
  float jets_mvapujet_;
  bool isLoosejetid_pass_;
  bool isTightjetid_pass_;

  // variables for electrons
  int isTightele;
  int isTightele_;
  //      double trkIso;
  double elecs_relIso03_;
  double elecs_relIso04_;
  double elecs_PFIsodbeta03_;
  double elecs_PFIsodbeta04_;
  double elecs_PFIsoRho03_;
  double elecs_PFIsoRho04_;

  double PfCharHadIso03_;
  double PfPUCharHadIso03_;
  double PfNeuHadIso03_;
  double PfGamIso03_;
  double PfCharHadIso04_;
  double PfPUCharHadIso04_;
  double PfNeuHadIso04_;
  double PfGamIso04_;
  bool elecs_IsoWrong_;

  double eles_pt_;
  double eles_eta_;
  double eles_phi_;
  double eles_energy_;
  int eles_pdgid_;
  int eles_charge_;

  int eles_eidLoose_;
  int eles_eidRobustHighEnergy_;
  int eles_eidRobustLoose_;
  int eles_eidRobustTight_;
  int eles_eidTight_;
  bool eles_SCB_Loose_;
  bool eles_SCB_Medium_;
  bool eles_SCB_Tight_;
  bool eles_SCB_Veto_;
  bool eles_chargeid_gsfctfpx_;
  bool eles_chargeid_gsfpx_;
  bool eles_chargeid_gsfctf_;
  double superclustereta_;

  double eles_gsfchar_;
  double eles_ctfchar_;
  double eles_spchar_;

  float mva1_;
  float mva2_;
  float mva3_;

  double gsftrack_dxy_;
  double gsftrack_dz_;
  double ctftrack_dxy_;
  double ctftrack_dz_;

  double effA03_;
  double effA04_;

  bool matchesConv;
  bool passconversionveto;
  bool passconversionveto1;

  float ooEmooP_;

  int nmhit_;

  // variables for pile up info.
  float NPU;
  int NPUin;
  int BX;

  // variables for pile up info.

  // variables for photon
  bool phos_pidLoose_;
  bool phos_pidTight_;

  //int trigName_;
  bool trigPass_;
  bool trigRun_;
  bool trigError_;

  // map for GenParticle
  std::map<int, int> idx_pdgId;
  std::map<int, int> idx_status;

  // information to make TLorentz
  std::map<int, double> idx_pt;
  std::map<int, double> idx_eta;
  std::map<int, double> idx_phi;
  std::map<int, double> idx_energy;

  // mother information
  std::map<int, int> idx_num_mother;
  std::map<int, int> idx_mother1;
  std::map<int, int> idx_mother2;

  // doughter information
  std::map<int, int> idx_num_doughter;
  std::map<int, int> idx_doughter1;
  std::map<int, int> idx_doughter2;

  // map for vertex
  std::map<double, reco::Vertex> map_vtx_sumptsquare;
  std::map<double, double> map_vtx_x;
  std::map<double, double> map_vtx_y;
  std::map<double, double> map_vtx_z;
  std::map<double, double> map_vtx_x_e;
  std::map<double, double> map_vtx_y_e;
  std::map<double, double> map_vtx_z_e;

  // map for Muoniso
  //      std::map<double, double> map_muons_trkIso03;

  // map for Jet

  // map for Electron
  //std::map<double, double> map_elecs_relIso03;

  // for Gen Infor

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

  int PythiaVersion;
  bool isSignal;
  bool isMINIAOD;
};

#endif
