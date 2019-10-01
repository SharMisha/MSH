/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstPgLong.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 25 ноября 2018 г., 20:08
 */
#include "mshDebag.h"


Tstr* tstLongAdd(Tlong *pLong,Tsize size,Tsize *pOff,Terr *perr);
void vmtstWriteSecLongs(TmapLong *pmapLong,TdivStream *pdiv);
void vmtstWriteStr(Tstr* pstr,TdivStream *pdiv);
void vmtstWriteAdrOff(TadrOff *pAdrOff,TdivStream *pdiv);
Terr LongDelInd(Tlong *pLong,Tsize ind);
Terr tstLongCompressed(Tlong *pLong);
Terr tstLongClear(Tlong *pLong,Tsize ind);
Tstr* tstLongIns(Tlong *pLong,Tsize ind,Tsize size,Tsize *pOff,Terr *perr);
Tstr* getStrOfLongPg(TDb *pDb,Tsize SizeChar,Tadr *pAdr,Tsize *pOff,Terr *perr);
Tsize tstLongSize(Tlong *pLong,Tsize Off);

/*
 * Simple C Test Suite
 */
#define tstLongInsStr(pLong,ind,pStrIn,pOff) ({Tstr *pStr;Tsize Size;Terr err; Size=mshDivUp(pStrSize(pStrIn),sizeof(Tsize));\
 if ((pStr=tstLongIns(pLong,ind,Size,pOff,&err))!=NULL){ pStrTopStr(pStr,pStrIn); } err; })


