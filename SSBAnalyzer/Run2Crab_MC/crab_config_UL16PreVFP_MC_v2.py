### For MC SinglePion ####
from CRABClient.UserUtilities import config 
config = config()


config.section_('General')
config.General.transferOutputs = True
config.section_('JobType')


config.General.workArea = 'UL2016_PreVFP'
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/ssbanalyzer_2016APV_cfg.py'
config.JobType.allowUndistributedCMSSW = True
config.JobType.outputFiles = ['SSBTree.root']

config.Data.inputDBS = 'global'

config.section_('Data')

config.Data.publication = False
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 25000
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T3_KR_KNU'
config.Data.outLFNDirBase = '/store/user/sha/Run2FULL/2016PreVFP/MC/'




if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    #config.General.requestName = 'TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8'
    #config.Data.inputDataset = '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    #crabCommand('submit', config = config)

    config.General.requestName = 'TTToHadronic_TuneCP5_13TeV-powheg-pythia8'
    config.Data.inputDataset = '/TTToHadronic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8'
    config.Data.inputDataset = '/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8'
    config.Data.inputDataset = '/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8'
    config.Data.inputDataset = '/TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8'
    config.Data.inputDataset = '/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    crabCommand('submit', config = config)
    
    config.General.requestName = 'TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8'
    config.Data.inputDataset = '/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8'
    config.Data.inputDataset = '/TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM'
    crabCommand('submit', config = config)
