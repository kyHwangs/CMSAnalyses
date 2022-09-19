import os
import sys
import subprocess
Dir = 'lcg-ls "srm://cluster142.knu.ac.kr:8443/srm/managerv2?SFN=/pnfs/knu.ac.kr/data/cms/store/user/sha/'
subDir = raw_input("where is SubDirectory ? : ")
ending = '" > list.txt'
listcmd = Dir+subDir+ending
print listcmd

#testcmd = "ls SSBTree* | grep SSBTree_%s_ | wc -l" % (i)
#result = subprocess.check_output (testcmd , shell=True)
os.system (listcmd)
os.system ('chmod 755 list.sh')
#listcmd
