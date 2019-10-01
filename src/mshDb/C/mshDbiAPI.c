/*
 * File:   mshDbiAPI.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 20 Август 2014 г., 12:15
 */

#include "mshDb.h"
#include "mshData.h"
#include "mshPgLong.h"
#include "mshPack.h"
#include "mshErr.h"

Tval* PgFindKey(TDb *pDb,TpgData *Pg,Tdata *pKey,Terr *perr);
TpgData* PgLinkFindAdr(TDb *pDb,Tadr *pAdr,Tdata *pKey,Terr *perr);
Tval* PgKeyUp(TDb *pDb,Tadr *pAdr,Tdata *pKey,Toff *poff,Terr *perr);
Tval* PgKeyDown(TDb *pDb,Tadr *pAdr,Tdata *pKey,Toff *poff,Terr *perr);
Tval* ValToInd(TDb *pDb,Tadr *pRoot,Tval *pVal,Terr *perr,int isData);
Tadr getSteckDown(TDb *pDb,Tadr Adr,Tdata *pKey,Terr *perr);
Terr DelpgData(TDb *pDb, Tadr Adr);
Terr PgToFree(TDb *pDb, Tadr AdrPg);
void LinkUpdateKey0(TDb *pDb,Tdata *pData);
void PgInit(TDb *pDb,TuPg *puPg,char Type);
Terr UpdateDataLong(TmapLong *pmLong,Tdata *pDataNew,Tdata *pDataDb);
void pDataKillLongArr(TmapLong *pmLong,Tdata *pDataIn);
Terr DbCreat(TDb *pDb,char *patch,enum TtypeDb typeDb);
Terr DbOpenDiv(TDb *pDb,char* patch,enum TtypeDb typeDb);
// в макросах
Terr LongDelInd(Tlong *pLong,Tsize ind);
void ListLongClear(TmapLong *pmLong);
TpgData* PgDownR(TDb *pDb,Tadr *pAdr);
TpgData* PgDownL(TDb *pDb,Tadr *pAdr);
void getPachtLibDir(Tstr *pname,const char *pRas,int sizeDir,char *pout,int sizeout);
void pdataLongRef(TmapLong *pmLong,Tdata *pData);
void pValLongRef(TmapLong *pmLong,Tval *pVal);
//void moveTohis(TmapLong *pmLong,Tdata *pData);
//
//const char *CdirDb[]={"ldb","gdb","tmp"};
//const char *CdirLoc="ldb";

//#define CidirDbLoc 0
//#define CidirDbGlb 1
//#define CidirDbTmp 2
/*
 *
 */

//void recusionpData(Tdata *pData,void *pUser,void fp(Tdata *pD,void *pUser) );

//void pDataAddRefLongs(Tdata *pD,void *pUser){ if (pDataIsLong(pD)) pD->datapmLong=(TmapLong*)pUser; }
/*
 * Установить новый размер массива
 */
void mapArrSetSize(TDb *pDb,Toff Off){
 Toff Size,SizeOld;
 //
// Size=mapSizeByte( (Off)?( mshOkrUp(Off+1,CsizeArrDef) ):CsizeArrDef );
 Size=mapSizeByte(Off) + CsizeArrPgDef;
 SizeOld=pDb->mpg.size;
 if (Size-SizeOld){
  mapReMap(pDb->mpg,Size);
  //установить размер массива
  if (Size>SizeOld){
   memNULLvoid( (pDb->mpg.psec + SizeOld + sizeof(Tdata) ) , Size - SizeOld);
   }
  }
 }
/*
 * Очистить дерево
 * TisPg dbisPg,TisArr dbisArr,TisLong dbisLong
 */
Terr mshgetDb(TDb *pDb,Tstr *pName,const char *pDir,enum TisSet dbisSet,int sizePartGlb,enum TtypeDb typeDb){
 char pfl[CsizePacthFl];
 Terr err;
 //
 if ((pDir==NULL)||(pName==NULL)||(pName->size==0)) return CERRnamenotFound;
 pName->psec[pName->size]=0;
 if (sizePartGlb){
  getPachtLibDir(pName,pDir,sizePartGlb,pfl,CsizePacthFl);
  }
 else{
  getPachtDir(pName,pDir,pfl);
  }
 if ((err=DbOpenDiv(pDb,pfl,typeDb) )<CERR) return err;
 if (dbisSet && (err==CWARglbnotFound))
  err=DbCreat(pDb,pfl,typeDb);
 return err;
 }
/*
 */
Terr mshDbLocNew(TDb *pDb,Tstr *pName,char *pDir,enum TtypeDb typeDb){
 char pfl[CsizePacthFl];
 //
 if ((pDir==NULL)||(pName==NULL)||(pName->size==0)) return CERRnamenotFound;
 pName->psec[pName->size]='\0';
 getPachtDir(pName,pDir,pfl);
 return DbCreat(pDb,pfl,typeDb);
 }
/*
 * Найти поле KeyData
 */
static Tval* KeyFindVal(TDb *pDb,Tadr *pAdr,Tdata *pKey,Terr *perr) {
 Tval *pVal;
 TuPg uPg;
 //
 uPg.pgD=PgLinkFindAdr(pDb,pAdr,pKey,perr);
 if (*perr!=COutKodOk) return NULL;
 if (TypePgIsNoData(uPg.pg)){
  *perr=CWARdataNotFound;
  return NULL;
  }
 pVal=PgFindKey(pDb,uPg.pgD,pKey,perr);
 if (*perr== CMPeq) {
  *perr=COutKodOk;
  return pVal;
  }
 *perr=CWARdataNotFound;
 return NULL;
 }
/*
 */
/*
static Tval* findKeyAdr(TDb *pDb,Tadr *pAdr,Tdata *pKey, TIterNext *pIterNext){
 TuPg uPg;
 //
 uPg.pgD=PgLinkFindAdr(pDb,pAdr,pKey,&(pIterNext->AdrOfferr.err));
 if (pIterNext->AdrOfferr.err != COutKodOk) return NULL;
 if (TypePgIsNoData(uPg.pg)){pIterNext->AdrOfferr.err=CERRnotFoundPgData;return NULL;}
 return PgFindKey(pDb,uPg.pgD,pKey,&(pIterNext->AdrOfferr.err));
 }
*/
/*
 * получить значение по индексу из дерева
 */
/*
 *  прочитать данные с заданными индексами
 *  выход : поле индекса на котором завершен поиск
*/
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr){
 Tdata *pind;
 Tval *pVal;
 TuPg uPg;
 Tadr *pAdr,Adr;
 //
 pVal=NULL;
 uPg.pvoid = AdrToPg(pDb,pgRoot(pDb));
 if (!(uPg.pg->busy)) {*perr=CWARdataNotFound;return NULL;}
 //
 Adr=pgRoot(pDb);
 pind=pInds->psec;
 while( pind <= parrptop(pInds) ) {
  pVal=KeyFindVal(pDb,&Adr,pind,perr);
  if (*perr !=COutKodOk) return NULL;
  if (pind == parrptop(pInds)) break;
  pAdr=indNext(pVal,pind);
  if (pAdrIsNULL(pAdr)) {*perr=CWARdataNotFound;return NULL;}
  Adr=*pAdr;
  }
// pValRefLong(pDb,pVal);
 *perr=COutKodOk;
 return pVal;
 }
/*
 */
Terr mshStat(TDb *pDb,TarrData *pInds,Tdata *pDataOut){
 Tval *pVal;
 Terr err;
 //
 pDataSetInt(pDataOut);
 pVal=mshGet(pDb,pInds,&err);
 if (pVal!=NULL) {
  pValStat(pVal,pDataOut->intgr);
  }
 return err;
 }