void test1() {
 printf("tstPgLong test 1\n");
 TdivStream *pdiv=NULL;
 Tlong *pLong,*pTop;
 Tstr *pstr,*pstrL,*pstrS,*pStrTemp,*pStr;
 char buf[]="00ABCDEFGHIJ01234567890123456789";
 char bufL[]="00Long0ABCDEFGHIJ01234567890123456789";
 char bufS[]="00ShortABCDEFGHIJ0123456789";
 Terr err;
 int n;
 Tsize size;
 TadrOff adrOff;
 TDb Db,*pDb;
 int indLong=0,indTop;
 char sdir[]="tst\0  tstPgLong\0";
 Tstr *psname;
 //
 AdrOffSetNULL(adrOff);
 psname=(Tstr*)(&(sdir[4]));
 psname->size=strlen(&(sdir[6]));
 pstr=(Tstr*)buf;
 pstr->size=30;
 pstrL=(Tstr*)bufL;
 pstrL->size=35;
 pstrS=(Tstr*)bufS;
 pstrS->size=25;
 pDb=&Db;
 if ((err=mshDbLocNew(pDb,psname,sdir,EtypeLong) )<CERR) {
  printf("Ошибка err=%d\n",err);
  return;
  }
 pLong=(Tlong*)(Db.mlong.psec);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"Пустая страница");
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 vmPrntBK(pdiv);
//*
 size=mshDivUp(pStrSize(pstr),sizeof(Tsize));
 pStrTemp=tstLongAdd(pLong,size,NULL,&err);
 pStrTopStr(pStrTemp,pstr);
 vmPrnt(pdiv,"добавить строку на страницу-%d",err);
 vmPrntBK(pdiv);
 vmtstWriteStr(pstr,pdiv);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 //*
 for (n=0;n<9;n++) {
  pstr->psec[0]='0'+n;
  //Tstr* getStrOfLongPg(TmapLong *pmlong ,Tsize SizeStr,Tadr *pAdr,Tuint4 *pOff,Terr *perr)
  if ((pStr=  getStrOfLongPg( &(pDb->mlong) ,pstr->size,&(adrOff.adr),&(adrOff.off),&err) )){ pStrTopStr(pStr,pstr); }
  }
 vmPrntBK(pdiv);
 pTop=Db.mlong.psec+Db.mlong.size-Db.mlong.sizesec;
 indTop=((void*)pTop - Db.mlong.psec)/Db.mlong.sizesec;
 vmPrnt(pdiv,"страница pLong %p= indLong= %d",pLong,indLong);
 vmPrnt(pdiv,"   страница pTop=%p indTop= %d",pTop,indTop);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"добавить строки в LongMap %p",Db.mlong.psec);
 vmPrntBK(pdiv);
 vmtstWriteAdrOff(&adrOff,pdiv);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 //*
 adrOff.off=1;
 err=LongDel(&(Db.mlong),adrOff.adr,adrOff.off);
 vmPrnt(pdiv,"err=%d Удалить строку# %d из секции %lu ",err,adrOff.off,adrOff.adr / Db.mlong.sizesec);
 vmPrntBK(pdiv);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 
 n=tstLongSize(pLong,2);
 vmPrnt(pdiv,"страница pLong %p= off= %d size=%d",pLong,2,n);
 vmPrntBK(pdiv);
 n=tstLongSize(pTop,2);
 vmPrnt(pdiv,"страница pTop %p= off= %d size=%d",pTop,2,n);
 vmPrntBK(pdiv);

 //*
 n=1; //LongInsStr(pLong,ind,pStrIn,pOff)
 err=tstLongInsStr(pLong,n,pstrL,&(adrOff.off));
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d заменить строку на более длинную# %d в секции %d Off=%d"
         ,err,n,indLong,adrOff.off);
 vmPrntBK(pdiv);
 vmtstWriteStr(pstrL,pdiv);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 //*
 err=tstLongInsStr(pTop,n,pstrS,&(adrOff.off));
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d заменить строку на более короткую# %d в секции %d Off=%d"
         ,err,n,indTop,adrOff.off);
 vmPrntBK(pdiv);
 vmtstWriteStr(pstrS,pdiv);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 //*
 n=0;
 err=tstLongInsStr(pTop,n,pstrL,&(adrOff.off));
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d заменить строку на более длинную# %d в секции %d Off=%d"
         ,err,n,indTop,adrOff.off);
 vmPrntBK(pdiv);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 //*
 n=4;
 err=tstLongInsStr(pTop,n,pstrL,&(adrOff.off));
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d заменить строку на более длинную# %d в секции %d Off=%d"
         ,err,n,indTop,adrOff.off);
 vmPrntBK(pdiv);
 vmtstWriteStr(pstrL,pdiv);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 //*
 n=3;
 pstr->psec[0]='@';
 err=tstLongInsStr(pTop,n,pstr,&(adrOff.off));
 vmPrntBK(pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d заменить строку на равную# %d в секции %d Off=%d"
         ,err,n,indTop,adrOff.off);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 //*
 n=1;
 err=tstLongClear(pLong,n);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d очистить данные #%d в секции %d "
         ,err,n,indLong);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 //*
 n=4;
 err=tstLongClear(pLong,n);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d очистить последние данные #%d в секции %d "
         ,err,n,indLong);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
//*
 n=0;
 err=tstLongClear(pLong,n);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d очистить данные #%d в секции %d "
         ,err,n,indLong);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
//*
adrOff.off=1;
err=LongDel(&(Db.mlong),adrOff.adr,adrOff.off);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d Удалить строку# %d из секции %lu "
         ,err,adrOff.off,adrOff.adr / Db.mlong.sizesec);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
//*
 err=tstLongCompressed(pTop);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"err=%d сжать секцию %d "
         ,err,indTop);
 vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 
 n=tstLongSize(pLong,2);
 vmPrnt(pdiv,"страница pLong %p= off= %d size=%d",pLong,2,n);
 vmPrntBK(pdiv);
 n=tstLongSize(pTop,2);
 vmPrnt(pdiv,"страница pTop %p= off= %d size=%d",pTop,2,n);
 vmPrntBK(pdiv);
 n=tstLongSize(pTop,4);
 vmPrnt(pdiv,"страница pTop %p= off= %d size=%d",pTop,4,n);
 vmPrntBK(pdiv);
 //*/
 vmPrnt(pdiv,"освободить данные");
 vmPrntBK(pdiv);
 mshDbFree( (TDb*)(&Db) );
 }

void test2() {
 printf("tstPgLong test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstPgLong) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstPgLong\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstPgLong)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstPgLong) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstPgLong)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstPgLong) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
