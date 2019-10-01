/*
 * File:   mshDb.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 20 Август 2014 г., 12:13
 */

#include "mshDb.h"
#include "mshData.h"
#include "mshPgLong.h"
#include "mshPack.h"
#include "mshErr.h"

void* PackUnVoid(TPacks *pPack,TsizePack *pSize);
Terr checkDivMag(Tdiv ff,const char *pmag);
Terr openDir(char *patchDir);

const static char* pCmshMagGlb="MSHGL000";
const static int pCmshMagGlbLen=8;
//
const char* CextDbHeder="mdh";
const char* CextArrHeder="mah";
const char* CextLongHeder="mlh";
const char* CextDbPg="mdp";
const char* CextDbLong="mdl";
const char* CextArrPg="map";
const char* CextArrLong="mal";
const char* CextLongLong="mlp";

Tval* ValAddPg(TDb *pDb, TpgData *pPg,Tval *pVal,int isData,Terr *perr);
Tval* ValInsToPg(TDb *pDb, TpgData *Pg, Tval *pVal, int isData, Terr *perr);
Tadr getSteckDown(TDb *pDb,Tadr Adr,Tdata *pKey,Terr *perr);
void PgInit(TDb *pDb,TuPg *puPg,char Type);
Terr LongDelInd(Tlong *pLong,Tsize ind);
void ListLongClear(TmapLong *pmLong);
void moveTohis(TmapLong *pmLong, Tdata *pData);
void pDataKillLongArr(TmapLong *pmLong,Tdata *pDataIn);
Terr mshKillDbDALL(TDb *pDb);
void mshKillArrALL(TDb *pDb);
/*
 * получить трассу спуска по страницам pgLink до данного ключа
 */
//static Tadr* LocSteckDown(TDbLoc *pDb,Tadr *pAdr,Tdata *pKey,Terr *perr);
//Tadr PgLinkStackFindAdr(TDb *pDb,Tadr Adr,Tdata *pKey,Terr *perr);
/*
 * Откорректировать 1-й ключ
 * void LocLinkUpdateKey0(TDbLoc *pDb,Tdata *pData)
*/
void LinkUpdateKey0(TDb *pDb,Tdata *pData){
 TuPg uPg;
 Tkey *pkey;
 //поднимаемся по трассе наверх
 moveTohis( &(pDb->mlong),pData );
 while ( pDb->trackdb.ptop ) {
  uPg.pvoid=AdrToPg(pDb,pDb->trackdb.ptop->adr);
  if (TypePgIsNoLink(uPg.pg)) return;
  pkey=pgOffpSec(uPg.pgL,pDb->trackdb.ptop->off);
  pValKey(pkey)=*pData;
  if (pDb->trackdb.ptop->off) break;
  pDbSteckPop(pDb);
  }
 }
/*
 * Преобразование страниц к типу pgFree
 */
static void UpdateToPgFree(TDb *pDb,Tpg *Pg){
 if (TypePgIsFree(Pg)) return;
 //
 Tkey *pKey,*pKeyEnd;
 Tusize busy;
 TuPg uPg;
 //
 busy=Pg->busy;
 Pg->busy=0;
 uPg.pg=Pg;
 if (busy) {
  switch (PgGetType(uPg.pg)) {
   case CTypePgLink :
    pKey=uPg.pgL->psec;
    pKeyEnd=uPg.pgL->psec+busy;
    break;  
   case CTypePgData :
    pKey=(Tkey*)(uPg.pgD->psec);
    pKeyEnd=(Tkey*)(uPg.pgD->psec+busy);
    break;  
   }  
  for (;pKey<pKeyEnd;++pKey){
   pDataKillLongArr(&(pDb->mlong), &((pKey)->data) );
   if (AdrIsNoNULL(pValAdr(pKey))) uPg.pgF->psec[Pg->busy++]=pValAdr(pKey);
   }
  }
 uPg.pgF->type=CTypePgFree;
 uPg.pgF->size=SizePgToSize(pDb,TpgFree,uPg.pgF);
 }
