/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*
 * File:   mshVmEvent.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:46
 */
#include "mshvmVm.h"
#include "mshvmCnst.h"

 int deleteFileFromDir(char *pDir);
 int deleteDir(char *pDir);
 void mshKillArrALL(TDb *pDb);
 Terr mshKillDbDALL(TDb *pDb);
 void ListLongClear(TmapLong *pmLong);
 void mshDbFree(TDb *pDb);
// int iniInitReadFL(char *pflname,TIni *pIni);
int iniDirInit(Tbstr *pbfl,const char *pDir,TIni *pIni);
 Terr mshDbLocNew(TDb *pDb,Tstr *pName,char *pDir,enum TtypeDb typeDb);
 TLock* LockResNew();

char *pCdirDb[3];
/*
 * События
 */
//
/* вспомогательные программы по обслуживанию curQuery */
static void vmcurQueryInit(TvmQuery *pQuery,void *pUser){
 pQueryInit(pQuery,NULL);
 refDbInit(pQuery->qrefDb);
 refDbInit(pQuery->qrefDbSave);
 areaInit(pQuery->areaStr,CvmsizeAreaTempsDef);
 pQuery->path=NULL;
 }
static void vmcurQueryFree(TvmQuery *pQuery,void *pUser){
 areaFree(pQuery->areaStr);
 refDbFree(pQuery->qrefDb);
 refDbFree(pQuery->qrefDbSave);
 pQueryFree(pQuery); 
 }
static void vmcurQueryClear(TvmQuery *pQuery,void *pUser){
 areaClear(pQuery->areaStr);
 pQueryClear(pQuery);
 refDbClear(pQuery->qrefDb);
 refDbClear(pQuery->qrefDbSave);
 pQuery->path=NULL;
 }
/*
void vmDbInit(TJob *pJob,TvmDb *pvmDb,Tstr *psName,enum TisLock dbisLock,enum TisGlb isGlb){
 //Инициализировать Глобаль
 pvmDb->isGlb=isGlb;
 pvmDb->name=*(psName);
 pvmDb->plock=((dbisLock==EisLock) && pJob)?LockResNew(pJob->pApp):NULL;
 }
*/
/*
 * инициализация блока Do
 int iD; //идентификатор блока
 TJob *pJob; //ссылка на свое задание
 // Регистры
 TarrData Args; //аргументы текущего блока Do
 TarrData iobjUp; //ссылка на объект
 TvmDb dbDo; //локальное дерево блока
 TBoxIterQuery curquery; //Регистр Итераторов
 TarrInt arrJob; //стек запущеных из блока заданий
 TarrData arInd; //текущий индекс блока Do
 TarrData refInd; //последняя ссылка при обращении к Данным
 Tarea Temps; //Область промежуточных данных
 TarrrefDb arrrefDb;
 //
 TrefDb refDb; //ссылка на последний индекс по которому было обращение Get, Set, Kill
 TarrData *piobj; //индекс объекта текущего блока Do
 TvmMod *pmod; // ссылка на описание модуля в App
 TflowCmd *pflowCmd;
 struct TvmsttDo *pDoParent;  //Блок Do который вызвал данный блок
 //
 TerrTr errTr;//код возврата
 Tstr nameDo; //имя блока
 char reserv[4]; //дополнительное место для продолжения строки
 uint isbusy; //свободно=0 или используется задание=1
 u_char flag; //флаги блока выполнения
 u_char stat; //статус выполнения Do
 */
/*
 int iJob; //идентифиватор задания
 struct sttTApp *pApp; //ссылка на приложение
 // Регистры
 TstkInd  inds; //индексный регистр
 TsecsvmDb secDbJob; //локальные деревья задания
 struct sttTtr exeTr; //блок трансляции
 TPacks bufpack; //буфер сериализации
 Tdiv TCP; //основной порт связи по TCP
 Tfile *pfile; //текущее устройство
 pthread_t idPth; //идентификатор потока
 pthread_cond_t waitEvent; //Управление ожиданием событий
 Tdata val; //возвращаемое значение
 uint isbusy; //свободно=0 или используется задание=1
 Terr err; //код возврата
 Tsize pachtsize;
 char pacht[CsizePacthFl];
 */

