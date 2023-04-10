import FWCore.ParameterSet.Config as cms

# Define the CMSSW process
process = cms.Process("SSB")

# Configurable options =======================================================================
year = 2016 # Options: 2016, 2017, 2018
period = 'UL2017' # Options: UL2016APV, UL2016, UL2017, UL2018

#configurable options =======================================================================
runOnData=False #data/MC switch
isMC=not runOnData
isSys=False
#===================================================================



# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

if runOnData : process.GlobalTag.globaltag = '106X_dataRun2_v37'

else:
    if period is 'UL2016' :   process.GlobalTag.globaltag = '106X_mcRun2_asymptotic_v17'
    elif period is 'UL2016APV' : process.GlobalTag.globaltag = '106X_mcRun2_asymptotic_preVFP_v11'
    elif period is 'UL2017' : process.GlobalTag.globaltag = '106X_mc2017_realistic_v9'
    elif period is 'UL2018' : process.GlobalTag.globaltag = '106X_upgrade2018_realistic_v16_L1v1'



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
   input = cms.untracked.int32(100),
   #input = cms.untracked.int32(-1),
)
########################
### Output filenames ###
########################
process.TFileService=cms.Service("TFileService",
        fileName=cms.string("SSBTree.root"),
        closeFileFast = cms.untracked.bool(True)
)

### =====================================================================================================
# Define the input source

corList = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])
    
if runOnData:
  fname = "/store/mc/RunIISummer20UL16MiniAODv2/DY1JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/106X_mcRun2_asymptotic_v17-v1/120000/00061BF0-5BB0-524E-A539-0CAAD8579386.root" #UL2016 MC
  corList = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])
  print ("Running on Data ...")

else:
  fname = 'file:../../0004BE39-823E-4A4B-9727-C2544050C4C0.root'

# Define the input source
process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring([ fname ])
)



### External JECs =====================================================================================================
# From :  https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#CorrPatJets

from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
updateJetCollection(
  process,
  jetSource = cms.InputTag('slimmedJets'),
  labelName = 'UpdatedJEC',
  jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
)
process.jecSequence = cms.Sequence(process.patJetCorrFactorsUpdatedJEC * process.updatedPatJetsUpdatedJEC)

