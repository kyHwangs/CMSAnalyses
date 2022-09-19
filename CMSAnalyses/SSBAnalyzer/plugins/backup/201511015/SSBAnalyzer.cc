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
// Original Author:  Sungwoong Cho, Seungkyu Ha, Sehwook Lee, Suyoung Choi, Jaehoon Lim, Dooyeon Gyun, 588 R-004, +41227678602
//         Created:  Mon Jun  2 11:21:34 CEST 2014
// $Id$
//
//


// system include files
#include <memory>
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBAnalyzer.h"

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
isMC(iConfig.getParameter<bool>("isMCTag")),
// needed for PDF 
pdfTag( iConfig.getParameter<edm::InputTag>("PDFInfoTag") ),
pdfSets( iConfig.getParameter<std::vector<std::string>>("PDFSetNames") ),
pdfCent( iConfig.getParameter<bool>("PDFCent") ),
pdfSys( iConfig.getParameter<bool>("PDFSys") ),


/*// for EventFilter 
PFCandidatesTag( iConfig.getParameter<edm::InputTag>("PFCandidates") ), 
ptMinTag( iConfig.getParameter<double>("ptMin") ),
maxPTDiffTag(iConfig.getParameter<double>("maxPTDiff") ),
eOverPMaxTag(iConfig.getParameter<double>("eOverPMax") ),*/

// needed for Vertex
//vertexTag( iConfig.getParameter<edm::InputTag>("pvTag") ),
vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("pvTag"))),

rhoTag( iConfig.getParameter<edm::InputTag>("RhoTag") ),
pileupTag( iConfig.getParameter<edm::InputTag>("puTag") ),

// needed for Trigger 
triggerList( iConfig.getParameter<std::vector<std::string>>("trigList") ),
triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"))),
triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"))),

// needed for Muon
//muonTag( iConfig.getParameter<edm::InputTag>("muTag") )
muonToken_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muTag"))),
// needed for Electron
elecmvaIdNoTrTag( iConfig.getParameter<edm::InputTag>("emvaIdNoTrTag") ),                                                               
elecmvaIdTrNoIPTag( iConfig.getParameter<edm::InputTag>("emvaIdTrNoIPTag") ),                                                           
elecmvaIdTrTag( iConfig.getParameter<edm::InputTag>("emvaIdTrTag") ),                                                                   
beamSpotInputTag( iConfig.getParameter<edm::InputTag>("bstag") ),  
conversionsInputTag( iConfig.getParameter<edm::InputTag>("convertag") ),
//electronToken_(consumes<pat::ElectronCollection>(iConfig.getParameter<edm::InputTag>("eleTag"))),

