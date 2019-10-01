
/* 
 * File:   writeData.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 23 ноября 2018 г., 9:23
 */
#include "mshData.h"
#include "mshDiv.h"
#include "mshErr.h"
#include "mshDebag.h"

Terr DbOpenDiv(TDb *pDb,char* patch,enum TtypeDb typeDb);
void bStrGet(Tdata *pData,Tbstr* pbstrOut);
void vmtstWriteBuf(char *pC,int len,TdivStream *pdiv);
int roundDataDbAlt(TDb *pDb, TarrData *pInds,int isAlt, int (*pfdata)(TtreeData *ptreeData) , void *pUser);

void writeArr(TarrData *pArr,TdivStream *pdiv);
/*
 * Распечатать поле Tdata
 */
void writeData(Tdata *pData,TdivStream *pdiv,int isno1){
 char type;
 TarrData Arr;
 Tbstr bStr;
 //
 type= pDataSight(pData);
 if (isno1){
  if  (pDataIsAlt(pData)) {vmPrnt(pdiv,"%c",CKodDlmAlt);}
  else {vmPrnt(pdiv,"%c",CKodDlmArg);}
  }
 //распечатать массив
 if (pDataIsList(pData)){
  bStrGet(pData,&bStr);
  if (bStr.psec) {
   StrToArrData(bStr,Arr);
   writeArr(&Arr,pdiv);
   }
  return;
  }
 //
 switch (type) {
  case CDataInt  :
   vmPrnt(pdiv,"%ji",pData->intgr);
   break;
  case CDataFloat  :
   vmPrnt(pdiv,"%f ",pData->fnum);
   break;
  case CDataPointer  :
   vmPrnt(pdiv,"%p",pData->pointer);
   break;
  //строки
  default :
   bStrGet(pData,&bStr);
   vmtstWriteBuf(bStr.psec,bStr.size,pdiv);
   break;
  }
 }
/*
 * Распечатать Массив Tdata
 */
void writeArr(TarrData *pArr,TdivStream *pdiv){
 Tdata *pData;
 //
 if (pArr->busy){
  writeData(pArr->psec,pdiv,0);
  if (pArr->busy>1){
   for (pData=pArr->psec+1;pData<=pArr->ptop;pData++) {writeData(pData,pdiv,1);}
   }
  }
 }
/*
 * Распечатать Индекс
 */
void writeInd(TarrData *pArr,char cB,char cE,TdivStream *pdiv){
 vmPrnt(pdiv,"%c",cB);
 writeArr(pArr,pdiv);
 vmPrnt(pdiv,"%c",cE);
 }
/*
 * Распечатка дерева
 */
/*
typedef struct {
 TDb *pDb;
 TarrData Inds;
 Tval *pkeydata;
 int Level;
 int isAlt;
 void *pUser;
 } TtreeData;
*/
int writeDbSub(TtreeData *ptreeData){
 char cB,cE;
 int isAlt;
 TdivStream *pdiv=NULL;
 int err=COutKodOk;
 //
 if (isAlt==EroundAlt) {cB='{',cE='}';} else {cB='[',cE=']';}
 writeInd(&(ptreeData->Inds),cB,cE,NULL);
 vmPrnt(pdiv," = ");
 writeData(&(pValData(ptreeData->pkeydata)),NULL,0);
 vmPrntBK(pdiv);
 return err;
 }

int writeDb(char* pfl,TarrData *pInds){
 TDb Db;
 int ret;
 //
 if ((ret=DbOpenDiv(&Db,pfl,EtypeDb) )==COutKodOk){
  ret=roundDataDbAlt(&Db,pInds,1, writeDbSub , NULL);
  }
 return ret;
 }
