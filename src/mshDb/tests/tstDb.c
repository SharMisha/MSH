/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstDb.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 25 ноября 2018 г., 20:04
 */

#include "mshDebag.h"

/*
 * Simple C Test Suite
 */
Tval* ValToInd(TDb *pDb,Tadr *pRoot,Tval *pVal,Terr *perr,int isData);
void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);
void vmtstWriteKeyData(Tval *pVal,TdivStream *pdiv);
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
Terr mshKill(TDb *pDb,TarrData *pInds,char tpK);
void mshDbFree(TDb *pDb);
Terr mshDbLocNew(TDb *pDb,Tstr *pName,char *pDir,enum TtypeDb typeDb);

void test1() {
 printf("tstDb test 1\n");
 //
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
 char sdir[]="$st\0  $stDb\0";
 Tstr *psname;
 //*
 psname=(Tstr*)(&(sdir[4]));
 psname->size=5;
 pstrKey=(Tstr*)(&bufKey[0]);
 pstrKey->size=35;
 pstrData=(Tstr*)(&bufData[0]);
 pstrData->size=35;
 arrInit(Inds,5);

 if ((err=mshDbLocNew((TDb*)(&Db),psname,sdir,EtypeDb) )<CERR) {
  printf("Ошибка err=%i",err);
  vmPrntBK(pdiv);
  return;
  }

 vmtstWriteTree(&Db,pdiv);
 printf("SizePg= %i CsizeLongDef=%i",Db.mpg.sizesec,Db.mlong.sizesec);
 vmPrntBK(pdiv);
 //*
 ValALLSetNULL(KeyData);
 DataSetInt(ValKey(KeyData));
 DataSetInt(ValData(KeyData));
 ValKey(KeyData).intgr=0;
 ValData(KeyData).intgr=100;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteKeyData(pKeyData,pdiv);

 vmtstWriteTree(&Db,pdiv);
 //*
 vmPrnt(pdiv,"*****записать данные*******");
 vmPrntBK(pdiv);
 for (j=0;j<6;j++) {
  ValKey(KeyData).intgr +=10;
  ValData(KeyData).intgr++;
  pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
  vmtstWriteKeyData(pKeyData,pdiv);
  }
 vmtstWriteTree(&Db,pdiv);
//*
 ValKey(KeyData).intgr =45;
 ValData(KeyData).intgr=245;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteKeyData(pKeyData,pdiv);
// vmtstWriteTree(&Db,pdiv);
 //*
 ValKey(KeyData).intgr =25;
 ValData(KeyData).intgr=225;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteKeyData(pKeyData,pdiv);
// vmtstWriteTree(&Db,pdiv);
 ValKey(KeyData).intgr =35;
 ValData(KeyData).intgr=235;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteKeyData(pKeyData,pdiv);
// vmtstWriteTree(&Db,pdiv);
 //*
 DataSetpStr(ValKey(KeyData));
 DataSetpStr(ValData(KeyData));
 ValKey(KeyData).pstr=pstrKey;
 ValData(KeyData).pstr=pstrData;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteKeyData(pKeyData,pdiv);
 DataSetInt(ValKey(KeyData));
 DataSetInt(ValData(KeyData));
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
//*
 pData=&(ValKey(KeyData));
 arrPushpDan(Inds,pData);
 vmPrnt(pdiv,"*****получить данные*******");
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
 vmPrnt(pdiv,"   Чтение строки  ");
 vmPrnt(pdiv,"{");
 pDataSetpStr(pData);
 pData->pstr=pstrKey;
 pKeyData=mshGet(&Db,&Inds,&err);
 vmtstWriteKeyData(pKeyData,pdiv);
 vmPrnt(pdiv,"}err=%d",err);
 vmPrntBK(pdiv);
 pDataSetInt(pData);
// vmtstWriteTree(&Db,pdiv);
 vmPrntBK(pdiv);
 //*
 vmPrnt(pdiv,"********Удалить данные*****60");
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"удалить=%d",60);
 pData->intgr=60;
 err=mshKill(&Db,&Inds,CtypeKiLL);
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
//*
 vmPrnt(pdiv,"записать данные [27]=227");
 ValKey(KeyData).intgr =27;
 ValData(KeyData).intgr=227;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteTree(&Db,pdiv);
 vmPrnt(pdiv,"***заменить данные ***[30]=330");
 ValKey(KeyData).intgr =30;
 ValData(KeyData).intgr=330;
 pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
 vmtstWriteTree(&Db,pdiv);
 //*
 vmPrnt(pdiv,"********Удалить данные*****");
 vmPrntBK(pdiv);
 int y[]={0,25,30,40,27,20,10};
 for(j=0;j<6;j++) {
  vmPrnt(pdiv," %d,",y[j]);
  pData->intgr=y[j];
  err=mshKill(&Db,&Inds,CtypeKiLL);
//  vmtstWriteTree(&Db,pdiv);
  }
 vmtstWriteTree(&Db,pdiv);
 vmPrnt(pdiv,"удалить=%d",y[6]);
 pData->intgr=y[6];
 err=mshKill(&Db,&Inds,CtypeKiLL);
 vmtstWriteTree(&Db,pdiv);
 //*
 pDataSetpStr(pData);
 pData->pstr=pstrKey;
 err=mshKill(&Db,&Inds,CtypeKiLL);
 vmPrnt(pdiv,"********Удалить данные строку******** err=%d",err);
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 //*
 pDataSetInt(pData);
 vmPrnt(pdiv,"********Вставить данные****0,40,20,60,10,30,50,55,51,5");
  vmPrntBK(pdiv);
 int ys[]={0,40,20,60,10,30,50,55,51,5};
 for(j=0;j<10;j++) {
// for(j=0;j<5;j++) {
  vmPrnt(pdiv,"[%d], ",ys[j]);
  ValKey(KeyData).intgr=ys[j];
  ValData(KeyData).intgr=400+ys[j];
  pKeyData=ValToInd(&Db,NULL,&KeyData,&err,isData);
  }
 vmPrntBK(pdiv);
 vmtstWriteTree(&Db,pdiv);
 //
 //*
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
 ///
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"освободить данные");
 vmPrntBK(pdiv);
 arrFree(Inds);
 mshDbFree( (TDb*)(&Db) );
 }

void test2() {
 printf("tstDb test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstDb) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstDb\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% tstDb\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 tstDb \n");
/*
 printf("%%TEST_STARTED%% test2 (tstDb)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstDb) \n");
*/
 printf("%%SUITE_FINISHED%% time=0 tstDb\n");

 return (EXIT_SUCCESS);
}
