/*
 * File:   mshVmCmd.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:41
 */
#include "../../mshDb/H/mshPgLong.h"

#include "mshvmVm.h"
#include "mshvmMacro.h"

extern struct sttTarrextFunc *parrextFunc;

//--------------
Terr mshFileReadToMap(char *pfl,TmapFixed *pMap);

void bStrGet(Tdata *pData,Tbstr* pbstrOut);
int vmRoundDb(TvmDb *pvmDb, TarrData *pInds,int isAlt, int (*pfdata)(TtreeData *ptreeData) , void *pUser);

Terr mshgetDb(TDb *pDbA,Tstr *pName,const char *pDir,enum TisSet dbisSet,int sizePartGlb,enum TtypeDb typeDb);
Tdata* mshGetArr(TDb *pDb,Tusize Off,Terr *perr);
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
Terr mshSetArr(TDb *pDb,Toff Off,Tdata *pSetData);
Terr mshPushArr(TDb *pDb,Tdata *pSetData);
void mapArrSetSize(TDb *pDb,Toff Off);
Terr mshKill(TDb *pDb,TarrData *pInds,char tpK);
Terr mshKillDbDALL(TDb *pDb);
Terr mshKillArr(TDb *pDb,Tusize Off);
void mshKillArrALL(TDb *pDb);
Terr mshQueryInit(TIterQuery *pQuery);
Terr tstfindNodeRoot(TIterNext *pIterNext,TarrData *pInds);
Terr mshNextInit(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey);
Terr mshQuery(TIterQuery *pQuery);
Terr mshNext(TIterNext *pIterNext);
Terr mshBack(TIterNext *pIterNext);
Terr mshQuery1(TIterQuery *pQuery);
Terr mshNext1(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey);
Terr mshArrNextBack(TIterNext *pNext);
Terr mshArrNextBackInit(TIterNext *pNext);
Terr mshArrQuery(TIterNext *pNext);
Terr mshArrQueryInit(TIterNext *pNext);
Terr mshArrNextBack1(TIterNext *pNext);
void expandDataToArr(Tdata *pD,TarrData* parTemp);
void pArrTopData(TarrData *pArr, Tdata *pDataOut, Tarea *pArea);
void expandArrToArr(TarrData* parIn,TarrData* parTemp);
void pdataLongRef(TmapLong *pmLong,Tdata *pData);
void pListdataLongRef(TmapLong *pmLong,TarrData *pArr);
void pValLongRef(TmapLong *pmLong,Tval *pVal);
void TrToMod(Ttr *pTr,TvmMod *pMod);

Terr castDataInt(Tdata *pData);
Terr castNorm(Tdata *pData);
Tstr* castId(Tdata *pData);
void castbStr(Tdata *pData,Tbstr *pbstrOut);
void pArrToArea(TarrData *pArr,Tarea *pArea);
void expandDataToArr(Tdata *pD,TarrData* parTemp);

TvmMod* getMod(Tstr *pname,Terr *perr);
Tsize getLabelSecMod(TvmMod *pMod,Tstr *psLabel,Tsize iSec);
Terr getLabelApp(TrefProg *prefProg);
void pDatasetRefLong(TmapLong *pmLong,Tdata *pDataSet);
Terr LongDelInd(Tlong *pLong,Tsize ind);
Terr StrToDataOrLong(TDb *pDb,Tdata *pData,Tbstr *pStrIn);
void pDataKillLongArr(Tdata *pDataIn);
void vmInnerFunct(TvmDo *pDo,TCmd *pCmd,TarrData *pArgs,Tdata *pOut);
void vmInnerStatSet(TvmDo *pDo,TCmd *pCmd,Tdata *pSet);
void vmInnerStat(TvmDo *pDo,TCmd *pCmd,Tdata *pOut);
Terr vmInnerMetodSet(TvmDo *pDo, TCmd *pCmd, TarrData *pArgs);
Terr vmInnerMetod(TvmDo *pDo, TCmd *pCmd, TarrData *pArgs,Tdata *pdataOut);
Terr vmInnerWin(TvmDo *pDo,TCmd *pCmd,TarrData *pArgs,Tdata *pOut);
//
TvmDo* vmGetDoflag(TvmDo *pDoParent,u_char flag);
//Terr pArrTopDataLoc(TmapLong *pmLong,TarrData *pArr,Tdata *pData);
//void vmDbInit(TJob *pJob,TvmDb *pvmDb,Tstr *psName,enum TisLock dbisLock,enum TisGlb isGlb);

void pArrToLong(TmapLong *pmLong,TarrData *pArr);
void ListLongClear(TmapLong *pmLong);
Tbstr* iniKeygetVal(TIni *pIni,Tbstr *pbsKey);
//--------------

TLock* LockResNew();
void TrRef(TvmDo *pDo,Ttr *pTr,Tdata *pX);
void mshVmDo(TvmDo *pDo, TdivStream *pdiv);
void EventTrapName(TvmDo *pDo,Tstr *psName);
void vmInnerProp(TvmDo *pDo,TCmd *pCmd,Tdata *pOut);
void vmInnerPropSet(TvmDo *pDo,TCmd *pCmd,Tdata *pSet);

void moveTohis(TmapLong *pmLong,Tdata *pDataSet);

//void pArrsetRefLong(TmapLong *pmLong,TarrData *parrData);

char *pCdirDb[3]={"dbl","dbg","dbt"};

/*
 * модифицировать имя модуля префиксом для вызова функции слева от =
 */
static void mshVmLabUpdate(Tstr *psLab,const char* pRef) {
 int sizePref;
 //
 sizePref=strlen(pRef);
 if ((psLab->size + sizePref) > CsizeStr) psLab->size=CsizeStr-sizePref;
 memMove(psLab->psec + sizePref,psLab->psec,psLab->size);
 memCopy(psLab->psec,pRef,sizePref);
 psLab->size +=sizePref;
 }
/*
 * Найти TvmDb в списке TsecsvmDb
 */
static TvmDb* vmDbFind(TsecsvmDb *psecDb,Tstr *psName,enum TtypeDb typeDb){
 TvmDb *pvmDb;
 //
 switch (typeDb) {
  case EtypeDb :
   //Ищем глобаль в списке определенных
   RegIter(psecDb->regDb,pvmDb)
    //глобаль найдена
    if (StrpEqu(pvmDb->name,psName)==CMPeq) return pvmDb;
   break;   
  case EtypeArr :
   //Ищем глобаль в списке определенных
   RegIter(psecDb->regArr,pvmDb)
    //глобаль найдена
    if (StrpEqu(pvmDb->name,psName)==CMPeq) return pvmDb;
   break;   
  case EtypeLong :
   break;
  default :
   return NULL;
  }  
 //глобаль НЕ найдена
 return NULL;
 }
/*
*/
TvmDb* vmDbGetSet(TvmDo *pDo,TsecsvmDb *psecDb,Tstr *psName,const char *pDir,enum TisSet dbisSet,enum TisLock dbisLock,int sizePart,enum TtypeDb typeDb,enum TisGlb isGlb,Terr *perr){
 TvmDb *pvmDb=NULL;
 //Ищем глобаль в списке определенных
 spinLock(psecDb->lock);
 if ((pvmDb=vmDbFind(psecDb,psName,typeDb))) {
  spinUnLock(psecDb->lock);
  return pvmDb;
  }
 //создаем глобаль на текущем узле pCdirDb[]
 switch (typeDb) {
  case EtypeDb :
   pRegBusyNull(&(psecDb->regDb));
   pvmDb=psecDb->regDb.ptop;
   *perr=mshgetDb(&(pvmDb->db),psName,pDir,dbisSet,sizePart,typeDb);
   if (*perr!=COutKodOk) {
    RegPop(psecDb->regDb);
    spinUnLock(psecDb->lock);
    return NULL;
    }
   break;   
  case EtypeArr :
   pRegBusyNull(&(psecDb->regArr));
   pvmDb=psecDb->regArr.ptop;
   *perr=mshgetDb(&(pvmDb->db),psName,pDir,dbisSet,sizePart,typeDb);
   if (*perr!=COutKodOk) {
    RegPop(psecDb->regArr);
    spinUnLock(psecDb->lock);
    return NULL;
    }
   break;   
  case EtypeLong :
   return NULL;
  default :
   return NULL;
  }
 if (pvmDb){
  //Инициализировать Глобаль
  pvmDbInit(pDo->pJob,pvmDb,psName,dbisLock,isGlb);
  spinUnLock(psecDb->lock);
  }
 return pvmDb;
 }
/*
 * 
 */
