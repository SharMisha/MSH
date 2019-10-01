/* 
 * File:   mshvmTrExp.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 4 января 2017 г., 16:25
 */
#include "mshvmVm.h"
#include "mshvmTr.h"
#include "mshvmCnst.h"
#include "mshvmMacro.h"
//#include "mshextLib.h"

//struct sttTarrextFunc *parrextFunc=NULL;

Terr DataCmp(Tdata *pkey1, Tdata *pkey2);
Terr trSelTerm(Ttr *pTr,char cEnd);
void trBufToData(char cTerm,char *pcIn,char *pcEnd,Tarea *pArea,Tdata *pData);
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
void bStrGet(Tdata *pData,Tbstr* pbstrOut);
int equCnst(char *pcIn,char *pcEnd,char *parrStr[]);

/**/
#define BufNtostatCnstCmd(pTr,pBuf,n) pTr->ptopStat->_sub.cnstCmd=CregComChar; pTr->ptopStat->_sub.cnstArg=0; memCopy(&(pTr->ptopStat->_sub.cnstArg),pBuf,n)
#define pTrErr(pTr,Err) pTr->errTr.err=Err

/*
 * загрузить на регистр Данные
 */
Tsize CnstToArrData(Ttr *pTr){
 Tdata *pData,*pTop;
 //
 if (pTr->arrData.busy <2) {return 0;}
 pTop=arrptop(pTr->arrData);
 for (pData=pTr->arrData.psec;pData < pTop;++pData)
  if (DataCmp(pTop,pData)==CMPeq) {
   arrPop(pTr->arrData);
   return arrpValToOff(pTr->arrData,pData);
   }
 return pTr->arrData.busy-1;
 }
/*
void BufToStatCnst(Ttr *pTr,Tbstr *pbStr){
 //является ли константой
 if (pbStr->size<3){
  BufNtostatCnstCmd(pTr,pbStr->psec,pbStr->size);
  }
 else{
  arrBusy(pTr->arrData);
  trBufToData(pTr->bufIn.cTerm,pbStr->psec,pbStr->psec + pbStr->size,&(pTr->areaLong),pTr->arrData.ptop);
  pTr->stat.ptop->_sub.cnstCmd=CregMod;
  pTr->stat.ptop->_sub.cnstArg=CnstToArrData(pTr);
  }
 }
*/
static void dataToCmd(Ttr *pTr,Tdata *pData,ToffMod *poffMod){
 if (pDataIsInt(pData)) {
  if ((pData->intgr> iInt16min)&&(pData->intgr<iInt16max)){
   pTr->stat.ptop->_sub.cnstCmd=CregCom;
   pTr->stat.ptop->_sub.cnstArg=pData->intgr;
   return;
   }
  }
 //не число
 if (pDataIsNoNum(pData)){
  bStrGet(pData,&(pTr->tempKey.bstr));
  if (pTr->tempKey.bstr.size<3){
   BufNtostatCnstCmd(pTr,pTr->tempKey.bstr.psec,pTr->tempKey.bstr.size);
   return;
   }
  }
 arrPushpDan(pTr->arrData,pData);
 pTr->stat.ptop->_sub.cnstCmd=CregMod;
 pTr->stat.ptop->_sub.cnstArg=CnstToArrData(pTr);
 if (poffMod) *poffMod=pTr->stat.ptop->_sub.cnstArg+1;
 }
/**/
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
/*
 * Выполнить операции из стека
 */
static int endOp(Ttr *pTr) {
 //стек операций перенести в команды VM
 if ((pTr->ptopStat==NULL)||(parrIsNull(pTr->ptopStat))) return CretNone;
 //константу перенести на стек
 ConstToCmdRegIf(pTr);
 do{
  pcmdPush4(pTr->pcmds,pTr->ptopStat->ptop->c2);
  trOpPop(pTr->ptopStat);
  }while(parrIsNoNull(pTr->ptopStat));
 return CretEndOp;
 }
#define endOpCnsIf(pTr) ConstToCmdRegIf(pTr);endOp(pTr)
/*
 * Конец массива
 */
