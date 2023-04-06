import FWCore.ParameterSet.Config as cms

# Define the CMSSW process
process = cms.Process("SSB")

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#################################################################
### METFilter -- To use badMuon and bad charged Hadron Filter ###
#################################################################
#process.load("CMSAnalyses.SSBAnalyzer.metFiltersV2_cff")
process.load("CMSAnalyses.SSBAnalyzer.metFilters_cff")

# Message Logger settings
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Set the process options -- Display summary at the end, enable unscheduled execution
process.options = cms.untracked.PSet( 
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(True) 
)

# How many events to process
process.maxEvents = cms.untracked.PSet( 
   input = cms.untracked.int32(-1),
)
########################
### Output filenames ###
########################
process.TFileService=cms.Service("TFileService",
        fileName=cms.string("SSBTree.root"),
        closeFileFast = cms.untracked.bool(True)
)

#configurable options =======================================================================
runOnData=False #data/MC switch
isMC=not runOnData
usePrivateSQlite=True #use external JECs (sqlite file)
useHFCandidates=True #create an additionnal NoHF slimmed MET collection if the option is set to false
redoPuppi=True # rebuild puppiMET
isSys=False
#===================================================================

### =====================================================================================================
# Define the input source

corList = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])
    
if runOnData:
  fname = 'file:/d3/scratch/sha/Analyses/DATA/MiniAOD/Run2016/PromptBv2/02D9C19F-571A-E611-AD8E-02163E013732.root'
  jecUncertainty="CMSAnalyses/SSBAnalyzer/data/Summer16_23Sep2016V4_DATA_Uncertainty_AK4PFchs.txt"
  corList = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])
  print ("Running on Data ...")

else:
#  fname = 'file:/pnfs/knu.ac.kr/data/cms/store/user/sha/MiniAODSample/MC/80X_v2/ForMoriond/TTbar/0693E0E7-97BE-E611-B32F-0CC47A78A3D8.root'
  fname = 'file:/pnfs/knu.ac.kr/data/cms/store/user/sha/MiniAODSample/MC/TTBarSync/36CDAE89-B3BE-E611-B022-0025905B8604.root'
  jecUncertainty="CMSAnalyses/SSBAnalyzer/data/JECDir/Summer16_23Sep2016/Summer16_23Sep2016V4_MC/Summer16_23Sep2016V4_MC_Uncertainty_AK4PFchs.txt"

# Define the input source
process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring([ fname ])
)



### External JECs =====================================================================================================

#from Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff import *
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.autoCond import autoCond
if runOnData:
  #process.GlobalTag.globaltag = autoCond['run2_data']
  process.GlobalTag.globaltag = '80X_dataRun2_2016SeptRepro_v7'
else:
  #process.GlobalTag.globaltag = autoCond['run2_mc']
  process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_TrancheIV_v8'

from CondCore.DBCommon.CondDBSetup_cfi import *
import os
if runOnData:
  Eras="Summer16_23Sep2016V4_DATA"
else:
  Eras="Spring16_25nsV6_MC"

##___________________________External JEC file________________________________||

process.jec = cms.ESSource("PoolDBESSource",CondDBSetup,
                           connect = cms.string("sqlite_fip:CMSAnalyses/SSBAnalyzer/data/JECDir/Spring16_25nsV6/Spring16_25nsV6_MC/Spring16_25nsV6_MC.db"),
                           toGet =  cms.VPSet(
        cms.PSet(
            record = cms.string("JetCorrectionsRecord"),
            tag = cms.string("JetCorrectorParametersCollection_"+Eras+"_AK4PF"),
            label= cms.untracked.string("AK4PF")
            ),
        cms.PSet(
            record = cms.string("JetCorrectionsRecord"),
            tag = cms.string("JetCorrectorParametersCollection_"+Eras+"_AK4PFchs"),
            label= cms.untracked.string("AK4PFchs")
            ),
        )
                           )
process.es_prefer_jec = cms.ESPrefer("PoolDBESSource",'jec')

from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

updateJetCollection(
    process,
    jetSource = cms.InputTag('slimmedJets'),
    labelName = 'UpdatedJEC',
    jetCorrections = ('AK4PFchs', corList, 'None')
#    btagDiscriminators = bTagDiscriminators
)
process.JEC = cms.Sequence(process.patJetCorrFactorsUpdatedJEC * process.updatedPatJetsUpdatedJEC)

########New

## Following lines are for default MET for Type1 corrections.
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD


# If you only want to re-correct for JEC and get the proper uncertainties for the default MET
runMetCorAndUncFromMiniAOD(process,
                       isData=runOnData,
                      )

