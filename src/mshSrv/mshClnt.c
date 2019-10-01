
/* 
 * File:   mshClnt.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 7 ноября 2016 г., 11:59
 */
#include "Srv.h"

void TrToMod(Ttr *pTr,TvmMod *pMod);
void mshVmDo(TvmDo *pDo, TdivStream *pdiv);
void trCodamshCmdBuf(Ttr *pTr,Tbstr *pbufPrg,Tstr *pName,TerrTr *perrTr);
void PackUnpMod(TvmMod *pMod);
int mshMrun(char *pcName,char *pcLabel,TerrTr *perrTr);
Terr StrToDataOrLong(TmapLong *pmLong,Tdata *pDataSet,Tbstr *pStrIn);
void pbStrToDb(TvmDo *pDo,Tbstr *pbStr);
TsrvBuf* bufGet(TsecsBuf *psecsBuf);
Terr mshKillDbDALL(TDb *pDb);
void stkindstorefdb(TvmDo *pDo,TarrData *prefInd,TrefDb *pDbArr,enum TisSet dbisSet);
/**/
static int mshClntIsNum(char *pb,char *pe){
 int isPoint=0;
 if (pb<=pe) return 0;
 if ((*pb=='-')||(*pb=='+')) ++pb;
 if (*pb=='.') {++pb;isPoint=1;}
 for (;pb<pe;++pb)
  if (*pb=='.') {
   if (isPoint) return 0;
   isPoint=1;
   }
  else
   if ((*pb<'0')||(*pb>'9')) return 0;
 return isPoint+1;
 }
/*
 *  поместить строку в поле данных и если надо то и на длинную страницу
 */
static Terr mshClntArg(char *pibeg,char *piend,Tdata *pData,TDb *pDb){
 int isNum;
 Terr err=COutKodOk;
 //
 if ((isNum=mshClntIsNum(pibeg,piend))) {
  *piend='\0';
  if (isNum<2) {
   pData->intgr = StrToInt(pibeg);
   pDataSetInt(pData);
   }
   //преобразуется к действительному числу
  else {
   StrToFloat(pibeg,pData->fnum);
   pDataSetFloat(pData);
   }
  }
 else
  if (piend-pibeg <= CCsizeStr ) {
   pData->str.size=piend-pibeg;
   memCopy(pData->str.psec,pibeg,pData->str.size);
   pDataSetStr(pData);
   }
  //длинная строка
  else{
   int w;
   w=piend-pibeg;
   char buf[w];
   Tbstr bStr;
   bStr.psec=buf;
   bStr.size=w;
   memCopy(buf,pibeg,w);
   err=StrToDataOrLong( &(pDb->mlong),pData,&bStr );
   }
 return err;
 }
//
#define pClntBegCmd(pdataClnt) pdataClnt->pDo->pmod=&(pdataClnt->Mod); pdataClnt->pDo->curOff=0
/**/
int mshClntRun(TsrvClnt *pClnt){
 TdataClnt *pdataClnt=(TdataClnt*)(pClnt->pData);
 pdataClnt->pDo->flag=CvmSubCmdDODo;
 //из Tr в модуль
 TrToMod(&(pdataClnt->pDo->pJob->exeTr),&(pdataClnt->Mod));
 pClntBegCmd(pdataClnt);
 //выполнить
 mshVmDo(pdataClnt->pDo,NULL);
 return pdataClnt->pDo->err;
 }
/*
 */
int mshClntMsh(TsrvClnt *pClnt){
 TdataClnt *pdataClnt=(TdataClnt*)(pClnt->pData);
 TerrTr errTr;
 Tbstr bsBf;
 //транслируем буфер
 pAreaTopbStr(pClnt->pbuf,bsBf);
 trCodamshCmdBuf(&(pdataClnt->pDo->pJob->exeTr),&bsBf,NULL,&errTr);
 return (errTr.err>CERR)?mshClntRun(pClnt):errTr.err;
 }
/**/
int mshClntDbm(TsrvClnt *pClnt){
 int err;
 //
 //убрать код команды
 pClnt->pbuf->psec++;
 pClnt->pbuf->busy--;
 err=mshClntMsh(pClnt);
 /*
 if (err<CERR) {
  char errout[CsizeStr+1];
  snprintf(&errout,CsizeStr,"%i\0",-err);
  mshSrvWriteWWW(pClnt->fdclnt,&errout,strlen(&errout));
  }
  */
 return err;
 }
/*
 *     выполнить программу виртуальной машины 
 */
