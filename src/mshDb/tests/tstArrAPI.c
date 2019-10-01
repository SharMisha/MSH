/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstArrAPI.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 25 ноября 2018 г., 20:03
 */
#include "mshDebag.h"

void vmtstWriteDbArr(TDb *pDb,TdivStream *pdiv);
void vmtstWriteData(Tdata *pData,TdivStream *pdiv);
void vmtstWriteKeyData(Tval *pVal,TdivStream *pdiv);
Terr mshArrNextBack(TIterNext *pNext);
Terr mshgetDb(TDb *pDb,Tstr *pName,const char *pDir,enum TisSet dbisSet,int sizePartGlb,enum TtypeDb typeDb);
Terr mshArrNextBackInit(TIterNext *pNext);
Terr mshSetArr(TDb *pDb,Toff Off,Tdata *pSetData);
Tdata* mshGetArr(TDb *pDb,Tusize Off,Terr *perr);
Terr mshKillArr(TDb *pDb,Tusize Off);
void mshDbFree(TDb *pDb);
/*
 * Simple C Test Suite
 */

void test1() {
 TdivStream *pdiv=NULL;
 TDb Db,*pDb=&Db;
 Tdata Data,*pData;
 int j;
 TintNum Off;
 Terr err;
 Tusize Num[]={0,5,10,100};
 char sdir[]="tst\0  tstArrAPI\0";
 Tstr *psname;
 //
 psname=(Tstr*)(&(sdir[4]));
 psname->size=9;
 err=mshgetDb((TDb*)(&Db),psname,&sdir,1,0,EtypeArr);
 //
 vmPrntBK(pdiv);
 vmtstWriteDbArr(&Db,pdiv);
 //*
 memNULL(&Data);
 DataSetInt(Data);
 Data.intgr=100;
 err=mshSetArr(&Db,0,&Data);
 vmPrnt(pdiv,"***Установить размер массива=%li err=%hi",Data.intgr,err);
 vmPrntBK(pdiv);
 vmtstWriteDbArr(&Db,pdiv);
 //*
 Data.intgr=0;
 err=mshSetArr(&Db,0,&Data);
 vmPrnt(pdiv,"***Размер массива=%li err=%hi ",Data.intgr,err);
 vmtstWriteDbArr(&Db,pdiv);
 for (j=1;j<16;++j){
  Data.intgr=100+j;
  err=mshSetArr(&Db,j,&Data);
  }
 vmPrnt(pdiv,"***Записать данные в массив с 1-15");
 vmPrntBK(pdiv);
 vmtstWriteDbArr(&Db,pdiv);
 //*
 vmPrnt(pdiv,"***Команда Get*** 0,5,10,100");
 vmPrntBK(pdiv);
 for (j=0;j<4;++j){
  if ((pData=mshGetArr(&Db,Num[j],&err))!=NULL) {
   vmPrnt(pdiv," %li,",pData->intgr);
   }
  else {
   vmPrnt(pdiv," err=%d",err);
   }
  }
 vmPrntBK(pdiv);
 //*
 TIterNext Next;
 Next.pdb=&Db;
 Next.type=CcmdNext;
 Next.AdrOfferr.off=0;
 err=mshArrNextBackInit(&Next);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"***Команда initNext*** err=%d",err);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"[ %d ]=",Next.AdrOfferr.off);
 pData=mapArrpDataOffpDb(Next.pdb,Next.AdrOfferr.off);
 vmtstWriteData( pData,pdiv);
 vmPrnt(pdiv," { "); vmtstWriteKeyData(&(Next.val),pdiv);vmPrnt(pdiv," }");
 vmPrntBK(pdiv);
 while((err=mshArrNextBack(&Next))!=CMPnull){
  vmPrnt(pdiv,"***Команды Next*** err=%d  ",err);
  vmPrnt(pdiv,"[ %d ]=",Next.AdrOfferr.off);
  vmtstWriteData(mapArrpDataOff(Next.pdb,Next.AdrOfferr.off),pdiv);
  vmPrnt(pdiv," { "); vmtstWriteKeyData(&(Next.val),pdiv);vmPrnt(pdiv," }");
  vmPrntBK(pdiv);
  }
 //*
 Next.type=CcmdBack;
 Next.AdrOfferr.off=0;
 err=mshArrNextBackInit(&Next);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"***Команда initBack*** err=%d",err);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"[ %d ]=",Next.AdrOfferr.off);
 vmtstWriteData( mapArrpDataOff(Next.pdb,Next.AdrOfferr.off),pdiv);
 vmPrnt(pdiv," { "); vmtstWriteKeyData(&(Next.val),pdiv);vmPrnt(pdiv," }");
 vmPrntBK(pdiv);
 while((err=mshArrNextBack(&Next))!=CMPnull){
  vmPrnt(pdiv,"***Команды Back*** err=%d  ",err);
  vmPrnt(pdiv,"[ %d ]=",Next.AdrOfferr.off);
  vmtstWriteData(mapArrpDataOff(Next.pdb,Next.AdrOfferr.off),pdiv);
  vmPrnt(pdiv," { "); vmtstWriteKeyData(&(Next.val),pdiv);vmPrnt(pdiv," }");
  vmPrntBK(pdiv);
  }
 //*
 Next.AdrOfferr.off=0;
 Next.type=CcmdNext;
 vmPrnt(pdiv,"***Команды Next1*** ");
 vmPrntBK(pdiv);
 while((err=mshArrNextBack(&Next))!=CMPnull){
  vmPrnt(pdiv,"***Команды Next1*** err=%d  ",err);
  vmPrnt(pdiv,"[ %d ]=",Next.AdrOfferr.off);
  vmtstWriteData(mapArrpDataOff(Next.pdb,Next.AdrOfferr.off),pdiv);
  vmPrnt(pdiv," { "); vmtstWriteKeyData(&(Next.val),pdiv);vmPrnt(pdiv," }");
  vmPrntBK(pdiv);
  } 
 Next.type=CcmdBack;
 vmPrnt(pdiv,"***Команды Back1*** ");
 vmPrntBK(pdiv);
 while((err=mshArrNextBack(&Next))!=CMPnull){
  vmPrnt(pdiv,"***Команды Back1*** err=%d  ",err);
  vmPrnt(pdiv,"[ %ld ]=",Next.AdrOfferr.off);
  vmtstWriteData(mapArrpDataOff(Next.pdb,Next.AdrOfferr.off),pdiv);
  vmPrnt(pdiv," { "); vmtstWriteKeyData(&(Next.val),pdiv);vmPrnt(pdiv," }");
  vmPrntBK(pdiv);
  } 
