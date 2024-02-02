import FWCore.ParameterSet.Config as cms

# Define the CMSSW process
process = cms.Process("SSB")

# Configurable options =======================================================================
#period = 'UL2018' # Options: UL2016APV, UL2016, UL2018, UL2018
period = 'UL2017' # Options: UL2016APV, UL2016, UL2018, UL2018

#configurable options =======================================================================
runOnData=False #data/MC switch
isMC=not runOnData
isSys=False
#===================================================================



# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
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
  fname = 'file:/pnfs/knu.ac.kr/data/cms/store/user/sha/Run2_UL/MC/TTBarDiLep/04A0B676-D63A-6D41-B47F-F4CF8CBE7DB8.root'

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
    #eleIDModules=['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Summer16UL_ID_ISO_cff','RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff','RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff']
    #phoIDModules=['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff']
elif period is 'UL2016' :
    labelEra = '2016postVFP-UL'                                                                                                        
    rerunIDs = True
    rerunEnergyCorrections = True                                                                                                      
    #eleIDModules=['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Summer17UL_ID_ISO_cff', 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff','RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff']
    #phoIDModules=['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff']
elif period is 'UL2017' :
    labelEra = '2017-UL'                                                                                                               
    rerunIDs = True
    rerunEnergyCorrections = True
    #eleIDModules=['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Summer17UL_ID_ISO_cff', 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff','RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff']
    #phoIDModules=['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff']

elif period is 'UL2018' :                                                                                                              
    labelEra = '2018-UL'                                                                                                               
    rerunIDs = True
    rerunEnergyCorrections = True
    #eleIDModules=['RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Summer18UL_ID_ISO_cff','RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff','RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V2_cff','RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff']
    #phoIDModules=['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff']

eleIDModules =  [
'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff',
'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff',
'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V2_cff',
'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V2_cff',
]
phoIDModules =  [
'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Fall17_94X_V2_cff',
'RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff',
]


#print ("eleIDModules : %s "%(eleIDModules))
setupEgammaPostRecoSeq(process,
                       runVID=rerunIDs,
                       runEnergyCorrections=rerunEnergyCorrections,
                       eleIDModules=eleIDModules,
                       phoIDModules=phoIDModules,
                       era=labelEra)

#a sequence egammaPostRecoSeq has now been created and should be added to your path, eg process.p=cms.Path(process.egammaPostRecoSeq)

#######################################################################

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                  ssbanalyzer    = cms.PSet( initialSeed = cms.untracked.uint32(8675389),
                                                      engineName = cms.untracked.string('TRandom3'),
                                                      )
                                                   )
#######################
#### L1 Prefiring #####
#### https://twiki.cern.ch/twiki/bin/viewauth/CMS/L1PrefiringWeightRecipe
#########################################################################

from PhysicsTools.PatUtils.l1PrefiringWeightProducer_cfi import l1PrefiringWeightProducer

#print ("eleIDModules : %s "%(eleIDModules))
if period is 'UL2016APV' :                                                                                                             
    DataEraECAL = 'UL2016preVFP'
    DataEraMuon = '2016preVFP'
elif period is 'UL2016' :
    DataEraECAL = 'UL2016postVFP'
    DataEraMuon = '2016postVFP'
elif period is 'UL2017' :
    DataEraECAL = 'UL2017BtoF'
    DataEraMuon = '20172018'
elif period is 'UL2018' :
    DataEraECAL = 'None'
    DataEraMuon = '20172018'