int mshClntVmm(TsrvClnt *pClnt){
 TdataClnt *pdataClnt=(TdataClnt*)(pClnt->pData);
 char *pName,*pLabel=NULL;
 TDb *pDb;
 char *pC,*pE,*piArg=NULL;
 int err;
 TvmDo *pDo;
 Tdata *pData;
 TerrTr errTr;
 //
 pDo=pdataClnt->pDo;
 pDb=&(pDo->pJob->vmDbLong.db);
 //Аргументы
 Stk3AllClear(pDo->pJob->inds);
// arrClear(pDo->args);
 //
 pC=(char*)(pClnt->pbuf->psec);
 pE=pC+pClnt->pbuf->busy;
 pName=pC;
 do{
  if ((smbIsAnsi(*pC))){
   //следующий Аргумент
   if (*pC==','){
    //следующий Аргумент
    if (piArg) {
     Stk3Busy(pDo->pJob->inds);
     pData=pDo->pJob->inds._core.ptop;
     //static Terr mshClntArg(char *pibeg,char *piend,Tdata *pData,TDb *pDb)
     err=mshClntArg(piArg,pC,pData,pDb);
//     arrBusy(pDo->args);
//     err=mshClntArg(piArg,pC,pDo->args.ptop,pDb);
     }
    //пошли Аргументы
    else{
     *pC='\0';
     piArg=pC+1;
     }
    }
   else
    //дальше идет Метка
    if (*pC=='.'){
     *pC='\0';
     pLabel=pC+1;
     }
   }
  else
   if (piArg==NULL) return CERRnameModNoIdn;
  }while((trSmbNext(pC))<pE);
 //последний Аргумент 
 if (piArg==NULL)
  *pC='\0';
 else {
//  arrBusy(pDo->args);
//  err=mshClntArg(piArg,pC,pDo->args.ptop,pDb);
  Stk3Busy(pDo->pJob->inds);
  pData=pDo->pJob->inds._core.ptop;
  //static Terr mshClntArg(char *pibeg,char *piend,Tdata *pData,TDb *pDb)
  err=mshClntArg(piArg,pC,pData,pDb);
  }
 Stk3toArr(pDo->pJob->inds,pDo->rarInd);
 parrCopy(&(pDo->arArgs),&(pDo->rarInd) );
// err=mshMrun(pName,pLabel,&numstr,&numpos,pname,pnameInclude);
 memNULL(&errTr);
 err=mshMrun(pName,pLabel,&errTr);
 Stk3AllClear(pDo->pJob->inds);
 return err;
 }
/*
 *  выполнить команды виртуальной машины 
 */
int mshClntPim(TsrvClnt *pClnt){
 TdataClnt *pdataClnt=(TdataClnt*)(pClnt->pData);
 TvmDo *pDo;
 //
 pDo=pdataClnt->pDo;
 //новый модуль
 //взять из запроса
 pdataClnt->Mod.modpack.psec=(Tsize*)(pClnt->pbuf->psec);
 pdataClnt->Mod.modpack.busy=mshDivUp(pClnt->pbuf->size,sizeof(*(pdataClnt->Mod.modpack.psec)) );
 //распаковать модуль
 PackUnpMod(&(pdataClnt->Mod));
 pClntBegCmd(pdataClnt);
 if ((pDo->pmod))
  mshVmDo(pDo,NULL);
 else
  pDo->err=CERRvmLabelNotFound;
 return pDo->err;
 }
/*
 * Передать программе пользователя строку с браузера 
 */
int mshClntUsr(TsrvClnt *pClnt){
 TdataClnt *pdataClnt=(TdataClnt*)(pClnt->pData);
 Tdata *pdarg;
 TvmDo *pDo;
 Tsrv *psSrv=pClnt->pSrv;
 //
 if (psSrv->modIs==CsrvIsModNo) return CERRsrvUserNo;
 pDo=pdataClnt->pDo;
 objSetBusy(pDo);
 //модуль
 pDo->pmod=&(psSrv->modTCP);
 pDo->curOff=0;
 //сообщение передаем в программу через строковый Аргумент
 //Аргументы
 arrClear(pDo->arArgs);
 arrBusy(pDo->arArgs);
 pdarg=pDo->arArgs.ptop;
 //адрес клиента заносим в 1-й Аргумент
 pdarg->intgr=pClnt->idClnt;
 pDataSetInt(pdarg);
 //сообщение заносим во 2-й Аргумент
 arrBusy(pDo->arArgs);
 pdarg=pDo->arArgs.ptop;
 pDataSetbStr(pdarg);
 pAreaTopbStr(pClnt->pbuf,pdarg->bstr);
 mshVmDo(pDo,NULL);
 return pDo->err;
 }
/*
 * Обработка сообщений браузера
 */
/**/
static void uriDeCodGet(char *pbufin,TsrvBuf *pbuf){
 char *pcOut,*pB,*pC;
 int n,w=0;
 //
 pcOut=pbuf->psec;
 pB=pbufin;
 while(1){
  pC=strchr(pB,'%');
  if (pC==NULL){
   n=strlen(pB);
   memcpy(pcOut+w,pB,n+1);
   w +=n;
   break;
   }
  n=pC-pB;
  if (n) {
   memcpy(pcOut+w,pB,n);
   w +=n;
   }
  if ( ((MisNum(pC[1])))&&((MisNum((pC[2])))) ){
   pcOut[w] ='\0' | ((MsetChar((pC[1])))<<4) | (MsetChar((pC[2])));
   pB=pC+3;
   }
  else{
   pcOut[w] = pC[0];
   pB=pC+1;
   }
  w++;
  }
 pbuf->busy=w;
 }

