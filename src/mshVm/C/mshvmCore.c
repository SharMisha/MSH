/*
 * File:   mshVmCore.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 14:52
 */
#include "mshvmVm.h"
#include "mshvmTr.h"
#include "mshvmMacro.h"

//extern struct sttTarrextFunc *parrextFunc;

void bStrGet(Tdata *pData,Tbstr* pbstrOut);
Tstr* castId(Tdata *pData);
//TApp App;
//TApp *pApp=&App;
TJob* vmJobInit();
void vmJobClear(TJob *pJob);
TvmDo* vmDoInit(TJob *pJob);
void vmDoClear(TvmDo *pDo);
Terr castNorm(Tdata *pData);
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);


void TrInit(Ttr *pTr);
void TrFree(Ttr *pTr);
void TrClear(Ttr *pTr);
void mshVmDo(TvmDo *pDo, TdivStream *pdiv);
void mshDbClear(TDb *pDb);
void pArrKillLongData(TarrData *pArr);
TvmMod* getMod(Tstr *pname,Terr *perr);

void trBufToTr(Ttr *pTr,Tbstr *pbufPrg,Tstr *pName);
Terr trrfExp(Ttr *pTr,char cEnd,char cSt);

void ClearsubEvent(TApp *pApp,Tstr *pevntName);
Terr vmCmdiSource(TvmDo *pDo,TCmd *pCmd,TrefDb *pDbArr,int iCmd);
//const char* CdirGlb;
void pListdataLongRef(TmapLong *pmLong,TarrData *pArr);
void pValLongRef(TmapLong *pmLong,Tval *pVal);
int roundDataDbAlt(TDb *pDb, TarrData *pInds,int isAlt, int (*pfdata)(TtreeData *ptreeData) , void *pUser);
Terr getLabelApp(TrefProg *prefProg);
/*
 */
//запись в дерево строки разделенной '=' на ключ и данные
void pbStrToDb(TvmDo *pDo,Tbstr *pbStr){
 Tdata Data;
 char *pB,*pC,*pE;
 //
 if (pbStr->size){
  DataSetbStr(Data);
  pB=pbStr->psec;
  pE=pB+pbStr->size;
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
     if ((pDo->err=castNorm(&Data))>CERR) {
      pDo->err=mshSet(&(pDo->refDb.pvmDb->db),&(pDo->refDb.rInds),&Data);
      }
     }
    }
   }
  }
 }

/*
 * Найти модуль и метку
 */
void vmfaindLabel(TvmDo *pDo,Tstr *psMod, Tstr *psLabel, TrefProg *prefProg) {
 //
 prefProg->pmod=pDo->pmod;
 if ( (psMod)&&(psMod->size>0) )
  prefProg->pmod=getMod(psMod,&(pDo->err));
 prefProg->off=0;
 if ((prefProg->pmod)&&(psLabel)){
  prefProg->label=*psLabel;
  pDo->err=getLabelApp(prefProg);
  }
 }
 //
void vmfaindLabelArg(TvmDo *pDo,TarrData *pArgs, TrefProg *prefProg) {
 Tstr *psLabel,*psMod=NULL;
 //
 if (pArgs->busy<1){pDo->err=CERRvmNotArg;return;}
 if ( (pArgs->busy>1)&&(pDataIsNoNULL(pArgs->psec+1)) )
  psMod=castId(pArgs->psec+1);
 psLabel=castId(pArgs->psec);
 vmfaindLabel(pDo,psMod, psLabel, prefProg);
 }
/*
 *   Управление разделяемыми ресурсами
 *  получить новый элемент управления разделяемым ресурсом
 */
TLock* LockResNew(){
 spinLock(App.locksRes.lock);
 pRegBusy(&(App.locksRes));
 pLockInit(App.locksRes.ptop);
 spinUnLock(App.locksRes.lock);
 return App.locksRes.ptop;
 }