/*
 * страницу перенести в область свободных
 * Terr PgDbLocToFree(TDbLoc *pDb, Tpg *Pg)
 */
Terr PgToFree(TDb *pDb, Tadr AdrPg) {
 Tadr Adr;
 Terr err = COutKodOk;
 TuPg uPg;
 // находим страницу AdrPg
 uPg.pvoid = AdrToPg(pDb,AdrPg);
 //преобразуем ее в свободную
 UpdateToPgFree(pDb,uPg.pg);
 //находим голову свободных страниц
 uPg.pvoid = AdrToPg(pDb,pgFree);
 //пытаемся разместить нашу страницу в списке свободных страниц
 while ((err = PgAddSec(uPg.pgF,AdrPg)) != COutKodOk) {
  //Адрес НЕ поместился на странице спускаемся по правому краю в поисках свободного места
  Adr = PgTop(uPg.pgF);
  uPg.pvoid = AdrToPg(pDb,Adr);
  UpdateToPgFree(pDb,uPg.pg);
  }
 return err;
 }
/*
 * Удалить страницу
 */
Terr DelpgData(TDb *pDb, Tadr Adr){
 TuPg uPg,uPg1;
 //
 uPg.pvoid = AdrToPg(pDb,Adr);
 //есть левая страница
 if (AdrIsNoNULL(uPg.pgD->left)){
  uPg1.pvoid = AdrToPg(pDb,uPg.pgD->left);
  uPg1.pgD->right=uPg.pgD->right;
  }
 if (AdrIsNoNULL(uPg.pgD->right)){
  uPg1.pvoid = AdrToPg(pDb,uPg.pgD->right);
  uPg1.pgD->left=uPg.pgD->left;
  }
 return PgToFree(pDb,Adr);
 }
/*
 * получить пустую страницу
 * Tpg* GetPgLocEmpty(TDbLoc *pDb, char Type)
 */
static void GetPgEmpty(TDb *pDb,TAdruPg *pAdruPg, char Type) {
 Tpg *pgParent;
 //
 pAdruPg->uPg.pvoid=AdrToPg(pDb,pgFree);
 //нет пустых страниц
 if (pAdruPg->uPg.pg->busy<1){
  PgNew(pDb,pAdruPg->Adr,pAdruPg->uPg,Type);
  return;
  }
 do{
  pgParent = pAdruPg->uPg.pg;
  pAdruPg->Adr=PgTop(pAdruPg->uPg.pgF);
  pAdruPg->uPg.pvoid = AdrToPg(pDb,pAdruPg->Adr);
  if (pAdruPg->uPg.pg->busy)
   UpdateToPgFree(pDb,pAdruPg->uPg.pg);
  }while(pAdruPg->uPg.pg->busy);
 PgPop(pgParent);
 PgInit(pDb,&(pAdruPg->uPg),Type);
 }
/*
 * Добавить поле KeyData
 */
