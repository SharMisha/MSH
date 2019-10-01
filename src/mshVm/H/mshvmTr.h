/* 
 * File:   mshvmTr.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 4 января 2017 г., 15:33
 */

#ifndef MSHVMTR_H
#define MSHVMTR_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "../../mshDb/H/mshData.h"
#include "../../mshDb/H/mshArea.h"
#include "../../mshDb/H/mshMap.h"
#include "../../mshDb/H/mshErr.h"
#include "mshvmMod.h"
#include "mshvmTask.h"
//
typedef struct {
 int numstr; //транслируемая строка
 int numpos; //позиция трансляции
 Tstr nameMod;
 Terr err;
 } TerrTr;

#define errTrInit(errTr) errTr.numstr=errTr.numpos=0;errTr.name.size=0;errTr.err=COutKodOk
#define perrTrInit(perrTr) perrTr->numstr=perrTr->numpos=0;perrTr->name.size=0;perrTr->err=COutKodOk
/*
 *  Структура командной строки
 *   ---- команды обращения к данным ----
 *   первые 4 байта команда
 *    pcmd[0].код команды
 *    pcmd[1].Sub команда
 * регистр источника
 *    pcmd[2] Индекс 
 * регистр приемника
 *    pcmd[3] Данные
 *   вторые 4 байта аргументы
 * аргумент источника
 *    arg[2] аргумент индекса
 * аргумент приемника
 *    arg[3] аргумент данных
 *   ----Copy--------
 *    0. код команды 
 *    1. Sub команда копирование или пересылка
 *    2.  приемник
 *    3.  источник
 *   ---- команды обмена между регистрами ----
 *    0. код команды 
 *    1. выполняемое над регистром действие
 *    2.  приемник или исходный регистр
 *    3.  источник если действие это пересылка
 *   ---- команда Lock ----
 *    0. код команды
 *    1. тип блокировки
 *    2.  Идентификатор блокировки
 *    3.  тип TimeOut
 * результат проверки статуса блокировки загружается на регистр данных !!
 *   ---- команда TimeOut ----
 *    0 .код команды
 *    2. TimeOut
 *   ---- команда Do ----
 *    0 .код команды
 *    1. флаг вызова блока Do
 *    2. имя модуля
 *    3. имя метки
 *   ---- команда Return ----
 *    0 .код команды
 *    2. возвращаемое Значение
 *   ---- команда Case ----
 *    0 .код команды
 *    2. Значение метки
 *    off3 номер секции меток команды CASE
 * 
 */
/**/
typedef struct { _contArr(TCmd); } TarrCmd;
/* таблица констант программы msh */
typedef struct {
 Tdata val;
 Tstr name;
 ToffMod offMod;
 } TConst;
//массив констант 
typedef struct { _contArr(TConst); } TarrConst;
// массив строк
typedef struct { _contArr(Tstr); } TarrStr;
/*
 * код состояния разбора
 */
typedef struct {
 _contArr(ToffMod);//выходы на конец
 ToffMod begCmd; //
 char type; //тип блока
 } TtrIter;
/*
 *константу перенести на стек данных
 */
#define ConstToCmdRegIf(pTr) ({if (pTr->ptopStat->_sub.cnstCmd) {cmdPushCnst(pTr,CvmCmdReg,CvmSubCmdRegMove );} })
#define ConstToCmdReg(pTr) cmdPushCnst(pTr,CvmCmdReg,CvmSubCmdRegMove )
 
typedef struct sttTBoxIter { _contArrBox(TtrIter); } TBoxIter;

#define	ptrstkIterTop (arrptop(pTr->boxIter))
#define	ptrstkIterPush(pTr,typeIter) arrBoxPush(pTr->boxIter);ptrstkIterTop->type=typeIter
//состояние трансляции команд 
typedef struct {
 char *pconstBf; // буфер предконстанты
// char lconstBf; // длинна буфера предконстанты
 TCmd lateCmd; //отложенная команда
 Toff offCmd; //смещение команды ?
 int16_t cnstArg;  //аргумент
 u_char  cnstCmd;  //команда
 u_char st; //текущее состояние уровня стека
 u_char isNoUno; //признак первого элемента выражения
 u_char isThis; //признак первого элемента выражения
 } TtrStsub;