void vmWaitJobExit(TJob* pJob){
 spinLock(App.lockApp);
 //остановить этот поток;
 if  (pJob->idPth!=CvmpThreadNULL) { mshJobcancel(pJob->idPth);}
 spinUnLock(App.lockApp);
 }
/*
 * Создать новый блок задания Job
 */
TJob* vmGetJob(){
 TJob *pJob;
 //
 spinLock(App.arrJob.lock);
 objGetSecs(App.arrJob,pJob);
 if (pJob) vmJobClear(pJob); else pJob=vmJobInit();
 spinUnLock(App.arrJob.lock);
 return pJob;
 }
/*
 */
TvmDo* vmGetDo(TJob *pJob){
 TvmDo *pDo;
 //
 spinLock(App.arrDo.lock);
 objGetSecs(App.arrDo,pDo);
 if (pDo) {
  pDo->pJob=pJob;
  vmDoClear(pDo);
  }
 else
  pDo=vmDoInit(pJob);
 objSetBusy(pDo);
 spinUnLock(App.arrDo.lock);
 return pDo;
 }
TvmDo* vmGetDoflag(TvmDo *pDoParent,u_char flag){
 TvmDo *pDo;
 TJob *pJob;
 //
 if (cmdDoIsJob(flag)){
  pJob=vmGetJob();
  //имя модуля и метку перенести в новый Job
  if (pDoParent->pJob->psLab==NULL)
   pJob->psLab=NULL;
  else {
   pJob->sLab=*pDoParent->pJob->psLab;
   pJob->psLab=&pJob->sLab;
   }
  if (pDoParent->pJob->psMod==NULL)
   pJob->psMod=NULL;
  else {
   pJob->sMod=*pDoParent->pJob->psMod;
   pJob->psMod=&pJob->sMod;
   }
  }
 else {
  pJob=pDoParent->pJob;
  objAdd(pJob);
  pJob->divSrv=pDoParent->pJob->divSrv;
  pJob->fsrvWrite=pDoParent->pJob->fsrvWrite;
  }
 pDo=vmGetDo(pJob);
 pDo->flag=flag;
 pDo->pDoParent=pDoParent;
 pDo->pmod=pDoParent->pmod;
 return pDo;
 }
//
TvmDo* vmGetJobDo(){
 TJob *pJob;
 //
 return ((pJob=vmGetJob()))?vmGetDo(pJob):NULL;
 }
/*
 * Создать новый блок выполнения Do
 */
/*
 Tstr name;   // имя модуля
 TPacks modpack; //буферный модуль
 Tsize *psec; // ссылка на секцию модуля
 Tstk2Label labels; //метки
 Tarea arealong; //область размещения длинных строк
 Tstr  *pext;   // родители объекта
 Tdata *pdata;   // начало констант
 TCmd *pbegcmds; // начало команд
 TCmd *pendcmds; // конец команд
 Tsize sumext;
 Tsize sumdata;
 Tsize sumlabel;
 Tsize sumcmd;
 */

void TrToMod(Ttr *pTr,TvmMod *pMod){
 pMod->name=pTr->bufIn.name;
 pMod->pext=pTr->vmExt.psec;
 pMod->sumext=pTr->vmExt.busy;
 pMod->pdata=pTr->arrData.psec;
 pMod->sumdata=pTr->arrData.busy;
 //
 pMod->pbegcmds=pTr->Cmds.psec;
 pMod->sumcmd=pTr->Cmds.busy;
 //
 pMod->labels=pTr->arr2Label;
 pMod->areaString=pTr->areaString;
 }
/*
 * в RunTime транслировать ссылку из строки
 */
