
/* 
 * File:   mshvmVm.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 декабря 2016 г., 9:28
 */

#ifndef MSHVM_H
#define MSHVM_H

#ifdef __cplusplus
extern "C" {
#endif

//
#include "../../mshDb/H/mshDb.h"
#include "../../mshDb/H/mshData.h"
#include "../../mshDb/H/mshArea.h"
#include "../../mshDb/H/mshDiv.h"
#include "../../mshDb/H/mshPack.h"
#include "../../mshDb/H/mshMap.h"
#include "../../mshDb/H/mshIni.h"
//
#include "mshvmTr.h"
#include "Net.h"

/*
 * Массив Индексов
 */
//typedef struct { _contArr(Tdata); } TarrInd;

enum TisLock{
 EisLockNo,
 EisLock
 };
//стек ссылок на объекты 
 /*
typedef struct {
 _contStk2(Tdata);
 } TstkThis;
 */
//3-х мерный стек индексов 
typedef struct {
 _contStk3(Tdata);
 } TstkInd;

#define stkInd(pDo) pDo->pJob->inds
 
//#define arr_stkInd(pDo) pDo->pJob->inds._arr
//#define arr_stkIndptop(pDo) pDo->pJob->inds._arr.ptop

#define stkIndInit(arrInd) Stk3Init(arrInd,CvmIndsDef,CvmIndsct0Def,CvmIndsct1Def)
#define stkIndFree(arrInd) Stk3Free(arrInd)
// 
#define stkIndLevClear(arrInd)	Stk3Lev1Clear(arrInd)
#define stkIndAllClear(arrInd) Stk3AllClear(arrInd)
#define stkIndLev1Up(pDo) Stk3Lev1Up(stkInd(pDo))
#define stkIndLev1Pop(pDo) Stk3Lev1Pop(stkInd(pDo))
 
#define stkIndLev0Up(pDo) Stk3Lev0Up(stkInd(pDo))
#define stkIndLev0Pop(pDo) Stk3Lev0Pop(stkInd(pDo))
#define stkIndLev0Clear(pDo) Stk3Lev0Clear(stkInd(pDo))
 
#define stkIndPopTop(pDo,pTop) pTop=Stk3ptop(stkInd(pDo)); Stk3Pop(stkInd(pDo))
#define stkIndPop(pDo) Stk3Pop(stkInd(pDo))
//#define stkIndClear(pDo) Stk3Clear(stkInd(pDo))
#define stkIndBusy(pDo) Stk3Busy(stkInd(pDo))
#define	stkIndBusypTop(pDo,pTop) Stk3Busy(stkInd(pDo)); pTop=((stkInd(pDo))._core.ptop)
#define stkIndPushData(pDo,Data) Stk3Busy(stkInd(pDo)); *((stkInd(pDo))._core.ptop)=Data
#define stkIndTopData(pDo) Stk3ptop(stkInd(pDo))

#define IndDoToArr(pDo,Arr) Stk3toArr(stkInd(pDo),Arr);stkIndLev0Pop(pDo)
//#define stkIndLev0PopArr(pDo,Arr) Stk3Lev0Pop(stkInd(pDo));Arr=*((TarrData*)(&(stkInd(pDo)._arr)))
/* таблица файлов */
typedef struct {
 Tstream *pfile;  //
 TintNum id; //идентификатор
 int sum; //количество открытых файлов
 Toff off; //смещение имени в области имен
 TmapFixed map;
 } Tfile;
 
typedef struct {
 TvmDb *pvmDb;
 u_int16_t off;
 TarrData rInds; //ссылочный индекс обращения к дереву данных
// TarrData *pnameInd;
 TarrData fullInds; //полный индекс
 Tbstr *pPath;
 u_char style;
 } TrefDb;
 
typedef struct {
 int (*pfdata)(TtreeData *ptreeData);
 void *pUser;
 u_char isglb; //имя метки
 } TrounDb;

 
#define refDbInit(refDb) memNULL(&(refDb)); arrInit(refDb.fullInds,CvmIndsDef)
#define refDbClear(refDb) refDb.pvmDb=NULL;refDb.off=0;refDb.style=0; arrClear(refDb.fullInds); if (refDb.pPath) refDb.pPath->size=0
#define prefDbClear(prefDb) prefDb->pvmDb=NULL;prefDb->off=0;prefDb->style=0; arrClear(prefDb->fullInds); prefDb->pPath=NULL
#define refDbFree(refDb) arrFree(refDb.fullInds)
 
 /*
 * Описание задания
 */

typedef int (*TfsrvWrite)(TdivSrv divSrv,Tbstr *pBuf);
 
struct sttTApp;
struct sttTtr;
//
typedef struct {
 int iD; //идентифиватор задания
// struct sttTApp *pApp; //ссылка на приложение
 // Регистры
 TstkInd  inds; //индексный регистр
// TstkThis stkthis; //стек ссылок на объекты
 TarrData thisObj; //ссылка на объект
 TvmDb vmDbLong; //длинные строки
 TvmDb vmDbBuf; //вспомогательный буфер
 TmapFixed mapBf; //отображение файла в память для операций ввода вывода
 TsecsvmDb secDbJob; //локальные деревья задания
 TarrData arrTemp; //вспомогательный массив
 struct sttTtr exeTr; //блок трансляции
 TPacks bufpack; //буфер сериализации
 Tfile *pfile; //текущее устройство
 pthread_t idPth; //идентификатор потока
 pthread_cond_t waitEvent; //Управление ожиданием событий
 TdivSrv divSrv; //основной порт связи с сервером
 TfsrvWrite fsrvWrite; //программа обмена с сервером
 Tval val; //последнее обращение к базе
 Tval valquery; //последнее обращение к базе в Итераторах
 Tstr *psMod;
 Tstr *psLab;
 Tstr sMod;
 Tstr sLab;
 uint isbusy; //свободно=0 или используется задание=1
 TerrTr errTr;//код возврата
 Tstrshr dlmIO;
 Tstr nameJob;
 } TJob;
/*
 * Итераторы
 */ 
typedef struct {
 _contIterQuery;
 TrefDb qrefDb;
 TrefDb qrefDbSave;
 Tdata Key;
 Tarea areaStr;
 Tbstr *path;
 } TvmQuery;
 /*
 * Box Итераторов
 */
_structArrBoxStruct(TBoxIterQuery,TvmQuery);

#define	vmBoxQueryInit(Box) arrBoxInit(Box,CvmsizeQueryDef,NULL,vmcurQueryInit,vmcurQueryClear,vmcurQueryFree)
/*
*/ 
 /*
 * блок выполнения команд
 */
typedef struct TvmsttDo {
 int iD; //идентификатор блока
 TJob *pJob; //ссылка на свое задание
 // Регистры
 TarrData arArgs; //аргументы текущего блока Do
 TvmDb dbDdo; //локальное дерево блока
 TvmDb dbAdo; //локальный массив блока
 TBoxIterQuery curquery; //Регистр Итераторов
 TarrInt arrJob; //стек запущеных из блока заданий
 TarrData rarInd; //текущий индекс блока Do
 Tarea Temps; //Область промежуточных данных
 TarrData this; //ссылка на объект
 //
 TrefDb refDb; //ссылка на последний индекс по которому было обращение Get, Set, Kill
// TarrData *piobj; //индекс объекта текущего блока Do
 TvmMod *pmod; // ссылка на описание модуля в App
// TflowCmd *pflowCmd;
 struct TvmsttDo *pDoParent;  //Блок Do который вызвал данный блок
 Terr err;
 Tsize curOff; //смещение текущей команды
 Tstr nameDo; //имя блока
 char reserv[4]; //дополнительное место для продолжения строки
 uint isbusy; //свободно=0 или используется задание=1
 u_char flag; //флаги блока выполнения
 u_char stat; //статус выполнения Do
 u_char eof; //конец операции ввода вывода
 } TvmDo;
/*
 * Описание TvmDo*
 */
typedef struct sttTBoxDo {
 _contReg(TvmDo);
 TspinLock lock;
 } TarrDo;
 
#define objpDopJobFree(pDo) objSetFree(pDo); objSub(pDo->pJob)
 
//#define arrDoInit RegInit(App.arrDo,CvmsizeboxAppDef);spinLockInit(App.arrDo.lock)
//#define arrDoFree RegFree(App.arrDo);spinLockFree(App.arrDo.lock)
/*
 * Описание TvmJob*
 */ 
typedef struct sttTBoxJob {
 _contReg(TJob);
 TspinLock lock; // управление доступом к таблице заданий
 } TarrJob;

//#define arrJobInit RegInit(App.arrJob,CvmsizeboxAppDef);spinLockInit(App.arrJob.lock)
//#define arrJobFree RegFree(App.arrJob);spinLockFree(App.arrJob.lock)
// 
typedef struct {
 _contReg(Tfile);
 Tarea names;
 } TsecsFile;
/*
 *   Управление событиями
 */ 
//массив ссылок блоки выполнения ожидающие возникновения события 
typedef struct { _contArr(TvmDo*); } TarrWait;
/*
 * ссылка на программу
 */ 
typedef struct {
 TvmMod *pmod;
 ToffMod off;
 Tstr label;
 } TrefProg;
//массив ссылок на программу 
typedef struct { _contArr(TrefProg);  } TarrrefProg;

typedef struct {
 _contWait;   
 TarrWait arrwaitEvn; //массив ссылок на программу обработки ожидания события
 TarrrefProg arrCallEvn; //массив ссылок на CallBack программы обработки события
 TarrData args; //аргументы события
 Tstr name; //имя события
 //
// pthread_mutex_t lock; //управление ожидающими событиями
// pthread_cond_t wait; //
 uint isbusy; //свободно=0 или используется задание=1
 } TEvent;

typedef struct {
 _contReg(TEvent);
 TspinLock lock; // управление доступом к таблице заданий
 } TsecsEvent;
 
//#define boxEventInit(App) RegInit(App.secsEvent,CvmsizeboxAppDef); spinLockInit(App.secsEvent.lock)
//#define boxEventFree(App) RegFree(App.secsEvent);spinLockFree(App.secsEvent.lock)

/* массив модулей */
typedef struct {
 _contReg(TvmMod);
 TspinLock lock;
 } TsecsMod;

//#define secsModInit(App) RegInit(App.Mods,CvmsumModDef); spinLockInit(App.Mods.lock)
//#define secsModFree(App) RegFree(App.Mods);spinLockFree(App.Mods.lock)
/* блокировки команды Lock */
typedef struct {
 TLock lock;
 Tsize ijob;
 Tstr name;
 } TctrlLock;

/* таблица блокировок */
typedef struct {
 _contReg(TctrlLock);
 TspinLock lock;
 } TsecsLock;
//#define secsLocksInit(App) RegInit(App.Locks,CvmsumLockDef); spinLockInit(App.Locks.lock)
//#define secsLocksFree(App) { TctrlLock *pctrlLock;RegIter(App.Locks,pctrlLock) {pLockFree(&(pctrlLock->lock));} }\
  RegFree(App.Locks); spinLockFree(App.Locks.lock)
/* блокировки Разделяемых ресурсов */
typedef struct {
 _contReg(TLock);
 TspinLock lock;
 } TLocksRes;
//#define secslocksResInit(App) RegInit(App.locksRes,CvmsumLockDef); spinLockInit(App.locksRes.lock)
//#define secslocksResFree(App) { TLock *pLock;RegIter(App.locksRes,pLock) {pLockFree(pLock);} }  RegFree(App.locksRes); spinLockFree(App.locksRes.lock)
 
#define  lenNameApp 3

typedef struct sttTApp {
 TarrJob arrJob; //список заданий
 TarrDo arrDo; //список блоков Do
 TsecsEvent secsEvent; //список пользовательских событий
 TsecsMod Mods; // загруженные модули
 TsecsLock Locks; //блокировки команды Lock
 TLocksRes locksRes; //блокировки Разделяемых ресурсов
 TsecsvmDb Glbs; //описание глобалей
 //
 TIni netDb; //список сетевых глобалей
 //
 TspinLock lockApp; // управление доступом к приложению
 TsecsFile arrFiles; //таблица файлов
 uint maxId; //Максимальный идентификатор
 char nameApp[lenNameApp+1]; //уникальный идентификатор приложения
// Tdiv TCP; //основной порт связи по TCP
// TfsrvWrite fsrvWrite; //программа обмена с сервером
 Tdata dataNULL; //пустое значение
 Tstrshr dlmIO; //разделитель записей потоке ввода вывода по умолчанию
 Tstr datanameNew; //имя конструктора
 Tstr sunname; //имя безымянного модуля
 char cbufErr[1024];
 } TApp;
 
TApp App; 

#ifdef __cplusplus
}
#endif

#endif /* MSHVM_H */

