// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
 * File:   mshPg.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 20 Август 2014 г., 12:11
 */

#include "mshDb.h"
#include "mshData.h"
#include "mshPgLong.h"
#include "mshErr.h"


Terr DataCmpNum(Tdata *pkey1, Tdata *pkey2);
void AreaToLong(TDb *pDb, Tdata *pData);
Terr UpdateDataLong(TmapLong *pmLong,Tdata *pDataNew,Tdata *pDataDb);
void moveTohis(TmapLong *pmLong, Tdata *pData);
Terr LongDelInd(Tlong *pLong,Tsize ind);
void bStrGet(Tdata *pData,Tbstr* pbstrOut);
/* +
 *     сравнение 2х полей данных
 *             key1== key2 то CMPeq
 *             key1 > key2 то  CMPup
 *             key1 < key2 то  CMPdown
 */
Terr pDbpDataCmp(TDb *pDb,Tdata *pkey1, Tdata *pkeyDb){
 Terr err;
 if ((err=DataCmpNum(pkey1,pkeyDb)) != CMPnull) return err;
//  *   оба ключа строки ??? могут быть не форматные данные сравнивать побайтно ?
 Tbstr bkey1,bkey2;
 Tstr *pskey;
 //
 bStrGet(pkey1,&bkey1);
 if (pDataIsLong(pkeyDb)){
  pskey=(Tstr*)(pgLongpStr(getLongOfmLong( pDb->mlong,(pkeyDb)->dataLongAdr),(pkeyDb)->dataLongOff));
  return StrCmrpStr(bkey1,pskey);
  }
 bStrGet(pkeyDb,&bkey2);
 return (bkey2.psec)?(StrCmrStr(bkey1,bkey2)):CMPup;
 }
/*
 * Инициализировать страницу
 */
void PgInit(TDb *pDb,TuPg *puPg,char Type) {
 puPg->pg->type=Type;
 switch (Type) {
  case CTypePgData :
   AdrSetNULL(puPg->pgD->left);
   AdrSetNULL(puPg->pgD->right);
   puPg->pgD->size= SizePgToSize(pDb,TpgData,puPg->pgD);
   break;
  case CTypePgLink : puPg->pgL->size=SizePgToSize(pDb,TpgLink,puPg->pgL);
   break;
  case CTypePgFree : puPg->pgF->size=SizePgToSize(pDb,TpgFree,puPg->pgF);
   break;
  }
 PgClear(puPg->pg);
 }
/*
 * найти Ключ на странице pgLink
 */
//#define pLongsTopgKey(pDb,pgKey) if ( DataIsLong(pValKey(pgKey)) ) pValKey(pgKey).datapmLong=&(pDb->mlong)

#define findKeyLinkFor(pDb,pKey,pgKey,pgEnd,err) for (;(pgKey) < pgEnd;(pgKey)++){\
  if ((err= pDbpDataCmp((TDb*)(pDb),pKey, &(pValKey(pgKey)) ) )==CMPeq) return pgKey; if (err == CMPdown) return pgKey-1;}

static Tkey* PgLinkFindKey(TDb *pDb,TpgLink *Pg,Tdata *pKey,Terr *perr){
 if (Pg->busy<1) {*perr=CWARdataNotFound;return NULL;}
 Tkey *pgKey;
 Tkey *pgEnd;
 int beg,end,cur,d;
 //сравниваем 0-ключ
 pgKey=pgOffpSec(Pg,0);
 if (pKey==NULL) return  pgKey;
 if ((*perr= pDbpDataCmp((TDb*)pDb,pKey, &(pValKey(pgKey)) ) )==CMPeq) return  pgKey;
 if (*perr==CMPdown) {*perr=CkeyFirst;return pgKey;}
 if (Pg->busy==1) {*perr=CMPup;return pgKey;}
 //сравниваем последний ключ
 pgEnd=pgOffpSec(Pg,Pg->busy-1);
 if (((*perr=pDbpDataCmp((TDb*)pDb,pKey, &(pValKey(pgEnd))) )==CMPeq)||(*perr==CMPup)) return pgEnd;
 if (Pg->busy==2) {*perr=CMPdown;return pgKey;} // pgEnd-1;
 //ключей мало ищем подряд
 if (Pg->busy < CsumFindKey){
  pgKey=Pg->psec+1;
  findKeyLinkFor(pDb,pKey,pgKey,pgEnd,*perr)
  *perr=CMPdown;
  return pgKey-1;
  }
 //ключей много ищем половинным делением
 beg=0;
 end=Pg->busy-1;
 do{
  d=(end-beg)/2;
  cur=d + beg;
  pgKey=pgOffpSec(Pg,cur);
  if ((*perr= pDbpDataCmp((TDb*)pDb,pKey, &(pValKey(pgKey)) ) )==CMPeq) return pgKey;
  if (*perr==CMPup) beg=cur; else end=cur;
  }while(d>CsumFindKey);
 pgKey=pgOffpSec(Pg,beg+1);
 pgEnd=pgOffpSec(Pg,end);
 findKeyLinkFor(pDb,pKey,pgKey,pgEnd,*perr)
 *perr=CMPdown;
 return pgKey-1;
 }