static int endArr(Ttr *pTr){
 if (((pTr->ptopStat==NULL)||(statIsNoArr(pTr)))) return CretNone;
 //Источник индекса массива
 ConstToCmdRegIf(pTr);
 pcmdPush(pTr->pcmds,CvmCmdGet,0,0);
 statPop(pTr);
 return CretEndArr;
 }
// завершение операции выбора
static int endCond(Ttr *pTr){
 TCmd *pCmd;
// TarTtrOp *psubTop,*psubMax;
 //
 if (((pTr->ptopStat==NULL)||(statIsNoFalse(pTr)))) return CretNone;
// ConstToCmdRegIf(pTr);
 endOpCnsIf(pTr);
 pCmd=parrOffpD(pTr->pcmds,pTr->ptopStat->_sub.offCmd);
 pCmdArg(pCmd)=pTr->pcmds->busy;
 /*
 for (psubTop=pTr->ptopStat+1,psubMax=pTr->stat.psec + pTr->busymax;
  (psubTop < psubMax) && psubTop &&((psubTop->_sub.st==CtrCmdCondFalse)||(psubTop->_sub.st==CtrCmdCond));
   psubTop++){
  pCmd=parrOffpD(pTr->pcmds,psubTop->_sub.offCmd);
  pCmdArg(pCmd)=pTr->pcmds->busy;
  }
 */
 statPop(pTr);
 return CretEndCond;
 }
// завершение функции
static int endFun(Ttr *pTr){
 if (pTr->ptopStat==NULL) return CretNone;
 ConstToCmdRegIf(pTr);
 if (pTr->ptopStat->_sub.lateCmd.c4){
  if (pTr->ptopStat->_sub.isThis) {
   cmdDoSetThis(pTr->ptopStat->_sub.lateCmd);
   }
  pcmdPush4(pTr->pcmds,pTr->ptopStat->_sub.lateCmd.c4);
  CmdInit(pTr->ptopStat->_sub.lateCmd);
  }
 else{
  if (pTr->ptopStat->_sub.isThis) {
   pcmdPush(pTr->pcmds,CvmCmdDO,CvmSubCmdDORet | CvmSubCmdDOthis,0);
   }
  else{
   pcmdPush(pTr->pcmds,CvmCmdDO,CvmSubCmdDORet,0);
   }
  }
 statPop(pTr);
 return CretEndFun;
 }

#define endFunMod(pTr) (statIsMod(pTr))?endFun(pTr):0
/*
 * Конец поля
 */
static Terr endField(Ttr *pTr){
 //завершить не завершенные операции
 while((pTr->stat.busy>1) && (statIsNoIndList(pTr) && statIsNoArg(pTr) )
         && (endArr(pTr)+endCond(pTr) + ( endFunMod(pTr) ) ) );
 if (statIsNoIndList(pTr) && statIsNoArg(pTr))
  pTrErr(pTr,CERRtrIndFiled);
 else{
  endOpCnsIf(pTr);
  }
 return CretEndInd;
 }
/*
 * Добавить спец символы в выходную последовательность
 */
static void begstatBf(Ttr *pTr){
 if ((pTr->ptopStat->_sub.pconstBf)&&(pTr->ptopStat->_sub.cnstCmd==0)) {
  BufNtostatCnstCmd(pTr,pTr->ptopStat->_sub.pconstBf,1);
//  BufNtostatCnstCmd(pTr,pTr->ptopStat->_sub.pconstBf,pTr->ptopStat->_sub.lconstBf);
  pTr->ptopStat->_sub.pconstBf=NULL;
//  pTr->ptopStat->_sub.lconstBf=0;
  pTr->bufIn.OldTerm = CtrTypeTermID;
  }
 }
/*
 * Открывающаяся скобка
 */