void stkindstorefdb(TvmDo *pDo,TarrData *prefInd,TrefDb *pDbArr,enum TisSet dbisSet){
 Tdata *pdataName;
 Tbstr sbName;
 Tsize Size;
 Terr isPsevdo;
 //Terr err;
 enum TtypeDb typeDb=EtypeDb;
 pDo->err=COutKodOk;
 //-----------------
 prefDbClear(pDbArr);
 pDbArr->style=CstyleDbNone;
 pDbArr->pvmDb=NULL;
 if (prefInd->busy<1) {pDo->err=CERRvmIndFiled;return;}
 parrCopy( &(pDbArr->fullInds),prefInd );
 pdataName=pDbArr->fullInds.psec;
 if (pdataName->str.size >= CsizeStr) pdataName->str.size=CsizeStr-1;
 pdataName->str.psec[pdataName->str.size]='\0';
 //имеем дело с массивом
 if (pDatastyleIsIndArr(pdataName)){
  isPsevdo=0;
  typeDb=EtypeArr;
  if (pdataName->str.size==1){
   isPsevdo=1;
   switch(pdataName->str.psec[0]){
    case CvmPsevdoArg :
     pDbArr->style=CstyleDbArg;
     break;
    case CvmPsevdoJob :
     pDbArr->style=CstyleDbArgJob;
     break;
    // Буфер
    case CvmPsevdoBuf :
     pDbArr->pvmDb=&(pDo->pJob->vmDbBuf);
     pDbArr->style=CstyleDbArr;
     break;
    default :
     isPsevdo=0;
     break;
    }
   }
  if (prefInd->busy<2) 
   pDo->err=CWARvmDbIndsEmpty;
  else {
   if ((pDo->err=castDataInt(parrOffpData(prefInd,1)))<CERR ) return;
   pDbArr->off=parrOffD(prefInd,1).intgr;
   }
  if (isPsevdo) return;
  pDbArr->style=CstyleDbArr;
  }
 //Дерево данных
 else{
  pDbArr->style=CstyleDbDb;
  if (pDbArr->fullInds.busy<2) {
   pDbArr->rInds.psec=NULL;
   pDbArr->rInds.size=1;
   pDbArr->rInds.busy=0;
   pDo->err=CWARvmDbIndsEmpty;
   }
  else{
   pDbArr->rInds.psec=arrOffpD(pDbArr->fullInds,1);
   pDbArr->rInds.size=pDbArr->rInds.busy=pDbArr->fullInds.busy-1;
   arrsetpTop(pDbArr->rInds);
   }
  }
 //Находим где хранятся данные
 //находим дерево данных pDb
 //данные без имени уровня блока Do
 if (pdataName->str.size==0){
  pDbArr->pvmDb=(typeDb==EtypeArr)?&(pDo->dbAdo):((TvmDb*)(&(pDo->dbDdo)));
  return;
  }
 //временная глобаль
 if (pdataName->str.psec[0]==CtrTypeDbTemp){
  if ((pDbArr->pvmDb=vmDbGetSet(pDo,&(App.Glbs),&(pdataName->str),pCdirDb[CidirDbTmp]
     ,dbisSet,EisLock,CsizePartGlbDef,typeDb,EisGlbNo,&(pDo->err)))==NULL){
//   if (err<CERR) pDo->err=err;
   return;
   }
  pDbArr->style |= CstyleDbGlb;
  return;
  }
 //глобали
 if (pdataName->str.psec[0]==CtrTypeDbGlb){
  //должно быть обязательно имя
  //if (pdataName->str.size<2) {pDo->err=CERRvmDbNameFiled;return;}
  if (pdataName->str.size<1) {pDo->err=CERRvmDbNameFiled;return;}
  //внешняя глобаль
  StrTobStr(pdataName->str,sbName);
  if ((pDbArr->pPath=iniKeygetVal(&(App.netDb), &sbName ) )) {
   pDbArr->style |= CstyleDbNet;
   return;
   }
  if ((pDbArr->pvmDb=vmDbGetSet(pDo,&(App.Glbs),&(pdataName->str),pCdirDb[CidirDbGlb]
          ,dbisSet,EisLock,CsizePartGlbDef,typeDb,EisGlb,&(pDo->err)))==NULL){
//   if (err<CERR) pDo->err=err;
   return;
   }
  pDbArr->style |= CstyleDbGlb;
  return;
  }
 //данные уровня задания
 Size=pDo->pJob->nameJob.size;
 memCopy(pDo->pJob->nameJob.psec+pDo->pJob->nameJob.size,pdataName->str.psec,pdataName->str.size);
 pDo->pJob->nameJob.size +=pdataName->str.size;
 pDo->pJob->nameJob.psec[pDo->pJob->nameJob.size] ='\0';
 pDbArr->pvmDb=vmDbGetSet(pDo,&(pDo->pJob->secDbJob),&(pDo->pJob->nameJob),pCdirDb[CidirDbLoc]
    ,dbisSet,EisLockNo,0,typeDb,EisGlbNo,&(pDo->err) );
// if (err<CERR) pDo->err=err;
 pDo->pJob->nameJob.size=Size;
 pDo->pJob->nameJob.psec[Size]='\0';
 }
//
void StkIndsTorefDb(TvmDo *pDo,TrefDb *pDbArr,enum TisSet dbisSet) {
 //извлечь из стека индекс в pDo->rarInd уровень стека понизить
 IndDoToArr(pDo,pDo->rarInd)
 if (pDo->rarInd.busy==0){
  pDo->err=CERRvmIndFiled;
  }
 else{
  stkindstorefdb(pDo,&(pDo->rarInd),pDbArr,dbisSet);
  }
 }
//
//    Извлечь данные из источника
//
static void RegToData(TvmDo *pDo,TCmd *pCmd,Tdata *pOut){
 int n;
 //источник
 pDataSetNULL(pOut);
 switch(regIstCmd(pCmd)){
  //команда
  case CregComChar :
   pDataSetStr(pOut);
   pOut->str.size=0;
   n=3;
   if (pCmd->c1[n]){
    pOut->str.size=2;
    memCopy(pOut->str.psec,pCmd->c2+1,pOut->str.size);
    break;
    }
   n--;
   if (pCmd->c1[n]){
    pOut->str.psec[0]=pCmd->c1[n];
    pOut->str.size=1;
    }
   break;
  case CregCom :
   pDataSetInt(pOut);
   pOut->intgr=pCmdArg(pCmd);
   break;
  //индекс хранится в модуле
  case CregMod :
   *pOut=pDo->pmod->pdata[pCmdArg(pCmd)];
   break;
   }
 }
//
void PopToData(TvmDo *pDo,TCmd *pCmd,Tdata *pOut){
 Tdata *pD;
 pDataSetNULL(pOut);
 switch(regIstCmd(pCmd)){
  //команда
  case CregComChar :
  case CregCom :
  //индекс хранится в модуле
  case CregMod :
   RegToData(pDo,pCmd,pOut);
   break;
  //индекс хранится на индексном регистре
  default :
   pD=stkIndTopData(pDo);
   if (pD) *pOut=*pD;
   break;
  }
 stkIndLev0Pop(pDo);
 }
//
static int RefToData(TvmDo *pDo,TCmd *pCmd,Tdata *pOut){
 Tdata *pTop;
 int isData=0;
 //источник
 pDataSetNULL(pOut);
 switch(regIstCmd(pCmd)){
  //команда
  case CregComChar :
  case CregCom :
  //индекс хранится в модуле
  case CregMod :
   RegToData(pDo,pCmd,pOut);
   isData=1;
   break;
  //индекс хранится на индексном регистре
  case CregInd :
   stkIndPopTop(pDo,pTop)
   if (pTop) *pOut=*pTop;
   isData=1;
   break;
  case CregNULL :
   isData=1;
   break;
  }
 return isData;
 }
//
static void IndToData(TvmDo *pDo,TCmd *pCmd,Tdata *pOut){
 //источник
 pDataSetNULL(pOut);
 switch(regIstCmd(pCmd)){
  //команда
  case CregComChar :
  case CregCom :
  //индекс хранится в модуле
  case CregMod :
   RegToData(pDo,pCmd,pOut);
   break;
  //индекс хранится на регистре данных 1-е поле имя дерева, остальные поля индекс
  case CregInd :
   IndDoToArr(pDo,pDo->rarInd)
   pArrTopData(&(pDo->rarInd), pOut, &(pDo->Temps));
   /*
   if (pDo->rarInd.busy>1){
    if (pDo->rarInd.busy==2){
     *pOut=*(pDo->rarInd.ptop);
     break;
     }
    TarrData Ar;
    Ar.psec=(Tdata*)(pDo->rarInd.psec)+1;
    Ar.busy=pDo->rarInd.busy-1;
    Ar.ptop=(Tdata*)(&(pDo->rarInd.ptop));
    pArrTopData(&Ar, pOut, &(pDo->Temps));
    }
   */
   break;
  }
 }

/*
 *   ---- команды обращения к данным ----
 *    0. код команды
 *    1. Sub команда для Kill что удалять
 *    2.   Индекс
 *    3.   Данные
 */
/* enum TisSet dbisSet
 * Выполнитьь обращение к базе по pDbArr
 */
void vmInOuTDbbArr(TvmDo *pDo,TrefDb *pDbArr,TCmd *pCmd,void *pVoid ,enum TisSet dbisSet
    ,void (*pfDb)(TvmDo *pDo,TrefDb *pDbArr,void *pVoid)
    ,void (*pfArr)(TvmDo *pDo,TrefDb *pDbArr,void *pVoid)
    ,void (*pfArg)(TvmDo *pDo,TrefDb *pDbArr,void *pVoid)
    ,void (*pfNet)(TvmDo *pDo,TrefDb *pDbArr,TCmd *pCmd,void *pVoid) ) {
 //
 if (typepRegIsArg(pDbArr)) {if (pfArg) pfArg(pDo,pDbArr,pVoid);return;}
 if (pDbArr->pvmDb==NULL) {pDo->err=CERRvmDbFiled;return;}
 if (typepRegIsNet(pDbArr)) {if (pfNet) pfNet(pDo,pDbArr,pCmd,pVoid);return;}
 if (pDbArr->pvmDb->plock){
  if (dbisSet==EisSet){ pLockW(pDbArr->pvmDb->plock); } else{ pLockR(pDbArr->pvmDb->plock); }
  }
 if ( typepRegIsInd(pDbArr) ){
  if (pfDb) pfDb(pDo,pDbArr,pVoid);
  }
 else
  if (typepRegIsArr(pDbArr)){
   if (pfArr) pfArr(pDo,pDbArr,pVoid);
   }
 if (pDbArr->pvmDb->plock){unpLock(pDbArr->pvmDb->plock);}
 }
/*
*/
//#define CmdToDataPop(pDo,pCmd,pdSet) CmdToData(pDo,pCmd,pdSet); stkIndLev0Pop(pDo)

/*
 * получить данные из дерева
 */
static void vmGetDb(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 Tval *pVal;
 //
 arrClear(pDo->pJob->arrTemp);
 expandArrToArr(&(pDbArr->rInds),&(pDo->pJob->arrTemp) );
 pVal=mshGet((TDb*)(&(pDbArr->pvmDb->db)),&(pDo->pJob->arrTemp),&(pDo->err) );
 if (pVal) {
  pDo->pJob->val=*pVal;
  if (pDbArr->pvmDb->isglb)
   pValLongRef(&(pDbArr->pvmDb->db.mlong),&(pDo->pJob->val));
   }
 else{
  DataSetNULL(pDo->pJob->val);
  }
 }

static void vmGetArg(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 //
 DataSetInt(ValKey(pDo->pJob->val));
 ValKey(pDo->pJob->val).intgr=pDbArr->off;
 DataSetNULL(ValData(pDo->pJob->val));
 //массив номеров заданий
 switch(pDbArr->style){
  case CstyleDbArgJob :
   if (pDbArr->off){
    if (pDbArr->off<=pDo->arrJob.busy){
     DataSetInt(ValData(pDo->pJob->val));
     ValData(pDo->pJob->val).intgr=pDo->arrJob.psec[pDbArr->off-1];
     }
    }
   else{
    DataSetInt(ValData(pDo->pJob->val));
    ValData(pDo->pJob->val).intgr=pDo->arrJob.busy;
    }
   break;
  //Аргументы блока pDo
  case CstyleDbArg :
   if (pDbArr->off){
    if (pDo->arArgs.busy){
     if (pDbArr->off==CvmArrOffNull){
      ValData(pDo->pJob->val)=*(pDo->arArgs.ptop);
      pDo->arArgs.busy--;
      arrsetpTop(pDo->arArgs);
      }
     else
      if (pDbArr->off<=pDo->arArgs.busy){
       ValData(pDo->pJob->val)=pDo->arArgs.psec[pDbArr->off-1];
       }
     }
    }
   else{
    DataSetInt(ValData(pDo->pJob->val));
    ValData(pDo->pJob->val).intgr=pDo->arArgs.busy;
    }
   break;
  } 
 }
/*
 * получить данные из массива
 */
