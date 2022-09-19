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
// Original Author: Seungkyu Ha, Sehwook Lee, Suyoung Choi, Jaehoon Lim. Korea University. SSB developers. 
//         Created:  Mon Dec 29 10:42:18 KST 2014
// $Id$
//
//

#include "CMSAnalyses/SSBAnalyzer/plugins/SSBGenInfor.h"

SSBGenInfor::SSBGenInfor(const edm::ParameterSet& iConfig)
{
   isMC = iConfig.getParameter<bool>("isMCTag");
//   isSignal = iConfig.getParameter<bool>("isSignal");

   //now do what ever initialization is needed
   if (isMC == true) 
   {
      genEvnInfoTag = consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genEvnTag"));
      genParInfoTag = consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParTag"));
      genJetInfoTag = consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genJetTag"));
      genMETInfoTag = consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("genMETTag"));
      cout << "test -----" << endl;
   }

}


SSBGenInfor::~SSBGenInfor()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}
//
// member functions
//
// ------------ method called for each event  ------------
void
SSBGenInfor::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    ssbtreeManager->GenInitializeVariables();

    Event = -999;
    Run = -999;
    Lumi = -999;
    isData = false;

    Event = iEvent.id().event();
    Run = iEvent.id().run();
    Lumi = iEvent.id().luminosityBlock();
    isData = iEvent.isRealData();

    ssbtreeManager->Fill( "Info_EventNumber", Event  );
    ssbtreeManager->Fill( "Info_RunNumber"  , Run    ); 
    ssbtreeManager->Fill( "Info_Luminosity" , Lumi   ); 
    ssbtreeManager->Fill( "Info_isData"     , isData ); 
    if (isData == false )
    {
       //////////////////////////////
       /// Generator Event Weight ///
       //////////////////////////////
       edm::Handle < GenEventInfoProduct > genEvtInfoProduct;
       iEvent.getByToken(genEvnInfoTag, genEvtInfoProduct);
       {
          ssbtreeManager->Fill( "Gen_EventWeight"     , genEvtInfoProduct->weight() );  
       }
       ////////////////////////////////
       /// Generator Particle Info. ///
       ////////////////////////////////

       GenPar(iEvent, ssbtreeManager);
       GenJet(iEvent, ssbtreeManager);
       GenMET(iEvent, ssbtreeManager);
    }
    ssbtreeManager->FillNtuple();
   
}

// ------------ method called once each job just before starting event loop  ------------
void 
SSBGenInfor::beginJob()
{
   ssbtree = ssbfs->make<TTree>("SSBGenTree", "Tree for Physics Analyses at CMS");
   ssbtreeManager = new SSBTreeManager();
   ssbtreeManager->GenBook(ssbtree);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SSBGenInfor::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
SSBGenInfor::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
SSBGenInfor::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
SSBGenInfor::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
SSBGenInfor::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SSBGenInfor::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
SSBGenInfor::GenPar(const edm::Event& iEvent, SSBTreeManager* ssbtreeManager) {

    ////////////////////////////////////////////
    /// Generator Level Particle Information ///
    ////////////////////////////////////////////

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
SSBGenInfor::IndexLinker(map_i IndexMap, int start_index, int target_depth, int target_index, int target_pdgid, int target_status, bool PrintError, int LoopDepth){
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
SSBGenInfor::InitializeGenPar(){
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
SSBGenInfor::FillGenPar(int GenIndex, int FirstMother, int SecondMother, int FirstDaughter, int SecondDaughter, GenParticleCollection::const_iterator itGenParFill, SSBTreeManager* ssbtreeManager){

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
SSBGenInfor::GenJet(const edm::Event& iEvent, SSBTreeManager* ssbtreeManager) {

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
SSBGenInfor::GenMET(const edm::Event& iEvent, SSBTreeManager* ssbtreeManager) {

    //////////////////////////////////////
    /// Generator Level MET informaton ///
    //////////////////////////////////////

    edm::Handle<GenMETCollection> genMETs;
    //iEvent.getByLabel(genMETInfoTag,genMETs);
    iEvent.getByToken(genMETInfoTag,genMETs);

    genMET_index = 0;

    for (GenMETCollection::const_iterator itgMET = genMETs->begin() ; itgMET !=genMETs->end(); itgMET++) {
	ssbtreeManager->Fill( "GenMET", (*itgMET).genMET()->pt(), 0, (*itgMET).genMET()->phi(), 0, genMET_index );
	genMET_index++;
    }
    ssbtreeManager->Fill( "GenMET_Count", genMET_index );

}

void
SSBGenInfor::TestCout(const edm::Event& iEvent, SSBTreeManager* ssbtreeManager) {
   std::cout<< "TestCout Working ../." << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(SSBGenInfor);