/*
 * Спуск по страницам TpgLink с заданным Ключом
 */
TpgData* PgLinkFindAdr(TDb *pDb,Tadr *pAdr,Tdata *pKey,Terr *perr) {
 Tkey *pgKey;
 TuPg uPg;
 //спуск по страницам pgLink
 uPg.pvoid = AdrToPg(pDb,*pAdr);
 if (uPg.pg->busy<1) {
  *perr=CERRnotFoundPgData;
  return NULL;
  }
 while ((TypePgIsLink(uPg.pg))&&(uPg.pg->busy)){
  pgKey=PgLinkFindKey(pDb,uPg.pgL,pKey,perr);
  if (*perr==CkeyFirst) return NULL;
  *pAdr=pValAdr(pgKey);
  uPg.pvoid = AdrToPg(pDb,*pAdr);
  }
 *perr=COutKodOk;
 return uPg.pgD;
 }
/*
 * Спуск по страницам TpgLink с заданным Ключом с созданием стека Адресов
 * Tadr* LocSteckDown(TDbLoc *pDb,Tadr *pAdr,Tdata *pKey,Terr *perr)
 */
Tadr getSteckDown(TDb *pDb,Tadr Adr,Tdata *pKey,Terr *perr) {
 Tkey *pgKey;
 TuPg uPg;
 //спуск по страницам pgLink
 uPg.pvoid = AdrToPg(pDb,Adr);
 if (uPg.pg->busy<1) {*perr=CWARdataNotFound;return CnotAdr;}
 while (TypePgIsLink(uPg.pg)){
  if ((pgKey=PgLinkFindKey(pDb,uPg.pgL,pKey,perr))==NULL) {*perr=CWARdataNotFound;return CnotAdr;}
  pDbSteckBusy(pDb);
  pDb->trackdb.ptop->off=OffSec(uPg.pgL,pgKey);
  pDb->trackdb.ptop->adr=Adr;
  pDb->trackdb.ptop->err=*perr;
  pDb->trackdb.ptop->isalt=0;
  Adr=pValAdr(pgKey);
  uPg.pvoid = AdrToPg(pDb,Adr);
  }
 *perr=COutKodOk;
 return Adr;
 }

#define findKeyFor(pDb,pKey,pgKey,pgEnd,err) for (;(pgKey) < pgEnd;(pgKey)++){ \
  if ( ((err= pDbpDataCmp((TDb*)pDb,pKey, &(pValKey(pgKey)) ) )==CMPeq)||(err == CMPdown) )  return pgKey;}

/*
 * найти Ключ на странице
 *  выход: ключ == CMPeq
 *         ключ < CMPdown
 *         ключ > CMPup
 */
