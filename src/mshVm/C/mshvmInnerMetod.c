
/* 
 * File:   mshvmInnerMetod.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 5 марта 2017 г., 15:33
 */
#include "mshvmVm.h"
#include "mshvmMacro.h"

void vmGetData(TvmDo *pDo,TrefDb *pDbArr);
void vmSetData(TvmDo *pDo,TrefDb *pDbArr,Tdata *pdSet);
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
void vmKillData(TvmDo *pDo,TrefDb *pDbArr,u_char subCmd);
void pArrTopData(TarrData *pArr, Tdata *pDataOut, Tarea *pArea);
void expandArrToArr(TarrData* parIn,TarrData* parTemp);
void expandDataToArr(Tdata *pD,TarrData* parTemp);
void pArrTopData(TarrData *pArr, Tdata *pDataOut, Tarea *pArea);
void vmfaindLabelArg(TvmDo *pDo,TarrData *pArgs, TrefProg *prefProg);
int roundDataDbAlt(TDb *pDb, TarrData *pInds,int isAlt, int (*pfdata)(TtreeData *ptreeData) , void *pUser);
//TvmMod* getMod(Tstr *pname,Terr *perr);
//Terr getLabelApp(TrefProg *prefProg);
//
Terr castDataInt(Tdata *pData);
Terr castNorm(Tdata *pData);
//
void vmQueryInitDb(TvmDo *pDo,TrefDb *pDbArr,void *pVoid);
void vmQueryDb(TvmDo *pDo,TrefDb *pDbArr,void *pVoid);
void castbStr(Tdata *pData,Tbstr* pbstrOut);
void bStrGet(Tdata *pData,Tbstr* pbstrOut);
Tstr* castId(Tdata *pData);
Tdata* mshGetArr(TDb *pDb,Tusize Off,Terr *perr);
void cmdExecDo(TvmDo *pDo,TarrData *pArgs,u_char flag,TdivStream *pdiv);
void mshVmDo(TvmDo *pDo, TdivStream *pdiv);
TvmDo* vmGetDo(TJob *pJob);
void stkindstorefdb(TvmDo *pDo,TarrData *prefInd,TrefDb *pDbArr,enum TisSet dbisSet);

void fileDirPacht(Tbstr *pname,const char *pDir,char *pout);
void cmdReadFile(TvmDo *pDo,Tbstr *pbStr);
void pbStrToDb(TvmDo *pDo,Tbstr *pbStr);
/*
 *  внутренние Методы
 *  ,"GetStr","CodeStr","PopStr","InsStr","SetStr","DelStr","FindStr","Repl"
 *  ,"GetField","PushField","PopField","InsField","SetField","DelField"
 *
 *  ,"GetList","PushList","PopList","InsList","SetList","DelList"
 *      Команда
 *    0. код команды = CvmCmdDO
 *    1. Sub команда = CvmSubCmdDOInMetod
 *    2.  источник
 *    Off3.  Id программы
 */
/**/
#define setArgCharDef(parrArgs,iStart,nArg) ({ iStart=1; if ((nArg)<parrArgs->busy) {castDataInt(parrOffpD(parrArgs,nArg));iStart=(parrOffpD(parrArgs,nArg))->intgr;} })
#define setArgCharnoTopDef(parrArgs,iStart,nArg) ({ iStart=1; if (nArg<parrArgs->busy-1) {castDataInt(parrOffpD(parrArgs,nArg));iStart=(parrOffpD(parrArgs,nArg))->intgr;} })

#define setArgCharCountDef(parrArgs,iStart,iCount,nArg) setArgCharDef(parrArgs,iStart,nArg);setArgCharDef(parrArgs,iCount,nArg+1)
#define setArgCharCountnoTopDef(parrArgs,iStart,iCount,nArg) setArgCharnoTopDef(parrArgs,iStart,nArg);setArgCharnoTopDef(parrArgs,iCount,nArg+1)
/**/
//#define setArgFieldDef(parrArgs,bsDel,iStart,nArg) castbStr(parrOffpD(parrArgs,1), &bsDel); setArgCharDef(parrArgs,iStart,nArg)
//#define setArgFieldnoTopDef(parrArgs,bsDel,iStart,nArg) castbStr(parrOffpD(parrArgs,1), &bsDel); setArgCharnoTopDef(parrArgs,iStart,nArg)
#define setArgFieldDef(parrArgs,bsDel,iStart,nArg) castbStr(parrArgs->psec, &bsDel); setArgCharDef(parrArgs,iStart,nArg)
#define setArgFieldnoTopDef(parrArgs,bsDel,iStart,nArg) castbStr(parrArgs->psec, &bsDel); setArgCharnoTopDef(parrArgs,iStart,nArg)

//#define setArgFieldCountDef(parrArgs,bsDel,iStart,iCount,nArg) castbStr(parrOffpD(parrArgs,1), &bsDel);setArgCharCountDef(parrArgs,iStart,iCount,nArg)
//#define setArgFieldCountnoTopDef(parrArgs,bsDel,iStart,iCount,nArg) castbStr(parrOffpD(parrArgs,1), &bsDel);setArgCharCountnoTopDef(parrArgs,iStart,iCount,nArg)
#define setArgFieldCountDef(parrArgs,bsDel,iStart,iCount,nArg) castbStr(parrArgs->psec, &bsDel);setArgCharCountDef(parrArgs,iStart,iCount,nArg)
#define setArgFieldCountnoTopDef(parrArgs,bsDel,iStart,iCount,nArg) castbStr(parrArgs->psec, &bsDel);setArgCharCountnoTopDef(parrArgs,iStart,iCount,nArg)

static char* getStartChar(Tbstr *pbStr,char *pC,int iStart){
 char *pE;
 int j;
 //
 if (!iStart) return pC;
 pE=pbStr->psec + pbStr->size;
 if ((pC+iStart)>=pE) return NULL;
 for (j=0;j < iStart;++j) {trSmbNext(pC);if (pC >= pE) return NULL;}
 return (pC<pE)?pC:NULL;
 }

static char* getStartCharNext(char *pC,char *pE,int iStart){
 int j;
 //
 if ((pC+iStart)>=pE) return pE;
 for (j=0;j++ < iStart;) {trSmbNext(pC);if (pC >= pE) return pE;}
 return (pC<pE)?pC:pE;
 }

static char* vmFindSubStr(Tbstr *pbStrFind,char *pC,char *pE){
 char *pS;
 //
 if (!(pbStrFind->size)) return NULL;
 if (pC > pE) return NULL;
 for (pS=pC;pS < pE ;trSmbNext(pS))
  if (!(memCompare(pS,pbStrFind->psec,pbStrFind->size))) return pS;
 return NULL;
 }

#define pDataInitArea(pData,pDo) pDataSetArea(pData); pData->areaOff.parea=&(pDo->Temps); pData->areaOff.off=pDo->Temps.busy
/**/
static void vmMetodOut(TvmDo *pDo,Tdata *pData,Tbstr *pbStrIn,Tbstr *pbStrIns,char *pcFinal,int n1){
 Tstr *pS;
 int n3,count,Count;
 char *pSec;
 Tsize *ps;
 //конец строки
 n3 = (pcFinal)?pbStrIn->psec + pbStrIn->size - pcFinal:0;
 count=n1+ n3;
 if (pbStrIns) count +=pbStrIns->size ;
 //
 pDataInitArea(pData, pDo);
 Count=count;
 areaBusyStr(pDo->Temps,Count,ps);
 pS=(Tstr*)ps;
 pS->size=count;
 pSec=pS->psec;
 if (n1) {memCopy(pSec,pbStrIn->psec,n1);pSec += n1;}
 if (pbStrIns) {memCopy(pSec,pbStrIns->psec,pbStrIns->size); pSec += pbStrIns->size;}
 if (n3) { memCopy(pSec,pcFinal,n3); }
 }
