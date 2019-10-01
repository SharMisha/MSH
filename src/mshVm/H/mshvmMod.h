
/* 
 * File:   mshMod.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 7 ноября 2016 г., 20:02
 */

#ifndef MSHMOD_H
#define MSHMOD_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "../../mshDb/H/mshArea.h"
#include "../../mshDb/H/mshPack.h"
#include "../../mshDb/H/mshCnst.h"
/*
 * Описание метки в модуле
 */
/*
 *  смещение меток в модуле от начала области команд
 */
typedef struct {
 ToffMod off; //смещение
 Tstr label; //имя метки
 } TLabel;
//
 
typedef struct {_contArr(TLabel);} TarrTLabel;
typedef struct {_contArr2up(TarrTLabel);} Tarr2Label;

#define	LabelsInit(arrLabel) arr2Init(arrLabel,CsizeLabelDef,CsizeCtrlLabelDef)

#define trLabelPush(pTr) arr2Push(pTr->arr2Label); pTr->pcurSecLabel=arrptop(pTr->arr2Label)
#define trLabelPop(pTr) if (pTr->pcurSecLabel > pTr->arr2Label.psec) pTr->pcurSecLabel--
// оценка области команд
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
typedef union {
 u_int32_t c4; //
 int16_t c2[2]; //аргументы
 u_char c1[4];  //команды
 } TCmd;
/* 
//аргумент команды
typedef union {
 int16_t arg;
 u_char  cmd[2];
 } TArgCom;
*/
/* 
typedef struct {
 TCmd *pbegcmds; // начало команд
 ToffMod endCmnds; //количество команды
 } TflowCmd;
 */
 /*
 *  заголовок Модуля
 */
/*
 *    Описание модуля на уровне приложения
 */
typedef struct {
 TPacks modpack; //буфер модуля
 Tmap mapmod; //отображение файла модуля в память
 Tarr2Label labels; //метки
 Tarea areaString; //область размещения длинных строк
 Tstr  *pext;   // начало области родителей объекта
 Tdata *pdata;   // начало области данных
 TCmd *pbegcmds; // начало команд
// TflowCmd flowCmd;
 Tsize sumext;
 Tsize sumdata;
 Tsize sumcmd;
 Tstr name;   // имя модуля
 } TvmMod;
 
//#define	vmModInit(Mod) memNULL(&Mod);PackInit((Mod).modpack);
//#define	pvmModInit(pMod) memNULL(pMod);PackInit((pMod)->modpack);
#define	pvmModInit(pMod) memNULL(pMod)
//#define	vmModFree(Mod) if (Mod.mapmod.psec) {mapFixedFree(Mod.mapmod);}PackFree((Mod).modpack)
//#define	pvmModFree(pMod) if ((pMod)->mapmod.psec) {mapFixedFree((pMod)->mapmod);}PackFree((pMod)->modpack)
#define	vmModFree(Mod) mapFixedFree((Mod).mapmod)
#define	pvmModFree(pMod) mapFixedFree((pMod)->mapmod)
//#define	vmModClear(Mod) //memNULL(&(Mod));areaClear((Mod).areaString);
#define	vmModClear(Mod) areaClear((Mod).areaString);\
  areaClear((Mod).modpack);(Mod).modpack.psec=(Mod).mapmod.psec;arr2Clear((Mod).labels)
//#define	pvmModClear(pMod) memNULL((pMod));areaClear((pMod)->areaString);
#define	pvmModClear(pMod) areaClear((pMod)->areaString);\
  areaClear((pMod)->modpack);(pMod)->modpack.psec=(pMod)->mapmod.psec;arr2Clear((pMod)->labels)

#define vmModbStrToMod(bStr,Mod) (Mod).modpack.psec=(bStr).sec;\
  (Mod).size=(Mod).busy=mshDivUp((bStr).size,sizeof(Tsize))
#define vmpModpbStrToMod(pbStr,pMod) (pMod)->modpack.psec=(pbStr)->sec;\
  (pMod)->size=(pMod)->busy=mshDivUp((pbStr)->size,sizeof(Tsize))

//#define	vmModNew(secMod) pSecsBusy( &(secMod) );pvmModInit( (secMod).psectop )
//#define	vmModPop(secMod) pvmModFree((secMod).psectop); pSecsPop( &(secMod) )


#ifdef __cplusplus
}
#endif

#endif /* MSHMOD_H */

