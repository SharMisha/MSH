/* 
 * File:   mshDb.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 20 Август 2014 г., 12:04
 */

#ifndef MSHDB_H
#define	MSHDB_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "mshCnst.h"
#include "mshData.h"

/*
 * получить строку из буфера
 */
 /*
 * Типовая страница
 */
#define	contHeadPg Tusize size;Tusize busy;u_char type

typedef struct TsttPg {contHeadPg;} Tpg;
 
#define	contHeadPgD contHeadPg;Tadr left;Tadr right

/* пустая страница */
typedef struct {
 contHeadPg;
 Tadr psec[]; /* адреса */
 } TpgFree;
/*
 * __________Страница Указателей на нижележащие блоки_______________
 * страница состоит из полей data= unsigned int long
 * 1.начало блока
 * 2.. ссылки на ключи
 * -32
*/

typedef struct {
 contHeadPg;
 Tkey psec[]; /* ключи */
 } TpgLink;
 
typedef struct {
 contHeadPgD;
 Tval psec[];  /*данные */
 } TpgData;
 
typedef union {
 TpgData *pgD;
 TpgLink *pgL;
 TpgFree *pgF;
 Tpg *pg;
 void *pvoid;
 } TuPg;
 
typedef struct {
 Tadr Adr;
 TuPg uPg;
 } TAdruPg;
//Размеры страниц по умолчанию 
// offsetof(Tstr,psec)
 #define CsizeDbPgDef CnumPgDef * sizeof(Tval) + offsetof(TpgData,psec)
 #define CsizeDbLongDef CnumLongDef
 #define CsizeArrPgDef CnumArrDef * sizeof(Tdata)
 #define CsizeArrLongDef CsizeDbLongDef
 #define CsizeLongDef CsizeDbLongDef
 #define CsizeModPgDef CnumModPgDef * sizeof(Tval) + offsetof(TpgData,psec)
 #define CsizeModLongDef CnumModLongDef
 
#define	AdruPgInit(pDb,AdruPg) (AdruPg).uPg.pvoid=AdrToPg(pDb,(AdruPg).Adr)
#define	pAdruPgInit(pDb,pAdruPg) (pAdruPg)->uPg.pvoid=AdrToPg(pDb,(pAdruPg)->Adr)
 
#define	PgGetType(Pg) (Pg)->type
#define	TypePgIsFree(Pg) (PgGetType(Pg))==CTypePgFree
#define	TypePgIsNoFree(Pg) (PgGetType(Pg))!=CTypePgFree
#define	TypePgIsLink(Pg) (PgGetType(Pg))== CTypePgLink
#define	TypePgIsNoLink(Pg) (PgGetType(Pg)) != CTypePgLink
#define	TypePgIsData(Pg) (PgGetType(Pg))== CTypePgData
#define	TypePgIsNoData(Pg) (PgGetType(Pg))!= CTypePgData
 
#define	TypePg(Pg) ((TypePgIsFree(Pg))?((TpgFree*)Pg):(TypePgIsLink(Pg))?((TpgLink*)Pg):(TypePgIsData(Pg))?((TpgData*)Pg):NULL)
#define	TypeuPg(puPg) ((TypePgIsFree(puPg->pg))?puPg->pgF:(TypePgIsLink(puPg->pg))?puPg->pgL:(TypePgIsData(puPg->pg))?puPg->pgD:NULL)
 
#define	SizePg(pDb) (pDb)->mpg.sizesec
#define	SizePgToSize(pDb,tpPg,Pg) (SizePg(pDb)-offsetof(tpPg,psec))/sizeof(*((Pg)->psec))
//#define	SizePgToSize(pDb,TypePg,Pg) (SizePg(pDb)-offsetof(TypePg,psec))/sizeof(typeof((Pg)->psec[0]))

