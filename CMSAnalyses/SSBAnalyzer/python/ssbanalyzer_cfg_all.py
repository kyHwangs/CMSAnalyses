import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.register ("isData", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "If 'isData' is true, input files should be Real Data...")
options.register ("isSig", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "If 'isSig' is true, input files should be MC signal Data..")
options.register ('globalTag',"80X_mcRun2_asymptotic_2016_TrancheIV_v7",VarParsing.multiplicity.singleton,VarParsing.varType.string,'input global tag to be used')
options.register ('certFile','',VarParsing.multiplicity.singleton,VarParsing.varType.string,"json file")
options.register ('JECUnc','Summer16_23Sep2016V4_MC',VarParsing.multiplicity.singleton,VarParsing.varType.string,"JEC File ")
options.parseArguments()

print "isData : %s"%(options.isData)
print "isSig : %s"%(options.isSig)
print "maxEvents : %s"%(options.maxEvents)
print "Cert JSon : %s "%(options.certFile)
print "GlobalTag : %s"%(options.globalTag)
print "JEC: %s"%(options.JECUnc)
if (options.globalTag =="Default") :
    options.globalTag = "80X_mcRun2_asymptotic_2016_TrancheIV_v8"

if (options.JECUnc =="Default") :
    options.JECUnc = "Summer16_23Sep2016V4_MC"
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
process.load("CMSAnalyses.SSBAnalyzer.metFilters_cff")

process.load('RecoMET.METFilters.badGlobalMuonTaggersMiniAOD_cff')
process.badGlobalMuonTagger = cms.EDFilter("BadGlobalMuonTagger",
                                           muons = cms.InputTag("slimmedMuons"),
                                           vtx   = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                           muonPtCut = cms.double(20),
                                           selectClones = cms.bool(False),
                                           verbose = cms.untracked.bool(False),
                                           taggingMode = cms.bool(True)
                                           )

process.cloneGlobalMuonTagger = process.badGlobalMuonTagger.clone(
  selectClones = True
  )
process.BadGlobalMuonFilter = cms.Sequence(process.cloneGlobalMuonTagger + process.badGlobalMuonTagger)
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
   #input = cms.untracked.int32(100)
   input = cms.untracked.int32(options.maxEvents) # Default is -1
)
########################
### Output filenames ###
########################
process.TFileService=cms.Service("TFileService",
        #fileName=cms.string("SSBTree.root"),
        fileName=cms.string(options.outputFile),
        closeFileFast = cms.untracked.bool(True)
)
#configurable options =======================================================================
runOnData=options.isData #data/MC switch
isMC = not runOnData
usePrivateSQlite=True #use external JECs (sqlite file)
useHFCandidates=True #create an additionnal NoHF slimmed MET collection if the option is set to false
redoPuppi=True # rebuild puppiMET
isSys=False
metftbit = "PAT"
trigbit = "HLT"
#===================================================================

### =====================================================================================================
# Define the input source

corList = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute'])
    
if runOnData:
  fname = 'file:/d3/scratch/sha/Analyses/DATA/MiniAOD/Run2016/PromptBv2/02D9C19F-571A-E611-AD8E-02163E013732.root'
  corList = cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])
  metftbit ="RECO"
  trigbit = "HLT"
  print ("Running on Data ...")

else:
  fname = 'file:/pnfs/knu.ac.kr/data/cms/store/user/sha/MiniAODSample/MC/80X_v2/ForMoriond/DYJetsToLL_M_50_HCALDebug/00312D7A-FEBD-E611-A713-002590DB923E.root'
if (not(options.inputFiles)):
    #print "options.inputFiles ? %s " %(options.inputFiles)
    options.inputFiles = fname
    print  "options.inputFiles ? %s " %(options.inputFiles)
# Define the input source
process.source = cms.Source("PoolSource", 
    #fileNames = cms.untracked.vstring([ fname ])
    fileNames = cms.untracked.vstring(options.inputFiles)
)


if options.certFile and options.certFile != "None" :
    import FWCore.PythonUtilities.LumiList as LumiList
    process.source.lumisToProcess = LumiList.LumiList(filename = options.certFile).getVLuminosityBlockRange()

### External JECs =====================================================================================================

Eras= options.JECUnc
era= options.JECUnc
jecUncertainty="CMSAnalyses/SSBAnalyzer/data/JECDir/Summer16_23Sep2016/"+Eras+"/"+Eras+"_Uncertainty_AK4PFchs.txt"
print " jecUncertainty : %s"%(jecUncertainty)
#from Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff import *
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = options.globalTag

if usePrivateSQlite:
    from CondCore.DBCommon.CondDBSetup_cfi import *
    import os

##___________________________External JEC file________________________________||

    process.jec = cms.ESSource("PoolDBESSource",CondDBSetup,
                               #connect = cms.string("sqlite_fip:PhysicsTools/PatUtils/data/"+era+".db"),
                               connect = cms.string("sqlite_fip:CMSAnalyses/SSBAnalyzer/data/JECDir/Summer16_23Sep2016/"+Eras + "/"+Eras+".db"),
                               toGet =  cms.VPSet(
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PF"),
                label= cms.untracked.string("AK4PF")
                ),
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PFchs"),
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
)
process.JEC = cms.Sequence(process.patJetCorrFactorsUpdatedJEC * process.updatedPatJetsUpdatedJEC)



