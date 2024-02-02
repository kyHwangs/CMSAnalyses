// -*- C++ -*-
//
// Package:    SSBAnalyzer
// Class:      SSBAnalyzer
// 
/**\class SSBAnalyzer SSBAnalyzer.cc CMSAnalyses/SSBAnalyzer/plugins/SSBAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Seungkyu Ha, Sehwook Lee, Suyoung Choi 588 R-004, +41227678602
//         Created:  Mon Jun  2 11:21:34 CEST 2014
// $Id$
//
//


// system include files
#include <memory>
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBAnalyzer.h"
//#include "Math/VectorUtil.h"
#include <math.h>
using namespace std;
using namespace reco;
using namespace isodeposit;
using namespace pat;
//
// constants, enums and typedefs
//

//
// static data member definitions
//
//
// constructors and destructor
//
SSBAnalyzer::SSBAnalyzer(const edm::ParameterSet& iConfig)
:
effectiveAreas_( (iConfig.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath()),
effAreaChHadrons_((iConfig.getParameter<edm::FileInPath>("effAreaChHadFile")).fullPath() ),
effAreaNeuHadrons_((iConfig.getParameter<edm::FileInPath>("effAreaNeuHadFile")).fullPath() ),
effAreaPhotons_((iConfig.getParameter<edm::FileInPath>("effAreaPhoFile")).fullPath() )
{
   runPeriod = iConfig.getUntrackedParameter<std::string> ("RunYear", "");
   isMC = iConfig.getParameter<bool>("isMCTag");
   isSignal = iConfig.getParameter<bool>("isSignal");
   doFragSys = iConfig.getParameter<bool>("doFragsys");
// needed for PDF 
   pdfTag     = consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("PDFInfoTag"));
   fixPOWHEG_ = iConfig.getUntrackedParameter<std::string> ("FixPOWHEG", "");
   pdfSets    = iConfig.getParameter<std::vector<std::string>>("PDFSetNames") ;
   pdfCent    = iConfig.getParameter<bool>("PDFCent");
   pdfSys     = iConfig.getParameter<bool>("PDFSys");
// For METFilter 
   triggerBitsPAT_          = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bitsPat"));
//now do what ever initialization is needed
   if (isMC == true) 
   {
      genEvnInfoTag = consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genEvnTag"));
      genLHEInfoTag = consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("genLHETag"));
      genParInfoTag = consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParTag"));
      genJetInfoTag = consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genJetTag"));
      genMETInfoTag = consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("genMETTag"));
//      ssbgeninfor = new SSBGenInfor(igenConfig); // for GenInfor 
   }

// needed for Vertex
   vtxToken_  = consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("pvTag"));
   rhoTag     = consumes<double>(iConfig.getParameter<edm::InputTag>("RhoTag"));
   pileupTag  = consumes<PileUpCollection>(iConfig.getParameter<edm::InputTag>("puTag"));
// needed for Trigger 
   triggerList       = iConfig.getParameter<std::vector<std::string>>("trigList") ;
   triggerBits_      = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"));
   triggerPrescales_ = consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"));
// needed for Muon
   isMuSys           = iConfig.getParameter<bool>("ismuSysTag");
   muonToken_        = consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muTag"));
   muonEnUpToken_    = consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muEnUpTag"));
   muonEnDownToken_  = consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muEnDownTag"));
// needed for Electron
   electronToken_       = mayConsume<edm::View<pat::Electron> >(iConfig.getParameter<edm::InputTag>("eleTag"));
   electronPATToken_    = consumes<pat::ElectronCollection>(iConfig.getParameter<edm::InputTag>("electronPATInput"));
   electronEnUpToken_   = mayConsume<edm::View<pat::Electron> >(iConfig.getParameter<edm::InputTag>("eleEnUpTag"));
   electronEnDownToken_ = mayConsume<edm::View<pat::Electron> >(iConfig.getParameter<edm::InputTag>("eleEnDownTag"));
   beamSpotInputTag =  consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("bstag") );
   conversionsInputTag = consumes<ConversionCollection>(iConfig.getParameter<edm::InputTag>("convertag") );
// needed for Electron VID
   eleVetoIdMapToken_   = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleVetoIdMap"));
   eleLooseIdMapToken_  = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleLooseIdMap"));
   eleMediumIdMapToken_ = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMediumIdMap"));
   eleTightIdMapToken_  = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleTightIdMap"));
   eleHEEPIdMapToken_   = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleHEEPIdMap"));
// needed for Electron MVA
   mva_eleMediumMapToken_ = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("mva_eleMediumMap"));
   mva_eleTightMapToken_  = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("mva_eleTightMap"));
   mva_eleHZZIDMapToken_  = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("mva_eleHZZIDMap"));
   mvaValuesMapToken_              = consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("mvaValuesMap"));
   mvaCategoriesMapToken_          = consumes<edm::ValueMap<int> >(iConfig.getParameter<edm::InputTag>("mvaCategoriesMap"));
   mvaValuesHZZMapToken_              = consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("mvaValuesHZZMap"));
   mvaCategoriesHZZMapToken_          = consumes<edm::ValueMap<int> >(iConfig.getParameter<edm::InputTag>("mvaCategoriesHZZMap"));
// for Photon
   photonToken_       = mayConsume<edm::View<pat::Photon> >(iConfig.getParameter<edm::InputTag>("phoTag"));
   photonPATToken_    = consumes<pat::PhotonCollection>(iConfig.getParameter<edm::InputTag>("photonPATInput"));
   phoLooseIdMapToken_  = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("phoLooseIdMap"));
   phoMediumIdMapToken_ = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("phoMediumIdMap"));
   phoTightIdMapToken_  = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("phoTightIdMap"));
// needed for Photon MVA and variables //
   pho_mva_NonTrigTightIdMapToken_  = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("pho_mva_NontrigTightIdMap"));
   pho_mvaValuesMapToken_           = consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("pho_mvaValuesMap"));
   full5x5SigmaIEtaIEtaMapToken_   = consumes <edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("full5x5SigmaIEtaIEtaMap"));
   phoChargedIsolationToken_       = consumes <edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("phoChargedIsolation"));
   phoNeutralHadronIsolationToken_ = consumes <edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("phoNeutralHadronIsolation"));
   phoPhotonIsolationToken_        = consumes <edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("phoPhotonIsolation"));
   phoWorstChargedIsolationToken_  = consumes <edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("phoWorstChargedIsolation"));
   recHitCollectionEBToken_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("recHitCollectionEB"));
   recHitCollectionEEToken_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("recHitCollectionEE"));
// needed for Jet
   isJetPreCut         = iConfig.getParameter<bool>("isjtcutTag"); 
   jetToken_           = consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jtTag"));
   jetpuppiToken_      = consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jtpuppiTag"));
//   jetUncTag           = iConfig.getParameter<std::string>("jtuncTag");
//   jetUncTag           = iConfig.getParameter<edm::FileInPath>("jtuncTag").fullPath();
   payloadName_        = iConfig.getParameter<std::string>("PayLoadName");
   phi_resol_mc_file   = iConfig.getParameter<edm::FileInPath>("phiResolMCFile").fullPath();
   phi_resol_data_file = iConfig.getParameter<edm::FileInPath>("phiResolDataFile").fullPath();
   pt_resol_mc_file    = iConfig.getParameter<edm::FileInPath>("ptResolMCFile").fullPath();
   pt_resol_data_file  = iConfig.getParameter<edm::FileInPath>("ptResolDataFile").fullPath();
   pt_resolsf_file     = iConfig.getParameter<edm::FileInPath>("ptResolSFFile").fullPath();
   pfLooseJetIDparam   = iConfig.getParameter<edm::ParameterSet> ("PFLooseJetID");
   pfTightJetIDparam   = iConfig.getParameter<edm::ParameterSet> ("PFTightJetID");
   csvBJetTag          = iConfig.getParameter<std::string>("csvbjetTag");
   btagList            = iConfig.getParameter<std::vector<std::string>>("btagListTag") ;
// needed for MET
   metToken_               = consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("metTag")); // MC & DATA with JEC

// L1 Prefireing 
   //prefweight_token = consumes<double>(edm::InputTag("prefiringweight:NonPrefiringProb"));
  // prefweightup_token = consumes<double>(edm::InputTag("prefiringweight:NonPrefiringProbUp"));
   //prefweightdown_token = consumes<double>(edm::InputTag("prefiringweight:NonPrefiringProbDown"));
   prefweight_token = consumes< double >(edm::InputTag("prefiringweight:nonPrefiringProb"));
   prefweightup_token = consumes< double >(edm::InputTag("prefiringweight:nonPrefiringProbUp"));
   prefweightdown_token = consumes< double >(edm::InputTag("prefiringweight:nonPrefiringProbDown"));

}


SSBAnalyzer::~SSBAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
SSBAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Event = -999;
   Run = -999;
   Lumi = -999;
   isData = false;

   //cout << "runPeriod " << runPeriod << endl;

   /// Initailizing variable 
   ssbtreeManager->InitializeVariables(); 
   //ssbtreeManager->GenInitializeVariables();

   Event = iEvent.id().event();
   Run = iEvent.id().run();
   Lumi = iEvent.id().luminosityBlock();
   isData = iEvent.isRealData();

   ssbtreeManager->Fill( "Info_EventNumber", Event  );
   ssbtreeManager->Fill( "Info_RunNumber"  , Run    ); 
   ssbtreeManager->Fill( "Info_Luminosity" , Lumi   ); 
   ssbtreeManager->Fill( "Info_isData"     , isData ); 
  
   ///////////////////
   //// L1 Prefire ///
   ///////////////////
   edm::Handle< double > theprefweight;
   iEvent.getByToken(prefweight_token, theprefweight ) ;
   double _prefiringweight =(*theprefweight);

   edm::Handle< double > theprefweightup;
   iEvent.getByToken(prefweightup_token, theprefweightup ) ;
   double _prefiringweightup =(*theprefweightup);

   edm::Handle< double > theprefweightdown;
   iEvent.getByToken(prefweightdown_token, theprefweightdown ) ;
   double _prefiringweightdown =(*theprefweightdown);

   ssbtreeManager->Fill( "L1_PreFire_Central" , _prefiringweight );
   ssbtreeManager->Fill( "L1_PreFire_Up"      , _prefiringweightup);
   ssbtreeManager->Fill( "L1_PreFire_Down"    , _prefiringweightdown);

   /////////////////////////////
   /// METFilter Information ///
   /////////////////////////////


   /// To get metfilter information from trigger bit ///
   edm::Handle<edm::TriggerResults> MetFilter;
   iEvent.getByToken(triggerBitsPAT_, MetFilter);
   const edm::TriggerNames &metFiltnames = iEvent.triggerNames(*MetFilter); 
   for (unsigned int i = 0, n = MetFilter->size(); i < n; ++i)
   {
      //cout << "Test Name : " << metFiltnames.triggerName(i) << endl;
      ssbtreeManager->Fill( "METFilter_Name"    , metFiltnames.triggerName(i) );
      ssbtreeManager->Fill( "METFilter_isPass"  , MetFilter->accept(i)        );
      ssbtreeManager->Fill( "METFilter_isError" , MetFilter->error(i)         );
      ssbtreeManager->Fill( "METFilter_isRun"   , MetFilter->wasrun(i)        ); 
   }
   /// Additional METFilter Information /// this is old version ...
/*   edm::Handle<bool> ifilterbadGlobal;
   iEvent.getByToken(badGlobalMuonTaggerToken_, ifilterbadGlobal);
   bool filterbadGlobal = *ifilterbadGlobal;
   string globalbadmuonfilter = "Flag_BadGlobalMuonFilter";
   ssbtreeManager->Fill( "METFilterAdd_Name"    , globalbadmuonfilter );
   ssbtreeManager->Fill( "METFilterAdd_isPass"  , filterbadGlobal     );

   edm::Handle<bool> ifilterbadCloneGlobal;
   iEvent.getByToken(cloneGlobalMuonFilterToken_, ifilterbadCloneGlobal);
   bool filtercloneGlobal = *ifilterbadCloneGlobal;
   string cloneglobalmuonfilter = "Flag_CloneGlobalMuonFilter";
   ssbtreeManager->Fill( "METFilterAdd_Name"    , cloneglobalmuonfilter );
   ssbtreeManager->Fill( "METFilterAdd_isPass"  , filtercloneGlobal     );*/
