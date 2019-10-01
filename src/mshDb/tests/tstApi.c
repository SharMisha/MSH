/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstApi.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 25 ноября 2018 г., 19:35
 */

#include "mshDebag.h"

void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);
void vmtstWriteArrData(TarrData *pArr,TdivStream *pdiv);
void vmtstWriteAdrOfferr(TAdrOfferr *pAdrOfferr,TdivStream *pdiv);
void vmtstWriteQuery(TIterQuery *pcurQuery,TdivStream *pdiv);
void vmtstWriteKeyData(Tval *pVal,TdivStream *pdiv);
//
Terr mshgetDb(TDb *pDb,Tstr *pName,const char *pDir,enum TisSet dbisSet,int sizePartGlb,enum TtypeDb typeDb);
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
Terr mshKill(TDb *pDb,TarrData *pInds,char tpK);
Terr mshNextInit(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey);
Terr mshNext(TIterNext *pIterNext);
Terr mshBack(TIterNext *pIterNext);
Terr mshNext1(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey);
Terr mshQueryInit(TIterQuery *pQuery);
Terr mshQuery(TIterQuery *pQuery);
Terr mshQuery1(TIterQuery *pQuery);
void mshDbClear(TDb *pDb);
void mshDbFree(TDb *pDb);
//

/*
 * Simple C Test Suite
 */
void test1() {
 printf("tstAPI test 1\n");
 TdivStream *pdiv=NULL;
 TDb Db;
 TarrData Inds;
 Tval *pKeyData;
 Tdata key,data,ind;
 TIterNext IterNext;
 int j;
 Terr err;
 TIterQuery IterQuery;
 char sdir[]="tst\0  tstApi\0";
 Tstr *psname;
 //
 memNULL(&key);
 memNULL(&data);
 memNULL(&ind);
 psname=(Tstr*)(&(sdir[4]));
 psname->size=6;
 err=mshgetDb((TDb*)(&Db),psname,sdir,1,0,EtypeDb);
 arrInit(Inds,5);
 arrsetptop(Inds);
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 //*
 DataSetInt(key);
 DataSetInt(data);
 for (j=0;j<5;j++) {
  key.intgr=j;
  arrPushDan(Inds,key);
  }
 data.intgr=100;
 vmPrntBK(pdiv);
 vmtstWriteArrData(&Inds,pdiv);
 vmPrntBK(pdiv);
 err=mshSet(&Db,&Inds,&data);
 vmPrnt(pdiv,"***Команда Set*** %d",err);
 vmtstWriteTree(&Db,pdiv);
 //*
 pKeyData=mshGet(&Db,&Inds,&err);
 vmPrnt(pdiv,"***Команда Get is*** %d",err);
 vmPrntBK(pdiv);
 vmtstWriteKeyData(pKeyData,pdiv);
 vmPrntBK(pdiv);
 //*
 err=mshKill(&Db,&Inds,CtypeKiLL);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"***Команда Kill*** %d",err);
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 //*
 pKeyData=mshGet(&Db,&Inds,&err);
 vmPrnt(pdiv,"***Команда Get not*** %d",err);
 vmPrntBK(pdiv);
 vmtstWriteKeyData(pKeyData,pdiv);
 vmPrntBK(pdiv);
 //
 //*
 vmtstWriteArrData(&Inds,pdiv);
 vmPrnt(pdiv,"***Расщипить нижний уровень***");
 err=mshSet(&Db,&Inds,&data);
 vmtstWriteTree(&Db,pdiv);
 ind=*(arrptop(Inds));
 data.intgr=200;
 for (j=0;j<5;j++) {
// for (j=0;j<4;j++) {
  arrptop(Inds)->intgr++;
  data.intgr++;
  vmPrnt(pdiv," %d[%ld]=%ld,",j,arrptop(Inds)->intgr,data.intgr);
  err=mshSet(&Db,&Inds,&data);
  }
 arrptop(Inds)->intgr++;
 data.intgr++;
 vmPrnt(pdiv," %d[%ld]=%ld,",j,arrptop(Inds)->intgr,data.intgr);
 vmPrntBK(pdiv);
 err=mshSet(&Db,&Inds,&data);
 vmtstWriteTree(&Db,pdiv);
 //* 
 vmPrnt(pdiv,"***Расщипить верхний уровень***");
 vmPrntBK(pdiv);
 data.intgr=300;
 for (j=0;j<6;j++) {
  arrsec(Inds).intgr++;
  data.intgr++;
  vmPrnt(pdiv," %d[%ld]=%ld,",j,arrsec(Inds).intgr,data.intgr);
  err=mshSet(&Db,&Inds,&data);
  vmtstWriteTree(&Db,pdiv);
  }
 vmtstWriteTree(&Db,pdiv);
 arrsec(Inds)=ind;