/*
 * Записать данные в дерево
 */
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData) {
 Tdata *pind;
 Tval Val,*pVal;
 Terr err;
 Tadr *pRoot=NULL;
 //
 if (pInds->busy==0) return CERRvmIndFiled;
 //ищем последнюю объектную ссылку
 pind=pInds->psec;
 ValALLSetNULL(Val);
 while(pind < pInds->ptop) {
  if (pDataIsNULL(pind)) return CERRnullKey;
  ValKey(Val)=*pind;
  pVal=ValToInd(pDb,pRoot,&Val,&err,0);
  if (err<CERR) return err;
  pRoot=indNext(pVal,pind);
  }
 if (pDataIsNULL(pind)) return CERRnullKey;
 ValKey(Val)=*pind;
 ValData(Val)=*psetData;
 (void)ValToInd(pDb,pRoot,&Val,&err,1);
 return err;
 }
/*
 * Удалить данные из дерева
*/
/*
 * Удалить Дерево
 */
Terr mshKillDbDALL(TDb *pDb) {
 TuPg uPg;
 //Очистить дерево
 mapReMap( pDb->mpg,pDb->mpg.sizesec * CcountPgDef);
 //инициализируем страницу pgFree
 uPg.pvoid = AdrToPg(pDb,pgFree);
 PgInit(pDb,&uPg,CTypePgFree);
 // находим головную страницу
 uPg.pvoid = AdrToPg(pDb,pgRoot(pDb));
 //инициализируем головную страницу страницу
 PgInit(pDb,&uPg,CTypePgData);
 pDbSteckClear(pDb);
 //очистить длинные страницы
 pDbLongClear(pDb);
 return COutKodOk;
 }
/**/
Terr mshKill(TDb *pDb,TarrData *pInds,u_char tpK) {
 Tdata *pind;
 Terr err=COutKodOk;;
 TuPg uPg;
 TAdrOfferr *pAdrOfferr;
 Tadr *pAdr,Adr;
 Tval *pVal;
 Tkey *pKey;
 u_char isAlt;
 //очистить дерево
 if ( (pInds==NULL)||(parrIsNull(pInds)) ) return mshKillDbDALL(pDb);
 //
 uPg.pvoid = AdrToPg(pDb,pgRoot(pDb));
 if (!(uPg.pg->busy)) return CWARdataNotFound;
 //спускаемся по индексу до последней вершины
 Adr=pgRoot(pDb);
 pind=pInds->psec;
 //очищаем стек спуска
 pDbSteckClear(pDb);
 for (;;) {
  Adr=getSteckDown(pDb,Adr,pind,&err);
  if (err<CERR) return err;
  uPg.pvoid = AdrToPg(pDb,Adr);
  if (TypePgIsNoData(uPg.pg)) return CERRnotFoundPgData;
  pVal=PgFindKey(pDb,uPg.pgD,pind,&err);
  if (err!=CMPeq) return CWARdataNotDown;
  if (pind >= parrptop(pInds)) break;
  pDbSteckBusy(pDb);
  pDb->trackdb.ptop->adr=Adr;
  pDb->trackdb.ptop->off=OffSec(uPg.pgD,pVal);
  pDb->trackdb.ptop->isalt=pDataIsAlt(pind);
  pDb->trackdb.ptop->err=err;
  pAdr=indNext(pVal,pind);
  if (pAdr==NULL) return CWARdataNotDown;
  Adr=*pAdr;
  }
 //удаление данных 
 if (tpK & CtypeKiLLData ){
  pValDataDelLong(&(pDb->mlong),pVal);
  pValDataSetNULL(pVal);
  }
 if ((tpK & CtypeKiLLDown )&&(pValAdrIsNoNULL(pVal))) {
  (void)PgToFree(pDb,pValAdr(pVal));
  pValAdrSetNULL(pVal);
  }
 if ((tpK & CtypeKiLLAlt )&&(pValAdrAltIsNoNULL(pVal))) {
  (void)PgToFree(pDb,pValAdrAlt(pVal));
  pValAdrAltSetNULL(pVal);
  }
 if (pValALLIsNoNULL(pVal)) return COutKodOk;
// PgDataValDel(uPg.pgD,pVal);
 PgValDelref(&(pDb->mlong),uPg.pgD,pVal);
 //страница не пуста закончить
 if (uPg.pgD->busy) {
  //ключ был первым корректируем ссылку
  if (pVal==uPg.pgD->psec) LinkUpdateKey0(pDb,&(pValKey(uPg.pgD->psec)));
  return COutKodOk;
  }
 if ((err=DelpgData(pDb,Adr))<CERR) return err;
 //обрабатываем вышележащие вершины
 isAlt=0;
 while(arrIsNoNull(pDb->trackdb)){
  pAdrOfferr=pDb->trackdb.ptop;
  pDbSteckPop(pDb);
  isAlt=pAdrOfferr->isalt;
  uPg.pvoid = AdrToPg(pDb,pAdrOfferr->adr);
  //страница ссылок
  if (TypePgIsLink(uPg.pg)){
   pKey=pgOffpSec(uPg.pgL,pAdrOfferr->off);
   pPgDelVal(uPg.pgL,pKey);
   if (uPg.pgL->busy){
    if (pKey==uPg.pgL->psec) LinkUpdateKey0(pDb,&(pValKey(uPg.pgL->psec)));
    return COutKodOk;
    }
   //корень дерева, удаление завершено
   if (pAdrOfferr->adr==pgRoot(pDb)) return COutKodOk;
   //страница пуста, удалить страницу
   if ((err=PgToFree(pDb,pAdrOfferr->adr))<CERR) return err;
   }
  //страница данных
  else{
   pVal=pgOffpSec(uPg.pgD,pAdrOfferr->off);
   if (isAlt){
    pValAdrAltSetNULL(pVal);
    }
   else{
    pValAdrSetNULL(pVal);
    }
   //есть в вершине данные
   if (pValALLIsNoNULL(pVal)) return COutKodOk;
//   PgDataValDel(uPg.pgD,pVal);
   PgValDelref(&(pDb->mlong),uPg.pgD,pVal);
   //страница не пуста закончить
   if (uPg.pgD->busy) {
    //ключ был первым корректируем ссылку
    if (pVal==uPg.pgD->psec) LinkUpdateKey0(pDb,&(pValKey(uPg.pgD->psec)));
    return COutKodOk;
    }
   //корень дерева, удаление завершено
   if (pAdrOfferr->adr==pgRoot(pDb)) return COutKodOk;
   //страница пуста, удалить страницу
   if ((err=DelpgData(pDb,pAdrOfferr->adr))<CERR) return err;
   }
  }
 return COutKodOk;
 }
/*
 *  найти следующую вершину
 */
static Terr next0(TIterNext *pIterNext) {
 TuPg uPg;
 //
 if (pIterNext->AdrOfferr.adr==0) return CMPnull;
 uPg.pvoid = AdrToPg(pIterNext->pdb,pIterNext->AdrOfferr.adr);
 if (pIterNext->AdrOfferr.off < uPg.pg->busy-1)
  ++(pIterNext->AdrOfferr.off);
 else{
  do{
   if (AdrIsNULL(uPg.pgD->right)) return CMPnull;
   pIterNext->AdrOfferr.adr=uPg.pgD->right;
   uPg.pvoid = AdrToPg(pIterNext->pdb,pIterNext->AdrOfferr.adr);
   }while(!(uPg.pg->busy));
  pIterNext->AdrOfferr.off=0;
  }
 pIterNext->val=pgOffSec(uPg.pgD,pIterNext->AdrOfferr.off);
 return COutKodOk;
 }