//   cout << globalbadmuonfilter << " : " << filterbadGlobal << endl;
//   cout << cloneglobalmuonfilter << " : " << filtercloneGlobal << endl;

   /////////////////////////////////
   /// Only For MC Samples!!!!!! ///
   /////////////////////////////////

   if (!isData)
   {

      ///////////////////////
      /// PDF Information ///
      ///////////////////////

      Handle<GenEventInfoProduct> pdfstuff;
      iEvent.getByToken(pdfTag, pdfstuff);

      std::vector<double> v_pdf_nom;
      std::vector<double> v_pdf_w_nom;
      v_pdf_nom.clear();
      v_pdf_w_nom.clear();

      
      if (pdfCent)
      {
         pdfWeight->SetPDFSet(1);

         double Q = pdfstuff->pdf()->scalePDF;
         double _pdf1 = pdfstuff->pdf()->xPDF.first;
         double _pdf2 = pdfstuff->pdf()->xPDF.second;

         int id1 = pdfstuff->pdf()->id.first;
         int id2 = pdfstuff->pdf()->id.second;

         if (id1 == 21) id1=0;
         if (id2 == 21) id2=0;

         if (fixPOWHEG_!="") 
         {
            edm::Handle<reco::GenParticleCollection> genParticles;
            if (!iEvent.getByToken(genParInfoTag, genParticles)) 
            {
                  edm::LogError("PDFWeightProducer") << ">>> genParticles  not found: " ;
                  return;
            }
            unsigned int gensize = genParticles->size();
            double mboson = 0.;
            for(unsigned int i = 0; i<gensize; ++i) 
            {
               const reco::GenParticle& part = (*genParticles)[i];
               int status = part.status();
               //if (status!=3) continue;
               if (status > 30 || status < 20) continue;
               //std::cout << "status : " << status << std::endl;
               int id = part.pdgId();
               if (id!=23 && abs(id)!=24) continue;
               mboson = part.mass();
               break;
            }
            Q = sqrt(mboson*mboson+Q*Q);
            pdfWeight->SetScalePDF( Q );// Setting the Q ... 
//            LHAPDF::usePDFMember(1,0);
//            _pdf1 = LHAPDF::xfx(1, pdfstuff->pdf()->x.first , Q, pdfstuff->pdf()->id.first )/pdfstuff->pdf()->x.first;
//            _pdf2 = LHAPDF::xfx(1, pdfstuff->pdf()->x.second, Q, pdfstuff->pdf()->id.second)/pdfstuff->pdf()->x.second;
            pdfWeight->SetIncomingPartion1( id1, pdfstuff->pdf()->x.first,  _pdf1);
            pdfWeight->SetIncomingPartion2( id2, pdfstuff->pdf()->x.second, _pdf2);
            pdfWeight->SetPDFSet(1); // For Central 
            _pdf1 = pdfWeight->getCentralPDFWeight(1);
            _pdf2 = pdfWeight->getCentralPDFWeight(2);
//            pdfWeight->SetPDFSet(2); // For Central 
//            cout << "Test1 : " << pdfWeight->getCentralPDFWeight(1) << endl;
//            cout << "Test2 : " << pdfWeight->getCentralPDFWeight(2) << endl
            for ( unsigned int j = 1; j <= pdfSets.size(); ++j )
            {
               pdfWeight->SetPDFSet(j);
               double _p1 = pdfWeight->getCentralPDFWeight(1);
               double _p2 = pdfWeight->getCentralPDFWeight(2);
               v_pdf_nom.push_back( _p1*_p2);
            }
         }
         double _nominal = _pdf1*_pdf2;

         for ( unsigned int j = 0; j < v_pdf_nom.size(); ++j )
         {
            v_pdf_w_nom.push_back(v_pdf_nom[j]/_nominal);
         }
         ssbtreeManager->Fill( "PDFWeight_Cent"   , v_pdf_w_nom );
  
         pdfWeight->SetNominalWeight(_nominal);

         ssbtreeManager->Fill( "PDFWeight_Id1"       , id1                        );
         ssbtreeManager->Fill( "PDFWeight_Id2"       , id2                        );
         ssbtreeManager->Fill( "PDFWeight_BjorkenX1" , pdfstuff->pdf()->x.first   );
         ssbtreeManager->Fill( "PDFWeight_BjorkenX2" , pdfstuff->pdf()->x.second  );
         ssbtreeManager->Fill( "PDFWeight_Q"         , Q                          );
         ssbtreeManager->Fill( "PDFWeight_PDF1"      , _pdf1                      );
         ssbtreeManager->Fill( "PDFWeight_PDF2"      , _pdf2                      );


      }

      if (pdfSys)
      {
         for (unsigned int i = 1; i <= pdfSets.size(); ++i )
         {
            pdfWeight->SetPDFSet(i);
            if( i == 1 )
            { 
               ssbtreeManager->Fill( "PDFWeight_Cent_Up"   , pdfWeight->getSys("Up")   );
               ssbtreeManager->Fill( "PDFWeight_Cent_Down" , pdfWeight->getSys("Down") );
            }
            if( i == 2 )
            { 
               ssbtreeManager->Fill( "PDFWeight_Var1_Up"   , pdfWeight->getSys("Up")   );
               ssbtreeManager->Fill( "PDFWeight_Var1_Down" , pdfWeight->getSys("Down") );
            }
            if( i == 3 )
            { 
               ssbtreeManager->Fill( "PDFWeight_Var2_Up"   , pdfWeight->getSys("Up")   );
               ssbtreeManager->Fill( "PDFWeight_Var2_Down" , pdfWeight->getSys("Down") );
            }
         }
//         ssbtreeManager->Fill( "PDFWeight_Up"   , pdfWeight->getSys("Up")   );
//         ssbtreeManager->Fill( "PDFWeight_Down" , pdfWeight->getSys("Down") );
      }

      //////////////////////////////
      /// Generator Event Weight ///
      //////////////////////////////
      edm::Handle < GenEventInfoProduct > genEvtInfoProduct;
      //iEvent.getByLabel(genEvnInfoTag, genEvtInfoProduct);
      iEvent.getByToken(genEvnInfoTag, genEvtInfoProduct);
      {
         ssbtreeManager->Fill( "Gen_EventWeight"     , genEvtInfoProduct->weight() );  
      }
      ////////////////////////////////////
      /// NEW PDF AND LHE Event Weight ///
      ////////////////////////////////////
      edm::Handle < LHEEventProduct > genLHEInfoProduct;
      iEvent.getByToken(genLHEInfoTag, genLHEInfoProduct);
      if (genLHEInfoProduct.isValid())
      {
      //   cout << "genLHEInfoProduct->originalXWGTUP() "<< genLHEInfoProduct->originalXWGTUP() << endl;
         ssbtreeManager->Fill( "LHE_Central"     , genLHEInfoProduct->originalXWGTUP() );
         for ( unsigned int weightIndex = 0; weightIndex < genLHEInfoProduct->weights().size(); ++weightIndex ) 
         {
            ssbtreeManager->Fill( "LHE_Weight"     ,  genLHEInfoProduct->weights()[weightIndex].wgt );
            ssbtreeManager->Fill( "LHE_Id"     ,  atoi(genLHEInfoProduct->weights()[weightIndex].id.c_str()) );
            //std::cout << weightIndex << " ID : " << genLHEInfoProduct->weights()[weightIndex].id << "  wgt : " << genLHEInfoProduct->weights()[weightIndex].wgt << std::endl;
            /*cout << "genLHEInfoProduct->weights()[" << weightIndex << "].wgt " <<  genLHEInfoProduct->weights()[weightIndex].wgt <<
            " atoi(genLHEInfoProduct->weights()[" << weightIndex<< "].id.c_str()) " << atoi(genLHEInfoProduct->weights()[weightIndex].id.c_str())  <<
            endl;*/
         }
      }

      ///////////////////////////////////////////
      /// Generator Level Particle Informaton ///
      ///////////////////////////////////////////
      GenPar(iEvent, ssbtreeManager);
      GenJet(iEvent, ssbtreeManager);
      GenMET(iEvent, ssbtreeManager);
   }/// isDATA ///


/*
   /////////////////////////////////////////
   /// inconsistentMuonPFCandidateFilter ///
   /////////////////////////////////////////

   incons_mu_pt_flt_ = false;
   pfreco_mu_flt_ = false;
   int foMu = 0;
   int pfremu =0;
  
   Handle<PFCandColl> pfCandidates;
   iEvent.getByLabel(PFCandidatesTag,pfCandidates);
   for ( unsigned i=0; i<pfCandidates->size(); i++ )
   {
      const reco::PFCandidate & cand = (*pfCandidates)[i];
      if ( cand.particleId() != reco::PFCandidate::mu ){continue;}

      const reco::MuonRef muon = cand.muonRef();
      if ( fabs( muon->pt() - cand.pt() ) > 100 ) {pfremu++;}

      if ( cand.pt() < ptMinTag ){continue;}

      if (  muon->isTrackerMuon()
         && muon->isGlobalMuon()
         && fabs(muon->innerTrack()->pt()/muon->globalTrack()->pt() - 1) > maxPTDiffTag){ foMu++; }
   }
   if (foMu == 0){
      incons_mu_pt_flt_ = true;
   }
   if (pfremu == 0 ){ 
      pfreco_mu_flt_ = true; 
   }

   ssbtreeManager->Fill( "Filter_Inconsistent_MuonPt", incons_mu_pt_flt_ ); 
   ssbtreeManager->Fill( "Filter_PFReco_Muon", pfreco_mu_flt_);  

   //////////////////////////
   /// Greedy Muon Filter ///
   //////////////////////////
         
   greedy_mu_flt_ = false;
   int grMu = 0;
   for( unsigned i=0; i<pfCandidates->size(); i++ )
   {

      const reco::PFCandidate & cand = (*pfCandidates)[i];

//    if( cand.particleId() != 3 ) // not a muon
      if( cand.particleId() != reco::PFCandidate::mu ) {continue;}// not a muon
      if(!PFMuonAlgo::isIsolatedMuon( cand.muonRef() ) ) {continue;} // muon is not isolated

       double totalCaloEnergy = cand.rawEcalEnergy() +  cand.rawHcalEnergy();
       double eOverP = totalCaloEnergy/cand.p();

       if( eOverP >= eOverPMaxTag ) { grMu++; }

    }

    if (grMu == 0) 
    {  
       greedy_mu_flt_ = true;
    }
    ssbtreeManager->Fill( "Filter_Greedy_Muon" ,greedy_mu_flt_);
*/
                                            
   ///////////////////////////////////
   // Primary Vertices Information ///
   ///////////////////////////////////

   vtx_x_ = -9999.0;
   vtx_y_ = -9999.0;
   vtx_z_ = -9999.0;
   vtx_x_e = -9999.0;
   vtx_y_e = -9999.0;
   vtx_z_e = -9999.0;

   int numpvidx =0;
   std::vector<double> v_sumPtSquare;
   v_sumPtSquare.clear();
   std::vector<reco::Vertex> v_vertex;
   v_vertex.clear();

   Handle<reco::VertexCollection> vertices;
   iEvent.getByToken(vtxToken_, vertices);

   for ( const reco::Vertex &itPV : *vertices)
   {
      PV_Filter_ = false;

      if ( !itPV.isFake() && itPV.ndof() > 4.0 && itPV.position().Rho() < 2. && abs(itPV.z()) < 24. )
      {
         PV_Filter_ = true;
         ssbtreeManager->Fill( "Filter_PV" , PV_Filter_ );

      }
      else 
      {  
         ssbtreeManager->Fill( "Filter_PV" , PV_Filter_ );
      }
   
      v_vertex.push_back(itPV);

      vtx_x_  = itPV.x(); 
      vtx_y_  = itPV.y();
      vtx_z_  = itPV.z();
      vtx_x_e = itPV.xError();
      vtx_y_e = itPV.yError();
      vtx_z_e = itPV.zError();

      ssbtreeManager->Fill( "Vertex_X", vtx_x_ );
      ssbtreeManager->Fill( "Vertex_Y", vtx_y_ );
      ssbtreeManager->Fill( "Vertex_Z", vtx_z_ );
      ssbtreeManager->Fill( "Vertex_X_Error", vtx_x_e );
      ssbtreeManager->Fill( "Vertex_Y_Error", vtx_y_e );
      ssbtreeManager->Fill( "Vertex_Z_Error", vtx_z_e );

      numpvidx++;
   }
   ssbtreeManager->Fill( "PV_Count", numpvidx );


/*
   for (unsigned int i=0; i < v_sumPtSquare.size(); i++)
   {


   }
*/

   /////////////////////////
   ///  Rho Information  ///
   /////////////////////////

   Handle<double> rhoHandle;
   iEvent.getByToken(rhoTag, rhoHandle);
