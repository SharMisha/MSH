/*
 * File:   mshVmEvent.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:46
 */
#include "mshvmVm.h"
#include "mshvmMacro.h"

Tstr* castId(Tdata *pData);

TvmMod* getMod(Tstr *pname,Terr *perr);
Terr getLabelApp(TrefProg *prefProg);
void PopToData(TvmDo *pDo,TCmd *pCmd,Tdata *pOut);
void mshVmDo(TvmDo *pDo, TdivStream *pdiv);
TvmDo* vmGetDo(TJob *pJob);
Terr vmJobArgToLong(TarrData *pArrFrom,TDb *pDbLong);
Terr LongDelInd(Tlong *pLong,Tsize ind);

//
static void vmEventInit(TEvent *pEvent){
 arrInit(pEvent->arrCallEvn,CvmsumEvntBackDef);
 arrInit(pEvent->arrwaitEvn,CvmsumEvntWaitDef);
 arrInit(pEvent->args,CvmsizeSteckIndDef);
 waitInit(pEvent);
 }
static void vmEventClear(TEvent *pEvent){
 arrClear(pEvent->arrCallEvn);
 arrClear(pEvent->arrwaitEvn);
 arrClear(pEvent->args);
 }
static void vmEventFree(TEvent *pEvent){
 arrFree(pEvent->arrCallEvn);
 arrFree(pEvent->arrwaitEvn);
 arrFree(pEvent->args);
 waitFree(pEvent);
 }
/*
 */
static TEvent* GetEvent(Tstr *pevntName){
 TEvent *pEvent;
 //
 objGetSecs(App.secsEvent,pEvent);
 if (pEvent)
  vmEventClear(pEvent);
 else {
  pRegBusy( &(App.secsEvent) );
  pEvent=App.secsEvent.ptop;
  vmEventInit(pEvent);
  }
 objSetBusy(pEvent);
 pEvent->name=*pevntName;
 return pEvent;
 }
/*
 * найти событие
 */
TEvent* FindEvent(Tstr *pevntName){
 TEvent *pEvent;
 //в таблице нет имен
 if (App.secsEvent.busy < 1) return NULL;
 //поиск имен
 RegIter(App.secsEvent,pEvent)
  //имя найдено в таблице
  if ( (objIsFree(pEvent))&&((pEvent->name.size==pevntName->size))&&(StrpEqu(pEvent->name,pevntName)==CMPeq) ) return pEvent;
 return NULL;
 }
/*
 * найти групповое событие
 */
static TEvent* FindsubEvent(Tstr *pevntName){
 TEvent *pEvent;
 //в таблице нет имен
 if (App.secsEvent.busy<1) return NULL;
 //поиск имен
 RegIter(App.secsEvent,pEvent){
  if ( objIsFree(pEvent) ){
   if (pEvent->name.size==pevntName->size){
    //имя найдено в таблице
    if (StrpEqu(pEvent->name,pevntName)==CMPeq) return pEvent;
    }
   else if (pEvent->name.size>pevntName->size) {
    if (StrNpEqu(pEvent->name,pevntName,pevntName->size)==CMPeq) return pEvent;
    }
   }
  }
 return NULL;
 }
/*
 * удалить событие
 */
static void ClearEvent(Tstr *pevntName){
 TEvent *pEvnt;
 //
 spinLock(App.secsEvent.lock);
 if ((pEvnt=FindEvent(pevntName))) {
  //удалить длинные строки
  arrClear(pEvnt->args);
  objSetFree(pEvnt);
  }
 spinUnLock(App.secsEvent.lock);
 }
/*
void ClearsubEvent(TApp *pApp,Tstr *pevntName){
 TEvent *pEvnt;
 //
 spinLock(App.secsEvent.lock);
 while ((pEvnt=FindsubEvent(pevntName))) {
  arrClear(pEvnt->args);
  objSetFree(pEvnt);
  }
 spinUnLock(App.secsEvent.lock);
 }
*/
/*
 * Проверить есть ли такой обработчик события
 */
