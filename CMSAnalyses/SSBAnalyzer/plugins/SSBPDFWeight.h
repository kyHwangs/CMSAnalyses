#ifndef SSBPDFWeight_h
#define SSBPDFWeight_h 

#include <iostream>

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include <LHAPDF/LHAPDF.h>

class SSBPDFWeight
{

public:
    
   SSBPDFWeight(int npdfset, std::string pdfname);
   ~SSBPDFWeight();

   void SetScalePDF(double Q){_Q=Q;}
   void SetIncomingPartion1(double id1, double x1, double pdf1);
   void SetIncomingPartion2(double id2, double x2, double pdf2);
   void SetPDFSet(int nthPDF){_pdfset = nthPDF;}
   /// Use member in PDF set nset (multi-set version).
   void UsePDF(int nset, int member);
 
   void SetNominalWeight(double nominal_w);
 
   ///Number of members available in the current set. 
   int getNumberPDF(int pdfset);
   double getPDF1(int pdfset);
   double getPDF2(int pdfset);
   double getNewPDF1(int pdfset);
   double getNewPDF2(int pdfset);

   double getCentralPDFWeight(int number_incoming_parton);
   std::vector<double> getSys(std::string sys_name);

private:

   /// Incoming partions
   double _id1, _id2;
   /// Bjorken x
   double _x1, _x2;
   /// PDF Weights
   double _pdf1, _pdf2;
   /// Scale of Hard Interaction
   double _Q;
   /// the number of pdfs
   int _numberPDFs;
   int _pdfset;
   double _nominal_w;
   double _w_up;
   double _w_down;
};

#endif
