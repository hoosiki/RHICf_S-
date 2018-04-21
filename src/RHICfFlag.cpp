#include "RHICfFlag.h"

#include <iostream>
#include <string>
#include "A1Cal2.h"
#include "TF1.h"
#include "TMath.h"
#include <bitset>

using namespace std;
//bool event_flag[16]={false};
//bool discri_flag[32]={false};


RHICfFlag::RHICfFlag()
{
    Init();
    
};
RHICfFlag::~RHICfFlag()
{
    delete [] f;
};

void RHICfFlag::Init()
{ //make fit discri efficiency curve
    static const double fitparam0[2][16] = {{0.0520662,0.0522364,0.0521464,0.581714,0.0531239, 0.052079, 0.0528969, 0.0520054, 0.0527186, 0.0538613, 0.0532493, 0.053268, 0.0531763, 0.0538221, 0.0521287, 0.052582},{0.0520774,0.0493944,0.0540197, 0.478771, 0.04742, 0.0497991, 0.0528811, 0.0501449, 0.0500626, 0.0514371, 0.0506653,0.0504131, 0.0513074, 0.0498368, 0.0509328,0.0492228}};
    static const double fitparam1[2][16] = {{ 371.766, 330.089, 272.756, 104.722, 353.41, 282.97, 390.376, 433.254, 353.479, 361.112, 315.594, 354.829, 361.157, 343.286, 299.029, 405.443},{379.235, 359.076, 383.886, 106.749, 360.998, 401.6, 423.13, 431.784, 334.176, 367.939, 358.987, 368.446, 409.109, 420.076, 376.832, 356.993}};

    
    for(int tower =0;tower<2;tower++){
        for(int layer =0; layer<16; layer++){
            if(layer!=3){
                f[tower][layer] = new TF1("fit","1./(1.+TMath::Exp(-1.*(x-[0])*[1]))",0.,0.1);
            }
            else{f[tower][layer] = new TF1("fit","1./(1.+TMath::Exp(-1.*(x-[0])*[1]))",0.,1.0);}
            f[tower][layer] ->SetParameter(0, fitparam0[tower][layer]);
            f[tower][layer] ->SetParameter(1, fitparam1[tower][layer]);
        }
    }
    // make seed random
    srand((unsigned int)time(NULL));
};

  
void RHICfFlag::SetFlag(A1Cal2 *a1cal2){
  /*
  static const double fitparam0[2][16] = {{0.0520662,0.0522364,0.0521464,0.581714,0.0531239, 0.052079, 0.0528969, 0.0520054, 0.0527186, 0.0538613, 0.0532493, 0.053268, 0.0531763, 0.0538221, 0.0521287, 0.052582},{0.0520774,0.0493944,0.0540197, 0.478771, 0.04742, 0.0497991, 0.0528811, 0.0501449, 0.0500626, 0.0514371, 0.0506653,0.0504131, 0.0513074, 0.0498368, 0.0509328,0.0492228}};
  static const double fitparam1[2][16] = {{ 371.766, 330.089, 272.756, 104.722, 353.41, 282.97, 390.376, 433.254, 353.479, 361.112, 315.594, 354.829, 361.157, 343.286, 299.029, 405.443},{379.235, 359.076, 383.886, 106.749, 360.998, 401.6, 423.13, 431.784, 334.176, 367.939, 358.987, 368.446, 409.109, 420.076, 376.832, 356.993}};
  
    
  for(int tower =0;tower<2;tower++){
    for(int layer =0; layer<16; layer++){
      if(layer!=3){
	f[tower][layer] = new TF1("fit","1./(1.+TMath::Exp(-1.*(x-[0])*[1]))",0.,0.1);
      }
      else{f[tower][layer] = new TF1("fit","1./(1.+TMath::Exp(-1.*(x-[0])*[1]))",0.,1.0);}
      f[tower][layer] ->SetParameter(0, fitparam0[tower][layer]);
      f[tower][layer] ->SetParameter(1, fitparam1[tower][layer]);
    }
  }
  */
    double xv =0;
    double xv1 =0;
    double yv =0;
    double r_eff=0;
    bool dsc[2][16] ={{false}};
    for(int tower=0;tower<2;tower++){
        for(int layer=0;layer<16;layer++){
	  xv = a1cal2->cal[tower][layer];
	  xv1 = xv/1000;
	  //cout << tower <<" " << layer << " " << xv1 << endl; 
    	  yv  = f[tower][layer]->Eval(xv); //unit GeV
          r_eff = (double)rand()/(1.0*RAND_MAX);
          if(yv > r_eff){dsc[tower][layer] = true;
          }
        }
        //return (f[tower][layer])
    }
    
    bool pat_trig[2][14] = {{false}};
    for(int tower =0; tower<2; tower++){
        if(dsc[tower][0]&&dsc[tower][1]&&dsc[tower][2]) pat_trig[tower][0] = true;
        if(dsc[tower][1]&&dsc[tower][2]) pat_trig[tower][1] = true;
        if(dsc[tower][2]&&dsc[tower][4]) pat_trig[tower][2] = true;
        if(dsc[tower][4]&&dsc[tower][5]) pat_trig[tower][3] = true;
        if(dsc[tower][4]&&dsc[tower][5]&&dsc[tower][6]) pat_trig[tower][4] = true;
        if(dsc[tower][5]&&dsc[tower][6]&&dsc[tower][7]) pat_trig[tower][5] = true;
        if(dsc[tower][6]&&dsc[tower][7]&&dsc[tower][8]) pat_trig[tower][6] = true;
        if(dsc[tower][7]&&dsc[tower][8]&&dsc[tower][9]) pat_trig[tower][7] = true;
        if(dsc[tower][8]&&dsc[tower][9]&&dsc[tower][10]) pat_trig[tower][8] = true;
        if(dsc[tower][9]&&dsc[tower][10]&&dsc[tower][11]) pat_trig[tower][9] = true;
        if(dsc[tower][10]&&dsc[tower][11]&&dsc[tower][12]) pat_trig[tower][10] = true;
        if(dsc[tower][11]&&dsc[tower][12]&&dsc[tower][13]) pat_trig[tower][11] = true;
        if(dsc[tower][12]&&dsc[tower][13]&&dsc[tower][14]) pat_trig[tower][12] = true;
        if(dsc[tower][13]&&dsc[tower][14]&&dsc[tower][15]) pat_trig[tower][13] = true;
    }

    bool flag_trig[2][3] = {{false}}; // [2] : tower, [3] trigger mode (0;shower, 1;pizero, 2;highem)
    for(int tower=0;tower<2;tower++){
        if(pat_trig[tower][0]||pat_trig[tower][1]||pat_trig[tower][2]||pat_trig[tower][3]||
                pat_trig[tower][4]||pat_trig[tower][5]||pat_trig[tower][6]||pat_trig[tower][7]||
                pat_trig[tower][8]||pat_trig[tower][9]||pat_trig[tower][10]||pat_trig[tower][11]||
                pat_trig[tower][12]||pat_trig[tower][13]){
            flag_trig[tower][0] = true;
            //cout <<"tower" << " " << tower << " "  << "shower trigger occur" << endl;
        }
            if(pat_trig[tower][0]||pat_trig[tower][1]||pat_trig[tower][2]||pat_trig[tower][3]||
                pat_trig[tower][4]){
            flag_trig[tower][1] = true;
        }
        if(dsc[tower][3]){flag_trig[tower][2] = true;}
    }

    int event_flag[16]={0};
    event_flag[0] = 1;   //F_BPTX1  
    event_flag[1] = 1;  //F_BPTX2
    event_flag[2] = 0;  //F_EXT_L1T  
    event_flag[3] = 1;  //F_BPTX_AND  
    if(flag_trig[0][0] || flag_trig[1][0])event_flag[4] =1;  //F_L2T_SHOWER 
    event_flag[5] = 0;  //F_L2T_PEDESTAL
    event_flag[6] = 0;  //F_L2T_EXT     
    if(flag_trig[0][1] && flag_trig[1][1])event_flag[7] =1;  //F_L2T_SPECIAL1
    event_flag[8] = 1;  //F_L2T_L1T      
    if(flag_trig[0][2] || flag_trig[1][2])event_flag[9] =1;  //F_L2T_SPECIAL2 
    if(flag_trig[0][0] ==1)event_flag[10] =1; //F_STRG  
    if(flag_trig[1][0] ==1)event_flag[11] =1; //F_LTRG     
    event_flag[12] = 1; //F_BEAMFLAG
    event_flag[13] = 0; //F_PEDEFLAG 
    event_flag[14] = 0; //F_LASERFLAG
    event_flag[15] = 1; //F_ENABLEFLAG 

    int discri_flag[32] ={0};
    for(int tower=0; tower<2; tower++){
        for(int layer=0;layer<16; layer++){
	  if(dsc[tower][layer] ==true)discri_flag[16*tower+layer] = 1;
	  //cout <<  discri_flag[16*tower+layer];
        }
	//cout << ""<< endl;
    }
   
    for(int i=0; i<16; i++){
      if (i==0){a1cal2->flag[0] =0;}
      a1cal2->flag[0] += event_flag[i]*pow(2,i);	
      //cout << a1cal2->flag[0] << endl;
      //if(i==15)cout << bitset<16>(a1cal2->flag[0]) << endl;
	
    } 
    for(int i=0; i<32; i++){
        if (i==0){a1cal2->flag[1] =0;}
        a1cal2->flag[1] += discri_flag[i]*pow(2,i);
	//cout << a1cal2->flag[0] << endl;
	//cout << bitset<32>(a1cal2->flag[1]) << endl;	
	//Junsang****if(i==31)cout << bitset<32>(a1cal2->flag[1]) << endl;	
    } 
};



// //Fill flag
// for(int i=0; i<16; i++){
//   a1cal2->flag[0] += event_flag[i]*pow(2,i);
// } 
// for(int i=0; i<32; i++){
//   a1cal2->flag[1] += discri_flag[i]*pow(2,i);
// } 