Terr mshNext(TIterNext *pIterNext) {
 //
 if (pIterNext==NULL) return CERRiteratorNotFound;
 ValALLSetNULL(pIterNext->val);
 pIterNext->AdrOfferr.err=CMPnull;
 if (pIterNext->AdrOfferr.adr==0) return pIterNext->AdrOfferr.err=CMPnull;
 return pIterNext->AdrOfferr.err=next0(pIterNext);
 }
Terr mshBack(TIterNext *pIterNext) {
 TuPg uPg;
 //
 if (pIterNext==NULL) return CERRiteratorNotFound;
 ValALLSetNULL(pIterNext->val);
 pIterNext->AdrOfferr.err=CMPnull;
 if (pIterNext->AdrOfferr.adr==0) return CMPnull;
 uPg.pvoid = AdrToPg(pIterNext->pdb,pIterNext->AdrOfferr.adr);
 if (pIterNext->AdrOfferr.off > 0)
  --(pIterNext->AdrOfferr.off);
 else {
  do{
   if (AdrIsNULL(uPg.pgD->left)) return pIterNext->AdrOfferr.err;
   pIterNext->AdrOfferr.adr=uPg.pgD->left;
   uPg.pvoid = AdrToPg(pIterNext->pdb,pIterNext->AdrOfferr.adr);
   }while(!(uPg.pg->busy));
  pIterNext->AdrOfferr.off=uPg.pg->busy-1;
  }
 pIterNext->AdrOfferr.err=COutKodOk;
 pIterNext->val=pgOffSec(uPg.pgD,pIterNext->AdrOfferr.off);
// ValRefLong(pIterNext->pdb,pIterNext->val);
 return pIterNext->AdrOfferr.err;
 }
/**/
static Tval* mshGetAdrOff(TDb *pDb,TarrData *pInds, TIterNext *pIterNext){
 Tdata *pind;
 Tval *pVal;
 TuPg uPg;
 Tadr *pAdr,Adr;
 //
 ValALLSetNULL(pIterNext->val);
 uPg.pvoid = AdrToPg(pDb,pgRoot(pDb));
 if (!(uPg.pg->busy)) {pIterNext->AdrOfferr.err=CWARdataNotFound;return NULL;}
 //
 pIterNext->AdrOfferr.adr=pgRoot(pDb);
 Adr=pIterNext->AdrOfferr.adr;
 pind=pInds->psec;
 while(pind <= parrptop(pInds)) {
  pVal=KeyFindVal(pDb,&Adr,pind,&(pIterNext->AdrOfferr.err) );
  if (pIterNext->AdrOfferr.err !=COutKodOk) return NULL;
  if (pind==parrptop(pInds)) break;
  pAdr=indNext(pVal,pind);
  if (pAdrIsNULL(pAdr)) {pIterNext->AdrOfferr.err=CWARdataNotFound;return NULL;}
  Adr=*pAdr;
  }
 pIterNext->AdrOfferr.err=COutKodOk;
 pIterNext->val=*pVal;
 pIterNext->AdrOfferr.adr=Adr;
 uPg.pvoid = AdrToPg(pDb,pIterNext->AdrOfferr.adr);
 pIterNext->AdrOfferr.off=OffSec(uPg.pgD,pVal);
 pIterNext->AdrOfferr.isalt=pDataIsAlt(pind);
// ValRefLong(pIterNext->pdb,pIterNext->val);
 return &(pIterNext->val);
 }
#define NextBack(pIterNext) (pIterNext->type==CcmdBack)?mshBack(pIterNext):mshNext(pIterNext)
/*
 *         Итератор по одному уровню
 * найти первую вершину
 */
static Terr findNodeRoot(TIterNext *pIterNext,TarrData *pInds) {
 Tadr *pAdr;
 Tval *pVal;
 //
 pIterNext->AdrOfferr.err=CMPnull;
 AdrSetNULL(pIterNext->AdrOfferr.adr);
 //вход с головы
 if (parrIsNull(pInds)){
  pAdr=&(pgRoot(pIterNext->pdb));
  }
 else{
  if ((pVal=mshGetAdrOff(pIterNext->pdb,pInds,pIterNext))==NULL) return pIterNext->AdrOfferr.err=CMPnull;
  pAdr=pValIndpAdr(pVal,pInds->ptop);
  if (pAdrIsNULL(pAdr)) return pIterNext->AdrOfferr.err;
  }
 pIterNext->AdrOfferr.adr=*pAdr;
 return pIterNext->AdrOfferr.err=COutKodOk;
 }
//
Terr mshNext0(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey) {
 Tval *pVal;
 //
 if ( (findNodeRoot(pIterNext,pInds))!= COutKodOk) {
  if (pKey) {pDataSetNULL(pKey);}
  ValALLSetNULL(pIterNext->val);
  return pIterNext->AdrOfferr.err=CMPnull;
  }
 //ищем ключ на следующем уровне
 if (pIterNext->type==CcmdBack)
  pVal=PgKeyDown(pIterNext->pdb,&(pIterNext->AdrOfferr.adr),pKey,&(pIterNext->AdrOfferr.off),&(pIterNext->AdrOfferr.err));
 else    
  pVal=PgKeyUp(pIterNext->pdb,&(pIterNext->AdrOfferr.adr),pKey,&(pIterNext->AdrOfferr.off),&(pIterNext->AdrOfferr.err));
 if (pVal==NULL) {
  if (pKey) {pDataSetNULL(pKey);}
  ValALLSetNULL(pIterNext->val);
  return pIterNext->AdrOfferr.err=CMPnull;
  }
 pIterNext->val=*pVal;
// ValRefLong(pIterNext->pdb,pIterNext->val);
 if (pKey) *pKey=ValKey(pIterNext->val);
 return pIterNext->AdrOfferr.err=COutKodOk;
 }
//
Terr tstfindNodeRoot(TIterNext *pIterNext,TarrData *pInds) {
 return findNodeRoot(pIterNext,pInds);
 }
/*
 * Инициализация итератора
 */
Terr mshNextInit0(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey) {
 TuPg uPg;
 //опорный узел
 if ( (findNodeRoot(pIterNext,pInds))!= COutKodOk) return pIterNext->AdrOfferr.err=CMPnull;
 uPg.pgD=PgDownLR(pIterNext->pdb,&(pIterNext->AdrOfferr.adr),pIterNext->type);
 //есть ли потомки у узла
 if (AdrIsNULL(pIterNext->AdrOfferr.adr)) return pIterNext->AdrOfferr.err=CMPnull;
 //потомки есть
// uPg.pvoid = AdrToPg(pIterNext->pdb,pIterNext->AdrOfferr.adr);
 pIterNext->AdrOfferr.off=(pIterNext->type==CcmdBack)?uPg.pg->busy-1:0;
 pIterNext->AdrOfferr.err=COutKodOk;
 pIterNext->val=pgOffSec(uPg.pgD,pIterNext->AdrOfferr.off);
// ValRefLong(pIterNext->pdb,pIterNext->val);
 if (pKey) *pKey=ValKey(pIterNext->val);
 return pIterNext->AdrOfferr.err=COutKodOk;
 }
Terr mshNextInit(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey) {
 //опорный узел
 ValALLSetNULL(pIterNext->val);
 if ((pKey)&&(pDataIsNoNULL(pKey)))
  return mshNext0(pIterNext,pInds,pKey);
 return mshNextInit0(pIterNext,pInds,pKey);
 }