#define	RefToOff(Ref) (void*)Ref - (pDb)->mpg.psec
#define	OffToRref(Off,Type) (Type*)((pDb)->mpg.psec + Off)
//меняем ссылку в предыдущем индексе
#define	AdrToRoot(pDb,Adr,offRoot) *((Tadr*)((pDb)->mpg.psec + offRoot))=Adr
/*
 * значение pVal заносим на страницу данных
*/
static Tval* ValToPgD(TDb *pDb,TAdruPg *pAdruPg,TAdruPg *pAdruPg1,Tval *pVal,Tval *pvalPg,int isData,Terr *perr){
 u_int64_t offVal;
 Terr ERR;
 TAdruPg AdruPg;
 //
 ERR=*perr;
 offVal=(pvalPg==NULL)?0:OffSec(pAdruPg->uPg.pgD,pvalPg);
 //данные не поместились на странице? создаем дополнительную страницу
 GetPgEmpty(pDb,pAdruPg1,CTypePgData);
 if (pAdruPg1->uPg.pg==NULL) {
  *perr=CERRpgNewEmptyNot;
  return NULL;
  }
 //
 pAdruPgInit(pDb,pAdruPg);
 //добавляем новое значение в новую страницу
 pvalPg=ValAddPg(pDb,pAdruPg1->uPg.pgD,pVal,isData,perr);
 if (*perr<CERR) return NULL;
 //ключ помещается на пустую страницу идущую первой
 if (ERR==CkeyFirstFull){
  if (pAdruPg->uPg.pgD->left){
   AdruPg.Adr=pAdruPg->uPg.pgD->left;
   AdruPgInit(pDb,AdruPg);
   AdruPg.uPg.pgD->right=pAdruPg1->Adr;
   }
  pAdruPg1->uPg.pgD->right=pAdruPg->Adr;
  pAdruPg1->uPg.pgD->left=pAdruPg->uPg.pgD->left;
  pAdruPg->uPg.pgD->left=pAdruPg1->Adr;
  }
 //ключ помещается на страницу идущую второй
 else{
  //ключ вставляется в середину
  if (ERR==CMPdown) PgDivadrSec(pAdruPg->uPg.pgD,pAdruPg1->uPg.pgD,offVal)
  if (pAdruPg->uPg.pgD->right){
   AdruPg.Adr=pAdruPg->uPg.pgD->right;
   AdruPgInit(pDb,AdruPg);
   AdruPg.uPg.pgD->left=pAdruPg1->Adr;
   }
  pAdruPg1->uPg.pgD->left=pAdruPg->Adr;
  pAdruPg1->uPg.pgD->right=pAdruPg->uPg.pgD->right;
  pAdruPg->uPg.pgD->right=pAdruPg1->Adr;
  }
 return pvalPg;
 }
/*
 * Добавляем в дерево индекс и данные если они есть
 */