//   iEvent.getByLabel(rhoTag, rhoHandle);
   rho = -999; 
   if(rhoHandle.isValid()) 
   {
      rho = *(rhoHandle.product());
   } else {
      cout << "Rho is invalid!!!" << endl;

   }
   
   ssbtreeManager->Fill( "Rho"     , rho  ); 
   //////////////////////////
   /// Pileup Information ///
   //////////////////////////

   NPU = -1;
   NPUin = 0;
   BX = -999;
   if (!isData)
   { 

       Handle<PileUpCollection> pileup;
       //iEvent.getByLabel( pileupTag, pileup ); 
       iEvent.getByToken( pileupTag, pileup ); 
       for (PileUpCollection::const_iterator itpu = pileup->begin() ; itpu != pileup->end(); itpu++)
       {
           BX = (*itpu).getBunchCrossing();
           if (BX == 0){
           NPU = (*itpu).getTrueNumInteractions();
           NPUin += (*itpu).getPU_NumInteractions();
           ssbtreeManager->Fill( "PileUp_Count_Intime"     , NPU  ); 
           ssbtreeManager->Fill( "PileUp_Count_Interaction", NPUin); 
           }
       }

   }

   /////////////////////////
   ///Trigger Information///
   /////////////////////////

   edm::Handle<edm::TriggerResults> triggerBits;                                                                                        
   iEvent.getByToken(triggerBits_, triggerBits);                                                                                        
   
   edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;                                                                           
   iEvent.getByToken(triggerPrescales_, triggerPrescales);                                                                              
   
   const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);                                                                  
   for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i)                                                                        
   {                                                                                                                                    
      //cout << "Trigger : " << names.triggerName(i) << endl;
      for (unsigned int j =0; j < triggerList.size() ;j++)                                                                              
      {                                                                                                                                 
         
         trigPass_ = false;
         trigError_ = false;                                                                                                            
         trigRun_ =false;
         unsigned int trigPreScale_ = 0;
         if (TString(names.triggerName(i)).Contains(triggerList.at(j)) )                                                                
         {  
            trigPass_ = triggerBits->accept(i);                                                                                         
            trigError_ = triggerBits->error(i);                                                                                         
            trigRun_ = triggerBits->wasrun(i) ;
            trigPreScale_ = triggerPrescales->getPrescaleForIndex(i);                                                                   
            
            ssbtreeManager->Fill( "Trigger_Name"    , names.triggerName(i) );
            ssbtreeManager->Fill( "Trigger_isPass"  , trigPass_            );
            ssbtreeManager->Fill( "Trigger_PreScale", trigPreScale_        );
            ssbtreeManager->Fill( "Trigger_isError" , trigError_           );
            ssbtreeManager->Fill( "Trigger_isRun"   , trigRun_             ); 
         }                                                                                                                              
      
      } // j                                                                                                                            
     
   } // i  

   /////////////////////////
   /// Muon Information ////
   /////////////////////////

   Handle<pat::MuonCollection> muons;
   iEvent.getByToken(muonToken_, muons);

   muon_index=0; 
   for (const pat::Muon &muon : *muons)
   {

      isLoose = false;
      isSoft = false;
      isTight = false;
      isMediumPrompt = false;
      isMedium = false;
      isHighPt = false;
      isHighTrkPt = false;
      goodGlob = false;
      //isMedium2016 = false;
      numTrackLayer = -1;
      isPassPFIsoVeryLoose=false;
      isPassPFIsoLoose=false;    
      isPassPFIsoMedium=false;   
      isPassPFIsoTight=false;
      isPassPFIsoVeryTight=false;
      isPassTkIsoLoose=false;
      isPassTkIsoTight=false;

      muTuneP_pt  = -999;
      muTuneP_eta = -999;
      muTuneP_phi = -999;
      muTuneP_charge = -999;

      ///////////////
      /// Muon ID ///
      ///////////////
      // https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonSelection#Muon_selectors_Since_9_4_X//
      // old method... recomendation is to use MuonSelector...///
/*      isLoose = muon.isLooseMuon() ;
      isMedium = muon.isMediumMuon() ;
      isSoft =  muon.isSoftMuon( v_vertex[0] );
      isTight = muon.isTightMuon( v_vertex[0] );
      isHighPt = muon.isHighPtMuon( v_vertex[0] );*/
      isLoose        = muon.passed(reco::Muon::CutBasedIdLoose);
      isMedium       = muon.passed(reco::Muon::Muon::CutBasedIdMedium);
      isMediumPrompt = muon.passed(reco::Muon::Muon::CutBasedIdMediumPrompt); //reco::Muon::isMediumMuon and dz<0.1 and dxy< 0.02
      isTight        = muon.passed(reco::Muon::CutBasedIdTight);
      isHighPt       = muon.passed(reco::Muon::CutBasedIdGlobalHighPt);
      isHighTrkPt    = muon.passed(reco::Muon::CutBasedIdTrkHighPt);
      isSoft         = muon.passed(reco::Muon::SoftCutBasedId);

      //cout << "03 " << muon.pfIsolationR03().sumChargedHadronPt << "   " << muon.pfIsolationR03().sumNeutralHadronEt << "   " 
      //<< muon.pfIsolationR03().sumPhotonEt << "   " << muon.pfIsolationR03().sumPUPt << endl;
      /// Medium ID - 2016 /// Remove this requirements after Legacy Sample ...
/*      goodGlob = muon.isGlobalMuon() && 
                 muon.globalTrack()->normalizedChi2() < 3 && 
                 muon.combinedQuality().chi2LocalPosition < 12 && 
                 muon.combinedQuality().trkKink < 20;
      isMedium2016 = muon::isLooseMuon(muon) && 
                     muon.innerTrack()->validFraction() > 0.49 && 
                     //muon.innerTrack()->validFraction() > 0.8 && 
                     muon::segmentCompatibility(muon) > (goodGlob ? 0.303 : 0.451); 
*/

      // Calculation of muon isolation (rel, PFiso) ....
      relIso03 = isolation->MuonRelTrkIso( muon.isolationR03().sumPt, muon.pt() );

      PFIsodbeta03 = isolation->PFIsodBeta( muon.pfIsolationR03().sumChargedHadronPt, muon.pfIsolationR03().sumNeutralHadronEt, 
                                                muon.pfIsolationR03().sumPhotonEt, muon.pfIsolationR03().sumPUPt, muon.pt() ,0.5);

      PFIsodbeta04 = isolation->PFIsodBeta( muon.pfIsolationR04().sumChargedHadronPt, muon.pfIsolationR04().sumNeutralHadronEt, 
                                                muon.pfIsolationR04().sumPhotonEt, muon.pfIsolationR04().sumPUPt, muon.pt() ,0.5);

      /// Calculation of muon isolation using muonselector /// recomanded method for UL RUn2 ...
      //cout << "muon.passed PFIso : " << muon.passed(reco::Muon::PFIsoTight) << endl;
      isPassPFIsoVeryLoose     = muon.passed(reco::Muon::PFIsoVeryLoose); // Relative PF-isolation (delta beta corrected, 0.4 cone) <0.40 
      isPassPFIsoLoose         = muon.passed(reco::Muon::PFIsoLoose); // Relative PF-isolation (delta beta corrected, 0.4 cone) <0.25
      isPassPFIsoMedium        = muon.passed(reco::Muon::PFIsoMedium); // Relative PF-isolation (delta beta corrected, 0.4 cone) <0.20
      isPassPFIsoTight         = muon.passed(reco::Muon::PFIsoTight); // Relative PF-isolation (delta beta corrected, 0.4 cone) <0.15
      isPassPFIsoVeryTight     = muon.passed(reco::Muon::PFIsoVeryTight); // Relative PF-isolation (delta beta corrected, 0.4 cone) <0.10
      isPassPFIsoVeryVeryTight = muon.passed(reco::Muon::PFIsoVeryVeryTight); // Relative PF-isolation (delta beta corrected, 0.4 cone) <0.05
      isPassTkIsoLoose         = muon.passed(reco::Muon::TkIsoLoose); // Relative Tracker isolation (0.3 cone) <0.10
      isPassTkIsoTight         = muon.passed(reco::Muon::TkIsoTight); // Relative Tracker isolation (0.3 cone) <0.05
      // TO-DO //MiniIso and MVA ID will be added ...  

      //cout << " iMu->passed(reco::Muon::PFMvaTight) : " << muon.passed(reco::Muon::MvaTight)<< endl;

      if (muon.innerTrack().isNonnull() && muon.innerTrack().isAvailable()  ){
         numTrackLayer = muon.innerTrack()->hitPattern().trackerLayersWithMeasurement();
      }
      // get random number generator
      edm::Service<edm::RandomNumberGenerator> rng;
      if (!rng.isAvailable()) edm::LogError("MUON : random number generator is missing !");
      CLHEP::HepRandomEngine& engine = rng->getEngine(iEvent.streamID());
      double mcShift1 = engine.flat();
      double mcShift2 = engine.flat();
      double gen_mupt = 0.0;
      double gen_mueta = 0.0;
      double gen_muphi = 0.0;
      double gen_muenergy = 0.0;
      //cout << "mcShift1 : " << mcShift1 << " mcShift2: " << mcShift2 << endl;
      if (muon.genParticleRef().isNonnull()){
         //cout << "muon pdgId : "<< muon.genParticle()->pdgId() << " pt : " << muon.genParticle()->pt() << " Reco Pt : " << muon.pt() << endl;
         gen_mupt = muon.genParticle()->pt(); 
         gen_mueta = muon.genParticle()->eta(); 
         gen_muphi = muon.genParticle()->phi(); 
         gen_muenergy = muon.genParticle()->energy(); 
      }

      // TuneP //
      if (muon.tunePMuonBestTrack().isNonnull()){
         //cout << " tunePMuonBestTrack() charge :" << muon.tunePMuonBestTrack()->charge()<< endl;
         muTuneP_pt = muon.tunePMuonBestTrack()->pt();
         muTuneP_eta = muon.tunePMuonBestTrack()->eta();
         muTuneP_phi = muon.tunePMuonBestTrack()->phi();
         muTuneP_charge = muon.tunePMuonBestTrack()->charge();
      }

      ssbtreeManager->Fill( "Muon", muon.pt(), muon.eta(), muon.phi(), muon.energy(), muon_index);
      ssbtreeManager->Fill( "GenMuon", gen_mupt, gen_mueta, gen_muphi, gen_muenergy, muon_index);
      ssbtreeManager->Fill( "Muon_isLoose"       , isLoose          );
      ssbtreeManager->Fill( "Muon_isMedium"      , isMedium         );
      //ssbtreeManager->Fill( "Muon_isMedium2016"  , isMedium2016     );
      ssbtreeManager->Fill( "Muon_isSoft"        , isSoft           );
      ssbtreeManager->Fill( "Muon_isTight"       , isTight          );
      ssbtreeManager->Fill( "Muon_isHighPt"      , isHighPt         );
      ssbtreeManager->Fill( "Muon_rand1"         , mcShift1         );
      ssbtreeManager->Fill( "Muon_rand2"         , mcShift2         );
      ssbtreeManager->Fill( "Muon_relIso03"      , relIso03         );
      ssbtreeManager->Fill( "Muon_PFIsodBeta03"  , PFIsodbeta03     );
      ssbtreeManager->Fill( "Muon_PFIsodBeta04"  , PFIsodbeta04     );
      ssbtreeManager->Fill( "Muon_pdgId"         , muon.pdgId()     );
      ssbtreeManager->Fill( "Muon_Charge"        , muon.charge()    );
      ssbtreeManager->Fill( "Muon_trackerLayers" , numTrackLayer    );
      ssbtreeManager->Fill( "Muon_isPFIsoVeryLoose" , isPassPFIsoVeryLoose     );
      ssbtreeManager->Fill( "Muon_isPFIsoLoose" , isPassPFIsoLoose     );
      ssbtreeManager->Fill( "Muon_isPFIsoMedium" , isPassPFIsoMedium     );
      ssbtreeManager->Fill( "Muon_isPFIsoTight" , isPassPFIsoTight     );
      ssbtreeManager->Fill( "Muon_isPFIsoVeryTight" , isPassPFIsoVeryTight     );
      ssbtreeManager->Fill( "Muon_isPFIsoVeryVeryTight" , isPassPFIsoVeryVeryTight     );
      ssbtreeManager->Fill( "Muon_tuneP_Pt"             , muTuneP_pt     );
      ssbtreeManager->Fill( "Muon_tuneP_Eta"            , muTuneP_eta    );
      ssbtreeManager->Fill( "Muon_tuneP_Phi"            , muTuneP_phi    );
      ssbtreeManager->Fill( "Muon_tuneP_Charge"         , muTuneP_charge );

      muon_index++;

   }
   ssbtreeManager->Fill( "Muon_Count", muon_index );

   ////////////////////////////
   /////// Electron infor//////
   ////////////////////////////
   ele_index=0;
   edm::Handle<edm::View<pat::Electron> >elecs;
   iEvent.getByToken(electronToken_, elecs);
   for (edm::View<pat::Electron>::const_iterator iEle = elecs->begin(); iEle != elecs->end(); ++iEle) {

      eles_pt_ = -9999.0;
      eles_eta_ = -9999.0;
      eles_phi_ = -9999.0;
      eles_energy_ = -9999.0;
      superclustereta_ = -999.0;

      elecs_relIso03_ = -999.0;
      elecs_relIso04_ = -999.0;
      elecs_PFIsodbeta03_ = -999.0;
      elecs_PFIsodbeta04_ = -999.0;
      elecs_PFIsoRho03_ = -999.0;
      elecs_PFIsoRho04_ = -999.0;

      PfCharHadIso03_   = -999.0;
      PfPUCharHadIso03_ = -999.0;
      PfNeuHadIso03_    = -999.0;
      PfGamIso03_       = -999.0;
      PfCharHadIso04_   = -999.0;
      PfPUCharHadIso04_ = -999.0;
      PfNeuHadIso04_    = -999.0;
      PfGamIso04_       = -999.0;

      elecs_IsoWrong_ = false;

      eles_SCB_Loose_  = false;
      eles_SCB_Medium_ = false;
      eles_SCB_Tight_  = false;
      eles_SCB_Veto_   = false;

      eles_pdgid_ = 0;
      eles_charge_ = -999;
      eles_chargeid_gsfctfpx_ = false;
      eles_chargeid_gsfpx_ = false;
      eles_chargeid_gsfctf_ = false;
      eles_gsfchar_ = -999;
      eles_ctfchar_ = -999;
      eles_spchar_ = -999;

      gsftrack_dxy_ = -999.0;
      gsftrack_dz_ = -999.0;
      ctftrack_dxy_ = -999.0;
      ctftrack_dz_ = -999.0; 

      mva1_ = -999.0;
      mva2_ = -999.0;
      mva3_ = -999.0;

      nmhit_ = 999;
      matchesConv = false;
      passconversionveto = false;

      eleScale_stat_up_ = false;
      eleScale_stat_dn_ = false;
      eleScale_syst_up_ = false;
      eleScale_syst_dn_ = false;
      eleScale_gain_up_ = false;
      eleScale_gain_dn_ = false;
      eleResol_rho_up_  = false;
      eleResol_rho_dn_  = false;
      eleResol_phi_up_  = false;
      eleResol_phi_dn_  = false;


      auto corrP4  = iEle->p4() * iEle->userFloat("ecalTrkEnergyPostCorr") / iEle->energy();
      //cout << "corrP4" << corrP4 << endl;
      eles_pt_ = corrP4.Pt();
      eles_eta_ = corrP4.Eta();
      eles_phi_ = corrP4.Phi();
      eles_energy_ = corrP4.energy();

      raweles_pt_ = iEle->pt();
      raweles_eta_ = iEle->eta();
      raweles_phi_ = iEle->phi();
      raweles_energy_ = iEle->energy();
      eles_pdgid_ = iEle->pdgId();
      eles_charge_ = iEle->charge();

      //cout << "eles_pt_ : " << eles_pt_ << " raweles_pt_ : " << raweles_pt_ << endl;


      ssbtreeManager->Fill("Elec", eles_pt_,eles_eta_,eles_phi_,eles_energy_,ele_index );
      ssbtreeManager->Fill("RawElec", raweles_pt_,raweles_eta_,raweles_phi_,raweles_energy_,ele_index );

      /// Syst. Unc ///
      eleScale_stat_up_ = iEle->userFloat("energyScaleStatUp");
      eleScale_stat_dn_ = iEle->userFloat("energyScaleStatDown");
      eleScale_syst_up_ = iEle->userFloat("energyScaleSystUp");
      eleScale_syst_dn_ = iEle->userFloat("energyScaleSystDown");
      eleScale_gain_up_ = iEle->userFloat("energyScaleGainUp");
      eleScale_gain_dn_ = iEle->userFloat("energyScaleGainDown");
      eleResol_rho_up_  = iEle->userFloat("energySigmaRhoUp");
      eleResol_rho_dn_  = iEle->userFloat("energySigmaRhoDown");
      eleResol_phi_up_  = iEle->userFloat("energySigmaPhiUp");
      eleResol_phi_dn_  = iEle->userFloat("energySigmaPhiDown");

      /*cout << "eleScale_stat_up_ : " << eleScale_stat_up_ << endl;
      cout << "eleScale_stat_dn_ : " << eleScale_stat_dn_ << endl;
      cout << "eleScale_syst_up_ : " << eleScale_syst_up_ << endl;
      cout << "eleScale_syst_dn_ : " << eleScale_syst_dn_ << endl;
      cout << "eleScale_gain_up_ : " << eleScale_gain_up_ << endl;
      cout << "eleScale_gain_dn_ : " << eleScale_gain_dn_ << endl;
      cout << "eleResol_rho_up_ : " << eleResol_rho_up_ << endl;
      cout << "eleResol_rho_dn_ : " << eleResol_rho_dn_ << endl;
      cout << "eleResol_phi_up_ : " << eleResol_phi_up_ << endl;
      cout << "eleResol_phi_dn_ : " << eleResol_phi_dn_ << endl;*/

      ssbtreeManager->Fill( "Elec_Scale_StatUp"    , eleScale_stat_up_);
      ssbtreeManager->Fill( "Elec_Scale_StatDown"  , eleScale_stat_dn_);
      ssbtreeManager->Fill( "Elec_Scale_SystUp"    , eleScale_syst_up_);
      ssbtreeManager->Fill( "Elec_Scale_SystDown"  , eleScale_syst_dn_);
      ssbtreeManager->Fill( "Elec_GainUp"          , eleScale_gain_up_);
      ssbtreeManager->Fill( "Elec_GainDown"        , eleScale_gain_dn_);
      ssbtreeManager->Fill( "Elec_GainUp"          , eleResol_rho_up_ );
      ssbtreeManager->Fill( "Elec_GainDown"        , eleResol_rho_dn_ );
      ssbtreeManager->Fill( "Elec_GainUp"          , eleResol_phi_up_ );
      ssbtreeManager->Fill( "Elec_GainDown"        , eleResol_phi_dn_ );

      elecs_relIso03_ = isolation->ElecRelIso( iEle->dr03HcalTowerSumEt(), iEle->dr03EcalRecHitSumEt(), iEle->dr03TkSumPt(), iEle->et() );
      elecs_relIso04_ = isolation->ElecRelIso( iEle->dr04HcalTowerSumEt(), iEle->dr04EcalRecHitSumEt(), iEle->dr04TkSumPt(), iEle->et() );

      reco::GsfElectron::PflowIsolationVariables ele_pfIso = iEle->pfIsolationVariables();
      elecs_PFIsodbeta03_ = isolation->PFIsodBeta(ele_pfIso.sumChargedHadronPt, ele_pfIso.sumNeutralHadronEt, ele_pfIso.sumPhotonEt, ele_pfIso.sumPUPt, iEle->pt() , 0.5);

      if (   iEle->neutralHadronIso() !=  iEle->userIsolation(pat::PfNeutralHadronIso) )
      {
         cout << "neutralHadron 04 : " << iEle->neutralHadronIso() << endl;
         cout << "userIsoneutralHadron 04 : " <<  iEle->userIsolation(pat::PfNeutralHadronIso) << endl;
      }
      if ( iEle->chargedHadronIso() != iEle->userIsolation(pat::PfChargedHadronIso) )
      {
         cout << "iEle->chargedHadronIso 04 : " << iEle->chargedHadronIso()  << endl;
         cout << "userPfChargedHadronIso 04 : " <<  iEle->userIsolation(pat::PfChargedHadronIso) << endl;
      }
      if ( iEle->puChargedHadronIso() != iEle->userIsolation(pat::PfPUChargedHadronIso) )
      {
         cout << "iEle->puChargedHadronIso 04 : " << iEle->puChargedHadronIso() << endl;
         cout << "userpuChargedHadronIso   04 : " << iEle->userIsolation(pat::PfPUChargedHadronIso) << endl;
      }

      if ( iEle->photonIso() != iEle->userIsolation(pat::PfGammaIso) )
      {
         cout << "iEle->puChargedHadronIso 04 : " << iEle->photonIso() << endl;
         cout << "userpuChargedHadronIso   04 : " << iEle->userIsolation(pat::PfGammaIso) << endl;
      }
      PfCharHadIso04_   = iEle->userIsolation(pat::PfChargedHadronIso);
      PfPUCharHadIso04_ = iEle->userIsolation(pat::PfPUChargedHadronIso);
      PfNeuHadIso04_    = iEle->userIsolation(pat::PfNeutralHadronIso);
      PfGamIso04_       = iEle->userIsolation(pat::PfGammaIso);


      elecs_PFIsodbeta04_ = isolation->PFIsodBeta(iEle->userIsolation(pat::PfChargedHadronIso), iEle->userIsolation(pat::PfNeutralHadronIso),
                                      iEle->userIsolation(pat::PfGammaIso), iEle->userIsolation(pat::PfPUChargedHadronIso), iEle->pt() , 0.5);

      superclustereta_ =  iEle->superCluster()->eta();
      float eA = effectiveAreas_.getEffectiveArea(fabs(superclustereta_));

      effA03_ = isolation->EffArea2015( superclustereta_ );
      effA04_ = isolation->EffArea2015( superclustereta_ );

      elecs_PFIsoRho03_ = isolation->PFIsoRho( ele_pfIso.sumChargedHadronPt, ele_pfIso.sumNeutralHadronEt, ele_pfIso.sumPhotonEt, rho, eA, iEle->pt() );

      elecs_PFIsoRho04_ = isolation->PFIsoRho( iEle->userIsolation(pat::PfChargedHadronIso), iEle->userIsolation(pat::PfNeutralHadronIso), iEle->userIsolation(pat::PfGammaIso), rho, effA04_, iEle->pt() );

      ssbtreeManager->Fill( "Elec_relIso03"    , elecs_relIso03_     );
      ssbtreeManager->Fill( "Elec_relIso04"    , elecs_relIso04_     );
      ssbtreeManager->Fill( "Elec_PFIsodBeta03", elecs_PFIsodbeta03_ );
      ssbtreeManager->Fill( "Elec_PFIsodBeta04", elecs_PFIsodbeta04_ );
      ssbtreeManager->Fill( "Elec_PFIsoRho03"  , elecs_PFIsoRho03_   );
      ssbtreeManager->Fill( "Elec_PFIsoRho04"  , elecs_PFIsoRho04_   );

      if (!(iEle->gsfTrack().isNull()))
      {

         //nmhit_ = iEle->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
         nmhit_ = iEle->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS);
         gsftrack_dxy_ = iEle->gsfTrack()->dxy( v_vertex[0].position() );
         gsftrack_dz_  = iEle->gsfTrack()->dz( v_vertex[0].position() );
         eles_gsfchar_ = iEle->gsfTrack()->charge();

      }

      if (!(iEle->closestCtfTrackRef().isNull()))
      {

         ctftrack_dxy_ = iEle->closestCtfTrackRef()->dxy( v_vertex[0].position() );
         ctftrack_dz_  = iEle->closestCtfTrackRef()->dz( v_vertex[0].position() );
         eles_ctfchar_ = iEle->closestCtfTrackRef()->charge();

      }

      ssbtreeManager->Fill( "Elec_Charge_GsfTr" , eles_gsfchar_ );
      ssbtreeManager->Fill( "Elec_Track_GsfdXY" , gsftrack_dxy_ );
      ssbtreeManager->Fill( "Elec_Track_GsfdZ"  , gsftrack_dz_  );
      ssbtreeManager->Fill( "Elec_Inner_Hit"    , nmhit_        );

      ssbtreeManager->Fill( "Elec_Charge_CtfTr", eles_ctfchar_ );
      ssbtreeManager->Fill( "Elec_Track_CtfdXY", ctftrack_dxy_ );
      ssbtreeManager->Fill( "Elec_Track_CtfdZ" , ctftrack_dz_  );

      eles_spchar_ = iEle->scPixCharge();
      ssbtreeManager->Fill( "Elec_Charge_Px", eles_spchar_);

      if (iEle->isGsfCtfScPixChargeConsistent())
      {
         eles_chargeid_gsfctfpx_ = true;
      }

      if (iEle->isGsfCtfChargeConsistent())
      {
         eles_chargeid_gsfctf_ = true;
      }

      if (iEle->isGsfScPixChargeConsistent())
      {
         eles_chargeid_gsfpx_ = true;
      }

      /////////////////////
      //// Electron ID ////
      /////////////////////
      eles_SCB_Veto_   = iEle->electronID("cutBasedElectronID-Fall17-94X-V2-veto");
      eles_SCB_Loose_  = iEle->electronID("cutBasedElectronID-Fall17-94X-V2-loose");
      eles_SCB_Medium_ = iEle->electronID("cutBasedElectronID-Fall17-94X-V2-medium");
      eles_SCB_Tight_  = iEle->electronID("cutBasedElectronID-Fall17-94X-V2-tight");

      /*cout << "eles_SCB_Veto_ : " << eles_SCB_Veto_ << endl;
      cout << "eles_SCB_Loose_ : " << eles_SCB_Loose_ << endl;
      cout << "eles_SCB_Medium_ : " << eles_SCB_Medium_ << endl;
      cout << "eles_SCB_Tight_ : " << eles_SCB_Tight_ << endl;*/

      ssbtreeManager->Fill( "Elec_SCB_Loose"          , eles_SCB_Loose_         );
      ssbtreeManager->Fill( "Elec_SCB_Medium"         , eles_SCB_Medium_        );
      ssbtreeManager->Fill( "Elec_SCB_Tight"          , eles_SCB_Tight_         );
      ssbtreeManager->Fill( "Elec_SCB_Veto"           , eles_SCB_Veto_          );

      bool isPassmvaTight = iEle->electronID("mvaEleID-Fall17-iso-V2-wp90");
      bool isPassmvaMedium = iEle->electronID("mvaEleID-Fall17-iso-V2-wp80");
      bool isPassmvaLoose = iEle->electronID("mvaEleID-Fall17-iso-V2-wpLoose");

      bool isPassmvaTightNonIso  =  iEle->electronID("mvaEleID-Fall17-noIso-V2-wp90");
      bool isPassmvaMediumNonIso  =  iEle->electronID("mvaEleID-Fall17-noIso-V2-wp80");
      bool isPassmvaLooseNonIso = iEle->electronID("mvaEleID-Fall17-noIso-V2-wpLoose");

      bool isPassHEEP   = iEle->electronID("heepElectronID-HEEPV70");
      //cout << " userFloat(\"ElectronMVAEstimatorRun2Fall17IsoV2Values\") : " << iEle->userFloat("ElectronMVAEstimatorRun2Fall17IsoV2Values") << endl;
      float elemva_iso    = iEle->userFloat("ElectronMVAEstimatorRun2Fall17IsoV2Values");
      float elemva_noniso = iEle->userFloat("ElectronMVAEstimatorRun2Fall17NoIsoV2Values");

      ssbtreeManager->Fill( "Elec_MVA_Loose"          , isPassmvaLoose         );
      ssbtreeManager->Fill( "Elec_MVA_Medium"         , isPassmvaMedium        );
      ssbtreeManager->Fill( "Elec_MVA_Tight"          , isPassmvaTight         );

      ssbtreeManager->Fill( "Elec_MVA_NonIso_Loose"          , isPassmvaLooseNonIso         );
      ssbtreeManager->Fill( "Elec_MVA_NonIso_Medium"         , isPassmvaMediumNonIso        );
      ssbtreeManager->Fill( "Elec_MVA_NonIso_Tight"          , isPassmvaTightNonIso         );
      ssbtreeManager->Fill( "Elec_SCB_HEEP"          , isPassHEEP          );
      ssbtreeManager->Fill( "Elec_MVA_IsoV"          , elemva_iso          );
      ssbtreeManager->Fill( "Elec_MVA_nonIsoV"       , elemva_noniso          );

      passconversionveto = iEle->passConversionVeto();// pat conversion veto
      ssbtreeManager->Fill( "Elec_Conversion"          , passconversionveto          );



      ooEmooP_ =  (1.0/iEle->ecalEnergy())*(1.0-iEle->eSuperClusterOverP()) ;

      ele_index++;
   }
   ssbtreeManager->Fill( "Elec_Count" , ele_index );

   //////////////////////////
   /// Photon Information ///
   //////////////////////////
   photon_index = 0;
   edm::Handle<edm::View<pat::Photon> >photons;
   iEvent.getByToken(photonToken_, photons);
   for (edm::View<pat::Photon>::const_iterator itPho = photons->begin(); itPho != photons->end(); ++itPho) {
      phos_pt_     = -999;
      phos_eta_    = -999;
      phos_phi_    = -999;
      phos_energy_ = -999;
      abseta = -9990;
      phos_SCB_Loose_ = false;
      phos_SCB_Medium_ = false;
      phos_SCB_Tight_ = false;
      isPassNonTrigTight = false;
      chIso = -999;
      nhIso = -999;
      phIso = -999;
      CHHad_effArea = -999;
      NeuHad_effArea = -999;
      Pho_effArea = -999;
      pho_r9 = -999;
      phoHoverE = -999;
      pho_scE = -999;
      pho_scRawE = -999;
      pho_sceta = -999;
      pho_scphi = -999;
      pho_scetawidth = -999;
      pho_scphowidth = -999;
      pho_EleVeto = -999;
      pho_full5x5_sigmaIetaIeta = -999;

      auto corrP4  = itPho->p4() * itPho->userFloat("ecalEnergyPostCorr") / itPho->energy();

      /*phos_pt_     = itPho.pt();
      phos_eta_    = itPho.eta();
      phos_phi_    = itPho.phi();
      phos_energy_ = itPho.energy();*/

      phos_pt_ = corrP4.Pt();
      phos_eta_ = corrP4.Eta();
      phos_phi_ = corrP4.Phi();
      phos_energy_ = corrP4.energy();


      abseta = fabs(itPho->superCluster()->eta());
      phos_SCB_Loose_ = itPho->photonID("cutBasedPhotonID-Fall17-94X-V2-loose");
      phos_SCB_Medium_ = itPho->photonID("cutBasedPhotonID-Fall17-94X-V2-medium");
      phos_SCB_Tight_ = itPho->photonID("cutBasedPhotonID-Fall17-94X-V2-tight");
      //bool isPassMVA_WP80 = itPho->photonID("mvaPhoID-RunIIFall17-v2-wp80");
      //bool isPassMVA_WP90 = itPho->photonID("mvaPhoID-RunIIFall17-v2-wp90");



      CHHad_effArea  = effAreaChHadrons_.getEffectiveArea(abseta);
      NeuHad_effArea = effAreaNeuHadrons_.getEffectiveArea(abseta);
      Pho_effArea    = effAreaPhotons_.getEffectiveArea(abseta);
//      double pfrho_iso   = isolation->PFIsoRho(chIso,nhIso,phIso,rho,Area,phos_pt_ );
      pho_r9         = itPho->r9();
      phoHoverE      = itPho->hadTowOverEm();
      pho_scE        = itPho->superCluster()->energy();
      pho_scRawE     = itPho->superCluster()->rawEnergy();
      pho_sceta      = itPho->superCluster()->eta();
      pho_scphi      = itPho->superCluster()->phi();
      pho_scetawidth = itPho->superCluster()->etaWidth();
      pho_scphowidth = itPho->superCluster()->phiWidth();
      pho_EleVeto    = itPho->passElectronVeto();

      ssbtreeManager->Fill("Photon", phos_pt_,phos_eta_,phos_phi_,phos_energy_,photon_index );
      ssbtreeManager->Fill("Photon_SCB_Loose",               phos_SCB_Loose_    );
      ssbtreeManager->Fill("Photon_SCB_Medium",              phos_SCB_Medium_   );
      ssbtreeManager->Fill("Photon_SCB_Tight",               phos_SCB_Tight_    );
      ssbtreeManager->Fill("Photon_MVANonTrig_Tight",        isPassNonTrigTight );
      ssbtreeManager->Fill("Photon_R9",                      pho_r9             );
      ssbtreeManager->Fill("Photon_HoverE",                  phoHoverE          );
      ssbtreeManager->Fill("Photon_SuperCluster_Eta",        pho_sceta          );
      ssbtreeManager->Fill("Photon_SuperCluster_Phi",        pho_scphi          );
      ssbtreeManager->Fill("Photon_SuperCluster_EtaWidth",   pho_scetawidth     );
      ssbtreeManager->Fill("Photon_SuperCluster_PhiWidth",   pho_scphowidth     );
      ssbtreeManager->Fill("Photon_SuperCluster_Energy",     pho_scE            );
      ssbtreeManager->Fill("Photon_SuperCluster_RawEnergy",  pho_scRawE         );
      ssbtreeManager->Fill("Photon_Full5x5_SigmaIetaIeta",   pho_full5x5_sigmaIetaIeta);
      ssbtreeManager->Fill("Photon_ElectronVeto"           , pho_EleVeto    );
      ssbtreeManager->Fill("Photon_ChaHadIso"              , chIso          );
      ssbtreeManager->Fill("Photon_NeuHadIso"              , nhIso          );
      ssbtreeManager->Fill("Photon_PhoIso"                 , phIso          );
      ssbtreeManager->Fill("Photon_WorstChargedIso"        , worstchIso     );
      ssbtreeManager->Fill("Photon_ChaHadEffArea"          , CHHad_effArea  );
      ssbtreeManager->Fill("Photon_NeuHadEffArea"          , NeuHad_effArea );
      ssbtreeManager->Fill("Photon_PhoHadEffArea"          , Pho_effArea    );
//      ssbtreeManager->Fill("Photon_PFIsoRho03"       , pfrho_iso          );
      photon_index++;


   }


   /////////////////////////
   /// Jets Information  ///
   /////////////////////////


   // Utility for Jet ID
   jet_index = 0;
 
   Handle<pat::JetCollection> jets;
   iEvent.getByToken(jetToken_, jets);

   Handle<pat::JetCollection> jetpuppis;
   iEvent.getByToken(jetpuppiToken_, jetpuppis);

   // Declare JetCorrectionUnc. //