static void vmGetArr(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 Tdata *pData;
 int isPop=0;
 //размер массива
 DataSetInt(ValKey(pDo->pJob->val));
 if (pDbArr->off==0){
  ValKey(pDo->pJob->val).intgr=pDbArr->off;
  ValData(pDo->pJob->val).intgr= maparrDbBusy(pDbArr->pvmDb->db);
  DataSetInt(ValData(pDo->pJob->val));
  return;
  }
 //массив пуст
 if ((maparrDbBusy(pDbArr->pvmDb->db))==0) return;
 //вытолкнуть из стека последнее значение
 if (pDbArr->off==CvmArrOffNull){
  isPop=1;
  pDbArr->off= maparrDbBusy(pDbArr->pvmDb->db);
  }
 pData=mshGetArr(&(pDbArr->pvmDb->db),pDbArr->off ,&(pDo->err));
 if (pData){
  ValData(pDo->pJob->val)=*pData;
  ValKey(pDo->pJob->val).intgr=pDbArr->off;
  if (pDbArr->pvmDb->isglb) pdataLongRef( &(pDbArr->pvmDb->db.mlong),&(ValData(pDo->pJob->val)) );
  }
 else{ValALLSetNULL(pDo->pJob->val);}
 if (isPop){
  maparrDbBusy(pDbArr->pvmDb->db)=pDbArr->off-1;
  }
 }
/*
 * получить данные из сети
 */
Tval* NetGetStat(TvmDo *pDo,TarrData *parrInd,TCmd *pCmd,Tbstr *path);

static void vmGetNet(TvmDo *pDo,TrefDb *pDbArr,TCmd *pCmd,void *pVoid){
 Tval *pVal;
 //
 pVal=NetGetStat(pDo,&(pDbArr->fullInds),pCmd,pDbArr->pPath);
 if (pVal) {
  pDo->pJob->val=*pVal;
  pValLongRef(&(pDbArr->pvmDb->db.mlong),&(pDo->pJob->val));
  }
 }
/*
 * получить данные
 *    0. код команды Get
 *    1. 0
 *    2. 0
 * индекс находится на индексном регистре
 */
void vmGet(TvmDo *pDo,TCmd *pCmd) {
// Tdata *pD;
 //
 DosetVal(pDo);
 StkIndsTorefDb(pDo,&(pDo->refDb),EisSetNo);
 //запомнить индекс последнего обращения
 pDataTypeSub(pDo->this.psec)=pDo->refDb.style;
 parrCopy(&(pDo->this),&(pDo->refDb.fullInds));
 //данные могут быть найдены
 if (pDo->err==COutKodOk) {
  vmInOuTDbbArr(pDo,&(pDo->refDb),pCmd,NULL,EisSetNo,vmGetDb,vmGetArr,vmGetArg,vmGetNet);
  }
 else {
  DataSetNULL(ValData(pDo->pJob->val));
  }
 //результат поместить на индексный регистр
 stkIndPushData(pDo,ValData(pDo->pJob->val));
 }
/*
*/
/**/
void vmGetData(TvmDo *pDo,TrefDb *pDbArr){
 TCmd Cmd;
 //
 if ( pDbArr->pvmDb==NULL) { pDo->err=CWARdataNotFound; return;}
 memNULL(&Cmd);
 CmdCmd(Cmd)=CvmCmdGet;
 DosetVal(pDo);
 vmInOuTDbbArr(pDo,pDbArr,&Cmd,NULL,EisSetNo,vmGetDb,vmGetArr,vmGetArg,vmGetNet);
 }
/*
 *    0. код команды Set
 *    1. данные для записи
 *    2. 0
 * на индексном регистре сначала индекс затем данные на запись
 */
static void vmSetDb(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 //Развернуть индекс
 arrClear(pDo->pJob->arrTemp);
 expandArrToArr(&(pDbArr->rInds),&(pDo->pJob->arrTemp) );
 //записать данные
 pDo->err=mshSet((TDb*)(&(pDbArr->pvmDb->db)),&(pDo->pJob->arrTemp),(Tdata*)pVoid);
 }
static void vmSetArr(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 if (pDbArr->off==CvmArrOffNull) pDo->err=mshPushArr(&(pDbArr->pvmDb->db),(Tdata*)pVoid);
 else pDo->err=mshSetArr(&(pDbArr->pvmDb->db),pDbArr->off,(Tdata*)pVoid);
 }
static void vmSetArg(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 Tdata *pData=(Tdata*)pVoid;
 //
 if (pDbArr->style!=CstyleDbArg) return;
 //Push
 if (pDbArr->off==CvmArrOffNull){
  arrBusy(pDo->arArgs);
  *(pDo->arArgs.ptop)=*pData;
  }
 else{
  if (pDbArr->off==0){
   pDo->err=castDataInt(pData);
   if (pDo->err<CERR) return;
   if (pDo->arArgs.busy<pData->intgr) pDo->arArgs.busy=pData->intgr;
   }
  else
   if (pDo->arArgs.busy<pDbArr->off) {
    arrWideSize(pDo->arArgs,pDbArr->off);
    pDo->arArgs.busy=pDbArr->off;
    }
   pDo->arArgs.psec[pDbArr->off-1]=*((Tdata*)pVoid);
   }
 }
//*/
void NetSet(TvmDo *pDo,TarrData *parrInd,TCmd *pCmd,Tbstr *path,Tdata *pData);

static void vmSetNet(TvmDo *pDo,TrefDb *pDbArr,TCmd *pCmd,void *pVoid){
 NetSet(pDo,&(pDbArr->fullInds),pCmd,pDbArr->pPath,(Tdata*)pVoid);
 }
/**/
//Terr vmSeTDbrr(TvmDo *pDo,Tdata *pData,TCmd *pCmd) { return vmInOuTDbbArr(pDo,&(pDo->refDb),pCmd,pData,vmSetDb,vmSetArr,NULL,vmSetNet); }
/**/
void vmSet(TvmDo *pDo,TCmd *pCmd,int isInd) {
 Tdata dSet;
 //получить данные на запись
 PopToData(pDo,pCmd,&dSet);
// if (pDo->err==CWARvmDbIndsEmpty) pDo->err=CERRvmIndEmpty;
 if (pDo->err<CERR) return;
 if (isInd){
  StkIndsTorefDb(pDo,&(pDo->refDb),EisSet);
  if ((pDo->err<CERR)||(pDo->err==CWARvmDbIndsEmpty)) return;
  }
 vmInOuTDbbArr(pDo,&(pDo->refDb),pCmd,&dSet,EisSet,vmSetDb,vmSetArr,vmSetArg,vmSetNet);
 }
/**/
void vmSetData(TvmDo *pDo,TrefDb *pDbArr,Tdata *pdSet){
 TCmd Cmd;
 //
 if (typepRegIsArg(pDbArr)) {
  vmSetArg(pDo,&(pDo->refDb),pdSet);
  return;
  }
 if ( pDbArr->pvmDb==NULL) { pDo->err=CERRDbNotCreat; return;}
 memNULL(&Cmd);
 CmdCmd(Cmd)=CvmCmdSet;
 vmInOuTDbbArr(pDo,pDbArr,&Cmd,pdSet,EisSet,vmSetDb,vmSetArr,vmSetArg,vmSetNet);
 }
/*
 */
static void vmKillDb(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TCmd *pCmd=pVoid;
 //
 arrClear(pDo->pJob->arrTemp);
 expandArrToArr(&(pDbArr->rInds),&(pDo->pJob->arrTemp) );
 pDo->err=mshKill(&(pDbArr->pvmDb->db),&(pDo->pJob->arrTemp),pCmdSub(pCmd) );
 }
//??
static void vmKillArr(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 pDo->err=mshKillArr(&(pDbArr->pvmDb->db),pDbArr->off);
 }
//??
static void vmKillArg(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 if (pDbArr->style!=CstyleDbArg) return;
 if ((pDo->arArgs.busy>pDbArr->off)&&(pDbArr->off)) {
  memMove(pDo->arArgs.psec+pDbArr->off-1,pDo->arArgs.psec+pDbArr->off,pDo->arArgs.busy-pDbArr->off);
  pDo->arArgs.busy--;
  }
 else
  if (pDo->arArgs.busy==pDbArr->off)
   pDo->arArgs.busy--;
 }

void NetKill(TvmDo *pDo,TarrData *parrInd,TCmd *pCmd,Tbstr *path);

static void vmKillNet(TvmDo *pDo,TrefDb *pDbArr,TCmd *pCmd,void *pVoid){
 NetKill(pDo,&(pDbArr->fullInds),pCmd,pDbArr->pPath);
 }
/*
 *    0. код команды Kill
 *    1. что удалять данные или потомков
 *    2. 0
 * на индексном регистре индекс
 */
void vmKill(TvmDo *pDo,TCmd *pCmd) {
 StkIndsTorefDb(pDo,&(pDo->refDb),EisSetNo);
 if ( !((pDo->err==COutKodOk)||(pDo->err==CWARvmDbIndsEmpty)) ) return;
 //удалить и массив и дерево данных с таким именем
 if ((pCmdSub(pCmd))==CtypeKiLLAll) {
  //удалить дерево данных
  (void)mshKillDbDALL( (TDb*)(&((pDo->refDb.pvmDb->db))) );
  //найти массив с таким же именем
  // оставить одно имя
  pDo->rarInd.busy=1;
  arrtop(pDo->rarInd);
  pDatastyleSetIndArr(pDo->rarInd.psec);
  stkindstorefdb(pDo,&(pDo->rarInd),&(pDo->refDb),EisSetNo);
  //удалить массив
  if (( pDo->err>CERR )&&(pDo->refDb.pvmDb))
   mshKillArrALL( &((pDo->refDb.pvmDb->db)) );
  }
 else
  vmInOuTDbbArr(pDo,&(pDo->refDb),pCmd,pCmd,EisSet,vmKillDb,vmKillArr,vmKillArg,vmKillNet);
 }

void vmKillData(TvmDo *pDo,TrefDb *pDbArr,u_char subCmd){
 TCmd Cmd;
 //
 if ( pDbArr->pvmDb==NULL) { pDo->err=CWARdataNotFound; return;}
 memNULL(&Cmd);
 CmdCmd(Cmd)=CvmCmdKiLL;
 CmdSub(Cmd)=subCmd;
 vmInOuTDbbArr(pDo,pDbArr,&Cmd,&Cmd,EisSet,vmKillDb,vmKillArr,vmKillArg,vmKillNet);
 if (pDo->err<CERR) pDo->err=CWARglbnotFound;
 }
/*
 *         Итераторы
 */
static void vmQuerySave(TvmDo *pDo,TvmQuery *pQuery){
 if (typeRegIsNoNone(pQuery->qrefDbSave))
  vmSetData(pDo,&(pQuery->qrefDbSave),&(ValKey(pQuery->iterNext.val)));
 }
//
#define KeyValToaddInd(pQuery) arrBusy1(pQuery->addInd); *(pQuery->addInd.psec)=ValKey(pQuery->iterNext.val);\
  if (pDbArr->pvmDb->isglb) pValLongRef(&(pDbArr->pvmDb->db.mlong),&(pQuery->iterNext.val))

#define ValToIndRefLong if (pQuery->addInd.busy>1){\
   if (pDbArr->pvmDb->isglb) pListdataLongRef(&(pDbArr->pvmDb->db.mlong),&(pQuery->addInd));\
   pArrTopData(&(pQuery->addInd) , &(ValKey(pQuery->iterNext.val)), &(pDo->Temps));  }\
  else if (pDbArr->pvmDb->isglb) pValLongRef(&(pDbArr->pvmDb->db.mlong),&(pQuery->iterNext.val))