#define	pgOffpSec(Pg,Off) (Pg)->psec +(Off)
#define	pgOffSec(Pg,Off) (Pg)->psec[Off]
#define	ValOfAdr(pDb,Adr,offVal) pgOffSec(AdrToPg(pDb,Adr),offVal)
 

#define	pgOffpSecData(Pg,Off) ((TpgData*)(Pg))->psec + (Off)
#define	pgOffSecData(Pg,Off) ((TpgData*)(Pg))->psec[Off]
//позиция поля внутри страницы 
#define	OffSec(Pg,pSec) (pSec-((Pg)->psec))

#define	PgClear(Pg) (Pg)->busy=0
#define	PgDClear(Pg) (Pg)->busy=0; AdrSetNULL(Pg.left);AdrSetNULL(Pg.right)
#define	PgIsNoNULL(Pg) (Pg)->busy
 
#define	PgNew(pDb,adrPg,uPg,Type) mapsecNew(pDb->mpg,adrPg,uPg.pvoid);PgInit(pDb,&uPg,Type)
 
#define	PgTop(Pg) (Pg)->psec[(Pg)->busy-1]
 
#define	PgPop(Pg) if ((Pg)->busy) (Pg)->busy--
/*
 * Добавить секцию на страницу
 */ 
#define	AddSecToPg(pPg,Sec) (pPg)->psec[(pPg)->busy++]=Sec
 
#define	PgAddSec(pPg,Sec) (pPg->size > pPg->busy)? ({AddSecToPg(pPg,Sec);COutKodOk;}) : CERRPgFull
 
#define	pPgDelVal(pPg,pSec) if ((pPg)->busy>1) {\
  if ((pSec) < ((pPg)->psec + (pPg)->busy - 1))\
   memMove(pSec,(pSec)+1,(((pPg)->psec + (pPg)->busy - 1)-(pSec)) * sizeof(*((pPg)->psec)));\
  (pPg)->busy--;} else if ((pPg)->psec == (pSec)) (pPg)->busy=0
/*
 * Удалить поле со страницы
 */
#define	PgValDelref(pmLong,Pg,pVal) pValDelLong(pmLong,pVal); pPgDelVal(Pg,pVal)
 
#define	PgFreeAdrDel(Pg,pVal) pPgDelVal(Pg,pVal)
//перебор КлючДанные на странице
#define	PgDataNext(Pg,pData) (pData==NULL)?(Pg->psec):(pData < Pg->psec + Pg->busy-1 ))?pData+1:NULL
//Итератор 
#define	PgIter(Pg,pSec) for (pSec=(Pg->psec); (pSec < (Pg->psec) + Pg->busy);pSec++)
// * Найти ключ на странице PgData
#define	PgEquKey(pDb,Pg,pKey,pVal,err) (err)=CMPnoEq; PgIter(Pg,pVal)\
  if (((err)=DataStrEqu(pDb,(pKey),&(pValKey(pVal))))==CMPeq) break
/*
 * вставить Ключ и данные на страницу
*/
#define	PgInsipSec(Pg,pSec,ind) if (ind < (Pg)->size){\
 if (ind<(Pg)->busy){\
  memMove(pgOffpSec(Pg,ind)+1,pgOffpSec(Pg,ind),((Pg)->busy - ind) * sizeof(*((Pg)->psec)));\
  }} pgOffSec(Pg,ind)=*(pSec);(Pg)->busy++

#define	PgInsiSec(Pg,Sec,ind) if (ind < (Pg)->size){\
 if (ind<(Pg)->busy){\
  memMove(pgOffpSec(Pg,ind)+1,pgOffpSec(Pg,ind),((Pg)->busy - ind) * sizeof(*((Pg)->psec)));\
  }} pgOffSec(Pg,ind)=Sec;(Pg)->busy++
/*
 * Удалить ключ со страницы
 */
#define	PgDelpSec(Pg,pSec) if ((Pg)->busy > 1) {\
  if (pSec < pgOffpSec(Pg,(Pg)->busy-1))\
   memCopy(pSec,pSec+1,(pgOffpSec(Pg,(Pg)->busy-1) - pSec) * sizeof(*((Pg)->psec)));\
  (Pg)->busy--;} else (Pg)->busy=0