static Terr FindEventCall(TEvent *pevnt,Tstr *psnameMod,Tstr *pLabel){
 if (pevnt->arrCallEvn.busy<1) return COutKodOk;
 TrefProg *prefProg;
 //
 arrIter(pevnt->arrCallEvn,prefProg)
  if (StrpEqu(prefProg->pmod->name,psnameMod)==CMPeq){
   if (pLabel==NULL) return CMPeq;
   if (StrpEqu(prefProg->label,pLabel)==CMPeq) return CMPeq;
   }
 return COutKodOk;
 }
 /*
 * выполнить обработчики события
 */
static void EventCall(TvmDo *pDo,TEvent *pEvnt){
 TrefProg *prefProg;
 TvmDo *pDoNew;
 //запуск EventCall программ
 if (pEvnt->arrCallEvn.busy) {
  pDoNew=vmGetDo(pDo->pJob);
  //стек Аргументов перенести в вызываемый DoNew
  parrCopy(&(pDoNew->arArgs),&(pEvnt->args));
  arrIter(pEvnt->arrCallEvn,prefProg){
   pDoNew->pmod=prefProg->pmod;
   pDoNew->curOff = prefProg->off;
   mshVmDo(pDoNew,NULL);
   }
  //освободить блок Do,освободить задание.
  objpDopJobFree(pDoNew);
  }
 arrClear(pEvnt->arrCallEvn);
 }
/*
 * Установить обработчик события
 */
static TEvent* SetEventCall(TvmDo *pDo,TCmd *pCmd){
 TEvent *pEvnt;
 Tstr *psEvnt;
 Tdata Data;
 Tstr *psLab=NULL;
 TvmMod *pvmMod=NULL;
 int isEv=0;
 //
 //снимаем с вершины стека ссылку на программу обработки
 IndDoToArr(pDo,pDo->rarInd)
 if (pDo->rarInd.busy<2) {pDo->err=CERRvmNotArg; return NULL;}
 if (pDataIsNULL(pDo->rarInd.psec))
  pvmMod=pDo->pmod;
 else
  pvmMod=getMod(castId(pDo->rarInd.psec),&(pDo->err));
 psLab=castId( arrOffpD(pDo->rarInd,1));
 //снимаем заглушку
 stkIndLev0Pop(pDo);
 //находим имя события
 PopToData(pDo,pCmd,&Data);
 if ( pDo->err<CERR ) return NULL;
 if (pvmMod==NULL) {pDo->err=CERRvmNotArg; return NULL;}
 psEvnt=castId(&Data);
 //есть ли такое событие
 spinLock(App.secsEvent.lock);
 //события нет
 if ((pEvnt=FindEvent(psEvnt))==NULL) {
  pEvnt=GetEvent(psEvnt);
  }
 //событие есть
 else {
  isEv=1;
  //проверить есть ли еще такой обработчик
  if (FindEventCall(pEvnt,&(pvmMod->name),psLab)!=COutKodOk) goto finish;
  }
 // в событие добавить обработчик
 arrBusy(pEvnt->arrCallEvn);
 pEvnt->arrCallEvn.ptop->pmod=pvmMod;
 if (psLab) pEvnt->arrCallEvn.ptop->label=*psLab; else pEvnt->arrCallEvn.ptop->label.size=0;
 if ((pDo->err=getLabelApp(pEvnt->arrCallEvn.ptop))!=COutKodOk) {
  arrPop(pEvnt->arrCallEvn);
  }
finish:
 objSetBusy(pEvnt); 
 spinUnLock(App.secsEvent.lock);
 if (pDo->err<CERR) return NULL;
 if (isEv==0) return pEvnt;
 //если событие произошло то обработать его
 EventCall(pDo,pEvnt);
 return NULL;
 }
/*
 * Ожидающее событие Задание
 */
static void EventWait(TvmDo *pDo,TEvent *pEvnt){
 TvmDo **ppDoEvnt;
 //проверяем программы ожидающие этого события
 if (pEvnt->arrwaitEvn.busy) {
 //переносим Аргументы События на стеки Аргументов заданий
  arrIter(pEvnt->arrwaitEvn,ppDoEvnt) {
   parrCopy(&((*ppDoEvnt)->arArgs),&(pEvnt->args));
   }
  //запускаем обработку ожидающих программ
  waitSignal(pEvnt);
  }
 //Очистить обработчики События
 arrClear(pEvnt->arrwaitEvn);
 }