/*
 * Инициализация Итератора
 */
void vmQueryInitDb(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TvmQuery *pQuery;
 //
 pQuery=(TvmQuery*)pVoid;
 arrClear(pQuery->rootInd);
 arrClear(pDo->pJob->arrTemp);
 if (pDbArr->rInds.busy) {
  expandArrToArr(&(pDbArr->rInds),&(pDo->pJob->arrTemp) );
  parrCopy(&(pQuery->rootInd),&(pDo->pJob->arrTemp));
  }
 if (pQuery->iterNext.type==CcmdQuery) {
  arrClear(pQuery->addInd);
  if (DataIsNoNULL(pQuery->Key)) expandDataToArr(&(pQuery->Key),&(pQuery->addInd));
  pQuery->iterNext.AdrOfferr.err=mshQueryInit((TIterQuery*)pQuery);
  if ( (pQuery->iterNext.AdrOfferr.err==CMPnull)||(pQuery->iterNext.AdrOfferr.err<CERR) ) return;
  ValToIndRefLong;
  }
 else {
  pQuery->iterNext.AdrOfferr.err=mshNextInit((TIterNext*)pQuery,&(pDo->pJob->arrTemp),&(pQuery->Key));
  if ( (pQuery->iterNext.AdrOfferr.err==CMPnull)||(pQuery->iterNext.AdrOfferr.err<CERR) ) return;
  KeyValToaddInd(pQuery);
  }
 }
//
static void vmQueryArr(TvmDo *pDo,TrefDb *pDbArr,void *pVoid);

static void vmQueryInitArr(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TvmQuery *pQuery;
 //
 pQuery=(TvmQuery*)pVoid;
 if (DataIsNoNULL(pQuery->Key)) {
  castDataInt(&(pQuery->Key));
  pQuery->qrefDb.off=pQuery->iterNext.AdrOfferr.off=pQuery->Key.intgr;
  return vmQueryArr(pDo,pDbArr,pVoid);
  }
 //
 pQuery->iterNext.AdrOfferr.off=1;
 if (pQuery->iterNext.type==CcmdQuery)
  pQuery->iterNext.AdrOfferr.err=mshArrQueryInit((TIterNext*)pQuery);
 else
  pQuery->iterNext.AdrOfferr.err=mshArrNextBackInit((TIterNext*)pQuery);
 if ( (pQuery->iterNext.AdrOfferr.err==CMPnull)||(pQuery->iterNext.AdrOfferr.err<CERR) ) return;
 KeyValToaddInd(pQuery);
 }
/**/

static void vmQueryinitArg(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TarrData *pArr;
 TvmQuery *pQuery;
 //
 pQuery=(TvmQuery*)pVoid;
 if (DataIsNoNULL(pQuery->Key)) {
  castDataInt(&(pQuery->Key));
  pQuery->qrefDb.off=pQuery->Key.intgr;
  }
 pQuery->iterNext.AdrOfferr.off=pQuery->qrefDb.off;
 if (pQuery->qrefDb.style==CstyleDbArgJob){
  if (pDo->arrJob.busy==0) {pQuery->iterNext.AdrOfferr.err=CMPnull;return;}
  if (pQuery->iterNext.type==CcmdBack){
   if ((pQuery->qrefDb.off < 1)||(pQuery->qrefDb.off >= pDo->arrJob.busy)) {
    pQuery->iterNext.AdrOfferr.off=pDo->arrJob.busy-1;
    }
   }
  else{
   if (pQuery->qrefDb.off >= pDo->arrJob.busy) {pQuery->iterNext.AdrOfferr.err=CMPnull;return;}
   if (pQuery->qrefDb.off < 1) pQuery->iterNext.AdrOfferr.off=0;
   }
  DataSetInt(ValData(pQuery->iterNext.val));
  ValData(pQuery->iterNext.val).intgr=arrOffD(pDo->arrJob,pQuery->iterNext.AdrOfferr.off);
  }
 else{
  if (pDo->arArgs.busy==0) {pQuery->iterNext.AdrOfferr.err=CMPnull;return;}
  pArr=&(pDo->arArgs);
  if (pQuery->iterNext.type==CcmdBack){
   if ((pDbArr->off < 1)||(pDbArr->off >= pArr->busy)) {
    pQuery->iterNext.AdrOfferr.off=pArr->busy-1;
    }
   }
  else{
   if (pDbArr->off >= pArr->busy) {pQuery->iterNext.AdrOfferr.err=CMPnull;return;}
   if (pDbArr->off < 1) pQuery->iterNext.AdrOfferr.off=0;
   }
  ValData(pQuery->iterNext.val)=parrOffD(pArr,pQuery->iterNext.AdrOfferr.off);
  }
 ValKey(pQuery->iterNext.val).intgr=pQuery->iterNext.AdrOfferr.off;
 DataSetInt(ValKey(pQuery->iterNext.val));
 KeyValToaddInd(pQuery);
 pQuery->iterNext.AdrOfferr.err=COutKodOk;
 }

void NetNextQuery(TvmDo *pDo,TvmQuery *pQuery,TCmd *pCmd);

static void vmQueryinitNet(TvmDo *pDo,TrefDb *pDbArr,TCmd *pCmd,void *pVoid){
 TvmQuery *pQuery;
 //
 pQuery=(TvmQuery*)pVoid;
 NetNextQuery(pDo,pQuery,pCmd);
 pValLongRef(&(pDbArr->pvmDb->db.mlong),&(pQuery->iterNext.val));
 }
/*
 * Итераторы
 *
 *   ---- команды Next и Query ----
 *  инициализация
 *   -инициализация и первая итерация цикла
 *    0. код команды CcmdQuery, CcmdNext, CcmdBack
 *    1. Sub команда CvmCmdSubInit | надо ли сохранять индекс
 *    2. Arg - переход если итерация завершена
 *
 *   продолжение Итераций командой
 *    0. код команды CcmdQuery, CcmdNext, CcmdBack
 *    1. Sub команда 0
 *    2. Arg - переход если итерация продолжена
 */
void vmQueryInit(TvmDo *pDo,TCmd *pCmd){
 TvmQuery *pQuery;
 //
 pQuery=pDo->curquery.ptop;
 DataSetNULL(pQuery->Key);
 ValALLSetNULL(pQuery->iterNext.val);
 pQuery->iterNext.AdrOfferr.err=COutKodOk;
 //индекс сохранения
 if ((pCmdSub(pCmd)) & CvmCmdSubSaveInd){
  StkIndsTorefDb(pDo,&(pQuery->qrefDbSave),EisSetNo);
  if ( pQuery->qrefDbSave.pvmDb==NULL) { pDo->err=CERRDbNotCreat; return;}
  if (pDo->err<CERR) {
   pQuery->iterNext.AdrOfferr.err=pDo->err;
   return;
   }
  vmGetData(pDo,&(pQuery->qrefDbSave));
  pQuery->Key=ValData(pDo->pJob->val);
  //перенести длинные строки в свою область
  pArrToArea(&(pQuery->qrefDbSave.fullInds),&(pQuery->areaStr));
  }
 //снимаем заглушку
 stkIndLev0Pop(pDo);
 StkIndsTorefDb(pDo,&(pQuery->qrefDb),EisSetNo);
 //Данные не обнаружены
 if (pQuery->qrefDb.pvmDb==NULL){
  pQuery->iterNext.AdrOfferr.err=CMPnull;
  pDo->err=CWARdataNotFound;
  return;
  }
 if (pDo->err<CERR) {
  pQuery->iterNext.AdrOfferr.err=pDo->err;
  return;
  }
 //перенести длинные строки в свою область
 pArrToArea(&(pQuery->qrefDb.fullInds),&(pQuery->areaStr));
 IterNextInit(pQuery->iterNext,&(pQuery->qrefDb.pvmDb->db),pCmdCmd(pCmd));
 pQuery->path=pQuery->qrefDb.pPath;
 vmInOuTDbbArr(pDo,&(pQuery->qrefDb),pCmd,pQuery,EisSetNo,vmQueryInitDb,vmQueryInitArr,vmQueryinitArg,vmQueryinitNet);
 //сохранить данные
 if (pQuery->iterNext.AdrOfferr.err>CERR) vmQuerySave(pDo,pQuery);
 pDo->pJob->valquery=pQuery->iterNext.val;
 }
/*
 */
void vmQueryDb(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TvmQuery *pQuery;
 //
 pQuery=(TvmQuery*)pVoid;
 if (pQuery->iterNext.type==CcmdQuery) {
  pQuery->iterNext.AdrOfferr.err=mshQuery((TIterQuery*)pQuery);
  if ( (pQuery->iterNext.AdrOfferr.err==CMPnull)||(pQuery->iterNext.AdrOfferr.err<CERR) ) return;
  ValToIndRefLong;
  }
 else {
  pQuery->iterNext.AdrOfferr.err=mshNextBack((TIterNext*)pQuery);
  if ( (pQuery->iterNext.AdrOfferr.err==CMPnull)||(pQuery->iterNext.AdrOfferr.err<CERR) ) return;
  KeyValToaddInd(pQuery);
  }
 //сформировать индекс объекта
 // parrIndCopy(&(pDo->iobjUp),pDbArr->pInd);
 }
static void vmQueryArr(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TvmQuery *pQuery;
 pQuery=(TvmQuery*)pVoid;
 if (pQuery->iterNext.type==CcmdQuery)
  pQuery->iterNext.AdrOfferr.err=mshArrQuery((TIterNext*)pQuery);
 else
  pQuery->iterNext.AdrOfferr.err=mshArrNextBack((TIterNext*)pQuery);
 if ( (pQuery->iterNext.AdrOfferr.err==CMPnull)||(pQuery->iterNext.AdrOfferr.err<CERR) ) return;
 KeyValToaddInd(pQuery);
 }

static void vmQueryArg(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TvmQuery *pQuery;
 TarrData *pArr;
 //
 pQuery=(TvmQuery*)pVoid;
 pQuery->iterNext.AdrOfferr.err=COutKodOk;
 //
 DataSetInt(ValKey(pQuery->iterNext.val));
 if (pQuery->iterNext.type==CcmdBack){
  if (pQuery->iterNext.AdrOfferr.off<1) {pQuery->iterNext.AdrOfferr.err=CMPnull;return;}
  pQuery->iterNext.AdrOfferr.off--;
  }
 else{
  if (pDbArr->style==CstyleDbArgJob){
   if (pQuery->iterNext.AdrOfferr.off >= (pDo->arrJob.busy-1)) {pQuery->iterNext.AdrOfferr.err=CMPnull;return;}
   }
  else {
   pArr=&(pDo->arArgs);
   if (pArr->busy==0) {pQuery->iterNext.AdrOfferr.err=CMPnull;return;}
   if (pQuery->iterNext.AdrOfferr.off >= (pArr->busy-1)) {pQuery->iterNext.AdrOfferr.err=CMPnull;return;}
   }
  pQuery->iterNext.AdrOfferr.off++;
  }
 if (pDbArr->style==CstyleDbArgJob){
  DataSetInt(ValData(pQuery->iterNext.val));
  ValData(pQuery->iterNext.val).intgr=arrOffD(pDo->arrJob,pQuery->iterNext.AdrOfferr.off);
  }
 else{
  ValData(pQuery->iterNext.val)=parrOffD(pArr,pQuery->iterNext.AdrOfferr.off);
  }
 DataSetInt(ValKey(pQuery->iterNext.val));
 ValKey(pQuery->iterNext.val).intgr=pQuery->iterNext.AdrOfferr.off;
 KeyValToaddInd(pQuery);
 }

