/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstInsNode.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 25 ноября 2018 г., 20:07
 */

#include "mshDebag.h"

Terr mshDbLocNew(TDb *pDb,Tstr *pName,char *pDir,enum TtypeDb typeDb);
void mshDbFree(TDb *pDb);
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
Terr mshKill(TDb *pDb,TarrData *pInds,char tpK);
void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);
Terr tstfindNodeRoot(TIterNext *pIterNext,TarrData *pInds);
Terr mshNext(TIterNext *pIterNext);
TpgData* PgDownL(TDb *pDb,Tadr *pAdr);
void vmtstWriteKeyData(Tval *pVal,TdivStream *pdiv);
/*
 * Simple C Test Suite
 */

void test1(){
 TDb Db;
 Terr err;
 TarrData Inds;
 Tdata setData;
// int countIter=1;
 int countIter=1000;
 TdivStream *pdiv=NULL;
 char sdir[]="tst\0  tstInsNode\0",tpK;
 Tstr *psname;
 TuPg uPg;
 int i;
 //
 psname=(Tstr*)(&(sdir[4]));
 psname->size=10;
 printf("tstInsNode test 1\n");
 if ((err=mshDbLocNew((TDb*)(&Db),psname,sdir,EtypeDb) )<CERR) {
  printf("Ошибка err=%d\n",err);
  return;
  }
 ///*
 arrInit(Inds,4);
 ///*
// vmtstWriteTree(&Db,NULL);
// arrBusy(Inds);
// pDataSetStat(Inds.ptop,CTypeDataInt);
// Inds.ptop->intgr=1;
 arrBusyNull(Inds);
 pDataSetInt(arrptop(Inds));
 memNULL(&setData);
 DataSetInt(setData);
 for (i = 0; i < countIter; ++i) {
// for (int i = countIter; i >0 ; --i) {
  if (i % 15 == 0) printf("\n");
  if (i==40){
   printf("i=%i\n",i);
//   vmtstWriteTree(&Db,NULL);
   }
  arrptop(Inds)->intgr=i;
  setData.intgr=i + countIter;
//  if (i==( 1283)){
//   vmtstWriteTree(&Db,NULL);
   err=mshSet(&Db,&Inds,&setData);
//   vmtstWriteTree(&Db,NULL);
//   }  else err=mshSet(&Db,&Inds,&setData);
  printf("[%i] err=%i ",i,err);
  }
//vmtstWriteTree(&Db,NULL);
///*
tpK=CtypeKiLLData;
arrptop(Inds)->intgr=884;
err=mshKill(&Db,&Inds,tpK);
arrptop(Inds)->intgr=881;
err=mshKill(&Db,&Inds,tpK);
arrptop(Inds)->intgr=281;
err=mshKill(&Db,&Inds,tpK);
printf("Удалить вершины 884, 881, 281 err=%d ",err);
vmtstWriteTree(&Db,NULL);
TIterNext IterNext;
IterNextInit(IterNext,(TDb*)(&Db),CcmdNext);
arrPop(Inds);
err=tstfindNodeRoot(&IterNext,&Inds);
uPg.pgD=PgDownL(&Db,&(IterNext.AdrOfferr.adr));
IterNext.val=pgOffSec(uPg.pgD,IterNext.AdrOfferr.off);
printf("Итератор Next err=%d\n",err);
printf(" ");
vmtstWriteKeyData( &(IterNext.val),NULL);
i=2;
while((err=mshNext(&IterNext))==COutKodOk){
 printf(" ");
 vmtstWriteKeyData(&(IterNext.val),pdiv);
 if (i++%5==0) printf("\n");
 }
//
arrFree(Inds);
//*/
vmPrntBK(pdiv);
vmPrnt(pdiv,"освободить данные");
vmPrntBK(pdiv);
mshDbFree( (TDb*)(&Db) );
}

void test2()
{
 printf("tstInsNode test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstInsNode) message=error message sample\n");
}

int main(int argc, char** argv)
{
 printf("%%SUITE_STARTING%% tstInsNode\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstInsNode)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstInsNode) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstInsNode)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstInsNode) \n");
*/

 printf("%%SUITE_FINISHED%% time=0\n");
 return (EXIT_SUCCESS);
}
