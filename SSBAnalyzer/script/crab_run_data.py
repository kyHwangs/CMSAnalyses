#!/usr/bin/env python3

"""
This is a small script that does the equivalent of multicrab.
"""

import os
import sys
from optparse import OptionParser
import json
import copy
from multiprocessing import Process

from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
import http.client as httplib


def merge_dicts(*dict_args):
    """
    Given any number of dicts, shallow copy and merge into a new dict,
    precedence goes to key value pairs in latter dicts.
    """
    result = {}
    for dictionary in dict_args:
        result.update(dictionary)
    return result

def getOptions():
    """
    Parse and return the arguments provided by the user.
    """
    usage = ("Usage: %prog --crabCmd CMD [--workArea WAD --crabCmdOpts OPTS --era ERA --sample SAMPLE --suffix SUFFIX]"
             "\nThe multicrab command executes 'crab CMD OPTS' for each project directory contained in WAD"
             "\nUse multicrab -h for help")

    parser = OptionParser(usage=usage)

    parser.add_option('-c', '--crabCmd',
                      dest = 'crabCmd',
                      default = '',
                      help = "crab command",
                      metavar = 'CMD')

    parser.add_option('-w', '--workArea',
                      dest = 'workArea',
                      default = 'crab',
                      help = "work area directory. Default: 'crab'.",
                      metavar = 'WAD')

    parser.add_option('-o', '--crabCmdOpts',
                      dest = 'crabCmdOpts',
                      default = '',
                      help = "options for crab command CMD",
                      metavar = 'OPTS')

    parser.add_option('-e', '--era',
                      dest = 'era',
                      default = 'UL2018',
                      help = "Era to run samples over. Options are 'UL2018'/'UL2017'/'UL2016'/'UL2016APV'.",
                      metavar = 'ERA')

    parser.add_option('-s', '--sample',
                      dest = 'sample',
                      default = 'LO_inc',
                      help = "samples to process: 'LO_inc' (default), custom (e.g. 'NLO_inc', 'NLO_10to50').",
                      metavar = 'SAMPLE')

    parser.add_option('-x', '--suffix',
                      dest = 'suffix',
                      default = '',
                      help = "addon suffix",
                      metavar = 'SUFFIX')

    parser.add_option('-f', '--configFile',
                      dest = 'configFile',
                      default = os.path.join(os.environ['CMSSW_BASE'], 'src/CMSAnalyses/SSBAnalyzer/config/ssbanalyzer_data_cfg.py'),
                      help = "CMSSW cfg file to use (default 'ssbanalyzer_data_cfg.py' in /config).",
                      metavar = 'CFG')

    parser.add_option('--splittingData',
                      dest = 'splittingData',
                      default = 'LumiBased',
                      help = "job splitting option for data",
                      metavar = 'SPLITTING_DATA')

    parser.add_option('--unitsPerJobData',
                      dest = 'unitsPerJobData',
                      type = 'int',
                      default = 10,
                      help = "unitsPerJob option for data",
                      metavar = 'UNIT_PER_JOB_DATA')

    # parser.add_option('--splittingMC',
    #                   dest = 'splittingMC',
    #                   default = 'LumiBased',
    #                   help = "job splitting option for MC",
    #                   metavar = 'SPLITTING_MC')

    # parser.add_option('--unitsPerJobMC',
    #                   dest = 'unitsPerJobMC',
    #                   type = 'int',
    #                   default = 25000,
    #                   help = "unitsPerJob option for MC",
    #                   metavar = 'UNIT_PER_JOB_MC')

    parser.add_option('--dryrun',
                      dest = 'dryrun',
                      action = 'store_true',
                      default = False,
                      help = "print out CRAB configuration instead of submitting it")

    (options, arguments) = parser.parse_args()

    if arguments:
        parser.error("Found positional argument(s): %s." % (arguments))
    if not options.crabCmd:
        parser.error("(-c CMD, --crabCmd=CMD) option not provided.")
    if options.crabCmd != 'submit':
        if not os.path.isdir(options.workArea):
            parser.error("'%s' is not a valid directory." % (options.workArea))

    return options