/*
 * Итератор должен содержать ссылку на базу данных
 * pIterNext->pdb
 *  и тип итератора
 * pIterNext->type
 * -------------------------------------------------------
 *  найти следующую вершину вне Итератора
 * pInds - Опорный индекс под которым проводится обход
 * pKey - индекс от которого начинать следующую итерацию
 *  начинать итерации надо с pKey==NULL или пустого pKey
 * 
 */
Terr mshNext1(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey) {
 //
 ValALLSetNULL(pIterNext->val);
 if ( (pKey==NULL)||(pDataIsNULL(pKey)) )
  return mshNextInit0(pIterNext,pInds,pKey);
 return mshNext0(pIterNext,pInds,pKey); 
 }
/*
 *         Итератор по всему дереву
 */
/*
 *  найти следующую вершину
 */
 //вставить ссылку на базу данных в возвращаемое значение
 //вставить ссылку на базу данных в возвращаемый индекс
/*
void static pQueryOutDataInspLong(TIterQuery *pQuery){
 Tdata *pD;
 //
 recusionpData(&(ValKey(pQuery->iterNext.val)),&(pQuery->iterNext.pdb->mlong),pDataAddRefLongs ); recusionpData(&(ValData(pQuery->iterNext.val)),&(pQuery->iterNext.pdb->mlong),pDataAddRefLongs );
 arrIter(pQuery->addInd,pD) recusionpData(pD,&(pQuery->iterNext.pdb->mlong),pDataAddRefLongs );
 }
*/
Terr mshQuery(TIterQuery *pQuery) {
 Tadr Adr;
 TuPg uPg;
 //
 pQuery->iterNext.AdrOfferr.err=CMPnull;
// pgOffSecAdrOffData(pDb,pQuery->iterNext.AdrOfferr,uPg,pQuery->iterNext.val);
down:
 Adr=ValAdr(pQuery->iterNext.val);
 if (AdrIsNULL(Adr)) goto right;
 //вершину сохранить в стеке
 uPg.pgD=PgDownL(pQuery->iterNext.pdb,&Adr);
 if ( (uPg.pgD)&&(uPg.pg->busy) ) {
  arrPushDan(pQuery->stackDown,pQuery->iterNext.AdrOfferr);
  pQuery->iterNext.AdrOfferr.adr=Adr;
  pQuery->iterNext.AdrOfferr.off=0;
  pQuery->iterNext.val=*(pgOffpSec(uPg.pgD,pQuery->iterNext.AdrOfferr.off));
  arrPushDan(pQuery->addInd,ValKey(pQuery->iterNext.val));
  if (ValDataIsNoNULL(pQuery->iterNext.val)) return pQuery->iterNext.AdrOfferr.err=COutKodOk;
  goto down;
  }
right:
 pQuery->iterNext.AdrOfferr.err=next0(&(pQuery->iterNext));
 if (pQuery->iterNext.AdrOfferr.err==COutKodOk) {
  //заменить последний индекс
  if (ValDataIsNoNULL(pQuery->iterNext.val)){
  *(arrptop(pQuery->addInd))=ValKey(pQuery->iterNext.val);
   return pQuery->iterNext.AdrOfferr.err=COutKodOk;
   }
  goto down;
  }
up:
 if (arrIsNull(pQuery->stackDown)) return pQuery->iterNext.AdrOfferr.err=CMPnull;
 pQuery->iterNext.AdrOfferr=*(arrptop(pQuery->stackDown));
 arrPop(pQuery->stackDown);
 arrPop(pQuery->addInd);
 goto right; 
 }
/**/
/*
static Terr mshQueryFirstNode(TDb *pDb,TIterQuery *pQuery,Tadr Root) {
 TuPg uPg;
 //Находим первый элемент
 pQuery->iterNext.AdrOfferr.adr=Root;
 uPg.pgD=PgDownL(pDb,&(pQuery->iterNext.AdrOfferr.adr));
 if (uPg.pvoid==NULL) return CERRnotFoundPgData; 
 pQuery->iterNext.val=*(uPg.pgD->psec);
 pQuery->iterNext.AdrOfferr.off=0;
 return COutKodOk;
 }
*/
/*
 * Итератор должен содержать ссылку на базу данных
 * pIterNext->pdb
 */
//
/*
Terr mshQueryInit(TIterQuery *pQuery) {
 TuPg uPg;
 TDb *pDb=pQuery->iterNext.pdb;
 //
 ValALLSetNULL(pQuery->iterNext.val);
 pDbSteckClear(pDb);
 arrClear(pQuery->addInd);
 memNULL(&(pQuery->iterNext.AdrOfferr));
 //есть входной индекс
 if (arrIsNoNull( pQuery->rootInd)){
  if ( mshGetAdrOff(pDb,&(pQuery->rootInd),&(pQuery->iterNext)) ==NULL)
   return pQuery->iterNext.AdrOfferr.err=CMPnull;
  }
 //Вход с головы. Индекс пустой.
 else{
  // спускаемся по страницам Link Находим первый элемент
  if ((pQuery->iterNext.AdrOfferr.err=mshQueryFirstNode(pDb,pQuery,pgRoot(pDb)))<CERR) return pQuery->iterNext.AdrOfferr.err;
  }
 //найдена опорный узел Данных
 //если он содержит данные то узел найден
 if (ValDataIsNoNULL(pQuery->iterNext.val)) return pQuery->iterNext.AdrOfferr.err=COutKodOk;
 //найдена опорный узел Данных
 //данных в узле не обнаружены
 //пытаемся найти 1-й нижележащий узел 
 pQuery->iterNext.AdrOfferr.adr=ValAdr(pQuery->iterNext.val);
 //если ниже нет данных то обход завершен
 if (AdrIsNULL(pQuery->iterNext.AdrOfferr.adr)) return pQuery->iterNext.AdrOfferr.err=CMPnull;
 //спуститься по страницам Link
 uPg.pgD=PgDownL(pDb,&pQuery->iterNext.AdrOfferr.adr);
 //если данные на странице данных не обнаружены то обход завершен
 if ( (uPg.pgD==NULL)||(uPg.pg->busy<1) ) return pQuery->iterNext.AdrOfferr.err=CMPnull;
 //встать в найденную страницу
 pQuery->iterNext.AdrOfferr.off=0;
 pQuery->iterNext.val=*(pgOffpSec(uPg.pgD,pQuery->iterNext.AdrOfferr.off));
 //если он содержит данные то узел найден
 if (ValDataIsNoNULL(pQuery->iterNext.val)) return pQuery->iterNext.AdrOfferr.err=COutKodOk;
 //продолжаем поис узла
 return mshQuery(pQuery);
 }
*/
/*
 * Итератор должен содержать ссылку на базу данных
 * pIterNext->pdb
 */
