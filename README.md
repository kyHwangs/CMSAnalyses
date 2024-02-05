# SSBNtuple

CMSSW_10_6_30 

cd CMSSW_10_6_30/src/

cmsenv 

git cms-init 

Egamma : 

git cms-addpkg RecoEgamma/EgammaTools  

git clone https://github.com/cms-egamma/EgammaPostRecoTools.git

mv EgammaPostRecoTools/python/EgammaPostRecoTools.py RecoEgamma/EgammaTools/python/.

git clone -b ULSSfiles_correctScaleSysMC https://github.com/jainshilpi/EgammaAnalysis-ElectronTools.git EgammaAnalysis/ElectronTools/data/

git cms-addpkg EgammaAnalysis/ElectronTools

Muon : 

git clone https://gitlab.cern.ch/akhukhun/roccor.git RoccoR  (https://twiki.cern.ch/twiki/bin/view/CMS/RochcorMuon)

git clone -b UL_Ntuple_10_6_X_v1 https://github.com/physicist87/CMSAnalyses.git

scram b -j10

scram setup lhapdf

Runing configuration for 2016 PreVFP(APV) 

MC (ttbar samples): 

cmsRun ./python/ssbanalyzer_2016_cfg.py

MC (bg samples):

cmsRun ./python/ssbanalyzer_2016_bg_cfg.py