// 
typedef union {
 u_int16_t c2;
 char c1[sizeof(u_int16_t)];
 } TtrOp;
 
typedef struct {
 _contArr(TtrOp);
 TtrStsub _sub;
 } TarTtrOp;


#define	trOpNew(pSt) parrBusyNull(pSt)
#define	trOpOff(pSt,Off) pSt->ptop->c1[Off]
#define	trOpPop(pSt) parrPop(pSt)
 
typedef struct {
 _contArr2up(TarTtrOp);
 } TStat;

#define statSet(pTr,St) pTr->ptopStat->_sub.st=St
#define statPush(pTr,St) ({arr2subPush(pTr->stat); pTr->ptopStat=pTr->stat.ptop; statSet(pTr,St);})
#define statPop(pTr)  arr2subPop(pTr->stat); pTr->ptopStat=pTr->stat.ptop; endArr(pTr)
/*
 * Обмен данными с Глобалями
 */
#define _contvmDb TLock *plock;enum TisGlb isglb;Tstr name;TDb db
 
typedef struct TstvmDb { _contvmDb; }TvmDb;
 //Инициализировать pvmDb
#define pvmDbInit(pJob,pvmDb,psName,dbisLock,isGlb) (pvmDb)->isglb=isGlb; (pvmDb)->name=*(psName);\
 (pvmDb)->plock=((dbisLock==EisLock) && pJob)?LockResNew():NULL
//
#define vmDbInit(pJob,vmDb,sName,dbisLock,isGlb) (vmDb).isglb=isGlb; (vmDb).name=sName;\
 (vmDb).plock=((dbisLock==EisLock) && pJob)?LockResNew():NULL

//#define vmDbParInit(vmDb) vmDb.name.size=0;vmDb.plock=NULL;vmDb.isGlb=0
#define vmDbFree(vmDb) mshDbFree( (TDb*)(&(vmDb.db)) );vmDb.name.size=0; if (vmDb.plock) pLockFree(vmDb.plock)
#define vmpDbFree(pvmDb) mshDbFree( (TDb*)(&(pvmDb->db)) );pvmDb->name.size=0; if (pvmDb->plock) pLockFree(pvmDb->plock)
 
//#define arrDbFree(arrDb) ({TvmDb *pDb; arrIter(arrDb,pDb) {mshDbClear(&(pDb->db));vmpDbFree(pDb);} arrFree(arrDb);})
//#define arrDbClear(arrDb) ({TvmDb *pDb; arrIter(arrDb,pDb) mshDbClear(&(pDb->db));})
//#define arrDbInit(arrDb) arrInit(arrDb,CarrDbsizeDef)
 /*  */
typedef struct {
 _contReg(TvmDb);
 } TregTvmDb;
 
typedef struct {
 TregTvmDb regDb;
 TregTvmDb regArr;
 TspinLock lock;
 } TsecsvmDb;

 
#define secsvmDbInit(Glbs) RegInit(Glbs.regDb,CvmsumLockDef);RegInit(Glbs.regArr,CvmsumLockDef);spinLockInit(Glbs.lock)
#define secsvmDbFree(Glbs) {TvmDb *pvmDb; RegIter(Glbs.regDb,pvmDb) {  vmpDbFree(pvmDb); } \
   RegIter(Glbs.regArr,pvmDb) {  vmpDbFree(pvmDb); } }\
  RegFree(Glbs.regDb);RegFree(Glbs.regArr);spinLockFree(Glbs.lock)
#define secsvmDbClear(Glbs) RegClear(Glbs.regDb);RegClear(Glbs.regArr)

 
typedef struct {
 TmapFixed map;
 char *pBuf; //Входной буфер
 char *pbegStr; //позиция начала строки
 char *pcur; //текущая позиция входного буфера
 char *pcurOld; //предыдущая позиция входного буфера
 char *pcurEnd; //конец входного буфера
 Tstr name; // имя модуля
 u_char cTerm; //Терм
 u_char OldTerm; //предыдущий Term
 u_char sumTerm;
 } TbufIn;