Tval* PgFindKey(TDb *pDb,TpgData *Pg,Tdata *pKey,Terr *perr){
 Tval *pgKey;
 Tval *pgEnd;
 int beg,end,cur,d;
 //сравниваем 0-ключ
 if (Pg->busy<1) {*perr=CWARdataNotFound;return NULL;}
 pgKey=pgOffpSec(Pg,0);
 if (pKey==NULL) return pgKey;
 //
 if ((*perr=pDbpDataCmp((TDb*)pDb,pKey, &(pValKey(pgKey)) ) )==CMPeq) return  pgKey;
 if (*perr==CMPdown) {*perr=CkeyFirst;return pgKey;}
 if (Pg->busy==1) {*perr=CMPup;return pgKey;}
 //сравниваем последний ключ
 pgEnd=pgOffpSec(Pg,Pg->busy-1);
 if (((*perr= pDbpDataCmp((TDb*)pDb,pKey, &(pValKey(pgEnd)) ) )==CMPeq)||(*perr==CMPup)) return pgEnd;
 if (Pg->busy==2) {*perr=CMPdown;return pgEnd;}
 //ключей мало ищем подряд
 if (Pg->busy<CsumFindKey){
  pgKey=Pg->psec+1;
  findKeyFor(pDb,pKey,pgKey,pgEnd,*perr)
  *perr=CMPdown;
  return pgKey;
  }
 //ключей много ищем половинным делением
 beg=0;
 end=Pg->busy-1;
 do{
  d=(end-beg)/2;
  cur=d + beg;
  pgKey=pgOffpSec(Pg,cur);
  if ( (*perr=pDbpDataCmp((TDb*)pDb,pKey, &(pValKey(pgKey)) ) )==CMPeq ) return pgKey;
  if (*perr==CMPup) beg=cur; else end=cur;
  }while(d>CsumFindKey);
 pgKey=pgOffpSec(Pg,beg+1);
 pgEnd=pgOffpSec(Pg,end);
 findKeyFor(pDb,pKey,pgKey,pgEnd,*perr)
 *perr=CMPdown;
 return pgKey;
 }
/*
 * Спуск по страницам TpgLink по правому краю
 */
TpgData* PgDownR(TDb *pDb,Tadr *pAdr) {
 TuPg uPg;
 //спуск по страницам pgLink
 uPg.pvoid = AdrToPg(pDb,*pAdr);
 while ((TypePgIsLink(uPg.pg))&&(uPg.pg->busy)){
  *pAdr=pValAdr(pgOffpSec(uPg.pgL,uPg.pg->busy-1));
  uPg.pvoid = AdrToPg(pDb,*pAdr);
  }
 return (uPg.pg->busy)?uPg.pgD:NULL;
 }
/*
 * Спуск по страницам TpgLink по левому краю
 */
TpgData* PgDownL(TDb *pDb,Tadr *pAdr) {
 TuPg uPg;
 //спуск по страницам pgLink
 uPg.pvoid = AdrToPg(pDb,*pAdr);
 while ((TypePgIsLink(uPg.pg))&&(uPg.pg->busy)){
  *pAdr=pValAdr(uPg.pgL->psec);
  uPg.pvoid = AdrToPg(pDb,*pAdr);
  }
 return (uPg.pg->busy)?uPg.pgD:NULL;
 }
static TpgData* PgLinkUpFindAdr(TDb *pDb,Tadr *pAdr,Tdata *pKey,Terr *perr) {
 Tkey *pgKey;
 TuPg uPg;
 //спуск по страницам pgLink
 uPg.pvoid = AdrToPg(pDb,*pAdr);
 if (uPg.pg->busy<1) {*perr=CWARdataNotFound;pAdrSetNULL(pAdr);return NULL;}
 while ((TypePgIsLink(uPg.pg))&&(uPg.pg->busy)){
  if ( (pgKey=PgLinkFindKey(pDb,uPg.pgL,pKey,perr))==NULL ) {
   *perr=CWARdataNotFound;pAdrSetNULL(pAdr);return NULL;
   }
  *pAdr=pValAdr(pgKey);
  uPg.pvoid = AdrToPg(pDb,*pAdr);
  }
 *perr=COutKodOk;
 return uPg.pgD;
 }
//
Tval* PgKeyUp(TDb *pDb,Tadr *pAdr,Tdata *pKey,Toff *poff,Terr *perr) {
 TpgData *pPgD;
 Tval *pVal;
 //спуск по страницам pgLink
 pPgD=PgLinkUpFindAdr(pDb,pAdr,pKey,perr);
 if ((pPgD==NULL)||(*perr<CERR)) {
  return NULL;
  }
 pVal = PgFindKey(pDb,pPgD,pKey,perr);
 if ((pVal==NULL)||(*perr<CERR)) {  return NULL;  }
 *poff=OffSec(pPgD,pVal);
 switch (*perr) {
  case CkeyFirst :
  case CMPdown :
   return pVal;
  case CMPeq :
   if (*poff < (pPgD->busy-1)){
    (*poff)++;
    return pgOffpSecData(pPgD,*poff);
    }
  case CMPup :
   *pAdr=pPgD->right;
   if ( AdrIsNoNULL(pPgD->right) ) {
    pPgD =(TpgData*)(AdrToPg(pDb,pPgD->right));
    *poff=0;
    if (pPgD->busy) return pgOffpSec(pPgD,0);
    }
  }
 return NULL;
 }