//*
 vmPrntBK(pdiv);
 vmtstWriteArrData(&Inds,pdiv);
 ind=*(arrptop(Inds));
 arrPop(Inds);
 Inds.psec->intgr=0;
 vmPrntBK(pdiv);
 vmtstWriteArrData(&Inds,pdiv);
 IterNextInit(IterNext,&Db,CcmdNext);
 err=mshNextInit(&IterNext,&Inds,NULL);
 vmPrnt(pdiv,"***Получить 1-ую вершину Next*** err=%d",err);
 vmPrntBK(pdiv);
 vmtstWriteKeyData(&(IterNext.val),pdiv);
 vmPrntBK(pdiv);
 vmtstWriteAdrOfferr((TAdrOfferr*)(&IterNext),pdiv);
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
  //*
 vmtstWriteArrData(&Inds,pdiv);
 IterNext.type=CcmdBack;
 err=mshNextInit(&IterNext,&Inds,NULL);
 vmPrnt(pdiv,"***Получить последнюю вершину Back*** err=%d",err);
 vmPrntBK(pdiv);
 vmtstWriteKeyData(&(IterNext.val),pdiv);
 vmPrntBK(pdiv);
 vmtstWriteAdrOfferr((TAdrOfferr*)(&IterNext),pdiv);
 vmPrnt(pdiv,"***Получить следующие вершины Back***");
 while((err=mshBack(&IterNext))==COutKodOk){
  vmPrntBK(pdiv);
  vmtstWriteKeyData(&(IterNext.val),pdiv);
  vmPrntBK(pdiv);
  vmtstWriteAdrOfferr((TAdrOfferr*)(&IterNext),pdiv);
  }
 //*
// vmtstWriteTree(&Db,pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"Индекс");
 vmtstWriteArrData(&Inds,pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"***Получить одиночные вершины Next1***");
 vmPrntBK(pdiv);
 IterNext.type=CcmdNext;
 DataSetNULL(key);
 while(1){
  err=mshNext1(&IterNext,&Inds,&key);
  if (err!=COutKodOk) break;
  vmtstWriteKeyData(&(IterNext.val),pdiv);
  vmPrntBK(pdiv);
  vmPrntBK(pdiv);
  vmtstWriteAdrOfferr((TAdrOfferr*)(&IterNext),pdiv);
  }
 //*
 vmPrnt(pdiv,"***Получить одиночные вершины Back1***");
 vmtstWriteArrData(&Inds,pdiv);
 IterNext.type=CcmdBack;
 DataSetNULL(key);
 vmPrntBK(pdiv);
 while(1){
  err=mshNext1(&IterNext,&Inds,&key);
  if (err!=COutKodOk) break;
  vmtstWriteKeyData(&(IterNext.val),pdiv);
  vmPrntBK(pdiv);
  vmPrntBK(pdiv);
  vmtstWriteAdrOfferr((TAdrOfferr*)(&IterNext),pdiv);
  }
//*
 vmPrnt(pdiv,"***Исходное дерево***");
 vmtstWriteTree(&Db,pdiv);
 QueryInit(IterQuery,&Db);
 arrPop(Inds);
 arrPop(Inds);
// vmtstWriteArrData(&Db,&Inds,pdiv);
// ind=*(Inds.ptop);
 arrPop(Inds);
// arrClear(Inds);
 vmPrnt(pdiv,"   Исходный индекс Query   ");
 vmtstWriteArrData(&Inds,pdiv);
 //
 parrCopy(&(IterQuery.rootInd),&Inds);
 err=mshQueryInit(&IterQuery);
 vmPrnt(pdiv,"***Получить 1-ую вершину Query*** err=%d",IterQuery.iterNext.AdrOfferr.err);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  Итератор Query  ");
 vmtstWriteQuery(&IterQuery,pdiv);
 vmPrntBK(pdiv);
 //*
 vmPrnt(pdiv,"   Получить следующие вершины   ");
 vmPrntBK(pdiv);
 while(1){
  err=mshQuery(&IterQuery);
  if (err!=COutKodOk) break;
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"  Итератор Query  ");
  vmtstWriteQuery(&IterQuery,pdiv);
  vmPrntBK(pdiv);
  }
 ///*
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"***-----Получить одиночные вершины Query1-----***");
 vmPrntBK(pdiv);
