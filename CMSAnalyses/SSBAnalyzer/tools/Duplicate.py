import os
import sys
import subprocess
#subprocess.check_output(['ls', '-l','SSBTree_*'])
x = 'SSBTree_'
print x
numFiles = input("How many files ? : ")
Dir = 'lcg-ls "srm://cluster142.knu.ac.kr:8443/srm/managerv2?SFN=/pnfs/knu.ac.kr/data/cms/store/user/sha/'
subDir = raw_input("where is SubDirectory ? : ")
ending = '" > list.txt'
listcmd = Dir+subDir+ending
print listcmd
os.system(listcmd)
s=open("list.txt").read()
for x in range (1,numFiles):
    if s.count("SSBTree_%s_"%(x) ) != 1 :
        print "SSBTree_%s_"%(x) 
#      catch = "grep SSBTree_%s_ | wc -l" % (x)
#       os.system(catch)

print "test"
"""
for i in range (1,100) :
    testcmd = Dir + subDir +" \" | grep SSBTree_%s_ | wc -l" % (i)
    result = subprocess.check_output(testcmd , shell=True)
    if result.find('1') <= -1 :
        print 'Fail = SSBTree_%s' % (i)
"""
print 'End'
cmd = 'ls SSBTree_* | grep "SSBTree_" | wc  -l'
#os.system(cmd)
os.system (Dir+ subDir + " \"| grep SSBTree | wc -l")


#result = subprocess.check_output ('./program' , shell=True)
#result = subprocess.check_output (cmd , shell=True)
#print 'test - '+str(type(result))
#subprocess.call (cmd, shell=True)
#print result
#else :
#    print 'Success'