/*   JetCorrectionUncertainty* jetcorr_uncertainty(0);
   //jetcorr_uncertainty = new JetCorrectionUncertainty("./JECDir/Summer15_25nsV2_MC/Summer15_25nsV2_MC_Uncertainty_AK4PFchs.txt");
   jetcorr_uncertainty = new JetCorrectionUncertainty(jetUncTag);*/

   JetCorrectionUncertainty* jetcorr_uncertainty(0);
   edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
   iSetup.get<JetCorrectionsRecord>().get(payloadName_, JetCorParColl);
   JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
   jetcorr_uncertainty = new JetCorrectionUncertainty(JetCorPar);

   // Declare JetPhiResolution // For Systematic Unc., we need to find out new receipe ...
   JME::JetResolution phiresol_mc;
   JME::JetResolution phiresol_data;
   JME::JetResolution ptresol_mc;
   JME::JetResolution ptresol_data;
   JME::JetResolutionScaleFactor res_sf;
   JME::JetParameters parameters;
   // Define resolution //
   phiresol_mc   = JME::JetResolution(phi_resol_mc_file);
   phiresol_data = JME::JetResolution(phi_resol_data_file);
   ptresol_mc    = JME::JetResolution(pt_resol_mc_file);
   ptresol_data  = JME::JetResolution(pt_resol_data_file);
   res_sf        = JME::JetResolutionScaleFactor(pt_resolsf_file);

   int jepuppiidx =0;
   for ( const pat::Jet &itJet : *jetpuppis ) 
   {
      if ( itJet.energy() < 20.0 || abs( itJet.eta() ) > 2.5 ) {continue;} // import at python config
      jepuppiidx++;
   }
   for ( const pat::Jet &itJet : *jets ) 
   {

      ////////////////////////////////////
      /// selection of resaonable jets ///
      ////////////////////////////////////
      
      if ( isJetPreCut == false ) { if ( itJet.pt() < 20.0 || abs( itJet.eta() ) > 2.5 ) {continue;} }// import at python config
      jets_pt_          = -9999;
      jets_eta_         = -9999;
      jets_phi_         = -9999;
      jets_energy_      = -9999;
      rawjets_pt_          = -9999;
      rawjets_eta_         = -9999;
      rawjets_phi_         = -9999;
      rawjets_energy_      = -9999;
      jets_pdgid_       = 0;
      jets_isJet_       = false;
      jets_bDisc_       = -9999;
      jets_charge_      = -9999;
      jets_pfjetid_     = -999;
      jets_pfjetidveto_     = -999;
      jets_mvapujetid_  = -999;
      jets_mvapujet_    = -999;
      isLoosejetid_pass = false;
      isTightjetid_pass = false;
      isLoosejetidLepVeto_pass = false;
      isTightjetidLepVeto_pass = false;
      jets_UncEnUp_     = 0;
      jets_UncEnDown_   = 0;

      NHF                 = -999;
      NEMF                = -999;
      CHF                 = -999;
      MUF                 = -999;
      CEMF                = -999;
      NumConst            = -999;
      NumNeutralParticles = -999;
      CHM                 = -999;

      phi_resol_mc      = -999;
      phi_resol_data    = -999;
      pt_resol_mc       = -999;
      pt_resol_data       = -999;
      jet_en_resol_sf   = -999;

      ////////////////////////////
      /// End of jet selection ///
      ////////////////////////////

      float mva   = -999;

      //// Jet Kinematics ////
      jets_pt_ = itJet.pt();
      jets_eta_ = itJet.eta();
      jets_phi_ = itJet.phi();
      jets_energy_ = itJet.energy();
      jets_charge_ = itJet.charge();
      //// RawJet Kinematics ////
      rawjets_pt_ = itJet.correctedP4("Uncorrected").pt();
      rawjets_eta_ = itJet.correctedP4("Uncorrected").eta();
      rawjets_phi_ = itJet.correctedP4("Uncorrected").phi();
      rawjets_energy_ = itJet.correctedP4("Uncorrected").energy();



      /// works only for JPT or PF jet
      jets_mvapujet_ = mva;
      jets_pdgid_ = itJet.pdgId();
      jets_isJet_ = itJet.isJet();



      /// Update PFJetID ( Muon Energy Veto )///
/*      NHF      = itJet.neutralHadronEnergyFraction();
      NEMF     = itJet.neutralEmEnergyFraction();
      CHF      = itJet.chargedHadronEnergyFraction();
      MUF      = itJet.muonEnergyFraction();
      CEMF     = itJet.chargedEmEnergyFraction();
      NumConst = itJet.chargedMultiplicity()+itJet.neutralMultiplicity();
      CHM      = itJet.chargedMultiplicity(); 
*/
      NHF  = itJet.neutralHadronEnergyFraction();
      NEMF = itJet.neutralEmEnergyFraction();
      CHF  = itJet.chargedHadronEnergyFraction();
      MUF  = itJet.muonEnergyFraction();
      CEMF = itJet.chargedEmEnergyFraction();
      NumConst = itJet.chargedMultiplicity()+itJet.neutralMultiplicity();
      NumNeutralParticles =itJet.neutralMultiplicity();
      CHM      = itJet.chargedMultiplicity(); 



      ///For PFJetIDSelectionFunctor

      /*if (isTightjetid_pass)
      {
         jets_pfjetid_ = 2;
      }
      else if (isLoosejetid_pass)
      {
         jets_pfjetid_ = 1;
      }
      else 
      {
         jets_pfjetid_ = 0;
      }*/


      if(runPeriod =="UL2016"|| runPeriod == "UL2016APV"){
        if(fabs(jets_eta_)<=2.4){
          //isTightjetid_pass = ( CHM>0 && CHF>0 && NumConst>1 && NEMF<0.9 && NHF < 0.9 ); 
          isTightjetid_pass = ( CHM>0 && CHF>0 && NumConst>1 && NEMF<0.9 && NHF < 0.9 ); 
          isTightjetidLepVeto_pass = isTightjetid_pass && CEMF<0.8 && MUF <0.8; 
        }else if(fabs(jets_eta_)<=2.7){
          isTightjetid_pass = ( NEMF<0.99 && NHF < 0.9 );
          isTightjetidLepVeto_pass = isTightjetid_pass;
        }else if(fabs(jets_eta_)<=3.0){
          isTightjetid_pass = ( NEMF>0.0 && NEMF<0.99 && NHF<0.9 && NumNeutralParticles>1 );
          isTightjetidLepVeto_pass = isTightjetid_pass;
        }else{
          isTightjetid_pass = ( NEMF<0.90 && NHF>0.2 && NumNeutralParticles>10 );
          isTightjetidLepVeto_pass = isTightjetid_pass;
        }
      }
      else if(runPeriod == "UL2017" || runPeriod == "UL2018"){
        if(fabs(jets_eta_)<=2.6){
          isTightjetid_pass = ( CHM>0 && CHF>0 && NumConst>1 && NEMF<0.9 && NHF < 0.9 ); 
          isTightjetidLepVeto_pass = isTightjetid_pass && CEMF<0.8 && MUF <0.8; 
        }else if(fabs(jets_eta_)<=2.7){
          isTightjetid_pass = ( CHM>0 && NEMF<0.99 && NHF < 0.9 );
          isTightjetidLepVeto_pass = isTightjetid_pass && CEMF<0.8 && MUF <0.8;
        }else if(fabs(jets_eta_)<=3.0){
          isTightjetid_pass = ( NEMF>0.01 && NEMF<0.99 && NumNeutralParticles>1 );
          isTightjetidLepVeto_pass = isTightjetid_pass;
        }else{
          isTightjetid_pass = ( NEMF<0.90 && NHF>0.2 && NumNeutralParticles>10 );
          isTightjetidLepVeto_pass = isTightjetid_pass;
        }
      }
      else {
         cout << "Error!! Check out runPeriod!!!!" << runPeriod << endl;
      }
      if (isTightjetid_pass)
      {
         jets_pfjetid_ = 1;
      }
      else 
      {
         jets_pfjetid_ = 0;
      }

      if (isTightjetidLepVeto_pass)
      {
         jets_pfjetidveto_ = 1;
      }
      else 
      {
         jets_pfjetidveto_ = 0;
      }


      /// Calculating JetEnergy Uncertainty
      jetcorr_uncertainty->setJetEta(jets_eta_);
      jetcorr_uncertainty->setJetPt(jets_pt_);
      double unc = jetcorr_uncertainty->getUncertainty(true);
      jets_UncEnUp_ = (1. +  unc);
      jetcorr_uncertainty->setJetEta(jets_eta_);
      jetcorr_uncertainty->setJetPt(jets_pt_);
      unc = jetcorr_uncertainty->getUncertainty(false);
      jets_UncEnDown_ = (1. -  unc);
/*
      jetcorr_test->setJetEta(jets_eta_);
      jetcorr_test->setJetPt(jets_pt_);
      double unc_test = jetcorr_test->getUncertainty(true);
      double jets_UncEnUp_test = (1. +  unc_test);
      jetcorr_test->setJetEta(jets_eta_);
      jetcorr_test->setJetPt(jets_pt_);
      unc_test = jetcorr_test->getUncertainty(false);
      double jets_UncEnDown_test = (1. -  unc_test);*/
       
      // Define parameters of Jet Phi (pT) Resol.//
      parameters.setJetPt(jets_pt_);
      parameters.setJetEta(jets_eta_);
      parameters.setRho(rho); 

      phi_resol_mc    = phiresol_mc.getResolution(parameters);
      phi_resol_data  = phiresol_data.getResolution(parameters);
      pt_resol_mc     = ptresol_mc.getResolution(parameters);
      pt_resol_data   = ptresol_data.getResolution(parameters);
      jet_en_resol_sf = res_sf.getScaleFactor({{JME::Binning::JetEta, jets_eta_}});
      double jet_en_resol_sf_up = res_sf.getScaleFactor({{JME::Binning::JetEta, jets_eta_}}, Variation::UP);
      double jet_en_resol_sf_dn = res_sf.getScaleFactor({{JME::Binning::JetEta, jets_eta_}}, Variation::DOWN);

      double fJER   = -999;
      double fJERUp = -999;
      double fJERDn = -999;

      if (!iEvent.isRealData()){
         //const reco::GenJet* genJet = itJet.genJet();
         auto genJet = itJet.genJetFwdRef();
         if ( genJet.isNonnull() and deltaR(genJet->p4(), itJet.p4()) < 0.2 and 
              std::abs(genJet->pt()-jets_pt_) < pt_resol_mc*3*jets_pt_ )
         {
            const double genJetPt = genJet->pt();
            const double dPt = jets_pt_-genJetPt;
            fJER   = std::max(0., (genJetPt+dPt*jet_en_resol_sf)/jets_pt_);
            fJERUp = std::max(0., (genJetPt+dPt*jet_en_resol_sf_up)/jets_pt_);
            fJERDn = std::max(0., (genJetPt+dPt*jet_en_resol_sf_dn)/jets_pt_);
            //cout << "fJER : " << fJER << " fJERUp " << fJERUp << " fJERDn " << fJERDn << endl;
         }
         else {
            edm::Service<edm::RandomNumberGenerator> rng;
            if (!rng.isAvailable()) edm::LogError("JET : random number generator is missing !");
            CLHEP::HepRandomEngine & engine = rng->getEngine( iEvent.streamID() );
            //float rnd = CLHEP::RandGauss::shoot();
            const double mcShift = CLHEP::RandGauss::shoot(&engine);
            fJER   = jet_en_resol_sf   <= 1 ? 1 : 1+mcShift*pt_resol_mc*sqrt(jet_en_resol_sf*jet_en_resol_sf-1);
            fJERUp = jet_en_resol_sf_up <= 1 ? 1 : 1+mcShift*pt_resol_mc*sqrt(jet_en_resol_sf_up*jet_en_resol_sf_up-1);
            fJERDn = jet_en_resol_sf_dn <= 1 ? 1 : 1+mcShift*pt_resol_mc*sqrt(jet_en_resol_sf_dn*jet_en_resol_sf_dn-1);
            //cout << "Rand fJER : " << fJER << " fJERUp " << fJERUp << " fJERDn " << fJERDn << endl;
         }

      }


      // Getting BTagging Information // https://btv-wiki.docs.cern.ch/ScaleFactors/
      //jets_bDisc_ = itJet.bDiscriminator("pfDeepFlavourJetTags:probb") + itJet.bDiscriminator("pfDeepFlavourJetTags:probbb") + itJet.bDiscriminator("pfDeepFlavourJetTags:problepb");
      jets_bDisc_ = itJet.bDiscriminator(csvBJetTag); // old version ///

      for (unsigned int ibtag =0; ibtag < btagList.size(); ++ibtag)
      {
         TString btagName = btagList.at(ibtag);
         btagName.ReplaceAll(":","_");
         ssbtreeManager->Fill( "Jet_bDisc_Name" ,  btagList); 
         ssbtreeManager->Fill( "Jet_bDisc_Value" , itJet.bDiscriminator( btagList.at(ibtag) ) ); 
         //cout << "btagList.at(ibtag) : " << btagList.at(ibtag)  << " disc: " << itJet.bDiscriminator( btagList.at(ibtag) ) << endl;
      }
      //cout << "jets_bDisc_ : "<< jets_bDisc_ << endl;

      //LorentzVector uncorrectedP4 = itJet.correctedP4("Uncorrected");
      ssbtreeManager->Fill( "Jet", jets_pt_, jets_eta_, jets_phi_, jets_energy_, jet_index);
      ssbtreeManager->Fill( "RawJet", rawjets_pt_, rawjets_eta_, rawjets_phi_, rawjets_energy_, jet_index);
      ssbtreeManager->Fill( "Jet_Charge"        , jets_charge_          );
      ssbtreeManager->Fill( "Jet_isJet"         , jets_isJet_           );
      ssbtreeManager->Fill( "Jet_bDisc"         , jets_bDisc_           );

      ssbtreeManager->Fill( "Jet_PFId"               ,     jets_pfjetid_     );
      ssbtreeManager->Fill( "Jet_PFIdVeto"           ,     jets_pfjetidveto_ );
      ssbtreeManager->Fill( "Jet_PileUpId"           ,     jets_mvapujetid_  );
      ssbtreeManager->Fill( "Jet_PileUpMVA"          ,     jets_mvapujet_    );

      ssbtreeManager->Fill( "Jet_EnShiftedUp"        ,     jets_UncEnUp_     );
      ssbtreeManager->Fill( "Jet_EnShiftedDown"      ,     jets_UncEnDown_   );
      ssbtreeManager->Fill( "Jet_PhiResolution_MC"   ,     phi_resol_mc      ); 
      ssbtreeManager->Fill( "Jet_PhiResolution_DATA" ,     phi_resol_data    ); 
      ssbtreeManager->Fill( "Jet_EnergyResolution_MC",     pt_resol_mc    ); 
      ssbtreeManager->Fill( "Jet_EnergyResolution_DATA",   pt_resol_data    ); 
      ssbtreeManager->Fill( "Jet_EnergyResolution_SF",     fJER              ); 
      ssbtreeManager->Fill( "Jet_EnergyResolution_SFUp",   fJERUp            ); 
      ssbtreeManager->Fill( "Jet_EnergyResolution_SFDown", fJERDn            ); 
 
      jet_index++;

   }
   ssbtreeManager->Fill( "Jet_Count", jet_index );
   delete jetcorr_uncertainty;

   ///////////////////////////////
   /////// MET Information ///////
   ///////////////////////////////
   MET_index=0;

   Handle<pat::METCollection> mets;
   iEvent.getByToken(metToken_, mets); // slimmedMets with JEC //
