/*
 * File:   mshvmTrParseArg.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:53
 */
#include "mshvmVm.h"
#include "mshvmMacro.h"
//
void trrfExp(Ttr *pTr,char cEnd,u_char cSt);
void trCondCmd(Ttr *pTr,ToffMod *pOff);
void trBufToData(char cTerm,char *pcIn,char *pcEnd,Tarea *pArea,Tdata *pData);
void trBufToID(char *pcIn,char *pcEnd,Tstr *pStr);
void trSelTerm(Ttr *pTr,char cEnd);
Tsize CnstToArrData(Ttr *pTr);
TvmDb* vmDbGetSet(TvmDo *pDo,TsecsvmDb *psecDb,Tstr *psName,const char *pDir,enum TisSet dbisSet,enum TisLock dbisLock,int sizePart,enum TtypeDb typeDb,enum TisGlb isGlb,Terr *perr);
Terr mshgetDb(TDb *pDb,Tstr *pName,const char *pDir,enum TisSet dbisSet,int sizePartGlb,enum TtypeDb typeDb);
Terr trSpaceSkip(Ttr *pTr);
/*
 */
static void trendCnstExp(Ttr *pTr,char c0,char c1){
 //
 if (pTr->ptopStat->_sub.cnstCmd) {
  cmdPushCnst(pTr,c0,c1 );
  }
 else{
  pcmdPush(pTr->pcmds,c0,c1,0);
  }
 }
/*
 */
static void trendProgExp(Ttr *pTr,char c0,char c1,TCmd *pCmd){
 if (pTr->pcmds->ptop==NULL) {pTr->errTr.err=CERRnameModNotFound;return;}
 if (pTr->ptopStat==NULL) {pTr->errTr.err=CERtrRefFiled;return;}
 if (pTr->ptopStat->_sub.cnstCmd) {
  pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp| CstyleIndMod | CregNULL,0);
  cmdPushCnst(pTr,CvmCmdReg,CvmSubCmdRegMove );
  pcmdPush(pTr->pcmds,c0,c1,0);
  return;
  }
 if (pCmdCmd(pTr->pcmds->ptop)==CvmCmdDO) {
  pCmdCmd(pTr->pcmds->ptop)=c0;
  pCmdSub(pTr->pcmds->ptop) |= c1;
  subcmdDoSetRet0(pCmdSub(pTr->pcmds->ptop));
//  pCmdSub(pTr->pcmds->ptop) = ( cmdDoIsIn(pCmdSub(pTr->pcmds->ptop)) ) | c1;
  }
 else{
  if (pTr->pcmds->ptop > pCmd){
   //не было подъема регистра его надо поднять
   pCmd=(pCmd)? pCmd+1 :pTr->pcmds->psec;
   parrInspData(pTr->pcmds,pCmd);
   pcmdSet(pCmd,CvmCmdReg,CvmSubCmdRegUp| CstyleIndMod | CregNULL,0);
   }
  pcmdPush(pTr->pcmds,c0,c1,0);
  }
 }
/*
 */
static void trExpRef(Ttr *pTr,char cEnd,char cSt){
 trrfExp(pTr,cEnd,cSt);
 if (pTr->errTr.err>CERR) {
  if ((pTr->pcmds->ptop)&&(pCmdCmd(pTr->pcmds->ptop)==CvmCmdGet)) {
   pcmdPop(pTr->pcmds);
   }
  else
   pTr->errTr.err=CERtrRefFiled;
  }
 }