static void begBkt(Ttr *pTr){
 if ((pTr->bufIn.OldTerm==CtrKodArr)||(pTr->bufIn.OldTerm==CtrKodMod)) {
  //выражение
  statPush(pTr,CtrKodBktBeg);
  return;
  }
 //завершаем массивы
 while((pTr->stat.busy>1) && endArr(pTr) );
 //находимся в функции
 if (statIsMod(pTr)) {
  //константу сбрасываем на регистр
  ConstToCmdRegIf(pTr);
  statSet(pTr,CtrKodArg);
  return;
  }
 //проверяем может это вызов функции
 if (pTr->ptopStat->_sub.cnstCmd) {
  //имени нет подымаем пустой индекс Модуля
  pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp | CstyleIndMod | CregNULL ,0);
  //константу записываем на место метки
  cmdPushCnst(pTr,CvmCmdReg,CvmSubCmdRegMove);
  //устанавливаем состояние Аргументы функции
  statPush(pTr,CtrKodArg);
  return;
  }
 //метка функции
 if ((pTr->pcmds->ptop)&& (pTr->bufIn.OldTerm != CtrKodArr) && (statIsNoArg(pTr))
        && (  endArr(pTr) || (pTr->bufIn.OldTerm == CtrKodBktEnd) || (pCmdCmd(pTr->pcmds->ptop)==CvmCmdGet) )  ) {
  //переносим метку функции с нижележащего индекса
  pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp| CstyleIndMod | CregInd,0);
  //устанавливаем состояние Аргументы функции
  statPush(pTr,CtrKodArg);
  return;
  }
 //выражение
 statPush(pTr,CtrKodBktBeg);
 }
/*
 * Закрывающаяся скобка
 */
static void endBkt(Ttr *pTr){
 if (pTr->stat.busy<1) {pTrErr(pTr,CERRtrStatNULL);return;}
 // опускаемся до нашего вложения
 while( (pTr->stat.busy>1) && (statIsNoBkt(pTr)) && (statIsNoArg(pTr)) && (endArr(pTr)+endCond(pTr) + ( endFunMod(pTr) ) ) ) ;
 endOpCnsIf(pTr);
 //это был список Аргументов
 if (statIsArg(pTr)) {
  endFun(pTr);
  }
 //скобка Выражения
 else{
  if (statIsBkt(pTr)) {
   statPop(pTr);
   }
  else
   pTrErr(pTr,CERRtrStatNULL);
  }
 }
/*
 * начало индекса
 */
static void begInd(Ttr *pTr,u_char ckodInd,u_char csyleInd, u_char isNo, u_char regNULL){
 //перед индексом признак Массива
 if (pTr->bufIn.OldTerm==CtrKodArr){
  if (!isNo){
   pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp| csyleInd | regNULL , 0 );
   }
  statPush(pTr,ckodInd);
  return;
  }
 //
 while((pTr->stat.busy>1) && (endArr(pTr) ) );
 if (!isNo){
  if (pTr->ptopStat->_sub.cnstCmd){
   if (pTr->bufIn.OldTerm != CtrTypeTermID) {pTr->errTr.err=CERRtrNameDbNotId;return;}
   cmdPushCnst(pTr,CvmCmdReg,CvmSubCmdRegUp | csyleInd);
   }
  else{
   if ((pTr->pcmds->ptop) && (pTr->bufIn.OldTerm != CtrTypeTermOp) && (pTr->bufIn.OldTerm != CtrKodDlmList)
           && ( (pCmdCmd(pTr->pcmds->ptop)==CvmCmdGet) || (pTr->bufIn.OldTerm == CtrKodBktEnd) || (pTr->bufIn.OldTerm == CtrKodListEnd))  ) {
    pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp| csyleInd | CregInd,0);
    }
   else{
    pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp| csyleInd | regNULL,0);
    }
   }
  }
 statPush(pTr,ckodInd);
 }
/*
 */
static void endInd(Ttr *pTr){
 while((pTr->stat.busy>1) &&(statIsNoIndList(pTr)) && (endArr(pTr)+endCond(pTr) + ( endFunMod(pTr) ) ) );
 if (statIsNoIndList(pTr))
  pTrErr(pTr,CERRtrIndFiled);
 else{
  endOpCnsIf(pTr);
  statPop(pTr);
  }
 }
/*
 * Начало Индекса
 * CtrKodMod,CstyleIndMod
 */