static Tval* mshGetAdrOffStackDown(TDb *pDb,Tadr root,TarrData *pInds,TarrAdrOfferr *pstackDown,TIterNext *pIter){
 Tdata *pind;
 TuPg uPg;
 Tadr *pAdr,Adr;
 TAdrOfferr *pAdrOfferr;
 Tval *pVal=NULL;
 //
 parrClear(pstackDown);
 uPg.pvoid=NULL;
 Adr=root;
 for ( pind=pInds->psec;pind <= parrptop(pInds); ){
  pVal=KeyFindVal(pDb,&Adr,pind,&(pIter->AdrOfferr.err));
  pIter->AdrOfferr.adr=Adr;
  //вершина не найдена
  if (pIter->AdrOfferr.err!=COutKodOk) return NULL;
  //последняя вершина в стек не помещается
  if (pind==parrptop(pInds)) break;
  //добавляем в стек узел
  parrBusyNull(pstackDown);
  pAdrOfferr=parrptop(pstackDown);
  pAdrOfferr->isalt=pDataIsAlt(pind);
  pAdrOfferr->adr=Adr;
  uPg.pvoid = AdrToPg(pDb,pAdrOfferr->adr);
  pAdrOfferr->off=(uPg.pgD)?(OffSec(uPg.pgD,pVal)):0;
  //Адрес погружения и следующий индекс
  pAdr=indNext(pVal,pind);
  //Адрес не обнаружен
  if (pAdrIsNULL(pAdr)) {pIter->AdrOfferr.err=CWARdataNotFound; return NULL;}
  Adr=*pAdr;
  //Адрес продолжения
  }
 if (pVal==NULL) {pIter->AdrOfferr.err=CWARdataNotFound; return NULL;}
 //найден искомый узел
 pIter->val=*pVal;
 pIter->AdrOfferr.adr=Adr;
 uPg.pvoid = AdrToPg(pDb,pIter->AdrOfferr.adr);
 pIter->AdrOfferr.off=(uPg.pgD)?(OffSec(uPg.pgD,pVal)):0;
 pIter->AdrOfferr.err=COutKodOk;
 return pVal;
 }
Terr mshQuery1(TIterQuery *pQuery) {
 Tval *pVal;
 Tdata *pKey;
 Tadr Adr;
 TDb *pDb=pQuery->iterNext.pdb;
 TuPg uPg;
 //
 ValALLSetNULL(pQuery->iterNext.val);
 pDbSteckClear(pDb);
 memNULL(&(pQuery->iterNext.AdrOfferr));
// Root=pgRoot(pDb);
 //опорный узел задан
 if (arrIsNoNull(pQuery->rootInd)){
  if ( (pVal=mshGetAdrOff(pDb,&(pQuery->rootInd),&(pQuery->iterNext)))==NULL) {
   pQuery->iterNext.AdrOfferr.err=CMPnull;
   goto err;
   }
  Adr=ValToAdrAlt(pQuery->iterNext.val,pQuery->iterNext.AdrOfferr.isalt);
  }
 //вход с головы входной индекс не задан
 else{
  Adr=pgRoot(pDb);
  //встаем на опорный индекс
  if (arrIsNull(pQuery->addInd)) goto down;
  }
 //найден опорный узел
 //если ниже нет данных то обход завершен
 if (AdrIsNULL(Adr)) {pQuery->iterNext.AdrOfferr.err=CMPnull; goto err;}
 //индекс входа не задан ищем следующую вершину от опорной
 if (arrIsNull(pQuery->addInd))
  goto query;
 //задан индекс входа, пытаемся по нему спуститься запоминая путь в стеке
 pVal=mshGetAdrOffStackDown(pDb,Adr,&(pQuery->addInd),&(pQuery->stackDown),&(pQuery->iterNext));
 if (pQuery->iterNext.AdrOfferr.err==COutKodOk) goto query;
 if (pQuery->iterNext.AdrOfferr.err==CkeyFirst)
  if ( pQuery->stackDown.busy==0 ) goto down;
 //вершина найдена ищем следующую
 //Нижележащих узлов нет
 if (pQuery->iterNext.AdrOfferr.err==CWARdataNotDown) {
  //встаем в последний найденный узел
  if (pQuery->stackDown.busy){
   pQuery->iterNext.AdrOfferr=*(pQuery->stackDown.ptop);
   arrPop(pQuery->stackDown);
   uPg.pvoid = AdrToPg(pDb,pQuery->iterNext.AdrOfferr.adr);
   pVal=pgOffpSec(uPg.pgD,pQuery->iterNext.AdrOfferr.off);
   //ищем следующий узел
   goto query;
   }
  //не найдено ни одного ключа на последней странице ищем ключ
  /*
  pKey=arrOffpData(pQuery->addInd,0);
  uPg.pvoid = AdrToPg(pDb,pQuery->iterNext.AdrOfferr.adr);
  pVal=PgFindKey(pDb,uPg.pgD,pKey,&(pQuery->iterNext.AdrOfferr.err));
  arrClear(pQuery->addInd);
  switch (pQuery->iterNext.AdrOfferr.err) {
   //на странице найден больший ключ его и возвращаем
   case CkeyFirst :
   case CMPdown :
    arrBusy(pQuery->addInd);
    *(pQuery->addInd.ptop)=pValKey(pVal);
    pQuery->iterNext.val=*pVal;
    return pQuery->iterNext.AdrOfferr.err=COutKodOk;
   //на странице только меньшие ключи, ищем следующую страницу
   case CMPup :
    //справа есть страница
    if (AdrIsNoNULL(uPg.pgD->right)) {
     pQuery->iterNext.AdrOfferr.adr=uPg.pgD->right;
     pQuery->iterNext.AdrOfferr.off=0;
     uPg.pvoid = AdrToPg(pDb,pQuery->iterNext.AdrOfferr.adr);
     pVal=pgOffpSec(uPg.pgD,0);
     arrBusy(pQuery->addInd);
     *(pQuery->addInd.ptop)=pValKey(pVal);
     pQuery->iterNext.val=*pVal;
     return pQuery->iterNext.AdrOfferr.err=COutKodOk;
     }
    //дальше страниц нет
   default: 
    pQuery->iterNext.AdrOfferr.err=CMPnull;
    goto err;
   }
  */
  }
 //последняя найденная страница
 pKey=arrOffpData(pQuery->addInd,pQuery->stackDown.busy);
 pQuery->addInd.busy=pQuery->stackDown.busy;
 arrsetpTop(pQuery->addInd);
// Adr=pQuery->iterNext.AdrOfferr.adr;
 //есть пройденные узлы
 uPg.pvoid = AdrToPg(pDb,pQuery->iterNext.AdrOfferr.adr);
 //-----------------------
 /*
 //встать в определенный узел
 uPg.pgD=PgDownL(pDb,&(pQuery->iterNext.AdrOfferr.adr));
 pVal=pgOffpSec(uPg.pgD,pQuery->iterNext.AdrOfferr.off);
 pAdr=pValIndpAdr(pVal,pKey);
 //нижележащих узлов нет
 if (pAdrIsNULL(pAdr)) goto query;
 //пытаемся уйти с последнего не найденного ключа
 Adr=*pAdr;
 uPg.pvoid=PgLinkFindAdr(pDb,&Adr,pKey,&(pQuery->iterNext.AdrOfferr.err));
 //ключ не найден он меньше первого.
 if (uPg.pvoid==NULL){
  uPg.pgD=PgDownL(pDb,&Adr);
  //если данные на странице данных не обнаружены то обход завершен
  if ( (uPg.pgD==NULL)||(uPg.pg->busy<1) ) {
   pQuery->iterNext.AdrOfferr.err=CMPnull;
   goto err;
   }
  pQuery->iterNext.AdrOfferr.adr=Adr;
  pQuery->iterNext.AdrOfferr.off=0;
  pVal=pgOffpSec(uPg.pgD,0);
  arrBusy(pQuery->addInd);
  *(pQuery->addInd.ptop)=pValKey(pVal);
  return pQuery->iterNext.AdrOfferr.err=COutKodOk;
  }
 */
 //------------------
 pVal=PgFindKey(pDb,uPg.pgD,pKey,&(pQuery->iterNext.AdrOfferr.err));
 switch (pQuery->iterNext.AdrOfferr.err) {
  //на странице найден больший ключ его и возвращаем
  case CkeyFirst :
  case CMPdown :
   arrBusy(pQuery->addInd);
   *(pQuery->addInd.ptop)=pValKey(pVal);
   pQuery->iterNext.val=*pVal;
   break;
  //на странице только меньшие ключи, ищем следующую страницу
  case CMPup :
   //справа есть страница
   if (AdrIsNoNULL(uPg.pgD->right)) {
    pQuery->iterNext.AdrOfferr.adr=uPg.pgD->right;
    pQuery->iterNext.AdrOfferr.off=0;
    uPg.pvoid = AdrToPg(pDb,pQuery->iterNext.AdrOfferr.adr);
    pVal=pgOffpSec(uPg.pgD,0);
    arrBusy(pQuery->addInd);
    *(pQuery->addInd.ptop)=pValKey(pVal);
    pQuery->iterNext.val=*pVal;
    break;
    }
   //ищем правый узел, если его нет то подымаемся выше по стеку
   while (pQuery->stackDown.busy) {
    //встать в последний найденный узел и от него начать поиск
    pQuery->iterNext.AdrOfferr=*(pQuery->stackDown.ptop);
    arrPop(pQuery->stackDown);
    uPg.pvoid = AdrToPg(pDb,pQuery->iterNext.AdrOfferr.adr);
    pVal=pgOffpSec(uPg.pgD,pQuery->iterNext.AdrOfferr.off);
    pQuery->iterNext.AdrOfferr.err=next0(&(pQuery->iterNext));
    if (pQuery->iterNext.AdrOfferr.err==COutKodOk) goto finish;
    }
  default: 
   pQuery->iterNext.AdrOfferr.err=CMPnull;
   goto err;
  }
finish: 
 //узел найден
 if ( (pVal)&&(pValDataIsNoNULL(pVal)) ) return pQuery->iterNext.AdrOfferr.err=COutKodOk;
query: 
 //продолжаем поиск узла
 if (pVal) pQuery->iterNext.val=*pVal;
 mshQuery(pQuery);
 if (pQuery->iterNext.AdrOfferr.err==COutKodOk){
//  arrPushDan(pQuery->addInd,ValKey(pQuery->iterNext.val));
  }
 return pQuery->iterNext.AdrOfferr.err;
down: 
 uPg.pgD=PgDownL(pDb,&Adr);
 //нет страниц данных
 if (uPg.pvoid==NULL){pQuery->iterNext.AdrOfferr.err=CMPnull; goto err;}
 pQuery->iterNext.AdrOfferr.off=0;
 pQuery->iterNext.AdrOfferr.adr=Adr;
 pVal=pgOffpSec(uPg.pgD,0);
 pQuery->iterNext.val=*pVal;
 arrBusy1(pQuery->addInd);
 *(pQuery->addInd.ptop)=pValKey(pVal);
 goto finish;
err:
 ValALLSetNULL(pQuery->iterNext.val);
 arrClear(pQuery->addInd);
 return pQuery->iterNext.AdrOfferr.err;
 }