//   const pat::MET &met = mets->front();
//   cout << "met "<< met.pt() << endl;

   for (const pat::MET &itMet : *mets)
   {
            /*cout << " MET MC pt Test1 : " << itMet.shiftedPt(pat::MET::NoShift, pat::MET::Type1XY) 
                 << " MET MC phi Test1 : " << itMet.shiftedPhi(pat::MET::NoShift, pat::MET::Type1XY) 
                 << " MET MC sumET Test1 : " << itMet.shiftedSumEt(pat::MET::NoShift, pat::MET::Type1XY) 
                 << " MET MC pt Test2 : " << itMet.pt() 
                 << " MET MC phi Test2 : " << itMet.phi() 
                 << " MET MC sumEt Test2 : " << itMet.sumEt() 
                 << " MET MC pt Test3 : " << itMet.shiftedPt(pat::MET::NoShift, pat::MET::Type1)
                 << " MET MC phi Test3 : " << itMet.shiftedPhi(pat::MET::NoShift, pat::MET::Type1) 
                 << " MET MC sumET Test3 : " << itMet.shiftedSumEt(pat::MET::NoShift, pat::MET::Type1) 
                 << endl;*/
      //cout << "met sig. : "<< itMet.significance()  << " met METsig. : "<< itMet.metSignificance()  << endl;
      //cout << "met pt. : "<< itMet.pt()  << " itMet.et : " << itMet.et() << " met phi. : "<< itMet.phi()  << endl;
      //ssbtreeManager->Fill( "MET" , itMet.shiftedPt(pat::MET::NoShift, pat::MET::Type1XY), 0, itMet.shiftedPhi(pat::MET::NoShift, pat::MET::Type1XY), 0, MET_index ); /// TO-DO  We need to check up receipe for MET XY(phi) correction ...
      ssbtreeManager->Fill( "MET" , itMet.pt(), 0, itMet.phi(), 0, MET_index ); /// TO-DO  We need to check up receipe for MET XY(phi) correction ... (MET Phi/XY correction is standalone So we need uncorrected MET pt and phi)
      ssbtreeManager->Fill( "MET_Significance", itMet.significance() );

      MET_index++;

   }
   /// Fill Ntuples at each event
   ssbtreeManager->FillNtuple();

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
SSBAnalyzer::beginJob()
{
   ssbtree = ssbfs->make<TTree>("SSBTree", "Tree for Physics Analyses at CMS");
   ssbtreeManager = new SSBTreeManager();
//   ssbtreeManager->book(ssbtree);
   ssbtreeManager->Book(ssbtree);

   if (pdfCent)
   {
      /// PDFWeight
      pdfWeight = new SSBPDFWeight(pdfSets.size(), pdfSets.at(0));
      //pdfWeight = new SSBPDFWeight(1, pdfSets.at(0));

   }

   /// Isolation calculation
   isolation = new SSBIsoCal(); 

   /// PDF
   for (unsigned int k=1; k<=pdfSets.size(); k++) 
   {
      LHAPDF::initPDFSet(k, pdfSets[k-1]);
      v_pdfWeights.push_back( new SSBPDFWeight(pdfSets.size(), pdfSets.at(k-1)) );
   }
//   LHAPDF::initPDFSet(1, pdfSets.at(0));
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SSBAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
SSBAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
SSBAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
SSBAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const&)
{
   //edm::Service<edm::RandomNumberGenerator> randomNumberEngine_;
   //randomNumberEngine = &randomNumberEngine_->getEngine(lumi.index());
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
SSBAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SSBAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
void
SSBAnalyzer::GenPar(const edm::Event& iEvent, SSBTreeManager* ssbtreeManager) {

    ////////////////////////////////////////////
    /// Generator Level Particle Information ///
    ////////////////////////////////////////////

    /// To Check Up input sample which is Herwig sample or PYHIA Sample ///

    edm::Handle < GenEventInfoProduct > genEvtInfoProduct;
    iEvent.getByToken(genEvnInfoTag, genEvtInfoProduct);
    std::string moduleName = "";
    if( genEvtInfoProduct.isValid() ) {
      const edm::Provenance& prov = iEvent.getProvenance(genEvtInfoProduct.id());
      moduleName = edm::moduleName(prov);
      //cout << "moduleName : " << moduleName << endl;
    }
    bool isPYTHIA = false;
    bool isHerwig = false;
    if( moduleName.find("Pythia")!=std::string::npos ) {isPYTHIA = true;}
    if( moduleName.find("PEGHadronizer")!=std::string::npos ) {isHerwig = true;}
    //cout << isPYTHIA << endl;
    if (isPYTHIA) {
    /// PYTHIA Event Histroy !!
    edm::Handle<GenParticleCollection> genParticles;
    iEvent.getByToken(genParInfoTag, genParticles); /* get genParticle information */

    InitializeGenPar(); /* initialize vector and map */
    isSignal = false;

    GenParticleCollection::const_iterator itGenParBegin = genParticles->begin();

    vector<const reco::Candidate *> cands; /* reco::Candidate vector (mother and daugher function return reco::Candidate) */
    for ( GenParticleCollection::const_iterator itGenParIndex = genParticles->begin(); itGenParIndex != genParticles->end(); ++itGenParIndex )
    {
	cands.push_back(&*itGenParIndex);
    } /* use for mother and daughter index */

    for ( GenParticleCollection::const_iterator itGenPar = genParticles->begin(); itGenPar != genParticles->end(); itGenPar++ )
    {
        if(itGenPar->status() == 62){
        if(itGenPar->pdgId() ==  6) ssbtreeManager->Fill( "GenTop", itGenPar->pt(), itGenPar->eta(), itGenPar->phi(), itGenPar->energy(), 0 );
        if(itGenPar->pdgId() == -6) ssbtreeManager->Fill( "GenAnTop", itGenPar->pt(), itGenPar->eta(), itGenPar->phi(), itGenPar->energy(), 0 );
        }
	int GenParIndex = itGenPar - itGenParBegin; /* get index */
	OriginalMom.clear();
	for ( unsigned int N_Mother = 0; N_Mother < itGenPar->numberOfMothers(); ++N_Mother ){
	    OriginalMom.push_back(find(cands.begin(), cands.end(), itGenPar->mother(N_Mother)) - cands.begin());
	} /* get all mother's index */
	OriginalDau.clear();
	for ( unsigned int N_Daughter = 0; N_Daughter < itGenPar->numberOfDaughters(); ++N_Daughter ){
	    OriginalDau.push_back(find(cands.begin(), cands.end(), itGenPar->daughter(N_Daughter)) - cands.begin());
	} /* get all daughter's index */

	AllParMom[GenParIndex] = OriginalMom; /* make mother index map */
	AllParDau[GenParIndex] = OriginalDau; /* make daughter index map */

	pdgId_status.clear();
	pdgId_status.push_back(itGenPar->pdgId());
	pdgId_status.push_back(itGenPar->status());
	AllParInfo[GenParIndex] = pdgId_status; /* make pdgid and status map */
	if ( (itGenPar->status() > 20 && itGenPar->status() < 24) ) { /* without proton */
		TreePar.push_back(GenParIndex); /* get tree level particles */
		if ( abs(itGenPar->pdgId()) == 6 || abs(itGenPar->pdgId()) == 24 ) {
		    SelectedpdgId[itGenPar->pdgId()] = GenParIndex; /* save index of top and W */
		}
	} 

	if ( (itGenPar->status() == 1 || itGenPar->status() == 2) &&
	     (abs(itGenPar->pdgId()) > 10 && abs(itGenPar->pdgId()) < 17) ) {
		FinalPar.push_back(GenParIndex); /* put final state and intermediate lepton and neutrino*/
	}
    }/* genpar loop end */

    if (SelectedpdgId.find(6) != SelectedpdgId.end() && SelectedpdgId.find(-6) != SelectedpdgId.end()) isSignal = true;
    if ( isSignal == true ) { /* if Signal start */
    for (unsigned int SelectedB = 0; SelectedB < TreePar.size(); ++SelectedB) {
	if ( abs(AllParInfo[TreePar.at(SelectedB)].at(0)) == 5 && abs(AllParInfo[AllParMom[TreePar.at(SelectedB)].at(0)].at(0)) == 6) {
	    SelectedpdgId[AllParInfo[TreePar.at(SelectedB)].at(0)] = TreePar.at(SelectedB);
	} /* find b form top and save index */
    } 

    for (unsigned int FinaltoTree = 0; FinaltoTree < FinalPar.size(); ++FinaltoTree){
	if ( abs( AllParInfo[AllParMom[FinalPar.at(FinaltoTree)].at(0)].at(0) ) == 24){ /* when First Mother is W */
	    TreePar.push_back(FinalPar.at(FinaltoTree));
	    FinalPar.erase(find(FinalPar.begin(), FinalPar.end(), FinalPar.at(FinaltoTree)));
	    --FinaltoTree;
	}
    } /* Move final state particle (from W decay) */

    SelectedPar.push_back(0);
    SelectedPar.push_back(1);

    SelectedPar.push_back(SelectedpdgId[6]);   	
    SelectedPar.push_back(SelectedpdgId[-6]);  	
    SelectedPar.push_back(SelectedpdgId[24]);   	
    SelectedPar.push_back(SelectedpdgId[5]); 	
    SelectedPar.push_back(SelectedpdgId[-24]);   	
    SelectedPar.push_back(SelectedpdgId[-5]);

    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[6]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[-6]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[24]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[-24]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[5]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[-5]));

    for (unsigned int RemoveLowIndex = 0; RemoveLowIndex < TreePar.size(); ++RemoveLowIndex) {
	if (TreePar.at(RemoveLowIndex) < 10) {
	    TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(RemoveLowIndex)));
	    --RemoveLowIndex;
	}
    } /* remove tree level gluon form ttbar mother (IT NEED STUDY) */
      /* + in MINIAOD, it will remove doughter of p+ */

    int FromWplusSum = 0;
    for (unsigned int FromWplus = 0; FromWplus < TreePar.size(); ++FromWplus){
	if (FromWplusSum == 2) {
	    break;
	}
	if (IndexLinker(AllParDau, SelectedpdgId[24], 0, TreePar.at(FromWplus)) != -1) {
	    SelectedPar.push_back(TreePar.at(FromWplus));
	    TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(FromWplus)));
	    --FromWplus;
	    ++FromWplusSum;
	}
    } /* from w+ */

    for (unsigned int FromWminus = 0; FromWminus < TreePar.size(); ++FromWminus){
	//if (IndexLinker(AllParDau, SelectedpdgId[-24], 0, TreePar.at(FromWminus)) != -1) {
   	    SelectedPar.push_back(TreePar.at(FromWminus));
	//}
    } /* from w- */

    /* SelectedPar_MINI : {p+, p+, 
			   0   1  
			   t, tbar, w+, b, w-, bbar, w+_first_daughter, w+_second_daughter, w-_first_daughter, w-_second_daughter}
			   2  3     4   5  6   7     8                  9                   10                 11 */

    if (SelectedPar.size() != 12) {
	cerr << "!!!!! Signal Sample : SelectedPar Error !!!!!" << endl;
	cout << "!!!!! Signal Sample : SelectedPar Error !!!!!" << endl;
	cout << endl << "SelectedPar : " << endl;
	for (unsigned int i = 0; i < SelectedPar.size(); ++i){
	    cout << ParName[AllParInfo[SelectedPar.at(i)].at(0)] << " ";
	}
	cout << endl;
	for (unsigned int i = 0; i < SelectedPar.size(); ++i){
	    cout << SelectedPar.at(i) << " ";
	}
	cout << endl;
    }

    FillGenPar(SelectedPar.at(0), -1, -1, SelectedPar.at(2), SelectedPar.at(3), itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(1), -1, -1, SelectedPar.at(2), SelectedPar.at(3), itGenParBegin, ssbtreeManager);

    FillGenPar(SelectedPar.at(2), SelectedPar.at(0), SelectedPar.at(1), SelectedPar.at(4), SelectedPar.at(5), itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(3), SelectedPar.at(0), SelectedPar.at(1), SelectedPar.at(6), SelectedPar.at(7), itGenParBegin, ssbtreeManager);

    FillGenPar(SelectedPar.at(4), SelectedPar.at(2), -1, SelectedPar.at(8), SelectedPar.at(9), itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(5), SelectedPar.at(2), -1, -1, -1, itGenParBegin, ssbtreeManager);
 
    FillGenPar(SelectedPar.at(6), SelectedPar.at(3), -1, SelectedPar.at(10), SelectedPar.at(11), itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(7), SelectedPar.at(3), -1, -1, -1, itGenParBegin, ssbtreeManager);

    FillGenPar(SelectedPar.at(8), SelectedPar.at(4), -1, -1, -1, itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(9), SelectedPar.at(4), -1, -1, -1, itGenParBegin, ssbtreeManager);
 
    FillGenPar(SelectedPar.at(10), SelectedPar.at(6), -1, -1, -1, itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(11), SelectedPar.at(6), -1, -1, -1, itGenParBegin, ssbtreeManager);

    } /* if Signal end */
    else { /* if Background start */
    SelectedPar.push_back(0);
    SelectedPar.push_back(1);
    for (unsigned int TreetoSel = 0; TreetoSel < TreePar.size(); ++TreetoSel){
	SelectedPar.push_back(TreePar.at(TreetoSel));
 	TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(TreetoSel)));
	--TreetoSel;
    }
    for (unsigned int FinaltoTree = 0; FinaltoTree < FinalPar.size(); ++FinaltoTree){
	int MompdgId = abs(AllParInfo[AllParMom[FinalPar.at(FinaltoTree)].at(0)].at(0));
	if ( MompdgId == 6 || MompdgId == 23 || MompdgId == 24 || MompdgId == 25 ){
	    TreePar.push_back(FinalPar.at(FinaltoTree));
	    FinalPar.erase(find(FinalPar.begin(), FinalPar.end(), FinalPar.at(FinaltoTree)));
	    --FinaltoTree;
	}
    }

    for (unsigned int TreetoSel = 0; TreetoSel < TreePar.size(); ++TreetoSel){
	for (unsigned int SelectedSize = 0; SelectedSize < SelectedPar.size(); ++SelectedSize){
	    int SelectedpdgId = abs(AllParInfo[SelectedPar.at(SelectedSize)].at(0));
	    if ( SelectedpdgId == 6 || SelectedpdgId == 23 || SelectedpdgId == 24 || SelectedpdgId == 25 ) { /* Mother : top, Z, W, H */
		if (IndexLinker(AllParMom, TreePar.at(TreetoSel), 0, SelectedPar.at(SelectedSize)) != -1 ) { /* when Mother is in SelectedPar */
		    SelectedPar.push_back(TreePar.at(TreetoSel));
		    TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(TreetoSel)));
		    --TreetoSel;
		    break;
		}
	    }
	}
    } /* Move final state particle */

    for (unsigned int AllSel = 0; AllSel < SelectedPar.size(); ++AllSel) {
	int FM = -1;	
	int SM = -1;	
	int FD = -1;	
	int SD = -1;	
	if ( AllParMom[SelectedPar[AllSel]].size() > 0 ) {
	    FM = AllParMom[SelectedPar[AllSel]].at(0);
	    if ( AllParMom[SelectedPar[AllSel]].size() == 2 ) {
		SM = AllParMom[SelectedPar[AllSel]].at(1);
	    }
	}
	if ( AllParDau[SelectedPar[AllSel]].size() > 0 ) {
	    FD = AllParDau[SelectedPar[AllSel]].at(0);
	    if ( AllParDau[SelectedPar[AllSel]].size() == 2 ) {
		SD = AllParDau[SelectedPar[AllSel]].at(1);
	    }
	}

	FillGenPar(SelectedPar[AllSel], FM, SM, FD, SD, itGenParBegin, ssbtreeManager);
    } /* Fill All Par */

    } /* if Background end */

    for (unsigned int RemoveTwo = 0; RemoveTwo < FinalPar.size(); ++RemoveTwo){
	if ( AllParInfo[FinalPar.at(RemoveTwo)].at(1) == 2 ) {
	    //for (unsigned int RemoveTwoSub = 0; RemoveTwoSub < FinalPar.size(); ++RemoveTwoSub){
		//if (RemoveTwo != RemoveTwoSub && IndexLinker(AllParDau, FinalPar.at(RemoveTwo), 0, FinalPar.at(RemoveTwoSub)) != -1) {
		    FinalPar.erase(find(FinalPar.begin(), FinalPar.end(), FinalPar.at(RemoveTwo)));
		    --RemoveTwo;
		    //break;
		//}
	    //}
	}
    } /* my original idea was remove status 2 particle if final state particle form this status 2 particle is in FinalPar */

