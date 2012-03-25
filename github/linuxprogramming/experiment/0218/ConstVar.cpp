/*
 * ConstVar.cpp
 *
 *  Created on: 2010-1-31
 *      Author: root
 */

#include "ConstVar.h"

int readLine = 200;
int readTimes  =5000;

double decayFactor = 0.998;
double cm = 3;// 2;
double cl = 1.0;
double beta =0.3;
int gap = 10000;
double eps =1;//1
int minPts = 3;//3
double src_bytes_factor = 1;
double dst_bytes_factor = 1;
double dst_factor = 1;
double dst_srv_factor = 1;
double count_factor = 1;
double srv_count_factor =125;// 125;;
double serror_factor = 1;
double protocol_factor  =3;//3
double service_factor  =3;//3
double flag_factor  =3;//3
double hot_factor =5000;//4000;//10e-4//4000//2000
double fagment_factor =30;//10e-1//25
int readPointNum = 0;
int minInnerPts = 3;
double minValue = 0.000001;
const char * fileName ="./file/kddcup.data_10_percent_corrected";//nor_smu_nep_bo.txt" ;//kddcup.data_10_percent_corrected";// "nor_smu_nep_bo.txt"; //; //"test_normal_neptune_smurf_back_ipsweep_guesspasswd";//
double partitionNum = 5;
int DIMENSION  = 36;
int VECTORDIMENSION = 36;
double globalWarp = 25;
double borderWarp = 7;
double localWarp = 2;

double borderPosMin1 = 0.36;
double borderPosMax1 = 0.64;

set<string>DOS;
set<string>R2L;
set<string>U2L;
set<string>PROBING;
vector<double>totalmax;
vector<double>totalmin;

int totalNormal = 0;
int totalDos = 0;
int totalR2l = 0;
int totalU2l = 0;
int totalProbing =0;

