/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstApiAlt.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 25 ноября 2018 г., 20:02
 */

#include "mshDebag.h"

void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);
void vmtstWriteArrData(TarrData *pArr,TdivStream *pdiv);
void vmtstWriteAdrOfferr(TAdrOfferr *pAdrOfferr,TdivStream *pdiv);
void vmtstWriteQuery(TIterQuery *pcurQuery,TdivStream *pdiv);
void vmtstWriteKeyData(Tval *pVal,TdivStream *pdiv);
Terr mshgetDb(TDb *pDb,Tstr *pName,const char *pDir,enum TisSet dbisSet,int sizePartGlb,enum TtypeDb typeDb);
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
Terr mshKill(TDb *pDb,TarrData *pInds,char tpK);
Terr mshNextInit(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey);
Terr mshNext(TIterNext *pIterNext);
Terr tstfindNodeRoot(TIterNext *pIterNext,TarrData *pInds);
void mshDbFree(TDb *pDb);

/*
 * Simple C Test Suite
 */

void test1() {
 TdivStream *pdiv=NULL;
 TDb Db;
 TarrData Inds;
 Tval *pKeyData;
 Tdata key,data,*pData;
 int j;
 Terr err;
 char sdir[]="tst\0  tstAPIAlt\0";
 const char *psdir=&sdir[0];
 Tstr *psname;
 //
 psname=(Tstr*)(&(sdir[4]));
 psname->size=9;
 err=mshgetDb((TDb*)(&Db),psname,psdir,1,0,EtypeDb);
//
 printf("tstApiAlt test 1\n");
 arrInit(Inds,5);
 arrsetptop(Inds);
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 //*
 memNULL(&key);
 memNULL(&data);
 DataSetInt(key);
 DataSetInt(data);
 for (j=0;j<5;j++) {
  key.intgr=j;
  arrPushDan(Inds,key);
  }
 data.intgr=100;
 pData= Inds.psec + 2;
 vmPrntBK(pdiv);
 pDataSetAlt(pData);
 vmPrnt(pdiv,"Исходный индекс");
 vmPrntBK(pdiv);
 vmtstWriteArrData(&Inds,pdiv);
 vmPrntBK(pdiv);
  //*
 err=mshSet(&Db,&Inds,&data);
 vmPrnt(pdiv,"***Команда Set*** %d",err);
 vmtstWriteTree(&Db,pdiv);
 //*
 pKeyData=mshGet(&Db,&Inds,&err);
 vmPrnt(pdiv,"***Команда Get is*** %d",err);
 vmPrntBK(pdiv);
 vmtstWriteKeyData(pKeyData,pdiv);
 vmPrntBK(pdiv);
/*
 vmtstWriteKeyData(&Db,pKeyData,pdiv);
*/
 vmPrntBK(pdiv);
 //*
 err=mshKill(&Db,&Inds,CtypeKiLL);
 vmPrnt(pdiv,"***Команда Kill*** %d",err);
 vmtstWriteArrData(&Inds,pdiv);
 vmPrntBK(pdiv);
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 //*
 pKeyData=mshGet(&Db,&Inds,&err);
 vmPrnt(pdiv,"***Команда Get not*** %d",err);
 vmPrntBK(pdiv);
 vmtstWriteKeyData(pKeyData,pdiv);
 vmPrntBK(pdiv);
 //*
 vmPrnt(pdiv,"***SetAlt 10:15*** %d",err);
 Inds.busy=2;
 arrsetptop(Inds);
 pData= arrptop(Inds);
 pDataSetAlt(Inds.psec);
 for(j=10;j++<16;){
  pData->intgr=j;
  data.intgr=100+j;
  err=mshSet(&Db,&Inds,&data);
  }
 vmtstWriteTree(&Db,pdiv);
 //*
 TIterNext IterNext;
 vmtstWriteArrData(&Inds,pdiv);
 arrPop(Inds);
 vmtstWriteArrData(&Inds,pdiv);
 IterNextInit(IterNext,&Db,CcmdNext);
 err=mshNextInit(&IterNext,&Inds,NULL);
 vmPrnt(pdiv,"***Получить 1-ую вершину Next*** err=%d",err);
 vmPrntBK(pdiv);
 vmtstWriteKeyData(&(IterNext.val),pdiv);
 vmPrntBK(pdiv);
 vmtstWriteAdrOfferr((TAdrOfferr*)(&IterNext),pdiv);
 vmPrntBK(pdiv);
 vmtstWriteArrData(&Inds,pdiv);
 vmPrntBK(pdiv);
 //*
 if (err==COutKodOk){
  vmPrnt(pdiv,"***Получить следующие вершины***");
  while((err=mshNext(&IterNext))==COutKodOk){
   vmPrntBK(pdiv);
   vmtstWriteKeyData(&(IterNext.val),pdiv);
   vmPrntBK(pdiv);
   vmtstWriteAdrOfferr((TAdrOfferr*)(&IterNext),pdiv);
   }
  }
 vmPrnt(pdiv," err=%d",err);
 vmPrntBK(pdiv);
 //*/
 arrFree(Inds);
 mshDbFree( (TDb*)(&Db) );
 }

void test2()
{
 printf("tstApiAlt test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstApiAlt) message=error message sample\n");
}

int main(int argc, char** argv)
{
 printf("%%SUITE_STARTING%% tstApiAlt\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstApiAlt)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstApiAlt) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstApiAlt)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstApiAlt) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