### ---------------------------------------------------------------------------
### Removing the HF from the MET computation
### ---------------------------------------------------------------------------

from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

#default configuration for miniAOD reprocessing, change the isData flag to run on data
#for a full met computation, remove the pfCandColl input
runMetCorAndUncFromMiniAOD(process,
                           isData=runOnData,
                           )

if redoPuppi:
  from PhysicsTools.PatAlgos.slimming.puppiForMET_cff import makePuppiesFromMiniAOD
  makePuppiesFromMiniAOD( process );

  runMetCorAndUncFromMiniAOD(process,
                             isData=runOnData,
                             pfCandColl=cms.InputTag("puppiForMET"),
                             recoMetFromPFCs=True,
                             reclusterJets=True,
                             jetFlavor="AK4PFPuppi",
                             postfix="Puppi"
                             )


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
                 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff',
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
#####################################
### Electron & Photon-- smearing ####
#####################################

process.load('Configuration.StandardSequences.Services_cff')
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                       calibratedPatElectrons  = cms.PSet( initialSeed = cms.untracked.uint32(81),
                                                                                                                 engineName = cms.untracked.string('TRandom3'),
                                                                                           ),
                                                       calibratedPatPhotons  = cms.PSet( initialSeed = cms.untracked.uint32(81),
                                                                                                                 engineName = cms.untracked.string('TRandom3'),
                                                                                           ),
                                                       )

#process.load('EgammaAnalysis.ElectronTools.calibratedElectronsRun2_cfi')
#process.load('EgammaAnalysis.ElectronTools.calibratedPhotonsRun2_cfi')
#correctionType = "80Xapproval"
process.calibratedPatElectrons  = cms.EDProducer("CalibratedPatElectronProducerRun2",
#calibratedPatElectrons = cms.EDProducer("CalibratedPatElectronProducerRun2",
                                        
                                        # input collections
                                        electrons = cms.InputTag('slimmedElectrons'),
                                        gbrForestName = cms.string("gedelectron_p4combination_25ns"),
                                        
                                        # data or MC corrections
                                        # if isMC is false, data corrections are applied
                                        isMC = cms.bool(isMC),
                                        
                                        # set to True to get special "fake" smearing for synchronization. Use JUST in case of synchronization
                                        isSynchronization = cms.bool(False),
                                        correctionFile = cms.string('EgammaAnalysis/ElectronTools/data/ScalesSmearings/Winter_2016_reReco_v1_ele'),
                                        )


process.calibratedPatPhotons = cms.EDProducer("CalibratedPatPhotonProducerRun2",
#calibratedPatPhotons = cms.EDProducer("CalibratedPatPhotonProducerRun2",
    correctionFile = cms.string('EgammaAnalysis/ElectronTools/data/ScalesSmearings/Winter_2016_reReco_v1_ele'),
    photons = cms.InputTag("slimmedPhotons"),
    isMC = cms.bool(isMC),
    isSynchronization = cms.bool(False)
)



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