Tval* ValToInd(TDb *pDb,Tadr *pRoot,Tval *pVal,Terr *perr,int isData) {
 Terr ERR=COutKodOk;
 TAdruPg AdruPg,AdruPgRoot,AdruPg1,AdruPg2,AdruPgL;
 Tval *pvalPg,*pvalOut;
 Tkey key,key2,*pkey;
 TAdrOfferr *pAdrOfferr;
 TuPg uPg;
 Tdata *pdataKey;
 int n;
 u_int64_t offRoot,offValOut;
 //  ищем корень ветки
 DataTypeSubSetNULL(pValKey(pVal));
 if (isData) { DataTypeSubSetNULL(pValData(pVal)); }
 else{ DataSetNULL(pValData(pVal)); }
 //это корень дерева
 if (pRoot==NULL) {
  offRoot=0;
  AdruPgRoot.Adr=pgRoot(pDb);
  }
 //это нижележащая ветвь
 else{
  offRoot=RefToOff(pRoot);
  if (pAdrIsNULL(pRoot)){
   GetPgEmpty(pDb,&AdruPgRoot,CTypePgData);
   if (AdruPgRoot.uPg.pg==NULL) {*perr=CERRpgNewEmptyNot;return NULL;}
   AdrToRoot(pDb,AdruPgRoot.Adr,offRoot);
   return ValAddPg(pDb,AdruPgRoot.uPg.pgD,pVal,isData,perr);
   }
  AdruPgRoot.Adr=*pRoot;
  }
 AdruPgInit(pDb,AdruPgRoot);
 //если узел пуст
 if (!(AdruPgRoot.uPg.pg->busy)) return ValAddPg(pDb,AdruPgRoot.uPg.pgD,pVal,isData,perr);
 *perr=COutKodOk;
 //в дереве есть данные
 //-------------в Индексе страница данных добавляем в нее-------------
 if (TypePgIsData(AdruPgRoot.uPg.pg)) {
  //pvalOut-то место куда происходит вставка данных
  pvalOut=ValInsToPg(pDb,AdruPgRoot.uPg.pgD,pVal,isData,&ERR);
  //данные поместились на странице
  if ((ERR==COutKodOk)||(ERR==CkeyFirst)) return pvalOut;
  *perr=ERR;
  pvalPg=ValToPgD(pDb,&AdruPgRoot,&AdruPg1,pVal,pvalOut,isData,perr);
  if (*perr<CERR) return NULL;
  offValOut=RefToOff(pvalPg);
  GetPgEmpty(pDb,&AdruPg2,CTypePgLink);
  if (AdruPg2.uPg.pg==NULL) {
   *perr=CERRpgNewEmptyNot;return NULL;
   }
  AdruPgInit(pDb,AdruPgRoot);
  AdruPgInit(pDb,AdruPg1);
  //меняем голову
  if (offRoot) {
   //меняем ссылку в предыдущем индексе
   AdrToRoot(pDb,AdruPg2.Adr,offRoot);
   }
  else {
   //переносим головную в новую страницу
   memCopy(AdruPg2.uPg.pvoid,AdruPgRoot.uPg.pvoid,SizePg(pDb));
   Swap(AdruPgRoot,AdruPg2);
   if (ERR==CkeyFirstFull)
    AdruPg1.uPg.pgD->right=AdruPgRoot.Adr;
   else
    AdruPg1.uPg.pgD->left=AdruPgRoot.Adr;
   }
  PgInit(pDb,&(AdruPg2.uPg),CTypePgLink);
  if (ERR==CkeyFirstFull){
   KeysToPgLink(AdruPg1.uPg.pgD->psec,AdruPgRoot.uPg.pgD->psec,AdruPg1.Adr,AdruPgRoot.Adr,AdruPg2.uPg.pgL);
   }
  else{
   KeysToPgLink(AdruPgRoot.uPg.pgD->psec,AdruPg1.uPg.pgD->psec,AdruPgRoot.Adr,AdruPg1.Adr,AdruPg2.uPg.pgL);
   }
  return OffToRref(offValOut,Tval);
  }
 //----------------------------------------------------
 //в корне ссылочная страница pgLink спуститься по ссылочным страницам записывая путь в стек
 pdataKey=&(pValKey(pVal));
 arrClear(pDb->trackdb);
 AdruPg.Adr=getSteckDown(pDb,AdruPgRoot.Adr,pdataKey,perr);
 if (*perr<CERR) return NULL;
 // спустились до странице pgData
 AdruPgInit(pDb,AdruPg);
 //пытаемся вставить данные в страницу
 pvalOut=ValInsToPg(pDb,AdruPg.uPg.pgD,pVal,isData,&ERR);
 //данные поместились на странице
 if (ERR==COutKodOk) return pvalOut;
 // корректируем 0-ключ в страницах pgLink
 *perr=COutKodOk;
 //данные добавляются
// moveTohis( &(pDb->mlong),pdataKey );
 if (ERR==CkeyFirst) {
  offValOut=RefToOff(pvalOut);
  LinkUpdateKey0(pDb,pdataKey);
  return OffToRref(offValOut,Tval);
  }
 //-------данные НЕ поместились на странице---------
 if (ERR==CMPup){
  if (AdrIsNoNULL(AdruPg.uPg.pgD->right)){
   AdruPg1.Adr=AdruPg.uPg.pgD->right;
   AdruPgInit(pDb,AdruPg1);
   if (AdruPg1.uPg.pg->busy < AdruPg1.uPg.pg->size) {
    uPg.pvoid=AdrToPg(pDb,pDb->trackdb.ptop->adr);
    if (pDb->trackdb.ptop->off+1<uPg.pg->busy){
     pDb->trackdb.ptop->err=CkeyFirst;
     pDb->trackdb.ptop->off++;
     pvalPg=ValInsToPg(pDb,AdruPg1.uPg.pgD,pVal,isData,perr);
     offValOut=RefToOff(pvalPg);
     LinkUpdateKey0(pDb,pdataKey);
     return OffToRref(offValOut,Tval);
     }
    }
   }
  }
 else
  if (AdrIsNoNULL(AdruPg.uPg.pgD->left)){
   AdruPg2.Adr=AdruPg.uPg.pgD->left;
   AdruPgInit(pDb,AdruPg2);
   if (AdruPg2.uPg.pg->busy < AdruPg2.uPg.pg->size){
     //пытаемся вставить данные в конец предыдущей страницы
    if (ERR==CkeyFirstFull)
     return ValAddPg(pDb,AdruPg2.uPg.pgD,pVal,isData,perr);
    AdruPg2.uPg.pgD->psec[AdruPg2.uPg.pgD->busy++]=AdruPg.uPg.pgD->psec[0];
    memMove(AdruPg.uPg.pgD->psec,AdruPg.uPg.pgD->psec + 1,(pvalOut - AdruPg.uPg.pgD->psec - 1) * sizeof(Tval));
    --pvalOut;
//    ValSetValPg( &(pDb->mlong) ,pvalOut,pVal);
    *pvalOut=*pVal;
    offValOut=RefToOff(pvalOut);
    pDb->trackdb.ptop->err=CkeyFirst;
    LinkUpdateKey0(pDb,&(pValKey(AdruPg.uPg.pgD->psec)));
    return OffToRref(offValOut,Tval);
    }
   }
 //
 *perr=ERR;
 pvalPg=ValToPgD(pDb,&AdruPg,&AdruPg1,pVal,pvalOut,isData,perr);
 // смещение поля данных в файле
 offValOut=RefToOff(pvalPg);
 //новый ключ для страниц pgLink
 ValKey(key)=pValKey(pVal);
 ValAdr(key)=AdruPg1.Adr;
 moveTohis( &(pDb->mlong),&(ValKey(key)) );
 //выполнить корректировку ключей на страницах pgLink
 while (arrIsNoNull(pDb->trackdb)) {
  pAdrOfferr=pDb->trackdb.ptop;
  arrPop(pDb->trackdb);
  AdruPgL.Adr=pAdrOfferr->adr;
  AdruPgInit(pDb,AdruPgL);
  //место на странице хватает
  if (AdruPgL.uPg.pgL->busy < AdruPgL.uPg.pgL->size) {
   //вставить в конец страницы
   switch(pAdrOfferr->err){
    case CMPup:
     AddSecToPg(AdruPgL.uPg.pgL,key);
     break;
    //первый ключ 
    case CkeyFirst:
     PgInsiSec(AdruPgL.uPg.pgL,key,0);
     LinkUpdateKey0(pDb,&(ValKey(key)));
     break;
    //вставка в середину 
    default :
     if (ERR!=CkeyFirst) pAdrOfferr->off++;
     PgInsiSec(AdruPgL.uPg.pgL,key,pAdrOfferr->off);
     break;
    }
   return OffToRref(offValOut,Tval);
   }
  //на странице места нет дробим страницу 
  GetPgEmpty(pDb,&AdruPg1,CTypePgLink);
  if (AdruPg1.uPg.pg==NULL){*perr=CERRpgNewEmptyNot;return NULL;}
  AddSecToPg(AdruPg1.uPg.pgL,key);
  AdruPgInit(pDb,AdruPgL);
  if (pAdrOfferr->err==CMPdown) {
   pAdrOfferr->off++;
   n=AdruPgL.uPg.pgL->busy - pAdrOfferr->off;
   pkey=pgOffpSec(AdruPgL.uPg.pgL,pAdrOfferr->off);
   memCopy((AdruPg1.uPg.pgL->psec + AdruPg1.uPg.pgL->busy),pkey,n * sizeof(*(AdruPgL.uPg.pgL->psec)));
   AdruPg1.uPg.pgL->busy +=n;
   AdruPgL.uPg.pgL->busy -=n;
   }
  key.adr=AdruPg1.Adr;
  ERR=pAdrOfferr->err;
  }
 //стек закончился раздроблена корневая вершина
 GetPgEmpty(pDb,&AdruPg2,CTypePgLink);
 if (AdruPg2.uPg.pg==NULL){*perr=CERRpgNewEmptyNot;return NULL;}
 AdruPgInit(pDb,AdruPgL);
 if (offRoot) {
  AdrToRoot(pDb,AdruPg2.Adr,offRoot);
  }
 else{
  memCopy(AdruPg2.uPg.pvoid,AdruPgL.uPg.pvoid,pDb->mpg.sizesec);
  PgClear(AdruPgL.uPg.pg);
  Swap(AdruPgL,AdruPg2);
  }
 ValAdr(key2)=AdruPgL.Adr;
 ValKey(key2)=pValKey(AdruPgL.uPg.pgL->psec);
 if ((ERR==CkeyFirst)||(ERR==CkeyFirstFull)) {
  AddSecToPg(AdruPg2.uPg.pgL,key);
  AddSecToPg(AdruPg2.uPg.pgL,key2);
  }
 else{
  AddSecToPg(AdruPg2.uPg.pgL,key2);
  AddSecToPg(AdruPg2.uPg.pgL,key);
  }
 return OffToRref(offValOut,Tval);
 }
