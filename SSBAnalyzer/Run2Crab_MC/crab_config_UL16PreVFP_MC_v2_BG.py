### For MC SinglePion ####
from CRABClient.UserUtilities import config 
config = config()


config.section_('General')
config.General.transferOutputs = True
config.section_('JobType')


config.General.workArea = 'UL2016_PreVFP'
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/ssbanalyzer_2016APV_bg_cfg.py'
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

    config.General.requestName = 'DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8'
    config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8'
    config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'DYJetsToLL_M-10to50_TuneCP5_13TeV-amcatnloFXFX-pythia8'
    config.Data.inputDataset = '/DYJetsToLL_M-10to50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8'
    config.Data.inputDataset = '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8'
    config.Data.inputDataset = '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'WJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-pythia8'
    config.Data.inputDataset = '/WJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM'
    crabCommand('submit', config = config)