/*
 * Обработка сообщений браузера
 */
static void argWWW(TsrvClnt *pClnt){
 Tdata dnameDb;
 TdataClnt *pdataClnt=(TdataClnt*)(pClnt->pData);
 Tdata *pdarg;
 TvmDo *pDo;
 int n;
 int w;
 char **ppenvp=pClnt->request.envp;
 Tbstr bStr;
 char *pcPar;
 char *pbfin;
 
 //
 pDo=pdataClnt->pDo;
 //сообщение передаем в программу через строковый Аргумент
 //Аргументы
 arrBusy1(pDo->arArgs);
 pdarg=pDo->arArgs.ptop;
 //адрес клиента заносим в 1-й Аргумент
 pdarg->intgr=pClnt->idClnt;
 pDataSetInt(pdarg);
 //сообщение заносим во 2-й Аргумент
 pClnt->pbuf=bufGet(&(pClnt->pSrv->arrBuf));
 pBufClear(pClnt->pbuf);
 pcPar=FCGX_GetParam("REQUEST_METHOD", pClnt->request.envp);
 if (pcPar[0]=='G'){
  pbfin=FCGX_GetParam("QUERY_STRING", pClnt->request.envp);
  pareaWide(pClnt->pbuf,strlen(pbfin) )
  uriDeCodGet(pbfin,pClnt->pbuf);
  }
 else{
  pcPar=FCGX_GetParam("CONTENT_LENGTH", pClnt->request.envp);
  w = strtol(pcPar, NULL, 10);
  pareaWide(pClnt->pbuf,w)
  n=FCGX_GetStr(pClnt->pbuf->psec,w,pClnt->request.in);
  pClnt->pbuf->busy=n;
  }
 arrBusy(pDo->arArgs);
 pdarg=pDo->arArgs.ptop;
 pDataSetbStr(pdarg);
 pAreaTopbStr(pClnt->pbuf,pdarg->bstr);
 //параметры запроса переносим в дерево A$
 DataSetStr(dnameDb);
 dnameDb.str.psec[0]=CnameDbPar;
 dnameDb.str.size=1;
 pDo->rarInd.busy=pDo->rarInd.capa=pDo->rarInd.size=1;
 pDo->rarInd.psec=pDo->rarInd.ptop=&dnameDb;
 stkindstorefdb(pDo,&(pDo->rarInd),&(pDo->refDb),EisSet);
 //не удалость создать 
 if (( pDo->err<CERR )||(pDo->refDb.pvmDb==NULL)) return;
 //удалить дерево данных
 (void)mshKillDbDALL( &(pDo->refDb.pvmDb->db) );
 pDo->refDb.rInds.busy=pDo->refDb.rInds.capa=pDo->refDb.rInds.size=1;
 pDo->refDb.rInds.psec=pDo->refDb.rInds.ptop=&dnameDb;
 DataSetbStr(dnameDb);
 for ( ; *ppenvp != NULL; ppenvp++) {
  bStr.psec=*ppenvp;
  bStr.size=strlen(*ppenvp);
  pbStrToDb(pDo,&bStr);
  }
 //отправка заголовка клиенту
 FCGX_PutStr(pClnt->pSrv->htmlheder.psec, pClnt->pSrv->htmlheder.busy, pClnt->request.out);
 }

int mshClntUsrWWW(TsrvClnt *pClnt){
 TdataClnt *pdataClnt=(TdataClnt*)(pClnt->pData);
 TvmDo *pDo;
 Tsrv *psSrv=pClnt->pSrv;
 //
 if (psSrv->modIs==CsrvIsModNo) return CERRsrvUserNo;
 pDo=pdataClnt->pDo;
 //модуль
 pDo->pmod=&(psSrv->modTCP);
 pDo->curOff=0;
 //сообщение передаем в программу через строковый Аргумент
 argWWW(pClnt);
 if (pClnt->pbuf->busy<2) return CERRwinNotArg;
 mshVmDo(pDo,NULL);
 objSetFree(pClnt->pbuf);
 return pDo->err;
 }
/*
 *  выполнить команды виртуальной машины 
 */
int mshClntPimWWW(TsrvClnt *pClnt){
 //сообщение передаем в программу через строковый Аргумент
 argWWW(pClnt);
 return mshClntPim(pClnt);
 }
/*
 *     выполнить программу виртуальной машины 
 */
int mshClntVmmWWW(TsrvClnt *pClnt){
 //
 argWWW(pClnt);
 return mshClntVmm(pClnt);
 }
/**/
int mshClntMshWWW(TsrvClnt *pClnt){
 //
 argWWW(pClnt);
 return mshClntMsh(pClnt);
 }

