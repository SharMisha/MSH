/*
 * File:   mshVmDo.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:43
 */
#include "mshvmVm.h"
#include "mshvmMacro.h"

void tstvmprnDo(TvmDo *pDo,TCmd *pcurCmd,TdivStream *pdiv);
void tstvmprnDoDb(TvmDo *pDo,TdivStream *pdiv);
void tstvmprnRun(TvmDo *pDo,TdivStream *pdiv);
void tstvmprnJob(TvmDo *pDo,TdivStream *pdiv);
void tstvmprnApp(TvmDo *pDo,TdivStream *pdiv);
void tstvmprnDbApp(TvmDo *pDo,TdivStream *pdiv);
//
void vmGet(TvmDo *pDo,TCmd *pCmd);
void vmSet(TvmDo *pDo,TCmd *pCmd,int isInd);
void vmKill(TvmDo *pDo,TCmd *pCmd);
Terr vmCmdiSource(TvmDo *pDo,TCmd *pCmd,TrefDb *pDbArr,int iCmd);
void vmQueryInit(TvmDo *pDo,TCmd *pCmd);
void vmQuery1(TvmDo *pDo,TCmd *pCmd);
void vmQuery(TvmDo *pDo,TCmd *pCmd);
void vmCmdReg(TvmDo *pDo,TCmd *pCmd);
void vmCopy(TvmDo *pDo,TCmd *pCmdIn);
Terr vmEvent(TvmDo *pDo,TCmd *pCmd);
void Execute(TvmDo *pDo,TCmd *pCmd, TdivStream *pdiv);
void cmdRet(TvmDo *pDo,TCmd *pCmd);
void cmdCase(TvmDo *pDo,TCmd *pCmd);
void cmdFile(TvmDo *pDo,TCmd *pCmd);
Terr OpRunUno(Tdata *pAr,char op,char op1);
Terr OpRun(TvmDo *pDo,Tdata *pAr1,Tdata *pAr2,char op1,char op2);
Terr Lock(TvmDo *pDo,TCmd *pCmd);
Terr TimeOut(TvmDo *pDo,TCmd *pCmd);
Terr cmdJmp(TvmDo *pDo,TCmd *pCmd);
void cmdGo(TvmDo *pDo);
void cmdDo(TvmDo *pDo,TCmd *pCmd,TdivStream *pdiv);
int cmbFindInStr(const char *pStrCnst,char C);
Terr LongDelInd(Tlong *pLong,Tsize ind);

void vmWaitJobExit(TJob* pJob);
/*
 * Выполнение блока команд
 */
