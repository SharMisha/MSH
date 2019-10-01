/* 
 * File:   mshvmInnerWin.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 14 февраля 2019 г., 19:28
 */
/*
 * внутренне Функции Win
 *      Команда
 *    0. код команды = CvmCmdDO
 *    1. Sub команда = CvmSubCmdDOInWin
 *    2.  источник
 *    Off3.  Id программы
 */
#include <gtk/gtk.h>

#include "mshvmVm.h"
#include "mshvmMacro.h"
#include "mwinCore.h"
//
Terr winInit();
Terr winFree();
Terr winNewObj(TarrData *pArgs);
Terr winSetAtrObj(TarrData *pArgs,Tdata *pOut,void* pUser);
Terr winGetAtrObj(TarrData *pArgs,Tdata *pOut,Tarea *pTemps);
Terr winIniToAtr(TarrData *pArgs,Tdata *pOut,void* pUser);
Terr winAtrToIni(TarrData *pArgs,Tdata *pOut,Tarea *pArea);
Tdiv winChannelOpen(TarrData *pArgs,int *perr);
Terr winStart(TarrData *pArgs);
int bufSend(Tdiv div,Tbstr *pbsBf);
int srvOut(TdivSrv div,Tbstr *pbsBf);
//
TvmMod* getMod(Tstr *pname,Terr *perr);
Terr getLabelApp(TrefProg *prefProg);
TvmDo* vmGetJobDo();
void mshVmDo(TvmDo *pDo, TdivStream *pdiv);
void mshKillArrALL(TDb *pDb);
//
Terr vmInnerWin(TvmDo *pDo,TCmd *pCmd,TarrData *pArgs,Tdata *pOut){
 //
 int err;
 if (pOut){
  pDataSetInt(pOut);
  pOut->intgr=0;
  }
 switch(pCmdArg(pCmd)){
  //---winInit---
  case CvmTypeFunctWinInit :
   return winInit();
  //---winFree---
  case CvmTypeFunctWinFree :
   return winFree();
  //---winNew---
  case CvmTypeFunctWinNew :
   return winNewObj(pArgs);
  //---winSetAtr---
  case CvmTypeFunctWinSetAtr :
   return winSetAtrObj(pArgs,pOut,pDo);
  //---winGetAtr---
  case CvmTypeFunctWinGetAtr :
   return winGetAtrObj(pArgs,pOut,&(pDo->Temps));
  //---winSetAtr---
  case CvmTypeFunctWinIniToAtr :
   return winIniToAtr(pArgs,pOut,pDo);
  //---winGetAtr---
  case CvmTypeFunctWinAtrToIni :
   return winAtrToIni(pArgs,pOut,&(pDo->Temps));
  case CvmTypeFunctWinOpenChannel :
   pDo->pJob->divSrv.TCP=winChannelOpen(pArgs,&err);
   pDo->pJob->fsrvWrite= srvOut;
   mshKillArrALL(&(pDo->pJob->vmDbBuf.db) );
   return err;
  case CvmTypeFunctWinStart :
   return winStart(pArgs);
  default : 
   return CERRnameInWinNo;
  }
 return CERRnameInWinNo;
 }
/*
 * Поиск Модуля и Метки 
 */
Terr vmwingetModLabel(Tstr *psMod,Tstr *psLabel,void *pUser,Tdata *pdataModLabel){
 TrefProg refProg;
 Terr err;
 TvmDo *pDo=(TvmDo*)pUser;
 //
 refProg.pmod=( (psMod==NULL)||(psMod->size<1) )?pDo->pmod:getMod(psMod,&err);
 if (refProg.pmod==NULL) return CERRvmModNotFound;
 if ((psLabel==NULL)||(psLabel->size<1) ){
  pdataModLabel->voidOff.off=0;
  }
 else{
  refProg.label=*psLabel;
  err=getLabelApp(&refProg);
  if (err<CERR) return err;
  pdataModLabel->voidOff.off=refProg.off;
  }
 pdataModLabel->voidOff.pvoid=refProg.pmod;
 return COutKodOk; 
 }
/*
 */
Tdata* getParObj(GtkWidget *pObj,EqType qpar);

void butCallBackOn(GtkWidget *pObj,EqType qpar,void* pUser){
 TvmDo *pDo;
 Tdata *pdPar;
 //новый блок Do в текущем контексте   
 pDo=(TvmDo*)pUser; //vmGetJobDo();
 if (pDo==NULL) return;
 pdPar=getParObj(pObj,qpar);
 if (pdPar==NULL) return;
 pDo->pmod=(TvmMod*)(pdPar->voidOff.pvoid);
 pDo->curOff=pdPar->voidOff.off;
 if (pDo->pmod==NULL) return;
 //в Аргументах передаем имя компонента
 arrBusy1(pDo->arArgs);
 pDataSetbStr(pDo->arArgs.psec);
 //
 pDo->arArgs.psec->bstr.psec=gtk_widget_get_name(pObj);
 pDo->arArgs.psec->bstr.size=strlen(pDo->arArgs.psec->bstr.psec);
// stkIndLev1Up(pDo);
 mshVmDo(pDo,NULL);
 //освободить блок Do,освободить задание.
 objpDopJobFree(pDo);
// stkIndLev1Pop( pDo )
 }
void butCallBack(GtkWidget *pObj,void* pUser){
 butCallBackOn(pObj,qOnModLab,pUser);
 }
/*
 */
/*
void winSetChannel(Tdiv tcp,TfsrvWrite fsrvWrite,void* pUser){
 App.TCP=tcp;
 App.fsrvWrite= fsrvWrite;
 mshKillArrALL(&(((TvmDo*)(pUser))->pJob->vmDbBuf.db) );
 }
*/
/**/
//Tstr* winnameMod(void* pUser){ return &( ((TvmDo*)(pUser))->pmod->name ); }
