/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstDbGlb.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 25 ноября 2018 г., 20:06
 */

#include "mshDebag.h"

/*
 * Simple C Test Suite
 */
Terr mshgetDb(TDb *pDb,Tstr *pName,const char *pDir,enum TisSet dbisSet,int sizePartGlb,enum TtypeDb typeDb);
Tval* ValToInd(TDb *pDb,Tadr *pRoot,Tval *pVal,Terr *perr,int isData);
void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);
void vmtstWriteKeyData(Tval *pVal,TdivStream *pdiv);
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
Terr mshKill(TDb *pDb,TarrData *pInds,char tpK);
void mshDbFree(TDb *pDb);

void test1() {
 printf("tstDbGlb test 1\n");
 TdivStream *pdiv=NULL;
 TDb Db;
 Tval KeyData,*pKeyData;
 Tdata *pData;
 int isData=1;
 int j;
 Terr err;
 Tstr *pstrKey,*pstrData;
 char bufKey[]="00KEY__ABCDEFGHIJ01234567890123456789";
 char bufData[]="00DATA_ABCDEFGHIJ01234567890123456789";
 TarrData Inds;
 char sdir[]="tst\0  tstDbGlb\0";
 Tstr *psname;
 //
 arrInit(Inds,5);
 psname=(Tstr*)(&(sdir[4]));
 psname->size=8;
 err=mshgetDb((TDb*)(&Db),psname,sdir,1,0,EtypeDb);
//
 pstrKey=(Tstr*)bufKey;
 pstrKey->size=35;
 pstrData=(Tstr*)bufData;
 pstrData->size=35;

 vmPrnt(pdiv,"*****0.исходное состояние*******\n");
// vmtstWriteTree(&Db,pdiv);
//*
 vmPrnt(pdiv,"*****1.состояние*******\n");
 ValALLSetNULL(KeyData);
 DataSetInt(ValKey(KeyData));
 DataSetInt(ValData(KeyData));
 ValKey(KeyData).intgr=0;
 ValData(KeyData).intgr=100;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteTree(&Db,pdiv);
 //*
 vmPrnt(pdiv,"*****записать данные*******");
 vmPrnt(pdiv,"*****2.состояние*******\n");
 vmPrntBK(pdiv);
 for (j=0;j<6;j++) {
  ValKey(KeyData).intgr +=10;
  ValData(KeyData).intgr++;
  pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
  }
// vmtstWriteTree(&Db,pdiv);
 //*
 vmPrnt(pdiv,"*****3.состояние*******\n");
 ValKey(KeyData).intgr =45;
 ValData(KeyData).intgr=245;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteTree(&Db,pdiv);
 vmPrnt(pdiv,"*****4.состояние*******\n");
 ValKey(KeyData).intgr =25;
 ValData(KeyData).intgr=225;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
// vmtstWriteTree(&Db,pdiv);
 vmPrnt(pdiv,"*****5.состояние*******\n");
 ValKey(KeyData).intgr =35;
 ValData(KeyData).intgr=235;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
// vmtstWriteTree(&Db,pdiv);
 vmPrnt(pdiv,"***** 5,5.состояние*******\n");
 //*
 DataSetpStr(ValKey(KeyData));
 DataSetpStr(ValData(KeyData));
 ValKey(KeyData).pstr=pstrKey;
 ValData(KeyData).pstr=pstrData;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
// vmtstWriteTree(&Db,pdiv);
 //*
 vmPrnt(pdiv,"*****6.состояние*******\n");
 DataSetInt(ValKey(KeyData));
 DataSetInt(ValData(KeyData));
 vmPrntBK(pdiv);
// vmtstWriteTree(&Db,pdiv);
//*
 vmPrnt(pdiv,"*****получить данные*******");
 pData=&(ValKey(KeyData));
 arrPushpDan(Inds,pData);
 vmPrntBK(pdiv);
 pData=Inds.psec;
 for (j=0;j<12;j++) {
  vmPrnt(pdiv,"{%d=",j);
  pData->intgr=j*10;
  pKeyData=mshGet(&Db,&Inds,&err);
  vmtstWriteKeyData(pKeyData,pdiv);
  vmPrnt(pdiv,"}err=%d",err);
  if (((j +1) % 3)==0) {vmPrntBK(pdiv);}
  }
 vmPrntBK(pdiv);
//*
 vmPrnt(pdiv,"{");
 pDataSetpStr(pData);
 pData->pstr=pstrKey;
 pKeyData=mshGet(&Db,&Inds,&err);
 vmtstWriteKeyData(pKeyData,pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"*****7.состояние*******");
 vmPrnt(pdiv,"}err=%d",err);
 vmPrntBK(pdiv);
 pDataSetInt(pData);
// vmtstWriteTree(&Db,pdiv);
 vmPrntBK(pdiv);
 //*
 vmPrnt(pdiv,"********Удалить данные*****60");
 vmPrnt(pdiv,"*****8.состояние*******");
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"удалить=%d",60);
 pData->intgr=60;
 err=mshKill(&Db,&Inds,CtypeKiLL);
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
//*
 vmPrnt(pdiv,"записать данные [27]=227");
 vmPrnt(pdiv,"*****9.состояние*******\n");
 ValKey(KeyData).intgr =27;
 ValData(KeyData).intgr=227;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteTree(&Db,pdiv);
 vmPrnt(pdiv,"***заменить данные ***[30]=330");
 vmPrnt(pdiv,"*****10.состояние*******\n");
 ValKey(KeyData).intgr =30;
 ValData(KeyData).intgr=330;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteTree(&Db,pdiv);
 //*
 vmPrnt(pdiv,"********Удалить данные*****");
 vmPrnt(pdiv,"*****11.состояние*******\n");
 int y[]={0,25,30,40,27,20,10};
 for(j=0;j<7;j++) {
  vmPrnt(pdiv," %d",y[j]);
  pData->intgr=y[j];
  err=mshKill(&Db,&Inds,CtypeKiLL);
  }
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 //*
 vmPrnt(pdiv,"********Удалить строковые данные********");
 vmPrnt(pdiv,"*****12.состояние*******\n");
 pDataSetpStr(pData);
 pData->pstr=pstrKey;
 err=mshKill(&Db,&Inds,CtypeKiLL);
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 //*
 pDataSetInt(pData);
 vmPrnt(pdiv,"********Вставить данные****");
 vmPrntBK(pdiv);
 int ys[]={0,40,20,60,10,30,50,55,51,5};
 for(j=0;j<10;j++) {
// for(j=0;j<1;j++) {
  vmPrnt(pdiv," %d, ",ys[j]);
  ValKey(KeyData).intgr=ys[j];
  ValData(KeyData).intgr=400+ys[j];
  pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
//  vmPrntBK(pdiv);
//  vmtstWriteTree(&Db,pdiv);
  }
 vmtstWriteTree(&Db,pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"*****13.состояние*******\n");
 ///*
 vmPrnt(pdiv,"********Замена данных********");
 vmPrntBK(pdiv);
 int ya[]={0,40,20,60,10,30,50,55,51,5}; for(j=0;j<10;j++) {
  vmPrnt(pdiv,"%d, ",ya[j]);
  ValKey(KeyData).intgr=ya[j];
  ValData(KeyData).intgr=500+ys[j];
  pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
  }
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 vmPrnt(pdiv,"*****14.состояние*******\n");
 //*/
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"освободить данные");
 vmPrntBK(pdiv);
 arrFree(Inds);
 mshDbFree( (TDb*)(&Db) );
 //*/
 }

void test2() {
 printf("tstDbGlb test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstDbGlb) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstDbGlb\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstDbGlb)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstDbGlb) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstDbGlb)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstDbGlb) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