/*
Terr mshQuery1(TIterQuery *pQuery) {
 Tval *pVal;
// Tadr Root;
 TDb *pDb=pQuery->iterNext.pdb;
 TuPg uPg;
 //
 ValALLSetNULL(pQuery->iterNext.val);
 pDbSteckClear(pDb);
 memNULL(&(pQuery->iterNext.AdrOfferr));
// Root=pgRoot(pDb);
 //опорный узел задан
 if (arrIsNoNull(pQuery->rootInd)){
  if ( (pVal=mshGetAdrOff(pDb,&(pQuery->rootInd),&(pQuery->iterNext)))==NULL) {
   pQuery->iterNext.AdrOfferr.err=CMPnull;
   goto err;
   }
  pQuery->iterNext.AdrOfferr.adr=ValToAdrAlt(pQuery->iterNext.val,pQuery->iterNext.AdrOfferr.isalt);
  }
 //вход с головы входной индекс не задан
 else{
  if ((pQuery->iterNext.AdrOfferr.err=mshQueryFirstNode(pDb,pQuery,pgRoot(pDb) ))<CERR) goto err;
  pQuery->iterNext.AdrOfferr.adr=ValAdr(pQuery->iterNext.val);
  }
 //найден опорный узел
 //вход с пустым ключем , данные есть их и возвращаем
 if ( (arrIsNull(pQuery->addInd))&&(ValDataIsNoNULL(pQuery->iterNext.val)) ) return pQuery->iterNext.AdrOfferr.err;
 //если ниже нет данных то обход завершен
 if (AdrIsNULL(pQuery->iterNext.AdrOfferr.adr)) {pQuery->iterNext.AdrOfferr.err=CMPnull; goto err;}
 //задан индекс входа, пытаемся по нему спуститься запоминая путь в стеке
 if (arrIsNoNull(pQuery->addInd)){
  //узел найден перейти к следующему
  pVal=mshGetAdrOffStackDown(pDb,pQuery->iterNext.AdrOfferr.adr,&(pQuery->addInd),&(pQuery->stackDown),&(pQuery->iterNext));
  if (pQuery->iterNext.AdrOfferr.err<CERR) goto err;
  if (pVal){
   switch (pQuery->iterNext.AdrOfferr.err) {
    //ключ найден
    case COutKodOk :
     pQuery->addInd.busy=pQuery->stackDown.busy;
     arrsetpTop(pQuery->addInd);
     mshQuery(pQuery);
     if (pQuery->iterNext.AdrOfferr.err==COutKodOk){
      arrPushDan(pQuery->addInd,ValKey(pQuery->iterNext.val));
      }
     return pQuery->iterNext.AdrOfferr.err;
    //найден больший ключ
    case CkeyFirst :
    case CMPdown :
     pQuery->addInd.busy=pQuery->stackDown.busy+1;
     arrsetpTop(pQuery->addInd);
     *(pQuery->addInd.ptop)=pValKey(pVal);
     return pQuery->iterNext.AdrOfferr.err=COutKodOk;
    }
   //на странице данных ключ меньше искомого ищем следующую страницу
   uPg.pvoid = AdrToPg(pDb,pQuery->iterNext.AdrOfferr.adr);
   //есть следующая страница 1-поле искомое
   if (AdrIsNoNULL( uPg.pgD->right )){
    uPg.pvoid = AdrToPg(pDb,uPg.pgD->right);
    if (uPg.pgD->busy){
     pVal= pgOffpSec(uPg.pgD,0);
     //узел найден
     if (pValKeyIsNoNULL(pVal)){
      pQuery->addInd.busy=pQuery->stackDown.busy+1;
      arrsetpTop(pQuery->addInd);
      *(pQuery->addInd.ptop)=pValKey(pVal);
      return pQuery->iterNext.AdrOfferr.err=COutKodOk;
      }
     }
    }
   }
  //узел не найден
  if (pQuery->iterNext.AdrOfferr.err<CERR) return pQuery->iterNext.AdrOfferr.err;
  return pQuery->iterNext.AdrOfferr.err=CMPnull;
  }
 //индекс входа не задан, ищем следующий после опорного
 //найдена опорный узел Данных
 //данных в узле не обнаружены
 //пытаемся найти 1-й нижележащий узел 
 //если ниже нет данных то обход завершен
 if (AdrIsNULL(pQuery->iterNext.AdrOfferr.adr)) return pQuery->iterNext.AdrOfferr.err=CMPnull;
 //спуститься по страницам Link
 uPg.pgD=PgDownL(pDb,&pQuery->iterNext.AdrOfferr.adr);
 //если данные на странице данных не обнаружены то обход завершен
 if ( (uPg.pgD==NULL)||(uPg.pg->busy<1) ) return pQuery->iterNext.AdrOfferr.err=CMPnull;
 //встать в найденную страницу
 pQuery->iterNext.AdrOfferr.off=0;
 pQuery->iterNext.val=*(pgOffpSec(uPg.pgD,pQuery->iterNext.AdrOfferr.off));
 //если он содержит данные то узел найден
 if (ValDataIsNoNULL(pQuery->iterNext.val)) {
  arrPushDan(pQuery->addInd,ValKey(pQuery->iterNext.val));
  return pQuery->iterNext.AdrOfferr.err=COutKodOk;
  }
 //продолжаем поиск узла
 mshQuery(pQuery);
 if (pQuery->iterNext.AdrOfferr.err==COutKodOk){
  arrPushDan(pQuery->addInd,ValKey(pQuery->iterNext.val));
  }
 return pQuery->iterNext.AdrOfferr.err;
err:
 ValALLSetNULL(pQuery->iterNext.val);
 arrClear(pQuery->addInd);
 return pQuery->iterNext.AdrOfferr.err;
 }
*/
Terr mshQueryInit(TIterQuery *pQuery) {
 return mshQuery1(pQuery);
 }
