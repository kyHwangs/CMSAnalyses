#from CRABClient.UserUtilities import config, getUsernameFromSiteDB 
from CRABClient.UserUtilities import config
config = config() 
config.section_('General')
config.General.transferOutputs = True
config.section_('JobType') 

#config.JobType.psetName = '../../python/ssbanalyzer_data_BCDEF_cfg.py' 
config.JobType.psetName = '../python/ssbanalyzer_2016APV_data_cfg.py'
config.JobType.pluginName = 'Analysis' 

config.JobType.outputFiles = ['SSBTree.root'] 
config.JobType.maxMemoryMB = 5000 
config.JobType.allowUndistributedCMSSW = True
config.section_('Data') 

config.Data.publication = False
config.Data.inputDataset = '/DoubleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD'
config.Data.unitsPerJob = 10 
config.Data.splitting = 'LumiBased'
#config.Data.lumiMask = 'Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
config.Data.lumiMask = '../JSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt'
config.Data.outLFNDirBase = 'store/user/sha/Run2FULL/2016PreVFP/DATA'

config.section_('User')
config.section_('Site') 
config.Site.storageSite = 'T3_KR_KNU'
#config.Data.outLFNDirBase = '/store/user/sha/Run2016/DATA/DoubleMuon_Run2016B/'

config.General.requestName = 'DoubleMuon_Run2016B-ver2'
config.Data.inputDataset = '/DoubleMuon/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v1/MINIAOD'