/*
static Tdata* vmPopgetStr(TvmDo *pDo) {
 if (pDo->pJob->thisObj.busy<2) {
  pDo->err=CERRvmIndFiled;
  return NULL;
  }
 stkindstorefdb(pDo,&(pDo->pJob->thisObj),&(pDo->refDb), EisSetNo);
 if (pDo->refDb.pvmDb==NULL){pDo->err=CERRvmDbFiled;return NULL;}
 if (pDo->err!=COutKodOk){pDo->err=CWARdataNotFound;return NULL;}
 Stk3toArr(stkInd(pDo),pDo->rarInd);
 if (pDo->rarInd.busy==0) {pDo->err=CWARdataNotFound;return NULL;}
 return pDo->rarInd.ptop;
 }
*/
/*
//запись в дерево параметров вершины
static void setDbPar(TvmDo *pDo,Tdata *pData){
 Tdata Data;
 Tbstr bStr;
 char *pB,*pC,*pE;
 //
 castbStr(pData,&bStr);
 pbStrToDb(pDo,&bStr);
 }
*/
//
static void iniRead(TvmDo *pDo, Tdata *pdataIni) {
 int Off,w;
 Tdata *pData;
 char fl[CsizePacthFl];
 Tbstr bsIn;
 Tbstr bStr;
 //
 
 castbStr(pdataIni, &bsIn);
 if (bsIn.size<1) return;
 fileDirPacht(&bsIn,pCmshDirIni,fl);
 bsIn.size=strlen(fl);
 bsIn.psec=fl;
 cmdReadFile(pDo,&bsIn);
 if ((w=maparrDbBusy(pDo->pJob->vmDbBuf.db))<1) {pDo->err=CERRdataNotFound;return;}
 //перебор введенных строк
 if (pDo->refDb.pvmDb->plock) { pLockW(pDo->refDb.pvmDb->plock); }
 for (Off=1;Off<=w;++Off){
  if ((pData=mshGetArr(&(pDo->pJob->vmDbBuf.db),Off,&(pDo->err)))){
   castbStr(pData,&bStr);
   pbStrToDb(pDo,&bStr);
   }
  }
 if (pDo->refDb.pvmDb->plock) { unpLock(pDo->refDb.pvmDb->plock); }
 }
//
static void metodiniRead(TvmDo *pDo, TarrData *pArgs) {
 Tstrshr dlmIO;
 int j;
 //
 if (pArgs->busy<2) {pDo->err=CERRdataNotFound;return;}
 arrBusy(pDo->pJob->thisObj);
 if (pDo->pJob->thisObj.busy<3) {pDo->err=CERRvmIndFiled;return;}
 pDataSetbStr(pDo->pJob->thisObj.ptop);
 stkindstorefdb(pDo,&(pDo->pJob->thisObj),&(pDo->refDb), EisSet);
 if (pDo->refDb.pvmDb==NULL){pDo->err=CERRvmDbFiled;return;}
 dlmIO=pDo->pJob->dlmIO;
 pDo->pJob->dlmIO.psec[0]=CCvmEndStr;
 pDo->pJob->dlmIO.psec[1]=Cvm0;
 pDo->pJob->dlmIO.size=1;
 for (j=1;j<pArgs->busy;j++){
  iniRead(pDo, pArgs->psec+j);
  }
 pDo->pJob->dlmIO=dlmIO;
 }
//
static void metodiniWrite(TvmDo *pDo, TarrData *pArgs) {
 Tdiv fFl;
 Tbstr bsKey,bsData;
 Tdata *pdataKey,*pdataData;
 int w;
 int sum=0;
 char Eq=CvmIniEqu;
 char BK=CCvmEndStr;
 char Es=CvmIniendStr;
 Tbstr bsIn;
// TarrData thisInd;
 char bf[CsizePacthFl];
 TvmQuery *pQuery;
 //
 if (pArgs->busy<2) {pDo->err=CERRdataNotFound;return;}
 castbStr(pArgs->psec+1, &bsIn);
 if (bsIn.size<1) {pDo->err=CERRdataNotFound;return;}
// Stk2toArr(pDo->pJob->stkthis,thisInd);
 if (pDo->pJob->thisObj.busy<2) {pDo->err=CERRvmIndFiled;return;}
 stkindstorefdb(pDo,&(pDo->pJob->thisObj),&(pDo->refDb), EisSetNo);
 if (pDo->err!=COutKodOk) return;
 // pbStrIn имя файла сформировать pDo->refDb
 fileDirPacht(&bsIn,pCmshDirIni,bf);
 if ((fFl=divCreat(bf))==CdivRetErr) {pDo->err=CERRvmDivFiled;return;}
 //Обход дерева
 arrBoxPush(pDo->curquery);
 pQuery=pDo->curquery.ptop;
 //индекс автоматически нигде не сохранять
 pDo->curquery.ptop->qrefDbSave.pvmDb=NULL;
 //
 IterNextInit(pDo->curquery.ptop->iterNext,&(pDo->refDb.pvmDb->db),CcmdNext);
 vmQueryInitDb(pDo,&(pDo->refDb),pQuery);
 if (pQuery->iterNext.AdrOfferr.err<CERR) goto finish;
 //данных не обнаружено
 if (pQuery->iterNext.AdrOfferr.err == CMPnull) {
  //сбросить уровень стека Итератора
  pDo->err=CERRvmdataNotFound;
  goto finish;
  }
 do {
  //
  pdataKey=&(ValKey(pDo->curquery.ptop->iterNext.val));
  castbStr(pdataKey,&bsKey);
  if (bsKey.psec==NULL) {pDo->err=CWARvmArgNot;goto finish;}
  pdataData=&(ValData(pDo->curquery.ptop->iterNext.val));
  castbStr(pdataData,&bsData);
  if (bsData.psec==NULL) {pDo->err=CWARvmArgNot;goto finish;}
  if ((w=divWrite(fFl,bsKey.psec,bsKey.size))<bsKey.size) {pDo->err=CERRvmDivFiled;goto finish;}
  divWrite(fFl,&Eq,1);
  divWrite(fFl,bsData.psec,bsData.size);
  divWrite(fFl,&Es,1);
  divWrite(fFl,&BK,1);
  //
  ++sum;
  vmQueryDb(pDo,&(pDo->refDb),pDo->curquery.ptop);
  }while( (pQuery->iterNext.AdrOfferr.err >CERR)&&(pQuery->iterNext.AdrOfferr.err != CMPnull)   );
 //если цикл завершен сбросить уровень стека Итератора
finish:
 flush(fFl); 
 arrBoxPop(pDo->curquery);
close:
 divClose(fFl);
 if (pDo->err>CERR) pDo->err=COutKodOk;
 }
//
static void getStart(TarrData *pArgs,int *piStart,int nA){
 *piStart=1;
 if (pArgs->busy>nA){castDataInt(pArgs->psec+nA); *piStart=pArgs->psec[nA].intgr;}
 }
//
static void getStartCount(TarrData *pArgs,int *piStart,int *piCount,int nA){
 *piStart=*piCount=1;
 if (pArgs->busy>nA){castDataInt(pArgs->psec+nA); *piStart=pArgs->psec[nA].intgr;
  if (pArgs->busy>(nA+1)){nA++; castDataInt(pArgs->psec+nA); *piCount=pArgs->psec[nA].intgr; }
  }
 }
//
static void vmMetodGet(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pDataOut) {
 char *pC, *pE;
 Tbstr bStr;
 int iStart,iCount;
 //
 getStartCount(pArgs,&iStart,&iCount,1);
 pDataSetStr(pDataOut);
 if (iStart>1) {
  if ((pC = getStartChar(pbStrIn, pbStrIn->psec, iStart-1))==NULL) return;
  }
 else pC = pbStrIn->psec;
 if ((pE = getStartChar(pbStrIn, pC, iCount))==NULL) pE=pbStrIn->psec + pbStrIn->size;
 bStr.psec = pC;
 bStr.size = pE - pC;
 pStrTopDataOrpArea(&bStr,&(pDo->Temps),pDataOut);
 }