/*----------------------------
 * Обращение к массиву данных
 -----------------------------
 */
Tdata* mshGetArr(TDb *pDb,Tusize Off,Terr *perr){
 Tdata *pData;   
 //данные не найдены
 if (Off > (maparrpDbBusy(pDb)) ){
  *perr=CWARdataNotFound; return NULL;
  }
 *perr=COutKodOk;
 pData=mapArrpDataOff( pDb,Off);
 return pData;
 }
Terr mshStatArr(TDb *pDb,Tusize Off,Tdata *pStatOut){
 Tdata *pData;
 Terr err;
 //данные не найдены
 pDataSetInt(pStatOut);
 pStatOut->intgr =((pData=mshGetArr(pDb,Off,&err))==NULL)?CstatDan0
  : (Off > (maparrpDbBusy(pDb)) )?CstatDan0:(pDataIsNULL(pData))?CstatDan0:CstatDanD;
 return err;
 }
/*
 * Запись в массив
 */
static void wideDataArr(TDb *pDb,Toff Off){
 TintNum *pBusy;
 TintNum sizeOldData,w;
 void *pB;
 // 
 pBusy=&(maparrpDbBusy(pDb));
 if ( Off>(*pBusy) ) {
  sizeOldData=mapSizeData(pDb->mpg.size);
  if ( Off > sizeOldData )
   mapArrSetSize(pDb,Off);
  //зачистить приростс массива
  w=mapSizeByte(sizeOldData - (*pBusy));
  pB=mapSizeByte((*pBusy)+1) + pDb->mpg.psec;
  memNULLvoid( pB , w);
  *pBusy=Off;
  }
 }
static Terr setDataArr(TDb *pDb,Toff Off,Tdata *pSetData){
 Tdata *pDataDb;
 //изменить размер массива
 wideDataArr(pDb,Off);
 pDataDb=mapArrpDataOff(pDb,Off);
 pDataTypeSubSetNULL(pSetData);
 return UpdateDataLong(&(pDb->mlong),pSetData,pDataDb);
 }

Terr mshSetArr(TDb *pDb,Toff Off,Tdata *pSetData){
 //изменить размер массива
 if (!Off){
  mapArrSetSize(pDb,pSetData->intgr );
  return COutKodOk;
  }
 //записать данные в массив
 return setDataArr(pDb,Off,pSetData);
 }
//
Terr mshPushArr(TDb *pDb,Tdata *pSetData){
 return setDataArr(pDb,maparrpDbBusy(pDb)+1,pSetData);
 }
/*
 * Очистить весь массив
 */
void mshKillArrALL(TDb *pDb){
 //очистить массив
 maparrpDbBusy(pDb)=0;
 //записать 0..0
 memNULLvoid(pDb->mpg.psec , pDb->mpg.size);
 //очистить длинные страницы
 pDbLongClear(pDb);
 }
/*
 * Удаление в массиве
 */
Terr mshKillArr(TDb *pDb,Tusize Off){
 Tdata *pData;
 TintNum *pBusy;
 //данные не найдены
 if (!Off) {
  mshKillArrALL(pDb);
  return COutKodOk;
  }
 pBusy=&(maparrpDbBusy(pDb));
 if ( Off > *pBusy ) return CWARdataNotFound;
 pData=mapArrpDataOff(pDb,Off);
 pDataKillLongArr(&(pDb->mlong),pData);
 //данные сдвинуть
 if ( Off < *pBusy ){
  memMove(pData,pData+1,mapSizeByte((*pBusy) - Off ));
  maparrpDbBusy(pDb)=(maparrpDbBusy(pDb))-1;
  }
 else
  (*pBusy)--;
 return COutKodOk;
 }
/**/
Terr mshArrNextBackInit(TIterNext *pNext){
 Tdata *pData;
 //
 ValALLSetNULL(pNext->val);
 if ((maparrpDbBusy(pNext->pdb)) == 0) return pNext->AdrOfferr.err=CMPnull;
 pNext->AdrOfferr.off=(pNext->type==CcmdBack)?(maparrpDbBusy(pNext->pdb)):1;
 pData=mapArrpDataOff(pNext->pdb,pNext->AdrOfferr.off);
 if (pDataIsNoNULL(pData)) {
//  pdataLongRef(&(pNext->pdb->mlong),pData);
  ValData(pNext->val)=*pData;
  }
 (ValKey(pNext->val)).intgr=pNext->AdrOfferr.off;
 DataSetInt(ValKey(pNext->val));
 return pNext->AdrOfferr.err=COutKodOk;
 }
/**/
Terr mshArrNextBack(TIterNext *pNext){
 Tdata *pData;
 //
 ValALLSetNULL(pNext->val);
 if ((maparrpDbBusy(pNext->pdb)) == 0) return pNext->AdrOfferr.err=CMPnull;
 pNext->AdrOfferr.err=COutKodOk;
 if (pNext->type==CcmdBack){
  if (pNext->AdrOfferr.off < 2)
   return pNext->AdrOfferr.err=CMPnull;
  pNext->AdrOfferr.off--;
  }
 else{
  if ( pNext->AdrOfferr.off >= (maparrpDbBusy(pNext->pdb)) )
   return pNext->AdrOfferr.err=CMPnull;
  pNext->AdrOfferr.off++;
  }
 pData=mapArrpDataOff(pNext->pdb,pNext->AdrOfferr.off);
 if ((pData)&&(pDataIsNoNULL(pData))) {
//  pdataLongRef(&(pNext->pdb->mlong),pData);
  ValData(pNext->val)=*pData;
  }
 (ValKey(pNext->val)).intgr=pNext->AdrOfferr.off;
 DataSetInt(ValKey(pNext->val));
 return pNext->AdrOfferr.err=COutKodOk;
 }
/**/
Terr mshArrQuery(TIterNext *pNext){
 Tdata *pData;
 //
 ValALLSetNULL(pNext->val);
 if ((maparrpDbBusy(pNext->pdb)) == 0) return pNext->AdrOfferr.err=CMPnull;
 while( (pNext->AdrOfferr.off) < (maparrpDbBusy(pNext->pdb)) ){
  pNext->AdrOfferr.off++;
  pData=mapArrpDataOff(pNext->pdb,pNext->AdrOfferr.off);
  if (pDataIsNoNULL(pData)) {
//   pdataLongRef(&(pNext->pdb->mlong),pData);
   ValData(pNext->val)=*pData;
   (ValKey(pNext->val)).intgr=pNext->AdrOfferr.off;
   DataSetInt(ValKey(pNext->val));
   return pNext->AdrOfferr.err=COutKodOk;
   }
  }
 return pNext->AdrOfferr.err=CMPnull;
 }
