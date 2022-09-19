import os
import sys
import subprocess
#subprocess.check_output(['ls', '-l','SSBTree_*'])
x = 'SSBTree_'
print x
Num_root = "l*.root | wc -l"

for i in range (1,110) :
    testcmd = "ls SSBTree* | grep SSBTree_%s_ | wc -l" % (i)
    result = subprocess.check_output (testcmd , shell=True)
    if result.find('1') <= -1 :
        print 'Fail = SSBTree_%s' % (i)
    #    print 'Success'
print 'End'
cmd = 'ls SSBTree_* | grep "SSBTree_" | wc  -l'
#os.system(cmd)
os.system (cmd)


#result = subprocess.check_output ('./program' , shell=True)
#result = subprocess.check_output (cmd , shell=True)
#print 'test - '+str(type(result))
#subprocess.call (cmd, shell=True)
#print result
#else :
#    print 'Success'