# Now you are creating the bad muon corrected MET
process.load('RecoMET.METFilters.badGlobalMuonTaggersMiniAOD_cff')
process.badGlobalMuonTaggerMAOD.taggingMode = cms.bool(True)
process.cloneGlobalMuonTaggerMAOD.taggingMode = cms.bool(True)



from PhysicsTools.PatUtils.tools.muonRecoMitigation import muonRecoMitigation

muonRecoMitigation(
                   process = process,
                   pfCandCollection = "packedPFCandidates", #input PF Candidate Collection
                   runOnMiniAOD = True, #To determine if you are running on AOD or MiniAOD
                   selection="", #You can use a custom selection for your bad muons. Leave empty if you would like to use the bad muon recipe definition.
                   muonCollection="", #The muon collection name where your custom selection will be applied to. Leave empty if you would like to use the bad muon recipe definition.
                   cleanCollName="cleanMuonsPFCandidates", #output pf candidate collection ame
                   cleaningScheme="computeAllApplyClone", #Options are: "all", "computeAllApplyBad","computeAllApplyClone". Decides which (or both) bad muon collections to be used for MET cleaning coming from the bad muon recipe.
                   postfix="" #Use if you would like to add a post fix to your muon / pf collections
                   )

runMetCorAndUncFromMiniAOD(process,
                           isData=runOnData,
                           pfCandColl="cleanMuonsPFCandidates",
                           recoMetFromPFCs=True,
                           postfix="MuClean"
                           )

#########New End
process.mucorMET = cms.Sequence(                     
                    process.badGlobalMuonTaggerMAOD *
                    process.cloneGlobalMuonTaggerMAOD *
                    #process.badMuons * # If you are using cleaning mode "all", uncomment this line
                    process.cleanMuonsPFCandidates *
                    process.fullPatMetSequenceMuClean
                   )

#####################################
### Electron & Photon-- smearing ####
#####################################

from EgammaAnalysis.ElectronTools.regressionWeights_cfi import regressionWeights
process = regressionWeights(process)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                  calibratedPatElectrons  = cms.PSet( initialSeed = cms.untracked.uint32(8675389),
                                                      engineName = cms.untracked.string('TRandom3'),
                                                      ),
                  calibratedPatPhotons    = cms.PSet( initialSeed = cms.untracked.uint32(8675389),
                                                      engineName = cms.untracked.string('TRandom3'),
                                                      ),

                  ssbanalyzer    = cms.PSet( initialSeed = cms.untracked.uint32(8675389),
                                                      engineName = cms.untracked.string('TRandom3'),
                                                      ),
                                                   )
process.load('EgammaAnalysis.ElectronTools.regressionApplication_cff')
process.load('EgammaAnalysis.ElectronTools.calibratedPatElectronsRun2_cfi')
process.load('EgammaAnalysis.ElectronTools.calibratedPatPhotonsRun2_cfi')

process.load("RecoEgamma.ElectronIdentification.ElectronIDValueMapProducer_cfi")
process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")

process.load("RecoEgamma.PhotonIdentification.PhotonIDValueMapProducer_cfi")
process.load("RecoEgamma.PhotonIdentification.PhotonMVAValueMapProducer_cfi")

process.electronIDValueMapProducer.srcMiniAOD = cms.InputTag('slimmedElectrons')
process.electronMVAValueMapProducer.srcMiniAOD = cms.InputTag('slimmedElectrons')
process.photonIDValueMapProducer.srcMiniAOD = cms.InputTag('slimmedPhotons')
process.photonMVAValueMapProducer.srcMiniAOD = cms.InputTag('slimmedPhotons')

#process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('calibratedPatElectrons')
################################
### Electron & Photon -- ID ####
################################
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate 
dataFormat = DataFormat.MiniAOD
switchOnVIDElectronIdProducer(process, dataFormat)
switchOnVIDPhotonIdProducer(process, dataFormat)

# define which IDs we want to produce

my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronHLTPreselecition_Summer16_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff',
                 'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_GeneralPurpose_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_HZZ_V1_cff']



#add them to the VID producer
### Electron ID
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)
                                                                
my_phoid_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring16_V2p2_cff',
                    'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Spring16_nonTrig_V1_cff']

#add them to the VID producer
### Photon ID
for idmod in my_phoid_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

####################################################
### GenJet B-Hadron Matcher For JetAngular Study ###
####################################################

genJetCollection = 'ak4GenJetsCustom'
genParticleCollection = 'prunedGenParticles'
genJetInputParticleCollection = 'packedGenParticles'

