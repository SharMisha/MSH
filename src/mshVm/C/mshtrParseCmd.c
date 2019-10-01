/*
 * File:   mshvmTrParseCmd.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:54
 */
#include "mshvmTr.h"
#include "mshvmCnst.h"
#include "mshvmMacro.h"
//
void trrfExp(Ttr *pTr,char cEnd,char cSt);
void trArgCmdEventDel(Ttr *pTr,char c0,char c1);
void trArgCmdEventWait(Ttr *pTr,char c0,char c1);
void trArgCmdEventCall(Ttr *pTr,char c0,char c1);
void trArgCmdEventTrap(Ttr *pTr,char c0,char c1);
//
void trArgCmdSet(Ttr *pTr,char c0,char c1);
void trArgCmdKill(Ttr *pTr,char c0,char c1);
void trArgCmdGoDoJob(Ttr *pTr,char c0,char c1);
void trArgCmdRet(Ttr *pTr);
void trArgCmdCopy(Ttr *pTr,char c0,char c1);
void trArgCmdTimeOut(Ttr *pTr,char c0,char c1);
void trArgCmdIf(Ttr *pTr,ToffMod *pcondJmp);
void trArgCmdElse(Ttr *pTr,ToffMod *pcondJmp,ToffMod begCmd);
void trArgCmdCase(Ttr *pTr);
void trArgCmdNext(Ttr *pTr,char vmCmd,char C2,ToffMod *pcondJmp);
void trArgCmdWhile(Ttr *pTr,ToffMod *pcondJmp,ToffMod begCmd);
void trArgCmdEnd(Ttr *pTr,ToffMod *pcondJmp);
void trArgCmdBreak(Ttr *pTr,ToffMod *pcondJmp);
void trArgCmdConst(Ttr *pTr,char c0,char c1);
void trArgCmdParent(Ttr *pTr,char c0,char c1);
void trArgCmdLock(Ttr *pTr,char c0,char c1);
void trArgCmdOpen(Ttr *pTr,char c0,char c1);
void trArgCmdClose(Ttr *pTr,char c0,char c1);
void trArgCmdReadWrite(Ttr *pTr,char c0,char c1);
void trArgCmdExecute(Ttr *pTr,char c0,char c1);
void trSelTerm(Ttr *pTr,char cEnd);
Terr trSmbConst(char **ppcIn,char *pcEnd ,char *ptypeTerm);
void trBufToTr(Ttr *pTr,Tbstr *pbufPrg,Tstr *pName);
Tstr* castId(Tdata *pData);
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
Terr trSpaceSkip(Ttr *pTr);
void skipComment(Ttr *pTr);
/**/

/*
#define skipendStrCur(pTr,pC) ({pC=NULL; while ((*(pTr->bufIn.pcur)==CCvmEndStr)||(*(pTr->bufIn.pcur)==CCvmBK)||(pTr->bufIn.pcur>pTr->bufIn.pcurEnd) ) {\
  if (*(pTr->bufIn.pcur)==CCvmEndStr) pC=pTr->bufIn.pcur; addNumStr(pTr);  trSmbNext(pTr->bufIn.pcur); } })
*/
 /*
 *  Обработка условия выполнения команды
 */
void trCondCmd(Ttr *pTr,ToffMod *pOff){
 ToffMod Off;
 //
 *pOff=0;
 //уйти со знака CprsCmdCond=?
 trSmbNext(pTr->bufIn.pcur);
 *pOff=0;
 //текущая команда
 Off=pTr->pcmds->busy;
 // вычислить выражение
 setErrnumpos(pTr);
 pcmdPush(pTr->pcmds,CvmCmdMarker,pTr->errTr.numpos+1,pTr->errTr.numstr);
 trrfExp(pTr,CtrKodSpace,CtrExpStatExp);
 if (pTr->errTr.err<CERR) return;
 ConstToCmdRegIf(pTr);
 if (Off==pTr->pcmds->busy) {
  pTr->errTr.err=COutExpNot;
  return;
  }
 pcmdPush(pTr->pcmds,CvmCmdJmp,CvmJmp0,0);
 *pOff=pTr->pcmds->busy;
 pTr->errTr.err=COutKodOk;
 }
