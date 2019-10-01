/*
 * File:   mshvmInner.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:44
 */
#include "mshvmVm.h"
#include "mshvmMacro.h"
Terr mshPushArr(TDb *pDb,Tdata *pSetData);
void pArrTopData(TarrData *pArr, Tdata *pDataOut, Tarea *pArea);
void pArrTopData(TarrData *pArr, Tdata *pDataOut, Tarea *pArea);
void ListLongClear(TmapLong *pmLong);
void mshKillArrALL(TDb *pDb);
//TvmMod* getMod(Tstr *pname,Terr *perr);
void vmfaindLabelArg(TvmDo *pDo,TarrData *pArgs, TrefProg *prefProg);
int mshMtoVM(char *pcName,int *pnumstr,int *pnumpos,char *pbuferr);
void LockWR(TvmDo *pDo,Tstr *pnameIn,char type,int time);
int LockStatus(TvmDo *pDo,Tstr *pnameIn);
//
Terr castDataInt(Tdata *pData);
Tstr* castId(Tdata *pData);
//Terr castNorm(Tdata *pData);
//Terr pArrTopDataLoc(TDb *pDb,TarrData *pArr,Tdata *pData);
//Terr LongDelInd(Tlong *pLong,Tsize ind);
//Terr StrToDataOrLong(TmapLong *pmLong,Tdata *pData,Tbstr *pStrIn);
//void stkindstorefdb(TvmDo *pDo,TarrData *prefInd,TrefDb *pDbArr,int isSet);
//
//void vmQueryInitDb(TvmDo *pDo,TrefDb *pDbArr,void *pVoid);
//void vmQueryDb(TvmDo *pDo,TrefDb *pDbArr,void *pVoid);
void castbStr(Tdata *pData,Tbstr* pbstrOut);
//void bStrGet(Tdata *pData,Tbstr* pbstrOut);
//Tstr* castId(Tdata *pData);
//void PopToData(TvmDo *pDo,TCmd *pCmd,Tdata *pOut);
//void cmdExecDo(TvmDo *pDo,u_char flag,TdivStream *pdiv);

//void cmdReadFile(TvmDo *pDo,Tbstr *pbStr);
/*
  * Системные свойства Переменных
 *  встроенные свойства
 * %idJob, %idDo, %idDiv, %statDo, %isTr, %this, %statData
 *     Команда
 *    0. код команды = CvmCmdDO
 *    1. Sub команда = CvmSubCmdDOInProp
 *    2. Off3.  Id программы
 * Чтение
 *   результат поместить на индексный регистр
 * Запись
 *   индексный регистр содержит данные для записи
 */
//свойство по чтению
void vmInnerProp(TvmDo *pDo,TCmd *pCmd,Tdata *pOut){
 //
 pDataSetNULL(pOut);
 switch (pCmdArg(pCmd)){
  // --- код ошибки ---
  case CvmTypePropErr :
   pOut->intgr=pDo->pJob->errTr.err;
   pDataSetInt(pOut);
   break;
  //значение ключа узла к которому было последнее обращение в Итераторе
  case CvmTypePropqueryKey :
   *pOut=ValKey(pDo->pJob->valquery);
   break;
  //значение данных в узле к которому было последнее обращение в Итераторе
  case CvmTypePropqueryData :
   *pOut=ValData(pDo->pJob->valquery);
   break;
  //значение данных в узле к которому было последнее обращение в Get
  case CvmTypePropData :
   *pOut=ValData(pDo->pJob->val);
   break;
  // --- состояние вершины к которой было последнее обращение---
  case CvmTypePropStat :
   ValStat(pDo->pJob->val,pOut->intgr);
   pDataSetInt(pOut);
   break;
  //--- разделитель полей текущего устройства ---
  case CvmTypePropdelemIO :
   pDataSetStr(pOut);
   memCopy(&(pOut->str),&(pDo->pJob->dlmIO),StrSize(pDo->pJob->dlmIO));
   break;
  //имя текущего модуля 
  case CvmTypePropnameMod :
   pDataSetStr(pOut);
   pOut->str=pDo->pmod->name;
   break;
  //??--- идентификатор текущего устройства ---
  case CvmTypePropIdDiv :
   break;
  //??--- состояние транзакции ---
  case CvmTypePropIsTr :
   break;
  //--- идентификатор задания Job ---
  case CvmTypePropIdJob :
   pDataSetInt(pOut);
   pOut->intgr=IdJobGetDo(pDo);
   break;
  //--- идентификатор блока Do ---
  case CvmTypePropIdDo :
   pDataSetInt(pOut);
   pOut->intgr=IdDoGet(pDo);
   break;
  // --- статус выполнения блока Do ---
  case CvmTypePropStatDo :
   pDataSetStr(pOut);
   pOut->str.psec[0]=pDo->stat;
   pOut->str.size=1;
   break;
  //признак завершения итератора 
  case CvmTypePropEof :
   pOut->intgr=pDo->eof;
   pDataSetInt(pOut);
   break;
  }
 }
