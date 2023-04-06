import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

#from PhysicsTools.PatAlgos.tools.helpers import listModules, applyPostfix

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
#        'file:/d3/scratch/sha/Analyses/Develop/MiniAOD/CMSSW7410patch1/CMSSW_7_4_1_patch1/src/CMSAnalyses/SSBAnalyzer/python/00C90EFC-3074-E411-A845-002590DB9262.root'
        'file:/d3/scratch/sha/Analyses/DATA/SingleMuon_Run2/160C08A3-4227-E511-B829-02163E01259F.root'
    )
)

process.TFileService = cms.Service("TFileService",
      fileName = cms.string("SSBTree.root"),
)

process.ssbgeninfor = cms.EDAnalyzer('SSBGenInfor',
                                     genParTag = cms.InputTag("prunedGenParticles"),
                                     genJetTag = cms.InputTag("slimmedGenJets",""),
                                     genMETTag = cms.InputTag("slimmedMETs",""),
                                     isSignal        = cms.bool(False)
                                    )

process.ssbanalyzer = cms.EDAnalyzer('SSBAnalyzer',
                                    pvTag     = cms.InputTag("offlineSlimmedPrimaryVertices",""),
                                    genParTag = cms.InputTag("prunedGenParticles"),
                                    genJetTag = cms.InputTag("slimmedGenJets",""),
                                    genMETTag = cms.InputTag("slimmedMETs",""),
                                    isSignal  = cms.bool(True),
                                    RhoTag    = cms.InputTag("fixedGridRhoAll"),
                                    trigList  = cms.vstring(
                                                            'HLT_Mu17_Mu8_v',   ## Dimuon trigger 
                                                            'HLT_Mu17_TkMu8_v',
                                                            'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v',
                                                            'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v',
                                                            'HLT_DoubleMu33NoFiltersNoVtx_v',
#                                                            'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v', # For 8TeV
#                                                            'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v', # For 8TeV
                                                            'HLT_Mu23_TrkIsoVVL_Ele12_Gsf_CaloId_TrackId_Iso_MediumWP_v',
                                                            'HLT_Mu8_TrkIsoVVL_Ele23_Gsf_CaloId_TrackId_Iso_MediumWP_v',
#                                                            'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',# For 8TeV
                                                            'HLT_Ele23_Ele12_CaloId_TrackId_Iso_v',
#                                                            'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet50_40_30_v',
#                                                            'HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet50_40_30_v',
#                                                            'HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet50_40_30_v'
                                                        'HLT_IsoMu20_eta2p1_IterTrk02_',
                                                        'HLT_IsoMu24_eta2p1_IterTrk02_',
                                                        'HLT_Ele27_eta2p1_WP85_Gsf',
                                                        'HLT_Ele32_eta2p1_WP85_Gsf'

#                                                          'HLT_Mu17_Mu8_v',
#                                                          'HLT_Mu17_TkMu8_v',
#                                                          'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
#                                                          'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
#                                                          'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
#                                                          'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet50_40_30_v',
#                                                          'HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet50_40_30_v',
#                                                          'HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet50_40_30_v' ),
                                                          ),
                                    bits      = cms.InputTag("TriggerResults","","HLT"),
                                    prescales = cms.InputTag("patTrigger"),

                                    muTag = cms.InputTag("slimmedMuons",""),
                                    eleTag = cms.InputTag("slimmedElectrons",""),
                                    emvaIdNoTrTag = cms.InputTag("mvaNonTrigV0",""),
                                    emvaIdTrNoIPTag = cms.InputTag("mvaTrigNoIPV0",""),
                                    emvaIdTrTag = cms.InputTag("mvaTrigV0",""),
                                    pfCands = cms.InputTag("packedPFCandidates"),
                                    bstag = cms.InputTag("offlineBeamSpot"),
                                    convertag = cms.InputTag("reducedEgamma","reducedConversions"),
                                    jtTag = cms.InputTag("slimmedJets",""), #For Jet
                                    csvbjetTag = cms.string("combinedInclusiveSecondaryVertexV2BJetTags"),
                                    PFTightJetID = cms.PSet(
                                                       version = cms.string('FIRSTDATA'),
                                                       quality = cms.string('TIGHT')
                                                      ),
                                    PFLooseJetID = cms.PSet(
                                                       version = cms.string('FIRSTDATA'),
                                                       quality = cms.string('LOOSE')
                                                      ),
                                    metTag = cms.InputTag("slimmedMETs","")
)


process.p = cms.Path( process.ssbanalyzer )
#process.p = cms.Path( process.ssbgeninfor )
#process.p = cms.Path(process.egmGsfElectronIDSequence * process.idDemo * process.ssbanalyzer)