/*
 * Обработка Аргументов команды
*/
/*
static void oldtrArg(Ttr *pTr,void (*f)(Ttr *pTr,char c0,char c1),char c0,char c1) {
 char *pC;
 //
 do {
  pC=NULL;
  trSmbSpacepTr(pTr);
  //___Обрабатываем Аргумент___
  if ( (f!=trArgCmdConst)&&(f!=trArgCmdParent) ) {
   setErrnumpos(pTr);
   pcmdPush(pTr->pcmds,CvmCmdMarker,pTr->errTr.numpos+1,pTr->errTr.numstr);
   }
  f(pTr,c0,c1);
  if ( (pTr->errTr.err<CERR) || (pTr->bufIn.cTerm == CtrKodEndCom) )  return;
  trSmbSpacepTr(pTr);
  while (
   (*(pTr->bufIn.pcur)==CCvmEndStr)||
   (*(pTr->bufIn.pcur)==CCvmBK)||
   (pTr->bufIn.pcur > pTr->bufIn.pcurEnd) ) {
   if (*(pTr->bufIn.pcur)==CCvmEndStr)
    pC=pTr->bufIn.pcur;
   addNumStr(pTr);
   trSmbNext(pTr->bufIn.pcur);
   }
  //
  if ( pTr->bufIn.pcur >= pTr->bufIn.pcurEnd ) return;
  if ( *(pTr->bufIn.pcur) == CtrKodDlmArg) {
   trSmbNext(pTr->bufIn.pcur);
   if ( pTr->bufIn.pcur >= pTr->bufIn.pcurEnd ) return;
   pTr->bufIn.cTerm = CtrKodDlmArg;
   }
  }while(pTr->bufIn.cTerm == CtrKodDlmArg);
 if (pC) pTr->bufIn.pcur=pC;
 }
*/
static void trArg(Ttr *pTr,void (*f)(Ttr *pTr,char c0,char c1),char c0,char c1) {
 do {
  if (trSpaceSkip(pTr)!=COutKodOk) return;
  pTr->bufIn.pcurOld=pTr->bufIn.pcur;
  //___Обрабатываем Аргумент___
  //выставляем маркер позиции курсора
  if ( (f!=trArgCmdConst)&&(f!=trArgCmdParent) ) {
   setErrnumpos(pTr);
   pcmdPush(pTr->pcmds,CvmCmdMarker,pTr->errTr.numpos+1,pTr->errTr.numstr);
   }
  //обработка Аргумента
  f(pTr,c0,c1);
  //буфер ввода исчерпан (pTr->bufIn.cTerm==CCvmEndStr)
  if ( ( pTr->bufIn.pcur >= pTr->bufIn.pcurEnd ) || (pTr->errTr.err<CERR) )  return;
  //перейти к следующему аргументу
  }while(pTr->bufIn.cTerm == CtrKodDlmArg);
 }
  /*
  if (( pTr->bufIn.pcur >= pTr->bufIn.pcurEnd ) || (pTr->errTr.err<CERR) || (pTr->bufIn.cTerm == CtrKodEndCom) )  return;
  trSmbSpacepTr(pTr);
  while (
   (*(pTr->bufIn.pcur)==CCvmEndStr)||
   (*(pTr->bufIn.pcur)==CCvmBK)||
   (pTr->bufIn.pcur > pTr->bufIn.pcurEnd) )
   *  {
   if (*(pTr->bufIn.pcur)==CCvmEndStr)
    pC=pTr->bufIn.pcur;
   addNumStr(pTr);
   trSmbNext(pTr->bufIn.pcur);
   }
  //
  if ( pTr->bufIn.pcur >= pTr->bufIn.pcurEnd ) return;
  if ( *(pTr->bufIn.pcur) == CtrKodDlmArg) {
   trSmbNext(pTr->bufIn.pcur);
   if ( pTr->bufIn.pcur >= pTr->bufIn.pcurEnd ) return;
   pTr->bufIn.cTerm = CtrKodDlmArg;
   }
//  }while(pTr->bufIn.cTerm == CtrKodDlmArg);
 if (pC) pTr->bufIn.pcur=pC;
 }
*/
/*
 * разбор одной Msh команды
*/
static int trStrmshCmd(Ttr *pTr){
 ToffMod condJmp,begCmd;
 char C0,C1,C2,C3,C4,c,c0,c1,*pbegCmd,*pendCmd,*pc,a0;
 int n;
 //Ищем код команды
 //пропустить символы пробелов
 pTr->bufIn.cTerm=Cvm0;
 while(1) {
  //если команды закончились
  if (trSpaceSkip(pTr)!=COutKodOk) return 1;
  //если признак конеца команды, а тела команды нет
  if (*(pTr->bufIn.pcur)==CtrKodEndCom){
   trSmbNext(pTr->bufIn.pcur);
   }
  else
   break;
  };
 //новая строка, далее возможна метка
 if (pTr->bufIn.cTerm==CCvmEndStr) return 0;
 //комментарии
 if (*(pTr->bufIn.pcur)==CtrKodCmt) {
  skipComment(pTr);
  //за комментариями команд следовать не должно, завершаем обработку команд
  if (pTr->bufIn.cTerm==CCvmEndStr) return pTr->errTr.err;
  }
 //считываем код команды  
 pbegCmd=pTr->bufIn.pcur;
 trSelTerm(pTr,CtrKodDlmCom);
 if (pTr->bufIn.cTerm==CCvmEndStr) return pTr->errTr.err;
 if ((pTr->errTr.err!=COutKodOk)||(pTr->bufIn.cTerm!=CtrTypeTermID)) {
  pTr->errTr.err=CERRtrIDbad;
  return 1;
  }
 //код команды считан
 pendCmd=pTr->bufIn.pcur;
 //условие выполнения команды
 condJmp=0;
 begCmd=pTr->pcmds->busy;
 if (*(pTr->bufIn.pcur)==CtrCmdCond) {
  trCondCmd(pTr,&condJmp);
//  if (pTr->bufIn.cTerm==CCvmEndStr) return pTr->errTr.err;
  if (pTr->errTr.err<CERR) return pTr->errTr.err;
//  if ((trSpaceSkip(pTr)!=COutKodOk)||(pTr->bufIn.cTerm==CCvmEndStr)) return 0;
  pTr->errTr.err=COutKodOk;
  }
 //приводим к стандартному виду код команды
 C0=C1=C2=C3=C4=0;
 n=pendCmd-pbegCmd;
 C0=toupper(*pbegCmd);
 if (n>1){
  C1=toupper(*(pbegCmd + 1));
  C2=toupper(*(pendCmd - 1));
  if (n>2){
   C3=toupper(*(pendCmd - 2));
   if (n>3) C4=toupper(*(pendCmd - 3));
   }
  }
 //разбор аргументов программы
 //Команды
 // c=0;
 c0=c1=CvmOpNULL;
 a0=0;
 //------------------
 switch (C0) {
  //
//  case 'A' :   break;
  // ---Команда Break, Back---
  case 'B' :
   //  Back
   if (C1=='A') {
    trArgCmdNext(pTr,CcmdBack,C2,&condJmp);
    break;
    }
   //  Break
   trArgCmdBreak(pTr,&condJmp);
   break;
  // ---Copy, Close, Constant ?Case---
  case 'C' :
   //Case
   if (C1=='A') {
    if (condJmp==0){
     pTr->errTr.err=CERRtrCaseNotExp;
     break;
     }
    else
     trArgCmdCase(pTr);
    condJmp=0;
    break;
    }
   //Close
   if (C1=='L') {
    trArg(pTr,trArgCmdClose,CvmCmdFile,CvmSubCmdClose);
    break;
    }
   //Constant
   if (C2=='T') {
    condJmp=0;
    trArg(pTr,trArgCmdConst,a0,a0);
    break;
    }
   //Copy
   trArg(pTr,trArgCmdCopy,CvmCmdCopy,a0);
   break;
  // ---Команда DO---
  case 'D' :
   trArg(pTr,trArgCmdGoDoJob,CvmCmdDO,CvmSubCmdDODo);
   break;
  // ---Команды Else, ElseIf, End, Ext---
  case 'E' :
   switch (C1) {
    // Команда Event
    case 'V' :
     //
     c=CvmCmdEvent;
     pc=pTr->bufIn.pcurOld + 2;
     if (toupper(*pc)=='E') ++pc;
     if (toupper(*pc)=='N') ++pc;
     if (toupper(*pc)=='T') ++pc;
     switch (toupper(*pc)) {
      // Команда EventCall
      case 'C' :
       trArg(pTr,trArgCmdEventCall,CvmCmdEvent,CvmSubCmdEventCall);
       break;
      // Команда EventWait
      case 'W' :
       trArg(pTr,trArgCmdEventWait,CvmCmdEvent,CvmSubCmdEventWait);
       break;
      // Команда EventDelete
      case 'D' :
       trArg(pTr,trArgCmdEventDel,CvmCmdEvent,CvmSubCmdEventDelete);
       break;
      // Команда EventTrap
      default :
       trArg(pTr,trArgCmdEventTrap,CvmCmdEvent,CvmSubCmdEventTrap);
       break;
      }
     break;
    // Команда End
    case 'N' :
     //конец подпрограммы ??
     if (ptrstkIterTop){
      trArgCmdEnd(pTr,&condJmp);
      condJmp=0;
      }
     break;
    //Команда ELSE
    default:
     trArgCmdElse(pTr,&condJmp,begCmd);
     condJmp=0;
     break;
    }
   break;
//  case 'F' :   break;
  // ---Команда GOTO---
  case 'G' :
   setErrnumpos(pTr);
   pcmdPush(pTr->pcmds,CvmCmdMarker,pTr->errTr.numpos+1,pTr->errTr.numstr);
   trArgCmdGoDoJob(pTr,CvmCmdGo,a0);
   break;
  //
//  case 'H' :   break;
  // ---Команда If---
  case 'I' :
   trArgCmdIf(pTr,&condJmp);
   break;
  //  ---Команда Job---
  case 'J' :
   trArg(pTr,trArgCmdGoDoJob,CvmCmdDO,CvmSubCmdDOJob);
   break;
  // ---Команда KiLL---
  case 'K' :
   n=pTr->bufIn.pcur - pTr->bufIn.pcurOld-1;
   c=0;
   if (n < 2)
    c=CtypeKiLL;
   else{
    if (n > 1)
     c |= (C2==CtypesKiLLData)?CtypeKiLLData:(C2==CtypesKiLLDown)?CtypeKiLLDown:(C2==CtypesKiLLAlt)?CtypeKiLLAlt:(C2==CtypesKiLL)?CtypeKiLL:0;
    if (n > 2)
     c |= (C3==CtypesKiLLData)?CtypeKiLLData:(C3==CtypesKiLLDown)?CtypeKiLLDown:(C3==CtypesKiLLAlt)?CtypeKiLLAlt:(C3==CtypesKiLL)?CtypeKiLL:0;
    if (n > 3)
     c |= (C4==CtypesKiLLData)?CtypeKiLLData:(C4==CtypesKiLLDown)?CtypeKiLLDown:(C4==CtypesKiLLAlt)?CtypeKiLLAlt:(C4==CtypesKiLL)?CtypeKiLL:0;
    if (c==0)
     c=CtypeKiLL;
    }
   trArg(pTr,trArgCmdKill,CvmCmdKiLL,c);
   break;
  // ---Lock---
  case 'L' :
   n=pTr->bufIn.pcur - pTr->bufIn.pcurOld;
   pc=pTr->bufIn.pcurOld + 1;
   if (toupper(*pc)=='O') {
    ++pc;
    if (toupper(*pc)=='C') {
     ++pc;
     if (toupper(*pc)=='K') {
      ++pc;
      }
     }
    }
   C2=toupper(*pc);
   if (C2=='W') c=CvmSubCmdLockW;
   else
    if (C2=='R') c=CvmSubCmdLockR;
    else
     if (C2=='U') c=CvmSubCmdLockU;
//     else if (C2=='S') {c=CvmSubCmdLockS;cTerm=trArg(prs,trArgCmdLockSt,NULL,&c,perr);break;}
      else {
       pTr->errTr.err=CERRtrIDbad;
       break;
       }
   trArg(pTr,trArgCmdLock,CvmCmdLock,c);
   break;
  // ---Move---
  case 'M' :
   trArg(pTr,trArgCmdCopy,CvmCmdCopy,CvmSubCmdCopyDelTo | CvmSubCmdCopyDelFrom);
   break;
  // ---Команда Next---
  case 'N' :
   trArgCmdNext(pTr,CcmdNext,C2,&condJmp);
   break;
  // ---открыть устройства Open---
  case 'O' :
   trArg(pTr,trArgCmdOpen,CvmCmdFile,CvmSubCmdOpen);
   break;
  case 'P' :
   trArg(pTr,trArgCmdParent,a0,a0);
   condJmp=0;
   break;
  // ---Команда Query---
  case 'Q' :
   trArgCmdNext(pTr,CcmdQuery,C2,&condJmp);
   break;
  // ---Команда Return Read---
  case 'R' :
   // Команда Read
   if (C2=='D'){
    break;
    }
   // Команда Return
   setErrnumpos(pTr);
   pcmdPush(pTr->pcmds,CvmCmdMarker,pTr->errTr.numpos+1,pTr->errTr.numstr);
   trArgCmdRet(pTr);
   break;
  // ---Команда Set---
  case 'S' :
   trArg(pTr,trArgCmdSet,CvmCmdSet,a0);
   break;
  // TimeOut
  case 'T' :
   if ((C1=='I')||(C1=='O'))
    trArg(pTr,trArgCmdTimeOut,CvmCmdTimeOut,a0);
   break;
  //
//  case 'U' :   break;
//  case 'V' :   break;
  case 'W' :
   // ---Команда While---
   if ((n>1)&&(C1=='H'))  {
    trArgCmdWhile(pTr,&condJmp,begCmd);
    condJmp=0;
    break;
    }
   //Write
   trArg(pTr,trArgCmdReadWrite,CvmCmdFile,CvmSubCmdWrite);
   break;
  //Execute
  case 'X' :
   trArg(pTr,trArgCmdExecute,CvmCmdExecute,a0);
   break;
//  case 'Y' :   break;
//  case 'Z' :   break;
  //конец строки
  case CCvmBK :
  case CCvmEndStr :
   break;
  default : {pTr->errTr.err=CERRtrIDbad;return 1;}
  }
 if (pTr->errTr.err<CERR) return pTr->errTr.err;
 if (condJmp) {pCmdArg(parrOffpD(pTr->pcmds,condJmp-1))=pTr->pcmds->busy;}
 return 0;
 }