//блок трансляции 
typedef struct sttTtr {
 TStat stat;
 TarTtrOp *ptopStat;
 TbufIn bufIn;
 TerrTr errTr;
 //области отранслированных данных
 Tarr2Label arr2Label; //секции меток
 TarrTLabel *pcurSecLabel;
 TBoxIter boxIter; //блок итераторов
 TarrCmd Cmds; //область оттранслированных команд
 TarrCmd *pcmds; // указатель на текущую область команды VM
 TarrConst arrCnst; //таблица констант
 TarrStr vmExt; // модули расширения
 TarrData arrData; // область данных
 Tarea areaString; //область размещения длинных строк
 TregTvmDb secDbConst; //константы внешних модулей
 Tusize busymax;
 Tdata tempKey; //для поиска констант
 TarrData tempInds; //
 char cbufErr[1024];
 } Ttr;

#define subCmdIsReg(subCmd) subCmd & CregMask
#define	CtrAssign '='
//
#define statIsArr(pTr) pTr->ptopStat->_sub.st == CtrExpStatArr
#define statIsNoArr(pTr) pTr->ptopStat->_sub.st != CtrExpStatArr
 
#define statIsMod(pTr) pTr->ptopStat->_sub.st == CtrKodMod
#define statIsNoMod(pTr) pTr->ptopStat->_sub.st != CtrKodMod
#define statIsArg(pTr) pTr->ptopStat->_sub.st == CtrKodArg
#define statIsNoArg(pTr) pTr->ptopStat->_sub.st != CtrKodArg
#define statIsModArg(pTr) (pTr->ptopStat->_sub.st == CtrKodMod)||(pTr->ptopStat->_sub.st == CtrKodArg)
#define statIsNoModArg(pTr) (pTr->ptopStat->_sub.st != CtrKodMod)&&(pTr->ptopStat->_sub.st != CtrKodArg)
 
#define statIsInd(pTr) pTr->ptopStat->_sub.st == CtrKodIndBeg
#define statIsNoInd(pTr) pTr->ptopStat->_sub.st != CtrKodIndBeg
#define statIsList(pTr) pTr->ptopStat->_sub.st == CtrKodListBeg
#define statIsNoList(pTr) pTr->ptopStat->_sub.st != CtrKodListBeg
#define statIsIndList(pTr) (statIsInd(pTr)) || (statIsList(pTr))
#define statIsNoIndList(pTr) (statIsNoInd(pTr)) && (statIsNoList(pTr))
 
#define statIsTrue(pTr) pTr->ptopStat->_sub.st == CtrCmdCond
#define statIsNoTrue(pTr) pTr->ptopStat->_sub.st != CtrCmdCond
#define statIsFalse(pTr) pTr->ptopStat->_sub.st == CtrCmdCondFalse
#define statIsNoFalse(pTr) pTr->ptopStat->_sub.st != CtrCmdCondFalse
 
#define statIsBkt(pTr) (pTr->ptopStat->_sub.st == CtrKodBktBeg)
#define statIsNoBkt(pTr) (pTr->ptopStat->_sub.st != CtrKodBktBeg)
//#define statIsNoCond(pTr) (pTr->ptopStat->_sub.st != CtrCmdCondFalse ) && ( pTr->ptopStat->_sub.st != CtrCmdCond )
 
#define statIsDlmInd(pTr) ( pTr->ptopStat->_sub.st == CtrExpStatArr )||( pTr->ptopStat->_sub.st == CtrKodIndBeg )||( pTr->ptopStat->_sub.st == CtrKodListBeg )
 
#define setErrnumpos(pTr) pTr->errTr.numpos=pTr->bufIn.pcurOld-pTr->bufIn.pbegStr;if (pTr->errTr.numpos<0) pTr->errTr.numpos=0
 
#define trupExp(pTr,cEnd,cSt) pcmdPush(pTr->pcmds,CvmCmdReg,CvmSubCmdRegUp,0); trrfExp(pTr,cEnd,cSt)

 
#ifdef __cplusplus
}
#endif

#endif /* MSHVMTR_H */