// needed for Electron VID
eleVetoIdMapToken_(   consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleVetoIdMap"))),
eleLooseIdMapToken_(  consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleLooseIdMap"))),
eleMediumIdMapToken_( consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMediumIdMap"))),
eleTightIdMapToken_(  consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleTightIdMap"))),
eleHEEPIdMapToken_(   consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleHEEPIdMap"))),

// needed for Electron MVA
mva_eleNonTrigMediumIdMapToken_( consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("mva_eleNontrigMediumIdMap"))),
mva_eleNonTrigTightIdMapToken_(  consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("mva_eleNontrigTightIdMap"))),
mva_eleTrigMediumIdMapToken_(    consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("mva_eleTrigMediumIdMap"))),
mva_eleTrigTightIdMapToken_(     consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("mva_eleTrigTightIdMap"))),
mvaValuesMapToken_(              consumes<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("mvaValuesMap"))),
mvaCategoriesMapToken_(          consumes<edm::ValueMap<int> >(iConfig.getParameter<edm::InputTag>("mvaCategoriesMap"))),
// needed for Jet
jetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jtTag"))),
jetpuppiToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jtpuppiTag"))),
//jetTokenUp_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jtTagUp"))),
jetUncTag( iConfig.getParameter<std::string>("jtuncTag") ),
pfLooseJetIDparam( iConfig.getParameter<edm::ParameterSet> ("PFLooseJetID") ),
pfTightJetIDparam( iConfig.getParameter<edm::ParameterSet> ("PFTightJetID") ),
csvBJetTag( iConfig.getParameter<std::string>("csvbjetTag") ),
metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("metTag"))),
metnoHFToken_(consumes<METCollection>(iConfig.getParameter<edm::InputTag>("metnoHFTag"))),
metJetEnUpToken_(consumes<METCollection>(iConfig.getParameter<edm::InputTag>("metJetEnUpTag"))),
metJetEnDownToken_(consumes<METCollection>(iConfig.getParameter<edm::InputTag>("metJetEnDownTag"))),
isSignal( iConfig.getParameter<bool>("isSignal") )
{
   //now do what ever initialization is needed
   if (isMC == true) 
   {
      genParInfoTag = iConfig.getParameter<edm::InputTag>("genParTag");
      genJetInfoTag = iConfig.getParameter<edm::InputTag>("genJetTag");
      //genMETInfoTag = iConfig.getParameter<edm::InputTag>("genMETTag");
   }

   electronToken_ = mayConsume<edm::View<pat::Electron> >(iConfig.getParameter<edm::InputTag>("eleTag"));
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

   /////////////////////////////////
   /// Only For MC Samples!!!!!! ///
   /////////////////////////////////

   if (!isData)
   {

       ///////////////////////
       /// PDF Information ///
       ///////////////////////

      Handle<GenEventInfoProduct> pdfstuff;
      iEvent.getByLabel(pdfTag, pdfstuff);

      if (pdfCent)
      {
         pdfWeight->SetScalePDF(pdfstuff->pdf()->scalePDF);
         pdfWeight->SetIncomingPartion1(pdfstuff->pdf()->id.first, pdfstuff->pdf()->x.first, pdfstuff->pdf()->xPDF.first);
         pdfWeight->SetIncomingPartion2(pdfstuff->pdf()->id.second, pdfstuff->pdf()->x.second, pdfstuff->pdf()->xPDF.second);
         pdfWeight->SetPDFSet(1);
         double _pdf1 = pdfWeight->getCentralPDFWeight(1);
         double _pdf2 = pdfWeight->getCentralPDFWeight(2);
         double _nominal = _pdf1*_pdf2;
         pdfWeight->SetNominalWeight(_nominal);

         ssbtreeManager->Fill( "PDFWeight_Id1"       , pdfstuff->pdf()->id.first  );
         ssbtreeManager->Fill( "PDFWeight_Id2"       , pdfstuff->pdf()->id.second );
         ssbtreeManager->Fill( "PDFWeight_BjorkenX1" , pdfstuff->pdf()->x.first   );
         ssbtreeManager->Fill( "PDFWeight_BjorkenX2" , pdfstuff->pdf()->x.second  );
         ssbtreeManager->Fill( "PDFWeight_Q"         , pdfstuff->pdf()->scalePDF  );
         ssbtreeManager->Fill( "PDFWeight_PDF1"      , _pdf1                      );
         ssbtreeManager->Fill( "PDFWeight_PDF2"      , _pdf2                      );
      }
      if (pdfSys)
      {
         pdfWeight->getSys("Up");
         pdfWeight->getSys("Down");

         ssbtreeManager->Fill( "PDFWeight_Up"   , pdfWeight->getSys("Up")   );
         ssbtreeManager->Fill( "PDFWeight_Down" , pdfWeight->getSys("Down") );
      }

      ///////////////////////////////////////////
      /// Generator Level Particle Informaton ///
      ///////////////////////////////////////////
      GenPar(iEvent, ssbtreeManager);
      GenJet(iEvent, ssbtreeManager);
//      ssbgeninfor->GenJet(iEvent, ssbtreeManager);
//      ssbgeninfor->GenMET(iEvent, ssbtreeManager);

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
   iEvent.getByLabel(rhoTag, rhoHandle);
   
   if(rhoHandle.isValid()) 
   {
      rho = *(rhoHandle.product());
   } else {
      cout << "Rho is invalid!!!" << endl;

   }

   //////////////////////////
   /// Pileup Information ///
   //////////////////////////

   NPU = -1;
   NPUin = 0;
   BX = -999;
   if (!isData)
   { 

       Handle<PileUpCollection> pileup;
       iEvent.getByLabel( pileupTag, pileup ); 
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

   string genfile  = "./Pileup/MC_Common_Pileup.root";
   string datafile = "./Pileup/Data_2015_Reco_Pileup.root";

   if (FILE *file = fopen(genfile.c_str(), "r")) {
      fclose(file);
   }
   else {
      genfile = "./MC_Common_Pileup.root";
   }
   if (FILE *file = fopen(datafile.c_str(), "r")) {
      fclose(file);
   }
   else {
      datafile = "./Data_2015_Reco_Pileup.root";
   }

   LumiWeights_ = edm::LumiReWeighting(genfile,
                                      datafile,
                                      "pileup",
                                      "pileup");
   double MyWeight = LumiWeights_.weight( NPU );
   ssbtreeManager->Fill( "Weight_PileUp",MyWeight);

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
//      cout << "Trigger : " << names.triggerName(i) << endl;
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
      isHighPt = false;

      isLoose = muon.isLooseMuon() ;
      isSoft =  muon.isSoftMuon( v_vertex[0] );
      isTight = muon.isTightMuon( v_vertex[0] );
      isHighPt = muon.isHighPtMuon( v_vertex[0] );

      //cout << "03 " << muon.pfIsolationR03().sumChargedHadronPt << "   " << muon.pfIsolationR03().sumNeutralHadronEt << "   " 
      //<< muon.pfIsolationR03().sumPhotonEt << "   " << muon.pfIsolationR03().sumPUPt << endl;

      relIso03 = isolation->MuonRelTrkIso( muon.isolationR03().sumPt, muon.pt() );

      PFIsodbeta03 = isolation->PFIsodBeta( muon.pfIsolationR03().sumChargedHadronPt, muon.pfIsolationR03().sumNeutralHadronEt, 
                                                muon.pfIsolationR03().sumPhotonEt, muon.pfIsolationR03().sumPUPt, muon.pt() ,0.5);

      PFIsodbeta04 = isolation->PFIsodBeta( muon.pfIsolationR04().sumChargedHadronPt, muon.pfIsolationR04().sumNeutralHadronEt, 
                                                muon.pfIsolationR04().sumPhotonEt, muon.pfIsolationR04().sumPUPt, muon.pt() ,0.5);

      //cout << "04 " << muon.pfIsolationR04().sumChargedHadronPt << "   " << muon.pfIsolationR04().sumNeutralHadronEt << "   " 
      //     << muon.pfIsolationR04().sumPhotonEt << "   " << muon.pfIsolationR04().sumPUPt << endl;

      ssbtreeManager->Fill( "Muon", muon.pt(), muon.eta(), muon.phi(), muon.energy(), muon_index);
      ssbtreeManager->Fill( "Muon_isLoose"     , isLoose          );
      ssbtreeManager->Fill( "Muon_isSoft"      , isSoft           );
      ssbtreeManager->Fill( "Muon_isTight"     , isTight          );
      ssbtreeManager->Fill( "Muon_isHighPt"    , isHighPt         );
      ssbtreeManager->Fill( "Muon_relIso03"    , relIso03         );
      ssbtreeManager->Fill( "Muon_PFIsodBeta03", PFIsodbeta03     );
      ssbtreeManager->Fill( "Muon_PFIsodBeta04", PFIsodbeta04     );
      ssbtreeManager->Fill( "Muon_pdgId"       , muon.pdgId()     );
      ssbtreeManager->Fill( "Muon_Charge"      , muon.charge()    );

      muon_index++;

   }
   ssbtreeManager->Fill( "Muon_Count", muon_index );
 

   ////////////////////////////
   /////// Electron infor//////
   ////////////////////////////
   ele_index=0;

//   Handle<pat::ElectronCollection> elecs;
   edm::Handle<edm::View<pat::Electron> >elecs;
   iEvent.getByToken(electronToken_, elecs);

   edm::Handle < reco::BeamSpot > bsHandle;
   iEvent.getByLabel(beamSpotInputTag, bsHandle);

   Handle<ConversionCollection> hConversions;
   iEvent.getByLabel(conversionsInputTag, hConversions);

   edm::Handle<edm::ValueMap<bool> > veto_id_decisions;  // Veto ID Value Map ...
   iEvent.getByToken(eleVetoIdMapToken_ ,veto_id_decisions);

   edm::Handle<edm::ValueMap<bool> > loose_id_decisions;  // Loose ID Value Map ...
   iEvent.getByToken(eleLooseIdMapToken_ ,loose_id_decisions);

   edm::Handle<edm::ValueMap<bool> > medium_id_decisions;  // Medium ID Value Map ...
   iEvent.getByToken(eleMediumIdMapToken_,medium_id_decisions);

   edm::Handle<edm::ValueMap<bool> > tight_id_decisions; // Tight ID Value Map ...
   iEvent.getByToken(eleTightIdMapToken_,tight_id_decisions);

   edm::Handle<edm::ValueMap<bool> > heep_id_decisions; // HEEP ID Value Map ...
   iEvent.getByToken(eleHEEPIdMapToken_ ,heep_id_decisions);

   edm::Handle<edm::ValueMap<bool> > mva_Nontrigmedium_id_decisions;  // MVA NonTrig. Medium
   iEvent.getByToken(mva_eleNonTrigMediumIdMapToken_,mva_Nontrigmedium_id_decisions); 

   edm::Handle<edm::ValueMap<bool> > mva_Nontrigtight_id_decisions;  // MVA NonTrig.Tight
   iEvent.getByToken(mva_eleNonTrigTightIdMapToken_,mva_Nontrigtight_id_decisions);

   edm::Handle<edm::ValueMap<bool> > mva_Trigmedium_id_decisions;  // MVA Trig. Medium
   iEvent.getByToken(mva_eleTrigMediumIdMapToken_,mva_Trigmedium_id_decisions); 

   edm::Handle<edm::ValueMap<bool> > mva_Trigtight_id_decisions; // MVA Trig. Tight
   iEvent.getByToken(mva_eleTrigTightIdMapToken_,mva_Trigtight_id_decisions);

   edm::Handle<edm::ValueMap<float> > mvaValues; // mva Value 
   iEvent.getByToken(mvaValuesMapToken_,mvaValues);

   edm::Handle<edm::ValueMap<int> > mvaCategories; // mva Categories ? 
   iEvent.getByToken(mvaCategoriesMapToken_,mvaCategories);


   for ( const pat::Electron & itEle : *elecs )
   {
      const auto itele = elecs->ptrAt(ele_index);

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
      passconversionveto1 = false;

      elecs_relIso03_ = isolation->ElecRelIso( itEle.dr03HcalTowerSumEt(), itEle.dr03EcalRecHitSumEt(), itEle.dr03TkSumPt(), itEle.et() );
      elecs_relIso04_ = isolation->ElecRelIso( itEle.dr04HcalTowerSumEt(), itEle.dr04EcalRecHitSumEt(), itEle.dr04TkSumPt(), itEle.et() );

      // PF iso 03
      reco::GsfElectron::PflowIsolationVariables ele_pfIso = itEle.pfIsolationVariables();
      elecs_PFIsodbeta03_ = isolation->PFIsodBeta(ele_pfIso.sumChargedHadronPt, ele_pfIso.sumNeutralHadronEt, ele_pfIso.sumPhotonEt, ele_pfIso.sumPUPt, itEle.pt() , 0.5);
//      cout << "elec pfiso: " << ele_pfIso.sumChargedHadronPt << "   " << ele_pfIso.sumNeutralHadronEt << "   " << ele_pfIso.sumPhotonEt << "   " << ele_pfIso.sumPUPt << endl;

      PfCharHadIso04_   = itEle.userIsolation(pat::PfChargedHadronIso);
      PfPUCharHadIso04_ = itEle.userIsolation(pat::PfPUChargedHadronIso);
      PfNeuHadIso04_    = itEle.userIsolation(pat::PfNeutralHadronIso);
      PfGamIso04_       = itEle.userIsolation(pat::PfGammaIso);

      if ( PfCharHadIso04_ < 0 || PfPUCharHadIso04_ < 0 || PfNeuHadIso04_ < 0 || PfGamIso04_ < 0 )
      {
/*  
         cout << "sk ele pf check" << endl; 
         cout << "PfCharHadIso03_  test : "   << PfCharHadIso03_   << endl;
         cout << "PfPUCharHadIso03_  test : " << PfPUCharHadIso03_ << endl;
         cout << "PfNeuHadIso03_  test : "    << PfNeuHadIso03_    << endl;
         cout << "PfGamIso03_  test : "       << PfGamIso03_       << endl;
*/
      }

//      cout << " elec pfiso 04 " << itEle.userIsolation(pat::PfChargedHadronIso) << "  " << itEle.userIsolation(pat::PfNeutralHadronIso) << "   " << itEle.userIsolation(pat::PfGammaIso) 
//                                << "   " << itEle.userIsolation(pat::PfPUChargedHadronIso) << endl;
      elecs_PFIsodbeta04_ = isolation->PFIsodBeta(itEle.userIsolation(pat::PfChargedHadronIso), itEle.userIsolation(pat::PfNeutralHadronIso),
                                      itEle.userIsolation(pat::PfGammaIso), itEle.userIsolation(pat::PfPUChargedHadronIso), itEle.pt() , 0.5);
      // In CMSSW_5_x_x for Run I
      //elecs_PFIsodbeta04_ = isolation->PFIsodBeta(itEle.userIsolation(pat::PfChargedHadronIso), itEle.userIsolation(pat::PfNeutralHadronIso),
      //                                itEle.userIsolation(pat::PfGammaIso), itEle.userIsolation(pat::PfPUChargedHadronIso), itEle.pt() , 1.0);

      superclustereta_ =  itEle.superCluster()->eta(); 

      effA03_ = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03,
                                                                itEle.superCluster()->eta(), ElectronEffectiveArea::kEleEAData2012);
      effA04_ = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04,
                                                                itEle.superCluster()->eta(), ElectronEffectiveArea::kEleEAData2012);

      elecs_PFIsoRho03_ = isolation->PFIsoRho( ele_pfIso.sumChargedHadronPt, ele_pfIso.sumNeutralHadronEt, ele_pfIso.sumPhotonEt, rho, effA03_, itEle.pt() );

      elecs_PFIsoRho04_ = isolation->PFIsoRho( itEle.userIsolation(pat::PfChargedHadronIso), itEle.userIsolation(pat::PfNeutralHadronIso), itEle.userIsolation(pat::PfGammaIso), rho, effA04_, itEle.pt() );

      ssbtreeManager->Fill( "Elec_relIso03"    , elecs_relIso03_     );
      ssbtreeManager->Fill( "Elec_relIso04"    , elecs_relIso04_     );
      ssbtreeManager->Fill( "Elec_PFIsodBeta03", elecs_PFIsodbeta03_ );
      ssbtreeManager->Fill( "Elec_PFIsodBeta04", elecs_PFIsodbeta04_ );
      ssbtreeManager->Fill( "Elec_PFIsoRho03"  , elecs_PFIsoRho03_   );      
      ssbtreeManager->Fill( "Elec_PFIsoRho04"  , elecs_PFIsoRho04_   );      

      if (!(itEle.gsfTrack().isNull())) 
      {

         nmhit_ = itEle.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS); 
         gsftrack_dxy_ = itEle.gsfTrack()->dxy( v_vertex[0].position() );
         gsftrack_dz_  = itEle.gsfTrack()->dz( v_vertex[0].position() );
         eles_gsfchar_ = itEle.gsfTrack()->charge();

      }

      ssbtreeManager->Fill( "Elec_Charge_GsfTr" , eles_gsfchar_ );
      ssbtreeManager->Fill( "Elec_Track_GsfdXY" , gsftrack_dxy_ ); 
      ssbtreeManager->Fill( "Elec_Track_GsfdZ"  , gsftrack_dz_  ); 
      ssbtreeManager->Fill( "Elec_Inner_Hit"    , nmhit_        ); 

      if (!(itEle.closestCtfTrackRef().isNull()))
      {

         ctftrack_dxy_ = itEle.closestCtfTrackRef()->dxy( v_vertex[0].position() );
         ctftrack_dz_  = itEle.closestCtfTrackRef()->dz( v_vertex[0].position() );
         eles_ctfchar_ = itEle.closestCtfTrackRef()->charge();

      }

      ssbtreeManager->Fill( "Elec_Charge_CtfTr", eles_ctfchar_ );
      ssbtreeManager->Fill( "Elec_Track_CtfdXY", ctftrack_dxy_ ); 
      ssbtreeManager->Fill( "Elec_Track_CtfdZ" , ctftrack_dz_  ); 

      eles_spchar_ = itEle.scPixCharge();
      ssbtreeManager->Fill( "Elec_Charge_Px", eles_spchar_);
    
      eles_pt_ = itEle.pt();
      eles_eta_ = itEle.eta();
      eles_phi_ = itEle.phi();
      eles_energy_ = itEle.energy();
      eles_pdgid_ = itEle.pdgId();
      eles_charge_ = itEle.charge();

      if (itEle.isGsfCtfScPixChargeConsistent())
      {
         eles_chargeid_gsfctfpx_ = true;
      }

      if (itEle.isGsfCtfChargeConsistent())
      {
         eles_chargeid_gsfctf_ = true;
      }

      if (itEle.isGsfScPixChargeConsistent())
      {
         eles_chargeid_gsfpx_ = true;
      }