//
static TEvent* SetEventWait(Tstr *pevntName,TvmDo *pDo){
 TEvent *pEvnt;
 int isEv=0;
 //
 spinLock(App.secsEvent.lock);
 if ((pEvnt=FindEvent(pevntName))==NULL) {
  pEvnt=GetEvent(pevntName);
  }
 else
  isEv=1;
 arrBusy(pEvnt->arrwaitEvn);
 *(pEvnt->arrwaitEvn.ptop)=pDo;
  //событие уже произошло
 if (isEv) {EventWait(pDo,pEvnt); pEvnt=NULL;}
 spinUnLock(App.secsEvent.lock);
 //ждать события
 waitWait(pEvnt);
 return pEvnt;
 }
/*
 * Обработчики событий
 */
/*
 * программный вызов События
 */

void pArrToLong(TmapLong *pmLong,TarrData *pArr);
//
void EventTrapName(TvmDo *pDo,Tstr *psName){
 TEvent *pEvent;
 //ищем его в таблице событий
 spinLock((App.secsEvent.lock));
 //событие не найдено создать его и ожидать появления обработчиков
 if ((pEvent=FindEvent(psName))==NULL) {
  if ((pEvent=GetEvent(psName))==NULL) {pDo->err=CERRvmNotCreateEvnt;return;}
  }
 else {
  //событие найдено обработать
  //проверяем программы ожидающие этого события
  EventWait(pDo,pEvent);
  //запуск EventCall программ
  EventCall(pDo,pEvent);
  //освободить событие
  vmEventClear(pEvent);
  objSetFree(pEvent);
  }
 spinUnLock(App.secsEvent.lock);
 }

static void EventTrap(TvmDo *pDo,TCmd *pCmd){
 Tstr *psName;
 TEvent *pEvent;
 //находим имя события
 IndDoToArr(pDo,pDo->rarInd)
 if (pDo->rarInd.busy<2) {pDo->err=CERRvmEvntNoName;return;}
 psName=castId(arrOffpD(pDo->rarInd,1));
 //есть такое событие
 spinLock((App.secsEvent.lock));
 if ((pEvent=FindEvent(psName))==NULL) {
 //аргументы копируем в область событий, длинные строки переносим в pApp_>dbApp
  if ((pEvent=GetEvent(psName))==NULL)
   pDo->err=CERRvmNotCreateEvnt;
  else{
   parrCopy(&(pEvent->args),&(pDo->rarInd));
   pArrToLong(&(pDo->pJob->vmDbLong.db.mlong),&(pEvent->args));
   }
  }
 spinUnLock(App.secsEvent.lock);
 }
/*
 * команды VM обработки событий
 *  EventCall на регистре индекса Модуль и Метка обработки Cmd3-имя события
 *  EventTrap имя события и Аргументы находятся в индексном регистре
 *  EventWait , EventDelete Cmd2- имя события
 */
#define getNameEv(Reg,psName,Data) PopToData(pDo,pCmd,&Data);if ( (pDo->err>CERR)&&(DataIsNoNULL(Data)) )  psName=castId(&Data); else psName=NULL

void vmEvent(TvmDo *pDo,TCmd *pCmd){
 Tdata Data;
 TEvent *pEvnt;
 Tstr *psName;
 //
 switch (cmdSubpCmdEvent(pCmd)){
  //команда вызвать событие EventTrap
  case CvmSubCmdEventTrap:
   EventTrap(pDo,pCmd);
   break;
  //EventCall
  case CvmSubCmdEventCall:
   //установить событие
   pEvnt=SetEventCall(pDo,pCmd);
   break;
  //EventWait
  case CvmSubCmdEventWait :
   getNameEv(CregNumFrom,psName,Data);
   if (pDo->err<CERR) return;
   if ((pEvnt=SetEventWait(psName,pDo))==NULL) return;
   break;
   //EventDelete
  case CvmSubCmdEventDelete:
   getNameEv(CregNumFrom,psName,Data);
   if (pDo->err>CERR) ClearEvent(psName);
   break;
  }
 }