static void begMod(Ttr *pTr){
 while((pTr->stat.busy>1) && (endArr(pTr) +endCond(pTr) ) );
 //точка встретилась повторно параметры
 if (statIsMod(pTr)) {
  ConstToCmdRegIf(pTr);
  return;
  }
 //точка встретилась впервые
 if (pTr->bufIn.OldTerm == CtrKodIndEnd) {
  pcmdPush(pTr->pcmds,CvmCmdRefToArr,0,0);
  }
 if (pTr->ptopStat->_sub.cnstCmd){
  if (pTr->bufIn.OldTerm != CtrTypeTermID) {pTr->errTr.err=CERRtrNameModNotId;return;}
  cmdPushCnst(pTr,CvmCmdReg,CvmSubCmdRegUp | CstyleIndMod);
  }
 else{
  if ((pTr->pcmds->ptop) && ( (pTr->bufIn.OldTerm == CtrKodBktEnd) || (pCmdCmd(pTr->pcmds->ptop)==CvmCmdGet) || (pCmdCmd(pTr->pcmds->ptop)==CvmCmdRefToArr) )  ) {
   pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp| CstyleIndMod | CregInd,0);
   }
  else{
   pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp | CstyleIndMod | CregNULL,0);
   }
  }
 statPush(pTr,CtrKodMod);
 if (pTr->bufIn.OldTerm == CtrKodIndEnd)
  pTr->stat.ptop->_sub.isThis=1;
 }
/*
 * разбор выражения
 */
