import FWCore.ParameterSet.Config as cms

#Based on https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2

#process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
from RecoMET.METFilters.BadPFMuonFilter_cfi import*
BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")

#process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
from RecoMET.METFilters.BadChargedCandidateFilter_cfi import*
BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")

"""
ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
   inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
   reverseDecision = cms.bool(False)
)

ApplyBaselineHBHEIsoNoiseFilter = cms.EDFilter('BooleanFlagFilter',
   inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHEIsoNoiseFilterResult'),
   reverseDecision = cms.bool(False)
)
"""

"""
metFilters = cms.Sequence(
    BadPFMuonFilter *
    BadChargedCandidateFilter 
   # HBHENoiseFilterResultProducer* #produces HBHE baseline bools
   # ApplyBaselineHBHENoiseFilter  #reject events based 
    #process.ApplyBaselineHBHEIsoNoiseFilter*   #reject events based  < 10e-3 mistake rate 
)
"""
