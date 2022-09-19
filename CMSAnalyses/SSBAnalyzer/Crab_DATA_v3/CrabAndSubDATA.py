import os
import sys
import subprocess

def Main():
    print "test Test"
    Cur_Dir = os.getcwd()
    Data_Channel = ["DoubleMuon","DoubleEG","MuonEG","SingleMuon","SingleElectron"]
    #Data_Channel = ["DoubleEG","MuonEG","SingleMuon","SingleElectron"]
    #Data_Channel = ["DoubleMuon","DoubleEG","MuonEG"]
    #Data_Channel = ["DoubleMuon"]
    #Data_Channel = ["DoubleEG"]
    #Data_Channel = ["MuonEG"]
    Data_RunRange = ["Run2016B","Run2016C","Run2016D","Run2016E","Run2016F","Run2016G","Run2016HV2","Run2016HV3"]
    TestArray(Data_Channel)
    JSON = "./Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt"
    Crab_DATA(Data_Channel,Data_RunRange,JSON,Cur_Dir) # Crab DATA 
    os.chdir(Cur_Dir)
    pass # End Of Main Function 

def WriteConfigData(fileName,SampleSet,SaveSEDir,cmsswconfig):
    #print fileName
    f=open(fileName, 'w')
    f.write("from CRABClient.UserUtilities import config, getUsernameFromSiteDB \n")
    f.write("config = config() \n")
    f.write("config.section_('General')\n")
    f.write("config.General.transferOutputs = True\n")
    f.write("config.section_('JobType') \n")
    f.write("\n")
    f.write("config.JobType.psetName = '../../python/%s' \n"%(cmsswconfig))
    f.write("config.JobType.pluginName = 'Analysis' \n")
    f.write("\n")
    f.write("config.JobType.outputFiles = ['SSBTree.root'] \n")
    f.write("config.JobType.maxMemoryMB = 5000 \n")
    f.write("config.JobType.allowUndistributedCMSSW = True\n")
    f.write("config.section_('Data') \n")
    f.write("\n")
    f.write("config.Data.publication = False\n")
    f.write("config.Data.inputDataset = '%s'\n"%(SampleSet))
    f.write("config.Data.unitsPerJob = 10 \n")
    f.write("config.Data.splitting = 'LumiBased'\n")
    f.write("config.Data.lumiMask = 'Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'\n")
    f.write("\n")
    f.write("config.section_('User')\n")
    f.write("config.section_('Site') \n")
    f.write("config.Site.storageSite = 'T2_KR_KNU'\n")
    #setoutdir = "config.Data.outLFNDirBase = '/store/user/%s/" + SaveSEDir + "'" + " % (getUsernameFromSiteDB())"
    setoutdir = "config.Data.outLFNDirBase = '/store/user/sha/" + SaveSEDir + "'"
    f.write(setoutdir+"\n")
    f.close()
def TestArray (Array):
    for iarray in Array:
        print iarray
        pass
    pass
def Crab_DATA (data_channel,data_runrange,Json_,cur_dir):
    for isam in data_channel : # for data Loop
        #print isam
        for irun in data_runrange : # Data Run Range #
            data_dir = "%s_%s" %(isam,irun)
            #print data_dir
            mk_data_dir = "mkdir -p %s" %(data_dir)
            #print mk_data_dir
            os.system(mk_data_dir)
      
            os.chdir("%s/%s"%(cur_dir,data_dir))
            copy_json = "cp ../%s ."%(Json_)
            os.system(copy_json)
            #### Make Crab Config ####
            crabcon = "./crab_cfg.py"
            version = "-v1"
            run_period = irun
            if irun == "Run2016B" : 
                version = "_ver2-v2"
            if irun == "Run2016HV2" :
                version = "_ver2-v1"
                run_period = "Run2016H" 
            if irun == "Run2016HV3" :
                version = "_ver3-v1"
                run_period = "Run2016H" 
            Duration = "03Feb2017"
            RunName = run_period + "-%s"%(Duration)
            #print "RunName ? %s"%(RunName)
            Tier ="MINIAOD"
            DBS_Set = "/%s/%s%s/%s"%(isam,RunName,version,Tier)
            print "DBS_Set ? %s"%(DBS_Set)
            SE_Dir = "Run2016/DATA/%s/" %(data_dir)
            cmsconfig = "ssbanalyzer_data_BCDEF_cfg.py"
            if irun == "Run2016G" or irun == "Run2016HV2" or irun == "Run2016HV3" :
               cmsconfig = "ssbanalyzer_data_GH_cfg.py"
            WriteConfigData(crabcon,DBS_Set,SE_Dir,cmsconfig)
            crabsubmit = "crab submit -c %s" %(crabcon)
            os.system(crabsubmit)
            print "crabsubmit ? %s "%(crabsubmit)
            os.chdir(cur_dir) ## Move To CurDir
            pass ## End of data_runrange 
        pass ## End of data loop 

Main()
