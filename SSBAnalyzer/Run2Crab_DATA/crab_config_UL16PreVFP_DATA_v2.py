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
config.Data.unitsPerJob = 20
config.Data.lumiMask = '../JSON/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt'


config.Site.storageSite = 'T3_KR_KNU'
config.Data.outLFNDirBase = '/store/user/sha/Run2FULL/2016PreVFP/DATA'
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # DoubleMuon
    #config.General.requestName = 'DoubleMuon_Run2016B-ver2'
    #config.Data.inputDataset = '/DoubleMuon/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)


    #config.General.requestName = 'DoubleMuon_Run2016C'
    #config.Data.inputDataset = '/DoubleMuon/Run2016C-HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'DoubleMuon_Run2016D'
    #config.Data.inputDataset = '/DoubleMuon/Run2016D-HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'DoubleMuon_Run2016E'
    #config.Data.inputDataset = '/DoubleMuon/Run2016E-HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'DoubleMuon_Run2016F_e'
    #config.Data.inputDataset = '/DoubleMuon/Run2016F-HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)


    # DoubleEG
    #config.General.requestName = 'DoubleEG_Run2016B-ver2'
    #config.Data.inputDataset = '/DoubleEG/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v1/MINIAOD' #WONG
    #config.Data.inputDataset = '/DoubleEG/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v3/MINIAOD'
    #crabCommand('submit', config = config)


    #config.General.requestName = 'DoubleEG_Run2016C'
    #config.Data.inputDataset = '/DoubleEG/Run2016C-HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'DoubleEG_Run2016D'
    #config.Data.inputDataset = '/DoubleEG/Run2016D-HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'DoubleEG_Run2016E'
    #config.Data.inputDataset = '/DoubleEG/Run2016E-HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'DoubleEG_Run2016F_e'
    #config.Data.inputDataset = '/DoubleEG/Run2016F-HIPM_UL2016_MiniAODv2-v1/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'MuonEG_Run2016B-ver2'
    #config.Data.inputDataset = '/MuonEG/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'MuonEG_Run2016C'
    #config.Data.inputDataset = '/MuonEG/Run2016C-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'MuonEG_Run2016D'
    #config.Data.inputDataset = '/MuonEG/Run2016D-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'MuonEG_Run2016E'
    #config.Data.inputDataset = '/MuonEG/Run2016E-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'MuonEG_Run2016F_e'
    #config.Data.inputDataset = '/MuonEG/Run2016F-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'SingleMuon_Run2016B-ver2'
    #config.Data.inputDataset = '/SingleMuon/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'SingleMuon_Run2016C'
    #config.Data.inputDataset = '/SingleMuon/Run2016C-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'SingleMuon_Run2016D'
    #config.Data.inputDataset = '/SingleMuon/Run2016D-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'SingleMuon_Run2016E'
    #config.Data.inputDataset = '/SingleMuon/Run2016D-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    #config.General.requestName = 'SingleMuon_Run2016F_e'
    #config.Data.inputDataset = '/SingleMuon/Run2016F-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    #crabCommand('submit', config = config)

    config.General.requestName = 'SingleElectron_Run2016B-ver2'
    config.Data.inputDataset = '/SingleElectron/Run2016B-ver2_HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    crabCommand('submit', config = config)

    config.General.requestName = 'SingleElectron_Run2016C'
    config.Data.inputDataset = '/SingleElectron/Run2016C-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    crabCommand('submit', config = config)

    config.General.requestName = 'SingleElectron_Run2016D'
    config.Data.inputDataset = '/SingleElectron/Run2016D-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    crabCommand('submit', config = config)

    config.General.requestName = 'SingleElectron_Run2016E'
    config.Data.inputDataset = '/SingleElectron/Run2016E-HIPM_UL2016_MiniAODv2-v5/MINIAOD'
    crabCommand('submit', config = config)

    config.General.requestName = 'SingleElectron_Run2016F_e'
    config.Data.inputDataset = '/SingleElectron/Run2016F-HIPM_UL2016_MiniAODv2-v2/MINIAOD'
    crabCommand('submit', config = config)