/*
 * Команда Set
*/
void trArgCmdSet(Ttr *pTr,char c0,char c1){
 TCmd Cmd,*pCmd;
// Tusize busy;
 // вычислить выражение слева от равенства результат попадает на индексный регистр
 pCmd=pTr->pcmds->ptop;
 trrfExp(pTr,CtrAssign,CtrExpStatRef);
 if (pTr->errTr.err<CERR) return;
 if (pTr->pcmds->ptop==NULL) {pTr->errTr.err=CERtrRefFiled;return;}
 if ((pTr->ptopStat)&&(pTr->ptopStat->_sub.cnstCmd)) {
  //имени нет подымаем пустой индекс Модуля
  pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp| CstyleIndMod | CregNULL ,0);
  //константу записываем на место метки
  cmdPushCnst(pTr,CvmCmdReg,CvmSubCmdRegMove);
  ConstToCmdRegIf(pTr);
  //это может быть только вызов функции
  pcmdPush(pTr->pcmds,CvmCmdDO,CvmSubCmdDOSet,0);
  /*
  trrfExp(pTr,CtrKodDlmList,CtrKodArg);
  if (pTr->bufIn.pcur >= pTr->bufIn.pcurEnd) return;
  if (pTr->errTr.err>CERR ) {
   ConstToCmdRegIf(pTr);
   pcmdPush(pTr->pcmds,CvmCmdDO,CvmSubCmdDOSet,0);
   }
  */
  }
 if (pCmdCmd(pTr->pcmds->ptop)==CvmCmdGet) {
  pcmdPop(pTr->pcmds);
  }
 else{
  if (pCmdCmd(pTr->pcmds->ptop)==CvmCmdDO){
   if (pTr->bufIn.cTerm!=CtrAssign) {
    pCmdSub(pTr->pcmds->ptop) = (cmdDoIsIn(pCmdSub(pTr->pcmds->ptop))) | CvmSubCmdDOSet;
    }
   else{
    //буфер окончен
    Cmd=*(pTr->pcmds->ptop);
    pcmdPop(pTr->pcmds);
    trrfExp(pTr,CtrKodDlmList,CtrKodArg);
    if (pTr->errTr.err>CERR ) {
     ConstToCmdRegIf(pTr);
     CmdSub(Cmd) |= CvmSubCmdDOSet;
     cmdDoSetRet0(Cmd);
     pcmdPush4(pTr->pcmds,Cmd.c4);
     }
    } 
   return;
   }
  //Не ссылка
  if  (pTr->bufIn.cTerm!=CtrAssign) goto shortExp;
  pTr->errTr.err=CERtrRefFiled;
  return;
  }
 // вычислить выражение справа от равенства результат попадает на индексный регистр
 if  (pTr->bufIn.cTerm==CtrAssign) {
  pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp,0);
  trrfExp(pTr,CtrKodDlmList,CtrExpStatExp);
  if (pTr->errTr.err>CERR ) trendCnstExp(pTr,c0,c1);
  return;
  }
shortExp : 
 parrInspData(pTr->pcmds,pCmd);
 pcmdSet(pCmd,CvmCmdReg,CvmSubCmdRegUp,0);
 trendCnstExp(pTr,CvmCmdSetSh,c1);
 }
/*
 * Команда Kill
 */
void trArgCmdKill(Ttr *pTr,char c0,char c1){
 Tusize busy;
 // вычислить выражение результат попадает на индексный регистр
 busy=pTr->pcmds->busy;
 trrfExp(pTr,CtrKodDlmList,CtrExpStatRef);
 if (pTr->errTr.err<CERR) return;
 if (pTr->pcmds->ptop==NULL) {pTr->errTr.err=CERtrRefFiled;return;}
 if (pTr->ptopStat->_sub.cnstCmd) {
  if ( busy==pTr->pcmds->busy ) {
   pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp | pTr->ptopStat->_sub.cnstCmd , pTr->ptopStat->_sub.cnstArg);
   pcmdPush(pTr->pcmds,c0,CtypeKiLLAll,0);
   }
  else {
   pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp | pTr->ptopStat->_sub.cnstCmd , pTr->ptopStat->_sub.cnstArg);
   pcmdPush(pTr->pcmds,c0,c1,0);
   }
  }
 else{
  c1 &= CtypeKiLL;
  if (pCmdCmd(pTr->pcmds->ptop)==CvmCmdGet) {
   pcmdSet(pTr->pcmds->ptop,c0,c1,0);
   }
  else
   if (pCmdCmd(pTr->pcmds->ptop)==CvmCmdDO){
    pCmdArg(pTr->pcmds->ptop)=c1;
    pCmdSub(pTr->pcmds->ptop) |=CvmSubCmdDOKill;
    }
  }
 }
/*
 * Команда Go
 * Команда Do, Run
 */
void trArgCmdGoDoJob(Ttr *pTr,char c0,char c1){
 TCmd *pCmd;
 // вычислить выражение справа от равенства результат попадает на индексный регистр
 pCmd=pTr->pcmds->ptop;
 trrfExp(pTr,CtrKodDlmList,CtrExpStatRef);
 if (pTr->errTr.err>CERR) trendProgExp(pTr,c0,c1,pCmd);
 }
/**/
void trArgCmdRet(Ttr *pTr){
 //вычисляем значение. Результат окажентся на индексном стеке
 trrfExp(pTr,CtrKodDlmList,CtrExpStatExp);
 if (pTr->errTr.err>CERR){
  trendCnstExp(pTr,CvmCmdReturn,0);
  }
 }
 //
