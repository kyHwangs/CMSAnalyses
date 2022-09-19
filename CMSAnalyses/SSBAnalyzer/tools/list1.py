import os
import sys
import subprocess
Dir = 'lcg-ls "srm://cluster142.knu.ac.kr:8443/srm/managerv2?SFN=/pnfs/knu.ac.kr/data/cms/store/user/sha/'
subDir = raw_input("where is SubDirectory ? : ")
listName = raw_input("what list name do you want ? : ")
ending = '" > ' + listName +".list"
listcmd = Dir+subDir+ending
print listcmd

#testcmd = "ls SSBTree* | grep SSBTree_%s_ | wc -l" % (i)
#result = subprocess.check_output (testcmd , shell=True)
os.system (listcmd)

def inplace_change(filename, old_string, new_string):
        s=open(filename).read()
        if old_string in s:
                print 'Changing "{old_string}" to "{new_string}"'.format(**locals())
                s=s.replace(old_string, new_string)
                f=open(filename, 'w')
                f.write(s)
                f.flush()
                f.close()
        else:
                print 'No occurances of "{old_string}" found.'.format(**locals())
inplace_change(listName +".list","/pnfs","dcap://cluster142.knu.ac.kr/")

def split_file(fileName,Num_Fils):
        f = open(fileName+".list", 'r')
        lines = f.readlines()
        print "Num_lines ? " ,len(lines)/Num_Fils +1 
        totalnumlines = len(lines)
        print "totalnumlines ? ", totalnumlines, " Num_Fils ? ", Num_Fils
        numlineM = totalnumlines/Num_Fils
        firstline = 0
        Maxline = len(lines)/Num_Fils +1 
        lastline = Maxline
        for i in range(0,Num_Fils):
            fnew=open(fileName + "_%s.list"%(i+1), 'w')
            sys.stdout.writelines(lines[firstline:lastline])
            fnew.writelines(lines[firstline:lastline])
            print "firstline ? ", firstline, "  lastline ? ", lastline
            firstline = lastline
            lastline  = Maxline*(i+2)
            print Num_Fils , " ----%s  " %(i)
NumFiles = input("How many list files do you want ? : ")
split_file(listName ,NumFiles)
#os.system ('chmod 755 list.sh')
#listcmd