// arrClear(IterQuery.rootInd);
 arrClear(IterQuery.addInd);
 for(j=0;;j++){
  vmPrnt(pdiv,"   Итерация %d",j);
  vmtstWriteArrData(&Inds,pdiv);
  err=mshQuery1(&IterQuery);
  if (err!=COutKodOk) break;
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"  Итератор Query  ");
  vmtstWriteQuery(&IterQuery,pdiv);
  vmPrntBK(pdiv);
  }
 //*/ 
 //*
 // 
 mshDbClear((TDb*)(&Db));
 //
 vmPrnt(pdiv,"***Очистить Дерево и создать новое***");
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 ///*
 vmPrnt(pdiv,"Записать вершины [1,A]=56; [1,2]=3.14; [1,B,7]=Привет; [1,3,7]=Привет2");
 vmPrntBK(pdiv);
 Inds.busy=2;
 arrsetptop(Inds);
 //
 DataSetInt(Inds.psec[0]);
 Inds.psec[0].intgr=1;
 DataSetStr(Inds.psec[1]);
 Inds.psec[1].str.size=1;
 Inds.psec[1].str.psec[0]='A';
 DataSetInt(data);
 data.intgr=56;
 err=mshSet(&Db,&Inds,&data);
 //*
 DataSetInt(Inds.psec[1]);
 Inds.psec[1].intgr=2;
 DataSetFloat(data);
 data.fnum=3.14;
 err=mshSet(&Db,&Inds,&data);
 //
 Inds.busy=3;
 arrsetptop(Inds);
 Inds.psec[1].intgr=3;
 DataSetInt(Inds.psec[2]);
 Inds.psec[2].intgr=7;
 char *pStrSet="Привет2";
 DataSetbStr(data);
 data.bstr.psec=pStrSet;
 data.bstr.size=strlen(pStrSet);
 err=mshSet(&Db,&Inds,&data);
 //
 DataSetStr(Inds.psec[1]);
 Inds.psec[1].str.size=1;
 Inds.psec[1].str.psec[0]='B';
 data.bstr.size--;
 err=mshSet(&Db,&Inds,&data);
 vmtstWriteTree(&Db,pdiv);
 //--------------------------------------------------------  
 Inds.busy=1;
 arrsetptop(Inds);
 //*
 vmPrnt(pdiv,"   Исходный индекс Query   ");
 vmtstWriteArrData(&Inds,pdiv);
 parrCopy(&(IterQuery.rootInd),&Inds);
 err=mshQueryInit(&IterQuery);
 vmPrnt(pdiv,"***Получить 1-ую вершину Query*** err=%d",IterQuery.iterNext.AdrOfferr.err);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"   Данные в вершине   ");
 vmtstWriteKeyData(&(IterQuery.iterNext.val),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  Итератор Query  ");
 vmtstWriteQuery(&IterQuery,pdiv);
 vmPrntBK(pdiv);
 
 //*
 vmPrnt(pdiv,"   Получить следующие вершины   ");
 while(1){
  err=mshQuery(&IterQuery);
  if (err!=COutKodOk) break;
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"  Итератор Query  ");
  vmtstWriteQuery(&IterQuery,pdiv);
  vmPrntBK(pdiv);
  }
 ///*
 vmPrnt(pdiv,"---Получить вершины Query1---");
 parrCopy(&(IterQuery.rootInd),&Inds);
 arrClear(IterQuery.addInd);
 while((err=mshQuery1(&IterQuery))==COutKodOk){
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"  Итератор Query  ");
  vmtstWriteQuery(&IterQuery,pdiv);
  vmPrntBK(pdiv);
  }
 vmPrntBK(pdiv);
 // */
 //--------------------------
 //*
 IterQuery.iterNext.type=CcmdNext;