####################
### SSB Analyzer ###
####################
process.ssbanalyzer = cms.EDAnalyzer('SSBAnalyzer',
                                    BadChargedCandidateFilter  = cms.InputTag("BadChargedCandidateFilter",""),
                                    BadPFMuonFilter  = cms.InputTag("BadPFMuonFilter",""),
                                    badGlobalMuonTagger  = cms.InputTag("badGlobalMuonTagger",""),
                                    cloneGlobalMuonTagger  = cms.InputTag("cloneGlobalMuonTagger",""),
                                    bitsPat                    = cms.InputTag("TriggerResults","",metftbit), ## for MET Filter
                                    isMCTag         = cms.bool(isMC),
                                    PDFInfoTag      = cms.InputTag("generator",""),
                                    FixPOWHEG = cms.untracked.string("NNPDF30_nlo_as_0118.LHgrid"),
                                    #FixPOWHEG = cms.untracked.string("CT10nnlo.LHgrid"),
                                    #PDFSetNames     = cms.vstring("CT10.LHgrid"),#"cteq66.LHgrid"), #'MRST2006nnlo.LHgrid'),# 'NNPDF10_100.LHgrid'), 
                                    #PDFSetNames     = cms.vstring('CT10nnlo.LHgrid','NNPDF30_nlo_as_0118'),#"cteq66.LHgrid"), #'MRST2006nnlo.LHgrid'),# 'NNPDF10_100.LHgrid'), 
                                    PDFSetNames     = cms.vstring('NNPDF30_nlo_as_0118.LHgrid','CT10nnlo.LHgrid'),#"cteq66.LHgrid"), #'MRST2006nnlo.LHgrid'),# 'NNPDF10_100.LHgrid'), 
                                    #PDFSetNames     = cms.vstring('CT10nnlo.LHgrid','NNPDF30_nlo_as_0118.LHgrid'),#"cteq66.LHgrid"), #'MRST2006nnlo.LHgrid'),# 'NNPDF10_100.LHgrid'), 
#                                    PDFSetNames     = cms.vstring('NNPDF30_nlo_as_0118.LHgrid'),#"cteq66.LHgrid"), #'MRST2006nnlo.LHgrid'),# 'NNPDF10_100.LHgrid'), 
                                    PDFCent         = cms.bool(True),
                                    PDFSys          = cms.bool(True),
                                    pvTag           = cms.InputTag("offlineSlimmedPrimaryVertices",""),
                                    genEvnTag = cms.InputTag("generator"),
                                    genParTag       = cms.InputTag("prunedGenParticles"),
                                    genJetTag       = cms.InputTag("slimmedGenJets",""),
                                    genJetReclusTag = cms.InputTag("ak4GenJetsCustom",""),
                                    genBHadPlusMothersTag   = cms.InputTag("matchGenBHadron","genBHadPlusMothers"),
                                    genBHadIndexTag   = cms.InputTag("matchGenBHadron","genBHadIndex"),
                                    genBHadFlavourTag   = cms.InputTag("matchGenBHadron","genBHadFlavour"),
                                    genBHadFromTopWeakDecayTag   = cms.InputTag("matchGenBHadron","genBHadFromTopWeakDecay"),
                                    genBHadJetIndexTag   = cms.InputTag("matchGenBHadron","genBHadJetIndex"),

                                    genMETTag = cms.InputTag("slimmedMETs","","SSB"),
                                    isSignal        = cms.bool(options.isSig),
                                    #RhoTag          = cms.InputTag("fixedGridRhoAll"),
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
                                    bits             = cms.InputTag("TriggerResults","",trigbit),
                                    prescales        = cms.InputTag("patTrigger"),

                                    ismuSysTag       = cms.bool(isSys), # For Systemtic Study ...

                                    muTag            = cms.InputTag("slimmedMuons",""),
                                    muEnUpTag        = cms.InputTag("shiftedPatMuonEnUp",""),
                                    muEnDownTag      = cms.InputTag("shiftedPatMuonEnDown",""),
                                    eleTag           = cms.InputTag("slimmedElectrons",""),
                                    electronPATInput = cms.InputTag("calibratedPatElectrons",""),
                                    #eleTag          = cms.InputTag("shiftedPatElectronEnDown","","SSB"),
                                    eleEnUpTag       = cms.InputTag("shiftedPatElectronEnUp",""),
                                    eleEnDownTag     = cms.InputTag("shiftedPatElectronEnDown",""),

                                    effAreasConfigFile = cms.FileInPath("RecoEgamma/ElectronIdentification/data/Summer16/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_80X.txt"),

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
#                                    jtTag = cms.InputTag("slimmedJets",""), #For Jet
#                                    jtTag = cms.InputTag("patJetsReapplyJEC",""), #For Jet
                                    
                                    isjtcutTag = cms.bool(False), #For Jet
                                    jtTag = cms.InputTag("updatedPatJetsUpdatedJEC",""), #For Jet
                                    jtpuppiTag = cms.InputTag("slimmedJets",""), #For Jet
                                    #jtuncTag = cms.string(jecUncertainty),
                                    jtuncTag = cms.FileInPath(jecUncertainty),
                                    PayLoadName = cms.string('AK4PFchs'),
                                    phiResolMCFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_MC_PhiResolution_AK4PFchs.txt'),
                                    phiResolDataFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_DATA_PhiResolution_AK4PFchs.txt'),
                                    ptResolMCFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_DATA_PtResolution_AK4PFchs.txt'),
                                    ptResolSFFile = cms.FileInPath('CMSAnalyses/SSBAnalyzer/data/Spring16_25nsV6/Spring16_25nsV6_MC_SF_AK4PFchs.txt'),
#                                    phiResolMCFile = cms.string('Spring16_25nsV6_MC_PhiResolution_AK4PFchs.txt'),
#                                    phiResolDataFile = cms.string('Spring16_25nsV6_DATA_PhiResolution_AK4PFchs.txt'),
#                                    ptResolSFFile = cms.string('Spring16_25nsV6_MC_SF_AK4PFchs.txt'),
#                                    csvbjetTag = cms.string("combinedInclusiveSecondaryVertexV2BJetTags"),
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
                                    #metnoHFTag = cms.InputTag("slimmedMETsNoHF",""),
                                    metnoHFTag = cms.InputTag("slimmedMETs",""),
)
process.p = cms.Path(
        process.fullPatMetSequence*
        process.JEC*
        process.calibratedPatElectrons*
        process.calibratedPatPhotons*
  	(process.egmPhotonIDSequence + process.egmGsfElectronIDSequence ) *
        process.BadGlobalMuonFilter *
#        process.noBadGlobalMuons*
        process.ssbanalyzer
)