/*    for (unsigned int RemoveNu = 0; RemoveNu < FinalPar.size(); ++RemoveNu){
        int Nu_pdgId = abs(AllParInfo[FinalPar.at(RemoveNu)].at(0));
	if ( Nu_pdgId == 12 || Nu_pdgId == 14 || Nu_pdgId == 16 ) {
		    FinalPar.erase(find(FinalPar.begin(), FinalPar.end(), FinalPar.at(RemoveNu)));
		    --RemoveNu;
	}
    } * remove neutrinos in FinalPar */ 

    int ChannelLepton = 0;
    int ChannelLeptonFinal = 0;
    int ChannelIndex = 0;
    int ChannelIndexFinal = 0;
    for (unsigned int OnlyLepton = 0; OnlyLepton < SelectedPar.size(); ++OnlyLepton) {
	int Lepton_pdgId = abs(AllParInfo[SelectedPar.at(OnlyLepton)].at(0));
	if (Lepton_pdgId == 11 || Lepton_pdgId == 13 || Lepton_pdgId == 15) {
	    ++ChannelLepton; /* check number of lepton */
	    if (Lepton_pdgId == 15) {
		ChannelIndex -= Lepton_pdgId;
	    }
	    else {
		ChannelIndex += Lepton_pdgId;
	    } /* distinguish channel */
	//} /*check all particle's final state end */
	    SelectedDau.clear();
	    if (Lepton_pdgId == 15) { /* check only tau's final state start */
	    for (unsigned int FinalCandidate = 0; FinalCandidate < FinalPar.size(); ++FinalCandidate) {
		if (IndexLinker(AllParDau, SelectedPar.at(OnlyLepton), 0, FinalPar.at(FinalCandidate)) != -1) {
		    SelectedDau.push_back(FinalPar.at(FinalCandidate));
		}
	    }
	    } /* check only tau's final state end */
	    SelParDau[SelectedPar.at(OnlyLepton)] = SelectedDau;
	} /* check all lepton's final state end */
    }

    ChannelLeptonFinal = ChannelLepton;
    ChannelIndexFinal = ChannelIndex;

    for (map_i_it FinaltoSel = SelParDau.begin(); FinaltoSel != SelParDau.end(); ++FinaltoSel) {
	int Lepton_Mom_pdgId = 0;
	int Lepton_Dau_pdgId = 0;
	int Lepton_Mom_flag = 0;
	for (unsigned int DauIndex = 0; DauIndex < (FinaltoSel->second).size(); ++DauIndex) {
	    if(SelectedPar.end() == find(SelectedPar.begin(), SelectedPar.end(), (FinaltoSel->second).at(DauIndex))) { /* count just 1 time */
		SelectedPar.push_back((FinaltoSel->second).at(DauIndex));
		Lepton_Mom_pdgId = abs(AllParInfo[FinaltoSel->first].at(0));
		Lepton_Dau_pdgId = abs(AllParInfo[(FinaltoSel->second).at(DauIndex)].at(0));
		if (Lepton_Mom_pdgId != Lepton_Dau_pdgId) { /* lepton decay to something */
		    if (Lepton_Mom_flag == 0) { /* check mother just 1 time  */
			++Lepton_Mom_flag;
			--ChannelLeptonFinal;
			if (Lepton_Mom_pdgId < 14) ChannelIndexFinal -= Lepton_Mom_pdgId;
			if (Lepton_Mom_pdgId > 14) ChannelIndexFinal += Lepton_Mom_pdgId;
		    }
		    if (Lepton_Dau_pdgId == 11 || Lepton_Dau_pdgId == 13 || Lepton_Dau_pdgId == 15) {
	                FillGenPar((FinaltoSel->second).at(DauIndex), FinaltoSel->first, -1, -1, -1, itGenParBegin, ssbtreeManager);
                        /* fill final state lepton */
			++ChannelLeptonFinal;
			if (Lepton_Dau_pdgId < 14) ChannelIndexFinal += Lepton_Dau_pdgId;
			if (Lepton_Dau_pdgId > 14) ChannelIndexFinal -= Lepton_Dau_pdgId;
		    }
		}
	    }
	}
    }

    int ChannelJets = 0;
    int ChannelJetsAbs = 0;
    if ( isSignal == true ) {
        for (unsigned int WIndex = 0; WIndex < 2; ++WIndex) {
            int Channel_pdgId = 0;
            unsigned int First_Dau_pdgId = 99;
            unsigned int Second_Dau_pdgId = 99;
            First_Dau_pdgId  = abs(AllParInfo[SelectedPar.at(2*WIndex+8)].at(0));
            Second_Dau_pdgId = abs(AllParInfo[SelectedPar.at(2*WIndex+9)].at(0));
            if ( First_Dau_pdgId < 10 ) {
                if ( First_Dau_pdgId%2 == WIndex ) Channel_pdgId = 10*First_Dau_pdgId + Second_Dau_pdgId;
                else Channel_pdgId = First_Dau_pdgId + 10*Second_Dau_pdgId;
                if ( ChannelJets == 0 ) ChannelJets = Channel_pdgId;
                else ChannelJets = 100*ChannelJets + Channel_pdgId;
            }
        }
    }
    if ( ChannelJets > 0 ) {
        ChannelJetsAbs = ChannelJets;
        if ( (ChannelJetsAbs/10)%10 > ChannelJetsAbs%10 ) ChannelJetsAbs = 100*(ChannelJetsAbs/100) + 10*(ChannelJetsAbs%10) + (ChannelJetsAbs/10)%10;
        if ( ChannelJetsAbs/1000 > (ChannelJetsAbs/100)%10 ) ChannelJetsAbs = 1000*((ChannelJetsAbs/100)%10) + 100*(ChannelJetsAbs/1000) + (ChannelJetsAbs%100);
        if ( ChannelJetsAbs/100 > ChannelJetsAbs%100 ) ChannelJetsAbs = 100*(ChannelJetsAbs%100) + ChannelJetsAbs/100;
    }

    ssbtreeManager->Fill( "GenPar_Count"          , genPar_index );
    ssbtreeManager->Fill( "Channel_Idx"           , ChannelIndex );
    ssbtreeManager->Fill( "Channel_Idx_Final"     , ChannelIndexFinal );
    ssbtreeManager->Fill( "Channel_Jets"          , ChannelJets );
    ssbtreeManager->Fill( "Channel_Jets_Abs"      , ChannelJetsAbs );
    ssbtreeManager->Fill( "Channel_Lepton_Count"       , ChannelLepton );
    ssbtreeManager->Fill( "Channel_Lepton_Count_Final" , ChannelLeptonFinal );
    }/// End of PYTHIA
    else if (isHerwig) {
    //cout << "HERWIG!!!!" << endl;
    /// HERWIG Event Histroy !! 
    edm::Handle<GenParticleCollection> genParticles;
    iEvent.getByToken(genParInfoTag, genParticles); /* get genParticle information */

    InitializeGenPar(); /* initialize vector and map */
    isSignal = false;

    GenParticleCollection::const_iterator itGenParBegin = genParticles->begin();

    vector<const reco::Candidate *> cands; /* reco::Candidate vector (mother and daugher function return reco::Candidate) */
    for ( GenParticleCollection::const_iterator itGenParIndex = genParticles->begin(); itGenParIndex != genParticles->end(); ++itGenParIndex )
    {
	cands.push_back(&*itGenParIndex);
    } /* use for mother and daughter index */

    for ( GenParticleCollection::const_iterator itGenPar = genParticles->begin(); itGenPar != genParticles->end(); itGenPar++ )
    {
        //if(itGenPar->status() == 62){
        //if(itGenPar->pdgId() ==  6) ssbtreeManager->Fill( "GenTop", itGenPar->pt(), itGenPar->eta(), itGenPar->phi(), itGenPar->energy(), 0 );
        //if(itGenPar->pdgId() == -6) ssbtreeManager->Fill( "GenAnTop", itGenPar->pt(), itGenPar->eta(), itGenPar->phi(), itGenPar->energy(), 0 );
        //}
	int GenParIndex = itGenPar - itGenParBegin; /* get index */
	OriginalMom.clear();
	for ( unsigned int N_Mother = 0; N_Mother < itGenPar->numberOfMothers(); ++N_Mother ){
	    OriginalMom.push_back(find(cands.begin(), cands.end(), itGenPar->mother(N_Mother)) - cands.begin());
	} /* get all mother's index */
	OriginalDau.clear();
	for ( unsigned int N_Daughter = 0; N_Daughter < itGenPar->numberOfDaughters(); ++N_Daughter ){
	    OriginalDau.push_back(find(cands.begin(), cands.end(), itGenPar->daughter(N_Daughter)) - cands.begin());
	} /* get all daughter's index */

	AllParMom[GenParIndex] = OriginalMom; /* make mother index map */
	AllParDau[GenParIndex] = OriginalDau; /* make daughter index map */

	pdgId_status.clear();
	pdgId_status.push_back(itGenPar->pdgId());
	pdgId_status.push_back(itGenPar->status());
	AllParInfo[GenParIndex] = pdgId_status; /* make pdgid and status map */

	//if ( (itGenPar->status() > 20 && itGenPar->status() < 24) ) { /* without proton */
	//	TreePar.push_back(GenParIndex); /* get tree level particles */
	//	if ( abs(itGenPar->pdgId()) == 6 || abs(itGenPar->pdgId()) == 24 ) {
	//	    SelectedpdgId[itGenPar->pdgId()] = GenParIndex; /* save index of top and W */
	//	}
	//} 

	//if ( (itGenPar->status() == 1 || itGenPar->status() == 2) &&
	//     (abs(itGenPar->pdgId()) > 10 && abs(itGenPar->pdgId()) < 17) ) {
	//	FinalPar.push_back(GenParIndex); /* put final state and intermediate lepton and neutrino */
	//}

        if ( abs(itGenPar->pdgId()) == 2212 && OriginalMom.size() == 0 ) {
            SelectedPar.push_back(GenParIndex);
        }
        if ( itGenPar->status() == 11
            && ( abs(itGenPar->pdgId()) == 6 || ( abs(itGenPar->pdgId()) > 22 && abs(itGenPar->pdgId()) < 26 ))){
            TreePar.push_back(GenParIndex); /* put t,W,Z and H */
        }
	if ( itGenPar->status() == 1 && (abs(itGenPar->pdgId()) > 10 && abs(itGenPar->pdgId()) < 17) ) {
            FinalPar.push_back(GenParIndex); /* put final state and intermediate lepton and neutrino */
	}
	
    } /* genpar loop end */
    
    /* 2nd loop for herwig sample start */
    for ( unsigned int i_tree = 0; i_tree < TreePar.size(); ++i_tree ){
        bool isDuplicated = false;
        for ( unsigned int i_mom = 0; i_mom < AllParMom[TreePar.at(i_tree)].size(); ++i_mom ){
            if ( abs(AllParInfo[AllParMom[TreePar.at(i_tree)].at(i_mom)].at(0)) == abs(AllParInfo[TreePar.at(i_tree)].at(0)) ) isDuplicated = true;
        }
        if ( isDuplicated ) {
            if ( abs(AllParInfo[TreePar.at(i_tree)].at(0)) == 24 && AllParDau[TreePar.at(i_tree)].size() > 1 && abs(AllParInfo[AllParDau[TreePar.at(i_tree)].at(0)].at(0)) != 24 ){
                TreePar.push_back(AllParDau[TreePar.at(i_tree)].at(0));
                TreePar.push_back(AllParDau[TreePar.at(i_tree)].at(1));
            }
            if ( abs(AllParInfo[TreePar.at(i_tree)].at(0)) == 6  ){
                if ( AllParDau[TreePar.at(i_tree)].size() > 0 && abs(AllParInfo[AllParDau[TreePar.at(i_tree)].at(0)].at(0)) == 5 ){
                    TreePar.push_back(AllParDau[TreePar.at(i_tree)].at(0));
                    SelectedpdgId[AllParInfo[AllParDau[TreePar.at(i_tree)].at(0)].at(0)] = AllParDau[TreePar.at(i_tree)].at(0);
                }
                if ( AllParDau[TreePar.at(i_tree)].size() > 1 && abs(AllParInfo[AllParDau[TreePar.at(i_tree)].at(1)].at(0)) == 5 ){
                    TreePar.push_back(AllParDau[TreePar.at(i_tree)].at(1));
                    SelectedpdgId[AllParInfo[AllParDau[TreePar.at(i_tree)].at(1)].at(0)] = AllParDau[TreePar.at(i_tree)].at(1);
                }
            }
	    TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(i_tree)));
	    --i_tree;
        }
        if ( abs(AllParInfo[TreePar.at(i_tree)].at(0)) == 24 && abs(AllParInfo[AllParMom[TreePar.at(i_tree)].at(0)].at(0)) != 6 ) {
	    TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(i_tree)));
	    --i_tree;
        } /* remove W */
    } /* genpar loop for herwig sample end */

    for (unsigned int i_tree = 0; i_tree < TreePar.size(); ++i_tree) {
        if ( abs(AllParInfo[TreePar.at(i_tree)].at(0)) == 6 || abs(AllParInfo[TreePar.at(i_tree)].at(0)) == 24 ) {
            SelectedpdgId[AllParInfo[TreePar.at(i_tree)].at(0)] = TreePar.at(i_tree);
        } 
    }

    if (SelectedpdgId.find(6) != SelectedpdgId.end() && SelectedpdgId.find(-6) != SelectedpdgId.end()) isSignal = true;
    if ( isSignal == true ) { /* if Signal start */
    //for (unsigned int SelectedB = 0; SelectedB < TreePar.size(); ++SelectedB) {
        //if ( abs(AllParInfo[TreePar.at(SelectedB)].at(0)) == 5 && abs(AllParInfo[AllParMom[TreePar.at(SelectedB)].at(0)].at(0)) == 6) {
            //SelectedpdgId[AllParInfo[TreePar.at(SelectedB)].at(0)] = TreePar.at(SelectedB);
        //} /* find b form top and save index */
    //}

    //for (unsigned int FinaltoTree = 0; FinaltoTree < FinalPar.size(); ++FinaltoTree){
	//if ( abs( AllParInfo[AllParMom[FinalPar.at(FinaltoTree)].at(0)].at(0) ) == 24){ /* when First Mother is W */
	    //TreePar.push_back(FinalPar.at(FinaltoTree));
	    //FinalPar.erase(find(FinalPar.begin(), FinalPar.end(), FinalPar.at(FinaltoTree)));
	    //--FinaltoTree;
	//}
    //} /* Move final state particle (from W decay) */

    //SelectedPar.push_back(0);
    //SelectedPar.push_back(1);

    SelectedPar.push_back(SelectedpdgId[6]);   	
    SelectedPar.push_back(SelectedpdgId[-6]);  	
    SelectedPar.push_back(SelectedpdgId[24]);   	
    SelectedPar.push_back(SelectedpdgId[5]); 	
    SelectedPar.push_back(SelectedpdgId[-24]);   	
    SelectedPar.push_back(SelectedpdgId[-5]);

    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[6]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[-6]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[24]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[-24]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[5]));
    TreePar.erase(find(TreePar.begin(), TreePar.end(), SelectedpdgId[-5]));

    //for (unsigned int RemoveLowIndex = 0; RemoveLowIndex < TreePar.size(); ++RemoveLowIndex) {
	//if (TreePar.at(RemoveLowIndex) < 10) {
	    //TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(RemoveLowIndex)));
	    //--RemoveLowIndex;
	//}
    //} /* remove tree level gluon form ttbar mother (IT NEED STUDY) */
      /* + in MINIAOD, it will remove doughter of p+ */

    int FromWplusSum = 0;
    for (unsigned int FromWplus = 0; FromWplus < TreePar.size(); ++FromWplus){
	if (FromWplusSum == 2) {
	    break;
	}
	if (IndexLinker(AllParDau, SelectedpdgId[24], 0, TreePar.at(FromWplus)) != -1) {
	    SelectedPar.push_back(TreePar.at(FromWplus));
	    TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(FromWplus)));
	    --FromWplus;
	    ++FromWplusSum;
	}
    } /* from w+ */

    for (unsigned int FromWminus = 0; FromWminus < TreePar.size(); ++FromWminus){
	if (IndexLinker(AllParDau, SelectedpdgId[-24], 0, TreePar.at(FromWminus)) != -1) {
   	    SelectedPar.push_back(TreePar.at(FromWminus));
	}
    } /* from w- */

    /* SelectedPar_MINI : {p+, p+, 
			   0   1  
			   t, tbar, w+, b, w-, bbar, w+_first_daughter, w+_second_daughter, w-_first_daughter, w-_second_daughter}
			   2  3     4   5  6   7     8                  9                   10                 11 */

    if (SelectedPar.size() != 12) {
	cerr << "!!!!! Signal Sample : SelectedPar Error !!!!!" << endl;
	cout << "!!!!! Signal Sample : SelectedPar Error !!!!!" << endl;
	cout << endl << "SelectedPar : " << endl;
	for (unsigned int i = 0; i < SelectedPar.size(); ++i){
	    cout << ParName[AllParInfo[SelectedPar.at(i)].at(0)] << " ";
	}
	cout << endl;
	for (unsigned int i = 0; i < SelectedPar.size(); ++i){
	    cout << SelectedPar.at(i) << " ";
	}
	cout << endl;
    }

    FillGenPar(SelectedPar.at(0), -1, -1, SelectedPar.at(2), SelectedPar.at(3), itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(1), -1, -1, SelectedPar.at(2), SelectedPar.at(3), itGenParBegin, ssbtreeManager);

    FillGenPar(SelectedPar.at(2), SelectedPar.at(0), SelectedPar.at(1), SelectedPar.at(4), SelectedPar.at(5), itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(3), SelectedPar.at(0), SelectedPar.at(1), SelectedPar.at(6), SelectedPar.at(7), itGenParBegin, ssbtreeManager);

    FillGenPar(SelectedPar.at(4), SelectedPar.at(2), -1, SelectedPar.at(8), SelectedPar.at(9), itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(5), SelectedPar.at(2), -1, -1, -1, itGenParBegin, ssbtreeManager);
 
    FillGenPar(SelectedPar.at(6), SelectedPar.at(3), -1, SelectedPar.at(10), SelectedPar.at(11), itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(7), SelectedPar.at(3), -1, -1, -1, itGenParBegin, ssbtreeManager);

    FillGenPar(SelectedPar.at(8), SelectedPar.at(4), -1, -1, -1, itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(9), SelectedPar.at(4), -1, -1, -1, itGenParBegin, ssbtreeManager);
 
    FillGenPar(SelectedPar.at(10), SelectedPar.at(6), -1, -1, -1, itGenParBegin, ssbtreeManager);
    FillGenPar(SelectedPar.at(11), SelectedPar.at(6), -1, -1, -1, itGenParBegin, ssbtreeManager);


    for ( GenParticleCollection::const_iterator itGenPar = genParticles->begin(); itGenPar != genParticles->end(); itGenPar++ ) {
	int GenParIndex = itGenPar - itGenParBegin;
        if ( itGenPar->pdgId() ==  6 && GenParIndex == SelectedpdgId[6]  ) ssbtreeManager->Fill( "GenTop", itGenPar->pt(), itGenPar->eta(), itGenPar->phi(), itGenPar->energy(), 0 );
        if ( itGenPar->pdgId() == -6 && GenParIndex == SelectedpdgId[-6] ) ssbtreeManager->Fill( "GenAnTop", itGenPar->pt(), itGenPar->eta(), itGenPar->phi(), itGenPar->energy(), 0 );
    }

    } /* if Signal end */
    else { /* if Background start */
    //SelectedPar.push_back(0);
    //SelectedPar.push_back(1);
    for (unsigned int TreetoSel = 0; TreetoSel < TreePar.size(); ++TreetoSel){
	SelectedPar.push_back(TreePar.at(TreetoSel));
 	TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(TreetoSel)));
	--TreetoSel;
    }
    //for (unsigned int FinaltoTree = 0; FinaltoTree < FinalPar.size(); ++FinaltoTree){
	//int MompdgId = abs(AllParInfo[AllParMom[FinalPar.at(FinaltoTree)].at(0)].at(0));
	//if ( MompdgId == 6 || MompdgId == 23 || MompdgId == 24 || MompdgId == 25 ){
	    //TreePar.push_back(FinalPar.at(FinaltoTree));
	    //FinalPar.erase(find(FinalPar.begin(), FinalPar.end(), FinalPar.at(FinaltoTree)));
	    //--FinaltoTree;
	//}
    //}

    for (unsigned int FinaltoSel = 0; FinaltoSel < FinalPar.size(); ++FinaltoSel){
	for (unsigned int SelectedSize = 0; SelectedSize < SelectedPar.size(); ++SelectedSize){
	    int SelectedpdgId = abs(AllParInfo[SelectedPar.at(SelectedSize)].at(0));
	    if ( SelectedpdgId == 6 || SelectedpdgId == 23 || SelectedpdgId == 24 || SelectedpdgId == 25 ) { /* Mother : top, Z, W, H */
		if (IndexLinker(AllParMom, FinalPar.at(FinaltoSel), 0, SelectedPar.at(SelectedSize)) != -1 ) { /* when Mother is in SelectedPar */
		    SelectedPar.push_back(FinalPar.at(FinaltoSel));
		    FinalPar.erase(find(FinalPar.begin(), FinalPar.end(), FinalPar.at(FinaltoSel)));
		    --FinaltoSel;
		    break;
		}
	    }
	}
    } /* Move final state particle */

    for (unsigned int AllSel = 0; AllSel < SelectedPar.size(); ++AllSel) {
	int FM = -1;	
	int SM = -1;	
	int FD = -1;	
	int SD = -1;	
	if ( AllParMom[SelectedPar[AllSel]].size() > 0 ) {
	    FM = AllParMom[SelectedPar[AllSel]].at(0);
	    if ( AllParMom[SelectedPar[AllSel]].size() == 2 ) {
		SM = AllParMom[SelectedPar[AllSel]].at(1);
	    }
	}
	if ( AllParDau[SelectedPar[AllSel]].size() > 0 ) {
	    FD = AllParDau[SelectedPar[AllSel]].at(0);
	    if ( AllParDau[SelectedPar[AllSel]].size() == 2 ) {
		SD = AllParDau[SelectedPar[AllSel]].at(1);
	    }
	}

	FillGenPar(SelectedPar[AllSel], FM, SM, FD, SD, itGenParBegin, ssbtreeManager);
    } /* Fill All Par */

    } /* if Background end */

    //for (unsigned int RemoveTwo = 0; RemoveTwo < FinalPar.size(); ++RemoveTwo){
	//if ( AllParInfo[FinalPar.at(RemoveTwo)].at(1) == 2 ) {
	    //for (unsigned int RemoveTwoSub = 0; RemoveTwoSub < FinalPar.size(); ++RemoveTwoSub){
		//if (RemoveTwo != RemoveTwoSub && IndexLinker(AllParDau, FinalPar.at(RemoveTwo), 0, FinalPar.at(RemoveTwoSub)) != -1) {
		    //FinalPar.erase(find(FinalPar.begin(), FinalPar.end(), FinalPar.at(RemoveTwo)));
		    //--RemoveTwo;
		    //break;
		//}
	    //}
	//}
    //} /* my original idea was remove status 2 particle if final state particle form this status 2 particle is in FinalPar */

