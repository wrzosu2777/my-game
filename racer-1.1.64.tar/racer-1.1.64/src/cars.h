
#ifndef _CARS_PARAMS_INCLUDE_
#define _CARS_PARAMS_INCLUDE_

#include "player.h"

// bitmapa auta
#define  DF_CAR_X    40 // 50 // rozmery obrazku auta
#define  DF_CAR_Y    80 //100
#define  BETA     26.56 // vypocet jako tg(beta) = DF_CAR_Y/DF_CAR_X - stupne
#define  BETARAD  0.464 // vypocet jako tg(beta) = DF_CAR_Y/DF_CAR_X - radiany
#define  D_CON       45 // vypocet jako D_CON^2  = (DF_CAR_Y/2)^2 + (DF_CAR_Y/2)^2
#define  D_KOLA      25 // mensi, nez DCON - urcuej polohu kol
#define  STOPPED_LIMIT 10 // minimalni pocat framu pri zastaveni, nez je povoleno hozeni na silnici

#define  DF_CAR_BMPS  100 // velikost tempove bitmapy - uhlopricka

// odsazeni stinu auta
#define SH_X  4  // doleva
#define SH_Y  4  // dolu

// globalni grafika
#define MAX_CARS_BITMAPS  100 // celkovy pocet bitmap MAX_CARS*3 - normal, noc, stin
#define MAX_CARS_POHAR_BITMAPS 40 // pocet tovarnich aut * 2 - den, noc
#define MAX_SMOGS_BITMAPS 8
#define MAX_SMOGS_CBITMAPS 4
#define MAX_CARS_LIGHTS    3
#define MAX_SMYK_BMP      18

#define MAX_KOL_BMP   5

// v stdh.cpp
extern BITMAP*  b_cars[MAX_CARS_BITMAPS]; // bitmapy aut
extern BITMAP*  b_carsPohar[MAX_CARS_POHAR_BITMAPS]; // zesedle bitmapy aut pro pohar
extern BITMAP*  b_obris; // bitmapa auticka velikosti 50*100 cerne barvy
extern BITMAP*  b_smogsC[MAX_SMOGS_CBITMAPS];    // bitmapy s kourem aut (32*32) - barva
extern BITMAP*  b_smogs[MAX_SMOGS_BITMAPS];     // bitmapy s kourem aut (32*32)
extern BITMAP*  b_carlights[MAX_CARS_LIGHTS];    // svetla aut
extern BITMAP*  b_smyk[MAX_SMYK_BMP];           // bitmapa smyku
extern BITMAP*  b_kola[MAX_KOL_BMP];           // bitmapa smyku




#include "car_params.h"

//LINUX

extern int  MAX_CARS;   // celkovy pocet aut
extern st_GCar_params  Cparams[MAX_CARS_TOTAL];

#endif // #ifndef _CARS_PARAMS_INCLUDE_