/*
 *   копировать узел в узел
 */

void trArgCmdCopy(Ttr *pTr,char c0,char c1){
 // вычислить выражение слева от равенства результат попадает на индексный регистр
 trExpRef(pTr,CCtrKodEqu,CtrExpStatRef);
 if (pTr->errTr.err>CERR) {
 // вычислить выражение справа от равенства результат попадает на индексный регистр
  if (pTr->bufIn.pcur >= pTr->bufIn.pcurEnd) return;
  trExpRef(pTr,CtrKodDlmList,CtrExpStatRef);
  if (pTr->errTr.err>CERR) {
   pcmdPush(pTr->pcmds,c0,c1,0);
   }
  }
 }
/**/
void trArgCmdExecute(Ttr *pTr,char c0,char c1){
 //найдем индекс приемника в Индексном регистре будет индекс переменной приемника
 trrfExp(pTr,CtrKodDlmList,CtrExpStatExp);
 if (pTr->errTr.err>CERR) trendCnstExp(pTr,c0,c1);
 }

/*
 * Временная задержка
 */
void trArgCmdTimeOut(Ttr *pTr,char c0,char c1){
 //найдем индекс приемника в Индексном регистре будет индекс переменной приемника
 trrfExp(pTr,CtrKodDlmList,CtrExpStatExp);
 if (pTr->errTr.err>CERR) trendCnstExp(pTr,c0,c1);
 }
/*
 *  Команда If
 */
void trArgCmdIf(Ttr *pTr,ToffMod *pcondJmp){
 //
 if (*pcondJmp){
  ptrstkIterPush(pTr,CtrTypeItrIf);
  pTr->boxIter.ptop->begCmd=*pcondJmp;
  }
 }
/*
 *  Команда Else
 */
void trArgCmdElse(Ttr *pTr,ToffMod *pcondJmp,ToffMod begCmd){
 TCmd *pCmd;
 
 //если предыдущего IF не было
 if ((pTr->boxIter.ptop==NULL)||(pTr->boxIter.ptop->type!=CtrTypeItrIf)) {
  pTr->errTr.err=CERRtrElseNoIf;
  return;
  }
 parrPushDan(pTr->boxIter.ptop,begCmd);
 //были команды проверки условия
 if (*pcondJmp){
  pCmd=parrOffpD(pTr->pcmds,begCmd);
  parrInspData(pTr->pcmds,pCmd);
  pcmdSet(pCmd,CvmCmdJmp,CvmJmpAny,0);
  }
 else{
  //в Блок сбросить коменду выхода на конец
  pcmdPush(pTr->pcmds,CvmCmdJmp,CvmJmpAny,0);
  }
 //перейти в эту точку с предыдущего условия
 if (pTr->boxIter.ptop->begCmd){
  CmdArg(pTr->pcmds->psec[pTr->boxIter.ptop->begCmd-1])=begCmd+1;
  }
 //проверка следующего условия
 pTr->boxIter.ptop->begCmd=(*pcondJmp)?pTr->pcmds->busy:0;
 }
/*
 * Команда CASE
 * результат условия помещается на индексный регистр
 */
void trArgCmdCase(Ttr *pTr){
 //проверить условие выполнения команды
 ptrstkIterPush(pTr,CtrTypeItrCase);
 pcmdSet(pTr->pcmds->ptop,CvmCmdCase,0,pTr->arr2Label.busy);
 //новая секция меток
 trLabelPush(pTr);
 }
/*
 * Итераторы
 */