Tval* PgKeyDown(TDb *pDb,Tadr *pAdr,Tdata *pKey,Toff *poff,Terr *perr) {
 TpgData *pPgD;
 Tval *pVal;
 //спуск по страницам pgLink
 pPgD=PgLinkFindAdr(pDb,pAdr,pKey,perr);
 if (pPgD==NULL) return NULL;
 pVal = PgFindKey(pDb,pPgD,pKey,perr);
 if ((pVal==NULL)||(*perr<CERR)) {
  return NULL;
  }
 *poff=OffSec(pPgD,pVal);
 switch (*perr) {
  case CMPup :
   return pVal;
  case CMPeq :
   if (*poff){
    (*poff)--;
    return pgOffpSecData(pPgD,*poff);
    }
  case CkeyFirst :
  case CMPdown :
   *pAdr=pPgD->left;
   if ( AdrIsNoNULL(pPgD->left) ) {
    pPgD =(TpgData*)(AdrToPg(pDb,pPgD->left));
    *poff=pPgD->busy-1;
    if (pPgD->busy) {
     return pgOffpSec(pPgD,*poff);
     }
    }
  }
 return NULL;
 }
/*
 * добавить поле на страницу
 */
/*
 * добавить поле на страницу и поместить туда значение
 * ValToPg(pDb,pgData,pVal);
 */

Tval* ValAddPg(TDb *pDb, TpgData *pPg,Tval *pVal,int isData,Terr *perr){
 Tval *pgKey;
 //
 if (pPg->size<=pPg->busy) {*perr =CERRPgFull;return NULL;}
 pgKey=pPg->psec + pPg->busy++;
 *pgKey=*pVal;
 moveTohis(&(pDb->mlong),&(pValKey(pgKey)));
 if (isData)
  moveTohis(&(pDb->mlong),&(pValData(pgKey)));
 else{
  DataSetNULL(pValData(pgKey));
  }
 *perr =COutKodOk;
 return pgKey;
 }
/*
 * вставить Ключ и данные на страницу
 */
Tval* ValInsToPg(TDb *pDb, TpgData *Pg, Tval *pVal, int isData, Terr *perr) {
 Tval *pgKey;
 int n;
 Terr err;
 //
 if (Pg->busy<1) return ValAddPg(pDb,Pg,pVal,isData,perr);
 pgKey=PgFindKey(pDb,Pg,&(pValKey(pVal)),perr);
 if (*perr<CERR) return NULL;
 switch(*perr){
  //добавляем минимальный ключ
  case CkeyFirst:
   if (Pg->size <= Pg->busy) {
    *perr = CkeyFirstFull;
    return pgKey;
    }
   goto down;
  case CMPdown:
   if (Pg->size <= Pg->busy) return pgKey;
   *perr =COutKodOk;
   goto down;
  //добавляем максимальный ключ
  case CMPup:
   return (Pg->size > Pg->busy) ? ValAddPg(pDb,Pg,pVal,isData,perr) : NULL;
  case CMPeq:
   if (isData) err = UpdateDataLong(&(pDb->mlong), &(pValData(pVal)), &(pValData(pgKey)));
   *perr =COutKodOk;
   return pgKey;
  }
 goto finish;
down: 
 n=(Pg->psec + Pg->busy - pgKey) * sizeof(Tval);
 memMove(pgKey + 1, pgKey, n);
 pValALLSetNULL(pgKey);
 Pg->busy++;
finish:
 *pgKey=*pVal;
 moveTohis(&(pDb->mlong),&(pValKey(pgKey)));
 if (isData)
  moveTohis(&(pDb->mlong),&(pValData(pgKey)));
 else{
  DataSetNULL(pValData(pgKey));
  }
 return pgKey;
 }