//вставить на страницу 0-данные
#define	PgInspSecFirst(Pg,pSec,err) if ((Pg)->size > (Pg)->busy) {\
 if ((Pg)->busy) {memMove(pgOffpSec(Pg,1),(Pg)->psec,sizeof(*((Pg)->psec)) * ((Pg)->busy));}\
 (Pg)->psec[0]=*(pSec); ((Pg)->busy)++; err=COutKodOk;} else err=CERRPgFull
#define	PgInsSecFirst(Pg,Sec,err) if ((Pg)->size > (Pg)->busy) {\
 if ((Pg)->busy) {memMove(pgOffpSec(Pg,1),(Pg)->psec,sizeof(*((Pg)->psec)) * ((Pg)->busy));}\
 (Pg)->psec[0]=Sec; ((Pg)->busy)++; err=COutKodOk;} else err=CERRPgFull
/*
 * Спуск по страницам TpgLink по правому или левому краев
 */
#define	PgDownLR(pDb,pAdr,tpN) (tpN==CcmdBack)?(PgDownR(pDb,pAdr)):(PgDownL(pDb,pAdr))
/*
 * дробление страницы Pg1 на 2 начиная с поля pkeyPg новая страница начинается с поля pkeyDiv
 *  страница Pg2 новая пустая
 */
#define	PgDivadrSec(Pg1,Pg2,adrSec) {int n; n=(Pg1)->busy-(adrSec);\
 memCopy(((Pg2)->psec + (Pg2)->busy),(Pg1)->psec +  adrSec,n * sizeof(*((Pg1)->psec)));\
 (Pg2)->busy +=n;(Pg1)->busy -=n;}
/*
 */ 
#define	KeysToPgLink(pvalKey1,pvalKey2,adrPg1,adrPg2,pgLink) ({Tkey keytemp;\
 ValKey(keytemp)=pValKey(pvalKey1); ValAdr(keytemp)=(adrPg1); AddSecToPg(pgLink,keytemp);\
 ValKey(keytemp)=pValKey(pvalKey2); ValAdr(keytemp)=(adrPg2); AddSecToPg(pgLink,keytemp);})

#define	ValToAdrAlt(Val,isAlt) (isAlt)?(ValAdrAlt(Val)):(ValAdr(Val))
#define	pValToAdrAlt(pVal,isAlt) (isAlt)?(pValAdrAlt(pVal)):(pValAdr(pVal))
 
#define	pValIndAdr(pVal,pInd) pValToAdrAlt(pVal,pDataIsAlt(pInd))
#define	ValIndAdr(Val,pInd) ValToAdrAlt(Val,pDataIsAlt(pInd))
#define	pValIndpAdr(pVal,pInd) (pDataIsAlt(pInd))?&(pValAdrAlt(pVal)):&(pValAdr(pVal))
// 
#define indNext(pVal,pind) (pValIndpAdr(pVal,pind));pind++

/*
 * вставить строку в списокстраниц
 */
#define LongInsStr(pLong,ind,pStrIn,pOff) ({Tstr *pStr;Tsize Size;Terr err; Size=mshDivUp(pStrSize(pStrIn),sizeof(Tsize));\
 if ((pStr=LongIns(pLong,ind,Size,pOff,&err))){ pStrTopStr(pStr,pStrIn); } err; })
//найти значение в массиве
#define mapArrpDataOffpDb(pDb,Off) ((maparrpDbBusy(pDb)) > Off)?mapArrpDataOff(pDb,Off):NULL
/*
*/
#define pgFree 0 
#define pgRoot(pDb) (pDb)->mpg.sizesec

