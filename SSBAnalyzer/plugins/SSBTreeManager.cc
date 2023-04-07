// SSBTMMaker v2.02
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBTreeManager.h"

void SSBTreeManager::FillNtuple(){
   ssbtree->Fill();
}

void SSBTreeManager::Fill(st VariableName, double pt, double eta, double phi, double e, int index){
    if ((it_VariableBox_LorentzVector = VariableBox_LorentzVector.find(VariableName.c_str())) != VariableBox_LorentzVector.end()){
	new ((*VariableBox_LorentzVector[VariableName.c_str()])[index]) TLorentzVector();
	( (TLorentzVector*)VariableBox_LorentzVector[VariableName.c_str()]->At(index) )->SetPtEtaPhiE(pt, eta, phi, e);
    }
    else {
	std::cout << "Fill_LV_Int Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, double pt, double eta, double phi, double e, unsigned int index){
    if ((it_VariableBox_LorentzVector = VariableBox_LorentzVector.find(VariableName.c_str())) != VariableBox_LorentzVector.end()){
	new ((*VariableBox_LorentzVector[VariableName.c_str()])[index]) TLorentzVector();
	( (TLorentzVector*)VariableBox_LorentzVector[VariableName.c_str()]->At(index) )->SetPtEtaPhiE(pt, eta, phi, e);
    }
    else {
	std::cout << "Fill_LV_UInt Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, bool VariableBool){
    if ((it_VariableBox_Bool = VariableBox_Bool.find(VariableName.c_str())) != VariableBox_Bool.end()){
	VariableBox_Bool[VariableName.c_str()] = VariableBool;
	//std::cout << "Val : " << VariableBool << " == " << it_VariableBox_Bool->second << std::endl;
    }
    else if ((it_VectorBox_Bool = VectorBox_Bool.find(VariableName.c_str())) != VectorBox_Bool.end()){
        (it_VectorBox_Bool->second).push_back(VariableBool);
	//std::cout << "Vec : " << VariableBool << " == " << (it_VectorBox_Bool->second).back() << std::endl;
    }
    else {
	std::cout << "Fill_Bool Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, int VariableInt){
    if ((it_VariableBox_Int = VariableBox_Int.find(VariableName.c_str())) != VariableBox_Int.end()){
	VariableBox_Int[VariableName.c_str()] = VariableInt;
	//std::cout << "Val : " << VariableInt << " == " << it_VariableBox_Int->second << std::endl;
    }
    else if ((it_VectorBox_Int = VectorBox_Int.find(VariableName.c_str())) != VectorBox_Int.end()){
        (it_VectorBox_Int->second).push_back(VariableInt);
	//std::cout << "Vec : " << VariableInt << " == " << (it_VectorBox_Int->second).back() << std::endl;
    }
    else {
	std::cout << "Fill_Int Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, unsigned int VariableUInt){
    if ((it_VariableBox_UInt = VariableBox_UInt.find(VariableName.c_str())) != VariableBox_UInt.end()){
	VariableBox_UInt[VariableName.c_str()] = VariableUInt;
	//std::cout << "Val : " << VariableUInt << " == " << it_VariableBox_UInt->second << std::endl;
    }
    else if ((it_VectorBox_UInt = VectorBox_UInt.find(VariableName.c_str())) != VectorBox_UInt.end()){
        (it_VectorBox_UInt->second).push_back(VariableUInt);
	//std::cout << "Vec : " << VariableUInt << " == " << (it_VectorBox_UInt->second).back() << std::endl;
    }
    else {
	std::cout << "Fill_UInt Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, float VariableFloat){
    if ((it_VariableBox_Float = VariableBox_Float.find(VariableName.c_str())) != VariableBox_Float.end()){
	VariableBox_Float[VariableName.c_str()] = VariableFloat;
	//std::cout << "Val : " << VariableFloat << " == " << it_VariableBox_Float->second << std::endl;
    }
    else if ((it_VectorBox_Float = VectorBox_Float.find(VariableName.c_str())) != VectorBox_Float.end()){
        (it_VectorBox_Float->second).push_back(VariableFloat);
	//std::cout << "Vec : " << VariableFloat << " == " << (it_VectorBox_Float->second).back() << std::endl;
    }
    else {
	std::cout << "Fill_Float Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, double VariableDouble){
    if ((it_VariableBox_Double = VariableBox_Double.find(VariableName.c_str())) != VariableBox_Double.end()){
	VariableBox_Double[VariableName.c_str()] = VariableDouble;
	//std::cout << "Val : " << VariableDouble << " == " << it_VariableBox_Double->second << std::endl;
    }
    else if ((it_VectorBox_Double = VectorBox_Double.find(VariableName.c_str())) != VectorBox_Double.end()){
        (it_VectorBox_Double->second).push_back(VariableDouble);
	//std::cout << "Vec : " << VariableDouble << " == " << (it_VectorBox_Double->second).back() << std::endl;
    }
    else {
	std::cout << "Fill_Double Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, st VariableString){
    if ((it_VariableBox_String = VariableBox_String.find(VariableName.c_str())) != VariableBox_String.end()){
	VariableBox_String[VariableName.c_str()] = VariableString;
	//std::cout << "Val : " << VariableString << " == " << it_VariableBox_String->second << std::endl;
    }
    else if ((it_VectorBox_String = VectorBox_String.find(VariableName.c_str())) != VectorBox_String.end()){
        (it_VectorBox_String->second).push_back(VariableString);
	//std::cout << "Vec : " << VariableString << " == " << (it_VectorBox_String->second).back() << std::endl;
    }
    else {
	std::cout << "Fill_String Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, vec_b VectorBool){
    if ((it_VectorBox_Bool = VectorBox_Bool.find(VariableName.c_str())) != VectorBox_Bool.end()){
        (it_VectorBox_Bool->second) = VectorBool;
    }
    else {
	std::cout << "Fill_VectorBool Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, vec_i VectorInt){
    if ((it_VectorBox_Int = VectorBox_Int.find(VariableName.c_str())) != VectorBox_Int.end()){
        (it_VectorBox_Int->second) = VectorInt;
    }
    else {
	std::cout << "Fill_VectorInt Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, vec_ui VectorUInt){
    if ((it_VectorBox_UInt = VectorBox_UInt.find(VariableName.c_str())) != VectorBox_UInt.end()){
        (it_VectorBox_UInt->second) = VectorUInt;
    }
    else {
	std::cout << "Fill_VectorUInt Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, vec_f VectorFloat){
    if ((it_VectorBox_Float = VectorBox_Float.find(VariableName.c_str())) != VectorBox_Float.end()){
        (it_VectorBox_Float->second) = VectorFloat;
    }
    else {
	std::cout << "Fill_VectorFloat Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, vec_d VectorDouble){
    if ((it_VectorBox_Double = VectorBox_Double.find(VariableName.c_str())) != VectorBox_Double.end()){
        (it_VectorBox_Double->second) = VectorDouble;
    }
    else {
	std::cout << "Fill_VectorDouble Error : " << VariableName << std::endl;
    }
}

void SSBTreeManager::Fill(st VariableName, vec_s VectorString){
    if ((it_VectorBox_String = VectorBox_String.find(VariableName.c_str())) != VectorBox_String.end()){
        (it_VectorBox_String->second) = VectorString;
    }
    else {
	std::cout << "Fill_VectorString Error : " << VariableName << std::endl;
    }
}


SSBTreeManager::SSBTreeManager(){

    VariableBox_LorentzVector["GenPar"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["GenJet"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["GenMET"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["GenBJet"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["GenBHad"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["Muon"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["GenMuon"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["Elec"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["Photon"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["Jet"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["MET"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["METEGClean"] = new TClonesArray("TLorentzVector"); // DATA
    VariableBox_LorentzVector["METMUEGClean"] = new TClonesArray("TLorentzVector"); // DATA
    VariableBox_LorentzVector["METMUEGCleanCor"] = new TClonesArray("TLorentzVector");// DATA
    VariableBox_LorentzVector["METMUCleanCor"] = new TClonesArray("TLorentzVector");// MC
    VariableBox_LorentzVector["METUnCor"] = new TClonesArray("TLorentzVector"); // DATA
    VariableBox_LorentzVector["GenTop"] = new TClonesArray("TLorentzVector");
    VariableBox_LorentzVector["GenAnTop"] = new TClonesArray("TLorentzVector");
}

SSBTreeManager::~SSBTreeManager(){


}

void SSBTreeManager::Book(TTree* tree){

    ssbtree = tree;
    ssbtree->Branch("Info_EventNumber", &VariableBox_Int["Info_EventNumber"], "Info_EventNumber/I");
    ssbtree->Branch("Info_Luminosity", &VariableBox_Int["Info_Luminosity"], "Info_Luminosity/I");
    ssbtree->Branch("Info_RunNumber", &VariableBox_Int["Info_RunNumber"], "Info_RunNumber/I");
    ssbtree->Branch("Info_isData", &VariableBox_Bool["Info_isData"], "Info_isData/B");

    ssbtree->Branch("Channel_Idx", &VariableBox_Int["Channel_Idx"], "Channel_Idx/I");
    ssbtree->Branch("Channel_Idx_Final", &VariableBox_Int["Channel_Idx_Final"], "Channel_Idx_Final/I");
    ssbtree->Branch("Channel_Lepton_Count", &VariableBox_Int["Channel_Lepton_Count"], "Channel_Lepton_Count/I");
    ssbtree->Branch("Channel_Lepton_Count_Final", &VariableBox_Int["Channel_Lepton_Count_Final"], "Channel_Lepton_Count_Final/I");
    ssbtree->Branch("Channel_Jets", &VariableBox_Int["Channel_Jets"], "Channel_Jets/I");
    ssbtree->Branch("Channel_Jets_Abs", &VariableBox_Int["Channel_Jets_Abs"], "Channel_Jets_Abs/I");

    ssbtree->Branch("METFilter_Name", &VectorBox_String["METFilter_Name"]);
    ssbtree->Branch("METFilter_isError", &VectorBox_Bool["METFilter_isError"]);
    ssbtree->Branch("METFilter_isPass", &VectorBox_Bool["METFilter_isPass"]);
    ssbtree->Branch("METFilter_isRun", &VectorBox_Bool["METFilter_isRun"]);
    ssbtree->Branch("METFilterAdd_Name", &VectorBox_String["METFilterAdd_Name"]);
    ssbtree->Branch("METFilterAdd_isPass", &VectorBox_Bool["METFilterAdd_isPass"]);

    ssbtree->Branch("Elec", "TClonesArray", &VariableBox_LorentzVector["Elec"], 32000, 0);
    VariableBox_LorentzVector["Elec"]->BypassStreamer();

    ssbtree->Branch("Elec_Charge", &VectorBox_Int["Elec_Charge"]);
    ssbtree->Branch("Elec_ChargeId_GsfCtf", &VectorBox_Bool["Elec_ChargeId_GsfCtf"]);
    ssbtree->Branch("Elec_ChargeId_GsfCtfPx", &VectorBox_Bool["Elec_ChargeId_GsfCtfPx"]);
    ssbtree->Branch("Elec_ChargeId_GsfPx", &VectorBox_Bool["Elec_ChargeId_GsfPx"]);
    ssbtree->Branch("Elec_Charge_CtfTr", &VectorBox_Double["Elec_Charge_CtfTr"]);
    ssbtree->Branch("Elec_Charge_GsfTr", &VectorBox_Double["Elec_Charge_GsfTr"]);
    ssbtree->Branch("Elec_Charge_Px", &VectorBox_Double["Elec_Charge_Px"]);
    ssbtree->Branch("Elec_Conversion", &VectorBox_Bool["Elec_Conversion"]);
    ssbtree->Branch("Elec_Count", &VariableBox_Int["Elec_Count"], "Elec_Count/I");
    ssbtree->Branch("Elec_Id_Loose", &VectorBox_Int["Elec_Id_Loose"]);
    ssbtree->Branch("Elec_Id_RobustHighEnergy", &VectorBox_Int["Elec_Id_RobustHighEnergy"]);
    ssbtree->Branch("Elec_Id_RobustLoose", &VectorBox_Int["Elec_Id_RobustLoose"]);
    ssbtree->Branch("Elec_Id_RobustTight", &VectorBox_Int["Elec_Id_RobustTight"]);
    ssbtree->Branch("Elec_Id_Tight", &VectorBox_Int["Elec_Id_Tight"]);
    ssbtree->Branch("Elec_Inner_Hit", &VectorBox_Int["Elec_Inner_Hit"]);
    ssbtree->Branch("Elec_MVA_Medium", &VectorBox_Bool["Elec_MVA_Medium"]);
    ssbtree->Branch("Elec_MVA_Tight", &VectorBox_Bool["Elec_MVA_Tight"]);
    ssbtree->Branch("Elec_MVA_HZZ", &VectorBox_Bool["Elec_MVA_HZZ"]);
    ssbtree->Branch("Elec_MVA_Values", &VectorBox_Float["Elec_MVA_Values"]);
    ssbtree->Branch("Elec_MVA_Categories", &VectorBox_Int["Elec_MVA_Categories"]);
    ssbtree->Branch("Elec_MVA_HZZ_Values", &VectorBox_Float["Elec_MVA_HZZ_Values"]);
    ssbtree->Branch("Elec_MVA_HZZ_Categories", &VectorBox_Int["Elec_MVA_HZZ_Categories"]);
    //ssbtree->Branch("Elec_MVATrig_Tight", &VectorBox_Bool["Elec_MVATrig_Tight"]);
    //ssbtree->Branch("Elec_MVA_NonTrigV0", &VectorBox_Float["Elec_MVA_NonTrigV0"]);
    //ssbtree->Branch("Elec_MVA_TrigNoIPV0", &VectorBox_Float["Elec_MVA_TrigNoIPV0"]);
    //ssbtree->Branch("Elec_MVA_TrigV0", &VectorBox_Float["Elec_MVA_TrigV0"]);
    ssbtree->Branch("Elec_PFIsoRho03", &VectorBox_Double["Elec_PFIsoRho03"]);
    ssbtree->Branch("Elec_PFIsoRho04", &VectorBox_Double["Elec_PFIsoRho04"]);
    ssbtree->Branch("Elec_PFIsoValid", &VectorBox_Bool["Elec_PFIsoValid"]);
    ssbtree->Branch("Elec_PFIsodBeta03", &VectorBox_Double["Elec_PFIsodBeta03"]);
    ssbtree->Branch("Elec_PFIsodBeta04", &VectorBox_Double["Elec_PFIsodBeta04"]);
    ssbtree->Branch("Elec_SCB_Loose", &VectorBox_Bool["Elec_SCB_Loose"]);
    ssbtree->Branch("Elec_SCB_Medium", &VectorBox_Bool["Elec_SCB_Medium"]);
    ssbtree->Branch("Elec_SCB_Tight", &VectorBox_Bool["Elec_SCB_Tight"]);
    ssbtree->Branch("Elec_SCB_Veto", &VectorBox_Bool["Elec_SCB_Veto"]);
    ssbtree->Branch("Elec_SCB_dEtaIn", &VectorBox_Float["Elec_SCB_dEtaIn"]);
    ssbtree->Branch("Elec_SCB_dPhiIn", &VectorBox_Float["Elec_SCB_dPhiIn"]);
    ssbtree->Branch("Elec_SCB_hOverE", &VectorBox_Float["Elec_SCB_hOverE"]);
    ssbtree->Branch("Elec_SCB_HEEP", &VectorBox_Bool["Elec_SCB_HEEP"]);
    ssbtree->Branch("Elec_SCB_ooEmooP", &VectorBox_Float["Elec_SCB_ooEmooP"]);
    ssbtree->Branch("Elec_SCB_sigmaIetaIeta", &VectorBox_Float["Elec_SCB_sigmaIetaIeta"]);
    ssbtree->Branch("Elec_ScaleUp", &VectorBox_Double["Elec_ScaleUp"]);
    ssbtree->Branch("Elec_ScaleDown", &VectorBox_Double["Elec_ScaleDown"]);
    ssbtree->Branch("Elec_SigmaUp", &VectorBox_Double["Elec_SigmaUp"]);
    ssbtree->Branch("Elec_SigmaDown", &VectorBox_Double["Elec_SigmaDown"]);
    ssbtree->Branch("Elec_ScSmUpUp", &VectorBox_Double["Elec_ScSmUpUp"]);
    ssbtree->Branch("Elec_ScSmUpDown", &VectorBox_Double["Elec_ScSmUpDown"]);
    ssbtree->Branch("Elec_ScSmDownUp", &VectorBox_Double["Elec_ScSmDownUp"]);
    ssbtree->Branch("Elec_ScSmDownDown", &VectorBox_Double["Elec_ScSmDownDown"]);
    ssbtree->Branch("Elec_Supercluster_Eta", &VectorBox_Double["Elec_Supercluster_Eta"]);
    ssbtree->Branch("Elec_Track_CtfdXY", &VectorBox_Double["Elec_Track_CtfdXY"]);
    ssbtree->Branch("Elec_Track_CtfdZ", &VectorBox_Double["Elec_Track_CtfdZ"]);
    ssbtree->Branch("Elec_Track_GsfdXY", &VectorBox_Double["Elec_Track_GsfdXY"]);
    ssbtree->Branch("Elec_Track_GsfdZ", &VectorBox_Double["Elec_Track_GsfdZ"]);
    ssbtree->Branch("Elec_pdgId", &VectorBox_Int["Elec_pdgId"]);
    ssbtree->Branch("Elec_relIso03", &VectorBox_Double["Elec_relIso03"]);
    ssbtree->Branch("Elec_relIso04", &VectorBox_Double["Elec_relIso04"]);
    ssbtree->Branch("Elec_Rnd", &VectorBox_Double["Elec_Rnd"]);





    ssbtree->Branch("Filter_Greedy_Muon", &VariableBox_Bool["Filter_Greedy_Muon"], "Filter_Greedy_Muon/B");
    ssbtree->Branch("Filter_Inconsistent_MuonPt", &VariableBox_Bool["Filter_Inconsistent_MuonPt"], "Filter_Inconsistent_MuonPt/B");
    ssbtree->Branch("Filter_PFReco_Muon", &VariableBox_Bool["Filter_PFReco_Muon"], "Filter_PFReco_Muon/B");
    ssbtree->Branch("Filter_PV", &VectorBox_Bool["Filter_PV"]);

    ssbtree->Branch("GenJet", "TClonesArray", &VariableBox_LorentzVector["GenJet"], 32000, 0);
    VariableBox_LorentzVector["GenJet"]->BypassStreamer();
    ssbtree->Branch("GenJet_Count", &VariableBox_Int["GenJet_Count"], "GenJet_Count/I");
    ssbtree->Branch("GenJet_ECalEnergy", &VectorBox_Float["GenJet_ECalEnergy"]);
    ssbtree->Branch("GenJet_HCalEnergy", &VectorBox_Float["GenJet_HCalEnergy"]);

    ssbtree->Branch("GenMET", "TClonesArray", &VariableBox_LorentzVector["GenMET"], 32000, 0);
    VariableBox_LorentzVector["GenMET"]->BypassStreamer();
    ssbtree->Branch("GenMET_Count", &VariableBox_Int["GenMET_Count"], "GenMET_Count/I");

    ssbtree->Branch("GenPar", "TClonesArray", &VariableBox_LorentzVector["GenPar"], 32000, 0);
    VariableBox_LorentzVector["GenPar"]->BypassStreamer();
    ssbtree->Branch("GenPar_Count", &VariableBox_Int["GenPar_Count"], "GenPar_Count/I");
    ssbtree->Branch("GenPar_Dau1_Idx", &VectorBox_Int["GenPar_Dau1_Idx"]);
    ssbtree->Branch("GenPar_Dau2_Idx", &VectorBox_Int["GenPar_Dau2_Idx"]);
    ssbtree->Branch("GenPar_Dau_Counter", &VectorBox_Int["GenPar_Dau_Counter"]);
    ssbtree->Branch("GenPar_Idx", &VectorBox_Int["GenPar_Idx"]);
    ssbtree->Branch("GenPar_Mom1_Idx", &VectorBox_Int["GenPar_Mom1_Idx"]);
    ssbtree->Branch("GenPar_Mom2_Idx", &VectorBox_Int["GenPar_Mom2_Idx"]);
    ssbtree->Branch("GenPar_Mom_Counter", &VectorBox_Int["GenPar_Mom_Counter"]);
    ssbtree->Branch("GenPar_Status", &VectorBox_Int["GenPar_Status"]);
    ssbtree->Branch("GenPar_pdgId", &VectorBox_Int["GenPar_pdgId"]);
    ssbtree->Branch("Gen_EventWeight", &VariableBox_Double["Gen_EventWeight"], "Gen_EventWeight/D");
    ssbtree->Branch("GenTop", "TClonesArray", &VariableBox_LorentzVector["GenTop"], 32000, 0);
    VariableBox_LorentzVector["GenTop"]->BypassStreamer();
    ssbtree->Branch("GenAnTop", "TClonesArray", &VariableBox_LorentzVector["GenAnTop"], 32000, 0);
    VariableBox_LorentzVector["GenAnTop"]->BypassStreamer();
    ssbtree->Branch("GenBJet", "TClonesArray", &VariableBox_LorentzVector["GenBJet"], 32000, 0);
    VariableBox_LorentzVector["GenBJet"]->BypassStreamer();
    ssbtree->Branch("GenBJet_Count", &VariableBox_Int["GenBJet_Count"], "GenBJet_Count/I");
    ssbtree->Branch("GenBHad", "TClonesArray", &VariableBox_LorentzVector["GenBHad"], 32000, 0);
    VariableBox_LorentzVector["GenBHad"]->BypassStreamer();
    ssbtree->Branch("GenBHad_Count", &VariableBox_Int["GenBHad_Count"], "GenBHad_Count/I");
    ssbtree->Branch("GenBHad_pdgId", &VectorBox_Int["GenBHad_pdgId"]);
    ssbtree->Branch("GenBHad_Flavour", &VectorBox_Int["GenBHad_Flavour"]);
    ssbtree->Branch("GenBHad_FromTopWeakDecay", &VectorBox_Int["GenBHad_FromTopWeakDecay"]);


    ssbtree->Branch("Jet", "TClonesArray", &VariableBox_LorentzVector["Jet"], 32000, 0);
    VariableBox_LorentzVector["Jet"]->BypassStreamer();
    ssbtree->Branch("Jet_Charge", &VectorBox_Int["Jet_Charge"]);
    ssbtree->Branch("Jet_Count", &VariableBox_Int["Jet_Count"], "Jet_Count/I");
    ssbtree->Branch("Jet_EnShiftedDown", &VectorBox_Double["Jet_EnShiftedDown"]);
    ssbtree->Branch("Jet_EnShiftedUp", &VectorBox_Double["Jet_EnShiftedUp"]);
    ssbtree->Branch("Jet_PartonFlavour", &VectorBox_Int["Jet_PartonFlavour"]);
    ssbtree->Branch("Jet_HadronFlavour", &VectorBox_Int["Jet_HadronFlavour"]);
    ssbtree->Branch("Jet_PFId", &VectorBox_Int["Jet_PFId"]);
    ssbtree->Branch("Jet_PFIdVeto", &VectorBox_Int["Jet_PFIdVeto"]);
    ssbtree->Branch("Jet_PhiResolution_MC", &VectorBox_Float["Jet_PhiResolution_MC"]);
    ssbtree->Branch("Jet_PhiResolution_DATA", &VectorBox_Float["Jet_PhiResolution_DATA"]);
    ssbtree->Branch("Jet_EnergyResolution_MC", &VectorBox_Double["Jet_EnergyResolution_MC"]);
    ssbtree->Branch("Jet_EnergyResolution_DATA", &VectorBox_Double["Jet_EnergyResolution_DATA"]);
    ssbtree->Branch("Jet_EnergyResolution_SF", &VectorBox_Double["Jet_EnergyResolution_SF"]);
    ssbtree->Branch("Jet_EnergyResolution_SFDown", &VectorBox_Double["Jet_EnergyResolution_SFDown"]);
    ssbtree->Branch("Jet_EnergyResolution_SFUp", &VectorBox_Double["Jet_EnergyResolution_SFUp"]);
    ssbtree->Branch("Jet_PileUpId", &VectorBox_Int["Jet_PileUpId"]);
    ssbtree->Branch("Jet_PileUpMVA", &VectorBox_Float["Jet_PileUpMVA"]);
    ssbtree->Branch("Jet_bDisc", &VectorBox_Float["Jet_bDisc"]);
    ssbtree->Branch("Jet_bDisc_Name", &VectorBox_String["Jet_bDisc_Name"]);
    ssbtree->Branch("Jet_bDisc_Value", &VectorBox_Float["Jet_bDisc_Value"]);
    ssbtree->Branch("Jet_isJet", &VectorBox_Bool["Jet_isJet"]);
    ssbtree->Branch("LHE_Central", &VariableBox_Double["LHE_Central"], "LHE_Central/D");
    ssbtree->Branch("LHE_Id", &VectorBox_Int["LHE_Id"]);
    ssbtree->Branch("LHE_Weight", &VectorBox_Double["LHE_Weight"]);
    /// BFrag
    ssbtree->Branch("Frag_Cen_Weight", &VariableBox_Double["Frag_Cen_Weight"], "Frag_Cen_Weight/D");
    ssbtree->Branch("Frag_Up_Weight", &VariableBox_Double["Frag_Up_Weight"], "Frag_Up_Weight/D");
    ssbtree->Branch("Frag_Down_Weight", &VariableBox_Double["Frag_Down_Weight"], "Frag_Down_Weight/D");
    ssbtree->Branch("Frag_Peterson_Weight", &VariableBox_Double["Frag_Peterson_Weight"], "Frag_Peterson_Weight/D");
    ssbtree->Branch("Semilep_BrUp_Weight", &VariableBox_Double["Semilep_BrUp_Weight"], "Semilep_BrUp_Weight/D");
    ssbtree->Branch("Semilep_BrDown_Weight", &VariableBox_Double["Semilep_BrDown_Weight"], "Semilep_BrDown_Weight/D");
    /// MET 
    ssbtree->Branch("MET", "TClonesArray", &VariableBox_LorentzVector["MET"], 32000, 0);
    VariableBox_LorentzVector["MET"]->BypassStreamer();
    ssbtree->Branch("MET_Significance", &VariableBox_Double["MET_Significance"], "MET_Significance/D");
    ssbtree->Branch("MET_JetEnShiftedUp_PT", &VectorBox_Double["MET_JetEnShiftedUp_PT"]);
    ssbtree->Branch("MET_JetEnShiftedUp_Phi", &VectorBox_Double["MET_JetEnShiftedUp_Phi"]);
    ssbtree->Branch("MET_JetEnShiftedDown_PT", &VectorBox_Double["MET_JetEnShiftedDown_PT"]);
    ssbtree->Branch("MET_JetEnShiftedDown_Phi", &VectorBox_Double["MET_JetEnShiftedDown_Phi"]);
    ssbtree->Branch("MET_MuonEnShiftedUp_PT", &VectorBox_Double["MET_MuonEnShiftedUp_PT"]);
    ssbtree->Branch("MET_MuonEnShiftedUp_Phi", &VectorBox_Double["MET_MuonEnShiftedUp_Phi"]);
    ssbtree->Branch("MET_MuonEnShiftedDown_PT", &VectorBox_Double["MET_MuonEnShiftedDown_PT"]);
    ssbtree->Branch("MET_MuonEnShiftedDown_Phi", &VectorBox_Double["MET_MuonEnShiftedDown_Phi"]);
    ssbtree->Branch("MET_ElectronEnShiftedUp_PT", &VectorBox_Double["MET_ElectronEnShiftedUp_PT"]);
    ssbtree->Branch("MET_ElectronEnShiftedUp_Phi", &VectorBox_Double["MET_ElectronEnShiftedUp_Phi"]);
    ssbtree->Branch("MET_ElectronEnShiftedDown_PT", &VectorBox_Double["MET_ElectronEnShiftedDown_PT"]);
    ssbtree->Branch("MET_ElectronEnShiftedDown_Phi", &VectorBox_Double["MET_ElectronEnShiftedDown_Phi"]);
    ssbtree->Branch("MET_UnclusteredEnShiftedUp_PT", &VectorBox_Double["MET_UnclusteredEnShiftedUp_PT"]);
    ssbtree->Branch("MET_UnclusteredEnShiftedUp_Phi", &VectorBox_Double["MET_UnclusteredEnShiftedUp_Phi"]);
    ssbtree->Branch("MET_UnclusteredEnShiftedDown_PT", &VectorBox_Double["MET_UnclusteredEnShiftedDown_PT"]);
    ssbtree->Branch("MET_UnclusteredEnShiftedDown_Phi", &VectorBox_Double["MET_UnclusteredEnShiftedDown_Phi"]);
    ssbtree->Branch("MET_JetResShiftedUp_PT", &VectorBox_Double["MET_JetResShiftedUp_PT"]);
    ssbtree->Branch("MET_JetResShiftedUp_Phi", &VectorBox_Double["MET_JetResShiftedUp_Phi"]);
    ssbtree->Branch("MET_JetResShiftedDown_PT", &VectorBox_Double["MET_JetResShiftedDown_PT"]);
    ssbtree->Branch("MET_JetResShiftedDown_Phi", &VectorBox_Double["MET_JetResShiftedDown_Phi"]);

    ssbtree->Branch("METEGClean", "TClonesArray", &VariableBox_LorentzVector["METEGClean"], 32000, 0);
    VariableBox_LorentzVector["METEGClean"]->BypassStreamer();
    ssbtree->Branch("METEGClean_Significance", &VariableBox_Double["METEGClean_Significance"], "METEGClean_Significance/D");
    ssbtree->Branch("METEGClean_JetEnShiftedUp_PT", &VectorBox_Double["METEGClean_JetEnShiftedUp_PT"]);
    ssbtree->Branch("METEGClean_JetEnShiftedUp_Phi", &VectorBox_Double["METEGClean_JetEnShiftedUp_Phi"]);
    ssbtree->Branch("METEGClean_JetEnShiftedDown_PT", &VectorBox_Double["METEGClean_JetEnShiftedDown_PT"]);
    ssbtree->Branch("METEGClean_JetEnShiftedDown_Phi", &VectorBox_Double["METEGClean_JetEnShiftedDown_Phi"]);
    ssbtree->Branch("METEGClean_MuonEnShiftedUp_PT", &VectorBox_Double["METEGClean_MuonEnShiftedUp_PT"]);
    ssbtree->Branch("METEGClean_MuonEnShiftedUp_Phi", &VectorBox_Double["METEGClean_MuonEnShiftedUp_Phi"]);
    ssbtree->Branch("METEGClean_MuonEnShiftedDown_PT", &VectorBox_Double["METEGClean_MuonEnShiftedDown_PT"]);
    ssbtree->Branch("METEGClean_MuonEnShiftedDown_Phi", &VectorBox_Double["METEGClean_MuonEnShiftedDown_Phi"]);
    ssbtree->Branch("METEGClean_ElectronEnShiftedUp_PT", &VectorBox_Double["METEGClean_ElectronEnShiftedUp_PT"]);
    ssbtree->Branch("METEGClean_ElectronEnShiftedUp_Phi", &VectorBox_Double["METEGClean_ElectronEnShiftedUp_Phi"]);
    ssbtree->Branch("METEGClean_ElectronEnShiftedDown_PT", &VectorBox_Double["METEGClean_ElectronEnShiftedDown_PT"]);
    ssbtree->Branch("METEGClean_ElectronEnShiftedDown_Phi", &VectorBox_Double["METEGClean_ElectronEnShiftedDown_Phi"]);
    ssbtree->Branch("METEGClean_UnclusteredEnShiftedUp_PT", &VectorBox_Double["METEGClean_UnclusteredEnShiftedUp_PT"]);
    ssbtree->Branch("METEGClean_UnclusteredEnShiftedUp_Phi", &VectorBox_Double["METEGClean_UnclusteredEnShiftedUp_Phi"]);
    ssbtree->Branch("METEGClean_UnclusteredEnShiftedDown_PT", &VectorBox_Double["METEGClean_UnclusteredEnShiftedDown_PT"]);
    ssbtree->Branch("METEGClean_UnclusteredEnShiftedDown_Phi", &VectorBox_Double["METEGClean_UnclusteredEnShiftedDown_Phi"]);
    ssbtree->Branch("METEGClean_JetResShiftedUp_PT", &VectorBox_Double["METEGClean_JetResShiftedUp_PT"]);
    ssbtree->Branch("METEGClean_JetResShiftedUp_Phi", &VectorBox_Double["METEGClean_JetResShiftedUp_Phi"]);
    ssbtree->Branch("METEGClean_JetResShiftedDown_PT", &VectorBox_Double["METEGClean_JetResShiftedDown_PT"]);
    ssbtree->Branch("METEGClean_JetResShiftedDown_Phi", &VectorBox_Double["METEGClean_JetResShiftedDown_Phi"]);

    ssbtree->Branch("METMUEGClean", "TClonesArray", &VariableBox_LorentzVector["METMUEGClean"], 32000, 0);
    VariableBox_LorentzVector["METMUEGClean"]->BypassStreamer();
    ssbtree->Branch("METMUEGClean_Significance", &VariableBox_Double["METMUEGClean_Significance"], "METMUEGClean_Significance/D");
    ssbtree->Branch("METMUEGClean_JetEnShiftedUp_PT", &VectorBox_Double["METMUEGClean_JetEnShiftedUp_PT"]);
    ssbtree->Branch("METMUEGClean_JetEnShiftedUp_Phi", &VectorBox_Double["METMUEGClean_JetEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGClean_JetEnShiftedDown_PT", &VectorBox_Double["METMUEGClean_JetEnShiftedDown_PT"]);
    ssbtree->Branch("METMUEGClean_JetEnShiftedDown_Phi", &VectorBox_Double["METMUEGClean_JetEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUEGClean_MuonEnShiftedUp_PT", &VectorBox_Double["METMUEGClean_MuonEnShiftedUp_PT"]);
    ssbtree->Branch("METMUEGClean_MuonEnShiftedUp_Phi", &VectorBox_Double["METMUEGClean_MuonEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGClean_MuonEnShiftedDown_PT", &VectorBox_Double["METMUEGClean_MuonEnShiftedDown_PT"]);
    ssbtree->Branch("METMUEGClean_MuonEnShiftedDown_Phi", &VectorBox_Double["METMUEGClean_MuonEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUEGClean_ElectronEnShiftedUp_PT", &VectorBox_Double["METMUEGClean_ElectronEnShiftedUp_PT"]);
    ssbtree->Branch("METMUEGClean_ElectronEnShiftedUp_Phi", &VectorBox_Double["METMUEGClean_ElectronEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGClean_ElectronEnShiftedDown_PT", &VectorBox_Double["METMUEGClean_ElectronEnShiftedDown_PT"]);
    ssbtree->Branch("METMUEGClean_ElectronEnShiftedDown_Phi", &VectorBox_Double["METMUEGClean_ElectronEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUEGClean_UnclusteredEnShiftedUp_PT", &VectorBox_Double["METMUEGClean_UnclusteredEnShiftedUp_PT"]);
    ssbtree->Branch("METMUEGClean_UnclusteredEnShiftedUp_Phi", &VectorBox_Double["METMUEGClean_UnclusteredEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGClean_UnclusteredEnShiftedDown_PT", &VectorBox_Double["METMUEGClean_UnclusteredEnShiftedDown_PT"]);
    ssbtree->Branch("METMUEGClean_UnclusteredEnShiftedDown_Phi", &VectorBox_Double["METMUEGClean_UnclusteredEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUEGClean_JetResShiftedUp_PT", &VectorBox_Double["METMUEGClean_JetResShiftedUp_PT"]);
    ssbtree->Branch("METMUEGClean_JetResShiftedUp_Phi", &VectorBox_Double["METMUEGClean_JetResShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGClean_JetResShiftedDown_PT", &VectorBox_Double["METMUEGClean_JetResShiftedDown_PT"]);
    ssbtree->Branch("METMUEGClean_JetResShiftedDown_Phi", &VectorBox_Double["METMUEGClean_JetResShiftedDown_Phi"]);

    ssbtree->Branch("METMUEGCleanCor", "TClonesArray", &VariableBox_LorentzVector["METMUEGCleanCor"], 32000, 0);
    VariableBox_LorentzVector["METMUEGCleanCor"]->BypassStreamer();
    ssbtree->Branch("METMUEGCleanCor_Significance", &VariableBox_Double["METMUEGCleanCor_Significance"], "METMUEGCleanCor_Significance/D");
    ssbtree->Branch("METMUEGCleanCor_JetEnShiftedUp_PT", &VectorBox_Double["METMUEGCleanCor_JetEnShiftedUp_PT"]);
    ssbtree->Branch("METMUEGCleanCor_JetEnShiftedUp_Phi", &VectorBox_Double["METMUEGCleanCor_JetEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGCleanCor_JetEnShiftedDown_PT", &VectorBox_Double["METMUEGCleanCor_JetEnShiftedDown_PT"]);
    ssbtree->Branch("METMUEGCleanCor_JetEnShiftedDown_Phi", &VectorBox_Double["METMUEGCleanCor_JetEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUEGCleanCor_MuonEnShiftedUp_PT", &VectorBox_Double["METMUEGCleanCor_MuonEnShiftedUp_PT"]);
    ssbtree->Branch("METMUEGCleanCor_MuonEnShiftedUp_Phi", &VectorBox_Double["METMUEGCleanCor_MuonEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGCleanCor_MuonEnShiftedDown_PT", &VectorBox_Double["METMUEGCleanCor_MuonEnShiftedDown_PT"]);
    ssbtree->Branch("METMUEGCleanCor_MuonEnShiftedDown_Phi", &VectorBox_Double["METMUEGCleanCor_MuonEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUEGCleanCor_ElectronEnShiftedUp_PT", &VectorBox_Double["METMUEGCleanCor_ElectronEnShiftedUp_PT"]);
    ssbtree->Branch("METMUEGCleanCor_ElectronEnShiftedUp_Phi", &VectorBox_Double["METMUEGCleanCor_ElectronEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGCleanCor_ElectronEnShiftedDown_PT", &VectorBox_Double["METMUEGCleanCor_ElectronEnShiftedDown_PT"]);
    ssbtree->Branch("METMUEGCleanCor_ElectronEnShiftedDown_Phi", &VectorBox_Double["METMUEGCleanCor_ElectronEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUEGCleanCor_UnclusteredEnShiftedUp_PT", &VectorBox_Double["METMUEGCleanCor_UnclusteredEnShiftedUp_PT"]);
    ssbtree->Branch("METMUEGCleanCor_UnclusteredEnShiftedUp_Phi", &VectorBox_Double["METMUEGCleanCor_UnclusteredEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGCleanCor_UnclusteredEnShiftedDown_PT", &VectorBox_Double["METMUEGCleanCor_UnclusteredEnShiftedDown_PT"]);
    ssbtree->Branch("METMUEGCleanCor_UnclusteredEnShiftedDown_Phi", &VectorBox_Double["METMUEGCleanCor_UnclusteredEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUEGCleanCor_JetResShiftedUp_PT", &VectorBox_Double["METMUEGCleanCor_JetResShiftedUp_PT"]);
    ssbtree->Branch("METMUEGCleanCor_JetResShiftedUp_Phi", &VectorBox_Double["METMUEGCleanCor_JetResShiftedUp_Phi"]);
    ssbtree->Branch("METMUEGCleanCor_JetResShiftedDown_PT", &VectorBox_Double["METMUEGCleanCor_JetResShiftedDown_PT"]);
    ssbtree->Branch("METMUEGCleanCor_JetResShiftedDown_Phi", &VectorBox_Double["METMUEGCleanCor_JetResShiftedDown_Phi"]);

    ssbtree->Branch("METMUCleanCor", "TClonesArray", &VariableBox_LorentzVector["METMUCleanCor"], 32000, 0);
    VariableBox_LorentzVector["METMUCleanCor"]->BypassStreamer();
    ssbtree->Branch("METMUCleanCor_Significance", &VariableBox_Double["METMUCleanCor_Significance"], "METMUCleanCor_Significance/D");
    ssbtree->Branch("METMUCleanCor_JetEnShiftedUp_PT", &VectorBox_Double["METMUCleanCor_JetEnShiftedUp_PT"]);
    ssbtree->Branch("METMUCleanCor_JetEnShiftedUp_Phi", &VectorBox_Double["METMUCleanCor_JetEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUCleanCor_JetEnShiftedDown_PT", &VectorBox_Double["METMUCleanCor_JetEnShiftedDown_PT"]);
    ssbtree->Branch("METMUCleanCor_JetEnShiftedDown_Phi", &VectorBox_Double["METMUCleanCor_JetEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUCleanCor_MuonEnShiftedUp_PT", &VectorBox_Double["METMUCleanCor_MuonEnShiftedUp_PT"]);
    ssbtree->Branch("METMUCleanCor_MuonEnShiftedUp_Phi", &VectorBox_Double["METMUCleanCor_MuonEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUCleanCor_MuonEnShiftedDown_PT", &VectorBox_Double["METMUCleanCor_MuonEnShiftedDown_PT"]);
    ssbtree->Branch("METMUCleanCor_MuonEnShiftedDown_Phi", &VectorBox_Double["METMUCleanCor_MuonEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUCleanCor_ElectronEnShiftedUp_PT", &VectorBox_Double["METMUCleanCor_ElectronEnShiftedUp_PT"]);
    ssbtree->Branch("METMUCleanCor_ElectronEnShiftedUp_Phi", &VectorBox_Double["METMUCleanCor_ElectronEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUCleanCor_ElectronEnShiftedDown_PT", &VectorBox_Double["METMUCleanCor_ElectronEnShiftedDown_PT"]);
    ssbtree->Branch("METMUCleanCor_ElectronEnShiftedDown_Phi", &VectorBox_Double["METMUCleanCor_ElectronEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUCleanCor_UnclusteredEnShiftedUp_PT", &VectorBox_Double["METMUCleanCor_UnclusteredEnShiftedUp_PT"]);
    ssbtree->Branch("METMUCleanCor_UnclusteredEnShiftedUp_Phi", &VectorBox_Double["METMUCleanCor_UnclusteredEnShiftedUp_Phi"]);
    ssbtree->Branch("METMUCleanCor_UnclusteredEnShiftedDown_PT", &VectorBox_Double["METMUCleanCor_UnclusteredEnShiftedDown_PT"]);
    ssbtree->Branch("METMUCleanCor_UnclusteredEnShiftedDown_Phi", &VectorBox_Double["METMUCleanCor_UnclusteredEnShiftedDown_Phi"]);
    ssbtree->Branch("METMUCleanCor_JetResShiftedUp_PT", &VectorBox_Double["METMUCleanCor_JetResShiftedUp_PT"]);
    ssbtree->Branch("METMUCleanCor_JetResShiftedUp_Phi", &VectorBox_Double["METMUCleanCor_JetResShiftedUp_Phi"]);
    ssbtree->Branch("METMUCleanCor_JetResShiftedDown_PT", &VectorBox_Double["METMUCleanCor_JetResShiftedDown_PT"]);
    ssbtree->Branch("METMUCleanCor_JetResShiftedDown_Phi", &VectorBox_Double["METMUCleanCor_JetResShiftedDown_Phi"]);

    ssbtree->Branch("METUnCor", "TClonesArray", &VariableBox_LorentzVector["METUnCor"], 32000, 0);
    VariableBox_LorentzVector["METUnCor"]->BypassStreamer();
    ssbtree->Branch("METUnCor_Significance", &VariableBox_Double["METUnCor_Significance"], "METUnCor_Significance/D");
    ssbtree->Branch("METUnCor_JetEnShiftedUp_PT", &VectorBox_Double["METUnCor_JetEnShiftedUp_PT"]);
    ssbtree->Branch("METUnCor_JetEnShiftedUp_Phi", &VectorBox_Double["METUnCor_JetEnShiftedUp_Phi"]);
    ssbtree->Branch("METUnCor_JetEnShiftedDown_PT", &VectorBox_Double["METUnCor_JetEnShiftedDown_PT"]);
    ssbtree->Branch("METUnCor_JetEnShiftedDown_Phi", &VectorBox_Double["METUnCor_JetEnShiftedDown_Phi"]);
    ssbtree->Branch("METUnCor_MuonEnShiftedUp_PT", &VectorBox_Double["METUnCor_MuonEnShiftedUp_PT"]);
    ssbtree->Branch("METUnCor_MuonEnShiftedUp_Phi", &VectorBox_Double["METUnCor_MuonEnShiftedUp_Phi"]);
    ssbtree->Branch("METUnCor_MuonEnShiftedDown_PT", &VectorBox_Double["METUnCor_MuonEnShiftedDown_PT"]);
    ssbtree->Branch("METUnCor_MuonEnShiftedDown_Phi", &VectorBox_Double["METUnCor_MuonEnShiftedDown_Phi"]);
    ssbtree->Branch("METUnCor_ElectronEnShiftedUp_PT", &VectorBox_Double["METUnCor_ElectronEnShiftedUp_PT"]);
    ssbtree->Branch("METUnCor_ElectronEnShiftedUp_Phi", &VectorBox_Double["METUnCor_ElectronEnShiftedUp_Phi"]);
    ssbtree->Branch("METUnCor_ElectronEnShiftedDown_PT", &VectorBox_Double["METUnCor_ElectronEnShiftedDown_PT"]);
    ssbtree->Branch("METUnCor_ElectronEnShiftedDown_Phi", &VectorBox_Double["METUnCor_ElectronEnShiftedDown_Phi"]);
    ssbtree->Branch("METUnCor_UnclusteredEnShiftedUp_PT", &VectorBox_Double["METUnCor_UnclusteredEnShiftedUp_PT"]);
    ssbtree->Branch("METUnCor_UnclusteredEnShiftedUp_Phi", &VectorBox_Double["METUnCor_UnclusteredEnShiftedUp_Phi"]);
    ssbtree->Branch("METUnCor_UnclusteredEnShiftedDown_PT", &VectorBox_Double["METUnCor_UnclusteredEnShiftedDown_PT"]);
    ssbtree->Branch("METUnCor_UnclusteredEnShiftedDown_Phi", &VectorBox_Double["METUnCor_UnclusteredEnShiftedDown_Phi"]);
    ssbtree->Branch("METUnCor_JetResShiftedUp_PT", &VectorBox_Double["METUnCor_JetResShiftedUp_PT"]);
    ssbtree->Branch("METUnCor_JetResShiftedUp_Phi", &VectorBox_Double["METUnCor_JetResShiftedUp_Phi"]);
    ssbtree->Branch("METUnCor_JetResShiftedDown_PT", &VectorBox_Double["METUnCor_JetResShiftedDown_PT"]);
    ssbtree->Branch("METUnCor_JetResShiftedDown_Phi", &VectorBox_Double["METUnCor_JetResShiftedDown_Phi"]);

    ssbtree->Branch("Muon", "TClonesArray", &VariableBox_LorentzVector["Muon"], 32000, 0);
    VariableBox_LorentzVector["Muon"]->BypassStreamer();
    ssbtree->Branch("GenMuon", "TClonesArray", &VariableBox_LorentzVector["GenMuon"], 32000, 0);
    VariableBox_LorentzVector["GenMuon"]->BypassStreamer();
    ssbtree->Branch("Muon_Charge", &VectorBox_Int["Muon_Charge"]);
    ssbtree->Branch("Muon_Count", &VariableBox_Int["Muon_Count"], "Muon_Count/I");
    ssbtree->Branch("Muon_PFIsodBeta03", &VectorBox_Double["Muon_PFIsodBeta03"]);
    ssbtree->Branch("Muon_PFIsodBeta04", &VectorBox_Double["Muon_PFIsodBeta04"]);
    ssbtree->Branch("Muon_isHighPt", &VectorBox_Bool["Muon_isHighPt"]);
    ssbtree->Branch("Muon_isLoose", &VectorBox_Bool["Muon_isLoose"]);
    ssbtree->Branch("Muon_isMedium", &VectorBox_Bool["Muon_isMedium"]);
    ssbtree->Branch("Muon_isMedium2016", &VectorBox_Bool["Muon_isMedium2016"]);
    ssbtree->Branch("Muon_isSoft", &VectorBox_Bool["Muon_isSoft"]);
    ssbtree->Branch("Muon_isTight", &VectorBox_Bool["Muon_isTight"]);
    ssbtree->Branch("Muon_pdgId", &VectorBox_Int["Muon_pdgId"]);
    ssbtree->Branch("Muon_rand1", &VectorBox_Double["Muon_rand1"]);
    ssbtree->Branch("Muon_rand2", &VectorBox_Double["Muon_rand2"]);
    ssbtree->Branch("Muon_relIso03", &VectorBox_Double["Muon_relIso03"]);
    ssbtree->Branch("Muon_relIso04", &VectorBox_Double["Muon_relIso04"]);
    ssbtree->Branch("Muon_trackerLayers", &VectorBox_Int["Muon_trackerLayers"]);
    ssbtree->Branch("PDFWeight_BjorkenX1", &VectorBox_Double["PDFWeight_BjorkenX1"]);
    ssbtree->Branch("PDFWeight_BjorkenX2", &VectorBox_Double["PDFWeight_BjorkenX2"]);
    ssbtree->Branch("PDFWeight_Cent", &VectorBox_Double["PDFWeight_Cent"]);
    ssbtree->Branch("PDFWeight_Cent_Down", &VectorBox_Double["PDFWeight_Cent_Down"]);
    ssbtree->Branch("PDFWeight_Cent_Up", &VectorBox_Double["PDFWeight_Cent_Up"]);
    ssbtree->Branch("PDFWeight_Id1", &VectorBox_Int["PDFWeight_Id1"]);
    ssbtree->Branch("PDFWeight_Id2", &VectorBox_Int["PDFWeight_Id2"]);
    ssbtree->Branch("PDFWeight_PDF1", &VectorBox_Double["PDFWeight_PDF1"]);
    ssbtree->Branch("PDFWeight_PDF2", &VectorBox_Double["PDFWeight_PDF2"]);
    ssbtree->Branch("PDFWeight_Q", &VectorBox_Double["PDFWeight_Q"]);
    ssbtree->Branch("PDFWeight_Var1_Down", &VectorBox_Double["PDFWeight_Var1_Down"]);
    ssbtree->Branch("PDFWeight_Var1_Up", &VectorBox_Double["PDFWeight_Var1_Up"]);
    ssbtree->Branch("PDFWeight_Var2_Down", &VectorBox_Double["PDFWeight_Var2_Down"]);
    ssbtree->Branch("PDFWeight_Var2_Up", &VectorBox_Double["PDFWeight_Var2_Up"]);
    ssbtree->Branch("Photon", "TClonesArray", &VariableBox_LorentzVector["Photon"], 32000, 0);
    VariableBox_LorentzVector["Photon"]->BypassStreamer();
    ssbtree->Branch("Photon_Count", &VariableBox_Int["Photon_Count"], "Photon_Count/I");
    ssbtree->Branch("Photon_SCB_Loose", &VectorBox_Bool["Photon_SCB_Loose"]);
    ssbtree->Branch("Photon_SCB_Medium", &VectorBox_Bool["Photon_SCB_Medium"]);
    ssbtree->Branch("Photon_SCB_Tight", &VectorBox_Bool["Photon_SCB_Tight"]);
    ssbtree->Branch("Photon_ChaHadIso", &VectorBox_Float["Photon_ChaHadIso"]);
    ssbtree->Branch("Photon_NeuHadIso", &VectorBox_Float["Photon_NeuHadIso"]);
    ssbtree->Branch("Photon_PhoIso", &VectorBox_Float["Photon_PhoIso"]);
    ssbtree->Branch("Photon_WorstChargedIso", &VectorBox_Float["Photon_WorstChargedIso"]);
    ssbtree->Branch("Photon_ChaHadEffArea", &VectorBox_Float["Photon_ChaHadEffArea"]);
    ssbtree->Branch("Photon_NeuHadEffArea", &VectorBox_Float["Photon_NeuHadEffArea"]);
    ssbtree->Branch("Photon_PhoHadEffArea", &VectorBox_Float["Photon_PhoHadEffArea"]);
    ssbtree->Branch("Photon_R9", &VectorBox_Double["Photon_R9"]);
    ssbtree->Branch("Photon_HoverE", &VectorBox_Double["Photon_HoverE"]);
    ssbtree->Branch("Photon_SuperCluster_Eta", &VectorBox_Double["Photon_SuperCluster_Eta"]);
    ssbtree->Branch("Photon_SuperCluster_Phi", &VectorBox_Double["Photon_SuperCluster_Phi"]);
    ssbtree->Branch("Photon_SuperCluster_EtaWidth", &VectorBox_Double["Photon_SuperCluster_EtaWidth"]);
    ssbtree->Branch("Photon_SuperCluster_PhiWidth", &VectorBox_Double["Photon_SuperCluster_PhiWidth"]);
    ssbtree->Branch("Photon_SuperCluster_Energy", &VectorBox_Double["Photon_SuperCluster_Energy"]);
    ssbtree->Branch("Photon_SuperCluster_RawEnergy", &VectorBox_Double["Photon_SuperCluster_RawEnergy"]);
    ssbtree->Branch("Photon_ElectronVeto", &VectorBox_Bool["Photon_ElectronVeto"]);
    ssbtree->Branch("Photon_Full5x5_SigmaIetaIeta", &VectorBox_Double["Photon_Full5x5_SigmaIetaIeta"]);
    ssbtree->Branch("Photon_MVANonTrig_Tight", &VectorBox_Bool["Photon_MVANonTrig_Tight"]);
    ssbtree->Branch("PV_Count", &VariableBox_Int["PV_Count"], "PV_Count/I");
    ssbtree->Branch("PileUp_Count_Interaction", &VariableBox_Int["PileUp_Count_Interaction"], "PileUp_Count_Interaction/I");
    ssbtree->Branch("PileUp_Count_Intime", &VariableBox_Float["PileUp_Count_Intime"], "PileUp_Count_Intime/F");
    ssbtree->Branch("Rho", &VectorBox_Double["Rho"]);
    ssbtree->Branch("Trigger_Name", &VectorBox_String["Trigger_Name"]);
    ssbtree->Branch("Trigger_PreScale", &VectorBox_UInt["Trigger_PreScale"]);
    ssbtree->Branch("Trigger_isError", &VectorBox_Bool["Trigger_isError"]);
    ssbtree->Branch("Trigger_isPass", &VectorBox_Bool["Trigger_isPass"]);
    ssbtree->Branch("Trigger_isRun", &VectorBox_Bool["Trigger_isRun"]);
    ssbtree->Branch("Vertex_SumPtSquare", &VectorBox_Double["Vertex_SumPtSquare"]);
    ssbtree->Branch("Vertex_X", &VectorBox_Double["Vertex_X"]);
    ssbtree->Branch("Vertex_X_Error", &VectorBox_Double["Vertex_X_Error"]);
    ssbtree->Branch("Vertex_Y", &VectorBox_Double["Vertex_Y"]);
    ssbtree->Branch("Vertex_Y_Error", &VectorBox_Double["Vertex_Y_Error"]);
    ssbtree->Branch("Vertex_Z", &VectorBox_Double["Vertex_Z"]);
    ssbtree->Branch("Vertex_Z_Error", &VectorBox_Double["Vertex_Z_Error"]);

}

void SSBTreeManager::InitializeVariables(){

    VariableBox_Int["Info_EventNumber"] = 0;
    VariableBox_Int["Info_Luminosity"] = 0;
    VariableBox_Int["Info_RunNumber"] = 0;
    VariableBox_Bool["Info_isData"] = false;
    VariableBox_Int["Channel_Idx"] = 0;
    VariableBox_Int["Channel_Idx_Final"] = 0;
    VariableBox_Int["Channel_Lepton_Count"] = 0;
    VariableBox_Int["Channel_Lepton_Count_Final"] = 0;
    VariableBox_Int["Channel_Jets"] = 0;
    VariableBox_Int["Channel_Jets_Abs"] = 0;

    VectorBox_String["METFilter_Name"].clear();
    VectorBox_Bool["METFilter_isError"].clear();
    VectorBox_Bool["METFilter_isPass"].clear();
    VectorBox_Bool["METFilter_isRun"].clear();
    VectorBox_String["METFilterAdd_Name"].clear();
    VectorBox_Bool["METFilterAdd_isPass"].clear();

    VariableBox_LorentzVector["Elec"]->Clear();
    VectorBox_Int["Elec_Charge"].clear();
    VectorBox_Bool["Elec_ChargeId_GsfCtf"].clear();
    VectorBox_Bool["Elec_ChargeId_GsfCtfPx"].clear();
    VectorBox_Bool["Elec_ChargeId_GsfPx"].clear();
    VectorBox_Double["Elec_Charge_CtfTr"].clear();
    VectorBox_Double["Elec_Charge_GsfTr"].clear();
    VectorBox_Double["Elec_Charge_Px"].clear();
    VectorBox_Bool["Elec_Conversion"].clear();
    VariableBox_Int["Elec_Count"] = 0;
    VectorBox_Int["Elec_Id_Loose"].clear();
    VectorBox_Int["Elec_Id_RobustHighEnergy"].clear();
    VectorBox_Int["Elec_Id_RobustLoose"].clear();
    VectorBox_Int["Elec_Id_RobustTight"].clear();
    VectorBox_Int["Elec_Id_Tight"].clear();
    VectorBox_Int["Elec_Inner_Hit"].clear();
    VectorBox_Bool["Elec_MVA_Medium"].clear();
    VectorBox_Bool["Elec_MVA_Tight"].clear();
    VectorBox_Bool["Elec_MVA_HZZ"].clear();
    VectorBox_Float["Elec_MVA_Values"].clear();
    VectorBox_Int["Elec_MVA_Categories"].clear();
    VectorBox_Float["Elec_MVA_HZZ_Values"].clear();
    VectorBox_Int["Elec_MVA_HZZ_Categories"].clear();
    //VectorBox_Bool["Elec_MVATrig_Tight"].clear();
    //VectorBox_Float["Elec_MVA_NonTrigV0"].clear();
    //VectorBox_Float["Elec_MVA_TrigNoIPV0"].clear();
    //VectorBox_Float["Elec_MVA_TrigV0"].clear();
    VectorBox_Double["Elec_PFIsoRho03"].clear();
    VectorBox_Double["Elec_PFIsoRho04"].clear();
    VectorBox_Bool["Elec_PFIsoValid"].clear();
    VectorBox_Double["Elec_PFIsodBeta03"].clear();
    VectorBox_Double["Elec_PFIsodBeta04"].clear();
    VectorBox_Bool["Elec_SCB_Loose"].clear();
    VectorBox_Bool["Elec_SCB_Medium"].clear();
    VectorBox_Bool["Elec_SCB_Tight"].clear();
    VectorBox_Bool["Elec_SCB_Veto"].clear();
    VectorBox_Bool["Elec_SCB_HEEP"].clear();
    VectorBox_Float["Elec_SCB_dEtaIn"].clear();
    VectorBox_Float["Elec_SCB_dPhiIn"].clear();
    VectorBox_Float["Elec_SCB_hOverE"].clear();
    VectorBox_Float["Elec_SCB_ooEmooP"].clear();
    VectorBox_Float["Elec_SCB_sigmaIetaIeta"].clear();
    VectorBox_Double["Elec_ScaleUp"].clear();
    VectorBox_Double["Elec_ScaleDown"].clear();
    VectorBox_Double["Elec_SigmaUp"].clear();
    VectorBox_Double["Elec_SigmaDown"].clear();
    VectorBox_Double["Elec_ScSmUpUp"].clear();
    VectorBox_Double["Elec_ScSmUpDown"].clear();
    VectorBox_Double["Elec_ScSmDownUp"].clear();
    VectorBox_Double["Elec_ScSmDownDown"].clear();
    VectorBox_Double["Elec_Supercluster_Eta"].clear();
    VectorBox_Double["Elec_Track_CtfdXY"].clear();
    VectorBox_Double["Elec_Track_CtfdZ"].clear();
    VectorBox_Double["Elec_Track_GsfdXY"].clear();
    VectorBox_Double["Elec_Track_GsfdZ"].clear();
    VectorBox_Int["Elec_pdgId"].clear();
    VectorBox_Double["Elec_relIso03"].clear();
    VectorBox_Double["Elec_relIso04"].clear();
    VectorBox_Double["Elec_Rnd"].clear();

    VariableBox_Bool["Filter_Greedy_Muon"] = false;
    VariableBox_Bool["Filter_Inconsistent_MuonPt"] = false;
    VariableBox_Bool["Filter_PFReco_Muon"] = false;
    VectorBox_Bool["Filter_PV"].clear();
    VariableBox_LorentzVector["GenJet"]->Clear();
    VariableBox_Int["GenJet_Count"] = 0;
    VectorBox_Float["GenJet_ECalEnergy"].clear();
    VectorBox_Float["GenJet_HCalEnergy"].clear();
    VariableBox_LorentzVector["GenMET"]->Clear();
    VariableBox_Int["GenMET_Count"] = 0;
    VariableBox_LorentzVector["GenPar"]->Clear();
    VariableBox_Int["GenPar_Count"] = 0;
    VectorBox_Int["GenPar_Dau1_Idx"].clear();
    VectorBox_Int["GenPar_Dau2_Idx"].clear();
    VectorBox_Int["GenPar_Dau_Counter"].clear();
    VectorBox_Int["GenPar_Idx"].clear();
    VectorBox_Int["GenPar_Mom1_Idx"].clear();
    VectorBox_Int["GenPar_Mom2_Idx"].clear();
    VectorBox_Int["GenPar_Mom_Counter"].clear();
    VectorBox_Int["GenPar_Status"].clear();
    VectorBox_Int["GenPar_pdgId"].clear();
    VariableBox_Double["Gen_EventWeight"] = 0;

    VariableBox_LorentzVector["Jet"]->Clear();
    VectorBox_Int["Jet_Charge"].clear();
    VariableBox_Int["Jet_Count"] = 0;
    VectorBox_Double["Jet_EnShiftedUp"].clear();
    VectorBox_Double["Jet_EnShiftedDown"].clear();
    VectorBox_Int["Jet_PartonFlavour"].clear();
    VectorBox_Int["Jet_HadronFlavour"].clear();
    VectorBox_Int["Jet_PFId"].clear();
    VectorBox_Int["Jet_PFIdVeto"].clear();
    VectorBox_Float["Jet_PhiResolution_MC"].clear();
    VectorBox_Float["Jet_PhiResolution_DATA"].clear();
    VectorBox_Double["Jet_EnergyResolution_MC"].clear();
    VectorBox_Double["Jet_EnergyResolution_DATA"].clear();
    VectorBox_Double["Jet_EnergyResolution_SF"].clear();
    VectorBox_Double["Jet_EnergyResolution_SFDown"].clear();
    VectorBox_Double["Jet_EnergyResolution_SFUp"].clear();
    VectorBox_Int["Jet_PileUpId"].clear();
    VectorBox_Float["Jet_PileUpMVA"].clear();
    VectorBox_Float["Jet_bDisc"].clear();
    VectorBox_String["Jet_bDisc_Name"].clear();
    VectorBox_Float["Jet_bDisc_Value"].clear();

    VectorBox_Bool["Jet_isJet"].clear();
    VariableBox_Double["LHE_Central"] = 0;
    VectorBox_Int["LHE_Id"].clear();
    VectorBox_Double["LHE_Weight"].clear();

    VariableBox_Double["Frag_Cen_Weight"] = 0;
    VariableBox_Double["Frag_Up_Weight"] = 0;
    VariableBox_Double["Frag_Down_Weight"] = 0;
    VariableBox_Double["Frag_Peterson_Weight"] = 0;
    VariableBox_Double["Semilep_BrUp_Weight"] = 0;
    VariableBox_Double["Semilep_BrDown_Weight"] = 0;

    VariableBox_LorentzVector["MET"]->Clear();
    VariableBox_Double["MET_Significance"] = 0;
    VectorBox_Double["MET_JetEnShiftedUp_PT"].clear();
    VectorBox_Double["MET_JetEnShiftedUp_Phi"].clear();
    VectorBox_Double["MET_JetEnShiftedDown_PT"].clear();
    VectorBox_Double["MET_JetEnShiftedDown_Phi"].clear();

    VectorBox_Double["MET_MuonEnShiftedUp_PT"].clear();
    VectorBox_Double["MET_MuonEnShiftedUp_Phi"].clear();
    VectorBox_Double["MET_MuonEnShiftedDown_PT"].clear();
    VectorBox_Double["MET_MuonEnShiftedDown_Phi"].clear();

    VectorBox_Double["MET_ElectronEnShiftedDown_PT"].clear();
    VectorBox_Double["MET_ElectronEnShiftedDown_Phi"].clear();
    VectorBox_Double["MET_ElectronEnShiftedUp_PT"].clear();
    VectorBox_Double["MET_ElectronEnShiftedUp_Phi"].clear();

    VectorBox_Double["MET_UnclusteredEnShiftedDown_PT"].clear();
    VectorBox_Double["MET_UnclusteredEnShiftedDown_Phi"].clear();
    VectorBox_Double["MET_UnclusteredEnShiftedUp_PT"].clear();
    VectorBox_Double["MET_UnclusteredEnShiftedUp_Phi"].clear();

    VectorBox_Double["MET_JetResShiftedUp_PT"].clear();
    VectorBox_Double["MET_JetResShiftedUp_Phi"].clear();
    VectorBox_Double["MET_JetResShiftedDown_PT"].clear();
    VectorBox_Double["MET_JetResShiftedDown_Phi"].clear();

    VariableBox_LorentzVector["METEGClean"]->Clear();
    VariableBox_Double["METEGClean_Significance"] = 0;
    VectorBox_Double["METEGClean_JetEnShiftedUp_PT"].clear();
    VectorBox_Double["METEGClean_JetEnShiftedUp_Phi"].clear();
    VectorBox_Double["METEGClean_JetEnShiftedDown_PT"].clear();
    VectorBox_Double["METEGClean_JetEnShiftedDown_Phi"].clear();

    VectorBox_Double["METEGClean_MuonEnShiftedUp_PT"].clear();
    VectorBox_Double["METEGClean_MuonEnShiftedUp_Phi"].clear();
    VectorBox_Double["METEGClean_MuonEnShiftedDown_PT"].clear();
    VectorBox_Double["METEGClean_MuonEnShiftedDown_Phi"].clear();

    VectorBox_Double["METEGClean_ElectronEnShiftedDown_PT"].clear();
    VectorBox_Double["METEGClean_ElectronEnShiftedDown_Phi"].clear();
    VectorBox_Double["METEGClean_ElectronEnShiftedUp_PT"].clear();
    VectorBox_Double["METEGClean_ElectronEnShiftedUp_Phi"].clear();

    VectorBox_Double["METEGClean_UnclusteredEnShiftedDown_PT"].clear();
    VectorBox_Double["METEGClean_UnclusteredEnShiftedDown_Phi"].clear();
    VectorBox_Double["METEGClean_UnclusteredEnShiftedUp_PT"].clear();
    VectorBox_Double["METEGClean_UnclusteredEnShiftedUp_Phi"].clear();

    VectorBox_Double["METEGClean_JetResShiftedUp_PT"].clear();
    VectorBox_Double["METEGClean_JetResShiftedUp_Phi"].clear();
    VectorBox_Double["METEGClean_JetResShiftedDown_PT"].clear();
    VectorBox_Double["METEGClean_JetResShiftedDown_Phi"].clear();

    VariableBox_LorentzVector["METMUEGClean"]->Clear();

    VariableBox_Double["METMUEGClean_Significance"] = 0;
    VectorBox_Double["METMUEGClean_JetEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGClean_JetEnShiftedUp_Phi"].clear();
    VectorBox_Double["METMUEGClean_JetEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGClean_JetEnShiftedDown_Phi"].clear();

    VectorBox_Double["METMUEGClean_MuonEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGClean_MuonEnShiftedUp_Phi"].clear();
    VectorBox_Double["METMUEGClean_MuonEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGClean_MuonEnShiftedDown_Phi"].clear();

    VectorBox_Double["METMUEGClean_ElectronEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGClean_ElectronEnShiftedDown_Phi"].clear();
    VectorBox_Double["METMUEGClean_ElectronEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGClean_ElectronEnShiftedUp_Phi"].clear();

    VectorBox_Double["METMUEGClean_UnclusteredEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGClean_UnclusteredEnShiftedDown_Phi"].clear();
    VectorBox_Double["METMUEGClean_UnclusteredEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGClean_UnclusteredEnShiftedUp_Phi"].clear();

    VectorBox_Double["METMUEGClean_JetResShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGClean_JetResShiftedUp_Phi"].clear();
    VectorBox_Double["METMUEGClean_JetResShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGClean_JetResShiftedDown_Phi"].clear();

    VariableBox_LorentzVector["METMUEGCleanCor"]->Clear();
    VariableBox_Double["METMUEGCleanCor_Significance"] = 0;
    VectorBox_Double["METMUEGCleanCor_JetEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_JetEnShiftedUp_Phi"].clear();
    VectorBox_Double["METMUEGCleanCor_JetEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_JetEnShiftedDown_Phi"].clear();

    VectorBox_Double["METMUEGCleanCor_MuonEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_MuonEnShiftedUp_Phi"].clear();
    VectorBox_Double["METMUEGCleanCor_MuonEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_MuonEnShiftedDown_Phi"].clear();

    VectorBox_Double["METMUEGCleanCor_ElectronEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_ElectronEnShiftedDown_Phi"].clear();
    VectorBox_Double["METMUEGCleanCor_ElectronEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_ElectronEnShiftedUp_Phi"].clear();

    VectorBox_Double["METMUEGCleanCor_UnclusteredEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_UnclusteredEnShiftedDown_Phi"].clear();
    VectorBox_Double["METMUEGCleanCor_UnclusteredEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_UnclusteredEnShiftedUp_Phi"].clear();

    VectorBox_Double["METMUEGCleanCor_JetResShiftedUp_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_JetResShiftedUp_Phi"].clear();
    VectorBox_Double["METMUEGCleanCor_JetResShiftedDown_PT"].clear();
    VectorBox_Double["METMUEGCleanCor_JetResShiftedDown_Phi"].clear();

    VariableBox_LorentzVector["METUnCor"]->Clear();
    VariableBox_Double["METUnCor_Significance"] = 0;
    VectorBox_Double["METUnCor_JetEnShiftedUp_PT"].clear();
    VectorBox_Double["METUnCor_JetEnShiftedUp_Phi"].clear();
    VectorBox_Double["METUnCor_JetEnShiftedDown_PT"].clear();
    VectorBox_Double["METUnCor_JetEnShiftedDown_Phi"].clear();

    VectorBox_Double["METUnCor_MuonEnShiftedUp_PT"].clear();
    VectorBox_Double["METUnCor_MuonEnShiftedUp_Phi"].clear();
    VectorBox_Double["METUnCor_MuonEnShiftedDown_PT"].clear();
    VectorBox_Double["METUnCor_MuonEnShiftedDown_Phi"].clear();

    VectorBox_Double["METUnCor_ElectronEnShiftedDown_PT"].clear();
    VectorBox_Double["METUnCor_ElectronEnShiftedDown_Phi"].clear();
    VectorBox_Double["METUnCor_ElectronEnShiftedUp_PT"].clear();
    VectorBox_Double["METUnCor_ElectronEnShiftedUp_Phi"].clear();

    VectorBox_Double["METUnCor_UnclusteredEnShiftedDown_PT"].clear();
    VectorBox_Double["METUnCor_UnclusteredEnShiftedDown_Phi"].clear();
    VectorBox_Double["METUnCor_UnclusteredEnShiftedUp_PT"].clear();
    VectorBox_Double["METUnCor_UnclusteredEnShiftedUp_Phi"].clear();

    VectorBox_Double["METUnCor_JetResShiftedUp_PT"].clear();
    VectorBox_Double["METUnCor_JetResShiftedUp_Phi"].clear();
    VectorBox_Double["METUnCor_JetResShiftedDown_PT"].clear();
    VectorBox_Double["METUnCor_JetResShiftedDown_Phi"].clear();

    VariableBox_LorentzVector["METMUCleanCor"]->Clear();
    VariableBox_Double["METMUCleanCor_Significance"] = 0;
    VectorBox_Double["METMUCleanCor_JetEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUCleanCor_JetEnShiftedUp_Phi"].clear();
    VectorBox_Double["METMUCleanCor_JetEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUCleanCor_JetEnShiftedDown_Phi"].clear();

    VectorBox_Double["METMUCleanCor_MuonEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUCleanCor_MuonEnShiftedUp_Phi"].clear();
    VectorBox_Double["METMUCleanCor_MuonEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUCleanCor_MuonEnShiftedDown_Phi"].clear();

    VectorBox_Double["METMUCleanCor_ElectronEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUCleanCor_ElectronEnShiftedDown_Phi"].clear();
    VectorBox_Double["METMUCleanCor_ElectronEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUCleanCor_ElectronEnShiftedUp_Phi"].clear();

    VectorBox_Double["METMUCleanCor_UnclusteredEnShiftedDown_PT"].clear();
    VectorBox_Double["METMUCleanCor_UnclusteredEnShiftedDown_Phi"].clear();
    VectorBox_Double["METMUCleanCor_UnclusteredEnShiftedUp_PT"].clear();
    VectorBox_Double["METMUCleanCor_UnclusteredEnShiftedUp_Phi"].clear();

    VectorBox_Double["METMUCleanCor_JetResShiftedUp_PT"].clear();
    VectorBox_Double["METMUCleanCor_JetResShiftedUp_Phi"].clear();
    VectorBox_Double["METMUCleanCor_JetResShiftedDown_PT"].clear();
    VectorBox_Double["METMUCleanCor_JetResShiftedDown_Phi"].clear();


    VariableBox_LorentzVector["Muon"]->Clear();
    VariableBox_LorentzVector["GenMuon"]->Clear();
    VectorBox_Int["Muon_Charge"].clear();
    VariableBox_Int["Muon_Count"] = 0;
    VectorBox_Double["Muon_PFIsodBeta03"].clear();
    VectorBox_Double["Muon_PFIsodBeta04"].clear();
    VectorBox_Bool["Muon_isHighPt"].clear();
    VectorBox_Bool["Muon_isLoose"].clear();
    VectorBox_Bool["Muon_isMedium"].clear();
    VectorBox_Bool["Muon_isMedium2016"].clear();
    VectorBox_Bool["Muon_isSoft"].clear();
    VectorBox_Bool["Muon_isTight"].clear();
    VectorBox_Int["Muon_pdgId"].clear();
    VectorBox_Double["Muon_rand1"].clear();
    VectorBox_Double["Muon_rand2"].clear();
    VectorBox_Double["Muon_relIso03"].clear();
    VectorBox_Double["Muon_relIso04"].clear();
    VectorBox_Int["Muon_trackerLayers"].clear();
    VectorBox_Double["PDFWeight_BjorkenX1"].clear();
    VectorBox_Double["PDFWeight_BjorkenX2"].clear();
    VectorBox_Double["PDFWeight_Cent"].clear();
    VectorBox_Double["PDFWeight_Cent_Down"].clear();
    VectorBox_Double["PDFWeight_Var1_Down"].clear();
    VectorBox_Double["PDFWeight_Var2_Down"].clear();
    VectorBox_Int["PDFWeight_Id1"].clear();
    VectorBox_Int["PDFWeight_Id2"].clear();
    VectorBox_Double["PDFWeight_PDF1"].clear();
    VectorBox_Double["PDFWeight_PDF2"].clear();
    VectorBox_Double["PDFWeight_Q"].clear();
    VectorBox_Double["PDFWeight_Cent_Up"].clear();
    VectorBox_Double["PDFWeight_Var1_Up"].clear();
    VectorBox_Double["PDFWeight_Var2_Up"].clear();
    VariableBox_LorentzVector["Photon"]->Clear();
    VariableBox_Int["Photon_Count"] = 0;
    VectorBox_Bool["Photon_SCB_Loose"].clear();
    VectorBox_Bool["Photon_SCB_Medium"].clear();
    VectorBox_Bool["Photon_SCB_Tight"].clear();
    VectorBox_Bool["Photon_MVANonTrig_Tight"].clear();
    VectorBox_Double["Photon_R9"].clear();
    VectorBox_Double["Photon_HoverE"].clear();
    VectorBox_Double["Photon_SuperCluster_Eta"].clear();
    VectorBox_Double["Photon_SuperCluster_Phi"].clear();
    VectorBox_Double["Photon_SuperCluster_EtaWidth"].clear();
    VectorBox_Double["Photon_SuperCluster_PhiWidth"].clear();
    VectorBox_Double["Photon_SuperCluster_Energy"].clear();
    VectorBox_Double["Photon_SuperCluster_RawEnergy"].clear();
    VectorBox_Double["Photon_Full5x5_SigmaIetaIeta"].clear();
    VectorBox_Float["Photon_ChaHadIso"].clear();
    VectorBox_Float["Photon_NeuHadIso"].clear();
    VectorBox_Float["Photon_PhoIso"].clear();
    VectorBox_Float["Photon_WorstChargedIso"].clear();
    VectorBox_Float["Photon_ChaHadEffArea"].clear();
    VectorBox_Float["Photon_NeuHadEffArea"].clear();
    VectorBox_Float["Photon_PhoHadEffArea"].clear();
    VectorBox_Bool["Photon_ElectronVeto"].clear();
    VariableBox_Int["PV_Count"] = 0;
    VariableBox_Int["PileUp_Count_Interaction"] = 0;
    VariableBox_Float["PileUp_Count_Intime"] = 0;
    VectorBox_Double["Rho"].clear();
    VectorBox_String["Trigger_Name"].clear();
    VectorBox_UInt["Trigger_PreScale"].clear();
    VectorBox_Bool["Trigger_isError"].clear();
    VectorBox_Bool["Trigger_isPass"].clear();
    VectorBox_Bool["Trigger_isRun"].clear();
    VectorBox_Double["Vertex_SumPtSquare"].clear();
    VectorBox_Double["Vertex_X"].clear();
    VectorBox_Double["Vertex_X_Error"].clear();
    VectorBox_Double["Vertex_Y"].clear();
    VectorBox_Double["Vertex_Y_Error"].clear();
    VectorBox_Double["Vertex_Z"].clear();
    VectorBox_Double["Vertex_Z_Error"].clear();
}

void SSBTreeManager::GenBook(TTree* tree){

    ssbtree = tree;
    ssbtree->Branch("Info_EventNumber", &VariableBox_Int["Info_EventNumber"], "Info_EventNumber/I");
    ssbtree->Branch("Info_Luminosity", &VariableBox_Int["Info_Luminosity"], "Info_Luminosity/I");
    ssbtree->Branch("Info_RunNumber", &VariableBox_Int["Info_RunNumber"], "Info_RunNumber/I");
    ssbtree->Branch("Info_isData", &VariableBox_Bool["Info_isData"], "Info_isData/B");

    ssbtree->Branch("Channel_Idx", &VariableBox_Int["Channel_Idx"], "Channel_Idx/I");
    ssbtree->Branch("Channel_Idx_Final", &VariableBox_Int["Channel_Idx_Final"], "Channel_Idx_Final/I");
    ssbtree->Branch("Channel_Jets", &VariableBox_Int["Channel_Jets"], "Channel_Jets/I");
    ssbtree->Branch("Channel_Jets_Abs", &VariableBox_Int["Channel_Jets_Abs"], "Channel_Jets_Abs/I");

    ssbtree->Branch("Channel_Lepton_Count", &VariableBox_Int["Channel_Lepton_Count"], "Channel_Lepton_Count/I");
    ssbtree->Branch("Channel_Lepton_Count_Final", &VariableBox_Int["Channel_Lepton_Count_Final"], "Channel_Lepton_Count_Final/I");

    ssbtree->Branch("GenJet", "TClonesArray", &VariableBox_LorentzVector["GenJet"], 32000, 0);
    VariableBox_LorentzVector["GenJet"]->BypassStreamer();
    ssbtree->Branch("GenJet_Count", &VariableBox_Int["GenJet_Count"], "GenJet_Count/I");
    ssbtree->Branch("GenJet_ECalEnergy", &VectorBox_Float["GenJet_ECalEnergy"]);
    ssbtree->Branch("GenJet_HCalEnergy", &VectorBox_Float["GenJet_HCalEnergy"]);

    ssbtree->Branch("GenMET", "TClonesArray", &VariableBox_LorentzVector["GenMET"], 32000, 0);
    VariableBox_LorentzVector["GenMET"]->BypassStreamer();
    ssbtree->Branch("GenMET_Count", &VariableBox_Int["GenMET_Count"], "GenMET_Count/I");
    ssbtree->Branch("GenPar", "TClonesArray", &VariableBox_LorentzVector["GenPar"], 32000, 0);
    VariableBox_LorentzVector["GenPar"]->BypassStreamer();
    ssbtree->Branch("GenPar_Count", &VariableBox_Int["GenPar_Count"], "GenPar_Count/I");
    ssbtree->Branch("GenPar_Dau1_Idx", &VectorBox_Int["GenPar_Dau1_Idx"]);
    ssbtree->Branch("GenPar_Dau2_Idx", &VectorBox_Int["GenPar_Dau2_Idx"]);
    ssbtree->Branch("GenPar_Dau_Counter", &VectorBox_Int["GenPar_Dau_Counter"]);
    ssbtree->Branch("GenPar_Idx", &VectorBox_Int["GenPar_Idx"]);
    ssbtree->Branch("GenPar_Mom1_Idx", &VectorBox_Int["GenPar_Mom1_Idx"]);
    ssbtree->Branch("GenPar_Mom2_Idx", &VectorBox_Int["GenPar_Mom2_Idx"]);
    ssbtree->Branch("GenPar_Mom_Counter", &VectorBox_Int["GenPar_Mom_Counter"]);
    ssbtree->Branch("GenPar_Status", &VectorBox_Int["GenPar_Status"]);
    ssbtree->Branch("GenPar_pdgId", &VectorBox_Int["GenPar_pdgId"]);
    ssbtree->Branch("Gen_EventWeight", &VariableBox_Double["Gen_EventWeight"], "Gen_EventWeight/D");
    ssbtree->Branch("GenTop", "TClonesArray", &VariableBox_LorentzVector["GenTop"], 32000, 0);
    VariableBox_LorentzVector["GenTop"]->BypassStreamer();
    ssbtree->Branch("GenAnTop", "TClonesArray", &VariableBox_LorentzVector["GenAnTop"], 32000, 0);
    VariableBox_LorentzVector["GenAnTop"]->BypassStreamer();
}

void SSBTreeManager::GenInitializeVariables(){

    VariableBox_Int["Info_EventNumber"] = 0;
    VariableBox_Int["Info_Luminosity"] = 0;
    VariableBox_Int["Info_RunNumber"] = 0;
    VariableBox_Bool["Info_isData"] = false;

    VariableBox_Int["Channel_Idx"] = 0;
    VariableBox_Int["Channel_Idx_Final"] = 0;

    VariableBox_Int["Channel_Jets"] = 0;
    VariableBox_Int["Channel_Jets_Abs"] = 0;

    VariableBox_Int["Channel_Lepton_Count"] = 0;
    VariableBox_Int["Channel_Lepton_Count_Final"] = 0;


    VariableBox_LorentzVector["GenJet"]->Clear();
    VariableBox_Int["GenJet_Count"] = 0;
    VectorBox_Float["GenJet_ECalEnergy"].clear();
    VectorBox_Float["GenJet_HCalEnergy"].clear();

    VariableBox_LorentzVector["GenMET"]->Clear();
    VariableBox_Int["GenMET_Count"] = 0;

    VariableBox_LorentzVector["GenPar"]->Clear();
    VariableBox_Int["GenPar_Count"] = 0;
    VectorBox_Int["GenPar_Dau1_Idx"].clear();
    VectorBox_Int["GenPar_Dau2_Idx"].clear();
    VectorBox_Int["GenPar_Dau_Counter"].clear();
    VectorBox_Int["GenPar_Idx"].clear();
    VectorBox_Int["GenPar_Mom1_Idx"].clear();
    VectorBox_Int["GenPar_Mom2_Idx"].clear();
    VectorBox_Int["GenPar_Mom_Counter"].clear();
    VectorBox_Int["GenPar_Status"].clear();
    VectorBox_Int["GenPar_pdgId"].clear();

    VariableBox_LorentzVector["GenTop"]->Clear();
    VariableBox_LorentzVector["GenAnTop"]->Clear();

    VariableBox_Double["Gen_EventWeight"] = 0;

}