## producing a subset of genParticles to be used for jet reclustering
from RecoJets.Configuration.GenJetParticles_cff import genParticlesForJetsNoNu
process.genParticlesForJetsCustom = genParticlesForJetsNoNu.clone(
   #src = genParticleCollection
    src = genJetInputParticleCollection
)
# Producing own jets for testing purposes
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.ak4GenJetsCustom = ak4GenJets.clone(
    src = 'genParticlesForJetsCustom',
    #src = 'genParticlesForJetsNoNu',
    rParam = cms.double(0.4),
    jetAlgorithm = cms.string("AntiKt")
)


# Supplies PDG ID to real name resolution of MC particles
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# Ghost particle collection used for Hadron-Jet association 
# MUST use proper input particle collection
from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone(
    particles = genParticleCollection
)

# Input particle collection for matching to gen jets (partons + leptons) 
# MUST use use proper input jet collection: the jets to which hadrons should be associated
# rParam and jetAlgorithm MUST match those used for jets to be associated with hadrons
# More details on the tool: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagMCTools#New_jet_flavour_definition
from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
process.genJetFlavourInfos = ak4JetFlavourInfos.clone(
    jets = genJetCollection,
)

# Plugin for analysing B hadrons
# MUST use the same particle collection as in selectedHadronsAndPartons
from PhysicsTools.JetMCAlgos.GenHFHadronMatcher_cff import matchGenBHadron
process.matchGenBHadron = matchGenBHadron.clone(
    genParticles = genParticleCollection,
    jetFlavourInfos = "genJetFlavourInfos"
)

# Plugin for analysing C hadrons
# MUST use the same particle collection as in selectedHadronsAndPartons
from PhysicsTools.JetMCAlgos.GenHFHadronMatcher_cff import matchGenCHadron
process.matchGenCHadron = matchGenCHadron.clone(
    genParticles = genParticleCollection,
    jetFlavourInfos = "genJetFlavourInfos"
)

process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = cms.untracked.vstring( "keep *_*_*_*",
                                            ),
    fileName = cms.untracked.string('corMETMiniAOD.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    ),
    dropMetaData = cms.untracked.string('ALL'),
    fastCloning = cms.untracked.bool(False),
    overrideInputFileSplitLevels = cms.untracked.bool(True)
)

####################
### SSB Analyzer ###
####################