static void vmQueryNet(TvmDo *pDo,TrefDb *pDbArr,TCmd *pCmd,void *pVoid){
 TvmQuery *pQuery;
 //
 pQuery=(TvmQuery*)pVoid;
 NetNextQuery(pDo,pQuery,pCmd);
 pValLongRef(&(pDbArr->pvmDb->db.mlong),&(pQuery->iterNext.val));
 }

void vmQuery(TvmDo *pDo,TCmd *pCmd){
 TvmQuery *pQuery;
 //
 pQuery=pDo->curquery.ptop;
 pQuery->iterNext.AdrOfferr.err=COutKodOk;
 vmInOuTDbbArr(pDo,&(pQuery->qrefDb),pCmd,pQuery,EisSetNo,vmQueryDb,vmQueryArr,vmQueryArg,vmQueryNet);
 //сохранить данные
 if (pQuery->iterNext.AdrOfferr.err>CERR) vmQuerySave(pDo,pQuery);
 pDo->pJob->valquery=pQuery->iterNext.val;
 }
/*
 * Инициализация Итератора
 */
static void vmQuery1Db(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TvmQuery *pQuery;
 //
 pQuery=(TvmQuery*)pVoid;
 if (pDbArr->rInds.busy) {
  arrClear(pDo->pJob->arrTemp);
  expandArrToArr(&(pDbArr->rInds),&(pDo->pJob->arrTemp) );
  parrCopy(&(pQuery->rootInd),&(pDo->pJob->arrTemp));
  }
 if (pQuery->iterNext.type==CcmdQuery) {
  arrClear(pQuery->addInd);
  if (DataIsNoNULL(pQuery->Key)) expandDataToArr(&(pQuery->Key),&(pQuery->addInd));
  pQuery->iterNext.AdrOfferr.err=mshQuery1((TIterQuery*)pQuery);
  if ( (pQuery->iterNext.AdrOfferr.err==CMPnull)||(pQuery->iterNext.AdrOfferr.err<CERR) ) return;
  ValToIndRefLong;
  }
 else {
  pQuery->iterNext.AdrOfferr.err = mshNext1( &(pQuery->iterNext) ,&(pQuery->rootInd) ,&(pQuery->Key) );
  if ( (pQuery->iterNext.AdrOfferr.err==CMPnull)||(pQuery->iterNext.AdrOfferr.err<CERR) ) return;
  KeyValToaddInd(pQuery);
  }
 // Глобаль все ссылки должны быть уже откорректированы
// if (pDbArr->pvmDb->plock) pValLongRef(&(pDbArr->pvmDb->db.mlong),&(pQuery->iterNext.val));
 }
/**/
static void vmQuery1Arr(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TvmQuery *pQuery;
 //
 pQuery=(TvmQuery*)pVoid;
 if (DataIsNULL(pQuery->Key)) goto init;
 pQuery->iterNext.AdrOfferr.err=castDataInt(&(pQuery->Key));
 if (pQuery->iterNext.AdrOfferr.err<CERR) return;
 pQuery->iterNext.AdrOfferr.off=pQuery->Key.intgr;
 if (pQuery->Key.intgr<1) goto init;
 if (pQuery->iterNext.type==CcmdQuery)
  pQuery->iterNext.AdrOfferr.err=mshArrQuery((TIterNext*)pQuery);
 else
  pQuery->iterNext.AdrOfferr.err=mshArrNextBack((TIterNext*)pQuery);
 goto end;
init: 
 if (pQuery->iterNext.type==CcmdQuery)
  pQuery->iterNext.AdrOfferr.err=mshArrQueryInit((TIterNext*)pQuery);
 else
  pQuery->iterNext.AdrOfferr.err=mshArrNextBackInit((TIterNext*)pQuery);
end:
 if ( (pQuery->iterNext.AdrOfferr.err==CMPnull)||(pQuery->iterNext.AdrOfferr.err<CERR) ) return;
 pQuery->Key.intgr=pQuery->iterNext.AdrOfferr.off;
 KeyValToaddInd(pQuery);
 }
/**/
static void vmQuery1Arg(TvmDo *pDo,TrefDb *pDbArr,void *pVoid){
 TvmQuery *pQuery;
 //
 pQuery=(TvmQuery*)pVoid;
 if ((pQuery->iterNext.AdrOfferr.err=castDataInt(&(pQuery->Key)))>CERR){
  pQuery->iterNext.AdrOfferr.off=pQuery->Key.intgr;
  vmQueryArg(pDo,pDbArr,pVoid);
  }
 }
/**/
static void vmQuery1Net(TvmDo *pDo,TrefDb *pDbArr,TCmd *pCmd,void *pVoid){
 TvmQuery *pQuery;
 pQuery=(TvmQuery*)pVoid;
 NetNextQuery(pDo,(TvmQuery*)pVoid,pCmd);
 pValLongRef(&(pDbArr->pvmDb->db.mlong),&(pQuery->iterNext.val));
 }
/**/
void vmQuery1(TvmDo *pDo,TCmd *pCmd){
 TvmQuery *pQuery;
 //нет индекса сохранения
 pQuery=pDo->curquery.ptop;
 DataSetNULL(pQuery->Key);
 ValALLSetNULL(pQuery->iterNext.val);
 if ( pCmdCmdSubNextNoSave(pCmd) ) {pQuery->iterNext.AdrOfferr.err=CERRvmIndSaveFiled;return;}
 pQuery->iterNext.AdrOfferr.err=COutKodOk;
 //индекс сохранения
 StkIndsTorefDb(pDo,&(pQuery->qrefDbSave),EisSet);
 pQuery->iterNext.AdrOfferr.err=pDo->err;
 if ( pQuery->qrefDbSave.pvmDb==NULL) { pDo->err=CERRDbNotCreat; return;}
 if (pDo->err<CERR) {
 //снимаем заглушку
  stkIndLev0Pop(pDo);
  return;
  }
 vmGetData(pDo,&(pQuery->qrefDbSave));
 pQuery->Key=ValData(pDo->pJob->val);
 //снимаем заглушку
 stkIndLev0Pop(pDo);
 //опорный индекс
 StkIndsTorefDb(pDo,&(pQuery->qrefDb),EisSetNo);
 pQuery->iterNext.AdrOfferr.err=pDo->err;
 if (pQuery->qrefDb.pvmDb==NULL){
  pQuery->iterNext.AdrOfferr.err=CMPnull;
  pDo->err=CWARdataNotFound;
  return;
  }
 if (pDo->err<CERR) return;
 IterNextInit(pQuery->iterNext,&(pQuery->qrefDb.pvmDb->db),pCmdCmd(pCmd));
 pQuery->path=pQuery->qrefDb.pPath;
 vmInOuTDbbArr(pDo,&(pQuery->qrefDb),pCmd,pQuery,EisSetNo,vmQuery1Db,vmQuery1Arr,vmQuery1Arg,vmQuery1Net);
 //сохранить данные
 if (pQuery->iterNext.AdrOfferr.err>CERR){
  vmQuerySave(pDo,pQuery);
  }
 pDo->pJob->valquery=pQuery->iterNext.val;
 return;
 }
/*
 * Копирование узлов
 *  Индекс узела приемника лежит под Индексом узла источника
 * ??Глобали не могут быть сетевыми ??
 */
typedef struct {
 TvmDo *pDo;
 TrefDb DbArr;
 } TpDoDbArr;
/*
 * pDoDbArr->DbArr.parr=pDoDbArr->pindTo это присвоение в StyleInd
 */
static int vmCopySub(TtreeData *ptreeData){
 TrounDb *proundDb;
 TpDoDbArr *pDoDbArr;
 int err=COutKodOk;
 //
 if (ptreeData->pkeydata==NULL) return err;
 proundDb=(TrounDb*)(ptreeData->pUser);
 pDoDbArr=(TpDoDbArr*)(proundDb->pUser);
 arrClear(pDoDbArr->pDo->pJob->arrTemp);
 if (pDoDbArr->DbArr.rInds.busy){
  parrCopy(&(pDoDbArr->pDo->pJob->arrTemp),&(pDoDbArr->DbArr.rInds));
  if (ptreeData->Inds.busy){
   arrExpand( pDoDbArr->pDo->pJob->arrTemp,ptreeData->Inds );
   }
  }
 else{
  if (ptreeData->Inds.busy<1) return err;
  parrCopy(&(pDoDbArr->pDo->pJob->arrTemp),&(ptreeData->Inds));
  }
 err=mshSet((TDb*)(&(pDoDbArr->DbArr.pvmDb->db)),&(pDoDbArr->pDo->pJob->arrTemp),&(pValData(ptreeData->pkeydata)));
 return err;
 }
/*
 *    0. код команды
 *    1. Sub команда копирование или пересылка
 *        0 - копирование Copy
 *        1 - удалить приемник перед копированием
 *        2 - удалить источник после копирования
 *        3 - удалить приемник перед копированием, удалить источник после копирования Move
 *    2.  0
 * на индексном регистре сначала ссылка на Источник затем ссылка на Приемник
 */