/*    for (unsigned int RemoveNu = 0; RemoveNu < FinalPar.size(); ++RemoveNu){
        int Nu_pdgId = abs(AllParInfo[FinalPar.at(RemoveNu)].at(0));
	if ( Nu_pdgId == 12 || Nu_pdgId == 14 || Nu_pdgId == 16 ) {
		    FinalPar.erase(find(FinalPar.begin(), FinalPar.end(), FinalPar.at(RemoveNu)));
		    --RemoveNu;
	}
    } * remove neutrinos in FinalPar */ 

    int ChannelLepton = 0;
    int ChannelLeptonFinal = 0;
    int ChannelIndex = 0;
    int ChannelIndexFinal = 0;
    for (unsigned int OnlyLepton = 0; OnlyLepton < SelectedPar.size(); ++OnlyLepton) {
	int Lepton_pdgId = abs(AllParInfo[SelectedPar.at(OnlyLepton)].at(0));
	if (Lepton_pdgId == 11 || Lepton_pdgId == 13 || Lepton_pdgId == 15) {
	    ++ChannelLepton; /* check number of lepton */
	    if (Lepton_pdgId == 15) {
		ChannelIndex -= Lepton_pdgId;
	    }
	    else {
		ChannelIndex += Lepton_pdgId;
	    } /* distinguish channel */
	//} /*check all particle's final state end */
	    SelectedDau.clear();
	    if (Lepton_pdgId == 15) { /* check only tau's final state start */
	    for (unsigned int FinalCandidate = 0; FinalCandidate < FinalPar.size(); ++FinalCandidate) {
		if (IndexLinker(AllParDau, SelectedPar.at(OnlyLepton), 0, FinalPar.at(FinalCandidate)) != -1) {
		    SelectedDau.push_back(FinalPar.at(FinalCandidate));
		}
	    }
	    } /* check only tau's final state end */
	    SelParDau[SelectedPar.at(OnlyLepton)] = SelectedDau;
	} /* check all lepton's final state end */
    }

    ChannelLeptonFinal = ChannelLepton;
    ChannelIndexFinal = ChannelIndex;

    for (map_i_it FinaltoSel = SelParDau.begin(); FinaltoSel != SelParDau.end(); ++FinaltoSel) {
	int Lepton_Mom_pdgId = 0;
	int Lepton_Dau_pdgId = 0;
	int Lepton_Mom_flag = 0;
	for (unsigned int DauIndex = 0; DauIndex < (FinaltoSel->second).size(); ++DauIndex) {
	    if(SelectedPar.end() == find(SelectedPar.begin(), SelectedPar.end(), (FinaltoSel->second).at(DauIndex))) { /* count just 1 time */
		SelectedPar.push_back((FinaltoSel->second).at(DauIndex));
		Lepton_Mom_pdgId = abs(AllParInfo[FinaltoSel->first].at(0));
		Lepton_Dau_pdgId = abs(AllParInfo[(FinaltoSel->second).at(DauIndex)].at(0));
		if (Lepton_Mom_pdgId != Lepton_Dau_pdgId) { /* lepton decay to something */
		    if (Lepton_Mom_flag == 0) { /* check mother just 1 time  */
			++Lepton_Mom_flag;
			--ChannelLeptonFinal;
			if (Lepton_Mom_pdgId < 14) ChannelIndexFinal -= Lepton_Mom_pdgId;
			if (Lepton_Mom_pdgId > 14) ChannelIndexFinal += Lepton_Mom_pdgId;
		    }
		    if (Lepton_Dau_pdgId == 11 || Lepton_Dau_pdgId == 13 || Lepton_Dau_pdgId == 15) {
	                FillGenPar((FinaltoSel->second).at(DauIndex), FinaltoSel->first, -1, -1, -1, itGenParBegin, ssbtreeManager);
                        /* fill final state lepton */
			++ChannelLeptonFinal;
			if (Lepton_Dau_pdgId < 14) ChannelIndexFinal += Lepton_Dau_pdgId;
			if (Lepton_Dau_pdgId > 14) ChannelIndexFinal -= Lepton_Dau_pdgId;
		    }
		}
	    }
	}
    }

    int ChannelJets = 0;
    int ChannelJetsAbs = 0;
    if ( isSignal == true ) {
        for (unsigned int WIndex = 0; WIndex < 2; ++WIndex) {
            int Channel_pdgId = 0;
            unsigned int First_Dau_pdgId = 99;
            unsigned int Second_Dau_pdgId = 99;
            First_Dau_pdgId  = abs(AllParInfo[SelectedPar.at(2*WIndex+8)].at(0));
            Second_Dau_pdgId = abs(AllParInfo[SelectedPar.at(2*WIndex+9)].at(0));
            if ( First_Dau_pdgId < 10 ) {
                if ( First_Dau_pdgId%2 == WIndex ) Channel_pdgId = 10*First_Dau_pdgId + Second_Dau_pdgId;
                else Channel_pdgId = First_Dau_pdgId + 10*Second_Dau_pdgId;
                if ( ChannelJets == 0 ) ChannelJets = Channel_pdgId;
                else ChannelJets = 100*ChannelJets + Channel_pdgId;
            }
        }
    }
    if ( ChannelJets > 0 ) {
        ChannelJetsAbs = ChannelJets;
        if ( (ChannelJetsAbs/10)%10 > ChannelJetsAbs%10 ) ChannelJetsAbs = 100*(ChannelJetsAbs/100) + 10*(ChannelJetsAbs%10) + (ChannelJetsAbs/10)%10;
        if ( ChannelJetsAbs/1000 > (ChannelJetsAbs/100)%10 ) ChannelJetsAbs = 1000*((ChannelJetsAbs/100)%10) + 100*(ChannelJetsAbs/1000) + (ChannelJetsAbs%100);
        if ( ChannelJetsAbs/100 > ChannelJetsAbs%100 ) ChannelJetsAbs = 100*(ChannelJetsAbs%100) + ChannelJetsAbs/100;
    }

    ssbtreeManager->Fill( "GenPar_Count"          , genPar_index );
    ssbtreeManager->Fill( "Channel_Idx"           , ChannelIndex );
    ssbtreeManager->Fill( "Channel_Idx_Final"     , ChannelIndexFinal );
    ssbtreeManager->Fill( "Channel_Jets"          , ChannelJets );
    ssbtreeManager->Fill( "Channel_Jets_Abs"      , ChannelJetsAbs );
    ssbtreeManager->Fill( "Channel_Lepton_Count"       , ChannelLepton );
    ssbtreeManager->Fill( "Channel_Lepton_Count_Final" , ChannelLeptonFinal );

    }// End of Herwig 
    else { cout << "CHECK OUT THE HADRONIZTION MODULE OF YOUR SAMPLE !!" << endl;}