/**/
Terr mshArrQueryInit(TIterNext *pNext){
 Tdata *pData;
 //
 ValALLSetNULL(pNext->val);
 if ((maparrpDbBusy(pNext->pdb)) == 0) return pNext->AdrOfferr.err=CMPnull;
 if ( (pNext->AdrOfferr.off) >= (maparrpDbBusy(pNext->pdb)) ) return pNext->AdrOfferr.err=CMPnull;
 //
 pData=mapArrpDataOff(pNext->pdb,pNext->AdrOfferr.off);
 if (pDataIsNULL(pData))  return mshArrQuery(pNext);
// pdataLongRef(&(pNext->pdb->mlong),pData);
 ValData(pNext->val)=*pData;
 (ValKey(pNext->val)).intgr=pNext->AdrOfferr.off;
 DataSetInt(ValKey(pNext->val));
 return pNext->AdrOfferr.err=COutKodOk;
 }
/*
 * Обход данных Дерева Рекурсивно
 */
static int roundDataDbAltSub(TDb *pDb, Tadr Adr, TtreeData *ptreeData,int isAlt, int (*pfdata)(TtreeData *ptreeData)) {
 TuPg uPg;
 Tsize nsec;
 int err = COutKodOk;
 //
 uPg.pvoid = AdrToPg(pDb,Adr);
 if (uPg.pg->busy==0) return err;
 //страница pgLink
 if (PgGetType(uPg.pg)!=CTypePgData) {
  if (PgGetType(uPg.pg)!=CTypePgLink) return CERRnotFoundPgLink;
  for (nsec = 0; nsec < uPg.pg->busy; nsec++) {
   if (AdrIsNoNULL(ValAdr(uPg.pgL->psec[nsec])))
    if ((err = roundDataDbAltSub(pDb,ValAdr(uPg.pgL->psec[nsec]), ptreeData, isAlt, pfdata)) != COutKodOk) break;
   }
  return err;
  }
 //страница данных 
 ++(ptreeData->Level);
 arrBusy(ptreeData->Inds);
 for (nsec = 0; nsec < uPg.pg->busy; nsec++) {
  ptreeData->pkeydata = uPg.pgD->psec + nsec;
  *(ptreeData->Inds.ptop) = pValKey(ptreeData->pkeydata);
//  pdataLongRef(&(pDb->mlong),&(pValKey(ptreeData->pkeydata)));
//  pdataLongRef(&(pDb->mlong),&(pValData(ptreeData->pkeydata)));
  err = pfdata(ptreeData);
  if (err==CroundBreak) break;
  if (err==CroundContinue) continue;
  if ( (isAlt==EroundAlt) && (err!=CroundNoAlt) && (AdrIsNoNULL(pValAdrAlt(ptreeData->pkeydata))) ){
   ptreeData->isAlt=EroundAlt;
   err = roundDataDbAltSub(pDb, pValAdrAlt(ptreeData->pkeydata), ptreeData, isAlt, pfdata);
   }
  ptreeData->pkeydata = uPg.pgD->psec + nsec;
  if (AdrIsNoNULL(pValAdr(ptreeData->pkeydata))) {
   ptreeData->isAlt=EroundNoAlt;
   err = roundDataDbAltSub(pDb, pValAdr(ptreeData->pkeydata), ptreeData, isAlt, pfdata);
   }
  }
 arrPop(ptreeData->Inds);
 --(ptreeData->Level);
 return err;
 }
/*
 * Обход страниц Дерева Рекурсивно
 */
void roundPgDbAlt(TDb *pDb, Tadr Adr, void (*pfbeg)(TuPg *puPg, int Level,void *pUser), void (*pfend)(TuPg *puPg, int Level,void *pUser), int *pLevel,void *pUser) {
 TuPg uPg;
 Tsize nsec;
 //
 if ((uPg.pvoid = AdrToPg(pDb,Adr))==NULL) return;
 (*pLevel)++;
 if (uPg.pg->busy) {
  if (pfbeg != NULL) pfbeg(&uPg, *pLevel,pUser);
  switch (PgGetType(uPg.pg)) {
   case CTypePgFree :
    for (nsec = 0; nsec < uPg.pg->busy; nsec++) {
     if (AdrIsNoNULL(uPg.pgF->psec[nsec]))
      roundPgDbAlt(pDb, uPg.pgF->psec[nsec] , pfbeg, pfend, pLevel,pUser);
     }
    break;
   case CTypePgLink :
    for (nsec = 0; nsec < uPg.pg->busy; nsec++) {
     if (AdrIsNoNULL(ValAdr(uPg.pgL->psec[nsec])))
      roundPgDbAlt(pDb, ValAdr(uPg.pgL->psec[nsec]), pfbeg, pfend, pLevel,pUser);
    }
    break;
   case CTypePgData:
    for (nsec = 0; nsec < uPg.pg->busy; nsec++) {
     if (AdrIsNoNULL(ValAdrAlt(uPg.pgD->psec[nsec])))
      roundPgDbAlt(pDb, ValAdrAlt(uPg.pgD->psec[nsec]), pfbeg, pfend, pLevel,pUser);
     if (AdrIsNoNULL(ValAdr(uPg.pgD->psec[nsec])))
      roundPgDbAlt(pDb, ValAdr(uPg.pgD->psec[nsec]), pfbeg, pfend, pLevel,pUser);
    }
    break;
   }
  }
 if (pfend != NULL) pfend(&uPg, *pLevel,pUser);
 (*pLevel)--;
 }
/*
 * Обход страниц Дерева
void roundPgDbLocAltRoot(TDbLoc *pDb, void (*pfbeg)(Tpg *pg, int Level), void (*pfend)(Tpg *pg, int Level));
 */
void roundPgDbAltRoot(TDb *pDb, void (*pfbeg)(TuPg *puPg, int Level,void *pUser), void (*pfend)(TuPg *puPg, int Level,void *pUser),void *pUser ) {
 int Level = 0;
 if (pgRoot(pDb))
  roundPgDbAlt(pDb, pgRoot(pDb), pfbeg, pfend, &Level,pUser);
 }
/*
 * Обход данных Дерева или поддерева ???
 */
int roundDataDbAlt(TDb *pDb, TarrData *pInds,int isAlt, int (*pfdata)(TtreeData *ptreeData) , void *pUser) {
 TuPg uPg;
 TtreeData treeData;
 Tadr Adr;
 Terr err = COutKodOk;
 //
 uPg.pvoid = AdrToPg(pDb,pgRoot(pDb));
 if (!uPg.pg->busy) return CWARdataNotFound;
 treeDataInit(treeData);
 treeData.pUser = pUser;
 treeData.pDb=pDb;
 //Обработка входящей вершины
 if ((pInds == NULL)||(pInds->busy<1))
  Adr = pgRoot(pDb);
 else {
  treeData.pkeydata = mshGet(pDb, pInds, &err);
  if (err != COutKodOk) return err;
  if (treeData.pkeydata == NULL) return CWARdataNotFound;
  Adr = pValAdr(treeData.pkeydata);
  }
 if (AdrIsNULL(Adr)) return CWARdataNotFound;
 //Обход потомков
 err = roundDataDbAltSub(pDb, Adr, &treeData, isAlt,pfdata);
 //
 treeDataFree(treeData);
 return err;
 }