void vmCopy(TvmDo *pDo,TCmd *pCmd){
 TpDoDbArr DoDbArr;
 TrefDb DbArrTo;
 Toff OffEnd;
 Tdata *pDataSet,*pKey,dataSet;
 Toff sizeFrom,sizeTo,Off,iTo;
 //найти источник
 StkIndsTorefDb(pDo,&(pDo->refDb),EisSetNo);
 if (pDo->err<CERR) return;
 if ( typeRegIsArr(pDo->refDb) ) {
  if (pDo->err!=CWARvmDbIndsEmpty) {
   pDo->err=CERRvmIndNotEmpty;
   return;
   }
  if (( typeRegIsNoArgJob(pDo->refDb) )&&(pDo->refDb.pvmDb==NULL)){
   pDo->err=CWARdataNotFound;
   return;
   }
  }
 else
  if (pDo->refDb.pvmDb==NULL){
   pDo->err=CWARdataNotFound;
   return;
   }
 //найти приемник
 refDbInit(DbArrTo);
 StkIndsTorefDb(pDo,&DbArrTo,EisSet);
 if (pDo->err<CERR) goto end;
 if (DbArrTo.pvmDb==NULL){
  pDo->err=CERRDbNotCreat;
  goto end;
  }
 if  ( typeRegIsArr(DbArrTo) ) {
  if  (pDo->err!=CWARvmDbIndsEmpty) {
   pDo->err=CERRvmIndNotEmpty;
   goto end;
   }
  //ошибка если источник не массив, приемник псевдо массив, не задана ссылка на данные
  if (( (typeRegIsArr(pDo->refDb))==0 )||(typeRegIsNoArr(DbArrTo))||(DbArrTo.pvmDb==NULL) ) {
   pDo->err=CERRrefDbNo;
   goto end;
   }
  }
 //снять уровень начала параметров Copy
// stkIndLev0Pop(pDo);
 //удалить приемник
 if (pCmdSub(pCmd) & CvmSubCmdCopyDelTo) {
  vmKillData(pDo,&DbArrTo,CtypeKiLL);
  if (pDo->err<CERR) goto end;
  }
 //
 pDo->err=COutKodOk;
 if (DbArrTo.pvmDb->plock){pLockW(DbArrTo.pvmDb->plock);}
 //копирование в массив
 if (typeRegIsArr(DbArrTo)) {
  //
  sizeTo=maparrDbBusy(DbArrTo.pvmDb->db);
  if (typeRegIsarrJob(pDo->refDb)){
   if ((sizeFrom=pDo->arrJob.busy)<1) goto finish;
   //установить размер массива получателя
   OffEnd=sizeFrom + sizeTo;
   mapArrSetSize(&(DbArrTo.pvmDb->db),OffEnd);
   maparrDbBusy(DbArrTo.pvmDb->db)=OffEnd;
   for (Off=sizeTo + 1,iTo=0;Off<=OffEnd;++Off,++iTo){
    pDataSet=mapArrpDataOff(&(DbArrTo.pvmDb->db),Off);
    pDataSetInt(pDataSet);
    pDataSet->intgr=pDo->arrJob.psec[iTo];
    }
   goto finish;
   }
  //источник Аргументы
  if (typeRegIsArg(pDo->refDb)) {
   //Id заданий запущенных из этого блока pDo
   //источник Аргументы pDo
   if ((sizeFrom=pDo->arArgs.busy)<1) goto finish;
   //установить размер массива получателя
   OffEnd=sizeFrom + sizeTo;
   mapArrSetSize(&(DbArrTo.pvmDb->db),OffEnd);
   maparrDbBusy(DbArrTo.pvmDb->db)=OffEnd;
   for (Off=sizeTo + 1,iTo=0;Off<=OffEnd;++Off,++iTo){
    pDataSet=mapArrpDataOff(&(DbArrTo.pvmDb->db),Off);
    *pDataSet=pDo->arArgs.psec[iTo];
    moveTohis(&(DbArrTo.pvmDb->db.mlong),pDataSet);
    }
   goto finish;
   }
  //источник массивы
  if ( (pDo->refDb.pvmDb==NULL)||((sizeFrom=maparrDbBusy(pDo->refDb.pvmDb->db))<1) ) goto finish;
  if (pDo->refDb.pvmDb->plock){pLockR(pDo->refDb.pvmDb->plock);}
  //установить размер массива получателя
  OffEnd=sizeFrom + sizeTo;
  mapArrSetSize(&(DbArrTo.pvmDb->db),OffEnd);
  maparrDbBusy(DbArrTo.pvmDb->db)=OffEnd;
  for (Off=sizeTo,iTo=0;Off<=OffEnd;++Off,++iTo){
   pDataSet=mapArrpDataOff(&(DbArrTo.pvmDb->db),Off);
   *pDataSet=mapArrDataOff(&(pDo->refDb.pvmDb->db),iTo);
   moveTohis(&(DbArrTo.pvmDb->db.mlong),pDataSet);
   }
  goto kill;
  } 
 //копирование в дерево
// parrCopy(&(pDo->pJob->arrTemp),&(DbArrTo.rInds)); DbArrTo.rInds=pDo->pJob->arrTemp;
 //копирование из массива
 if (typeRegIsArr(pDo->refDb)) {
  //подготавливаем дополнительное место в индексе
  arrWide(DbArrTo.fullInds)
  //Накладываем на него псевдо массив
  DbArrTo.rInds=DbArrTo.fullInds;
  //начало сдвигаем с имени на индекс
  DbArrTo.rInds.psec=DbArrTo.fullInds.psec+1;
  //ключ устанавливаем на расширенное место
  pKey=DbArrTo.rInds.ptop=DbArrTo.fullInds.ptop+1;
  pDataSetInt(pKey);
  //источник псевдо массив
  if (typeRegIsArg(pDo->refDb)) {
   //Id заданий запущенных из этого блока pDo
   if (typeRegIsarrJob(pDo->refDb)){
    if ((sizeFrom=pDo->arrJob.busy)<1) goto finish;
    DataSetInt(dataSet);
    for (iTo=0;iTo<=sizeFrom;++iTo){
     pKey->intgr=iTo;
     dataSet.intgr=pDo->arrJob.psec[iTo];
     if (DataIsNoNULL(dataSet)) (void)mshSet((TDb*)(&(DbArrTo.pvmDb->db)),&(DbArrTo.rInds),&dataSet);
     }
    goto finish;
    }
   //источник Аргументы pDo
   if ((sizeFrom=pDo->arArgs.busy)<1) goto finish;
   for (iTo=0;iTo<=sizeFrom;++iTo){
    pKey->intgr=iTo;
    dataSet=pDo->arArgs.psec[iTo];
    if (DataIsNoNULL(dataSet)) {
     moveTohis(&(DbArrTo.pvmDb->db.mlong),&dataSet);
     (void)mshSet((TDb*)(&(DbArrTo.pvmDb->db)),&(DbArrTo.rInds),&dataSet);
     }
    }
   goto finish;
   }
  //источник массивы
  if ( (pDo->refDb.pvmDb==NULL)||((sizeFrom=maparrDbBusy(pDo->refDb.pvmDb->db))<1) ) goto finish;
  if (pDo->refDb.pvmDb->plock){pLockR(pDo->refDb.pvmDb->plock);}
  for (iTo=0;iTo<=sizeFrom;++iTo){
   pKey->intgr=iTo;
   dataSet=mapArrDataOff(&(pDo->refDb.pvmDb->db),iTo);
   if (DataIsNoNULL(dataSet)) {
    moveTohis(&(DbArrTo.pvmDb->db.mlong),&dataSet);
    (void)mshSet((TDb*)(&(DbArrTo.pvmDb->db)),&(DbArrTo.rInds),&dataSet);
    }
   }
  goto kill;
  }
 //копирование из дерева в дерево
 if (pDo->refDb.pvmDb==NULL) goto finish;
 if (pDo->refDb.pvmDb->plock){pLockR(pDo->refDb.pvmDb->plock);}
 DoDbArr.pDo=pDo;
 DoDbArr.DbArr=DbArrTo;
 //скопировать опорную вершину
 if ( (DbArrTo.rInds.busy) && (pDo->refDb.rInds.busy) ){
  //ссылки источника корректируются при чтении, а приемник переписывает данные к себе с исправленными ссылками
  vmGetData( pDo, &(pDo->refDb) );
  if (DataIsNoNULL(ValData(pDo->pJob->val))){
   vmSetData(pDo, &DbArrTo ,&(ValData(pDo->pJob->val)) );
   }
  }
 pDo->err=vmRoundDb((TvmDb*)(pDo->refDb.pvmDb),&(pDo->refDb.rInds),EroundAlt, vmCopySub , &DoDbArr);
kill: 
 if ( ( pDo->err > CERR )&&(pCmdSub(pCmd) & CvmSubCmdCopyDelFrom) ) vmKillData(pDo,&(pDo->refDb),CtypeKiLL);
 if (pDo->refDb.pvmDb->plock){unpLock(pDo->refDb.pvmDb->plock);}
finish :
 if (DbArrTo.pvmDb->plock){unpLock(DbArrTo.pvmDb->plock);}
end: refDbFree(DbArrTo);
 }
/*
 *    0. код команды Регистры
 *    1. выполняемое над регистром действие
 *       если CvmSubCmdRegMoveObj - то перед копированием 1 - поле данных модифицировать свойством Альтернативного ключа
 *    2.  источник
 */

void vmCmdReg(TvmDo *pDo,TCmd *pCmd){
 Tdata Data,*pData;
 u_char subCmd;
 int isData;
 //
 subCmd=pCmdSub(pCmd);
 switch (regCmdGet(subCmd)) {
  // ---увеличить уровень стека регистра ---
  // 2  источник имени
  // 3. 0 подымается всегда Индексный регистр
  case CvmSubCmdRegUp :
   /*
   //запомнить индекс объекта на стеке
   if (((styleGet(subCmd))==CstyleIndMod)&&(pDo->pJob->iobjUp.busy>0) ){
    Stk2Lev0Up(pDo->pJob->this);
    arrExpand(pDo->pJob->this._core,pDo->pJob->iobjUp);
    }
   */
   //из команды извлекаем данные
   isData=RefToData(pDo,pCmd,&Data);
   //Поднять индекс
   stkIndLev0Up(pDo);
   //установить признак массива
   DatastyleSet(Data,styleGet(subCmd));
   //данные записать в индексный регистр
   if (isData) {stkIndPushData(pDo,Data);}
   break;
  // ---уменьшить уровень стека регистра ---
  // результаты сохранились
  case CvmSubCmdRegPop :
   stkIndLev0Pop(pDo);
   break;
  //в последнее поле индекса записать признак Альтернативной ветви 
  case CvmSubCmdRegAlt :
   if ((pData=stkIndTopData(pDo))) {pDataSetAlt(pData);}
   break;
  //---пересылка данных между регистрами---
  case CvmSubCmdRegMove :
   //из команды извлекаем данные
   IndToData(pDo,pCmd,&Data);
   if (pDo->err>CERR){
    //установить признак Альтернативной ветви
//    if (subCmdIsAlt(subCmd)) DataSetAlt(Data);
    //данные записать в индексный регистр
    stkIndPushData(pDo,Data);
    }
   break;
  }
 }
/**/
static void exevmDo(TvmDo *pDo,Ttr *pTr){
 TvmMod Mod;
 TvmMod *pmod;
 Tsize curOff; //смещение текущей команды
 //
 pvmModInit(&Mod);
 if (pTr)
  TrToMod(pTr,&Mod);
 else{
  Mod.pbegcmds=pDo->pJob->exeTr.Cmds.psec;
  Mod.sumcmd=pDo->pJob->exeTr.Cmds.busy;
  }
 curOff=pDo->curOff;
 pmod=pDo->pmod;
 pDo->pmod=&Mod;
 stkIndLev1Up(pDo);
 mshVmDo(pDo,NULL);
 stkIndLev1Pop( pDo )
 if (cmdDoIsRet(pDo->flag)) {
  stkIndPushData(pDo,ValData(pDo->pJob->val));
  }
 pDo->curOff=curOff;
 pDo->pmod=pmod;
 vmModFree(Mod);
 }
/*
*/
void Execute(TvmDo *pDo,TCmd *pCmd,TdivStream *pdiv){
 Tdata X;
 //из команды извлекаем данные
 PopToData(pDo,pCmd,&X);
 if (pDo->err<CERR) return;
  //выполнить строку ???
 TrRef(pDo,&(pDo->pJob->exeTr),&X);
 if (pDo->pJob->exeTr.errTr.err>CERR) {
  exevmDo(pDo,&(pDo->pJob->exeTr));
  }
 else
  pDo->err=pDo->pJob->exeTr.errTr.err;
 }