/*
 * Инициализация дерева
 */
void mapInit(Tmap *pMap,size_t sizeSec);

static void DbInit(TDb *pDb,TdbHead *phdr){
 //Инициализировать Дерево pDb
 pDb->typeDb=phdr->typeDb;
 if (phdr->sizelong){
  mapInit((Tmap*)(&(pDb->mlong)),phdr->sizelong);
  pDb->mlong.limitViewlong=ClimitLongDef;
  }
 switch (pDb->typeDb) {
  case EtypeDb :
   pDbSteckInit(pDb);
  case EtypeArr :
   mapInit((Tmap*)(&(pDb->mpg)),phdr->sizepg);
   break;  
  case EtypeLong :
   break;
  default :
   break;
  }  
 }
/*
 *   CextHeder
 */
static Terr dbHeadCreat(char* patch,TdbHead *phdr,const char *pextHead){
 int wp;
 ssize_t off,w;
 Tdiv divHdr;
 Terr err=CERRdivCreateBad;
 //Заголовок
 wp=strlen(patch);
 mshaddExtToFL(pextHead,patch,wp);
 if ((divHdr=divCreat(patch))==CdivRetErr) goto finish;
 w=strlen(pCmshMagGlb);
 if ((off=divWrite(divHdr,pCmshMagGlb,w))<w){
  divClose(divHdr);
  goto finish;
  }
 w=sizeof(TdbHead);
 off=divWrite(divHdr,phdr,w);
 divClose(divHdr);
 if (off==w) err=COutKodOk;
finish: 
 patch[wp]='\0';
 return err;
 }