void trrfExp(Ttr *pTr,char cEnd,u_char cSt) {
 int iArg;
 Tdata Data;
 char *pb,*pe;
 Toff Off;
 TCmd *pCmd;
 //
 arr2subClear(pTr->stat);
 statPush(pTr,cSt);
 //Маркер начала выражения
 setErrnumpos(pTr);
 pTr->ptopStat->_sub.isNoUno=0;
 pTr->bufIn.cTerm=CtrTypeTerm0;
// pcmdPush(pTr->pcmds,CvmCmdMarker,CvmCmdMarkerExp,pTr->bufIn.numpos);
 //подготовить регистр индекса для расчетов
 do {
  if (pTr->ptopStat==NULL) {pTr->errTr.err=CERRvmStkMshCmdFiled;return;}
  pTr->errTr.err=COutKodOk;
  pTr->bufIn.OldTerm=pTr->bufIn.cTerm;
  if (pTr->busymax < pTr->stat.busy) pTr->busymax = pTr->stat.busy;
  //выбрать следующий терм
  trSelTerm(pTr,cEnd);
  //разбор завершен
  if (pTr->errTr.err<CERR) goto finish;
//  { if (pTr->errTr.err<CERR) goto finish;  pTr->errTr.err=COutKodOk;   break;   }
  //окончание разбора
  if (pTr->bufIn.cTerm==CtrKodEndCom) break;
  if (pTr->bufIn.cTerm==cEnd){
   if (pTr->stat.busy==1) break;
   while((pTr->stat.busy>1) && (endArr(pTr)+endCond(pTr) + (endFunMod(pTr)) ) );
   if (pTr->stat.busy==1) break;
   }
//  if ((pTr->bufIn.cTerm==CtrKodEndCom)||(pTr->bufIn.cTerm==CCvmEndStr)||(pTr->bufIn.cTerm==cEnd)) break;
  /*
  if (pTr->bufIn.cTerm==CCvmEndStr) {
   while ( ((pb=--(pTr->bufIn.pcur ) ) > pTr->bufIn.pcurOld)&& (*pb!=CCvmEndStr)) ;
   break;
   }
  if (pTr->bufIn.cTerm==cEnd) {
   //до этого идет вызов функции
   while((pTr->stat.busy>1) && ( endArr(pTr)+endCond(pTr)) + (endFunMod(pTr) ) );
   if (pTr->stat.busy<2) {
    trSmbSpacepTr(pTr);
    break;
    }
   }
  */
 //??? if (( *(pTr->bufIn.pcurOld)==CvmOpEqu )&&(pTr->bufIn.cTerm!=CtrTypeTermOp)){ pTr->bufIn.cTerm=CtrTypeTermOp; }
  //Обработка терма
  switch (pTr->bufIn.cTerm) {
   //----спецсимволы-----
   case CtrTypeDbTemp :
   case CtrTypeDbGlb :
    if ( (pTr->bufIn.OldTerm!=CtrTypeDbTemp)&&(pTr->bufIn.OldTerm!=CtrTypeDbGlb) )
     pTr->ptopStat->_sub.pconstBf=pTr->bufIn.pcurOld;
    break;
   //-------Идентификатор------- 
   case CtrTypeTermID:
    //возможно это внутренние методы, функции и свойства
    if (pTr->bufIn.OldTerm==CtrKodTypeInner) {
     //внутренние свойства
     if ((iArg=equCnst(pTr->bufIn.pcurOld,pTr->bufIn.pcur,pCvmInnerProp))>=0){
      //обработка
      pcmdPush(pTr->pcmds,CvmCmdDO,CvmSubCmdDOInProp | CvmSubCmdDORet,iArg);
      break;
      }
     //   Свойства переменных
     if ( (iArg=equCnst(pTr->bufIn.pcurOld,pTr->bufIn.pcur,pCvmInnerStat))>=0 ){
      //внутреннее свойство не привязано к переменной
      if (pTr->ptopStat->_sub.st!=CtrKodMod) {pTr->errTr.err=CERRtrInnerNoVal;return;}
      pcmdPush(pTr->pcmds,CvmCmdDO,CvmSubCmdDOInStat | CvmSubCmdDORet,iArg);
      statPop(pTr);
      break;
      }
     //   строковые методы
     if ( (iArg=equCnst(pTr->bufIn.pcurOld,pTr->bufIn.pcur,pCvmInnerMetod))>=0 ){
      if (pTr->ptopStat->_sub.st!=CtrKodMod) {pTr->errTr.err=CERRtrInnerNoVal;return;}
      if (pTr->pcmds->busy){
//       if (pCmdCmd(pTr->pcmds->ptop)==CvmCmdReg ) { pCmdSub(pTr->pcmds->ptop) &=CregunMaskInd; }
       if ((pTr->pcmds->busy>1)&&(pCmdCmd(pTr->pcmds->ptop-1)!=CvmCmdRefToArr )) {
        pcmdPush(pTr->pcmds,CvmCmdRefToArr,0,0);
        }
       }
      //----------------
      //нужна только ссылка значение не нужно
//      if (iArg>=CvmTypeMetodPopStr) { pcmdPush(pTr->pcmds,CvmCmdRefToArr,0,0); }
      cmdSet(pTr->ptopStat->_sub.lateCmd,CvmCmdDO,CvmSubCmdDOInMetod | CvmSubCmdDORet,iArg);
      break;
      }
     //встроенные Функции
     if ( (iArg=equCnst(pTr->bufIn.pcurOld,pTr->bufIn.pcur,pCvmInnerFunct))>=0 ) {
      //поднять индексный регистр
      pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp | CstyleIndMod,0);
      statPush(pTr,CtrKodMod);
      cmdSet(pTr->ptopStat->_sub.lateCmd,CvmCmdDO,CvmSubCmdDOInFun | CvmSubCmdDORet,iArg);
      break;
      }
     //встроенные Функции библиотеки Win
     if ( (iArg=equCnst(pTr->bufIn.pcurOld,pTr->bufIn.pcur,pCvmWin))>=0 ) {
      //поднять индексный регистр
      pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp | CstyleIndMod,0);
      statPush(pTr,CtrKodMod);
      cmdSet(pTr->ptopStat->_sub.lateCmd,CvmCmdDO,CvmSubCmdDOInWin | CvmSubCmdDORet,iArg);
      break;
      }
     }
    //Идентификатор
    //включить в идентификатор спецсимволы
    if ((pTr->bufIn.OldTerm==CtrTypeDbTemp)||(pTr->bufIn.OldTerm==CtrTypeDbGlb)) {
     pTr->bufIn.pcurOld=pTr->ptopStat->_sub.pconstBf;
     pTr->ptopStat->_sub.pconstBf=NULL;
     }
   case CtrTypeTermCnst :
    bufToCnst(pTr);
    break;
   // --- Целое число ---
   case CtrTypeTermInt :
    trBufToData(pTr->bufIn.cTerm,pTr->bufIn.pcurOld,pTr->bufIn.pcur,&(pTr->areaString),&Data);
    if ((pTr->ptopStat->ptop)&&(trOpOff(pTr->ptopStat,0)==CvmOpMinus) && (trOpOff(pTr->ptopStat,1)==CvmOpUno)) {
     Data.intgr=-Data.intgr;
     arr2subPopSec(pTr->stat)
     }
    if ((Data.intgr> iInt16min)&&(Data.intgr<iInt16max)){
     pTr->ptopStat->_sub.cnstCmd=CregCom;
     pTr->ptopStat->_sub.cnstArg=Data.intgr;
     }
    else{
     arrPushDan(pTr->arrData,Data);
     pTr->ptopStat->_sub.cnstCmd=CregMod;
     pTr->ptopStat->_sub.cnstArg=CnstToArrData(pTr);
     }
    endArr(pTr);
    break;
   // --- Действительное число ---
   case CtrTypeTermFloat :
    //загрузить константу в модуль
    arrBusyNull(pTr->arrData);
    trBufToData(pTr->bufIn.cTerm,pTr->bufIn.pcurOld,pTr->bufIn.pcur,&(pTr->areaString),pTr->arrData.ptop);
    if (((pTr->bufIn.cTerm)==CtrTypeTermFloat)&&(pTr->ptopStat->ptop)
        &&(trOpOff(pTr->ptopStat,0)==CvmOpMinus)&&(trOpOff(pTr->ptopStat,1)==CvmOpUno)) {
     pTr->arrData.ptop->fnum= -pTr->arrData.ptop->fnum;
     arr2subPopSec(pTr->stat)
     }
    pTr->ptopStat->_sub.cnstCmd=CregMod;
    pTr->ptopStat->_sub.cnstArg=CnstToArrData(pTr);
    break;
   // -----Кавычки---------
   case CCtrKodKav1 :
    pb=pTr->bufIn.pcurOld+1;
    pe=pTr->bufIn.pcur-1;
    if ((pe-pb) < 3){
     pTr->bufIn.pcurOld++;
     pTr->bufIn.pcur--;
     bufToCnst(pTr);
     pTr->bufIn.pcurOld--;
     pTr->bufIn.pcur++;
     }
    //загрузить константу в модуль
    else{
     arrBusyNull(pTr->arrData);
     trBufToData(pTr->bufIn.cTerm,pb,pe,&(pTr->areaString),pTr->arrData.ptop);
     pTr->stat.ptop->_sub.cnstCmd=CregMod;
     pTr->stat.ptop->_sub.cnstArg=CnstToArrData(pTr);
     }
    break;
   // --- это метка или параметры --- .
   case CtrKodMod :
    begMod(pTr);
    break;
   // --- начало Массива ---$
   case CtrKodArr :
    begstatBf(pTr);
    begInd(pTr,CtrKodArr,CstyleIndArr,0,CregNULL);
    break;
   // --- начало индекса ---[
   case CtrKodIndBeg :
    begstatBf(pTr);
    begInd(pTr,CtrKodIndBeg,CstyleIndInd,0,CregNULL);
    break;
   // --- конец индекса ---]
   case CtrKodIndEnd :
    endInd(pTr);
    pcmdPush(pTr->pcmds,CvmCmdGet,0,0);
    break;
   // --- начало списка --- {
   case CtrKodListBeg :
    begInd(pTr,CtrKodListBeg,CstyleIndInd,statIsInd(pTr),CregNone);
    break;
   // --- конец списка --- }
   case CtrKodListEnd :
    endInd(pTr);
    pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegMove | CregInd,0);
    break;
   // --- разделитель в списке --- 
   case CtrKodDlmList :
    if ((statIsNoIndList(pTr) && statIsNoArg(pTr))&&(pTr->stat.busy==1) ) goto finish;
    if (pTr->bufIn.OldTerm==CtrKodDlmList){
     pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegMove | CregComChar , 0);
     }
    else
     endField(pTr);
    break;
   // --- разделитель в списке --- 
   case CtrKodDlmAlt :
    endField(pTr);
    pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegAlt,0);
    break;
   // --- скобки открывается (---
   case CtrKodBktBeg :
    begBkt(pTr);
    break;
   // --- скобки закрывается )---
   case CtrKodBktEnd :
    endBkt(pTr);
    break;
   // --- код операции ---
   case CvmOpUsr :
   case CtrTypeTermOp :