def main():

    # Read options
    options = getOptions()

    era = options.era
    sample = options.sample

    suffix = options.suffix

    crabCmd = options.crabCmd
    crabCmdOpts = options.crabCmdOpts
    workArea = options.workArea
    configFile = options.configFile

    # The submit command needs special treatment.
    if crabCmd == 'submit':

        #--------------------------------------------------------
        # This is the base config:
        #--------------------------------------------------------
        from WMCore.Configuration import Configuration

        config = Configuration()

        config.section_('General')
        config.General.workArea = workArea
        config.General.transferOutputs = True
        config.General.transferLogs = True

        config.section_('JobType')
        config.JobType.pluginName = 'Analysis'
        config.JobType.psetName = configFile
        config.JobType.allowUndistributedCMSSW = True
        config.JobType.maxMemoryMB = 2500
        config.JobType.outputFiles = ['SSBTree.root']

        config.section_('Data')
        config.Data.publication = False
        config.Data.allowNonValidInputDataset = True # for validation samples
        config.Data.inputDBS = 'global'
        config.Data.outLFNDirBase = '/store/user/khwang/CMS/SSB/%s/%s/%s/' % (suffix, era, sample)

        config.section_('Site')
        config.Site.storageSite = 'T3_KR_KNU'

        #--------------------------------------------------------

        LM_prefix = 'https://cms-service-dqmdc.web.cern.ch/CAF/certification/Collisions'
        LM_perEra = ''

        if era == 'UL2016APV' :
            LM_perEra = LM_prefix + '16/13TeV/Legacy_2016/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt'
        elif era == 'UL2016' :
            LM_perEra = LM_prefix + '16/13TeV/Legacy_2016/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt'
        elif era == 'UL2017' :
            LM_perEra = LM_prefix + '17/13TeV/Legacy_2017/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt'
        # elif era == 'UL2018' :
        #     LM_perEra = LM_prefix + '18/13TeV/Legacy_2018/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt'

        elif era == 'UL2018' :
            LM_perEra = '/u/user/kyhwang/WorkingDir/CMS/HighMassDY/ntupler/240306/CMSSW_10_6_30/src/CMSAnalyses/SSBAnalyzer/script/240527/UL2018/Run2018A/crab_SSB_nTuplizer_UL2018_Run2018A_240527/results/notFinishedLumis.json'

        config.Data.lumiMask = LM_perEra

        sample_db = os.path.join(os.environ['CMSSW_BASE'], "src/CMSAnalyses/SSBAnalyzer/dataset", "database_data.json")

        input_dataset = ''
        globalTag = ''
        output_dir = ''

        with open(sample_db, 'r') as db_file:

            db = json.load(db_file)

            try:
                input_dataset = db[era][sample]["dataset"]
                globalTag = db[era][sample]["globalTag"]
                output_dir = db[era][sample]["output"]

            except:
                print ("Error!! Requested era and sample are likely not valid. Please check argument.")
                sys.exit()

        config.Data.unitsPerJob = options.unitsPerJobData
        config.Data.splitting = options.splittingData

        config.JobType.pyCfgParams = [
                'era={}'.format(era),
                'gt={}'.format(globalTag)
                ]

        config.Data.inputDataset = input_dataset
        config.General.requestName = '_'.join(['SSB_nTuplizer', era, sample, suffix])

        print (configFile)
        print (era)
        print (sample)
        print (input_dataset)
        print (output_dir)
        print (globalTag)
        print (LM_perEra)
        print ("")

        # return

        # If we need to pull input files from a list file instead of CRAB:
        # config.Data.userInputFiles = open(basedir + sample + '.list').readlines()

        # Submit.
        def submit(config, options):
            try:
                print ("Submitting for input dataset %s with options %s" % (input_dataset, options.crabCmdOpts))
                if options.dryrun:
                    print ('-'*50)
                    print (config)
                else:
                    crabCommand(options.crabCmd, config = config, *options.crabCmdOpts.split())
            except HTTPException as hte:
                print ("Submission for input dataset %s failed: %s" % (input_dataset, hte.headers))
            except ClientException as cle:
                print ("Submission for input dataset %s failed: %s" % (input_dataset, cle))

        # Need to submit using multiprocessing module because of CRAB issue with different configs
        p = Process(target=submit, args=(config,options,))
        p.start()
        p.join()

    # All other commands can be simply executed.
    elif workArea:

        for dir in os.listdir(workArea):
            projDir = os.path.join(workArea, dir)
            if not os.path.isdir(projDir):
                continue
            # Execute the crab command.
            msg = "Executing (the equivalent of): crab %s --dir %s %s" % (crabCmd, projDir, crabCmdOpts)
            print ("-"*len(msg))
            print (msg)
            print ("-"*len(msg))
            try:
                crabCommand(crabCmd, dir = projDir, *crabCmdOpts.split())
            except HTTPException as hte:
                print ("Failed executing command %s for task %s: %s" % (crabCmd, projDir, hte.headers))
            except ClientException as cle:
                print ("Failed executing command %s for task %s: %s" % (crabCmd, projDir, cle))

if __name__ == '__main__':
    main()