/*
 * Создать страницы длинных строк
 */
static Terr creatLongDiv(TDb *pDb,char* patch,const char *pextLong,Tusize sizelong){
 int wp;
 Terr err=CERRdivCreateBad;
 //   Long
 wp=strlen(patch);
 mshaddExtToFL(pextLong,patch,wp);
 if ((pDb->mlong.div=divCreat(patch))==CdivRetErr) goto finish;
 divSetSize(pDb->mlong.div,sizelong *CcountLongDef);
 err=COutKodOk;
finish: 
 patch[wp]='\0';
 return err;
 }
/**/
static Terr DbcreatLongDiv(TDb *pDb,char* patch,TdbHead *phdr){
 Terr err;
 //Заголовок
 if((err=dbHeadCreat(patch,phdr,CextLongHeder))<CERR) return err;
 //   Long
 return creatLongDiv(pDb,patch,CextLongLong,phdr->sizelong);
 }

/*
 * Создать новое дерево
 */
static Terr DbcreatDbDiv(TDb *pDb,char* patch,TdbHead *phdr){
 Terr err;
 int wp;
 //Заголовок
 wp=strlen(patch);
 //Заголовок
 if((err=dbHeadCreat(patch,phdr,CextDbHeder))<CERR) return err;
 //Pg
 mshaddExtToFL(CextDbPg,patch,wp);
 if ((pDb->mpg.div=divCreat(patch))==CdivRetErr) return CERRdivCreateBad;
 divSetSize(pDb->mpg.div,phdr->sizepg * CcountPgDef );
 patch[wp]='\0';
 if ( (phdr->sizelong)&&((err=creatLongDiv((TDb*)pDb,patch,CextDbLong,phdr->sizelong))<CERR)){ divClose(pDb->mpg.div); }
 return err;
 }