/*
 *   ---- команды Lock ----
 *    0. код команды
 *    1. тип блокировки
 *    2.  Идентификатор блокировки
 * 
 */
void LockWR(TvmDo *pDo,Tstr *pstr,char type,int time);
void LockUn(Tstr *pnameIn);
//
void Lock(TvmDo *pDo,TCmd *pCmd){
 Tdata IdLock;
 Tstr *pstr;
 char type;
 //Идентификатор Блокировки
 PopToData(pDo,pCmd,&IdLock);
 if (pDo->err<CERR) return;
 if (DataIsNULL(IdLock)) {pDo->err=CERRvmNotArg;return;}
 pstr=castId(&IdLock);
 if ((pstr==NULL)||(pstr->size<1)) {pDo->err=CERRvmNotArg;return;}
 type=cmdSubpCmdLock(pCmd);
 (type==CvmSubCmdLockU)?LockUn(pstr):LockWR(pDo,pstr,type,-1);
 }
/*
 *    0 .код команды
 *    2. TimeOut
 */
void TimeOut(TvmDo *pDo,TCmd *pCmd){
 Tdata TimeOut;
 //TimeOut
 PopToData(pDo,pCmd,&TimeOut);
 if (pDo->err>CERR) {
  if (DataIsNoNULL(TimeOut)) {
   pDo->err = castDataInt(&TimeOut);
   mshJobYield;
   if (TimeOut.intgr > 0) usleep(TimeOut.intgr);
   }
  }
 }
/**/
Terr cmdJmp(TvmDo *pDo,TCmd *pCmd){
 Tdata *pCond;
 //
 if (pCmdSub(pCmd) == CvmJmpAny) return 1;
 //снять со стека условие перехода
 stkIndPopTop(pDo,pCond);
 if ((pCond==NULL)||(pDataIsNULL(pCond))) return pCmdSub(pCmd)==CvmJmp0;
 castDataInt(pCond);
 return (pCond->intgr)?(pCmdSub(pCmd)==CvmJmp1):(pCmdSub(pCmd)==CvmJmp0);
 }
/*
 *   ---- команды Lock ----
 *    0. код команды
 *    1. 
 *    2.
 * на индексном регистре лежит имя модуля и метка
 */
static void findpModLab(TvmDo *pDo){
 TrefProg refProg;
 //Модуль , Метка
 if ( (pDo->pJob->psMod==NULL)||(pDo->pJob->psMod->size<1) )
  refProg.pmod=pDo->pmod;
 else
  if ((refProg.pmod=getMod(pDo->pJob->psMod,&(pDo->err)))==NULL )
   return;
 if ( (pDo->pJob->psLab==NULL)||(pDo->pJob->psLab->size<1) ){
  pDo->err=CERRvmLabelNotFound;
  return;
  }
 if ( pDo->pJob->psLab ){
  refProg.label=*(pDo->pJob->psLab);
  if ((pDo->err=getLabelApp(&refProg))!=COutKodOk)
   return;
  }
 pDo->pmod=refProg.pmod;
 pDo->curOff=refProg.off;
 }
/*
 */
static void sModsLab(TvmDo *pDo){
 IndDoToArr(pDo,pDo->rarInd)
 if (pDo->rarInd.busy<2 ){
  pDo->err=CERRnameModNotFound;
  return;
  }
 if (pDataIsNoNULL(pDo->rarInd.psec)){
  if (pDataIsNum(pDo->rarInd.psec)){
   pDo->err=CERRnameModNoIdn;
   return;
   }
  pDo->pJob->psMod=castId(pDo->rarInd.psec);
  }
 if (pDataIsNum(pDo->rarInd.psec+1)){
  pDo->err=CERRnameModNoIdn;
  return;
  }
 pDo->pJob->psLab=castId( pDo->rarInd.psec+1);
 if (pDo->pJob->psLab->size<1){
  pDo->err=CERRvmLabelNotFound;
  return;
  }
 if ( pDo->rarInd.busy<3 ){
  pDo->rarInd.busy =0;
  pDo->rarInd.ptop =NULL;
  }
 else{
  pDo->rarInd.psec +=2;
  pDo->rarInd.busy -=2;
  arrsetptop(pDo->rarInd);
  }
 }
/*
 */
void cmdGo(TvmDo *pDo){
 sModsLab(pDo);
 //если заданы Аргументы, то установить указатель на новые аргументы
 if ( pDo->rarInd.busy ) {
  parrCopy(&(pDo->arArgs),(TarrData*)(&(pDo->rarInd)) );
  }
 findpModLab(pDo);
 }
/*
 *  создать новый блок Do
 */
void tstvmprnDo(TvmDo *pDo,TCmd *pcurCmd,TdivStream *pdiv);
/*
 *  из блока Do запустить новое задание
 */
TEvent* FindEvent(Tstr *pevntName);

static void* vmDoJob(void *pDoVoid){
 TvmDo *pDo,*pDoParent;
 TJob *pJob;
 int *piJob,w;
 TEvent *pEvnt;
 //
 pDo=(TvmDo*)pDoVoid;
 pJob=pDo->pJob;
 //в вызвавшем блоке удалить текущее задание
 pDoParent=pDo->pDoParent;
 //выполнить задание
 mshVmDo(pDo,NULL);
 //сигнал завершения задания
 //событие не найдено создать его и ожидать появления обработчиков
 w=pJob->nameJob.size;
 pJob->nameJob.size +=3;
 memCopy(pJob->nameJob.psec+w,"end",3);
 spinLock((App.secsEvent.lock));
 //  событие найдено вызватьего
 if ( (pEvnt=FindEvent(&(pJob->nameJob))) ) {
  EventTrapName(pDo,&(pJob->nameJob));
  pJob->nameJob.size=w;
  }
 spinUnLock(App.secsEvent.lock);
 pJob->idPth=CvmpThreadNULL;
 //освободить регистр объекта
 arrIter(pDoParent->arrJob,piJob)
  if (pJob->iD==*piJob) {parrDel(&(pDoParent->arrJob),piJob); }
 //освободить блок Do и задание Job
 objpDopJobFree(pDo);
 return NULL;
 }
// 
void pArrToLong(TmapLong *pmLong,TarrData *pArr);

void cmdExecDo(TvmDo *pDo,TarrData *pArgs,u_char flag,TdivStream *pdiv){
 TvmDo *pDoNew;
 //
 DataSetNULL(pDo->pJob->val);
 //новый блок Do
 pDoNew=vmGetDoflag(pDo,flag);
 //установить указатель на Аргументы
 if (pArgs) {
  parrCopy(&(pDoNew->arArgs),pArgs );
  }
 //Модуль , Метка
 pDoNew->pmod=pDo->pmod;
 findpModLab(pDoNew);
 if (pDoNew->err<CERR) {
  goto err;
  }
 //новое задание
 if (cmdDoIsJob(flag)) {
  //перенести аргументы в новое задание pJob
  parrCopy(&(stkInd(pDoNew)._core),&(pDoNew->arArgs) );
  //перенести длинные строки
  if (pDoNew->arArgs.ptop) pArrToLong(&(pDoNew->pJob->vmDbLong.db.mlong),&(pDoNew->arArgs) );
  //скоректировать ссылку на аргументы
  IndDoToArr(pDoNew,pDoNew->arArgs);
  //в стек заданий добавить новое задание
  arrPushDan(pDo->arrJob,IdJobGetDo(pDoNew));
  pDoNew->pJob->pfile=pDo->pJob->pfile;
  //запустить новое задание
  mshJobStart(pDoNew->pJob->idPth,vmDoJob,pDoNew);
  }
 else{
  //открыть следующий фрагмент индексного стека
  stkIndLev1Up(pDoNew);
  //выполнить подпрограмму
  mshVmDo(pDoNew,pdiv);
  stkIndLev1Pop(pDoNew)
  if (cmdDoIsRet(pDoNew->flag)) {
   stkIndPushData(pDoNew,ValData(pDoNew->pJob->val));
   }
  objpDopJobFree(pDoNew);
  }
 return;
err:
 pDo->err=pDoNew->err;
 //освободить блок Do,освободить задание.
 objpDopJobFree(pDoNew);
 }
//----------------------------------
/*
 *    0 .код команды
 *    1. флаг вызова блока Do
 *    2. индекс
 */