/*
	cout << endl << endl << "SelectedPar : " << endl;
	for (unsigned int i = 0; i < SelectedPar.size(); ++i){
	    cout << ParName[AllParInfo[SelectedPar.at(i)].at(0)] << " ";
	}
	cout << endl;
	for (unsigned int i = 0; i < SelectedPar.size(); ++i){
	    cout << SelectedPar.at(i) << " ";
	}
	cout << endl;
*/
}

int
SSBAnalyzer::IndexLinker(map_i IndexMap, int start_index, int target_depth, int target_index, int target_pdgid, int target_status, bool PrintError, int LoopDepth){
    if ( ((start_index == target_index) || (target_index == -999)) && 
         ((AllParInfo[start_index].at(0) == target_pdgid) || (target_pdgid == 0)) &&
         ((AllParInfo[start_index].at(1) == target_status) || (target_status == 0)) ) {
	if (PrintError) {
	    cout << endl << "Here is your target" << endl << "Depth : " << LoopDepth << endl << "Index : " << start_index << endl;
	    cout << "Status : " << AllParInfo[start_index].at(1) << endl;
	    cout << "pdgId : " << AllParInfo[start_index].at(0) << endl << endl;
	    cout << ParName[AllParInfo[start_index].at(0)] << " ";
	}
	return start_index;
    }
    else {
	++LoopDepth;
	int IndexLinkerResult = -1;
	for (unsigned int MapLoopIndex = 0; MapLoopIndex < IndexMap[start_index].size(); ++MapLoopIndex){
	    if(IndexMap[start_index].at(MapLoopIndex) != -1) {
		IndexLinkerResult = IndexLinker(IndexMap, IndexMap[start_index].at(MapLoopIndex), target_depth, target_index, target_pdgid, target_status, PrintError, LoopDepth);
		if (IndexLinkerResult != -1){
		    if (LoopDepth != 1) {
			if (PrintError) cout << "(" << IndexMap[start_index].at(MapLoopIndex) << "/" << ParName[IndexMap[start_index].at(MapLoopIndex)] << ") -> ";
		    }
		    else {
			if (PrintError) cout << "(" << IndexMap[start_index].at(MapLoopIndex) << "/" << ParName[IndexMap[start_index].at(MapLoopIndex)] << ") -> (" << start_index << ") " << ParName[AllParInfo[start_index].at(0)] << endl;
		    }
		    if (LoopDepth == target_depth) {
			return IndexMap[start_index].at(MapLoopIndex);
		    }
		    break;
		}
	    }
	}
	if (LoopDepth == 1 && IndexLinkerResult == -1 && PrintError) cout << "Not Found" << endl;
	return IndexLinkerResult;
    }
}

void
SSBAnalyzer::InitializeGenPar(){

    genPar_index = 0;
    AllParMom.clear();
    OriginalMom.clear();
    AllParDau.clear();
    OriginalDau.clear();
    AllParInfo.clear();
    pdgId_status.clear();
    SelParDau.clear();
    SelectedDau.clear();

    TreePar.clear();
    FinalPar.clear();
    SelectedPar.clear();

    SelectedpdgId.clear();

    ParName.clear();

    ParName[1] = "d";
    ParName[-1] = "dbar";
    ParName[2] = "u";
    ParName[-2] = "ubar";
    ParName[3] = "s";
    ParName[-3] = "sbar";
    ParName[4] = "c";
    ParName[-4] = "cbar";
    ParName[5] = "b";
    ParName[-5] = "bbar";
    ParName[6] = "t";
    ParName[-6] = "tbar";

    ParName[11] = "e-";
    ParName[-11] = "e+";
    ParName[12] = "nu_e";
    ParName[-12] = "nu_ebar";
    ParName[13] = "mu-";
    ParName[-13] = "mu+";
    ParName[14] = "nu_mu";
    ParName[-14] = "nu_mubar";
    ParName[15] = "tau-";
    ParName[-15] = "tau+";
    ParName[16] = "nu_tau";
    ParName[-16] = "nu_taubar";

    ParName[21] = "g";
    ParName[23] = "Z";
    ParName[24] = "W+";
    ParName[-24] = "W-";
    ParName[25] = "H";

    ParName[2212] = "p+";
}
void
SSBAnalyzer::FillGenPar(int GenIndex, int FirstMother, int SecondMother, int FirstDaughter, int SecondDaughter, GenParticleCollection::const_iterator itGenParFill, SSBTreeManager* ssbtreeManager){

    itGenParFill += GenIndex;
    int nMo = 2;
    int nDa = 2;

    if (FirstMother == -1) {
	--nMo;
	if (SecondMother == -1) {
	    --nMo;
	}
	else {
	    //std::swap(FirstMother, SecondMother);
	    FirstMother = SecondMother;
	}
    }
    else if (SecondMother == -1) {
	--nMo;
	SecondMother = FirstMother;
    }
    else if (FirstMother == SecondMother) {
	--nMo;
    }

    if (FirstDaughter == -1) {
	--nDa;
	if (SecondDaughter == -1) {
	    --nDa;
	}
	else {
	    //std::swap(FirstDaughter, SecondDaughter);
	    FirstDaughter = SecondDaughter;
	}
    }
    else if (SecondDaughter == -1) {
	--nDa;
	SecondDaughter = FirstDaughter;
    }
    else if (FirstDaughter == SecondDaughter) {
	--nDa;
    } 

    ssbtreeManager->Fill( "GenPar_Idx"         , GenIndex               );
    ssbtreeManager->Fill( "GenPar_pdgId"       , itGenParFill->pdgId()  );
    ssbtreeManager->Fill( "GenPar_Status"      , itGenParFill->status() );

    ssbtreeManager->Fill( "GenPar_Mom1_Idx"    , FirstMother            );
    ssbtreeManager->Fill( "GenPar_Mom2_Idx"    , SecondMother           );
    ssbtreeManager->Fill( "GenPar_Mom_Counter" , nMo                    );           

    ssbtreeManager->Fill( "GenPar_Dau1_Idx"    , FirstDaughter          );
    ssbtreeManager->Fill( "GenPar_Dau2_Idx"    , SecondDaughter         );
    ssbtreeManager->Fill( "GenPar_Dau_Counter" , nDa                    );           

    ssbtreeManager->Fill( "GenPar", itGenParFill->pt(), itGenParFill->eta(), itGenParFill->phi(), itGenParFill->energy(), genPar_index );

    genPar_index++;


}

void
SSBAnalyzer::GenJet(const edm::Event& iEvent, SSBTreeManager* ssbtreeManager) {

    //////////////////////////////////////
    ///Generator Level Jet information ///
    //////////////////////////////////////

    edm::Handle<GenJetCollection> genJets;
    iEvent.getByToken(genJetInfoTag,genJets);

    genJet_index = 0;

    for (GenJetCollection::const_iterator itgJet = genJets->begin() ; itgJet !=genJets->end(); itgJet++) {
	ssbtreeManager->Fill( "GenJet", (*itgJet).pt(), (*itgJet).eta(), (*itgJet).phi(), (*itgJet).energy(), genJet_index );
        ssbtreeManager->Fill( "GenJet_HCalEnergy"   , (*itgJet).hadEnergy()     );
        ssbtreeManager->Fill( "GenJet_ECalEnergy"   , (*itgJet).emEnergy()      );
	genJet_index++;   
    }

    ssbtreeManager->Fill( "GenJet_Count", genJet_index );

}
void
SSBAnalyzer::GenMET(const edm::Event& iEvent, SSBTreeManager* ssbtreeManager) {

    //////////////////////////////////////
    /// Generator Level MET informaton ///
    //////////////////////////////////////

    edm::Handle<GenMETCollection> genMETs;
    iEvent.getByToken(genMETInfoTag,genMETs);

    genMET_index = 0;

    for (GenMETCollection::const_iterator itgMET = genMETs->begin() ; itgMET !=genMETs->end(); itgMET++) {
	ssbtreeManager->Fill( "GenMET", (*itgMET).genMET()->pt(), 0, (*itgMET).genMET()->phi(), 0, genMET_index );
	genMET_index++;
    }
    ssbtreeManager->Fill( "GenMET_Count", genMET_index );

}
//define this as a plug-in
DEFINE_FWK_MODULE(SSBAnalyzer);