void TrInit(Ttr *pTr);

TJob* vmJobInit(){
 TJob *pJob;
 char fl[CsizePacthFl];
 Tstr Str; 
 //
 pRegBusyNull(&(App.arrJob));
 pJob=App.arrJob.ptop;
 objSetBusy(pJob);
 pJob->iD=App.arrJob.busy-1;
 pJob->divSrv.TCP=CvmpTCPNULL;
 pJob->fsrvWrite=NULL;
 stkIndInit(pJob->inds);
 arrInit(pJob->arrTemp,CvmIndsDef);
 secsvmDbInit(pJob->secDbJob);
 TrInit(&(pJob->exeTr));
 PackInit(pJob->bufpack);
// Stk2Init(pJob->stkthis,CvmThisDef,CvmThisct0Def);
 arrInit(pJob->thisObj,CvmIndsDef);
 pJob->pfile=NULL;
 pJob->idPth=CvmpThreadNULL;
 (void)condLockInit(pJob->waitEvent);
 DataSetNULL(pJob->val);
 snprintf(pJob->nameJob.psec,CsizePacthFl,"%04i%c",pJob->iD,CdelimFile);
 pJob->nameJob.size=strlen(pJob->nameJob.psec);
 getPachtDir(&(pJob->nameJob),pCdirDb[CidirDbLoc],fl);
 deleteFileFromDir(fl);
 memNULL(&(pJob->vmDbLong));
 (void)mshDbLocNew(&(pJob->vmDbLong.db),&(pJob->nameJob),pCdirDb[CidirDbLoc],EtypeLong);
 vmDbInit(pJob,pJob->vmDbLong,pJob->nameJob,EisLockNo,EisGlbNo);
 Str=pJob->nameJob;
 Str.psec[Str.size-1]='B';
 memNULL(&(pJob->vmDbBuf));
 (void)mshDbLocNew((TDb*)(&(pJob->vmDbBuf.db)),&Str,pCdirDb[CidirDbLoc],EtypeArr);
 vmDbInit(pJob,pJob->vmDbBuf,Str,EisLockNo,EisGlbNo);
 pJob->vmDbBuf.plock=NULL;
 //
 return pJob;
 }
void TrClear(Ttr *pTr);
void vmJobClear(TJob *pJob){
 char fl[CsizePacthFl];
 //
 objSetBusy(pJob);
 stkIndAllClear(pJob->inds);
 ListLongClear(&(pJob->vmDbLong.db.mlong));
 mshKillArrALL(&(pJob->vmDbBuf.db));
// Stk2AllClear(pJob->stkthis);
 arrClear(pJob->thisObj);
 secsvmDbClear(pJob->secDbJob);
 TrClear(&(pJob->exeTr));
 PackClear(pJob->bufpack);
 arrClear(pJob->arrTemp);
 DataSetNULL(pJob->val);
 pJob->pfile=NULL;
 pJob->idPth=CvmpThreadNULL;
 memNULL(&(pJob->errTr));
 memNULL(&(pJob->dlmIO));
 ValALLSetNULL(pJob->val);
 getPachtDir(&(pJob->nameJob),pCdirDb[CidirDbLoc],fl);
 deleteFileFromDir(fl);
 }
//
void TrFree(Ttr *pTr);
void vmJobFree(TJob *pJob){
 //
 stkIndFree(pJob->inds);
// Stk2Free(pJob->stkthis);
 arrFree(pJob->thisObj);
 vmDbFree(pJob->vmDbLong);
 vmDbFree(pJob->vmDbBuf);
 secsvmDbFree(pJob->secDbJob);
 arrFree(pJob->arrTemp);
 TrFree(&(pJob->exeTr));
 PackFree(pJob->bufpack);
 (void)condLockFree(pJob->waitEvent);
 }