// vmtstWriteArrData(&Db,&(IterQuery.rootInd),pdiv);
 Inds.busy=2;
 arrsetptop(Inds);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"   Исходный индекс   ");
 vmtstWriteArrData(&Inds,pdiv);
 err=mshNextInit(&(IterQuery.iterNext),&Inds,NULL);
 vmPrnt(pdiv,"***Получить 1-ую вершину Next***");
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d узел=",err);
 vmtstWriteKeyData(&(IterQuery.iterNext.val),pdiv);
 vmPrntBK(pdiv);
 vmtstWriteAdrOfferr((TAdrOfferr*)(&(IterQuery.iterNext) ),pdiv);
 vmPrntBK(pdiv);
 //*
 if (err==COutKodOk){
  vmPrnt(pdiv,"***Получить следующие вершины Next***");
  while(1){
   err=mshNext(&(IterQuery.iterNext));
   vmPrntBK(pdiv);
   vmPrnt(pdiv,"err=%d узел=",err);
   if (err!=COutKodOk) break;
   vmtstWriteKeyData(&(IterQuery.iterNext.val),pdiv);
   vmPrntBK(pdiv);
   vmtstWriteAdrOfferr((TAdrOfferr*)(&(IterQuery.iterNext)),pdiv);
   vmPrntBK(pdiv);
   }
  }
 vmPrntBK(pdiv);
 //*
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"   Исходный индекс   ");
 vmtstWriteArrData(&Inds,pdiv);
 IterQuery.iterNext.type=CcmdBack;
 err=mshNextInit(&(IterQuery.iterNext),&Inds,NULL);
 vmPrnt(pdiv,"***Получить последнюю вершину Back***");
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d узел=",err);
 vmtstWriteKeyData(&(IterQuery.iterNext.val),pdiv);
 vmPrntBK(pdiv);
 vmtstWriteAdrOfferr((TAdrOfferr*)(&(IterQuery.iterNext)),pdiv);
 vmPrntBK(pdiv);
 if (err==COutKodOk){
  vmPrnt(pdiv,"***Получить следующие вершины Back***");
  while(1){
   err=mshBack(&(IterQuery.iterNext));
   vmPrntBK(pdiv);
   vmPrnt(pdiv,"err=%d узел=",err);
   if (err!=COutKodOk) break;
   vmtstWriteKeyData(&(IterQuery.iterNext.val),pdiv);
   vmPrntBK(pdiv);
   vmtstWriteAdrOfferr((TAdrOfferr*)(&(IterQuery.iterNext)),pdiv);
   vmPrntBK(pdiv);
   }
  }
 //*
 vmPrntBK(pdiv);
 IterQuery.iterNext.type=CcmdNext;
 vmPrnt(pdiv,"   Исходный индекс   ");
 vmtstWriteArrData(&Inds,pdiv);
 vmPrnt(pdiv,"***Получить вершины Next1***");
 DataSetNULL(key);
 vmPrntBK(pdiv);
 while(1){
  err=mshNext1(&(IterQuery.iterNext),&Inds,&key);
  vmPrnt(pdiv,"err=%d узел=",err);
  if (err!=COutKodOk) break;
  vmtstWriteKeyData(&(IterQuery.iterNext.val),pdiv);
  vmPrntBK(pdiv);
  vmtstWriteAdrOfferr((TAdrOfferr*)(&(IterQuery.iterNext)),pdiv);
  vmPrntBK(pdiv);
  }
 //*
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"   Исходный индекс   ");
 vmtstWriteArrData(&Inds,pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"***Получить вершины Back1***");
 vmPrntBK(pdiv);
 IterQuery.iterNext.type=CcmdBack;
 DataSetNULL(key);
 while(1){
  err=mshNext1(&(IterQuery.iterNext),&Inds,&key);
  vmPrnt(pdiv,"err=%d узел=",err);
  if (err!=COutKodOk) break;
  vmtstWriteKeyData(&(IterQuery.iterNext.val),pdiv);
  vmPrntBK(pdiv);
  vmtstWriteAdrOfferr((TAdrOfferr*)(&(IterQuery.iterNext)),pdiv);
  vmPrntBK(pdiv);
  }
 //----------------------------------------------------------
// */
 QueryFree(IterQuery);
 //*/
 //
// arrAdd(Inds);
// arrAdd(Inds);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"освободить данные");
 vmPrntBK(pdiv);
 arrFree(Inds);
 mshDbFree( (TDb*)(&Db) );
 }

void test2() {
 printf("tstApi test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstApi) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstApi\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstApi)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstApi) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstApi)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstApi) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
