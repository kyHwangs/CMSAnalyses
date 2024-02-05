from CRABClient.UserUtilities import config
config = config()

#config.General.transferLogs = True
config.General.workArea = 'UL2016_PreVFP'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../python/ssbanalyzer_2016APV_data_cfg.py'
config.JobType.allowUndistributedCMSSW = True
config.JobType.outputFiles = ["SSBTree.root"]

config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
#config.Data.unitsPerJob = 20
config.Data.unitsPerJob = 50
config.Data.lumiMask = '../JSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt'


config.Site.storageSite = 'T3_KR_KNU'
#config.Data.outLFNDirBase = '/store/user/sha/Run2FULL/2016PreVFP/MC/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8'
config.Data.outLFNDirBase = 'store/user/sha/Run2FULL/2016PreVFP/DATA'
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand


#    config.General.requestName = 'DoubleMuon_Run2016B'
#    config.Data.inputDataset = '/DoubleMuon/Run2016B-ver1_HIPM_UL2016_MiniAODv2-v1/MINIAOD'
#    crabCommand('submit', config = config)

    config.General.requestName = 'DoubleMuon_Run2016B-ver2'
    config.Data.inputDataset = '/DoubleMuon/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    crabCommand('submit', config = config)

    #config.General.requestName = 'Run2018Bv2-v13-2'
    #config.Data.inputDataset = '/ZeroBias/Run2018B-12Nov2019_UL2018-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'SingleCone_Run2018B_v2p3-1'
    #config.Data.inputDataset = '/ZeroBias/Run2018B-UL2018_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'Run2018Cv1-v13-2'
    #config.Data.inputDataset = '/ZeroBias/Run2018C-12Nov2019_UL2018_rsb-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'SingleCone_Run2018C_v2p3-1'
    #config.Data.inputDataset = '/ZeroBias/Run2018C-UL2018_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'Run2018Dv1-v13-2'
    #config.Data.inputDataset = '/ZeroBias/Run2018D-12Nov2019_UL2018_rsb-v1/MINIAOD'
    #crabCommand('submit', config = config)
    #
    #
    #config.General.requestName = 'SingleCone_Run2018D_v2p3'
    #config.General.requestName = 'SingleCone_Run2018D_v2p3-1'
    #config.Data.inputDataset = '/ZeroBias/Run2018D-UL2018_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)
