/*
 * File:   mshvmNet.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 14:51
 */
#include "../../mshDb/H/mshPgLong.h"
//
#include "mshvmVm.h"
#include "mshvmMacro.h"
//#include "mshCore.h"

void packArrData(TPacks *pPack,TarrData *parrData);
void packData(TPacks *pPack,Tdata *pData);
Tdata* unpackData(TPacks *pPack);
void unpackArrData(TPacks *pPack,TarrData *parrData);

static void packCmd(TPacks *pPack,TCmd *pCmd){
 Tbstr bufOut;
 //
 pPackClear(pPack);
 //Запаковать Команду
 bufOut.size=sizeof(TCmd);
 bufOut.psec=(char*)pCmd;
 packVoid(pPack,&bufOut);
 }

/*
 * Обращение к узлам сети  ??Заглушка??
 */
static Terr NetWrite(TPacks *pPack,Tbstr *path){
 return COutKodOk;
 }
static Terr NetRead(TPacks *pPack,Tbstr *path){
 pPackClear(pPack);
 //
 pPackPtr0(pPack);
 return COutKodOk;
 }

static Terr obmNet(TPacks *pPack,Tbstr *path){
 Terr err;
 if ((err=NetWrite(pPack,path))<CERR) return err;
 if ((err=NetRead(pPack,path))<CERR) return err;
 return COutKodOk;
 }
/*
 * Команды обращения к Сетевой Глобали
 */
Tval* NetGetStat(TvmDo *pDo,TarrData *parrInd,TCmd *pCmd,Tbstr *path){
 Tval *pVal;
 //
 packCmd(&(pDo->pJob->bufpack),pCmd);
 //Запаковать Индекс
 packArrData(&(pDo->pJob->bufpack),parrInd);
 //Обращение к сети
 if ((pDo->err=obmNet(&(pDo->pJob->bufpack),path))<CERR) return NULL;
 //Обработать возвращаемое значение
 pVal=(Tval*)(pDo->pJob->bufpack.Ptr.pPtrStr->psec);
 if (DataIsLong(pValKey(pVal) ) ) pValKey(pVal).datapmLong=&(pDo->refDb.pvmDb->db.mlong);
 if (DataIsLong(pValData(pVal) ) ) pValData(pVal).datapmLong=&(pDo->refDb.pvmDb->db.mlong);
 return pVal;
 }
/*
 */
void NetSet(TvmDo *pDo,TarrData *parrInd,TCmd *pCmd,Tbstr *path,Tdata *pData){
 //
 packCmd(&(pDo->pJob->bufpack),pCmd);
 //запаковать данные
 packData(&(pDo->pJob->bufpack),pData);
 //Запаковать Индекс
 packArrData(&(pDo->pJob->bufpack),parrInd);
 //Обращение к сети
 pDo->err=NetWrite(&(pDo->pJob->bufpack),path);
 }
/**/
void NetKill(TvmDo *pDo,TarrData *parrInd,TCmd *pCmd,Tbstr *path){
 packCmd(&(pDo->pJob->bufpack),pCmd);
 //Запаковать Индекс
 packArrData(&(pDo->pJob->bufpack),parrInd);
 //Обращение к сети
 pDo->err=NetWrite(&(pDo->pJob->bufpack),path);
 }
/**/
void NetNextQuery(TvmDo *pDo,TvmQuery *pQuery,TCmd *pCmd){
 TIterNext *pcurNext;
 TPacks *pPack;
 Tbstr bufOut;
 //
 pPack=&(pDo->pJob->bufpack);
 packCmd(pPack,pCmd);
 //
 pcurNext=(TIterNext*)pQuery;
 //команда Инициализации Итератора
 if ((pCmdSub(pCmd)==CvmCmdSubInit)||(pCmdSub(pCmd)==CvmCmdSubNext1)) {
  //Запаковать Индекс
  packArrData(pPack,&(pQuery->rootInd));
  packData(pPack,&(pQuery->Key));
  }
 //команда следующей Итерации
 else {
  //добавочный индекс
  packArrData(pPack,&(pQuery->addInd));
  //текущий курсор
  bufOut.size=sizeof(TIterNext);
  bufOut.psec=(char*)pcurNext;
  packVoid(pPack,&bufOut);
  }
 //Обращение к сети
 if ((pDo->err=obmNet(pPack,pQuery->path))<CERR) return;
 //Обработать возвращаемое значение
 pcurNext=(TIterNext*)(pPack->psec);
 if ((pcurNext->AdrOfferr.err=pDo->err)<CERR) return;
 //следующее значение
 pPackNextVoid(pPack);
 //сохранить Ключ
 ValData(pQuery->iterNext.val)=*(unpackData(pPack));
 //сохранить Данные
 ValData(pQuery->iterNext.val)=*(unpackData(pPack));
 if (pCmdSub(pCmd)==CcmdQuery) {
  //получить добавочный индекс
  unpackArrData(pPack,&(pQuery->addInd));
  }
 }
/*
 * Обмен с сервером
 */
void fsrvWrite(TvmDo *pDo,Tbstr *pBuf){
 if ((pDo->pJob->fsrvWrite)&&(pBuf->size)&&(pDo->pJob->divSrv.TCP!=CvmpTCPNULL) )
  pDo->err=(Terr)(pDo->pJob->fsrvWrite(pDo->pJob->divSrv,pBuf));
 }