static void vmMetodCode(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pDataOut) {
 char *pC, *pE;
 int iStart;
 //
 getStart(pArgs,&iStart,1);
 if (iStart > 1) pC = getStartChar(pbStrIn, pbStrIn->psec, iStart-1); else pC = pbStrIn->psec;
 if (pC==NULL) return;
 pE=pC;
 trSmbNext(pE);
 if (pC < pE) {
  pDataSetInt(pDataOut);
  pDataOut->intgr = 0;
  memCopy(pDataOut->st, pC, pE - pC);
  }
 }
//
static void metodSavePop(TvmDo *pDo,char *pC,char *pE,int isList){
 Tbstr bsPop;
 Tdata Data;
 //
 bsPop.psec = pC;
 bsPop.size = pE-pC;
 //
 if (bsPop.size <= 0){
  DataSetNULL(Data);
  }
 else{
  pStrTopDataOrpArea(&bsPop,&(pDo->Temps),&Data);
  castNorm(&Data);
  }
 if ((isList)&&(bsPop.size>sizeof(Tdata))){ DataListSet(Data); }
 vmSetData(pDo,&(pDo->refDb),&Data);
 }

static void vmMetodPop(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pOut) {
 char *pC,*pE;
 Tbstr bsPop;
 int iCount;
 //
 if (pbStrIn->size<1) return;
 getStart(pArgs,&iCount,1);
 if (!iCount) return;
 pE=pbStrIn->psec + pbStrIn->size;
 if ((pC = getStartCharNext(pbStrIn->psec,pE, iCount))==NULL) pC=pE;
 if (pOut){
  bsPop.psec = pbStrIn->psec;
  bsPop.size = pC - pbStrIn->psec;
  pStrTopDataOrpArea(&bsPop,&(pDo->Temps),pOut);
  castNorm(pOut);
  }
 metodSavePop(pDo,pC,pE,0);
 }
/*
 */
static void vmMetodIns(TvmDo *pDo,TarrData *pArgs,Tbstr *pbStrIn, Tdata *pData) {
 int n1;
 Tbstr bsIns;
 char *pB;
 int iStart;
 //
 getStart(pArgs,&iStart,1);
 //вставляемая строка
 castbStr(pArgs->ptop, &bsIns);
 if (bsIns.size < 1) return;
 //начало
 n1=0;
 pB = pbStrIn->psec;
 if (iStart > 1) {
  if ((pB = getStartChar(pbStrIn, pbStrIn->psec, iStart-1))==NULL) return;
  n1 = pB - pbStrIn->psec;
  }
 vmMetodOut(pDo,pData,pbStrIn,&bsIns,pB,n1);
 }
//
static void vmMetodSet(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pData) {
 int n1;
 Tbstr bIns;
 char *pB,*pE;
 int iCount=1, iStart;
 //
 if (pArgs->busy < 2) return;
 //вставляемая строка
 castbStr(pArgs->ptop, &bIns);
 if (bIns.size < 1) return;
 if (pArgs->busy < 3)
  getStart(pArgs,&iStart,1);
 else
  getStartCount(pArgs,&iStart,&iCount,1);
 //начало
 n1=0;
 pB = pbStrIn->psec;
 //начало
 if (iStart>1) {
  castDataInt(pArgs->psec);
  if ((pB = getStartChar(pbStrIn, pbStrIn->psec, iStart-1))==NULL) return;
  n1 = pB - pbStrIn->psec;
  }
 pE = pB;
 if (iCount) {
  if ((pE = getStartChar(pbStrIn, pB, iCount))==NULL) pE=pbStrIn->psec+pbStrIn->size;
  }
 vmMetodOut(pDo,pData,pbStrIn,&bIns,pE,n1);
 }
/*
*/
static void vmMetodDel(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pData) {
 int n1;
 char *pB,*pE;
 int iCount, iStart;
 //
 getStartCount(pArgs,&iStart,&iCount,1);
 if (!iCount) return;
 //начало
 n1=0;
 pB = pbStrIn->psec;
 //начало
 if (iStart>1) {
  castDataInt(pArgs->psec);
  if ((pB = getStartChar(pbStrIn, pbStrIn->psec, iStart-1))==NULL) return;
  n1 = pB - pbStrIn->psec;
  }
 if ((pE = getStartChar(pbStrIn, pB, iCount))==NULL) pE=pbStrIn->psec+pbStrIn->size;
 vmMetodOut(pDo,pData,pbStrIn,NULL,pE,n1);
 }
/*
 * в строке найти подстроку
 */
static void vmMetodFind(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pDataOut) {
 char *pC,*pE;
 int Count,iStart;
 Tbstr bsSub;
 //
 if (pArgs->busy < 2) return;
 castbStr(parrOffpD(pArgs,1), &bsSub);
 if (!(bsSub.size)) return;
 getStart(pArgs,&iStart,2);
 //
 if (iStart > 1) {if ((pC = getStartChar(pbStrIn, pbStrIn->psec, iStart-1))==NULL) return;}
 else pC = pbStrIn->psec;
 pE=pbStrIn->psec + pbStrIn->size - bsSub.size;
 for (Count=0;pC < pE ;++Count){
  if (!(memCompare(pC,bsSub.psec,bsSub.size))) {
   pDataSetInt(pDataOut);
   pDataOut->intgr = iStart + Count;
   return;
   }
  trSmbNext(pC);
  }
 pDo->err=COutFound;
 }
/*
 * Замена символов
 */
static void vmMetodReplSub(TvmDo *pDo,Tbstr *pbStrIn,Tbstr *pbStrFind,Tbstr *pbStrRepl,char *pC,char *pE, Tdata *pData,Toff busy,int isRecur) {
 //
 if ( (pC = vmFindSubStr(pbStrFind,pC,pE) ) ) {
  Tbstr bStr;
  //начало строки
  vmMetodOut(pDo,pData,pbStrIn,pbStrRepl,pC + pbStrFind->size,pC - pbStrIn->psec);
  if (isRecur) {
   if (pData->areaOff.off > busy){
    Tsize *pTo,*pFrom;
    pTo=pareaOff(pData->areaOff.parea,busy);
    pFrom=pareaOff(pData->areaOff.parea,pData->areaOff.off);
    memMove(pTo,pFrom,pData->areaOff.parea->busy -  pData->areaOff.off);
    pData->areaOff.parea->busy -= pFrom - pTo;
    pData->areaOff.off=busy;
    }
   bStrGet(pData,&bStr);
   (void)vmMetodReplSub(pDo,&bStr,pbStrFind,pbStrRepl,bStr.psec,bStr.psec +bStr.size - pbStrFind->size, pData,busy,isRecur);
   }
  }
 else
  pDo->err=COutFound;
 }

static void vmMetodRepl(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pData ,int isRecur) {
 int count;
 Tbstr bStrFind, bStrRepl,*pbStrRepl;
 Toff busy;
 //
 busy=pDo->Temps.busy;
 pDataSetNULL(pData);
 castbStr(parrOffpD(pArgs,1),&bStrFind);
 count=pbStrIn->size - bStrFind.size;
 pbStrRepl=NULL;
 if (pArgs->busy > 2) {
  castbStr(parrOffpD(pArgs,2) ,&bStrRepl);
  count += bStrRepl.size;
  pbStrRepl=&bStrRepl;
  }
 //символов в строке не может быть, сохранить исходную строку
 if (count <=0) { pStrTopDataOrpArea(pbStrIn,&(pDo->Temps),pData);}
 else
  vmMetodReplSub(pDo,pbStrIn,&bStrFind,pbStrRepl,pbStrIn->psec,pbStrIn->psec + pbStrIn->size - bStrFind.size,pData,busy,isRecur);
 }
/*
 */
static char* vmFindFieldNum(char *pC, char *pE, Tbstr *pDel, int Num) {
 int i;
 Tbstr bS;
 //
 if (Num<2) return pC;
 pE -=pDel->size;
 bS.size = pDel->size;
 bS.psec = pC;
 i = 1;
 while (bS.psec  <= pE) {
  if (StrpEqu(bS, pDel) == CMPeq) {
   if (++i >= Num) return bS.psec;
   bS.psec += bS.size;
   }
  else {  trSmbNext(bS.psec);  }
  }
 return NULL;
 }