//свойство по записи
void vmInnerPropSet(TvmDo *pDo,TCmd *pCmd,Tdata *pSet){
 Tbstr bsSet;
 //    
 switch (pCmdArg(pCmd)){
  //--- разделитель полей текущего устройства ---
  case CvmTypePropdelemIO :
   pDo->pJob->dlmIO.size=0;
   pDo->pJob->dlmIO.psec[0]=Cvm0;
   if (pDataIsNULL(pSet)) return;
   castbStr(pSet,&bsSet);
   if (bsSet.size<1) return;
   if (bsSet.size>CCsizeStrShort) bsSet.size=CCsizeStrShort;
   memCopy(pDo->pJob->dlmIO.psec,bsSet.psec,bsSet.size);
   pDo->pJob->dlmIO.size=bsSet.size;
   break;
  }
 }
/*
 * Создание файловой директории
 */
#define ArgTobStr(pDo,pArgs,bStr) if ((pArgs)->busy) {\
 castbStr((pArgs)->psec,&bStr); bStr.psec[bStr.size]='\0';\
 if (bStr.psec==NULL) pDo->err=CERRvmNotArg;\
 } else pDo->err=CERRvmArgLittle
#define Arg1TobStr(pArgs,pArg,bStr) ArgTobStr(pArgs,bStr);pArg=(pArgs)->psec
#define getArg2(pArgs,pArg) if ((pArgs)->busy>1) pArg=(pArgs)->psec+1
/**/
static void faindLabel(TvmDo *pDo,TarrData *pArgs, Tdata *pdataOut) {
 TrefProg refProg;
 //
 pDataSetInt(pdataOut);
 vmfaindLabelArg(pDo,pArgs, &refProg);
 pdataOut->intgr=(pDo->err==COutKodOk)?1:0;
 }
//
static void CreateDir(TvmDo *pDo,TarrData *pArgs,Tdata *pOut){
 Tbstr bStr;
 //
 ArgTobStr(pDo,pArgs,bStr);
 if (pDo->err>CERR) {
  if (pOut) {pOut->intgr=divMakeDir(bStr.psec);} else {divMakeDir(bStr.psec);}
  }
 }
/*
 * Удаление файловой директории
 */
static void DeleteDir(TvmDo *pDo,TarrData *pArgs,Tdata *pOut){
 Tbstr bStr;
 //
 ArgTobStr(pDo,pArgs,bStr);
 if (pDo->err>CERR) {
  if (pOut) {pOut->intgr=divDeleteDir(bStr.psec);} else {divDeleteDir(bStr.psec);}
  }
 }
/*
 * Удаление файла
 */
static void DeleteFile(TvmDo *pDo,TarrData *pArgs,Tdata *pOut){
 Tbstr bStr;
 ArgTobStr(pDo,pArgs,bStr);
 if (pDo->err>CERR) {
  if (pOut) {pOut->intgr=divDeleteFile(bStr.psec);} else {divDeleteFile(bStr.psec);}
  }
 }
/*
 * Чтение файловой директории
 */
static struct TReadDir {
 TvmDo *pDo;
 Tdata Data;
 Toff Off;
 int isDir;
 } ReadDirData;

static int ReadDirSub(const char *pfilename,const struct stat *pstatus, int flag){
 Terr err;
 //не директория
 if (ReadDirData.isDir && ((flag==FTW_F)||(flag==FTW_NS)||(flag==FTW_F) ) ) return 0;
 //не файл
 if ( !(ReadDirData.isDir) && ((flag==FTW_D)||(flag==FTW_DNR)||(flag==FTW_DP) ) ) return 0;
 //
 ReadDirData.Data.bstr.size=strlen(pfilename);
 ReadDirData.Data.bstr.psec=(char*)pfilename;
 ++(ReadDirData.Off);
 return ((err=mshPushArr(&(ReadDirData.pDo->pJob->vmDbBuf.db),&(ReadDirData.Data)))<CERR)?err:0;
 }