process.ssbanalyzer = cms.EDAnalyzer('SSBAnalyzer',
                                    BadChargedCandidateFilter  = cms.InputTag("BadChargedCandidateFilter",""),
                                    BadPFMuonFilter  = cms.InputTag("BadPFMuonFilter",""),
                                    #badGlobalMuonTagger  = cms.InputTag("badGlobalMuonTagger",""),
                                    #cloneGlobalMuonTagger  = cms.InputTag("cloneGlobalMuonTagger",""),
                                    bitsPat                    = cms.InputTag("TriggerResults","","PAT"),
                                    isMCTag         = cms.bool(isMC),
                                    PDFInfoTag      = cms.InputTag("generator",""),
                                    FixPOWHEG = cms.untracked.string("NNPDF30_nlo_as_0118.LHgrid"),
                                    PDFSetNames     = cms.vstring('NNPDF30_nlo_as_0118.LHgrid','CT10nnlo.LHgrid'),#"cteq66.LHgrid"), #'MRST2006nnlo.LHgrid'),# 'NNPDF10_100.LHgrid'), 
                                    PDFCent         = cms.bool(True),
                                    PDFSys          = cms.bool(True),
                                    pvTag           = cms.InputTag("offlineSlimmedPrimaryVertices",""),
                                    genEvnTag = cms.InputTag("generator"),
                                    genLHETag = cms.InputTag("externalLHEProducer"),
                                    genParTag       = cms.InputTag("prunedGenParticles"),
                                    genJetTag       = cms.InputTag("slimmedGenJets",""),
                                    genJetReclusTag = cms.InputTag("ak4GenJetsCustom",""),
                                    genBHadPlusMothersTag   = cms.InputTag("matchGenBHadron","genBHadPlusMothers"),
                                    genBHadIndexTag   = cms.InputTag("matchGenBHadron","genBHadIndex"),
                                    genBHadFlavourTag   = cms.InputTag("matchGenBHadron","genBHadFlavour"),
                                    genBHadFromTopWeakDecayTag   = cms.InputTag("matchGenBHadron","genBHadFromTopWeakDecay"),
                                    genBHadJetIndexTag   = cms.InputTag("matchGenBHadron","genBHadJetIndex"),

                                    genMETTag = cms.InputTag("slimmedMETsMuClean","","SSB"),
                                    isSignal        = cms.bool(True),
                                    RhoTag          = cms.InputTag("fixedGridRhoFastjetAll"),
                                    puTag           = cms.InputTag("slimmedAddPileupInfo",""),
                                    trigList        = cms.vstring(
                                                        ### SingleMuon ###
                                                        'HLT_IsoMu20_v',
                                                        'HLT_IsoMu24_v',# 2016 study
                                                        'HLT_IsoTkMu24_v',# 2016 study
                                                        'HLT_IsoMu22_eta2p1_v',# 2016 study
                                                        'HLT_IsoTkMu22_eta2p1_v',# 2016 study
                                                        'HLT_IsoMu20_eta2p1_TriCentralPFJet30_v',
                                                        'HLT_IsoMu20_eta2p1_TriCentralPFJet50_40_30_v',
                                                        ### SingleElectron ###
                                                        'HLT_Ele23_WPLoose_Gsf_v',
                                                        'HLT_Ele27_eta2p1_WPLoose_Gsf_v', ## For 2015 study 
                                                        'HLT_Ele27_eta2p1_WPLoose_Gsf_TriCentralPFJet30_v', # Will be Removed
                                                        'HLT_Ele27_eta2p1_WPLoose_Gsf_CentralPFJet30_BTagCSV07_v', # Will be Removed
                                                        'HLT_Ele27_eta2p1_WPLoose_Gsf_TriCentralPFJet50_40_30_v', # Will be Removed
                                                        'HLT_Ele32_eta2p1_WPTight_Gsf_v', ## 2016 study
                                                        'HLT_Ele27_WPTight_Gsf_v', ## 2016 study
                                                        'HLT_Ele25_eta2p1_WPTight_Gsf_v', ## 2016 study
                                                        ### DoubleMuon ###
                                                        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v', ## 2016 study
                                                        'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v', ## 2016 study
                                                        'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v', ## 2015 study
                                                        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v', ## 2015 study
                                                        ### DoubleElectron ###
                                                        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v', # for 2016
                                                        'HLT_DoubleEle24_22_eta2p1_WPLoose_Gsf_v', # for 2016
                                                        'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v',#for 2015
                                                        ### MuonElectron ###
                                                        'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',#for 2016 
                                                        'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v',#for 2016 
                                                        'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v',#for 2016 
                                                        'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v',#for 2016 
                                                        'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v',#for 2016
                                                        'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v',# for 2016
                                                        ### AllHadronic ### 
                                                        'HLT_PFHT450_SixJet40_BTagCSV_p056_v',#for 2016 
                                                        'HLT_PFHT400_SixJet30_DoubleBTagCSV_p056_v',#for 2016 
                                                        'HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',#for 2015
                                                        'HLT_BTagMu_DiJet20_Mu5_v', # For SoftMuon Trigger 
                                                        'HLT_BTagMu_DiJet40_Mu5_v',
                                                        'HLT_BTagMu_DiJet70_Mu5_v',
                                                        'HLT_BTagMu_DiJet110_Mu5_v',
                                                        'HLT_BTagMu_DiJet300_Mu5_v',
                                                        ),
                                    bits             = cms.InputTag("TriggerResults","","HLT"),
                                    prescales        = cms.InputTag("patTrigger"),

                                    ismuSysTag       = cms.bool(isSys), # For Systemtic Study ...

                                    muTag            = cms.InputTag("slimmedMuons",""),
                                    muEnUpTag        = cms.InputTag("shiftedPatMuonEnUp",""),
                                    muEnDownTag      = cms.InputTag("shiftedPatMuonEnDown",""),
                                    eleTag           = cms.InputTag("slimmedElectrons",""),
                                    electronPATInput = cms.InputTag("calibratedPatElectrons",""),
                                    eleEnUpTag       = cms.InputTag("shiftedPatElectronEnUp",""),
                                    eleEnDownTag     = cms.InputTag("shiftedPatElectronEnDown",""),

                                    effAreasConfigFile = cms.FileInPath("RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt"),

                                    eleVetoIdMap    = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto"   ),
                                    eleLooseIdMap   = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose"  ),
                                    eleMediumIdMap  = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium"   ),
                                    eleTightIdMap   = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight"    ),
                                    eleHEEPIdMap    = cms.InputTag( "egmGsfElectronIDs:heepElectronID-HEEPV70"                      ),
                                    eleHLTIdMap     = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronHLTPreselection-Summer16-V1" ),

                                    # ID decisions (mavid)

                                    mva_eleMediumMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp90"),
                                    mva_eleTightMap  = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp80"),
                                    mva_eleHZZIDMap  = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring16-HZZ-V1-wpLoose"), # will be removed
                                    #
                                    # ValueMaps with MVA results
                                    #
                                    mvaValuesMap     = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Values"),
                                    mvaCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Categories"),
                                    mvaValuesHZZMap  = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16HZZV1Values"),
                                    mvaCategoriesHZZMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16HZZV1Categories"),
                                    
                                    phoTag          = cms.InputTag("slimmedPhotons",""),
                                    photonPATInput  = cms.InputTag("calibratedPatPhotons",""),
                                    phoLooseIdMap   = cms.InputTag( "egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-loose"  ),
                                    phoMediumIdMap  = cms.InputTag( "egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-medium" ),
                                    phoTightIdMap   = cms.InputTag( "egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-tight"  ), 

                                    pho_mva_NontrigTightIdMap = cms.InputTag("egmPhotonIDs:mvaPhoID-Spring16-nonTrig-V1-wp90"),

                                    pho_mvaValuesMap          = cms.InputTag("photonMVAValueMapProducer:PhotonMVAEstimatorRun2Spring16NonTrigV1Values"),
                                    full5x5SigmaIEtaIEtaMap   = cms.InputTag("photonIDValueMapProducer:phoFull5x5SigmaIEtaIEta"),
                                    phoChargedIsolation       = cms.InputTag('photonIDValueMapProducer:phoChargedIsolation'),
                                    phoNeutralHadronIsolation = cms.InputTag('photonIDValueMapProducer:phoNeutralHadronIsolation'),
                                    phoPhotonIsolation        = cms.InputTag('photonIDValueMapProducer:phoPhotonIsolation'),
                                    phoWorstChargedIsolation  = cms.InputTag('photonIDValueMapProducer:phoWorstChargedIsolation'),


                                    effAreaChHadFile  = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfChargedHadrons_90percentBased.txt"),
                                    effAreaNeuHadFile = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfNeutralHadrons_90percentBased.txt"),
                                    effAreaPhoFile    = cms.FileInPath("RecoEgamma/PhotonIdentification/data/Spring16/effAreaPhotons_cone03_pfPhotons_90percentBased.txt"),

                                    pfCands         = cms.InputTag("packedPFCandidates"),
                                    bstag           = cms.InputTag("offlineBeamSpot"),
                                    convertag       = cms.InputTag("reducedEgamma","reducedConversions"),
                                    
                                    isjtcutTag = cms.bool(False), #For Jet
                                    jtTag = cms.InputTag("updatedPatJetsUpdatedJEC",""), #For Jet
                                    jtpuppiTag = cms.InputTag("slimmedJets",""), #For Jet
                                    PayLoadName = cms.string('AK4PFchs'),
                                    phiResolMCFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_MC_PhiResolution_AK4PFchs.txt'),
                                    phiResolDataFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_DATA_PhiResolution_AK4PFchs.txt'),
                                    ptResolMCFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_DATA_PtResolution_AK4PFchs.txt'),
                                    ptResolSFFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_MC_SF_AK4PFchs.txt'),
                                    csvbjetTag = cms.string("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
                                    btagListTag        = cms.vstring(
                                                         'pfCombinedInclusiveSecondaryVertexV2BJetTags',
                                                         'softPFMuonBJetTags',
                                                         'softPFMuonByIP3dBJetTags',
                                                         'softPFElectronByPtBJetTags',
                                                         'softPFElectronBJetTags',
                                                         'softPFMuonByPtBJetTags',
                                                         'softPFElectronByIP3dBJetTags',
                                                         'softPFMuonByIP2dBJetTags',
                                                         'softPFElectronByIP2dBJetTags'
                                                      ),
                                    PFTightJetID = cms.PSet(
                                                       version = cms.string('RUNIISTARTUP'),
                                                       quality = cms.string('TIGHT')
                                                      ),
                                    PFLooseJetID = cms.PSet(
                                                       version = cms.string('RUNIISTARTUP'),
                                                       quality = cms.string('LOOSE')
                                                      ),
                                    metTag = cms.InputTag("slimmedMETs","","SSB"),
                                    metmucleancorTag = cms.InputTag("slimmedMETsMuClean","","SSB"),
)
process.p = cms.Path(
        process.regressionApplication*
        process.calibratedPatElectrons*
        process.calibratedPatPhotons*
        process.mucorMET*
        process.fullPatMetSequence*
        process.JEC*
  	(process.egmPhotonIDSequence + process.egmGsfElectronIDSequence ) *
        process.ssbanalyzer
)