/*
 */
static int vmFindFieldCount(Tbstr *pbStrIn, Tbstr *pDel) {
 int i;
 Tbstr bS;
 char *pE;
 //
 bS=*pbStrIn;
 bS.size = pDel->size;
 pE =pbStrIn->psec + pbStrIn->size - pDel->size;
 i = 1;
 while (bS.psec  <= pE) {
  if (StrpEqu(bS, pDel) == CMPeq) {
   bS.psec += pDel->size;
   ++i;
   }
  else {  trSmbNext(bS.psec);  }
  }
 return i;
 }
/*
 * 
 */
static void vmMetodFieldGet(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pData) {
 char *pB, *pE, *pC;
 Tbstr bsDel;
 int iStart, iCount;
 //
 getStartCount(pArgs,&iStart,&iCount,2);
 castbStr(parrOffpD(pArgs,1),&bsDel);
 if ((bsDel.psec == NULL) || (bsDel.size < 1) || (!iCount) ) return;
 pB=pbStrIn->psec;
 pE = pbStrIn->psec + pbStrIn->size;
 if (iStart > 1 ) {
  if ((pB = vmFindFieldNum(pbStrIn->psec, pE, &bsDel, iStart))==NULL) return;
  pB += bsDel.size;
  }
 if ((pC = vmFindFieldNum(pB, pE, &bsDel, iCount+1)) == NULL) pC=pE;
 pDataSetbStr(pData);
 pData->bstr.psec = pB;
 pData->bstr.size = pC - pB;
 }

static void vmMetodFieldPush(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pData) {
 Tbstr bsDel,bsIns;
 int count,Count;
 char *pSec;
 Tstr *pS;
 Tsize *ps;
 //
 if (pArgs->busy < 2) return;
 castbStr(parrOffpD(pArgs,1), &bsDel);
 if (pArgs->busy < 3) {memNULL(&bsIns);} else castbStr(pArgs->ptop , &bsIns);
 Count=count=pbStrIn->size + bsIns.size;
 pDataInitArea(pData, pDo);
 //исходная строка пустая
 if (pbStrIn->size==0){
  areaBusyStr(pDo->Temps,Count,ps);
  pS=(Tstr*)ps;
  pS->size=count;
  pSec=pS->psec;
  }
 else{
  count += bsDel.size;
  Count=count;
  areaBusyStr(pDo->Temps,Count,ps);
  pS=(Tstr*)ps;
  pS->size=count;
  pSec=pS->psec;
  memCopy(pSec,pbStrIn->psec,pbStrIn->size);pSec += pbStrIn->size;
  if (bsDel.size>0) {memCopy(pSec,bsDel.psec,bsDel.size); pSec += bsDel.size;}
  }
 if (bsIns.size>0) memCopy(pSec,bsIns.psec,bsIns.size);
 }
/*
 */
static void vmMetodFieldPop(TvmDo *pDo, TarrData *pArgs, Tbstr *pbStrIn, Tdata *pOut) {
 char *pE, *pC;
 Tbstr bsDel, bsPop;
 int iCount;
 //
 if ((pArgs->busy < 2)||(pbStrIn->size<1)) return;
 castbStr(parrOffpD(pArgs,1), &bsDel);
 if (bsDel.size<1) return;
 getStart(pArgs,&iCount,2);
 pE = pbStrIn->psec + pbStrIn->size;
 if ((pC = vmFindFieldNum(pbStrIn->psec, pE, &bsDel, iCount+1)) == NULL) {
  bsPop.psec = pbStrIn->psec;
  bsPop.size = pE - pbStrIn->psec;
  pStrTopDataOrpArea(&bsPop,&(pDo->Temps),pOut);
  castNorm(pOut);
  pC=pE;
  metodSavePop(pDo,pC,pE,0);
  return;
  }
 if (pOut){
  bsPop.psec = pbStrIn->psec;
  bsPop.size = pC - pbStrIn->psec;
  pStrTopDataOrpArea(&bsPop,&(pDo->Temps),pOut);
  castNorm(pOut);
  }
 pC += bsDel.size;
 metodSavePop(pDo,pC,pE,0);
 }
/*
 */
static void vmMetodFieldIns(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pData) {
 int iStart;
 Tbstr bsIns, bsDel;
 int count,Count;
 char *pSec;
 Tstr *pS;
 Tsize *ps;
 char *pB, *pE;
 int n;
 //
 if (pArgs->busy < 2) return;
 castbStr(parrOffpD(pArgs,1), &bsDel);
 if (bsDel.size<1) return;
 getStart(pArgs,&iStart,2);
 //вставляемая строка
 if (pArgs->busy < 3) {memNULL(&bsIns); } else castbStr(pArgs->ptop, &bsIns);
 //
 Count=count=pbStrIn->size + bsIns.size + bsDel.size;
 pDataInitArea(pData, pDo);
 areaBusyStr(pDo->Temps,Count,ps);
 pS=(Tstr*)ps;
 pS->size=count;
 pSec=pS->psec;
 pB=pbStrIn->psec;
 pE=pbStrIn->psec + pbStrIn->size;
 //начало строки
 if (iStart>1){
  if ((pB = vmFindFieldNum(pbStrIn->psec, pE, &bsDel, iStart))==NULL) return;
  if (pB<pE) pB += bsDel.size;
  n=pB-pbStrIn->psec;
  memCopy(pSec,pbStrIn->psec,n);pSec += n;
  }
 //вставка
 memCopy(pSec,bsIns.psec,bsIns.size); pSec += bsIns.size;
 //конец строки
 if (pB<pE){
  memCopy(pSec,bsDel.psec,bsDel.size); pSec += bsDel.size;
  memCopy(pSec,pB,pE-pB);
  }
 }

static void vmMetodSetField(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pData) {
 int iStart, iCount;
 Tbstr bsDel, bsIns;
 int count,Count;
 char *pSec;
 Tstr *pS;
 Tsize *ps;
 char *pB,*pE,*pC;
 int n;
 //
 if (pArgs->busy < 3) return;
 castbStr(parrOffpD(pArgs,1), &bsDel);
 if (bsDel.size<1) return;
 getStartCount(pArgs,&iStart,&iCount,2);
 //вставляемая строка
 if (pArgs->busy == 4) {memNULL(&bsIns); } else castbStr(pArgs->ptop, &bsIns);
 //
 pE=pbStrIn->psec+pbStrIn->size;
 pB=pbStrIn->psec;
 count=bsIns.size;
 if (iStart>1){
  if ((pB = vmFindFieldNum(pbStrIn->psec, pE, &bsDel, iStart))==NULL) return;
  if (pB<pE) pB += bsDel.size;
  count += pB-pbStrIn->psec;
  }
 if ((pC = vmFindFieldNum(pB, pE, &bsDel, iCount+1))==NULL) pC=pE;
 if (pC<pE) count+=pE-pC;
 Count=count;
 pDataInitArea(pData, pDo);
 areaBusyStr(pDo->Temps,Count,ps);
 pS=(Tstr*)ps;
 pS->size=count;
 pSec=pS->psec;
 if (pB>pbStrIn->psec){
  n=pB-pbStrIn->psec;
  memCopy(pSec,pbStrIn->psec,n);pSec += n;
  }
 //вставка
 if (bsIns.size){
  memCopy(pSec,bsIns.psec,bsIns.size); pSec += bsIns.size;
  }
 //конец строки
 if (pC<pE){
  memCopy(pSec,pC,pE-pC);
  }
 }