/*
 * обработка начала строки здесь может быть Метка:
static void bufToCnst(Ttr *pTr){
 TConst *pCnst;
 TvmDb *pvmDb;
 Tval *pVal;
 //если встретилась константа
 pTr->tempKey.bstr.psec=pTr->bufIn.pcurOld;
 pTr->tempKey.bstr.size=pTr->bufIn.pcur - pTr->bufIn.pcurOld;
 if (pTr->tempKey.bstr.size > CsizeStr) pTr->tempKey.bstr.size=CsizeStr;
 arrIter(pTr->arrCnst,pCnst)
  if (StrEqu(pCnst->name,pTr->tempKey.bstr)==CMPeq) {
   if (pCnst->offMod){
    pTr->stat.ptop->_sub.cnstCmd=CregMod;
    pTr->stat.ptop->_sub.cnstArg=pCnst->offMod-1;
    return;
    }
   dataToCmd(pTr,&(pCnst->val),&(pCnst->offMod));
   return;
   }
 //ищем по константам внешних модулей
 arrBusy1(pTr->tempInds);
 pTr->tempInds.psec->bstr=pTr->tempKey.bstr;
 pDataSetbStr(pTr->tempInds.psec);
 RegIter(pTr->secDbConst,pvmDb){
  if ((pVal=mshGet(&(pvmDb->db),&(pTr->tempInds),&(pTr->errTr.err)))){
   dataToCmd(pTr,&(pValData(pVal)),NULL);
   return;
   }
  }
 //не является константой
 if (pTr->tempKey.bstr.size<3){
  BufNtostatCnstCmd(pTr,pTr->tempKey.bstr.psec,pTr->tempKey.bstr.size);
  }
 else{
  arrBusyNull(pTr->arrData);
  trBufToData(pTr->bufIn.cTerm,pTr->bufIn.pcurOld,pTr->bufIn.pcur,&(pTr->areaString),pTr->arrData.ptop);
  pTr->stat.ptop->_sub.cnstCmd=CregMod;
  pTr->stat.ptop->_sub.cnstArg=CnstToArrData(pTr);
  }
 }
 */
