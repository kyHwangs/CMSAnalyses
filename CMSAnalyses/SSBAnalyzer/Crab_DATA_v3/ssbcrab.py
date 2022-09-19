import os
import sys
import subprocess
import time

def Main():
    print "test Test"
    Cur_Dir = os.getcwd()
    #Data_Channel = ["DoubleMuon","DoubleEG","MuonEG"]
    #Data_Channel = ["DoubleMuon","DoubleEG","MuonEG","SingleMuon","SingleElectron"]
    Data_Channel = ["DoubleEG","MuonEG","SingleMuon","SingleElectron"]
    Data_RunRange = ["Run2016B","Run2016C","Run2016D","Run2016E","Run2016F","Run2016G","Run2016HV2","Run2016HV3"]
    MCSamples = ["TTbar_Powheg","DYJetsToLL_M_10To50","DYJetsToLL_M_50","WJetsToLNu","WW","WZ","ZZ","ST_tW_top","ST_tW_antitop"]
    now = time.localtime()
    Samples = []
    for ich in Data_Channel :
        for irun in Data_RunRange :
            Samples.append("%s_%s"%(ich,irun))
            pass
        pass
    for insamp in Samples:
        disc_= "%s_%s"%("crab",now.tm_year) 
        crabdir = findcrabdir(insamp,disc_)
        if crabdir == "" : print "There is no Crab Dir %s"%(insamp)
        else :
            logf =   "log_%s.txt"%(insamp)
            crabcmd = "crab status -d %s > %s"%(crabdir,logf)
            print "%s"%(crabcmd)
            os.system(crabcmd)
            stat = iscrabcomple(logf)
             
            resubmitcrab(stat,crabdir) 
    pass # End Of Main Function

#os.path.isfile
def findcrabdir(dirname,discr):
    filenames = os.listdir(dirname)
    crabdir = ""
    for filename in filenames:
        full_filename = os.path.join(dirname, filename)
        if not os.path.isdir(full_filename) : continue
        if full_filename.count(discr) :
            crabdir = full_filename
    return crabdir
    pass

def iscrabcomple(logfile):
    print "logfile %s" %(logfile)
    f = open(logfile, 'r')
    lines = f.readlines()
    isfinish = False 
    isfailed = False
    isRun    = False
    for line in lines :
        #print line
        if line.count("COMPLETED") >0 :
            isfinish = True
            print "isfinish %s "%(isfinish) 
        if line.count("failed") : 
            isfailed = True
            pass
        if line.count("running") or line.count("cool") or line.count("unsubmitted"): 
            isRun = True
            pass
    f.close()
    Status = []
    Status.append(isfinish) 
    Status.append(isfailed) 
    Status.append(isRun)
    return Status
    #resubmitcrab(Status,"Test") 
    pass
def resubmitcrab(statusArray,crabdir):
    if statusArray[0] == True: print "FINISH CRAB!! %s"%(crabdir)
    elif statusArray[1] == True: 
        print "Resubmit !! %s"%(crabdir)
        cmd = "crab resubmit -d %s"%(crabdir)
        os.system(cmd)
    elif statusArray[2] == True:
        print "Still Runing "
    else : print "crab error ???"

Main()
#iscrabcomple("log_TTbar_Powheg.txt")
