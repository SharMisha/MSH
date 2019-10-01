/*
 * File:   mshvmNet.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Декабря 2016 г., 19:23
 */
#include "mshvmTr.h"
#include "mshvmCnst.h"
#include "../../mshDb/H/mshDb.h"

void mshDbFree(TDb *pDb);
/* вспомогательные программы */
void trIterInit(TtrIter *psec,void *pUser){
 memNULL(psec);
 parrInit(psec,CsizeBoxIterDef);
 }

void trIterFree(TtrIter *psec,void *pUser){
 parrFree(psec);
 }

void trIterClear(TtrIter *psec,void *pUser){
 psec->type=0;
 psec->begCmd=0;
 parrClear(psec);
 }
/*
 * Стек состояния блока трансляции
 TStat stat;
 //области отранслированных данных
 Tarr2Label arr2Label; //секции меток
 TarrTLabel *pcurSecLabel;
 TBoxIter boxIter; //блок итераторов
 TarrCmd Cmds; //область оттранслированных команд
 TarrCmd subCmds; //дополнительная область оттранслированных команд
 TarrConst arrCnst; //таблица констант
 TarrStr vmExt; // модули расширения
 TarrData arrData; // область данных
 Tarea areaLong; //область размещения длинных строк
 TarrCmd *pcmds; // указатель на текущую область команды VM
 struct sttTApp *pApp; //ссылка на приложение
 */
void TrInit(Ttr *pTr){
 memNULL(pTr);
 arr2subInit(pTr->stat,CtrsizeTtrStCtrlDef,CtrsizeTtrStDef);
 arr2Init(pTr->arr2Label,CtrSum2LabelDef,CtrSizeLabelDef);
 trLabelPush(pTr);
 arrBoxInit(pTr->boxIter,CsizeBoxIterDef,NULL,trIterInit,trIterClear,trIterFree);
 arrInit(pTr->Cmds,CtrSizeCmdDef);
 arrInit(pTr->arrCnst,CtrSizeConstDef);
 arrInit(pTr->vmExt,CtrSizeExtDef);
 arrInit(pTr->arrData,CtrSizeDataDef);
 areaInit(pTr->areaString,CtrSizeAreaLongDef);
 RegInit(pTr->secDbConst,CvmsumLockDef);
 //
 pTr->pcmds=&(pTr->Cmds);
 pTr->tempInds.busy=pTr->tempInds.capa=pTr->tempInds.size=1;
 pTr->tempInds.psec=pTr->tempInds.ptop=&(pTr->tempKey);
 DataSetbStr(pTr->tempKey);
 }
void TrFree(Ttr *pTr){
 TvmDb *pDbD;
 //очистить стеки деревьев
 RegIter(pTr->secDbConst,pDbD){
  pDbSteckFree(&(pDbD->db));
  }
 RegFree(pTr->secDbConst);
 areaFree(pTr->areaString);
 arrFree(pTr->arrData);
 arrFree(pTr->vmExt);
 arrFree(pTr->arrCnst);
 arrFree(pTr->Cmds);
 arrBoxFree(pTr->boxIter);
 arr2Free(pTr->arr2Label);
 arr2subFree(pTr->stat);
 }
void TrClear(Ttr *pTr){
 arr2subClear(pTr->stat);
 arr2Clear(pTr->arr2Label);
 trLabelPush(pTr);
 //
 arrBoxClear(pTr->boxIter);
 arrClear(pTr->Cmds);
 arrClear(pTr->arrCnst);
 arrClear(pTr->vmExt);
 arrClear(pTr->arrData);
 areaClear(pTr->areaString);
 RegClear(pTr->secDbConst);
 }