void trArgCmdNext(Ttr *pTr,char vmCmd,char C2,ToffMod *pcondJmp){
 ToffMod beg;
 u_char isSaveInd=0;
 //блочные команды Next, Back, Query
 if (C2!='1') 
  {
  //Новый Итератор
  ptrstkIterPush(pTr,vmCmd);
  //обработать условие завершения цикла
  if (*pcondJmp){
   parrPushDan(ptrstkIterTop,*pcondJmp-1);
   }
  }
 setErrnumpos(pTr);
 pcmdPush(pTr->pcmds,CvmCmdMarker,pTr->errTr.numpos+1,pTr->errTr.numstr);
 //Опорный узел Итератора
 trExpRef(pTr,CtrKodDlmList,CtrExpStatRef);
 if (pTr->bufIn.pcur >= pTr->bufIn.pcurEnd) return;
 if (pTr->errTr.err<CERR) return;
 pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp,0);
 if (pTr->bufIn.cTerm == CtrKodDlmList) {
  beg=pTr->pcmds->busy;
  //узел сохранения индекса
  trExpRef(pTr,CtrKodDlmList,CtrExpStatRef);
  if (pTr->errTr.err<CERR) return;
  if (pTr->pcmds->busy > beg)
   isSaveInd=CvmCmdSubSaveInd;
  else
   if (C2=='1')
    {
    pTr->errTr.err=CERRtrIndSaveFiled;
    return;
    }
  }
 else
  if (C2=='1')  {
    pTr->errTr.err=CERRtrIndSaveFiled;
    return;
    }
 //следующая вершина Next1, Back1, Query1
 if (C2=='1'){
  pcmdPush(pTr->pcmds,vmCmd,CvmCmdSubNext1 | isSaveInd,0);
  }
 else {
  pcmdPush(pTr->pcmds,vmCmd,CvmCmdSubInit | isSaveInd,0);
  //если при Инициализации данных не обнаружено то уйти в конец цикла
  parrPushDan(pTr->boxIter.ptop,pTr->pcmds->busy-1);
  //начало цикла
  ptrstkIterTop->begCmd=pTr->pcmds->busy;
  }
 *pcondJmp=0;
 }
/*
 * Команда While
 */
void trArgCmdWhile(Ttr *pTr,ToffMod *pcondJmp,ToffMod begCmd){
 //
 ptrstkIterPush(pTr,CtrTypeItrFor);
 //начало цикла
 ptrstkIterTop->begCmd=begCmd;
 //обработать условие завершения цикла
 if (*pcondJmp) {parrPushDan(ptrstkIterTop,*pcondJmp-1);}
 }
/*
 *  Команда End
 */
void trArgCmdEnd(Ttr *pTr,ToffMod *pcondJmp){
 ToffMod *pOff;
 TCmd *pCmd;
 //выходы на END
 switch (ptrstkIterTop->type) {
  //конец команды Case
  case CtrTypeItrCase:
   if (ptrstkIterTop->begCmd){
    pCmd=parrOffpD(pTr->pcmds,ptrstkIterTop->begCmd-1);
    pCmdArg(pCmd)=pTr->pcmds->busy;
    }
   trLabelPop(pTr);
   break;
  //конец команды IF
  case CtrTypeItrIf:
   if (ptrstkIterTop->begCmd){
    pCmd=parrOffpD(pTr->pcmds,ptrstkIterTop->begCmd-1);
    pCmdArg(pCmd)=pTr->pcmds->busy;
    }
   break;
  //Конец Итераторов
  case CcmdNext  :
  case CcmdBack  :
  case CcmdQuery :
   //следующая Итерация
   if (*pcondJmp) {
    pCmdSub(pTr->pcmds->ptop)=CvmJmp1;
    parrPushDan(ptrstkIterTop,*pcondJmp-1);
    }
   pcmdPush(pTr->pcmds,ptrstkIterTop->type , CvmCmdSubNext,ptrstkIterTop->begCmd);
   break;
   //конец команды FOR
  case CtrTypeItrFor :
   //обработать условие завершения цикла
   if (*pcondJmp) {
    pCmdArg(pTr->pcmds->ptop)=ptrstkIterTop->begCmd;
    }
   else {
    pcmdPush(pTr->pcmds,CvmCmdJmp,CvmJmpAny,ptrstkIterTop->begCmd);
    }
   break;
  default : return; 
  }
 //выходы на END
 parrIter(ptrstkIterTop,pOff) {
  pCmd=parrOffpD(pTr->pcmds,*pOff);
  pCmdArg(pCmd)=pTr->pcmds->busy;
  }
 arrBoxPop(pTr->boxIter);
 }
/**/
void trArgCmdBreak(Ttr *pTr,ToffMod *pcondJmp){
 TCmd *pCmd;
 //нет блока прерывания
 if ((ptrstkIterTop)==NULL) {pTr->errTr.err=CERRtrBreakNoFo;return;}
 if (*pcondJmp) {
  (*pcondJmp)--;
  pCmd=parrOffpD(pTr->pcmds,*pcondJmp);
  pCmdSub(pCmd)=1;
  parrPushDan(ptrstkIterTop,*pcondJmp);
  *pcondJmp=0;
  }
 else{
  parrPushDan(ptrstkIterTop,pTr->pcmds->busy);
  pcmdPush(pTr->pcmds,CvmCmdJmp,CvmJmpAny,0);
  }
 }