void mshVmDo(TvmDo *pDo, TdivStream *pdiv) {
 Tdata *pAr1, *pAr2;
 TCmd *pCmd, *pcurCmdOld;
 Terr ret;
 u_char Sub;
 Tusize busy;
 Tsize curOffbeg; //смещение текущей команды
 //
 pCmd = NULL;
 pDo->stat = CvmStatDoRun;
 pDo->pJob->errTr.nameMod=pDo->pmod->name;
 curOffbeg=pDo->curOff;
///* 
#ifdef	mshDEBAG
 pcurCmdOld = NULL;
 tstvmprnDo(pDo,pCmd,NULL);
#endif
// */
 //
do{
// IndDoToArr(pDo,pDo->rarInd)
 pcurCmdOld = pCmd;
 pCmd = pDo->pmod->pbegcmds + pDo->curOff;
 pDo->err = COutKodOk;
/*
 //Если это код Операции
*/
 if ( smbIsOp(pCmdCmd(pCmd)) ) {
  busy=Stk3size(stkInd(pDo));
  if (busy < 1) {pDo->err = CERRvmNotArg; goto com; }
  if ((pCmdSub(pCmd)) == CvmOpUno) {
   pAr1 = stkIndTopData(pDo);
   if (pAr1==NULL) {pDo->err = CERRvmNotArg; goto com; }
   pDo->err = OpRunUno( pAr1, pCmdCmd(pCmd), pCmdSub(pCmd));
   }
  else {
   if (busy < 2) { pDo->err = CERRvmNotArg; goto com;}
   stkIndPopTop(pDo,pAr2);
   pAr1 = stkIndTopData(pDo);
   if ((pAr1==NULL)||(pAr2==NULL)) {pDo->err = CERRvmNotArg; goto com; }
   pDo->err = OpRun(pDo, pAr1, pAr2, pCmdCmd(pCmd), pCmdSub(pCmd));
   }
  goto com;
  }
  //Обработка остальных команд
  switch (pCmdCmd(pCmd)) {
   case CvmCmdMarker :
    areaClear(pDo->Temps);
//очищать стек не стоит, можно потерять возвращаемое значение    
//    stkIndLevClear(pDo->pJob->inds);
    pDo->pJob->errTr.numstr=pCmdArg(pCmd);
    pDo->pJob->errTr.numpos=pCmdSub(pCmd);
    break;
   case CvmCmdRefToArr :
//    Stk2Lev0Up(pDo->pJob->stkthis);
//    if (pDo->this.busy>0) {arrExpand(pDo->pJob->stkthis._core,pDo->this);}
    parrCopy(&(pDo->pJob->thisObj),&(pDo->this));
    break;
    /*
     *    Команды обращения к дереву
     */
    //---- чтение данных---
   case CvmCmdGet:
    vmGet(pDo,pCmd);
    break;
    //-----Запись данных-----
   case CvmCmdSet:
    vmSet(pDo,pCmd,1);
    break;
   case CvmCmdSetSh:
    vmSet(pDo,pCmd,0);
    break;
    //----- Удаление данных-----
   case CvmCmdKiLL:
    vmKill(pDo,pCmd);
    break;
    //-----Итераторы по Данным------
   case CcmdBack  :
   case CcmdNext  :
   case CcmdQuery :
    Sub=pCmdCmdSubNext(pCmd);
    pDo->eof=0;
    ValALLSetNULL(pDo->pJob->valquery);
    //инициализация Итератора и нахождение 1-й вершины
    if (Sub & CvmCmdSubInit){
     arrBoxPush(pDo->curquery);
     //данных не обнаружено
     vmQueryInit(pDo,pCmd);
     pDo->err=pDo->curquery.ptop->iterNext.AdrOfferr.err;
     if ( (pDo->curquery.ptop->iterNext.AdrOfferr.err== CMPnull)||(pDo->curquery.ptop->iterNext.AdrOfferr.err<CERR) ) {
      //сбросить уровень стека Итератора
      arrBoxPop(pDo->curquery);
      //обойти тело цикла если нет узлов
      pDo->curOff = pCmdArg(pCmd);
      pDo->eof=1;
      goto next;
      }
     pDo->pJob->val=pDo->pJob->valquery;
     break;
     }
    if (Sub==CvmCmdSubNext1){
     arrBoxPush(pDo->curquery);
     vmQuery1(pDo,pCmd);
     pDo->err=pDo->curquery.ptop->iterNext.AdrOfferr.err;
     //сбросить уровень стека Итератора
     if ( (pDo->err== CMPnull)||(pDo->err<CERR) ) {
      pDo->eof=1;
      }
     else{
      pDo->pJob->val=pDo->pJob->valquery;
      pDo->err=pDo->curquery.ptop->iterNext.AdrOfferr.err;
      }
     arrBoxPop(pDo->curquery);
     break;
     }
    vmQuery(pDo,pCmd);
    //если цикл завершен сбросить уровень стека Итератора
    if ( (pDo->curquery.ptop->iterNext.AdrOfferr.err == CMPnull)||(pDo->curquery.ptop->iterNext.AdrOfferr.err <CERR) ) {
     arrBoxPop(pDo->curquery);
     pDo->eof=1;
     break;
     }
    pDo->pJob->val=pDo->pJob->valquery;
    //если цикл не завершен то перейти в начало цикла
    pDo->curOff = pCmdArg(pCmd);
    goto next;
//    continue;
   //обмен данными между регистрами
   case CvmCmdReg :
    vmCmdReg(pDo,pCmd);
    break;
   //---копирование узла---перемещение узла---
   case CvmCmdCopy:
    vmCopy(pDo,pCmd);
    break;
    //------------- команды Lock ----------
   case CvmCmdLock :
    Lock(pDo,pCmd);
    break;
    //------------- команды Execute ----------
   case CvmCmdExecute :
    Execute(pDo,pCmd,pdiv);
    break;
    //--------команда TimeOut--------
   case CvmCmdTimeOut:
    TimeOut(pDo,pCmd);
    break;
    //-------------Обработка событий--------------
   case CvmCmdEvent:
    vmEvent(pDo, pCmd);
    break;
    //-------перейти на Адрес в модуле при условии-----
   case CvmCmdJmp:
    ret=cmdJmp(pDo,pCmd);
    if (ret){
     pDo->curOff = pCmdArg(pCmd);
     goto next;
     }
    break;
    //-------перейти на метку в программе-----
   case CvmCmdGo:
    //если нет критических ошибок то продолжить с выбранной команды
    cmdGo(pDo);
    if (pDo->err>CERR) goto next;
    break;
    //--------выполнить Подпрограмму--------
   case CvmCmdDO:
    cmdDo(pDo,pCmd,pdiv);
    pDo->pJob->errTr.nameMod=pDo->pmod->name;
    break;
    //-------вернуться из подпрограммы-------
   case CvmCmdReturn:
    //вернуть значение в Вызвавшую программу
    cmdRet(pDo,pCmd);
    goto next;
   //-----Команда CASE-----
   case CvmCmdCase:
    cmdCase(pDo,pCmd);
    if (pDo->err==COutKodOk) goto next;
    break;
    // ------Команды управления устройствами-----
   case CvmCmdFile :
    cmdFile(pDo,pCmd);
    break;
   } 
com:
 //перейти к следующей команде
 ++(pDo->curOff); 
next: ;
// if (pDo->err<CERR) {  pDo->pJob->errTr.err=pDo->err;  }
 //Отладка
/*    
#ifdef	mshDEBAG
 ret=pCmd - pDo->pmod->pbegcmds;
 if (pCmdCmd(pCmd)!=CvmCmdMarker) {
  tstvmprnDo(pDo,pCmd,NULL);
  if ((pCmdCmd(pCmd)==CvmCmdGet)||(pCmdCmd(pCmd)==CvmCmdSet)||(pCmdCmd(pCmd)==CvmCmdSetSh)||(pCmdCmd(pCmd)==CvmCmdKiLL)
          ||(pCmdCmd(pCmd)==CvmCmdDO)||(pCmdCmd(pCmd)==CvmCmdCopy)||(pCmdCmd(pCmd)==CcmdNext)||(pCmdCmd(pCmd)==CcmdQuery) )
//  if ((pCmdCmd(pCmd)==CvmCmdSet)||(pCmdCmd(pCmd)==CvmCmdSetSh)||(pCmdCmd(pCmd)==CvmCmdKiLL)||(pCmdCmd(pCmd)==CvmCmdCopy))
  {
//  tstvmprnDoDb(pDo,pdiv);
//  tstvmprnDbApp(pDo,pdiv);
//  sleep(1);
  }
 }
#endif
  */
 // ??Обработать ошибки выполнения??
 } while ((pDo->curOff < pDo->pmod->sumcmd)
    &&(pDo->err > CERR)&&(pDo->stat != CvmStatDoEnd)&&(pDo->curOff>=0) );
finish:
 //
 /*
 //завершить все задания запущенные из этого блока
 int *pidJob;
 if (pDo->arrJob.busy)
  arrIter(pDo->arrJob,pidJob) vmWaitJobExit(IdgetpJob(pDo->pJob->pApp,*pidJob));
*/
 //Отладка
// /*
#ifdef	mshDEBAG
// if ((pDo->pmod->name.size>4)&& ( ((pDo->pmod->name.psec[0]=='b') ) || (pDo->pmod->name.psec[0]=='n')) ) {
     
  printf("  ***** --------------------------------------------------------------------------------- ******\n");
  tstvmprnDo(pDo,pCmd,pdiv);
  tstvmprnJob(pDo,pdiv);
  tstvmprnApp(pDo,pdiv);
//  sleep(1);
  tstvmprnDoDb(pDo,pdiv);
//  }
#endif
 //*/
 //  if (pdiv!=NULL) tstvmprnRun(pDo,pdiv);
 //статус завершения
 pDo->stat = (pDo->stat & CvmStatDoNo) | CvmStatDoEnd;
 //Освободить ресурсы 
 // возвращаемое значение вернуть на стек предыдущего уровня
 if (cmdDoIsRet(pDo->flag)) {
  pAr1=stkIndTopData(pDo);
  if (pAr1){
   ValData(pDo->pJob->val)=*pAr1;
   }
  else {
   ValDataSetNULL(pDo->pJob->val);
   }
  }
 pDo->curOff=curOffbeg;
 }
