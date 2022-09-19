#include <iostream>
#include <string.h>
#include <stdio.h>
#include "CMSAnalyses/SSBAnalyzer/plugins/SSBPDFWeight.h"


SSBPDFWeight::SSBPDFWeight(int npdfset, std::string pdfname)
{
   LHAPDF::initPDFSet(npdfset, pdfname);
}

SSBPDFWeight::~SSBPDFWeight()
{
}

void SSBPDFWeight::SetIncomingPartion1(double id1, double x1, double pdf1)
{
   _id1 = id1;
   _x1 = x1;
   _pdf1 = pdf1;
}

void SSBPDFWeight::SetIncomingPartion2(double id2, double x2, double pdf2)
{
   _id2 = id2;
   _x2 = x2;
   _pdf2 = pdf2;
}

void SSBPDFWeight::SetNominalWeight(double nominal_w)
{
   _nominal_w = 1;
   _nominal_w = nominal_w;
}

int SSBPDFWeight::getNumberPDF(int pdfset)
{
//   std::cout << "getNumberPDF " << LHAPDF::numberPDF(pdfset) << std::endl;
   return LHAPDF::numberPDF(pdfset);
}

double SSBPDFWeight::getPDF1(int pdfset)
{
   double pdf1 = 1;
   //std::cout << "pdfset ? in the getPDF1 Function : "<< pdfset << std::endl;
   pdf1 = LHAPDF::xfx(pdfset, _x1, _Q, _id1);
   //std::cout << "IN THE getPDF1 _x1 : " << _x1 << " _id1 " << _id1  << " Q : " << _Q << std::endl;
   //pdf1 = LHAPDF::xfx(pdfset, _x1, _Q, _id1)/_x1;// Modified by Seungkyu
   //std::cout << " getPDF1 : ? " << pdf1<< std::endl;
   return pdf1;
}

double SSBPDFWeight::getPDF2(int pdfset)
{
   double pdf2 = 1;
   pdf2 = LHAPDF::xfx(pdfset, _x2, _Q, _id2);
   //std::cout << "pdfset ? in the getPDF2 Function : "<< pdfset << std::endl;
   //std::cout << "IN THE getPDF2 _x2 : " << _x2 << " _id2 " << _id2 << " Q : " << _Q << std::endl;
   //pdf2 = LHAPDF::xfx(pdfset, _x2, _Q, _id2)/_x2;//Modified by Seungkyu
   //std::cout << " getPDF2 : ? " << pdf2<< std::endl;
   return pdf2;
}

void SSBPDFWeight::UsePDF(int nset, int member)
{
   LHAPDF::usePDFMember(nset, member);
}

double SSBPDFWeight::getCentralPDFWeight(int number_incoming_parton)
{
   double CentralPdfWeight=1;
   
   UsePDF(_pdfset, 0);
//   std::cout << "_pdfset " << _pdfset << std::endl; 
   if (number_incoming_parton == 1)
   {
      CentralPdfWeight = getPDF1(_pdfset);
   } else if (number_incoming_parton == 2) {
      CentralPdfWeight = getPDF2(_pdfset);
   }

   return CentralPdfWeight;
}

std::vector<double> SSBPDFWeight::getSys(std::string sys_name)
{
   std::vector<double> sys;
   sys.clear();
   
   int i=0;
   int npdfsets = getNumberPDF(_pdfset);
   double weight_sys;
   //std::cout << "Num. PDF Set ? : " << npdfsets << std::endl;

   for (int j=1; j <= npdfsets; j++)
   {
      if (strcmp(sys_name.c_str(), "Up") == 0)
      {
         i = 2*j - 1;

      } else if (strcmp(sys_name.c_str(), "Down") == 0) {
         i = 2*j;
      
      } 

      if (i > npdfsets) {break;}
      
      UsePDF(_pdfset, i);
      weight_sys = getPDF1(_pdfset) * getPDF2(_pdfset) / _nominal_w;
      sys.push_back(weight_sys);
      //std::cout << "Here are getSys ... " << weight_sys << std::endl;
      /// Debug
      //std::cout << _nominal_w << std::endl;
      //if (strcmp(sys_name.c_str(), "Up") == 0){std::cout << "up " << i << "   " << weight_sys << std::endl;}
      //else {std::cout << "down " << i << "   " << weight_sys << std::endl;}
   }

   return sys;
}