process.prefiringweight = l1PrefiringWeightProducer.clone(
  TheJets = cms.InputTag("updatedPatJetsUpdatedJEC"), #this should be the slimmedJets collection with up to date JECs !
  DataEraECAL = cms.string(DataEraECAL),
  DataEraMuon = cms.string(DataEraMuon),
  #DataEraMuon = cms.string("20172018"),
  UseJetEMPt = cms.bool(False),
  PrefiringRateSystematicUnctyECAL = cms.double(0.2),
  PrefiringRateSystematicUnctyMuon = cms.double(0.2)
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

#########################
####### JER Files #######
#########################
if period is 'UL2016APV' :                                                                                                             
    phiResolMCFileName   = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16APV_JRV3_MC/Summer20UL16APV_JRV3_MC_PhiResolution_AK4PFchs.txt'
    phiResolDataFileName = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16APV_JRV3_DATA/Summer20UL16APV_JRV3_DATA_PhiResolution_AK4PFchs.txt'
    ptResolMCFileName    = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16APV_JRV3_MC/Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs.txt'
    ptResolDataFileName  = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16APV_JRV3_DATA/Summer20UL16APV_JRV3_DATA_PtResolution_AK4PFchs.txt'
    ptResolSFFileName    = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16APV_JRV3_MC/Summer20UL16APV_JRV3_MC_SF_AK4PFchs.txt'
elif period is 'UL2016' :
    phiResolMCFileName   = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16_JRV3_MC/Summer20UL16_JRV3_MC_PhiResolution_AK4PFchs.txt'
    phiResolDataFileName = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16_JRV3_DATA/Summer20UL16_JRV3_DATA_PhiResolution_AK4PFchs.txt'
    ptResolMCFileName    = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16_JRV3_MC/Summer20UL16_JRV3_MC_PtResolution_AK4PFchs.txt'
    ptResolDataFileName  = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16_JRV3_DATA/Summer20UL16_JRV3_DATA_PtResolution_AK4PFchs.txt'
    ptResolSFFileName    = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer20UL16_JRV3_MC/Summer20UL16_JRV3_MC_SF_AK4PFchs.txt'
elif period is 'UL2017' :
    phiResolMCFileName   = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL17_JRV3_MC/Summer19UL17_JRV3_MC_PhiResolution_AK4PFchs.txt'
    phiResolDataFileName = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL17_JRV3_DATA/Summer19UL17_JRV3_DATA_PhiResolution_AK4PFchs.txt'
    ptResolMCFileName    = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL17_JRV3_MC/Summer19UL17_JRV3_MC_PtResolution_AK4PFchs.txt'
    ptResolDataFileName  = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL17_JRV3_DATA/Summer19UL17_JRV3_DATA_PtResolution_AK4PFchs.txt'
    ptResolSFFileName    = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL17_JRV3_MC/Summer19UL17_JRV3_MC_SF_AK4PFchs.txt'
elif period is 'UL2018' :
    phiResolMCFileName   = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL18_JRV2_MC/Summer19UL18_JRV2_MC_PhiResolution_AK4PFchs.txt'
    phiResolDataFileName = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL18_JRV2_DATA/Summer19UL18_JRV2_DATA_PhiResolution_AK4PFchs.txt'
    ptResolMCFileName    = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL18_JRV2_MC/Summer19UL18_JRV2_MC_PtResolution_AK4PFchs.txt'
    ptResolDataFileName  = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL18_JRV2_DATA/Summer19UL18_JRV2_DATA_PtResolution_AK4PFchs.txt'
    ptResolSFFileName    = 'CMSAnalyses/SSBAnalyzer/data/JRDatabase/Summer19UL18_JRV2_MC/Summer19UL18_JRV2_MC_SF_AK4PFchs.txt'
else : print("Check out Run Period!!!")

####################
### SSB Analyzer ###
####################

process.ssbanalyzer = cms.EDAnalyzer('SSBAnalyzer',
                                    RunYear = cms.untracked.string(period),
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
                                                        'HLT_IsoMu20_v', #2016
                                                        'HLT_IsoMu24_v', # 2016 2018
                                                        'HLT_IsoTkMu24_v', # 2016
                                                        'HLT_IsoMu22_eta2p1_v', #2016
                                                        'HLT_IsoTkMu22_eta2p1_v', #2016
                                                        'HLT_Mu50_v', #2016
                                                        'HLT_TkMu50_v', #2016
                                                        'HLT_Mu45_eta2p1_v',#2016 
                                                        'HLT_IsoMu27_v',#2017
                                                        'HLT_IsoMu24_eta2p1_v', #2017
                                                        'HLT_IsoMu27_v', #2017
                                                        'HLT_OldMu100_v', #2017&2018
                                                        'HLT_TkMu100_v', #2017&2018
                                                        ### SingleElectron ###
                                                        'HLT_Ele32_eta2p1_WPTight_Gsf_v', #2016 
                                                        'HLT_Ele27_WPTight_Gsf_v', ## 2016 study
                                                        'HLT_Ele25_eta2p1_WPTight_Gsf_v', ## 2016 study
                                                        'HLT_Ele35_WPTight_Gsf_v', ## 2017 study
                                                        'HLT_Ele38_WPTight_Gsf_v', ## 2017 study
                                                        'HLT_Ele40_WPTight_Gsf_v', ## 2017 study
                                                        'HLT_Ele32_WPTight_Gsf_L1DoubleEG_v', ## 2017 study
                                                        'HLT_Ele28_eta2p1_WPTight_Gsf_HT150_v', ## 2017 & 2018 study #E+HT Trigger
                                                        'HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned_v', ## 2017 & 2018 study #E+HT Trigger
                                                        'HLT_Ele32_WPTight_Gsf_v', ## 2018 study 
                                                        ### DoubleMuon ###
                                                        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v', ## 2016 study
                                                        'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v', ## 2016 study
                                                        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ', ## 2017 study
                                                        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v', ## 2017 study
                                                        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v', ## 2017 & 2018 study
                                                        ### DoubleElectron ###
                                                        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v', # for 2016 & 2017 & 2018
                                                        'HLT_DoubleEle24_22_eta2p1_WPLoose_Gsf_v', # for 2016
                                                        'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v',#for 2016
                                                        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v',#for 2017
                                                        #'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v',#for 2017
                                                        ### MuonElectron ###
                                                        'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',# for 2016
                                                        'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v',# for 2016
                                                        'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v',# for 2016
                                                        'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v',# for 2016 & 2017 & 2018
                                                        'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v',# for 2016
                                                        'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v',# for 2016
                                                        #'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',# for 2017 & 2016
                                                        #'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v',# for 2017 & 2016
                                                        #'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v',# for 2017 & 2016
                                                        #'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v',# for 2016 & 2017
                                                        ### AllHadronic ### 
                                                        'HLT_PFHT450_SixJet40_BTagCSV_p056_v',#for 2016 
                                                        'HLT_PFHT400_SixJet30_DoubleBTagCSV_p056_v',#for 2016 
                                                        'HLT_PFHT380_SixJet32_DoubleBTagCSV_p075_v',#for 2016 
                                                        'HLT_PFHT430_SixJet40_BTagCSV_p080_v',#for 2017
                                                        'HLT_PFHT380_SixPFJet32_DoublePFBTagCSV_2p2_v',#for 2017
                                                        'HLT_PFHT430_SixPFJet40_PFBTagCSV_1p5_v',#for 2017
                                                        'HLT_PFHT380_SixPFJet32_DoublePFBTagDeepCSV_2p2_v',#for 2017
                                                        'HLT_PFHT430_SixPFJet40_PFBTagDeepCSV_1p5_v',#for 2018
                                                        #'HLT_PFHT380_SixPFJet32_DoublePFBTagDeepCSV_2p2_v',#for 2017
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
                                    
                                    phoTag          = cms.InputTag("slimmedPhotons",""),
                                    #photonPATInput  = cms.InputTag("calibratedPatPhotons",""),
                                    #phoTag          = cms.InputTag("slimmedElectrons",""),
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
                                    phiResolMCFile   = cms.FileInPath(phiResolMCFileName),
                                    phiResolDataFile = cms.FileInPath(phiResolDataFileName),
                                    ptResolMCFile    = cms.FileInPath(ptResolMCFileName),
                                    ptResolDataFile  = cms.FileInPath(ptResolDataFileName),
                                    ptResolSFFile    = cms.FileInPath(ptResolSFFileName),
                                    csvbjetTag = cms.string("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
                                    #csvbjetTag = cms.string('pfDeepCSVDiscriminatorsJetTags:BvsAll'), 
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
        process.prefiringweight* 
        process.ssbanalyzer
)