static void ReadDir(TvmDo *pDo,TarrData *pArgs,int isDir,int isRecursiv,Tdata *pOut){
 Tbstr bStr;
 DIR *pDir;
 struct dirent *pFile;
 Tdata Data;
 Toff Off;
 //
 if ((pArgs==NULL)||(pArgs->busy<1)) {pDo->err=CERRvmNotArg;return;}
 //путь к директории
 castbStr(pArgs->psec,&bStr);
 if (bStr.psec==NULL) {pDo->err=CERRvmNotArg;return;}
 bStr.psec[bStr.size]='\0';
 //Очистить буфер
 ListLongClear(&(pDo->pJob->vmDbBuf.db.mlong));
 mshKillArrALL(&(pDo->pJob->vmDbBuf.db));
 if (isRecursiv){
  ReadDirData.pDo=pDo;
  ReadDirData.isDir=isDir;
  ReadDirData.Off=0;
  DataSetbStr(ReadDirData.Data);
  pDo->err=ftw(bStr.psec, ReadDirSub ,0);
  Off=(pDo->err>CERR)?ReadDirData.Off:pDo->err;
  }
 else
  if ((pDir=divOpenDir(bStr.psec))) {
  Off=0;
  DataSetbStr(Data);
  while((pFile=divReadDir(pDir))){
   if (pFile->d_name[0]!='.'){
    Data.bstr.size=strlen(pFile->d_name);
    Data.bstr.psec=pFile->d_name;
    if ( (isDir && (pFile->d_type & 0x4) )||(!isDir && (!(pFile->d_type & 0x4)) ) ){
     ++Off;
     if ((pDo->err=mshPushArr(&(pDo->pJob->vmDbBuf.db),&Data))<CERR) {
      Off=pDo->err;
      break;
      }
     }
    }
   }
  divCloseDir(pDir);
  }
 if (pOut) pOut->intgr=Off;
 }
/*
 * Компиляция модуля
 */
static void mshCompile(TvmDo *pDo,TarrData *pArgs,Tdata *pOut){
 Tbstr bStr;
 int numstr,numpos;
 //
 if ((pArgs==NULL)||(pArgs->busy<1)) {pDo->err=CERRvmNotArg;return;}
 castbStr(pArgs->psec,&bStr);
 if (bStr.psec==NULL) {pDo->err=CERRvmNotArg;return;}
 bStr.psec[bStr.size]='\0';
 pDo->Temps.psec[0]='\0';
 pDo->err= mshMtoVM(bStr.psec,&numstr,&numpos,NULL);
 if (pOut){
  memNULL(pOut);
  pDataSetInt(pOut);
  pOut->int32[0]=pDo->err;
  pOut->int32[1]=numstr;
  pOut->int32[2]=numpos;
  }
 }
/*
 * Компиляция модуля
 */
static void mshLock(TvmDo *pDo,TarrData *pArgs,int16_t type,Tdata *pOut){
 Tstr *psL;
 int tO=0;
 //
 if ((pArgs==NULL)||(pArgs->busy<1)) {pDo->err=CERRvmNotArg;return;}
 //имя блокировки
 psL=castId(pArgs->psec);
 if ((psL==NULL)||(psL->size<1)) {pDo->err=CERRvmNotArg;return;}
 if (pArgs->busy>1) {
  castDataInt(pArgs->psec+1);
  tO=pArgs->psec[1].intgr;
  }
 switch(type){
  //---LockR---
  case CvmTypeFunctLockR :
  //---LockW---
  case CvmTypeFunctLockW :
   LockWR(pDo,psL,type,tO);
   break;
  //---LockSt---
  case CvmTypeFunctLockSt :
   if (pOut) {
    pDataSetInt(pOut);
    pOut->intgr=LockStatus(pDo,psL);
    }
   break;
  }
 }
/*
 * внутренне Функции
 *  "LockR","LockW","LockSt","Open"
 *      Команда
 *    0. код команды = CvmCmdDO
 *    1. Sub команда = CvmSubCmdDOInFun
 *    2.  источник
 *    Off3.  Id программы
 */