/*  // This Method is old version.... 
      if (itEle.electronIDs().size()==3)
      {
         mva1_ = itEle.electronIDs().at(0).second;
         mva2_ = itEle.electronIDs().at(1).second;
         mva3_ = itEle.electronIDs().at(2).second;

      }
      else 
      {
         for (unsigned int iel = 0; iel < itEle.electronIDs().size(); ++iel )
         {
            cout << " ele Id ? Name ? "<< itEle.electronIDs().at(iel).first << endl;
         }
         eles_SCB_Loose_  = itEle.electronIDs().at(4).second; //loose
         eles_SCB_Medium_ = itEle.electronIDs().at(5).second; //medium
         eles_SCB_Tight_  = itEle.electronIDs().at(6).second; //tight
         eles_SCB_Veto_   = itEle.electronIDs().at(7).second; //veto
      }*/ 

      eles_SCB_Veto_  = (*veto_id_decisions)[itele];
      eles_SCB_Loose_ = (*loose_id_decisions)[itele];
      eles_SCB_Medium_ = (*medium_id_decisions)[itele];
      eles_SCB_Tight_  = (*tight_id_decisions)[itele];
      bool elecs_SCB_HEEB = (*heep_id_decisions)[itele];

      bool isPassNonTrigMedium = (*mva_Nontrigmedium_id_decisions)[itele];
      bool isPassNonTrigTight  = (*mva_Nontrigtight_id_decisions)[itele];
      bool isPassTrigMedium = (*mva_Trigmedium_id_decisions)[itele];
      bool isPassTrigTight  = (*mva_Trigtight_id_decisions)[itele];
     
      passconversionveto1 = itEle.passConversionVeto();// pat conversion veto

      ooEmooP_ =  (1.0/itEle.ecalEnergy())*(1.0-itEle.eSuperClusterOverP()) ;

      ssbtreeManager->Fill("Elec", eles_pt_,eles_eta_,eles_phi_,eles_energy_,ele_index );

      ssbtreeManager->Fill( "Elec_Conversion"         , passconversionveto1     );
      ssbtreeManager->Fill( "Elec_SCB_Loose"          , eles_SCB_Loose_         );
      ssbtreeManager->Fill( "Elec_SCB_Medium"         , eles_SCB_Medium_        );
      ssbtreeManager->Fill( "Elec_SCB_Tight"          , eles_SCB_Tight_         );
      ssbtreeManager->Fill( "Elec_SCB_Veto"           , eles_SCB_Veto_          );
      ssbtreeManager->Fill( "Elec_MVA_NonTrigV0"      , mva1_                   );
      ssbtreeManager->Fill( "Elec_MVA_TrigNoIPV0"     , mva2_                   );
      ssbtreeManager->Fill( "Elec_MVA_TrigV0"         , mva3_                   );
      ssbtreeManager->Fill( "Elec_pdgId"              , eles_pdgid_             );
      ssbtreeManager->Fill( "Elec_Charge"             , eles_charge_            );
      ssbtreeManager->Fill( "Elec_ChargeId_GsfCtfPx"  , eles_chargeid_gsfctfpx_ );
      ssbtreeManager->Fill( "Elec_ChargeId_GsfCtf"    , eles_chargeid_gsfctf_   );
      ssbtreeManager->Fill( "Elec_ChargeId_GsfPx"     , eles_chargeid_gsfpx_    );
      ssbtreeManager->Fill( "Elec_Supercluster_Eta"   , superclustereta_        );
      ssbtreeManager->Fill( "Elec_SCB_dEtaIn"         , itEle.deltaEtaSuperClusterTrackAtVtx() );
      ssbtreeManager->Fill( "Elec_SCB_dPhiIn"         , itEle.deltaPhiSuperClusterTrackAtVtx() );
      ssbtreeManager->Fill( "Elec_SCB_sigmaIetaIeta"  , itEle.full5x5_sigmaIetaIeta() );
      ssbtreeManager->Fill( "Elec_SCB_hOverE"         , itEle.hadronicOverEm() );
      ssbtreeManager->Fill( "Elec_SCB_HEEP"           , elecs_SCB_HEEB          );
      ssbtreeManager->Fill( "Elec_MVANonTrig_Medium"  , isPassNonTrigMedium            );
      ssbtreeManager->Fill( "Elec_MVANonTrig_Tight"   , isPassNonTrigTight             );
      ssbtreeManager->Fill( "Elec_MVATrig_Medium"     , isPassTrigMedium            );
      ssbtreeManager->Fill( "Elec_MVATrig_Tight"      , isPassTrigTight             );
      ssbtreeManager->Fill( "Elec_SCB_ooEmooP"        , ooEmooP_                );

      ele_index++;

   }
   ssbtreeManager->Fill( "Elec_Count" , ele_index );
  

   /////////////////////////
   /// Jets Information  ///
   /////////////////////////


   // Utility for Jet ID
   PFJetIDSelectionFunctor LooseJetID(pfLooseJetIDparam);
   pat::strbitset looseJetIdSel = LooseJetID.getBitTemplate();

   PFJetIDSelectionFunctor TightJetID(pfTightJetIDparam);
   pat::strbitset tightJetIdSel = TightJetID.getBitTemplate();

   jet_index = 0;
 
   Handle<pat::JetCollection> jets;
   iEvent.getByToken(jetToken_, jets);

   Handle<pat::JetCollection> jetpuppis;
   iEvent.getByToken(jetpuppiToken_, jetpuppis);

   JetCorrectionUncertainty* jetcorr_uncertainty(0);
   //jetcorr_uncertainty = new JetCorrectionUncertainty("./JECDir/Summer15_25nsV2_MC/Summer15_25nsV2_MC_Uncertainty_AK4PFchs.txt"); // if you want to use your private Uncertainty file, you can use this line.
   jetcorr_uncertainty = new JetCorrectionUncertainty(jetUncTag);

   for ( const pat::Jet &itJet : *jetpuppis ) 
   {
      if ( itJet.energy() < 20.0 || abs( itJet.eta() ) > 2.5 ) {continue;} // import at python config
//      cout << "jepuppiidx ? " << jepuppiidx << " Jet puppi energy ? " << itJet.energy() << endl;
   }
   for ( const pat::Jet &itJet : *jets ) 
   {

      ////////////////////////////////////
      /// selection of resaonable jets ///
      ////////////////////////////////////
      
      if ( itJet.pt() < 20.0 || abs( itJet.eta() ) > 2.5 ) {continue;} // import at python config

      jets_pt_          = -9999;
      jets_eta_         = -9999;
      jets_phi_         = -9999;
      jets_energy_      = -9999;
      jets_pdgid_       = 0;
      jets_isJet_       = false;
      jets_bDisc_       = -9999;
      jets_charge_      = -9999;
      jets_pfjetid_     = -999;
      jets_mvapujetid_  = -999;
      jets_mvapujet_    = -999;
      isLoosejetid_pass = false;
      isTightjetid_pass = false;
      jets_UncEnUp_     = 0;
      jets_UncEnDown_   = 0;


      ///For PFJetIDSelectionFunctor
      looseJetIdSel.set(false);
      isLoosejetid_pass = LooseJetID(itJet, looseJetIdSel);

      tightJetIdSel.set(false);
      isTightjetid_pass = TightJetID(itJet, tightJetIdSel);

      if (isTightjetid_pass)
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
      }


      ////////////////////////////
      /// End of jet selection ///
      ////////////////////////////

      float mva   = -999;
          
      jets_pt_ = itJet.pt();
      jets_eta_ = itJet.eta();
      jets_phi_ = itJet.phi();
      jets_energy_ = itJet.energy();
      jets_charge_ = itJet.charge();

      /// works only for JPT or PF jet
      jets_mvapujet_ = mva;
      jets_pdgid_ = itJet.pdgId();
      jets_isJet_ = itJet.isJet();
      jets_bDisc_ = itJet.bDiscriminator(csvBJetTag);


      /// Calculating JetEnergy Uncertainty
      jetcorr_uncertainty->setJetEta(jets_eta_);
      jetcorr_uncertainty->setJetPt(jets_pt_);
      double unc = jetcorr_uncertainty->getUncertainty(true);
      jets_UncEnUp_ = (1. +  unc);
      jetcorr_uncertainty->setJetEta(jets_eta_);
      jetcorr_uncertainty->setJetPt(jets_pt_);
      unc = jetcorr_uncertainty->getUncertainty(false);
      jets_UncEnDown_ = (1. -  unc);

      ssbtreeManager->Fill( "Jet", jets_pt_, jets_eta_, jets_phi_, jets_energy_, jet_index);
      ssbtreeManager->Fill( "Jet_Charge"       , jets_charge_     );
      ssbtreeManager->Fill( "Jet_isJet"        , jets_isJet_      );
      ssbtreeManager->Fill( "Jet_bDisc"        , jets_bDisc_      );
      ssbtreeManager->Fill( "Jet_PFId"         , jets_pfjetid_    );
      ssbtreeManager->Fill( "Jet_PileUpId"     , jets_mvapujetid_ );
      ssbtreeManager->Fill( "Jet_PileUpMVA"    , jets_mvapujet_   );
      ssbtreeManager->Fill( "Jet_EnShiftedUp"  , jets_UncEnUp_    );
      ssbtreeManager->Fill( "Jet_EnShiftedDown", jets_UncEnDown_  );
     
  
      jet_index++;

   }
   ssbtreeManager->Fill( "Jet_Count", jet_index );


   ///////////////////////////////
   /////// MET Information ///////
   ///////////////////////////////
   MET_index=0;

   Handle<pat::METCollection> mets;
   iEvent.getByToken(metToken_, mets);
   //const pat::MET &met = mets->front();

   for (const pat::MET &itMet : *mets)
   {
      ssbtreeManager->Fill( "MET" , itMet.pt(), 0, itMet.phi(), 0, MET_index );
/*      for ( unsigned int unc = 0; unc < 12; ++unc ) 
      {
//        cout << "METUncertainty" << "unc ? " << unc << " -- "<< itMet.shiftedPt( pat::MET::METUncertainty(unc) )<< endl;
      }*/
//      cout << "nominal pt : " << itMet.pt() << endl;
      MET_index++;
   }

   int METNoHF_index = 0;
   Handle<pat::METCollection> metnohfs;
   iEvent.getByToken(metnoHFToken_, metnohfs);

   for (const pat::MET &itMetno : *metnohfs)
   {
//      ssbtreeManager->Fill( "MET" , itMet.pt(), 0, itMet.phi(), 0, MET_index );
      ssbtreeManager->Fill( "METNoHF" , itMetno.pt(), 0, itMetno.phi(), 0, METNoHF_index );
      METNoHF_index++;
   }

   /// MET Jet Energy Up ///
   Handle<pat::METCollection> metjetenup;
   iEvent.getByToken(metJetEnUpToken_, metjetenup);
   for (const pat::MET &itMetno : *metjetenup)
   {
//      cout << "Up   : " << itMetno.pt() << endl;
      ssbtreeManager->Fill( "MET_JetEnShiftedUp_PT"  , itMetno.pt()  );
      ssbtreeManager->Fill( "MET_JetEnShiftedUp_Phi" , itMetno.phi() );
   }

   /// MET Jet Energy Up ///
   Handle<pat::METCollection> metjetendown;
   iEvent.getByToken(metJetEnDownToken_, metjetendown);
   for (const pat::MET &itMetno : *metjetendown)
   {
//      cout << "Down : " << itMetno.pt() << endl;
      ssbtreeManager->Fill( "MET_JetEnShiftedDown_PT"  , itMetno.pt()  );
      ssbtreeManager->Fill( "MET_JetEnShiftedDown_Phi" , itMetno.phi() );
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
   }

   /// Isolation calculation
   isolation = new SSBIsoCal(); 

   /// PDF
   for (unsigned int k=1; k<=pdfSets.size(); k++) 
   {
      LHAPDF::initPDFSet(k, pdfSets[k-1]);
   }

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
SSBAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
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

    /// PYTHIA Event Histroy !! 
    edm::Handle<GenParticleCollection> genParticles;
    iEvent.getByLabel(genParInfoTag, genParticles); /* get genParticle information */

    GenParticleCollection::const_iterator itGenParBegin = genParticles->begin();
    if (itGenParBegin->status() == 4) {
	PythiaVersion = 8;
    }
    else {
	PythiaVersion = 6;
    } /* check pythia version (status code is different) */

    vector<const reco::Candidate *> cands; /* reco::Candidate vector (mother and daugher function return reco::Candidate) */
    for ( GenParticleCollection::const_iterator itGenParIndex = genParticles->begin(); itGenParIndex != genParticles->end(); ++itGenParIndex )
    {
	cands.push_back(&*itGenParIndex);
    } /* use for mother and daughter index */

   InitializeGenPar(); /* initialize vector and map */

   for ( GenParticleCollection::const_iterator itGenPar = genParticles->begin(); itGenPar != genParticles->end(); itGenPar++ )
   {

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

	//if ( (PythiaVersion == 8 && (itGenPar->status() == 4 || (itGenPar->status() > 20 && itGenPar->status() < 24))) || /* with proton */
	if ( (PythiaVersion == 8 && (itGenPar->status() > 20 && itGenPar->status() < 24)) || /* without proton */
	     (PythiaVersion == 6 && itGenPar->status() == 3) ){ /* in pythia6, proton status is also 3 */
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
    if (PythiaVersion == 6) {
	TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(1)));
	TreePar.erase(find(TreePar.begin(), TreePar.end(), TreePar.at(0)));
    } /* remove proton in Pythia6 */

    if ( ((AllParInfo[AllParDau[0].at(0)].at(1) > 20) && (AllParInfo[AllParDau[0].at(0)].at(1) < 24)) || ((AllParInfo[AllParDau[1].at(0)].at(1) > 20) && (AllParInfo[AllParDau[1].at(0)].at(1) < 24)) ) {
	isMINIAOD = true;
    }
    else {
	isMINIAOD = false;
    } /* 'p+ to Status 2X particle directly' means MINIAOD */

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

    if (isMINIAOD == true) {
	SelectedPar.push_back(0);
	SelectedPar.push_back(1);
    }
    else {
	int FirstPtoFirstTMom = IndexLinker(AllParDau, 0, 1, AllParMom[SelectedpdgId[6]].at(0));//, 0, 0, true);
	int SecondPtoSecondTMom = IndexLinker(AllParDau, 1, 1, AllParMom[SelectedpdgId[6]].at(1));//, 0, 0, true); 
	if (FirstPtoFirstTMom != -1 && SecondPtoSecondTMom != -1) {
	    SelectedPar.push_back(0);
	    SelectedPar.push_back(1);
	    SelectedPar.push_back(FirstPtoFirstTMom);
	    SelectedPar.push_back(SecondPtoSecondTMom);
	}
	else {
	    int FirstPtoSecondTMom = IndexLinker(AllParDau, 0, 1, AllParMom[SelectedpdgId[6]].at(1));//, 0, 0, true); 
	    int SecondPtoFirstTMom = IndexLinker(AllParDau, 1, 1, AllParMom[SelectedpdgId[6]].at(0));//, 0, 0, true);
	    SelectedPar.push_back(1);
	    SelectedPar.push_back(0);
	    SelectedPar.push_back(SecondPtoFirstTMom);
	    SelectedPar.push_back(FirstPtoSecondTMom);
	}

	SelectedPar.push_back(AllParMom[SelectedpdgId[6]].at(0));
	SelectedPar.push_back(AllParMom[SelectedpdgId[6]].at(1));
	TreePar.erase(find(TreePar.begin(), TreePar.end(), AllParMom[SelectedpdgId[6]].at(0)));
	TreePar.erase(find(TreePar.begin(), TreePar.end(), AllParMom[SelectedpdgId[6]].at(1)));
    }

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

    /* SelectedPar_AOD  : {p+, p+, first_p+_daughter, second_pt+_daughter, ttbar_mother_form_first_p+_daughter, ttbar_mother_form_second_p+_daughter,
			   0   1   2                  3                    4                                    5
			   t, tbar, w+, b, w-, bbar, w+_first_daughter, w+_second_daughter, w-_first_daughter, w-_second_daughter}
			   6  7     8   9  10  11    12                 13                  14                 15 */
    /* SelectedPar_MINI : {p+, p+, 
			   0   1  
			   t, tbar, w+, b, w-, bbar, w+_first_daughter, w+_second_daughter, w-_first_daughter, w-_second_daughter}
			   2  3     4   5  6   7     8                  9                   10                 11 */

    if ((isMINIAOD == false && SelectedPar.size() != 16) || (isMINIAOD == true && SelectedPar.size() != 12)) {
	cerr << "!!!!! Signal Sample : SelectedPar Error !!!!!" << endl;
	cerr << "!!!!! Signal Sample : SelectedPar Error !!!!!" << endl;
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

    if (isMINIAOD == false) { /* AOD Fill */

	FillGenPar(SelectedPar.at(0), -1, -1, SelectedPar.at(2), -1, itGenParBegin, ssbtreeManager);
	FillGenPar(SelectedPar.at(1), -1, -1, SelectedPar.at(3), -1, itGenParBegin, ssbtreeManager);

	FillGenPar(SelectedPar.at(2), SelectedPar.at(0), -1, SelectedPar.at(4), -1, itGenParBegin, ssbtreeManager);
	FillGenPar(SelectedPar.at(3), SelectedPar.at(1), -1, SelectedPar.at(5), -1, itGenParBegin, ssbtreeManager);

	FillGenPar(SelectedPar.at(4), SelectedPar.at(2), -1, SelectedPar.at(6), SelectedPar.at(7), itGenParBegin, ssbtreeManager);
	FillGenPar(SelectedPar.at(5), SelectedPar.at(3), -1, SelectedPar.at(6), SelectedPar.at(7), itGenParBegin, ssbtreeManager);

	FillGenPar(SelectedPar.at(6), SelectedPar.at(4), SelectedPar.at(5), SelectedPar.at(8), SelectedPar.at(9), itGenParBegin, ssbtreeManager);
	FillGenPar(SelectedPar.at(7), SelectedPar.at(4), SelectedPar.at(5), SelectedPar.at(10), SelectedPar.at(11), itGenParBegin, ssbtreeManager);

	FillGenPar(SelectedPar.at(8), SelectedPar.at(6), -1, SelectedPar.at(12), SelectedPar.at(13), itGenParBegin, ssbtreeManager);
	FillGenPar(SelectedPar.at(9), SelectedPar.at(6), -1, -1, -1, itGenParBegin, ssbtreeManager);
 
	FillGenPar(SelectedPar.at(10), SelectedPar.at(7), -1, SelectedPar.at(14), SelectedPar.at(15), itGenParBegin, ssbtreeManager);
	FillGenPar(SelectedPar.at(11), SelectedPar.at(7), -1, -1, -1, itGenParBegin, ssbtreeManager);

	FillGenPar(SelectedPar.at(12), SelectedPar.at(8), -1, -1, -1, itGenParBegin, ssbtreeManager);
	FillGenPar(SelectedPar.at(13), SelectedPar.at(8), -1, -1, -1, itGenParBegin, ssbtreeManager);
 
	FillGenPar(SelectedPar.at(14), SelectedPar.at(10), -1, -1, -1, itGenParBegin, ssbtreeManager);
	FillGenPar(SelectedPar.at(15), SelectedPar.at(10), -1, -1, -1, itGenParBegin, ssbtreeManager);

    }
    else { /* MINIAOD Fill */

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
    }

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
	    FillGenPar((FinaltoSel->second).at(DauIndex), FinaltoSel->first, -1, -1, -1, itGenParBegin, ssbtreeManager); /* fill final state */
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
			++ChannelLeptonFinal;
			if (Lepton_Dau_pdgId < 14) ChannelIndexFinal += Lepton_Dau_pdgId;
			if (Lepton_Dau_pdgId > 14) ChannelIndexFinal -= Lepton_Dau_pdgId;
		    }
		}
	    }
	}
    }

    ssbtreeManager->Fill( "GenPar_Count"          , genPar_index );
    ssbtreeManager->Fill( "Channel_Idx"           , ChannelIndex );
    ssbtreeManager->Fill( "Channel_Idx_Final"     , ChannelIndexFinal );
    ssbtreeManager->Fill( "Channel_Lepton_Count"       , ChannelLepton );
    ssbtreeManager->Fill( "Channel_Lepton_Count_Final" , ChannelLeptonFinal );

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
    iEvent.getByLabel(genJetInfoTag,genJets);

    genJet_index = 0;

    for (GenJetCollection::const_iterator itgJet = genJets->begin() ; itgJet !=genJets->end(); itgJet++) {
	ssbtreeManager->Fill( "GenJet", (*itgJet).pt(), (*itgJet).eta(), (*itgJet).phi(), (*itgJet).energy(), genJet_index );
	genJet_index++;   
    }

    ssbtreeManager->Fill( "GenJet_Count", genJet_index );

}
//define this as a plug-in
DEFINE_FWK_MODULE(SSBAnalyzer);
