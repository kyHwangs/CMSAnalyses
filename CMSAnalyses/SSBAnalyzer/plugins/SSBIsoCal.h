#ifndef SSBIsoCal_h
#define SSBIsoCal_h 

#include <iostream>

class SSBIsoCal
{
public:
    
   SSBIsoCal();
   ~SSBIsoCal();

   // Detector-based Isolation
   double MuonRelTrkIso(double trkIso, double muPt);
   double ElecRelIso(double HcalTowerSumEt, double EcalRecHitSumEt, double TkSumPt, double elePt); 
   // Effective Area 
   double EffArea2015(double abseleEta); 
   double EffArea2016(double abseleEta); 

   /// PF Isolation
   // for muon, pfChargedHadronEt, pfNeutralHadronEt, pfPhotonEt, pfChargedParticlePileUpPt
   double PFIsodBeta(double pfCH, double pfNH, double pfPho, double pfPU, double lepPt, double dbfac);
   double PFIsoRho(double pfCH, double pfNH, double pfPho, double rho, double effA, double lepPt);

private:

};

#endif