static void vmMetodDelField(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pData) {
 int iStart, iCount;
 Tbstr bsDel;
 int count,Count;
 char *pSec;
 Tstr *pS;
 Tsize *ps;
 char *pB,*pE,*pC;
 int n;
 //
 if (pArgs->busy < 3) return;
 castbStr(parrOffpD(pArgs,1), &bsDel);
 if (bsDel.size<1) return;
 getStartCount(pArgs,&iStart,&iCount,2);
 count=0;
 pE=pbStrIn->psec+pbStrIn->size;
 pB=pbStrIn->psec;
 if (iStart>1){
  if ((pB = vmFindFieldNum(pbStrIn->psec, pE, &bsDel, iStart))==NULL) return;
  count += pB-pbStrIn->psec;
  }
 if ((pC = vmFindFieldNum(pB + bsDel.size, pE, &bsDel, iCount+1))==NULL)
  pC=pE;
 if (pC<pE) {
  if (pB==pbStrIn->psec) pC += bsDel.size;
  count += pE-pC;
  }
 if (!count) return;
 Count=count;
 pDataInitArea(pData, pDo);
 areaBusyStr(pDo->Temps,Count,ps);
 pS=(Tstr*)ps;
 pS->size=count;
 pSec=pS->psec;
 //Начало строки
 if (pB>pbStrIn->psec){
  n=pB-pbStrIn->psec;
  memCopy(pSec,pbStrIn->psec,n);pSec += n;
  }
 //конец строки
 if (pC<pE){
  memCopy(pSec,pC,pE-pC);
  }
 }
/**/
static void vmMetodCountField(TarrData *pArgs, Tbstr *pbStrIn, Tdata *pDataOut) {
 Tbstr bsDel;
 if (pArgs->busy < 1) return;
 castbStr(parrOffpD(pArgs,1), &bsDel);
 pDataSetInt(pDataOut);
 pDataOut->intgr=vmFindFieldCount( pbStrIn,&bsDel);
 }
/* --------------------------------
 *          Методы списков
 */
static void inDataToArr(TvmDo *pDo,TarrData *pArgs,int beg,int count) {
 TarrData arD;
 //
 arD.busy=arD.size=count;
 arD.psec=pArgs->psec + beg;
 if (count==1) arD.ptop=arD.psec; else arrsetptop(arD);
 expandArrToArr(&arD,&(pDo->pJob->arrTemp) );
 }
/*
 */
static void vmMetodGetList(TvmDo *pDo,TarrData *pArgs,Tbstr *pbsIn , Tdata *pDataOut) {
 int iStart,iCount;
 TarrData arOut,arIn;
 //
 getStartCount(pArgs,&iStart,&iCount,1);
 //исходная строка не список
 if (!(pDataIsList(pArgs->psec))){
  if (iStart<2){ *pDataOut=*(pArgs->psec); }
  return;
  }
 //исходная строка список
 pStrToArrData(pbsIn,arIn);
 if (iStart >= arIn.busy) return;
 arOut.psec=(iStart)?(arIn.psec+iStart-1):arIn.psec;
 arOut.busy=arOut.size=arOut.capa=iCount;
 arrsetpTop(arOut);
 pArrTopData(&arOut,pDataOut, &(pDo->Temps)); 
 }
/**/
static void pushList(TvmDo *pDo,TarrData *parrIn,TarrData *parrPush,Tdata *pDataOut) {
 if ((parrIn==NULL)||(parrIn->busy==0) ){
  pArrTopData(parrPush, pDataOut ,&(pDo->Temps));
  return;
  }
 parrCopy(&(pDo->pJob->arrTemp),parrIn);
 parrExpand(&(pDo->pJob->arrTemp),parrPush);
 pArrTopData(&(pDo->pJob->arrTemp), pDataOut ,&(pDo->Temps));
 }
/**/
static void vmMetodPushList(TvmDo *pDo,TarrData *pArgs , Tdata *pDataOut) {
 //
 if (pArgs->busy < 2) return;
 arrClear(pDo->pJob->arrTemp);
 (void)inDataToArr(pDo,pArgs,0,1);
 (void)inDataToArr(pDo,pArgs,1,pArgs->busy-1);
 pArrTopData(&(pDo->pJob->arrTemp), pDataOut ,&(pDo->Temps));
 }
//
static void vmMetodPopList(TvmDo *pDo,TarrData *pArgs, Tbstr *pbStrIn, Tdata *pOut) {
 int iCount,Count;
// Tbstr bsPop,bStrIn;
 Tdata *pDataIn,Data;
 TarrData arD;
 //
 if (pbStrIn->size<1) return;
 getStart(pArgs,&iCount,1);
 if (!iCount) return;
 pDataIn=pArgs->psec;
 if ( pDataIsNULL(pDataIn) ) {pDo->err=CERRdataNotFound;return;}
 //исходная строка не список
 if (!(pDataIsList(pDataIn))){
  //вынести это поле в результат
  if (pOut){
   *pOut=*pDataIn;
   //исходную строку удалить
   if (pDataIsNoNULL(pOut)) goto err; //vmKillData(pDo,&(pDo->refDb),CtypeKiLLData);
   }
  return;
  }
 arrClear(pDo->pJob->arrTemp);
 expandDataToArr( pDataIn,&(pDo->pJob->arrTemp) );
 Count=pDo->pJob->arrTemp.busy;
 if (Count==1){
  if (pOut){
   *pOut=*pDataIn;
   castNorm(pOut);
   }
  //исходную строку удалить
  goto err;
  }
 if (iCount>=Count) iCount=Count;
 if (pOut){
  if (iCount==1) {
   *pOut=pDo->pJob->arrTemp.psec[0];
   castNorm(pOut);
   }
  else{
   arD.psec=pDo->pJob->arrTemp.psec;
   arD.busy=arD.size=iCount;
   arrsetptop(pDo->pJob->arrTemp);
   pArrTopData(&arD,pOut, &(pDo->Temps)); 
   }
  }
 //вытолкнули весь массив
 if (iCount>=Count) goto err;
 //остаток поместить в исходную строку
 arD.psec=pDo->pJob->arrTemp.psec+iCount;
 arD.busy=arD.size=Count-iCount;
 if (arD.busy<2) {
  castNorm(arD.psec);
  vmSetData(pDo,&(pDo->refDb),arD.psec);
  }
 else{
  arrsetptop(arD);
  pArrTopData(&arD,&Data, &(pDo->Temps)); 
  DataListSet(Data);
  vmSetData(pDo,&(pDo->refDb),&Data);
  }
 return;
err: DataSetNULL(Data);
 vmSetData(pDo,&(pDo->refDb),&Data);
 }
//

static void vmInsList(TvmDo *pDo,TarrData *pArgs,int iStart,int iCount,int beg, Tdata *pDataOut) {
 TarrData arIn,arD,arOut;
 //исходный массив
 arrClear(pDo->pJob->arrTemp);
 expandDataToArr(pArgs->psec,&(pDo->pJob->arrTemp) );
 //исходный массив
 arIn=arD=pDo->pJob->arrTemp;
 //выбираем начало
 if (iStart>1) {
  arD.busy=arD.size=iStart-1;
  arrsetptop(arD);
  expandArrToArr(&arD,&(pDo->pJob->arrTemp) );
  //если вставка за массивом добавить пустые поля
  if ( (iStart-arIn.busy-1)>0 ) {
   arrExtNull(pDo->pJob->arrTemp,iStart-arIn.busy-1 );
   }
  }
 // добавляем вставку
 if (beg){
  arD.psec=pArgs->psec+beg;
  arD.busy=arD.size=pArgs->busy-beg;
  arrsetptop(arD);
  if (arD.busy<2)
   expandDataToArr(arD.psec,&(pDo->pJob->arrTemp) );
  else{
   arrsetptop(arD);
   expandArrToArr(&arD,&(pDo->pJob->arrTemp) );
   }
  }
 //добавляем остаток строки
 iStart +=iCount;
 if (iStart<=arIn.busy) {
  arD.psec=arIn.psec+iStart-1;
  arD.busy=arD.size=arIn.busy-iStart+1;
  if (arD.busy==1)
   expandDataToArr(arD.psec,&(pDo->pJob->arrTemp) );
  else{
   arrsetptop(arD);
   expandArrToArr(&arD,&(pDo->pJob->arrTemp) );
   }
  }
 arOut.busy=pDo->pJob->arrTemp.busy-arIn.busy;
 if (arOut.busy){
  arOut.psec=pDo->pJob->arrTemp.psec+arIn.busy;
  arrsetptop(arOut);
  pArrTopData(&arOut, pDataOut ,&(pDo->Temps));
  }
 }