#define pDbSteckInit(pDb) arrInit( (pDb)->trackdb,CountSecLayoutDef) 
#define pDbSteckFree(pDb) arrFree( (pDb)->trackdb) 
#define pDbSteckBusy(pDb) arrBusy((pDb)->trackdb)
#define pDbSteckPop(pDb) arrPop((pDb)->trackdb)
#define pDbSteckClear(pDb) arrClear((pDb)->trackdb)
// 
#define	treeDataInit(treeData) arrInit((treeData).Inds,CsizetreeDataDef);(treeData).Level=0;(treeData).isAlt=0;\
 (treeData).pkeydata=NULL;(treeData).pUser=NULL
#define	treeDataFree(treeData) arrFree((treeData).Inds)

#define DbLongClear(Db) ListLongClear(&((Db).mlong))
#define pDbLongClear(pDb) ListLongClear(&((pDb)->mlong))
 
/* Итераторы */ 

#define _contIterQuery TIterNext iterNext; TarrAdrOfferr stackDown; TarrData addInd; TarrData rootInd

typedef struct sttIterQuery {
 _contIterQuery;
 } TIterQuery;
 
#define	IterNextInit(Next,pDb,Type) memNULL(&(Next));(Next).pdb=pDb;(Next).type=Type

#define mshNextBack(pNext) ((pNext)->type==CcmdBack)?mshBack(pNext):mshNext(pNext)

#define QueryGetType(Query) ((TIterNext)(Query)).type
#define pQueryGetType(pQuery) ((TIterNext*)(pQuery))->type
 
#define QuerySetType(Query,St) ((TIterNext)(Query)).type =St
#define pQuerySetType(pQuery,St) ((TIterNext*)(pQuery))->type =St

#define QueryIsQuery(Query) ((TIterNext)(Query)).type == CcmdQuery
#define pQueryIsQuery(pQuery) ((TIterNext*)(pQuery))->type == CcmdQuery
#define QueryIsNoQuery(Query) ((TIterNext)(Query)).type != CcmdQuery
#define pQueryIsNoQuery(pQuery) ((TIterNext*)(pQuery))->type != CcmdQuery

#define QueryIsNext(Query) ((TIterNext)(Query)).type == CcmdNext
#define pQueryIsNext(pQuery) ((TIterNext*)(pQuery))->type == CcmdNext

#define QueryIsBack(Query) ((TIterNext)(Query)).type == CcmdBack
#define pQueryIsBack(pQuery) ((TIterNext*)(pQuery))->type == CcmdBack

#define	pQueryInit(pQuery,pDb) IterNextInit(pQuery->iterNext,pDb,CcmdQuery);\
 arrInit(pQuery->stackDown,CsizeIterQueryrDef);\
 arrInit(pQuery->addInd,CsizeIterQueryrDef);\
 arrInit(pQuery->rootInd,CsizeIterQueryrDef)
 
 
#define	QueryInit(Query,pDb) IterNextInit(Query.iterNext,pDb,CcmdQuery);\
 arrInit(Query.stackDown,CsizeIterQueryrDef);\
 arrInit(Query.addInd,CsizeIterQueryrDef);\
 arrInit(Query.rootInd,CsizeIterQueryrDef)

#define	pQueryFree(pQuery) arrFree(pQuery->stackDown);arrFree(pQuery->addInd);arrFree(pQuery->rootInd)
#define	QueryFree(Query) arrFree(Query.stackDown);arrFree(Query.addInd);arrFree(Query.rootInd)
 
#define	pQueryClear(pQuery) AdrOfferrSetNULL(pQuery->iterNext);arrClear(pQuery->stackDown);\
 arrClear(pQuery->addInd);arrClear(pQuery->rootInd);((TIterNext*)(pQuery))->type=0
#define	QueryClear(Query) AdrOfferrSetNULL(Query.iterNext);arrClear(Query.stackDown);\
 arrClear(Query.addInd);arrClear(Query.rootInd);((TIterNext)Query).type=0
 
#ifdef	__cplusplus
}
#endif

#endif	/* MSHDB_H */