//    while((pTr->stat.busy>1) && (endArr(pTr) + endCond(pTr) + ( endFunMod(pTr) ) ) );
    while((pTr->stat.busy>1) && (endArr(pTr) + ( endFunMod(pTr) ) ) );
    endOpCnsIf(pTr);
    trOpNew(pTr->ptopStat);
    Off=pTr->bufIn.pcur - pTr->bufIn.pcurOld;
    if (Off>CsizeCodeOpDef) Off=CsizeCodeOpDef;
    memCopy(pTr->ptopStat->ptop->c1,pTr->bufIn.pcurOld,Off);
    if (pTr->ptopStat->_sub.isNoUno==0) trOpOff(pTr->ptopStat,1)=CvmOpUno;
    break;
   // ---условная операция---True
   case CtrCmdCond :
//    while((pTr->stat.busy>1) && (endArr(pTr)+endCond(pTr) + ( endFunMod(pTr) ) ) );
    while((pTr->stat.busy>1) && (endArr(pTr)+( endFunMod(pTr) ) ) );
//    ConstToCmdRegIf(pTr);
    endOpCnsIf(pTr);
    statPush(pTr,CtrCmdCond);
    pTr->ptopStat->_sub.offCmd=pTr->pcmds->busy;
    pcmdPush(pTr->pcmds,CvmCmdJmp,CvmJmp0,0);
    break;
   // ---условная операция---False
   case CtrCmdCondFalse :