static void vmMetodInsList(TvmDo *pDo,TarrData *pArgs, Tdata *pDataOut) {
 int iStart;
 //нечего вставлять
 if (pArgs->busy<3) return;
 getStart(pArgs,&iStart,1);
 if (iStart<1) return;
 vmInsList(pDo,pArgs,iStart,0,2,pDataOut);
 }
//
static void vmMetodSetList(TvmDo *pDo,TarrData *pArgs, Tdata *pDataOut) {
 int iStart , iCount;
 //нечего вставлять
 if (pArgs->busy<4) return;
 getStart(pArgs,&iStart,1);
 if (iStart<1) return;
 getStart(pArgs,&iCount,2);
 vmInsList(pDo,pArgs,iStart,iCount,3,pDataOut);
 }
//
static void vmMetodDelList(TvmDo *pDo,TarrData *pArgs, Tdata *pDataOut) {
 int iStart , iCount;
 //
 if (pArgs->busy<2) return;
 getStart(pArgs,&iStart,1);
 if (iStart<1) return;
 getStart(pArgs,&iCount,2);
 vmInsList(pDo,pArgs,iStart,iCount,0,pDataOut);
 }
/**/
static void vmMetodCountList(TvmDo *pDo,TarrData *pArgs, Tdata *pDataOut) {
 //
 arrClear(pDo->pJob->arrTemp);
 inDataToArr(pDo,pArgs,1,1);
 pDataSetInt(pDataOut);
 pDataOut->intgr=pDo->pJob->arrTemp.busy;
 }
//запись в дерево параметров вершины
static void setDbParMod(TvmDo *pDo,TarrData *pArgs){
 Tdata Data,*pD;
 Tbstr bStr;
 char *pB,*pC,*pE;
 TarrData arD;
 //
 //данные находятся в pDo->refDb.fullInds его и расширяем
 pDataSetAlt(pDo->refDb.rInds.ptop);
 arrBusy(pDo->refDb.fullInds);
 pDo->refDb.rInds.psec=pDo->refDb.fullInds.psec+1;
 pDo->refDb.rInds.busy++;
 arrsetptop(pDo->refDb.rInds);
 pDataSetbStr(pDo->refDb.rInds.ptop);
 arD.busy=arD.size=1;
 arD.psec=arD.ptop=&Data;
 pDo->pJob->psLab=&(pDo->pJob->sLab) ;
 for (pD=pArgs->psec+1;pD<pArgs->ptop;pD++) {
  castbStr(pD,&bStr);
  if (bStr.size){
   pB=bStr.psec;
   pE=pB+bStr.size;
   trSmbSpace(pB,pE);
   if (pB<pE){
    if ((pC=memFindChar(pB,CvmIniEqu,pE-pB))){
     pDo->refDb.rInds.ptop->bstr.psec=pB;
     pDo->refDb.rInds.ptop->bstr.size=pC-pB;
     pB=pC+1;
     trSmbSpace(pB,pE);
     if (*(pE-1)==CCvmBK) --pE;
     if (*(pE-1)==CvmIniendStr) --pE;
     if (pB<pE) {
      Data.bstr.psec=pB;
      Data.bstr.size=pE-pB;
      DataSetbStr(Data);
      if ((pDo->err=castNorm(&Data))>CERR) {
       StrToStrID(pDo->pJob->sLab,pDo->refDb.rInds.ptop->bstr);
       cmdExecDo(pDo,&arD,CvmSubCmdDODo,NULL);
       //если обработчик не найден то просто записать параметр
       if (pDo->err<CERR) {
        pDo->err=mshSet(&(pDo->refDb.pvmDb->db),&(pDo->refDb.rInds),&Data);
        }
       }
      }
     }
    }
   }
  }
 }
/**/
void tstvmprnDo(TvmDo *pDo,TCmd *pcurCmd,TdivStream *pdiv);
void vmtstWriteArrData(TarrData *pArr,TdivStream *pdiv);
//
static void vmMetodobjNew(TvmDo *pDo, TarrData *pArgs) {
// TarrData thisInd;
 //
 if (pArgs->busy<1){pDo->err=CERRvmNotArg;return;}
// Stk2toArr(pDo->pJob->stkthis,thisInd);
 if (pDo->pJob->thisObj.busy<2) {pDo->err=CERRvmIndFiled;return;}
 stkindstorefdb(pDo,&(pDo->pJob->thisObj),&(pDo->refDb), EisSet);
 if (pDo->err<CERR) return;
 //запишем в вершину тип переменной
 (void)castId(pArgs->ptop);
 pDataSetObj(pArgs->ptop);
 vmSetData(pDo,&(pDo->refDb),pArgs->ptop);
 /*
 //пытаемся вызвать конструктор модуля pConstructor
 pDo->pJob->psMod=&(pArgs->ptop->str);
 pDo->pJob->psLab=&(pDo->pJob->pApp->datanameNew);
 cmdExecDo(pDo,pArgs,CvmSubCmdDODo,NULL);
 if (pDo->err<CERR) return;
 //обрабатываем свойства объекта
 if (pArgs->busy>2)
  setDbParMod(pDo,pArgs);
 */
 pDo->err = COutKodOk;
 }
/*
 *    vmMetodQuery(pDo, pArgs, pdataOut);

 */
static int vmMetodQuerySub(TtreeData *ptreeData){
 TvmDo *pDoNew=(TvmDo*)(ptreeData->pUser);
 Tdata *pdRet;
 //
 pDoNew->arArgs.busy=3;
 arrsetptop(pDoNew->arArgs);
 //сформировать Аргументы
 pDataSetInt(pDoNew->arArgs.psec);
 pDoNew->arArgs.psec->intgr=ptreeData->isAlt;
 pDoNew->arArgs.psec[1]=pValKey(ptreeData->pkeydata);
 pDoNew->arArgs.psec[2]=pValData(ptreeData->pkeydata);
 arrExpand(pDoNew->arArgs,ptreeData->Inds);
 stkIndLev1Up(pDoNew);
 //выполнить программу обработки
 mshVmDo(pDoNew,NULL);
 stkIndLev1Pop(pDoNew)
 pdRet=&(ValData(pDoNew->pJob->val));
 castDataInt(pdRet);
 return (pDoNew->err<CERR)?pDoNew->err:pdRet->intgr;
 }
//
static void vmMetodQuery(TvmDo *pDo, TarrData *pArgs) {
 TvmDo *pDoNew;
 TrefProg refProg;
 int isAlt=0;
// void *pUser=NULL;
 //
 if (pArgs->busy<1){pDo->err=CERRvmNotArg;return;}
 if (pDo->pJob->thisObj.busy<1) {pDo->err=CERRvmIndFiled;return;}
 stkindstorefdb(pDo,&(pDo->pJob->thisObj),&(pDo->refDb), EisSetNo );
 if (pDo->err!=COutKodOk) return;
 vmfaindLabelArg(pDo,pArgs, &refProg);
 if (pDo->err!=COutKodOk) return;
 if (pArgs->busy>2){
  pDo->err=castDataInt(pArgs->psec+2);
  if (pDo->err<CERR) return;
  isAlt=pArgs->psec[2].intgr;
  }
 DataSetNULL(pDo->pJob->val);
 //новый блок Do
 pDoNew=vmGetDo(pDo->pJob);
 pDoNew->flag=CvmSubCmdDORet;
 pDoNew->pDoParent=pDo;
 pDoNew->pmod=refProg.pmod;
 pDoNew->curOff=refProg.off;
 pDo->err=roundDataDbAlt(&(pDo->refDb.pvmDb->db),&(pDo->refDb.rInds),isAlt, vmMetodQuerySub , pDoNew);
 }