void cmdDo(TvmDo *pDo,TCmd *pCmd,TdivStream *pdiv){
 Tdata Data,*pSet;
 u_char flag,isret=0,isIn;
 int cmdArg;
 Tbstr bStr;
 //
 pDo->err=COutKodOk;
 flag=CvmSubCmdDODo;
 if (pCmd){
  flag=pCmdSub(pCmd);
  isIn=cmdDoIsIn(flag);
  //внутренние функции и свойства
  if (isIn) {
   DataSetNULL(Data);
   //Свойства обрабатываются особенно. У них нет аргументов и на индексном стеке их тоже нет. Поэтому индексный стек сбрасывать не надо.
   if (isIn==CvmSubCmdDOInProp){
    if ((pCmdArg(pCmd))==CvmTypePropThis) {
     if (pDo->pJob->thisObj.busy){
      pDataSetAlt(pDo->pJob->thisObj.ptop);
      stkIndLev0Clear(pDo);
      parrExpand( &((stkInd(pDo))._core), &(pDo->pJob->thisObj) );
      }
     goto finish;
     }
    if (cmdDoIsSet(flag)){
     stkIndPopTop(pDo,pSet);
     vmInnerPropSet(pDo,pCmd,pSet);
     }
    else{
     vmInnerProp(pDo,pCmd,&Data);
     isret=1;
     }
    }
   else {
    //получить Аргументы
    IndDoToArr(pDo,pDo->rarInd);
    switch(isIn){
     //внутренние функции
     case CvmSubCmdDOInFun :
      //индекс переносим на Аргументы и понижаем уровень стека
      if (cmdDoIsRet(flag)) {
       vmInnerFunct(pDo,pCmd,&(pDo->rarInd),&Data);
       isret=1;
       }
      else{
       vmInnerFunct(pDo,pCmd,&(pDo->rarInd),NULL);
       }
      break;
    /* 
    // внешние функции
    case CvmSubCmdDOextFun :
     //находим имя программы
     pnameMod=&(pDo->rarInd.psec->str);
     //если оно есть ищем среди внешних библиотек
     if (pnameMod->size){
      TextFun *pextFun;
      parrIter(parrextFunc,pextFun)
       //внешняя библиотека найдена
       if  ( (memCompare(pnameMod->psec,pextFun->pref,lpref) )==0 ) {
        if (cmdDoIsRet(flag)) {
         pextFun->pextfn(pDo,pCmd,pnameMod,&(pDo->rarInd),&Data);
         isret=1;
         }
        else{
         pextFun->pextfn(pDo,pCmd,pnameMod,&(pDo->rarInd),NULL);
         }
        break;
        }
      }
     break;
     */
     case CvmSubCmdDOInStat :
      if (pDo->rarInd.busy) Data=pDo->rarInd.psec[0];
      vmInnerStat(pDo,pCmd,&Data);
      isret=1;
      break;
     case CvmSubCmdDOInMetod :
      if (cmdDoIsSet(flag)){
       vmInnerMetodSet(pDo,pCmd,&(pDo->rarInd));
       }
      else{
       vmInnerMetod(pDo,pCmd,&(pDo->rarInd),&Data);
       if (cmdDoIsRet(flag)) isret=1;
       }
      break;
     //внутренние функции
     case CvmSubCmdDOInWin :
      //индекс переносим на Аргументы и понижаем уровень стека
      if (cmdDoIsRet(flag)) {
       pDo->err=vmInnerWin(pDo,pCmd,&(pDo->rarInd),&Data);
       isret=1;
       }
      else{
       pDo->err=vmInnerWin(pDo,pCmd,&(pDo->rarInd),NULL);
       }
      break;
     }
    }
   //закрыть кадр стека
   if (isret){
    if (DataIsOut(Data)){
     bStrGet(&Data,&bStr);
     pStrTopDataOrpArea(&bStr,&(pDo->Temps),&Data);
     }
//    stkIndLev0Up(pDo);
    //
    stkIndPushData(pDo,Data);
    }
   goto finish;
   } //внутренние функции
  //
  }
 sModsLab(pDo);
 if (pDo->err < CERR) goto finish;
 // модифицировать метку для записи в свойство
 if (cmdDoIsSet(flag)) {
  mshVmLabUpdate(pDo->pJob->psLab,pCvmprefSet);
  }
 else if ((cmdDoIsKill(flag))) {
  mshVmLabUpdate(pDo->pJob->psLab,pCvmprefKill);
  //если это команда Kill то аргумент команды перенести в массив Аргументов DO
  cmdArg=pCmdArg(pCmd);
  // pDo->rarInd - псевдо массив он ссылается на массив хранения его расширять нельзя, сам он ничего не хранит в данном случае он наложен на индексный массив
  //поэтому расширяем индексный массив в программе sModsLab(pDo); массив pDo->rarInd уменьшается поэтому в нем заведомо есть хотя бы одно место
  arrBusy(pDo->rarInd);
  pSet=pDo->rarInd.ptop;
  //преобразовать аргумент
  pDataSetStr(pSet);
  pSet->str.size=0;
   if ((cmdArg & CtypeKiLLAll)==0 ) {
   if ( cmdArg & CtypeKiLLData ) {
    pSet->str.psec[pSet->str.size]=CtypesKiLLData;
    pSet->str.size++;
    }
   if ( cmdArg & CtypeKiLLDown ) {
    pSet->str.psec[pSet->str.size]=CtypesKiLLDown;
    pSet->str.size++;
    }
   if ( cmdArg & CtypeKiLLAlt ) {
    pSet->str.psec[pSet->str.size]=CtypesKiLLAlt;
    pSet->str.size++;
    }
   }
  }
 cmdExecDo(pDo,(TarrData*)(&(pDo->rarInd)),flag,pdiv);
finish:;
 // if (cmdDoIsThis(flag)){  Stk2Lev0Pop(pDo->pJob->stkthis)  }
 }
/*
 *    0 .код команды
 *    2. возвращаемое Значение
 */
void cmdRet(TvmDo *pDo,TCmd *pCmd){
 Tdata Out;
 //вернуть значение в Вызвавшую программу
 if ((cmdDoIsRet(pDo->flag))&& (regIstCmd(pCmd)!=CregInd))  {
  //значение для возврата должно быть
  RegToData(pDo,pCmd,&Out);
  stkIndPushData(pDo,Out);
  }
 pDo->stat = CvmStatDoEnd;
 }
/*
 *   ---- команда Case ----
 *    0 .код команды
 *     Значение метки на индексном регистре
 *     off3 номер секции меток команды CASE в Arg
 */
void cmdCase(TvmDo *pDo,TCmd *pCmd){
 Tstr *pStr;
 Tdata *pTop;
 Tsize Off;
 //убрать уровень стека
 stkIndPopTop(pDo,pTop);
 if (pTop==NULL) {pDo->err=CERRvmNotArg;return;}
 pStr=castId(pTop);
 if ((pStr==NULL)||(pStr->size<1)) {pDo->err=CERRvmNotArg;return;}
 //имя вычисленной метки
 if ( (Off = getLabelSecMod(pDo->pmod,pStr,pCmdArg(pCmd)))>=0 )
  pDo->curOff = Off;
// else pDo->err=CWARLabelNotFound;
 }
/*
 *   ---- команда WriteFile ----
 *    0 .код команды
 *    2. Куда читать
 */
/*
const static char CvmSTDOUT[]="OUT";
const static char CvmSTDERR[]="ERR";
*/
static Tdiv cmdFileIsStd(Tbstr *pbStr){
 if (pbStr->size>1){
  if ((pbStr->psec[0]=='S')&&(pbStr->psec[1]=='T'))
   return STDOUT_FILENO;
  if ((pbStr->psec[0]=='E')&&(pbStr->psec[1]=='R'))
   return STDERR_FILENO;
  if ((pbStr->psec[0]=='S')&&(pbStr->psec[1]=='R'))
   return CvmpTCP;
  }
 return CdivRetErr;
 }
//
void fsrvWrite(TvmDo *pDo,Tbstr *pBuf);
//
static void WriteFileData(TvmDo *pDo,Tdiv fFl,Tdata *pData,Tstrshr *pdlmIO){
 Tbstr bData;
// Tbstr bdlm;
 //
 castbStr(pData,&bData);
 if (fFl==CvmpTCP){
  fsrvWrite(pDo,&bData);
//  if ((pdlmIO) && (pdlmIO->size)){   bdlm.size=pdlmIO->size;   bdlm.psec=pdlmIO->psec;   fsrvWrite(pDo,&bdlm);   }
  }
 else{
  divWrite(fFl,bData.psec,bData.size);
  if ((pdlmIO) && (pdlmIO->size)){ divWrite(fFl,pdlmIO->psec,pdlmIO->size); }
  }
 }
//
void cmdWriteFile(TvmDo *pDo,TCmd *pCmd,Tbstr *pbStr){
 Tdiv fFl;
 Tdata *pData;
 int isOpen=0;
 Tusize Off,w;
 Tstrshr dlmIO;
 //
 if ((w=maparrDbBusy(pDo->pJob->vmDbBuf.db))<1) {pDo->err=CERRdataNotFound;return;}
 if ((fFl=cmdFileIsStd(pbStr))==CdivRetErr){
  if ((fFl=divOpenCreate(pbStr->psec))==CdivRetErr) {pDo->err=CERRvmDivFiled;return;}
  isOpen=1;
  }
// if (fFl==CvmpTCP){ dlmIO.psec[0]=Cvm0; dlmIO.size=1; } else
  dlmIO=(pDo->pJob->dlmIO.size)?pDo->pJob->dlmIO:App.dlmIO;
 //вывод из буфера $bf
 for (Off=1;Off <= w; ++Off) {
  pData=mshGetArr(&(pDo->pJob->vmDbBuf.db), Off ,&(pDo->err));
  WriteFileData(pDo,fFl,pData,&dlmIO);
  }
 flush(fFl); 
 //закрыть устройство
 if (isOpen){  divClose(fFl);  }
 mshKillArrALL(&(pDo->pJob->vmDbBuf.db));
 }
//
static Terr strToBufRead(TDb *pDb,char *pCb,char *pCc,Tdata *pData){
 pDataSetbStr(pData);
 pData->bstr.psec=pCb;
 pData->bstr.size=pCc-pCb;
 (void)castNorm(pData);
 return mshPushArr(pDb,pData);
 }
//
void cmdReadFile(TvmDo *pDo,Tbstr *pbStr){
 Tdata Data;
 Tstrshr dlmIO;
 char *pCb,*pCc,*pCe;
 //Очистить буфер
 ListLongClear(&(pDo->pJob->vmDbBuf.db.mlong));
 mshKillArrALL(&(pDo->pJob->vmDbBuf.db));
 if ((pDo->err=mshFileReadToMap(pbStr->psec,&(pDo->pJob->mapBf)))<CERR) return;
 //ввод в буфер $bf
 dlmIO=(pDo->pJob->dlmIO.size)?pDo->pJob->dlmIO:App.dlmIO;
 pCb=pCc=pDo->pJob->mapBf.psec;
 pCe=pCb+pDo->pJob->mapBf.size;
 memNULL(&Data);
 DataSetbStr(Data);
 //если чтение без разделителя
 if (dlmIO.size==0){
   int w;
   w=SizeLongToSizeChar(&(pDo->pJob->vmDbBuf.db.mlong)) - (sizeof(Tsize) * 2);
   while(pCc<pCe){
    pCc=pCb + w;
    if (pCc>pCe) {
     pCc=pCe;
     if (pCc>pCb)
      pDo->err=strToBufRead(&(pDo->pJob->vmDbBuf.db),pCb,pCe,&Data);
     break;
     }
    else{
     for (;trSmbIsNo1(*pCc);pCc--);
     pDo->err=strToBufRead(&(pDo->pJob->vmDbBuf.db),pCb,pCc,&Data);
     pCb=pCc;
     }
    }
   }
  //через разделитель
  else{
   while(pCc<pCe){
    if (*pCc==dlmIO.psec[0]){
     if (dlmIO.size>1){
      if (memCompare(pCc,dlmIO.psec,dlmIO.size)){
       pCc +=dlmIO.size;
       continue;
       }
      }
     //разделитель найден
     pDo->err=strToBufRead(&(pDo->pJob->vmDbBuf.db),pCb,pCc,&Data);
     pCc=pCb=pCc + dlmIO.size;
     continue;
     }
    trSmbNext(pCc);
    }
   if (pCc>pCb)
    pDo->err=strToBufRead(&(pDo->pJob->vmDbBuf.db),pCb,pCc,&Data);
   }
 //
finish:
 mapFixedFree(pDo->pJob->mapBf);
 }
/*
 *   ---- команда OpenFile ----?? так не получится!! полей в команде не хватает
 *    0 .код команды
 *    1. действие над файлом
 *    2. имя файла
 */

void cmdFile(TvmDo *pDo,TCmd *pCmd){
 Tbstr bStr;
 Tdata dFl;
 //Имя файла
 PopToData(pDo,pCmd,&dFl);
 if (pDo->err<CERR) return;
 if (DataIsNULL(dFl)) {pDo->err=CERRvmNotFileName;return;}
 castbStr(&dFl,&bStr);
 if (bStr.psec==NULL) {pDo->err=CWARvmArgNot;return;}
 bStr.psec[bStr.size]='\0';
 switch (CcmdSubpCmdFile(pCmd)){
  case CvmSubCmdRead:
   cmdReadFile(pDo,&bStr);
   break;
  case CvmSubCmdWrite:
   cmdWriteFile(pDo,pCmd,&bStr);
   break;
  }
 }