static Tstr* bufToCnstLabel(Ttr *pTr,Tbstr *pbsIn){
 TConst *pCnst;
 TvmDb *pvmDb;
 Tval *pVal;
 Tstr *pStr;
 //если встретилась константа
 arrIter(pTr->arrCnst,pCnst)
  if (StrpEqu(pCnst->name,pbsIn)==CMPeq) {
   return castId(&(pCnst->val));
   }
 //ищем по константам внешних модулей
 arrBusy1(pTr->tempInds);
 pTr->tempInds.psec->bstr=*pbsIn;
 pDataSetbStr(pTr->tempInds.psec);
 RegIter(pTr->secDbConst,pvmDb){
  if ((pVal=mshGet(&(pvmDb->db),&(pTr->tempInds),&(pTr->errTr.err)))){
   pStr=castId(&(pValData(pVal)));
   return ((pStr==NULL)||(pStr->size<1))?NULL:pStr;
   }
  }
 return NULL;
 }

static void trLabel(Ttr *pTr){
 char *pCur,*pcurE;
 TerrTr errTr;
 TLabel *pLabel;
 Tbstr bStr;
 Tstr *pStr;
 //
 pTr->errTr.err=COutKodOk;
 pCur=pTr->bufIn.pcur;
 errTr=pTr->errTr;
 trSelTerm(pTr,CCtrKodLabel);
 pcurE=pTr->bufIn.pcur;
 if (pTr->bufIn.cTerm==CCvmEndStr) return;
 if ( (pTr->errTr.err!=COutKodOk)||(pTr->bufIn.cTerm!=CtrTypeTermID) ) goto err;
 if ( ( pTr->errTr.err=trSpaceSkip(pTr) )!=COutKodOk ) goto err;
 if (*(pTr->bufIn.pcur)!=CCtrKodLabel) goto err;
 parrBusy(pTr->pcurSecLabel);
 pLabel=pTr->pcurSecLabel->ptop;
 if ((ptrstkIterTop)&&(ptrstkIterTop->type == CtrTypeItrIf)){
  ToffMod Cond=0;
  trArgCmdEnd(pTr,&Cond);
  if (pTr->errTr.err<CERR) {
   trSmbNext(pTr->bufIn.pcur);
   return;
   }
  }
 //метка CASE
 if ((ptrstkIterTop)&&(ptrstkIterTop->type == CtrTypeItrCase)){
  parrPushDan(ptrstkIterTop,pTr->pcmds->busy);
  pcmdPush(pTr->pcmds,CvmCmdJmp,CvmJmpAny,0);
  }
 bStr.psec=pTr->bufIn.pcurOld;
 bStr.size=pcurE - bStr.psec;
 //имя метки
 if (bStr.size>CsizeLabel) bStr.size=CsizeLabel;
 if ((pStr=bufToCnstLabel(pTr,&bStr))){  pStrToStr(pLabel->label,pStr);  } else{  StrToStr(pLabel->label,bStr);  }
 //StrToStr(pLabel->label,bStr);
 //смещение метки в буфере команд
 pLabel->off=pTr->Cmds.busy;
 //сдвинуть курсор с ':'
 trSmbNext(pTr->bufIn.pcur);
 return;
err: 
 pTr->bufIn.pcur=pCur;
 pTr->errTr=errTr;
 }