/**/
static void vmMetodarrPush(TvmDo *pDo,TarrData *pArgs, Tdata *pDataOut) {
 Tdata *pD;
 TarrData Ar;
 //
 if (pArgs->busy<2) {pDo->err =CERRvmNotArg;return;}
 if (pDo->pJob->thisObj.busy!=1) {pDo->err=CERRvmIndFiled;return;}
 stkindstorefdb(pDo,&(pDo->pJob->thisObj),&(pDo->refDb), EisSet);
 if (pDo->err<CERR) return;
 pDo->refDb.off=CvmArrOffNull;
 for (pD=pArgs->psec+1;pD<=pArgs->ptop;pD++)
  vmSetData(pDo,&(pDo->refDb), pD);
 if (pDataOut) {
  if (pArgs->busy==2)
   *pDataOut=pArgs->psec[1];
  else{
   Ar.size=Ar.capa=Ar.busy=pArgs->busy-1;
   Ar.psec=pArgs->psec+1;
   Ar.ptop=pArgs->ptop;
   pArrTopData(&Ar, pDataOut, &(pDo->Temps));
   }
  }
 }
//
static void vmMetodarrPop(TvmDo *pDo, Tdata *pDataOut) {
// TarrData thisInd;
 //
// Stk2toArr(pDo->pJob->stkthis,thisInd);
 if (pDo->pJob->thisObj.busy!=1) {pDo->err=CERRvmIndFiled;return;}
 stkindstorefdb(pDo,&(pDo->pJob->thisObj),&(pDo->refDb), EisSetNo);
 if (pDo->err<CERR) return;
 pDo->refDb.off=CvmArrOffNull;
 vmGetData(pDo,&(pDo->refDb));
 if (pDataOut) *pDataOut=ValData(pDo->pJob->val);
 }
/**/
static void Vector(TvmDo *pDo,int16_t cmdArg,TarrData *pArgs, Tdata *pdataOut) {
 int n;
 //
 if (pArgs->busy < 1) {pDo->err =CERRvmNotArg;return;}
 if ((pDo->err=castDataInt(pArgs->psec+1))<CERR) return;
 n=pArgs->psec[1].intgr;
 pDataSetInt(pdataOut);
 switch (cmdArg) {
  case CvmTypeMetodv64:
   if (n>=CcountInt)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->intgr=pArgs->psec->int64[n];
   break;
  case CvmTypeMetodvu64:
   if (n>=CcountInt)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->uint64[0]=pArgs->psec->uint64[n];
   break;
  case CvmTypeMetodv32:
   if (n>=CcountInt32)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->intgr=pArgs->psec->int32[n];
   break;
  case CvmTypeMetodvu32:
   if (n>=CcountInt32)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->uint64[0]=pArgs->psec->uint32[n];
   break;
  case CvmTypeMetodv16:
   if (n>=CcountInt16)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->intgr=pArgs->psec->int16[n];
   break;
  case CvmTypeMetodvu16:
   if (n>=CcountInt16)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->uint64[0]=pArgs->psec->uint16[n];
   break;
  case CvmTypeMetodv8:
   if (n>=CcountInt8)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->intgr=pArgs->psec->int8[n];
   break;
  case CvmTypeMetodvu8:
   if (n>=CcountInt8)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->uint64[0]=pArgs->psec->uint8[n];
   break;
  }
 }
/*
 * Исходная строка на индексном регистре регистре в 0 аргументе
 * параметры методов в аргументах
 */
void vmInnerMetod(TvmDo *pDo, TCmd *pCmd, TarrData *pArgs,Tdata *pdataOut) {
 Tbstr bsIn;
 int16_t cmdArg;
 //
 pDo->err = COutKodOk;
 cmdArg=pCmdArg(pCmd);
 switch (cmdArg) {
   // PushField
  case CvmTypeMetodPushField:
   vmMetodFieldPush(pDo,pArgs, &bsIn, pdataOut);
   return;
   // PushList
  case CvmTypeMetodPushList:
   vmMetodPushList(pDo,pArgs, pdataOut);
   return;
  case CvmTypeMetodarrPop:
   vmMetodarrPop(pDo, pdataOut);
   return;
  case CvmTypeMetodarrPush:
   vmMetodarrPush(pDo,pArgs, pdataOut);
   return;
   //---iniRead---
  case CvmTypeMetodiniRead:
   metodiniRead(pDo, pArgs);
   return;
   //---iniWrite---
  case CvmTypeMetodiniWrite:
   metodiniWrite(pDo, pArgs);
   return;
  // objNew Конструктор объекта
  case CvmTypeMetodobjNew :
   vmMetodobjNew(pDo, pArgs);
   return;
  // Обход дерева ????
  case CvmTypeMetodQuery :
   vmMetodQuery(pDo, pArgs);
   return;
  case CvmTypeMetodv64 ... CvmTypeMetodvu8:
   if (pArgs->busy)
    Vector(pDo,cmdArg,pArgs,pdataOut);
   return;
  }
 // Исходная строка
 castbStr(pArgs->psec, &bsIn);
 if (bsIn.size<1){pDo->err =CWARdataNotFound;return;}
 //Аргументы
 switch (cmdArg) {
   //---PopStr---
  case CvmTypeMetodPopStr:
   vmMetodPop(pDo,pArgs, &bsIn, pdataOut);
   return;
   // PopField
  case CvmTypeMetodPopField:
   vmMetodFieldPop(pDo,pArgs, &bsIn, pdataOut);
   return;
   // PopList
  case CvmTypeMetodPopList:
   vmMetodPopList(pDo,pArgs, &bsIn, pdataOut);
   return;
   // +++++Строки+++++
   //   "GetStr","CodeStr","PopStr","InsStr","SetStr","DelStr","FindStr","ReplStr"
   //---GetStr---
  case CvmTypeMetodGetStr:
   vmMetodGet(pDo,pArgs, &bsIn, pdataOut);
   break;
   //---CodeStr---
  case CvmTypeMetodCodeStr:
   vmMetodCode(pDo,pArgs, &bsIn, pdataOut);
   break;
   //---InsStr---
  case CvmTypeMetodInsStr:
   vmMetodIns(pDo,pArgs, &bsIn, pdataOut);
   break;
   //---SetStr---
  case CvmTypeMetodSetStr:
   vmMetodSet(pDo,pArgs, &bsIn, pdataOut);
   break;
   //---DelStr---
  case CvmTypeMetodDelStr:
   vmMetodDel(pDo,pArgs, &bsIn, pdataOut);
   break;
   //---FindStr---
  case CvmTypeMetodFindStr:
   vmMetodFind(pDo,pArgs, &bsIn, pdataOut);
   break;
   //---ReplStr---
  case CvmTypeMetodReplStr:
   vmMetodRepl(pDo,pArgs, &bsIn, pdataOut,0);
   break;
   //---ReplStrALL---
  case CvmTypeMetodReplALLStr:
   vmMetodRepl(pDo,pArgs, &bsIn, pdataOut,1);
   break;
   // +++++Поля+++++
   //  "GetField","PushField","PopField","InsField","SetField"."DelField"
   // GetField
  case CvmTypeMetodGetField:
   vmMetodFieldGet(pDo,pArgs, &bsIn, pdataOut);
   break;
   // InsField
  case CvmTypeMetodInsField:
   vmMetodFieldIns(pDo,pArgs, &bsIn, pdataOut);
   break;
   // SetField
  case CvmTypeMetodSetField:
   vmMetodSetField(pDo,pArgs, &bsIn, pdataOut);
   break;
  // DelField
  case CvmTypeMetodDelField:
   vmMetodDelField(pDo,pArgs, &bsIn, pdataOut);
   break;
  // CountField
  case CvmTypeMetodCountField:
   vmMetodCountField(pArgs, &bsIn, pdataOut);
   break;
   // +++++Списки+++++
   //  "GetList","PushList","PopList","InsList","SetList","DelList","CountList"
   // GetList
  case CvmTypeMetodGetList:
   vmMetodGetList(pDo,pArgs, &bsIn , pdataOut);
   return;
   // InsList
  case CvmTypeMetodInsList:
   vmMetodInsList(pDo,pArgs, pdataOut);
   return;
   // SetList
  case CvmTypeMetodSetList:
   vmMetodSetList(pDo,pArgs, pdataOut);
   return;
   // DelList
  case CvmTypeMetodDelList:
   vmMetodDelList(pDo,pArgs, pdataOut);
   return;
   // CountList
  case CvmTypeMetodCountList:
   vmMetodCountList(pDo,pArgs, pdataOut);
   return;
  }
 }