TvmDo* vmDoInit(TJob *pJob){
 TvmDo *pDo;
 //
 pRegBusyNull(&(App.arrDo));
 pDo=App.arrDo.ptop;
 pDo->iD=App.arrDo.busy-1;
 pDo->pJob=pJob;
 //регистры
 refDbInit(pDo->refDb);
 arrInit(pDo->this,CvmIndsDef);
 vmBoxQueryInit(pDo->curquery);
 arrInit(pDo->arrJob,CvmsizeArrJobDef);
 arrInit(pDo->arArgs,CvmIndsDef);
 areaInit(pDo->Temps,CvmsizeAreaTempsDef);
 ValDataAdrSetNULL(pDo->pJob->valquery);
 //
 snprintf(pDo->nameDo.psec,CCsizeStr,"%s%04i",pJob->nameJob.psec,pDo->iD);
 pDo->nameDo.size=strlen(pDo->nameDo.psec);
 memNULL(&(pDo->dbDdo));
 (void)mshDbLocNew((TDb*)(&(pDo->dbDdo.db)),&(pDo->nameDo),pCdirDb[CidirDbLoc],EtypeDb);
 vmDbInit(pDo->pJob,pDo->dbDdo,pDo->nameDo,EisLockNo,EisGlbNo);
 memNULL(&(pDo->dbAdo));
 (void)mshDbLocNew((TDb*)(&(pDo->dbAdo.db)),&(pDo->nameDo),pCdirDb[CidirDbLoc],EtypeArr);
 vmDbInit(pDo->pJob,pDo->dbAdo,pDo->nameDo,EisLockNo,EisGlbNo);
 pDo->flag=CvmSubCmdDODo;
 pDo->stat=CvmStatDoWait;
 return pDo;
 }
/*
 */
void mshDbClear(TDb *pDb);

void vmDoClear(TvmDo *pDo){
 //регистры
 refDbClear(pDo->refDb);
 arrClear(pDo->this);
 (void)mshKillDbDALL(&(pDo->dbDdo.db));
 mshKillArrALL(&(pDo->dbAdo.db));
 areaClear(pDo->Temps);
 arrBoxClear(pDo->curquery);
 arrClear(pDo->arrJob);
 arrClear(pDo->arArgs);
 ValDataAdrSetNULL(pDo->pJob->valquery);
 //
 memNULL(&(pDo->rarInd));
 pDo->pmod=NULL;
 pDo->curOff=0;
 pDo->pDoParent=NULL;
 //
 memNULL(&(pDo->err));
 pDo->flag=CvmSubCmdDODo;
 pDo->stat=CvmStatDoWait;
 }

void vmDoFree(TvmDo *pDo){
 //
 refDbFree(pDo->refDb);
 arrFree(pDo->this);
 vmDbFree(pDo->dbDdo);
 vmDbFree(pDo->dbAdo);
 arrBoxFree(pDo->curquery);
 arrFree(pDo->arrJob);
 areaFree(pDo->Temps);
 arrFree(pDo->arArgs);
 }
/*
 TBoxJob boxJob; //список заданий
 TBoxRun boxRun; //список блоков Run
 TBoxDo boxDo; //список блоков Do
 TboxEvent boxEvent; //список пользовательских событий
 TsecsMod Mods; // загруженные модули
 TsecsLock Locks; //блокировки команды Lock
 TLocksRes locksRes; //блокировки Разделяемых ресурсов
 TsecsvmDb Glbs; //описание глобалей
 //
 TvmIni netDb; //список сетевых глобалей
 TvmDb dbApp; //локальное дерево приложения, используется в событиях
 //
 TspinLock lockApp; // управление доступом к приложению
 TsecsFile arrFiles; //таблица файлов
 uint maxId; //Максимальный идентификатор
 char nameApp[lenNameApp+1]; //уникальный идентификатор приложения
 Tdata dataNULL; //пустое значение
 Tstr sunname; //имя безымянного модуля
 */

