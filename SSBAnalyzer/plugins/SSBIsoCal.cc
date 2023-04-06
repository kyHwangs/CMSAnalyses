#include <iostream>
#include <math.h>
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBIsoCal.h"

using namespace std;

SSBIsoCal::SSBIsoCal()
{
}

SSBIsoCal::~SSBIsoCal()
{
}

double SSBIsoCal::MuonRelTrkIso(double trkIso, double muPt)
{  
   double mu_relTrkIso;

   mu_relTrkIso = trkIso / muPt;

   return mu_relTrkIso;
}  

double SSBIsoCal::ElecRelIso(double HcalTowerSumEt, double EcalRecHitSumEt, double TkSumPt, double elePt)
{
   double ele_reliso;

   ele_reliso = (HcalTowerSumEt + EcalRecHitSumEt + TkSumPt) / elePt;

   return ele_reliso;
}

double SSBIsoCal::EffArea2015(double abseleEta)
{
   double eleEta = fabs(abseleEta);
   if      ( 0.0000 <= eleEta && eleEta < 1.0000 ) { return 0.1752; }
   else if ( 1.0000 <= eleEta && eleEta < 1.4790 ) { return 0.1862; }
   else if ( 1.4790 <= eleEta && eleEta < 2.0000 ) { return 0.1411; }
   else if ( 2.0000 <= eleEta && eleEta < 2.2000 ) { return 0.1534; }
   else if ( 2.2000 <= eleEta && eleEta < 2.3000 ) { return 0.1903; }
   else if ( 2.3000 <= eleEta && eleEta < 2.4000 ) { return 0.2243; }
   else if ( 2.4000 <= eleEta && eleEta < 5.0000 ) { return 0.2687; }
   else { cout << "EffArea2015 Error !!! CHECK YOUR ELECTRON ETA VARIABLE!!!" << endl; return -999; }
}

double SSBIsoCal::EffArea2016( double abseleEta)
{
   double eleEta = fabs( abseleEta );
   if      ( 0.0000 <= eleEta && eleEta < 1.0000 ) { return 0.1703; }
   else if ( 1.0000 <= eleEta && eleEta < 1.4790 ) { return 0.1715; }
   else if ( 1.4790 <= eleEta && eleEta < 2.0000 ) { return 0.1213; }
   else if ( 2.0000 <= eleEta && eleEta < 2.2000 ) { return 0.1230; }
   else if ( 2.2000 <= eleEta && eleEta < 2.3000 ) { return 0.1635; }
   else if ( 2.3000 <= eleEta && eleEta < 2.4000 ) { return 0.1937; }
   else if ( 2.4000 <= eleEta && eleEta < 5.0000 ) { return 0.2393; }
   else { cout << "EffArea2015 Error !!! CHECK YOUR ELECTRON ETA VARIABLE!!!" << endl; return -999; }}

double SSBIsoCal::PFIsodBeta(double pfCH, double pfNH, double pfPho, double pfPU, double lepPt, double dbfac)
{
   double pfisodbeta;

   pfisodbeta = ( pfCH + std::max( 0., pfNH + pfPho - dbfac*pfPU) ) / lepPt;

   return pfisodbeta;
}  

double SSBIsoCal::PFIsoRho(double pfCH, double pfNH, double pfPho, double rho, double effA, double lepPt)
{
   double pfisorho;

   pfisorho = ( pfCH + std::max( 0., pfNH + pfPho - rho*effA) ) / lepPt;

   return pfisorho;
}