//    while((pTr->stat.busy>1) && (statIsNoTrue(pTr) ) && (endArr(pTr)+endCond(pTr) + ( endFunMod(pTr) ) ) );
    while( pTr->stat.busy > 1 ) {
     if ( (endArr(pTr)+( endFunMod(pTr) ) )==0){
      if ( statIsNoFalse(pTr) ) break;
      else endCond(pTr);
      }
     }
    if ( statIsNoTrue(pTr) ){
     pTrErr(pTr,CERRtrCondFiled);
     break;
     }
    //
//    ConstToCmdRegIf(pTr);
    endOpCnsIf(pTr);
    pCmd=parrOffpD(pTr->pcmds,pTr->ptopStat->_sub.offCmd);
    pTr->ptopStat->_sub.offCmd=pTr->pcmds->busy;
    pcmdPush(pTr->pcmds,CvmCmdJmp,CvmJmpAny,0);
    pCmdArg(pCmd)=pTr->pcmds->busy;
    pTr->ptopStat->_sub.st=CtrCmdCondFalse;
    break;
   case CCvmEndStr :
    goto finish;
   default :
    if (trisSpaceBas(pTr->bufIn.cTerm)) break;
    pTr->errTr.err=CERRtrCharFiled;
    return;
   }
  // признак отрицательного числа
  pTr->ptopStat->_sub.isNoUno=((pTr->bufIn.cTerm==CtrKodArr)||(pTr->bufIn.cTerm==CtrKodIndBeg)||(pTr->bufIn.cTerm==CtrKodListBeg)
          ||(pTr->bufIn.cTerm==CtrKodDlmList)||(pTr->bufIn.cTerm==CtrKodDlmAlt)||(pTr->bufIn.cTerm==CtrKodBktBeg)
          ||(pTr->bufIn.cTerm==CtrCmdCond)||(pTr->bufIn.cTerm==CtrCmdCondFalse)
          ) ? 0 : 1 ;
  //снять признак спецсимволы
  if ((pTr->ptopStat->_sub.pconstBf)&&(pTr->bufIn.cTerm!=CtrTypeDbTemp)&&(pTr->bufIn.cTerm!=CtrTypeDbGlb)) pTr->ptopStat->_sub.pconstBf=NULL;
  }
 while( (pTr->bufIn.pcur < pTr->bufIn.pcurEnd)&&(pTr->errTr.err>CERR) ) ;
 //
finish: 
 while((pTr->stat.busy>1) && (endArr(pTr)+endCond(pTr) + (endFunMod(pTr)) ) );
 if (pTr->errTr.err>CERR) {
  endOp(pTr);
  if (pTr->stat.busy>1) pTr->errTr.err=CERRtrParseExpNotFinished;
  }
// trSpaceSkip(pTr,NULL)
          ;
 }
/**/