/**/
static void setVector(TvmDo *pDo,int16_t cmdArg,TarrData *pArgs, Tdata *pdataOut) {
 int n;
 Tdata D;
 //
 if (pArgs->busy<3) {pDo->err=CERRvmArgLittle; return;}
 if ((pDo->err=castDataInt(pArgs->psec+1))<CERR) return;
 if ((pDo->err=castDataInt(pArgs->psec+2))<CERR) return;
 if (!(pDataIsInt(pArgs->psec))) {
  if ( !(pDataIsNULL(pArgs->psec)) ){
   pDo->err=CERRtrArgFiled;
   return;
   }
  }
 else
  *pdataOut=pArgs->psec[0];
 D=pArgs->psec[2];
 n=pArgs->psec[1].intgr;
 pDataSetInt(pdataOut);
 switch (cmdArg) {
  case CvmTypeMetodv64:
   if (n>=CcountInt)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->int64[n]=D.intgr;
   break;
  case CvmTypeMetodvu64:
   if (n>=CcountInt)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->uint64[n]=D.uint64[0];
   break;
  case CvmTypeMetodv32:
   if (n>=CcountInt32)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->int32[n]=D.intgr;
   break;
  case CvmTypeMetodvu32:
   if (n>=CcountInt32)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->uint32[n]=D.uint64[0];
   break;
  case CvmTypeMetodv16:
   if (n>=CcountInt16)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->int16[n]=D.intgr;
   break;
  case CvmTypeMetodvu16:
   if (n>=CcountInt16)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->uint16[n]=D.uint64[0];
   break;
  case CvmTypeMetodv8:
   if (n>=CcountInt8)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->int8[n]=D.intgr;
   break;
  case CvmTypeMetodvu8:
   if (n>=CcountInt8)
    pDo->err=CERRvmcoorVekFiled;
   else
    pdataOut->uint8[n]=D.uint64[0];
   break;
  }
 }

/*
 *      Команда
 *    0. код команды
 *    1. Sub команда Что за внутренняя обработка
 *    2. Off3.  Id программы
 */
void vmInnerMetodSet(TvmDo *pDo, TCmd *pCmd,TarrData *pArgs) {
 Tdata dataOut;
 Tbstr bsIn;
 int16_t cmdArg;
 //
 cmdArg=pCmdArg(pCmd);
 memNULL(&dataOut);
 //
 switch (cmdArg) {
   // PushField
  case CvmTypeMetodPushField:
   vmMetodFieldPush(pDo,pArgs, &bsIn, &dataOut);
   goto finish;
   // PushList
  case CvmTypeMetodPushList:
   vmMetodPushList(pDo,pArgs, &dataOut);
   goto finish;
  case CvmTypeMetodarrPop:
   vmMetodarrPop(pDo, NULL);
   return;
  case CvmTypeMetodarrPush:
   vmMetodarrPush(pDo,pArgs, NULL);
   return;
   //---iniRead---
  case CvmTypeMetodiniRead:
   metodiniRead(pDo, pArgs);
   return;
   //---iniRead---
  case CvmTypeMetodiniWrite:
   metodiniWrite(pDo, pArgs);
   return;
  // objNew Конструктор объекта
  case CvmTypeMetodobjNew :
   vmMetodobjNew(pDo, pArgs);
   return;
  // Обход дерева
  case CvmTypeMetodQuery :
   vmMetodQuery(pDo, pArgs);
   return;
  case CvmTypeMetodv64 ... CvmTypeMetodvu8:
   if (pArgs->busy)
    setVector(pDo,cmdArg,pArgs,&dataOut);
   goto finish;
  }
// 
 if (pArgs->busy < 2) {pDo->err =CERRvmNotArg;return;}
 castbStr(pArgs->psec, &bsIn);
 if (bsIn.size<1){pDo->err =CWARdataNotFound;return;}
 pDo->err = COutKodOk;
 // Исходная строка
 castbStr(pArgs->psec, &bsIn);
 switch (pCmdArg(pCmd)) {
   // +++++Строки+++++
   //   "GetStr","CodeStr","PopStr","InsStr","SetStr","DelStr","FindStr","ReplStr"
   //---GetStr---
  case CvmTypeMetodGetStr:
   vmMetodGet(pDo,pArgs, &bsIn, &dataOut);
   break;
   //---CodeStr---
  case CvmTypeMetodCodeStr:
   vmMetodCode(pDo,pArgs, &bsIn, &dataOut);
   break;
   //---PopStr---
  case CvmTypeMetodPopStr:
   vmMetodPop(pDo,pArgs,&bsIn, NULL);
   return;
   //---InsStr---
  case CvmTypeMetodInsStr:
   vmMetodIns(pDo,pArgs, &bsIn, &dataOut);
   break;
   //---SetStr---
  case CvmTypeMetodSetStr:
   vmMetodSet(pDo,pArgs, &bsIn, &dataOut);
   break;
   //---DelStr---
  case CvmTypeMetodDelStr:
   vmMetodDel(pDo,pArgs, &bsIn, &dataOut);
   break;
   //---FindStr---
  case CvmTypeMetodFindStr:
   vmMetodFind(pDo,pArgs, &bsIn, &dataOut);
   break;
   //---ReplStr---
  case CvmTypeMetodReplStr:
   vmMetodRepl(pDo,pArgs, &bsIn, &dataOut ,0);
   break;
   //---ReplStrALL---
  case CvmTypeMetodReplALLStr:
   vmMetodRepl(pDo,pArgs, &bsIn, &dataOut,1);
   break;
   // +++++Поля+++++
   //  "GetField","PushField","PopField","InsField","SetField"."DelField"
   // GetField
  case CvmTypeMetodGetField:
   break;
   // PopField
  case CvmTypeMetodPopField:
   vmMetodFieldPop(pDo,pArgs,&bsIn, NULL);
   return;
   // InsField
  case CvmTypeMetodInsField:
   vmMetodFieldIns(pDo,pArgs, &bsIn, &dataOut);
   break;
   // SetField
  case CvmTypeMetodSetField:
   vmMetodSetField(pDo,pArgs, &bsIn, &dataOut);
   break;
   // DelField
  case CvmTypeMetodDelField:
   vmMetodDelField(pDo,pArgs, &bsIn, &dataOut);
   break;
  // CountField
  case CvmTypeMetodCountField:
   vmMetodCountField(pArgs, &bsIn, &dataOut);
   break;
   // +++++Списки+++++
   //  "GetList","PopList","InsList","SetList","DelList"
   // GetList
  case CvmTypeMetodGetList:
   goto finish;
   // PopList
  case CvmTypeMetodPopList:
   vmMetodPopList(pDo,pArgs, &bsIn, NULL);
   return;
   // InsList
  case CvmTypeMetodInsList:
   vmMetodInsList(pDo,pArgs,  &dataOut);
   goto finish;
   // SetList
  case CvmTypeMetodSetList:
   vmMetodSetList(pDo,pArgs, &dataOut);
   goto finish;
   // DelList
  case CvmTypeMetodDelList:
   vmMetodDelList(pDo,pArgs, &dataOut);
   goto finish;
   // CountList
  case CvmTypeMetodCountList:
   goto finish;
  }
finish: 
 if (pDo->err>CERR) {
  stkindstorefdb(pDo,&(pDo->pJob->thisObj),&(pDo->refDb),EisSet);
  vmSetData(pDo,&(pDo->refDb),&dataOut);
  }
 }