/*
 * Создать новый массив
 */
static Terr DbcreatArrDiv(TDb *pDb,char* patch,TdbHead *phdr){
 Terr err;
 int wp;
 //Заголовок
 wp=strlen(patch);
 if((err=dbHeadCreat(patch,phdr,CextArrHeder))<CERR) return err;
 pDb->mpg.div=CdivRetErr;
 //Pg
 mshaddExtToFL(CextArrPg,patch,wp);
 if ((pDb->mpg.div=divCreat(patch))==CdivRetErr) return CERRdivCreateBad;
 divSetSize(pDb->mpg.div,phdr->sizepg);
 patch[wp]='\0';
 if ((err=creatLongDiv((TDb*)pDb,patch,CextArrLong,phdr->sizelong))<CERR){
  divClose(pDb->mpg.div);
  }
 return err;
 }
Terr DbCreatDb(TDb *pDb,char *patch,TdbHead *phdr){
 Terr err;
 TuPg uPg;
 //
 if ((err=DbcreatDbDiv(pDb,patch,phdr))<CERR) return err;
 DbInit(pDb,phdr);
 uPg.pvoid = AdrToPg(pDb,pgFree);
 PgInit(pDb,&uPg,CTypePgFree);
 uPg.pvoid = AdrToPg(pDb,pgRoot(pDb));
 PgInit(pDb,&uPg,CTypePgData);
 return COutKodOk;
 }
/*
 * Создать дерево
 */
Terr DbCreat(TDb *pDb,char *patch,enum TtypeDb typeDb){
 Terr err;
 TdbHead hdr;
 Tlong *pLong;
 int j;
 //
 memNULL(pDb);
 if ((err=openDir(patch))<CERR) return err;
 memNULL(&hdr);
 hdr.typeDb=typeDb;
 switch (typeDb) {
  case EtypeDb :
   hdr.sizepg=CsizeDbPgDef;
   hdr.sizelong=CsizeDbLongDef;
   if ((err=DbCreatDb(pDb,patch,&hdr))<CERR) return err;
   break;  
  case EtypeArr :
   hdr.sizepg=CsizeArrPgDef;
   hdr.sizelong=CsizeArrLongDef;
   err=DbcreatArrDiv(pDb,patch,&hdr);
   DbInit(pDb,&hdr);
   memNULLvoid( (pDb)->mpg.psec , (pDb)->mpg.size);
   maparrpDbBusy(pDb)=0;
   break;  
  case EtypeLong :
   hdr.sizelong=CsizeLongDef;
   if ((err=DbcreatLongDiv(pDb,patch,&hdr))<CERR) return err;
   DbInit(pDb,&hdr);
   break;
  default :
   break;
  }
 //инициаплизация страниц Long
 if (hdr.sizelong)
  for (j=0;j<CcountLongDef;++j){
   pLong=getLongOfmLong(pDb->mlong,j);
   pLongInit(&(pDb->mlong),pLong);
   }
 return err;
 }
/*
 * Создать дерево заданных размеров
 */
Terr DbCreatSize(TDb *pDb,char *patch,Tusize sizePg,Tusize sizeLong){
 Terr err;
 TdbHead hdr;
 TuPg uPg;
 Tlong *pLong;
 int j;
 //
 memNULL(pDb);
 if ((err=openDir(patch))<CERR) return err;
 memNULL(&hdr);
 hdr.sizepg=sizePg;
 hdr.sizelong=sizeLong;
 hdr.typeDb=EtypeDb;
 if ((err=DbcreatDbDiv(pDb,patch,&hdr))<CERR) return err;
 DbInit(pDb,&hdr);
 uPg.pvoid = AdrToPg(pDb,pgFree);
 PgInit(pDb,&uPg,CTypePgFree);
 uPg.pvoid = AdrToPg(pDb,pgRoot(pDb));
 PgInit(pDb,&uPg,CTypePgData);
 //инициаплизация страниц Long
 if (hdr.sizelong)
  for (j=0;j<CcountLongDef;++j){
   pLong=getLongOfmLong(pDb->mlong,j);
   pLongInit(&(pDb->mlong),pLong);
   }
 return err;
 }

