import os
import sys
def main():
    dtGSet = [
               "dtG_0",
               "dtG_0p5207",
               "dtG_2p60364",
               "dtG_m0p5207",
               "dtG_m2p60364",
             ]
    NumLists = [
                 10,
                 10,
                 10,
                 10,
                 10 
               ]
    for index_, idtg in enumerate(dtGSet):
        numList = NumLists[index_]
        for inum in range(1,numList+1) :
            #cmd "cmsRun ssbanalyzer_cfg_dtg.py inputFiles_load=FileList/TTJets_Signal_dtG_0/TTJets_Signal_dtG_0_1.list outputFile=SSBTree_dtG_0_1.root"
            print "dtg : %s ,, inum %s"%(idtg,inum)
            cmd = "cmsRun ssbanalyzer_cfg_dtg.py inputFiles_load=FileList/DoubleMuon_%s/DoubleMuon_%s_%s.list outputFile=SSBTree_%s_%s.root"%(idtg,idtg,inum,idtg,inum)
            print cmd
            os.system(cmd)
            pass
        pass
    pass
main()