/*
 *   заполнить область констант
 */
void getPachtLibDir(Tstr *pname,const char *pRas,int sizeDir,char *pout,int sizeout);
Terr DbOpenDiv(TDb *pDb,char* patch,enum TtypeDb typeDb);

static Terr DbCnst(TregTvmDb *psecCnst,Tstr *psName){
 Terr err;
 TvmDb *pDbD;
 int w;
 char pfl[CsizePacthFl];
 //Ищем глобаль в списке определенных
 //Нашли
 pRegIter(psecCnst,pDbD)
  //глобаль найдена
  if (StrpEqu(pDbD->name,psName)==CMPeq) return COutKodOk;
 //создаем глобаль на текущем узле pCdirDb[]
 pRegBusyNull(psecCnst);
 pDbD=psecCnst->ptop;
 //проверяем есть ли такие константы в модуле
 psName->psec[psName->size]='\0';
 getPachtLibDir(psName,pCmshDirVm,CsizePartModDef,pfl,CsizePacthFl);
 mshExtaddtoFL(pCmshExtCnst,pfl);
 w=strlen(pfl);
 pfl[w]=CdelimFile;
 pfl[w+1]='\0';
 err=DbOpenDiv(&(pDbD->db),pfl,EtypeDb);
 //
 if (err!=COutKodOk) {
  pRegPop(psecCnst);
  return err;
  }
 //если есть то он остается в списке модулей с константами 
 pDbD->name=*(psName);
 return COutKodOk;
 }
/**/
void trArgCmdConst(Ttr *pTr,char c0,char c1){
 char *pcb,*pce,*pMinus,C;
 char bf[CsizePacthFl];
 Tstr *pName;
 TConst *pCnst;
 //
 pName=(Tstr*)bf;
 if (trSpaceSkip(pTr)!=COutKodOk) return;
 //считываем имя константы
 trSelTerm(pTr,CCtrKodEqu);
 if (pTr->errTr.err<CERR) return;
 if (pTr->bufIn.cTerm!=CtrTypeTermID) {pTr->errTr.err=CERRtrFrmId;return;}
 pName->size=pTr->bufIn.pcur - pTr->bufIn.pcurOld;
 if (pName->size<1) {pTr->errTr.err=CERRtrNameFound;return;}
 if (pName->size > CsizeStr) {  pName->size=CsizeStr; }
 memCopy(pName->psec,pTr->bufIn.pcurOld,pName->size);
 //уйти с CCtrKodEqu
 if (trSpaceSkip(pTr)!=COutKodOk) return;
 if (*(pTr->bufIn.pcur)!=CCtrKodEqu) {
  /*
  pName->psec[pName->size++]=CdelimFile;
  w=strlen(pCmshExtCnst);
  memCopy(pName->psec+pName->size,pCmshExtCnst,w+1);
  pName->size +=w;
  */
  //импортировать константы модуля
  if ((pTr->errTr.err=DbCnst(&(pTr->secDbConst),pName)) != COutKodOk) pTr->errTr.err=CERRvmModNotFound;
  goto finish;
  }
 //уйти с =
 trSmbNext(pTr->bufIn.pcur);
 arrIter(pTr->arrCnst,pCnst)
  if (StrpEqu(pCnst->name,pName)==CMPeq) {
   //сбросить константу
   trSelTerm(pTr,CtrKodDlmList);
   goto finish;
   }
 //значение константы
 arrBusyNull(pTr->arrCnst);
 pTr->arrCnst.ptop->offMod=0;
 pTr->arrCnst.ptop->name=*pName;
 if (trSpaceSkip(pTr)!=COutKodOk) return;
 if (((*(pTr->bufIn.pcur))==CvmOpPlus)||((*(pTr->bufIn.pcur))==CvmOpMinus)){
  pMinus=pTr->bufIn.pcur++;
  }
 else
  pMinus=NULL;
 //считываем значение константы
 if (pTr->bufIn.pcur >= pTr->bufIn.pcurEnd) return;
 setErrnumpos(pTr);
 trSelTerm(pTr,CtrKodDlmList);
 if (pTr->errTr.err!=COutKodOk) return;
 //убрать кавычки
 pcb=pTr->bufIn.pcurOld;
 pce=pTr->bufIn.pcur;
 C='A';
 if (pTr->bufIn.cTerm==CCtrKodKav1){
  ++pcb;
  if ((*(pTr->bufIn.pcur-1)==CCtrKodKav1)||(*(pTr->bufIn.pcur-1)==CCtrKodKav2))
   --pce;
  if (pcb[0]=='\\'){
   C=(pcb[1]=='n')?CCvmEndStr:(pcb[1]=='t')?CCvmGT:(pcb[1]=='r')?CCvmBK:(pcb[1]=='0')?CCvmBK:'A';
   }
  }
 else
  if (pMinus) pcb=pMinus;
 if (C!='A'){
  DataSetStr(pTr->arrCnst.ptop->val);
  pTr->arrCnst.ptop->val.str.size=1;
  pTr->arrCnst.ptop->val.str.psec[0]=C;
  }
 else
  trBufToData(pTr->bufIn.cTerm,pcb,pce,&(pTr->areaString),&(pTr->arrCnst.ptop->val));
finish: //ищем окончание аргумента
 if (trSpaceSkip(pTr)!=COutKodOk) return;
 pTr->bufIn.cTerm=*(pTr->bufIn.pcur);
 trSmbNext(pTr->bufIn.pcur);
 }