/*
 * Открыть устройство
 */
static Terr dbHeadOpen(char* patch,TdbHead *phdr,const char *pextHead){
 Tdiv divHdr;
 Terr err=CWARglbnotFound;
 ssize_t off=0;
 int wp;
 //Заголовок
 wp=strlen(patch);
 //Заголовок Глобали
 mshExtaddtoFL(pextHead,patch);
 divHdr=divOpenR(patch);
 if (divHdr==CdivRetErr) goto finish;
 //проверка Магической последовательности
 if ((err=checkDivMag(divHdr,pCmshMagGlb))>CERR) {
  //Магическая последовательность верна
  off=divRead(divHdr,phdr,sizeof(TdbHead));
  }
 divClose(divHdr);
 err=(off<sizeof(TdbHead))?CERRglbHead:COutKodOk;
finish: 
 patch[wp]='\0';
 return err;
 }
#define DbopenDivCrt(patch,Div) if ((Div=divOpen(patch))==CdivRetErr){err=CWARglbnotFound; goto finish;}
/*
*/
Terr DbOpenDiv(TDb *pDb,char* patch,enum TtypeDb typeDb){
 Terr err;
 TdbHead hdr;
 int wp;
 //
 wp=strlen(patch);
 //
 memNULL(pDb);
 switch (typeDb) {
  case EtypeDb :
   if ((err=dbHeadOpen(patch,&hdr,CextDbHeder))!=COutKodOk) goto finish;
   if (hdr.sizepg>0) {
    mshaddExtToFL(CextDbPg,patch,wp);
    DbopenDivCrt(patch,pDb->mpg.div)
    }
   if (hdr.sizelong>0) {
    mshaddExtToFL(CextDbLong,patch,wp);
    DbopenDivCrt(patch,pDb->mlong.div)
    }
   break;  
  case EtypeArr :
   if ((err=dbHeadOpen(patch,&hdr,CextArrHeder))!=COutKodOk) goto finish;
   if (hdr.sizepg>0) {
    mshaddExtToFL(CextArrPg,patch,wp);
    DbopenDivCrt(patch,pDb->mpg.div)
    }
   if (hdr.sizelong>0) {
    mshaddExtToFL(CextArrLong,patch,wp);
    DbopenDivCrt(patch,pDb->mlong.div)
    }
   break;  
  case EtypeLong :
   if ((err=dbHeadOpen(patch,&hdr,CextLongHeder))!=COutKodOk) goto finish;
   if (hdr.sizelong==0) {err=CWARglbnotFound; goto finish;}
   mshaddExtToFL(CextLongLong,patch,wp);
   DbopenDivCrt(patch,pDb->mlong.div)
   break;
  default :
   break;
  }
 DbInit(pDb,&hdr);
finish: 
 patch[wp]='\0';
 return err;
 }
/*
 * Очистить произвольное дерево дерево
 */
void mshDbClear(TDb *pDb){
 //
 switch (pDb->typeDb) {
 //Очистить дерево
  case EtypeDb :
   (void)mshKillDbDALL(pDb);
   break;
 //очистить массив
  case EtypeArr :
   mshKillArrALL(pDb);
   break;  
  case EtypeLong :
   //очистить длинные страницы
   pDbLongClear(pDb);
   break;  
  default :
   break;
  }  
 }
/*
 * Освободить локальне дерево
 */
void mshDbFree(TDb *pDb) {
 switch (pDb->typeDb) {
  //Очистить дерево
  case EtypeDb :
   pDbSteckFree(pDb);
  //очистить массив
  case EtypeArr :
   mapFree(pDb->mpg);
  case EtypeLong :
   break;  
  default :
   break;
  }  
 mapFree(pDb->mlong);
 }