void TrRef(TvmDo *pDo,Ttr *pTr,Tdata *pX){
 Tbstr bufM;
 //
 bStrGet(pX,&bufM);
 pTr=&(pDo->pJob->exeTr);
 TrClear(pTr);
 trBufToTr(pTr,&bufM,NULL);
 //разбор не пустой строки
 pTr->errTr.numpos=0;
 pTr->bufIn.pbegStr=pTr->bufIn.pcur;
 pTr->bufIn.pcurOld=pTr->bufIn.pcur;
 trrfExp(pTr,CCtrKodEqu,CtrExpStatRef);
 if ((pTr->errTr.err>CERR)&&(pTr->pcmds->busy<1)) pTr->errTr.err=CERRvmIndFiled;
 }
/*
 * Выполнить блок Tr
 */
void mshTrRunpDo(Ttr *pTr,TApp *pApp,TvmDo **ppDo,u_char flag, TdivStream *pdiv){
 TvmDo *pDo;
 TvmMod Mod;
 //
 if ((pTr)->Cmds.busy==0) {
  if (ppDo) *ppDo=NULL;
  pTr->errTr.err=CWARtrCmdEmpty;
  return;
  }
 pDo=vmGetJobDo();
 if (pDo==NULL) {pTr->errTr.err=CERRdoFiled;return;}
 pvmModInit(&Mod);
 pTr->bufIn.name=pApp->sunname;
 TrToMod(pTr,&Mod);
 pDo->pmod=&Mod;
// cmdDoSetJob(pDo->flag);
 pDo->flag=flag;
 mshVmDo(pDo,NULL);
 pTr->errTr=pDo->pJob->errTr;
 vmModFree(Mod);
 if (ppDo) *ppDo=pDo;
 //освободить блок Do,освободить задание.
 objpDopJobFree(pDo);
 }
void mshTrRun(Ttr *pTr,TApp *pApp,u_char flag, TdivStream *pdiv){
 TvmDo *pDo=NULL;
 mshTrRunpDo(pTr,pApp,&pDo,flag,pdiv);
 }
/*
void msharrextFuncFree(TextFun *pextFun){
 if (parrextFunc) parrFree(parrextFunc);
 }
*/
/*
 * Инициализация массива Аргументов
 */
/*
void mshArgarrInit(TvmDo *pDo,Tbstr *pnameMod,Tbstr *psLabel){
 Tdata *pData;
 //новый уровень индекса
 stkIndLev1Pop(pDo);
 //имя модуля
 stkIndBusypTop(pDo,pData);
 if (pnameMod){
  pStrTopStrID(&(pData->bstr),pnameMod);
  pDataSetStr(pData);
  }
 else {
  pDataSetNULL(pData);
  }
 //занести в массив метку
 stkIndBusypTop(pDo,pData);
 pStrTopStrID(&(pData->bstr),psLabel);
 pDataSetStr(pData);
 }
*/
/*
typedef struct {
 TDb *pDb;
 TarrData Inds;
 Tval *pkeydata;
 int Level;
 int isAlt;
 void *pUser;
 } TtreeData;
  
typedef struct {
 int (*pfdata)(TtreeData *ptreeData);
 void *pUser;
 u_char isGlb; //имя метки
 } TrounDb;

 */
int vmRoundDbSub(TtreeData *ptreeData){
 TrounDb *proundDb;
 //
 proundDb=(TrounDb*)ptreeData->pUser;
 //откорректировать ссылки
 if (proundDb->isglb){
  pListdataLongRef(&(ptreeData->pDb->mlong),&(ptreeData->Inds));
  pValLongRef( &(ptreeData->pDb->mlong),(ptreeData->pkeydata) );
  }
 return proundDb->pfdata(ptreeData);
 }

int vmRoundDb(TvmDb *pvmDb, TarrData *pInds,int isAlt, int (*pfdata)(TtreeData *ptreeData) , void *pUser) {
 TrounDb roundDb;
 //
 roundDb.pfdata=pfdata ;
 roundDb.pUser=pUser;
 roundDb.isglb=pvmDb->isglb;
 return roundDataDbAlt(&(pvmDb->db),pInds,isAlt, vmRoundDbSub , &roundDb);
 }