updateJetCollection(
  process,
  jetSource = cms.InputTag('slimmedJetsPuppi'),
  labelName = 'UpdatedJECPuppi',
  jetCorrections = ('AK4PFPuppi', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
)
process.jecSequencepuppi = cms.Sequence(process.patJetCorrFactorsUpdatedJECPuppi * process.updatedPatJetsUpdatedJECPuppi)

### END JECs ==========================================================================================


## Following lines are for default MET for Type1 corrections.
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

# If you only want to re-correct for JEC and get the proper uncertainties for the default MET
runMetCorAndUncFromMiniAOD(process,
                          isData=runOnData,
                          )
#####################################
### Electron & Photon-- smearing ####
#####################################

from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq

#print ("eleIDModules : %s "%(eleIDModules))
if period is 'UL2016APV' :                                                                                                             
    labelEra = '2016preVFP-UL'
    rerunIDs = True                                                                                                                    
    rerunEnergyCorrections = True
 #   eleIDModules=['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Summer16UL_ID_ISO_cff','RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff'],
 #   phoIDModules=['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff'],
elif period is 'UL2016' :
    labelEra = '2016postVFP-UL'                                                                                                        
    rerunIDs = True
    rerunEnergyCorrections = True                                                                                                      
 #   eleIDModules=['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Summer17UL_ID_ISO_cff', 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff'],
 #   phoIDModules=['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff'],
elif period is 'UL2017' :
    labelEra = '2017-UL'                                                                                                               
    rerunIDs = True
    rerunEnergyCorrections = True
 #   eleIDModules=['','RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Summer17UL_ID_ISO_cff', 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff'],
 #   phoIDModules=['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff'],

elif period is 'UL2018' :                                                                                                              
    labelEra = '2018-UL'                                                                                                               
    rerunIDs = True
    rerunEnergyCorrections = True
 #   eleIDModules=['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Summer18UL_ID_ISO_cff','RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff','RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V2_cff'],
 #   phoIDModules=['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff'],

setupEgammaPostRecoSeq(process,
                       runVID=rerunIDs,
                       runEnergyCorrections=rerunEnergyCorrections,
                       era=labelEra)

#a sequence egammaPostRecoSeq has now been created and should be added to your path, eg process.p=cms.Path(process.egammaPostRecoSeq)

#######################################################################

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                  ssbanalyzer    = cms.PSet( initialSeed = cms.untracked.uint32(8675389),
                                                      engineName = cms.untracked.string('TRandom3'),
                                                      )
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
                                    bitsPat                    = cms.InputTag("TriggerResults","","PAT"),
                                    isMCTag         = cms.bool(isMC),
                                    PDFInfoTag      = cms.InputTag("generator",""),
                                    FixPOWHEG = cms.untracked.string("NNPDF30_nlo_as_0118.LHgrid"),
                                    PDFSetNames     = cms.vstring('NNPDF30_nlo_as_0118.LHgrid','CT10nnlo.LHgrid'),#"cteq66.LHgrid"), #'MRST2006nnlo.LHgrid'),# 'NNPDF10_100.LHgrid'), 
                                    PDFCent         = cms.bool(True),
                                    PDFSys          = cms.bool(True),
                                    doFragsys        = cms.bool(True),
                                    pvTag           = cms.InputTag("offlineSlimmedPrimaryVertices",""),
                                    genEvnTag = cms.InputTag("generator"),
                                    genLHETag = cms.InputTag("externalLHEProducer"),
                                    genParTag       = cms.InputTag("prunedGenParticles"),
                                    genJetTag       = cms.InputTag("slimmedGenJets",""),
                                    genJetReclusTag = cms.InputTag("ak4GenJetsCustom",""),
                                    genMETTag = cms.InputTag("slimmedMETs","","SSB"),
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
                                    electronPATInput = cms.InputTag("selectedElectrons",""),
                                    #electronPATInput = cms.InputTag("calibratedPatElectrons",""),
                                    #eleTag           = cms.InputTag("selectedElectrons",""),
                                    #electronPATInput = cms.InputTag("selectedElectrons",""),
                                    eleEnUpTag       = cms.InputTag("shiftedPatElectronEnUp",""),
                                    eleEnDownTag     = cms.InputTag("shiftedPatElectronEnDown",""),

                                    effAreasConfigFile = cms.FileInPath("RecoEgamma/ElectronIdentification/data/Fall17/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_92X.txt"),

                                    eleVetoIdMap    = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto"   ),
                                    eleLooseIdMap   = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose"  ),
                                    eleMediumIdMap  = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium"   ),
                                    eleTightIdMap   = cms.InputTag( "egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight"    ),
                                    eleHEEPIdMap    = cms.InputTag( "egmGsfElectronIDs:heepElectronID-HEEPV60"                      ),
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
                                    
                                    #phoTag          = cms.InputTag("slimmedPhotons",""),
                                    #photonPATInput  = cms.InputTag("calibratedPatPhotons",""),
                                    phoTag          = cms.InputTag("selectedPhotons",""),
                                    photonPATInput  = cms.InputTag("selectedPhotons",""),
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

                                    recHitCollectionEB  = cms.InputTag("reducedEgamma", "reducedEBRecHits"),
                                    recHitCollectionEE  = cms.InputTag("reducedEgamma", "reducedEBRecHits"),
                                    pfCands         = cms.InputTag("packedPFCandidates"),
                                    bstag           = cms.InputTag("offlineBeamSpot"),
                                    convertag       = cms.InputTag("reducedEgamma","reducedConversions"),
                                    
                                    isjtcutTag = cms.bool(False), #For Jet
                                    jtTag = cms.InputTag("updatedPatJetsUpdatedJEC",""), #For Jet
                                    jtpuppiTag = cms.InputTag("slimmedJets",""), #For Jet
                                    PayLoadName = cms.string('AK4PFchs'),
                                    phiResolMCFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV10/Spring16_25nsV10_MC_PhiResolution_AK4PFchs.txt'),
                                    phiResolDataFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_DATA_PhiResolution_AK4PFchs.txt'),
                                    ptResolMCFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV10/Spring16_25nsV10_MC_PtResolution_AK4PFchs.txt'),
                                    ptResolDataFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_DATA_PtResolution_AK4PFchs.txt'),
                                    #ptResolSFFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV10/Spring16_25nsV10_MC_SF_AK4PFchs.txt'),
                                    ptResolSFFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV10a/Spring16_25nsV10a_MC_SF_AK4PFchs.txt'),
                                    csvbjetTag = cms.string("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
                                    btagListTag        = cms.vstring(
                                                         'pfCombinedInclusiveSecondaryVertexV2BJetTags',
                                                         'pfDeepCSVJetTags:probb',
                                                         'pfDeepCSVJetTags:probbb',
                                                         'pfDeepCSVJetTags:probc',
                                                         'pfDeepCSVJetTags:probudsg',
                                                         'pfDeepFlavourJetTags:probb',
                                                         'pfDeepFlavourJetTags:probbb',
                                                         'pfDeepFlavourJetTags:problepb',
                                                         'pfDeepFlavourJetTags:probc',
                                                         'pfDeepFlavourJetTags:probuds',
                                                         'pfDeepFlavourJetTags:probg',
                                                         'pfDeepCSVDiscriminatorsJetTags:BvsAll', 
                                                         'pfDeepCSVDiscriminatorsJetTags:CvsB',
                                                         'pfDeepCSVDiscriminatorsJetTags:CvsL'
                                                         ),

                                    PFTightJetID = cms.PSet(
                                                       version = cms.string('RUN2ULCHS'),
                                                       quality = cms.string('TIGHT')
                                                      ),
                                    PFLooseJetID = cms.PSet(
                                                       version = cms.string('RUN2ULCHS'),
                                                       quality = cms.string('TIGHT')
                                                      ),
                                    metTag = cms.InputTag("slimmedMETs","","SSB"),
)
process.p = cms.Path(
        process.egammaPostRecoSeq*
        process.fullPatMetSequence*
        process.jecSequence*
        process.ssbanalyzer
)
