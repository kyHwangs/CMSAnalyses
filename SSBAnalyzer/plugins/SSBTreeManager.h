// SSBTMMaker v2.02

#ifndef SSBTreeManager_h
#define SSBTreeManager_h 

#include <vector>
#include <iostream>
#include <TROOT.h>
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TClonesArray.h"
#include "TH1D.h"


class SSBTreeManager
{

    typedef std::string 				st;

    typedef std::map<st, bool> 				map_b;
    typedef std::map<st, bool>::iterator 		map_b_it;
    typedef std::map<st, int> 				map_i;
    typedef std::map<st, int>::iterator 		map_i_it;
    typedef std::map<st, unsigned int> 			map_ui;
    typedef std::map<st, unsigned int>::iterator 	map_ui_it;
    typedef std::map<st, float> 			map_f;
    typedef std::map<st, float>::iterator 		map_f_it;
    typedef std::map<st, double> 			map_d;
    typedef std::map<st, double>::iterator 		map_d_it;
    typedef std::map<st, st> 				map_s;
    typedef std::map<st, st>::iterator 			map_s_it;
    typedef std::map<st, TClonesArray*>			map_lv;
    typedef std::map<st, TClonesArray*>::iterator 	map_lv_it;

    typedef std::vector<bool> 				vec_b;
    typedef vec_b::iterator 				vec_b_it;
    typedef std::vector<int> 				vec_i;
    typedef vec_i::iterator 				vec_i_it;
    typedef std::vector<unsigned int> 			vec_ui;
    typedef vec_ui::iterator 				vec_ui_it;
    typedef std::vector<float> 				vec_f;
    typedef vec_f::iterator 				vec_f_it;
    typedef std::vector<double> 			vec_d;
    typedef vec_d::iterator 				vec_d_it;
    typedef std::vector<st> 				vec_s;
    typedef vec_s::iterator 				vec_s_it;

    typedef std::map<st, vec_b> 			map_vb;
    typedef std::map<st, vec_b>::iterator 		map_vb_it;
    typedef std::map<st, vec_i> 			map_vi;
    typedef std::map<st, vec_i>::iterator 		map_vi_it;
    typedef std::map<st, vec_ui> 			map_vui;
    typedef std::map<st, vec_ui>::iterator 		map_vui_it;
    typedef std::map<st, vec_f> 			map_vf;
    typedef std::map<st, vec_f>::iterator 		map_vf_it;
    typedef std::map<st, vec_d> 			map_vd;
    typedef std::map<st, vec_d>::iterator 		map_vd_it;
    typedef std::map<st, vec_s> 			map_vs;
    typedef std::map<st, vec_s>::iterator 		map_vs_it;

    map_b	VariableBox_Bool;
    map_i	VariableBox_Int;
    map_ui	VariableBox_UInt;
    map_f	VariableBox_Float;
    map_d	VariableBox_Double;
    map_s	VariableBox_String;
    map_lv	VariableBox_LorentzVector;

    map_b_it	it_VariableBox_Bool;
    map_i_it	it_VariableBox_Int;
    map_ui_it	it_VariableBox_UInt;
    map_f_it	it_VariableBox_Float;
    map_d_it	it_VariableBox_Double;
    map_s_it	it_VariableBox_String;
    map_lv_it	it_VariableBox_LorentzVector;

    map_vb	VectorBox_Bool;
    map_vi	VectorBox_Int;
    map_vui	VectorBox_UInt;
    map_vf	VectorBox_Float;
    map_vd	VectorBox_Double;
    map_vs	VectorBox_String;

    map_vb_it	it_VectorBox_Bool;
    map_vi_it	it_VectorBox_Int;
    map_vui_it	it_VectorBox_UInt;
    map_vf_it	it_VectorBox_Float;
    map_vd_it	it_VectorBox_Double;
    map_vs_it	it_VectorBox_String;

public:

    SSBTreeManager();
    ~SSBTreeManager();

    void Book(TTree* tree);
    void InitializeVariables();

    void GenBook(TTree* tree);
    void GenInitializeVariables();

    void FillNtuple();

    void Fill(st, bool);
    void Fill(st, int);
    void Fill(st, unsigned int);
    void Fill(st, float);
    void Fill(st, double);
    void Fill(st, st);

    void Fill(st, vec_b);
    void Fill(st, vec_i);
    void Fill(st, vec_ui);
    void Fill(st, vec_f);
    void Fill(st, vec_d);
    void Fill(st, vec_s);

    void Fill(st, double, double, double, double, int);
    void Fill(st, double, double, double, double, unsigned int);

private:

    TTree* ssbtree;

};

#endif