int vmAppInit(char *pnameApp){
 char pname[]="mapGLB.ini";
 Terr err=COutKodOk;
 Tbstr bname;
 //
 memCopy(App.nameApp,pnameApp,lenNameApp);
 App.nameApp[lenNameApp]='\0';
 //блоки Job
// arrJobInit;
 RegInit(App.arrJob,CvmsizeboxAppDef);
 spinLockInit(App.arrJob.lock);
 //ссылки на блоки Do
 RegInit(App.arrDo,CvmsizeboxAppDef);
 spinLockInit(App.arrDo.lock);
 //блок управления событиями
// boxEventInit(App);
 RegInit(App.secsEvent,CvmsizeboxAppDef);
 spinLockInit(App.secsEvent.lock);
 //
// secsModInit(App);
 RegInit(App.Mods,CvmsumModDef);
 spinLockInit(App.Mods.lock);
 //Массив блокировок
// secsLocksInit(App);
 RegInit(App.Locks,CvmsumLockDef);
 spinLockInit(App.Locks.lock);
// secslocksResInit(App);
 RegInit(App.locksRes,CvmsumLockDef);
 spinLockInit(App.locksRes.lock);
 //таблица имен Lock
 //синхронизация обращений к приложению
 if (spinLockInit(App.lockApp)) return CERRvmSpinLockFiled;
 //Глобали Узла
 secsvmDbInit(App.Glbs);
 //Глобали Сети
 bname.psec=pname;
 bname.size=strlen(pname);
 err=iniDirInit(&bname,pCmshDirIni,&(App.netDb));
 // загруженные модули
 App.maxId=CminIdFile;
 RegInit(App.arrFiles,CvmsumFileDef);
 areaInit(App.arrFiles.names,CsizeFileNamesDef);
 App.sunname.psec[0]='u';
 App.sunname.psec[1]='n';
 App.sunname.psec[2]=Cvm0;
 App.sunname.size=2;
 DataSetNULL(App.dataNULL);
 memNULL(&(App.dlmIO));
 App.datanameNew.size=strlen(pConstructor);
 memCopy(App.datanameNew.psec,pConstructor,App.datanameNew.size+1);
 deleteDir(pCdirDb[CidirDbTmp]);
 return err;
 }
//
void vmAppFree(){
 TJob *pJob;
 TvmDo *pDo;
 TctrlLock *pctrlLock;
 TLock *pLock;
 //освободить блоки Job
 RegIter(App.arrJob,pJob) vmJobFree(pJob);
// arrJobFree;
 RegFree(App.arrJob);
 spinLockFree(App.arrJob.lock);
 //освободить блоки Do
 RegIter(App.arrDo,pDo) vmDoFree(pDo);
// arrDoFree;
 RegFree(App.arrDo);
 spinLockFree(App.arrDo.lock);
 //
// boxEventFree(App);
 RegFree(App.secsEvent);
 spinLockFree(App.secsEvent.lock);
 //освободить таблицу модулей
 while(App.Mods.ptop) {
  //удаляем только саму голову, секции не были распределены
  arrFree(App.Mods.ptop->labels);
  pvmModFree(App.Mods.ptop);
  pRegPop( &(App.Mods) );
  }
// secsModFree(App);
 RegFree(App.Mods);
 spinLockFree(App.Mods.lock);
 //таблица имен Lock
// secsLocksFree(App);
 RegIter(App.Locks,pctrlLock) {pLockFree(&(pctrlLock->lock));}
 RegFree(App.Locks);
 spinLockFree(App.Locks.lock);

 //
// secslocksResFree(App);
// #define secslocksResFree(App)
 RegIter(App.locksRes,pLock) {pLockFree(pLock);}
 RegFree(App.locksRes);
 spinLockFree(App.locksRes.lock);

 spinLockFree(App.lockApp);
 secsvmDbFree(App.Glbs);
 //освободить Ini файл
 iniFree(App.netDb);
 areaFree(App.arrFiles.names);
 RegFree(App.arrFiles);
 }