/*
 * Extent расширение объектов
 */
void trArgCmdParent(Ttr *pTr,char c0,char c1){
 Tstr *pStr;
 //считываем имя константы
 trSelTerm(pTr,CCtrKodEqu);
 if (pTr->errTr.err>CERR) {
  if (pTr->bufIn.cTerm!=CtrTypeTermID) {pTr->errTr.err=CERRtrFrmId;return;}
  arrBusy(pTr->vmExt);pStr=pTr->vmExt.ptop;
  trBufToID(pTr->bufIn.pcurOld,pTr->bufIn.pcur,pStr);
  }
 return;
 }
/*
 * Команда Lock
 */
//
void trArgCmdLock(Ttr *pTr,char c0,char c1){
 trrfExp(pTr,CtrKodDlmList,CtrExpStatExp);
 if (pTr->errTr.err>CERR) trendCnstExp(pTr,c0,c1);
 }
/*
 * Обработка событий
 */
void trArgCmdEventDel(Ttr *pTr,char c0,char c1){
 trrfExp(pTr,CtrKodDlmList,CtrExpStatExp);
 if (pTr->errTr.err>CERR) trendCnstExp(pTr,c0,c1);
 }
//
void trArgCmdEventWait(Ttr *pTr,char c0,char c1){
 trrfExp(pTr,CtrKodDlmList,CtrExpStatExp);
 if (pTr->errTr.err>CERR) trendCnstExp(pTr,c0,c1);
 }
void trArgCmdEventCall(Ttr *pTr,char c0,char c1){
 TCmd *pCmd;
 // вычислить выражение слева от равенства
 trrfExp(pTr,CCtrKodEqu,CtrExpStatExp);
 if (pTr->errTr.err<CERR) return;
 ConstToCmdRegIf(pTr);
 // вычислить выражение справа от равенства
 pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp,0);
 pCmd=pTr->pcmds->ptop;
 if ((pTr->bufIn.pcur >= pTr->bufIn.pcurEnd)||(pTr->bufIn.cTerm!=CCtrKodEqu)) {
  pTr->errTr.err=CERRvmRpartNot;
  return;
  }
 trrfExp(pTr,CtrKodDlmList,CtrExpStatRef);
 if (pTr->errTr.err>CERR) trendProgExp(pTr,c0,c1,pCmd);
 }
void trArgCmdEventTrap(Ttr *pTr,char c0,char c1){
 trArgCmdGoDoJob(pTr,c0,c1);
 }
/*
 * Открыть устройство
 */
void trArgCmdOpen(Ttr *pTr,char c0,char c1){
 trArgCmdSet(pTr,c0,c1);
 }
/*
 * Закрыть устройство
 */
void trArgCmdClose(Ttr *pTr,char c0,char c1){
 trrfExp(pTr,CtrKodDlmList,CtrExpStatExp);
 if (pTr->errTr.err>CERR) trendCnstExp(pTr,c0,c1);
 }
/*
 */
void trArgCmdReadWrite(Ttr *pTr,char c0,char c1){
 trrfExp(pTr,CtrKodDlmList,CtrExpStatExp);
 if (pTr->errTr.err>CERR) trendCnstExp(pTr,c0,c1);
 }
