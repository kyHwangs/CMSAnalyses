#from CRABClient.UserUtilities import config, getUsernameFromSiteDB
from CRABClient.UserUtilities import config
config = config()
config.section_('General')
config.General.transferOutputs = True
config.section_('JobType')

config.JobType.psetName = '../../python/ssbanalyzer_bg_cfg.py'
config.JobType.pluginName = 'Analysis'
#config.JobType.inputFiles = ['../../Spring16_25nsV6_MC.db', '../../Spring16_25nsV6_MC_Uncertainty_AK4PFchs.txt', '../../Data_2016_Reco_Pileup.root', '../../MC_Common_Pileup.root']

config.JobType.outputFiles = ['SSBTree.root']
config.JobType.maxMemoryMB = 5000
config.JobType.allowUndistributedCMSSW = True

config.section_('Data')

config.Data.publication = False
#config.Data.inputDataset = '/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3_ext3-v1/MINIAODSIM'
#config.Data.inputDataset = '/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv1-PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/MINIAODSIM'
config.Data.inputDataset = '/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 25000

config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_KR_KNU'
#config.Data.outLFNDirBase = '/store/user/%s/Run2016/MC/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX_80X_V2_forMoriond' % (getUsernameFromSiteDB())
config.Data.outLFNDirBase = '/store/user/sha/Run2016/MC/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX_80X_V2_forMoriond'