void vmInnerFunct(TvmDo *pDo,TCmd *pCmd,TarrData *pArgs,Tdata *pOut){
 int Off;
 TJob *pJob;
 //
 pDo->err=COutKodOk;
 if (pOut){
  pDataSetInt(pOut);
  pOut->intgr=0;
  }
 switch(pCmdArg(pCmd)){
  //---findLabel---
  case CvmTypeFunctfindLabel:
   faindLabel(pDo,pArgs,pOut);
   break;
  //---LockR---
  case CvmTypeFunctLockR :
  //---LockW---
  case CvmTypeFunctLockW :
  //---LockSt---
  case CvmTypeFunctLockSt :
   mshLock(pDo,pArgs,pCmdArg(pCmd),pOut);
   break;
  //---CreateDir---
  case CvmTypeFunctCreateDir :
   CreateDir(pDo,pArgs,pOut);
   break;
  //---DeleteDir---
  case CvmTypeFunctDeleteDir :
   DeleteDir(pDo,pArgs,pOut);
   break;
  //---ReadDirDir---
  case CvmTypeFunctReadDir :
   ReadDir(pDo,pArgs,1,0,pOut);
   break;
  //---ReadDirFile---
  case CvmTypeFunctReadDirFile :
   ReadDir(pDo,pArgs,0,0,pOut);
   break;
  //---ReadDirR---
  case CvmTypeFunctReadDirR :
   ReadDir(pDo,pArgs,1,1,pOut);
   break;
  //---ReadDirFileR---
  case CvmTypeFunctReadDirFileR :
   ReadDir(pDo,pArgs,0,1,pOut);
   break;
  //---DeleteDir---
  case CvmTypeFunctDeleteFile :
   DeleteFile(pDo,pArgs,pOut);
   break;
  //---jobStatus---
  case CvmTypeFunctjobStatus :
   if (pArgs && pArgs->busy &&( (pDo->err=castDataInt(pArgs->psec))>CERR ) ){
    Off=pArgs->psec->intgr;
    if (Off && (Off< pDo->arrJob.busy) ){
     if ( (pJob=pAppgetpJob(Off)) )
      pOut->intgr=pJob->isbusy;
     }
    }
   break;
  //---jobStatus---
  case CvmTypeFunctjobErr :
   if (pArgs && pArgs->busy &&( (pDo->err=castDataInt(pArgs->psec))>CERR ) ){
    Off=pArgs->psec->intgr;
    if (Off && (Off< pDo->arrJob.busy) ){
     if ( (pJob=pAppgetpJob(Off)) )
      pOut->intgr=pJob->errTr.err;
     }
    }
   break;
  //---jobStatus---
  case CvmTypeFunctjobValue :
   pDataSetNULL(pOut);
   if (pArgs && pArgs->busy &&( (pDo->err=castDataInt(pArgs->psec))>CERR ) ){
    Off=pArgs->psec->intgr;
    if (Off && (Off< pDo->arrJob.busy) ){
     if ( (pJob=pAppgetpJob(Off)) )
      *pOut=ValData(pJob->val);
     }
    }
   break;
  //---Comp--- Компиляция файла
  case CvmTypeFunctCompile :
   mshCompile(pDo,pArgs,pOut);
   break;
  }
 }
/*
 *  состояние переменных
 *  "type", "size", "stat"
 */
void vmInnerStat(TvmDo *pDo,TCmd *pCmd,Tdata *pOut){
 char Type,*pC,*pE;
 Tbstr bstr;
 int j;
 //
 switch(pCmdArg(pCmd)){
  //свойства любых переменных
  // ---type---
  case CvmTypeStatType :
   Type=pDataSight(pOut);
   pDataSetStr(pOut);
   pOut->str.size=1;
   pOut->str.psec[0]=(pDataIsList(pOut))?CvmDataList:(Type==CDataNull)?CvmDataNull
     :(TypeIsFloat(Type))?CvmDataFloat:(TypeIsInt(Type))?CvmDataInt
     :(Type==CDataPointer)?CvmDataPointer:(Type==CDatanameDb)?CvmDatarefDb:(Type==CDataObj)?CvmDataObj: CvmDataStr;
   break;
  // ---stat---
  case CvmTypeStatStat :
   ValStat(pDo->pJob->val,pOut->intgr);
   pDataSetInt(pOut);
   break;
  // ---byte---
  case CvmTypeStatByte :
   castbStr(pOut,&bstr);
   pDataSetInt(pOut);
   pOut->intgr=bstr.size;
   break;
  // ---size---
  case CvmTypeStatSize :
   castbStr(pOut,&bstr);
   for (j=0,pC=bstr.psec,pE=pC+bstr.size;pC<pE;++j,trSmbNext(pC));
   pOut->intgr=j;
   pDataSetInt(pOut);
   break;
  }
 }