//
//* 
 Tusize NumKill[]={2,1,5,13};
 for (j=0;j<4;++j){
  err=mshKillArr(&Db,NumKill[j]);
  vmPrnt(pdiv,"***Команда Kill*** %d err=%d",NumKill[j],err);
  vmPrntBK(pdiv);
  vmtstWriteDbArr(&Db,pdiv);
  }
 //*/
 //*
 j=maparrDbBusy(Db);
 vmPrnt(pdiv,"***Команда Kill*** %d err=%d busy=%d",1,err,j);
 while( (j=maparrDbBusy(Db)) ){
  err=mshKillArr(&Db,1);
  vmPrnt(pdiv,"***Команда Kill*** %d err=%d busy=%d",1,err,j);
  vmPrntBK(pdiv);
  vmtstWriteDbArr(&Db,pdiv);
  }
 //*/ //
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"освободить данные");
 vmPrntBK(pdiv);
 mshDbFree( (TDb*)(&Db) );
 }
/*
void test2() {
    printf("tstArrAPI test 2\n");
    printf("%%TEST_FAILED%% time=0 testname=test2 (tstArrAPI) message=error message sample\n");
}
*/
int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% tstArrAPI\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% test1 (tstArrAPI)\n");
    test1();
    printf("%%TEST_FINISHED%% time=0 test1 (tstArrAPI) \n");
/*
    printf("%%TEST_STARTED%% test2 (tstArrAPI)\n");
    test2();
    printf("%%TEST_FINISHED%% time=0 test2 (tstArrAPI) \n");
*/
    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