/*
 *  разбор Командной строки
 */
static void trStrCmds(Ttr *pTr){
 pTr->errTr.err=COutKodOk;
 pTr->bufIn.cTerm=CtrTypeTerm0;
 //поиск следующей команды
 while (pTr->bufIn.pcur < pTr->bufIn.pcurEnd) {
  //завершить строку если команда не обработана нормально
  if ( (trStrmshCmd(pTr))||(pTr->bufIn.pcur >= pTr->bufIn.pcurEnd)||(pTr->errTr.err<CERR) ) return;
  if (pTr->bufIn.cTerm==CCvmEndStr) return;
  //ищем конец команды
  if (pTr->bufIn.cTerm!=CtrKodEndCom) {
   //входной буфер завершен или конец строки
   if ( (trSpaceSkip(pTr)!=COutKodOk)||(pTr->bufIn.cTerm==CCvmEndStr) ) return;
   //если конца команды нет, то ищем следующие команды
   if (*(pTr->bufIn.pcur)!=CtrKodEndCom) continue;
   trSmbNext(pTr->bufIn.pcur);
   }
  //ищем конец строки
  if ( (trSpaceSkip(pTr)!=COutKodOk)||(pTr->bufIn.cTerm==CCvmEndStr) ) return;
  }
 }
/*
 * Разбор строк
 */
static void trCodCmdBuf(Ttr *pTr){
 //
 while (pTr->bufIn.pcur < pTr->bufIn.pcurEnd) {
  //разбор не пустой строки
  trLabel(pTr);
  if (pTr->bufIn.cTerm!=CCvmEndStr){
   if (pTr->errTr.err<CERR) break;
   trStrCmds(pTr);
   if (pTr->errTr.err<CERR) break;
   }
  }
 }
void trCodamshCmdBuf(Ttr *pTr,Tbstr *pbufPrg,Tstr *pName){
 trBufToTr(pTr,pbufPrg,pName);
 trCodCmdBuf(pTr);
 }
/**/
void trCodamshCmd(Ttr *pTr){
 Tbstr bufPrg;
 int off=strlen(pCmshMagMsh),numstr;
 //
 numstr=1;
 if (*((char*)(pTr->bufIn.map.psec + off))=='\n') {++off;++numstr;}
 bufPrg.psec=pTr->bufIn.map.psec+off;
 bufPrg.size=pTr->bufIn.map.size-off;
 trBufToTr(pTr,&bufPrg,&(pTr->bufIn.name));
 pTr->errTr.numstr=numstr;
 trCodCmdBuf(pTr);
 }
