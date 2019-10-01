
/* 
 * File:   mshSrv.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 7 ноября 2016 г., 9:40
 */
#include "Srv.h"
/**/
char *psnameSrv[]={"команды суперсервера","команды Msh","программа VM  mod.label","команды виртуальной машины Msh"
        ,"программа Пользователя","команды Msh при отладке"};

int vmAppInit(char *pnameApp);
Terr srvFree(Tsrv *pSrv);
void TrInit(Ttr *pTr);
void TrFree(Ttr *pTr);
void vmDoFree(TvmDo *pDo);
void vmJobFree(TJob* pJob);
int iniDirInit(Tbstr *pbfl,const char *pDir,TIni *pIni);
int srvOut(TdivSrv div,Tbstr *pbsBf);
int bufSend(Tdiv div,Tbstr *pbsBf);
int bufReadMsg(Tdiv div,Tbuf *pBf);
//GIOStatus bufReadChannelMsg(GIOChannel *pChannel,Tbuf *pBf);
int iniReadFL(char *pflname,TIni *pIni);
Tbstr* iniChargetVal(TIni *pIni,char cKey);

void secsBufInit(TsecsBuf *psecsBuf);
void secsBufFree(TsecsBuf *psecsBuf);
TsrvBuf* bufGet(TsecsBuf *psecsBuf);

void changedstatSrv(Tsrv *pSrv,TsrvClnt *pClnt);

TvmDo* vmGetJobDo();

long long int castbStrToInt(Tbstr *pbStr);

 //контролируем изменение состояния серверов
#define statChangeSrvs(pSrv) RegIter(arrSrv,pSrv) pSrv->pfchangedstatSrv=changedstatSrv
 //снять контроль состояния серверов
#define statNoChangeSrvs(pSrv) RegIter(arrSrv,pSrv) pSrv->pfchangedstatSrv=NULL
/*
 */
static int dataClntInit(TsrvClnt *pClnt){
 TdataClnt *pdataClnt;
 //
 pdataClnt=pClnt->pData;
 /**/
 pRegBusy( &( ((Tsrv*)(pClnt->pSrv))->arrdataClnt) );
 pdataClnt=((Tsrv*)(pClnt->pSrv))->arrdataClnt.ptop;
 pClnt->pData=pdataClnt;
 pvmModInit( &(pdataClnt->Mod) );
 if ((pdataClnt->pDo=vmGetJobDo())) {
  pdataClnt->pDo->pJob->divSrv.TCP=pClnt->sctrw;
  pdataClnt->pDo->pJob->fsrvWrite=srvOut;
  if (pdataClnt->Mod.name.size<1){
   pdataClnt->Mod.name.size=1;
   pdataClnt->Mod.name.psec[0]=CsrvModUnDef;
   }
  return COutKodOk;
  }
 return CERRdoFiled;
 }
static int dataClntWWWInit(TsrvClnt *pClnt){
 TdataClnt *pdataClnt;
 //
 pdataClnt=pClnt->pData;
 /**/
// FCGX_InitRequest(&(pClnt->request),((Tsrv*)(pClnt->pSrv))->sctlisten, 0);
 pRegBusy( &( ((Tsrv*)(pClnt->pSrv))->arrdataClnt) );
 pdataClnt=((Tsrv*)(pClnt->pSrv))->arrdataClnt.ptop;
 pClnt->pData=pdataClnt;
 pvmModInit( &(pdataClnt->Mod) );
 if ((pdataClnt->pDo=vmGetJobDo())) {
  pdataClnt->pDo->pJob->divSrv.TCP=pClnt->sctrw;
  pdataClnt->pDo->pJob->fsrvWrite=srvOut;
  if (pdataClnt->Mod.name.size<1){
   pdataClnt->Mod.name.size=1;
   pdataClnt->Mod.name.psec[0]=CsrvModUnDef;
   }
  return COutKodOk;
  }
 return CERRdoFiled;
 }
/*
 */
static int dataClntFree(TsrvClnt *pClnt){
 TdataClnt *pdataClnt=(TdataClnt*)(pClnt->pData);
 //
 if (pdataClnt->Mod.labels.size) {arrFree(pdataClnt->Mod.labels);}
 vmModFree(pdataClnt->Mod);
 return COutKodOk;
 }
void TrClear(Ttr *pTr);
void vmJobClear(TJob *pJob);
void vmDoClear(TvmDo *pDo);
 /*
 */
static int dataClntClear(TsrvClnt *pClnt){
 TdataClnt *pdataClnt=(TdataClnt*)(pClnt->pData);
 //
 vmModClear(pdataClnt->Mod);
 vmDoClear(pdataClnt->pDo);
  pdataClnt->pDo->pJob->divSrv.TCP=pClnt->sctrw;
 return COutKodOk;
 }
static void srvClntFree(TsrvClnt *pClnt){
 if (pClnt->pSrv->pffree) pClnt->pSrv->pffree(pClnt);
 //клиент обслуживает канал
 if ( (objIsBusy(pClnt))  && (pClnt->sctrw!=CsctNULL) ) close(pClnt->sctrw);
 }
/**/
static void stopClnt(Tsrv *pSrv,TsrvClnt *pClnt){
 int isBusy,countBusy,countLoop;
 //завершаем работу клиентов отключая каналы связи
 if (pClnt->sctrw!=CsctNULL) close(pClnt->sctrw);
 mshJobYield;
//проверяем завершение клиентов
 countBusy=countLoop=isBusy=0;
 do {
  isBusy=0;
  if ( objIsBusy(pClnt) ){
   mshJobYield;
   if ( objIsBusy(pClnt) ) ++isBusy;
   }
  if (isBusy==0) break;
  if (isBusy==countBusy) ++countLoop;
  countBusy=isBusy;
  }while(countLoop<5);
 //если не все задания остановлены, пытаемся снять задания принудительно
 if (isBusy) if ( objIsBusy(pClnt) ) {srvClntFree(pClnt);  mshJobcancel(pClnt->thread);  }
 }
/*
 */
static void stopClntALL(Tsrv *pSrv){
 TsrvClnt *pClnt;
 int isBusy,countBusy,countLoop;
 //завершаем работу клиентов отключая каналы связи
 pRegIter(pSrv,pClnt)  if (pClnt->sctrw!=CsctNULL) close(pClnt->sctrw);
 mshJobYield;
 //проверяем завершение клиентов
 countBusy=countLoop=isBusy=0;
 do {
  isBusy=0;
  pRegIter(pSrv,pClnt) {
   if ( objIsBusy(pClnt) ){
    mshJobYield;
    if ( objIsBusy(pClnt) ) ++isBusy;
    }
   }
  if (isBusy==0) break;
  if (isBusy==countBusy) ++countLoop;
  countBusy=isBusy;
  }while(countLoop<5);
 //если не все задания остановлены, пытаемся снять задания принудительно
 if (isBusy) pRegIter(pSrv,pClnt) if ( objIsBusy(pClnt) ) {srvClntFree(pClnt);  mshJobcancel(pClnt->thread);  }
 }
//
int mshClntMsh(TsrvClnt *pClnt);
int mshClntDbm(TsrvClnt *pClnt);
int mshClntVmm(TsrvClnt *pClnt);
int mshClntPim(TsrvClnt *pClnt);
int mshClntUsr(TsrvClnt *pClnt);
int mshClntUsrWWW(TsrvClnt *pClnt);
//
//
int srvOutWWW(TdivSrv div,Tbstr *pbsBf){
 FCGX_Stream *pout=(FCGX_Stream*)(div.pWWW);
 //отправляем само сообщение
 if ( FCGX_PutStr(pbsBf->psec,pbsBf->size,pout ) )
// if ( FCGX_PutS(pbsBf->psec,pout ) )
  {
//  FCGX_FFlush(pout);
  return COutKodOk;
  }
 return CERRSendFiled;
 }
/**/
static Terr getAdr(Tstr *phostport,struct sockaddr_in *paddr,char *pstat){
 char *pC;
 int Port;
 //
 Port=0;
 bzero(paddr,sizeof(*paddr));
 paddr->sin_family=PF_INET;
 if (phostport->psec[0]==CdelemiterHostPort){
  paddr->sin_addr.s_addr=htonl( INADDR_ANY );
  pC=phostport->psec+1;
  }
 else{
  pC=memFindChar(phostport->psec,CdelemiterHostPort,phostport->size);
  if (pC==NULL) {
   *pstat=CsrvStatSocketerr;
   return CERRPortFiled;
   }
  *pC=Cvm0;
  paddr->sin_addr.s_addr = inet_addr(phostport->psec);
  pC++;
  }
 if (pC==NULL) {
  *pstat=CsrvStatSocketerr;
  return CERRPortFiled;
  }
 Port=StrToInt(pC);
 paddr->sin_port=htons( Port );
 *pstat=CsrvStatBind;
 return COutKodOk;
 }
/*
 * Инициализация клиента
 * 
 */
static void srvClntInit(Tsrv *pSrv,TsrvClnt *pClnt,long numClnt){
 objSetBusy(pClnt);
 srvClntBeg(pClnt,pSrv);
 pClnt->pSrv=pSrv;
 pClnt->idClnt=numClnt;
 pClnt->pData=NULL;
 pClnt->sctrw=pSrv->sctClnt;
 if (pSrv->pfinit) pSrv->pfinit(pClnt);
 }
/**/
static void srvClntClear(TsrvClnt *pClnt){
 objSetBusy(pClnt);
 srvClntBeg(pClnt,pClnt->pSrv);
 if (pClnt->pSrv->pfclear) pClnt->pSrv->pfclear(pClnt);
 }
/**/
static void sockpSrvInit(Tsrv *pSrv,Terr *perr) {
 struct sockaddr_in addr;
 //
 *perr=COutKodOk;
 pSrv->stat=CsrvStatSocket;
 if ((pSrv->sctlisten=netSocket(pSrv->protocol) )<0) {
  *perr=CERRScktFiled;
  pSrv->stat=CsrvStatSocketerr;
  return;
  }
 *perr=getAdr(&(pSrv->hostport),&addr,&(pSrv->stat) );
 if (*perr==COutKodOk){
  pSrv->stat=CsrvStatListen;
  if ((listen( pSrv->sctlisten,CsizeListen)) ) {
   *perr=CERRsrvListenFiled;
   pSrv->stat=CsrvStatListenerr;
   }
  }
 }
/*
 * Сервер TCP
 */
static Terr srvInit(Tsrv *pSrv
  ,int (*pfwork)(TsrvClnt *pClnt),int (*pfinit)(TsrvClnt *pClnt),int (*pffree)(TsrvClnt *pClnt),int (*pfclear)(TsrvClnt *pClnt)
  ,void (*pfchangedstatSrv)(Tsrv *pSrv,TsrvClnt *pClnt)){
 Terr err;
 //
 pSrv->isWWW=0;
 memNULL(&(pSrv->htmlheder));
 sockpSrvInit(pSrv,&err);
 if (err>CERR) {
  pSrv->stat=CsrvStatInit;
  pSrv->pfwork=pfwork;
  pSrv->pfinit=pfinit;
  pSrv->pffree=pffree;
  pSrv->pfclear=pfclear;
  pSrv->pfchangedstatSrv=pfchangedstatSrv;
  pRegInit(pSrv,CsumClnt);
  secsBufInit( &(pSrv->arrBuf) );
  }
 else
  pSrv->stat=CsrvStatIniterr;
 return err;
 }
/*
 * Сервер WWW
 */
static Terr srvWWWInit(Tsrv *pSrv
  ,int (*pfwork)(TsrvClnt *pClnt),int (*pfinit)(TsrvClnt *pClnt),int (*pffree)(TsrvClnt *pClnt),int (*pfclear)(TsrvClnt *pClnt)
  ,void (*pfchangedstatSrv)(Tsrv *pSrv,TsrvClnt *pClnt)){
 Tdiv div;
 char *pflname="./ini/htmlheder.ini";
 char *phtmlheder="Content-type: text/html; charset=UTF-8\r\nServer: Noname\r\n\r\n";
 int n;
 //
 pSrv->isWWW=1;
 FCGX_Init(); //инициализация библилиотеки
 printf("Lib is inited\n");        
 pSrv->sctlisten = FCGX_OpenSocket(pSrv->hostport.psec,20); //открываем новый сокет 
 if(pSrv->sctlisten < 0) { //ошибка при открытии сокета
  pSrv->stat=CsrvStatIniterr;
  return CERRScktFiled; 
  }
 pSrv->stat=CsrvStatInit;
 pSrv->pfwork=pfwork;
 pSrv->pfinit=pfinit;
 pSrv->pffree=pffree;
 pSrv->pfclear=pfclear;
 pSrv->pfchangedstatSrv=pfchangedstatSrv;
 pRegInit(pSrv,CsumClnt);
 secsBufInit( &(pSrv->arrBuf) );
 BufInit(pSrv->htmlheder);
 div=divOpenR(pflname);
 //файл не найден
 if (div==CdivRetErr){
  n=strlen(phtmlheder);
  areaWide(pSrv->htmlheder,n)
  pSrv->htmlheder.busy=n;
  memCopy(pSrv->htmlheder.psec,phtmlheder,n);
  }
 else{
  divSize(div,n);
  areaWide(pSrv->htmlheder,n+2)
  divRead(div,pSrv->htmlheder.psec,n);
  if (pSrv->htmlheder.psec[n-1]!='\n') {
   pSrv->htmlheder.psec[n]='\r';
   pSrv->htmlheder.psec[n+1]='\n';
   pSrv->htmlheder.psec[n+2]='\r';
   pSrv->htmlheder.psec[n+3]='\n';
   pSrv->htmlheder.busy=n+4;
   }
  else
   if (pSrv->htmlheder.psec[n-3]!='\n') {
    pSrv->htmlheder.psec[n]='\r';
    pSrv->htmlheder.psec[n+1]='\n';
    pSrv->htmlheder.busy=n+2;
    }
  divClose(div);
  }
 return COutKodOk;
 }
/*
void srvClear(Tsrv *pSrv){
 TsrvClnt *pClnt;
 //
 if (pSrv->stat!=CsrvStatFree){
  secsBufClear(&(pSrv->arrBuf));
  pRegIter(pSrv,pClnt) {
   if ( objIsFree(pClnt) ){
    objSetFree(pClnt);
    srvClntBeg(pClnt,pClnt->pSrv);
    if (pClnt->pSrv->pfclear) pClnt->pSrv->pfclear(pClnt);
    }
   }
  }
 }
*/
/*
 */
Terr loadDivMod0(TvmMod *pMod);
/*
 * Запуск сервера
 */
static Terr tcpSrvInit(Tsrv *pSrv,Tbstr *pbHostPort,Tbstr *pbArg){
// char snmod[CCsizeStr+offsetof(Tstr,psec)]="00sysNetUser";
// Tstr *psmod;
 Terr err=COutKodOk;
 //
 pSrv->stat=CsrvStatWait;
 pSrv->modIs=CsrvIsModNo;
 pSrv->id=arrSrv.busy-1;
 pStrToStr(pSrv->hostport, pbHostPort);
 pSrv->hostport.psec[pSrv->hostport.size]=Cvm0;
 //Входной поток этого сервера обрабатывает пользовательская программа. Ее и надо найти.
 pvmModInit(&(pSrv->modTCP));
 if ( (pSrv->typeSrv==CsrvTypeUsr)||(pSrv->typeSrv==CsrvTypeUsrWWW) ) {
  if (pbArg->size){
   StrToStrID(pSrv->modTCP.name,*pbArg);
   }
  else{
   if (pSrv->typeSrv==CsrvTypeUsr) {
    strCopy(pSrv->modTCP.name.psec,"sysNetUser");
    }
   else{
    strCopy(pSrv->modTCP.name.psec,"sysNetWWW");
    }
   pSrv->modTCP.name.size=strlen(pSrv->modTCP.name.psec);
//   psmod=(Tstr*)snmod;
//   psmod->size=strlen(snmod)-offsetof(Tstr,psec);
//   StrToStrID(pSrv->modTCP.name,*psmod);
   }
  pSrv->modTCP.name.psec[pSrv->modTCP.name.size]=Cvm0;
  if ((err=loadDivMod0(&(pSrv->modTCP))<CERR)) {pSrv->modIs=CsrvIsModNo;return CERRsrvUserNo;}
  else{
   if (pSrv->modTCP.mapmod.psec==NULL) {pSrv->modIs=CsrvIsModNo;return CERRsrvUserNo;}
   pSrv->modIs=CsrvIsMod;
   }
  }
 // 
 RegInit(pSrv->arrdataClnt,CsumarrdataClntDef);
 //Запускаем соответствующий типу сервер.
 ((Tsrv*)pSrv)->protocol=CprotoNet;
 switch(pSrv->typeSrv){
  //выполнить Msh команду
  case CsrvTypeMsh :
   err=srvInit(pSrv, mshClntMsh,dataClntInit,dataClntFree,dataClntClear,NULL );
   break;
  //отладка Msh команды
  case CsrvTypeDbm :
   err=srvInit(pSrv, mshClntDbm,dataClntInit,dataClntFree,dataClntClear,NULL );
   break;
  //выполнить программу виртуальной машины 
  case CsrvTypeVmm : 
   err=srvInit(pSrv, mshClntVmm,dataClntInit,dataClntFree,dataClntClear,NULL );
   break;
  //выполнить команды виртуальной машины 
  case CsrvTypePim :
   err=srvInit(pSrv, mshClntPim,dataClntInit,dataClntFree,dataClntClear,NULL );
   break;
  //вернуть Hmm документ 
//  case CsrvTypeHmm :  err=srvInit((Tsrv*)pSrv, Port,mshClntPim,dataClntInit,dataClntFree,dataClntClear,NULL );   break;
  //обработать команды пользователя 
  case CsrvTypeUsr :
   err=srvInit(pSrv, mshClntUsr,dataClntInit,dataClntFree,dataClntClear,NULL );
   break;
  case CsrvTypeUsrWWW :
   ((Tsrv*)pSrv)->protocol=IPPROTO_TCP;
   err=srvWWWInit(pSrv, mshClntUsrWWW,dataClntWWWInit,dataClntFree,dataClntClear,NULL );
   break;
  }
 return err;
 }
/*
 * Освобождение ресурсов сервера
 */
//
static void stopSrv(Tsrv *pSrv){
 //
 if ( (pSrv->stat==CsrvStatFree)||(pSrv->stat==CsrvStatStop) ) return;
 pSrv->stat=CsrvStatstop;
 //обрываем канал приема клиентов
 if (pSrv->sctlisten!=CsctNULL) {close(pSrv->sctlisten);pSrv->sctlisten=CsctNULL;}
 //завершаем работу клиентов отключая каналы связи
 stopClntALL(pSrv);
// srvClear(pSrv);
// if (pSrv->pfchangedstatSrv) pSrv->pfchangedstatSrv(pSrv,NULL);
 }

static void tcpSrvFree(Tsrv *pSrv){
 //остановить поток
 if (pSrv->modTCP.labels.size) {arrFree(pSrv->modTCP.labels);}
 mshJobcancel( pSrv->thread );
 if ( (pSrv->stat!=CsrvStatFree)&&(pSrv->stat!=CsrvStatStop) ){
  stopSrv(pSrv); 
  secsBufFree( &(pSrv->arrBuf) );
  pRegFree(pSrv);
  pSrv->stat=CsrvStatFree;
  }
 if (pSrv->pfchangedstatSrv) pSrv->pfchangedstatSrv(pSrv,NULL);
 }
//поток клиента
void* pthrsrvClnt(void *pArg){
 TsrvClnt* pClnt;
 int err=COutKodOk;
// GIOStatus st;
 //
 pClnt=(TsrvClnt*)pArg;
 pClnt->pid=getpid();
 pClnt->ppid=getppid();
 pClnt->pChannel=g_io_channel_unix_new(pClnt->sctrw);
 if (pClnt->pSrv->pfchangedstatSrv) pClnt->pSrv->pfchangedstatSrv(pClnt->pSrv,pClnt);
 while (pClnt->pSrv->stat!=CsrvStatStop) {
  //читаем сообщение
  pClnt->stat=CsrvStatRead;
  pClnt->pbuf=bufGet(&(pClnt->pSrv->arrBuf));
  err=bufReadMsg(pClnt->sctrw,(Tbuf*)(pClnt->pbuf));
  if (err!=COutKodOk) break;
//  st=bufReadChannelMsg(pClnt->pChannel,(Tbuf*)(pClnt->pbuf)); if (st==G_IO_STATUS_EOF) break;
  if (pClnt->pbuf->busy>0){
   //обработка полученного сообщения
   if (pClnt->pSrv->pfwork)
    err=pClnt->pSrv->pfwork(pClnt);
   }
  objSetFree(pClnt->pbuf);
  }
 if (pClnt->sctrw!=CsctNULL) {
  close(pClnt->sctrw);
  pClnt->sctrw=CsctNULL;
  }
 pClnt->stat=CsrvStatEnd;
 objSetFree(pClnt);
 if (pClnt->pSrv->pfchangedstatSrv) pClnt->pSrv->pfchangedstatSrv(pClnt->pSrv,pClnt);
 objSetFree(pClnt);
 return NULL;
 }
//поток клиента
void pthrsrvWWWClnt(TsrvClnt *pClnt){
 int err=COutKodOk;
// GIOStatus st;
 pClnt->pid=getpid();
 pClnt->ppid=getppid();
 pClnt->pChannel=g_io_channel_unix_new(pClnt->sctrw);
 if (pClnt->pSrv->pfchangedstatSrv) pClnt->pSrv->pfchangedstatSrv(pClnt->pSrv,pClnt);
 if (pClnt->pSrv->pfwork)
  err=pClnt->pSrv->pfwork(pClnt);
 pClnt->stat=CsrvStatEnd;
 objSetFree(pClnt);
 if (pClnt->pSrv->pfchangedstatSrv) pClnt->pSrv->pfchangedstatSrv(pClnt->pSrv,pClnt);
 objSetFree(pClnt);
 FCGX_Finish();
 }
//
/*
 * получить нового клиента
 */
TsrvClnt* srvClntGet(Tsrv *pSrv,long numClnt){
 TsrvClnt *pClnt,*pclnt0=NULL;
 //
 pRegIter(pSrv,pClnt) {
  // такой клиент уже есть
  if (pClnt->idClnt==numClnt) {
   pClnt->sctrw=pSrv->sctClnt;
   if (objIsBusy(pClnt)) return NULL;
   objSetBusy(pClnt);
//   srvClntClear(pClnt);
   return pClnt;
   }
  if (objIsFree(pClnt)) pclnt0=pClnt;
  }
 if (pclnt0){
  objSetBusy(pClnt);
  srvClntClear(pclnt0);
  pclnt0->idClnt=numClnt;
  return pclnt0;
  }
 //Такого клиента еще нет добавить его
 pRegBusy(pSrv);
 srvClntInit(pSrv,pSrv->ptop,numClnt);
 return pSrv->ptop;
 }
/*
 */
int srvListenClnt(Tsrv *pSrv){
 socklen_t sa;
 struct sockaddr_in addr;
 long numClnt;
 TsrvClnt* pClnt;
 int err=COutKodOk;
 FCGX_Request request;
 char *pkeyAdr="REMOTE_PORT";
 char *pAdr;
 TdataClnt *pdataClnt;
 //
  //ошибка при инициализации структуры запроса
 if ((pSrv->isWWW)&&(FCGX_InitRequest(&request, pSrv->sctlisten, 0) != 0)) {
  return CERRrequestFiled;
  } 
 sa=sizeof(addr);
 while(pSrv->stat!=CsrvStatstop) {
  pSrv->stat=CsrvStatAccept;
  if (pSrv->pfchangedstatSrv) pSrv->pfchangedstatSrv(pSrv,NULL);
  if (pSrv->isWWW){
   if (FCGX_Accept_r(&request)<0) return CERRrequestFiled;
   pSrv->sctClnt=request.nWriters;
   }
  else
   pSrv->sctClnt=accept(pSrv->sctlisten,(struct sockaddr *)(&addr),&sa);
  if (pSrv->sctClnt<0){
   err=errno;
   if ( (errno==EINTR)||(errno==ECONNABORTED)||(errno==EAGAIN) ) continue;
   pSrv->stat=CsrvStatAccepterr;
   err=CERRAccept;
   break;
   }
  pSrv->stat=CsrvStatRead;
  if (pSrv->pfchangedstatSrv) pSrv->pfchangedstatSrv(pSrv,NULL);
  if (pSrv->isWWW){
   pAdr=FCGX_GetParam(pkeyAdr, request.envp);
   numClnt=strtoint(pAdr);
   }
  else
   numClnt=(int long)(addr.sin_addr.s_addr);
  if ((pClnt=srvClntGet(pSrv,numClnt))==NULL) return CERRsrvClntFiled;
  pClnt->sctrw=pSrv->sctClnt;
  // новый клиент запустить его в новом потоке
  if (pSrv->isWWW){
   pdataClnt=(TdataClnt*)(pClnt->pData); 
   pClnt->request=request;
   pdataClnt->pDo->pJob->divSrv.pWWW=pClnt->request.out;
//   pdataClnt->pDo->pJob->divSrv.TCP=pSrv->sctClnt;
   pthrsrvWWWClnt(pClnt);
   }
  else{
   mshJobStart(pClnt->thread,pthrsrvClnt,pClnt);
   }
  if (pSrv->stat==CsrvStatstop) break;
  }
 if (pSrv->sctlisten!=CsctNULL) {close(pSrv->sctlisten);pSrv->sctlisten=CsctNULL;}
 //связь прослушивающего клиента потеряна 
 stopSrv(pSrv);
 pSrv->stat=CsrvStatStop;
 if (pSrv->pfchangedstatSrv) pSrv->pfchangedstatSrv(pSrv,NULL);
 return err;
 }
//поток клиента сервера
void* pthrSrv(void *pArg){
 Tsrv *pSrv= (Tsrv*)pArg;
 //
 ((Tsrv*)(pSrv))->pid=getpid();
 ((Tsrv*)(pSrv))->ppid=getppid ();
 pSrv->err=srvListenClnt(pSrv);
 return NULL;
 }
/*
 */
//
static void stoptcpSrv(Tsrv *pSrv){
 if (pSrv->stat!=CsrvStatStop) {
  pSrv->stat=CsrvStatstop;
  if (pSrv->sctlisten!=CsctNULL) {close(pSrv->sctlisten);pSrv->sctlisten=CsctNULL;}
  mshJobYield;
  if (pSrv->stat!=CsrvStatStop){
   stopSrv(pSrv);
   mshJobcancel(pSrv->thread);
   mshJobYield;
   if (pSrv->stat!=CsrvStatStop){
    pSrv->stat=CsrvStatStop;
    }
   }
  }
 if (pSrv->pfchangedstatSrv) pSrv->pfchangedstatSrv((Tsrv*)pSrv,NULL);
 }
/*
 * Управление серверами из внешних программ
 */
#define addBufLen(pb,len) BufBusy(supSrv.buf,len); pb=areaTop(supSrv.buf)

static void viewSrvSub(Tsrv *pSrv){
 char *pb,*pS,pNum[2];
 int w,num,wn;
 //имя сервера
 pNum[0]=pSrv->typeSrv;
 pNum[1]='\0';
 //Id сервера
 num=(pSrv->typeSrv <='9' )?atoi(pNum):pSrv->typeSrv - 'A' + 10;
 pS=psnameSrv[num];
 wn=strlen(pS);
 w =wn + 52;
 areaWide(supSrv.buf,w)
 pb=areaTop(supSrv.buf);
 snprintf(pb,5,"%04i",pSrv->id);
 addBufLen(pb,4);
 *pb=CrazCol;
 addBufLen(pb,1);
 memCopy(pb,pS,wn);
 addBufLen(pb,wn);
 //состояние сервера
 pb[0]=CrazCol;
 pb[1]=pSrv->stat;
 pb[2]=CrazCol;
 addBufLen(pb,3);
 //поток сервера
 snprintf(pb,21," %9i/%9i",pSrv->pid,pSrv->ppid);
 addBufLen(pb,20);
 *pb=CrazCol;
 addBufLen(pb,1);
 //Soket сервера
 snprintf(pb,11," %9i",pSrv->sctlisten);
 addBufLen(pb,10);
 *pb=CrazCol;
 addBufLen(pb,1);
 //Порт сервера
// snprintf(pb,11," %s",pSrv->hostport);
// addBufLen(pb,10);
 snprintf(pb,11," %s",pSrv->hostport.psec);
 addBufLen(pb,10);
 //Программа
 *pb=CrazCol;
 addBufLen(pb,1);
 snprintf(pb,pSrv->modTCP.name.size+2," %s",pSrv->modTCP.name.psec );
 addBufLen(pb,pSrv->modTCP.name.size+1);
 }
//
static void viewSrv(void){
 Tsrv *pSrv;
 char *pb;
 int w;
 Tbstr bsBf;
 //запрос по состоянию серверов
 spinLock(supSrv.lock);
 BufClear(supSrv.buf);
 w=(arrSrv.busy>1)?arrSrv.busy+3:4 ;
 areaWide(supSrv.buf,w)
 pb=supSrv.buf.psec;
 pb[0]=CsrvCodGuideSrv;
 pb[1]=CbegDataTree;
 addBufLen(pb,2);
 //перебор серверов
 RegIter(arrSrv,pSrv) {
  pSrv->pfchangedstatSrv=NULL;
  if (pSrv != *(arrSrv._ct0.psec) ){
   *pb=CrazRow;
   addBufLen(pb,1);
   }
  viewSrvSub(pSrv);
  pb=areaTop(supSrv.buf);
  }
 //
 pb[0]=CendDataTree;
 pb[1]='\0';
 addBufLen(pb,2);
 srvBufSend(supSrv.sctClnt,supSrv.buf,bsBf);
// srvsendMsg(supSrv.pChannel,supSrv.buf,bsBf);
 #ifdef DEB_LOG
  write(logflout,supSrv.buf.sbuf.psec,supSrv.buf.sbuf.size);
 #endif
 statChangeSrvs(pSrv); 
 spinUnLock(supSrv.lock);
 }
/*
 * протокол
 * s5\0<строка сервера>\0
 */
static void viewSrv1(Tsrv *pSrv){
 char *pb;
 char bnum[6];
 int w,wn;
 Tbstr bsBf;
 //запрос по состоянию серверов
 spinLock(supSrv.lock);
 pSrv->pfchangedstatSrv=NULL;
 BufClear(supSrv.buf);
 snprintf(bnum,5,"%i",pSrv->id);
 wn=strlen(bnum)+1;
 w =wn + 4;
 areaWide(supSrv.buf,w)
 pb=supSrv.buf.psec;
// areaCharBusyTop(supSrv.buf,1,pb);
 pb[0]=CsrvCodGuideSrv1;
 addBufLen(pb,1);
 // areaCharBusyTop(supSrv.buf,w,pb);
 memCopy(pb,bnum,wn);
 addBufLen(pb,wn);
// areaCharBusyTop(supSrv.buf,1,pb);
 pb[0]=CbegDataTree;
 addBufLen(pb,1);
 //
 viewSrvSub(pSrv);
 pb=areaTop(supSrv.buf);
 //
 pb[0]=CendDataTree;
 pb[1]='\0';
 addBufLen(pb,2);
 srvBufSend(supSrv.sctClnt,supSrv.buf,bsBf);
 #ifdef DEB_LOG
  write(logflout,supSrv.buf.sbuf.psec,supSrv.buf.sbuf.size);
 #endif
 pSrv->pfchangedstatSrv=changedstatSrv;
 spinUnLock(supSrv.lock);
 }
//
#define statusviewSrv  viewSrv();  supSrv.viewCmd=CsrvCodGuideSrv;  supSrv.pviewSrv=NULL
/*
 */
static void viewClntSub(Tsrv *pSrv,TsrvClnt *pClnt){
 char *pb;
 int w;
 //формирование выходного сообщения
 //имя модуля
 TdataClnt *pData;
 pData=(TdataClnt*)(pClnt->pData);
 //--------------
 //Id клиента
 w=pData->Mod.name.size+38;
 areaWide(supSrv.buf,w)
 pb=areaTop(supSrv.buf);
 snprintf(pb,5,"%04li",pClnt->idClnt);
 addBufLen(pb,4);
 *pb=CrazCol;
 addBufLen(pb,1);
 //имя модуля
// areaCharBusyTop(supSrv.buf,pData->Mod.name.size+1,pb);
 if (pData->Mod.name.size) {
  memCopy(pb,pData->Mod.name.psec,pData->Mod.name.size);
  addBufLen(pb,pData->Mod.name.size);
  }
 else {
  *pb=CsrvModUnDef;
  addBufLen(pb,1);
  }
 *pb=CrazCol;
 addBufLen(pb,1);
 //состояние клиента
 pb[0]=pClnt->stat;
 pb[1]=CrazCol;
 addBufLen(pb,2);
 //поток клиента
 snprintf(pb,21," %9i/%9i",pClnt->pid,pClnt->ppid);
 addBufLen(pb,20);
 *pb=CrazCol;
 addBufLen(pb,1);
 //Soket сервера
 snprintf(pb,11,"%10i",pClnt->sctrw);
 addBufLen(pb,10);
 }
static void viewClnt(Tsrv *pSrv){
 TsrvClnt *pClnt;
 char *pb;
 int w;
 Tbstr bsBf;
 //формирование выходного сообщения
 spinLock(supSrv.lock);
 statNoChangeSrvs(pSrv);
// areaClear(supSrv.buf);
 BufClear(supSrv.buf);
 w=(pSrv->busy>1)?pSrv->busy+3:4;
 areaWide(supSrv.buf,w)
 pb=supSrv.buf.psec;
// areaCharBusyTop(supSrv.buf,2,pb);
 pb[0]=CsrvCodGuideClnt;
 pb[1]=CbegDataTree;
 addBufLen(pb,2);
 pRegIter(pSrv,pClnt) {
  if (pClnt != *(pSrv->_ct0.psec) ){
   *pb=CrazRow;
   addBufLen(pb,1);
   }
  //
  viewClntSub(pSrv,pClnt);
  pb=areaTop(supSrv.buf);
  }
 //
// areaCharBusyTop(supSrv.buf,2,pb);
 pb[0]=CendDataTree;
 pb[1]='\0';
 addBufLen(pb,2);
 srvBufSend(supSrv.sctClnt,supSrv.buf,bsBf);
 #ifdef DEB_LOG
  write(logflout,supSrv.buf.sbuf.psec,supSrv.buf.sbuf.size);
 #endif
 statChangeSrvs(pSrv); 
 spinUnLock(supSrv.lock);
 }
static void viewClnt1(Tsrv *pSrv,TsrvClnt *pClnt){
 char *pb;
 char bnum[6];
 int w,wn;
 Tbstr bsBf;
 //запрос по состоянию серверов
 spinLock(supSrv.lock);
 pSrv->pfchangedstatSrv=NULL;
// areaClear(supSrv.buf);
 BufClear(supSrv.buf);
 snprintf(bnum,5,"%li",pClnt->idClnt);
 wn=strlen(bnum)+1;
 w=wn+4;
 areaWide(supSrv.buf,w)
 pb=supSrv.buf.psec;
// areaCharBusyTop(supSrv.buf,1,pb);
 pb[0]=CsrvCodGuideClnt1;
 addBufLen(pb,1);
// areaCharBusyTop(supSrv.buf,w,pb);
 memCopy(pb,bnum,wn);
 addBufLen(pb,wn);
// areaCharBusyTop(supSrv.buf,1,pb);
 pb[0]=CbegDataTree;
 addBufLen(pb,1);
 //
 viewClntSub(pSrv,pClnt);
 pb=areaTop(supSrv.buf);
 //
 pb[0]=CendDataTree;
 pb[1]='\0';
 addBufLen(pb,2);
 srvBufSend(supSrv.sctClnt,supSrv.buf,bsBf);
 #ifdef DEB_LOG
  write(logflout,supSrv.buf.sbuf.psec,supSrv.buf.sbuf.size);
 #endif
 pSrv->pfchangedstatSrv=changedstatSrv;
 spinUnLock(supSrv.lock);
 }
/*
 * состояние сервера изменилось
 */
void changedstatSrv(Tsrv *pSrv,TsrvClnt *pClnt){
 switch (supSrv.viewCmd){
  case CsrvCodGuideSrv :
   if (pSrv){
    viewSrv1((Tsrv*)pSrv);
    }
   else viewSrv();
   break;
  case CsrvCodGuideClnt :
   if (pSrv == (Tsrv*)(supSrv.pviewSrv) ) {
   if (pClnt){
    viewClnt1((Tsrv*)pSrv,pClnt);
    }
   else viewClnt((Tsrv*)pSrv);
    }
   break;
  }
 }
/*
 */
static int getPortSrv(Tsrv *pSrv){
 Tbstr *pbHostPort;
 //
 pSrv->hostport.size=0;
 if ((pbHostPort=iniChargetVal(&(supSrv.iniPipe),pSrv->typeSrv))==NULL) return CERRPortNotFound;
 pStrToStr(pSrv->hostport,pbHostPort);
 return COutKodOk;
 }
/**/
static void ValToPortArg(Tbstr *pVal,Tbstr *pbHostPort,Tbstr *pbArg){
 //
 pbHostPort->psec=pVal->psec;
 pbHostPort->size=pVal->size;
 pbArg->size=0;
 if ((pbArg->psec=memFindChar(pVal->psec,CCvmGT,pVal->size))){
  pbHostPort->size=pbArg->psec-pVal->psec;
  pbArg->size=pVal->size-pbHostPort->size-1;
  pbArg->psec++;
  }
 else
  pbArg->psec=NULL;
 }
/*
 * запустить все сервера кроме суперсервера
 */
static int startalltcpSrv(void){
 Tsrv *pSrv;
 TbKeyVal *pbKeyVal;
 Tbstr bArg,bHostPort;
 Terr err;
 //
 arrIter(supSrv.iniPipe,pbKeyVal)
  if (pbKeyVal->key.psec[0]==CsrvTypeSup){
   StrToStr(supSrv.hostport, pbKeyVal->val);
   supSrv.hostport.psec[supSrv.hostport.size]=Cvm0;
   }
  else {
   pRegBusy(&arrSrv);
   arrSrv.ptop->typeSrv=pbKeyVal->key.psec[0];
   ValToPortArg(&(pbKeyVal->val),&bHostPort,&bArg);
   if ( (err=tcpSrvInit(arrSrv.ptop,&bHostPort,&bArg)) <CERR) {
    pRegPop(&arrSrv);
    printf("\nerrCode=%hi\ntype Server %c\n",err,pbKeyVal->key.psec[0]);
    }
   }
 //нет описаний Серверов
 if (arrSrv.busy<1) {
  printf("\nerrCode=%hi\n",CERRsrvNot);
  return CERRsrvNot;
  }
 //Запускаем Сервера
 RegIter(arrSrv,pSrv) {mshJobStart(pSrv->thread,pthrSrv,pSrv);}
 return COutKodOk;
 }
/*
 * Рестарт серверов
 */
static int restarttcpSrv(void){
 Tsrv *pSrv;
 int nSrv=0;
 Terr err;
 TbKeyVal *pbKeyVal;
 Tbstr bArg,bHostPort;
 //
 if ((err=iniReadFL(supSrv.pflIni,&(supSrv.iniPipe) ))<CERR) return err;
 
 arrIter(supSrv.iniPipe,pbKeyVal)
  if (pbKeyVal->key.psec[0]!=CsrvTypeSup){
   //массив серверов закончен
   if (nSrv >= arrSrv.busy){
    pRegBusy(&arrSrv);
    arrSrv.ptop->typeSrv=pbKeyVal->key.psec[0];
    ValToPortArg(&(pbKeyVal->val),&bHostPort,&bArg);
    if ( tcpSrvInit(arrSrv.ptop,&bHostPort,&bArg) <CERR) {
     pRegPop(&arrSrv);
     }
    }
   else{
    pSrv=RegOffpD(arrSrv,nSrv);
    arrSrv.ptop->typeSrv=pbKeyVal->key.psec[0];
    ValToPortArg(&(pbKeyVal->val),&bHostPort,&bArg);
    StrToStr(pSrv->hostport,bHostPort);
    sockpSrvInit((Tsrv*)pSrv,&err);
    }
   if (err>CERR) ++nSrv;
   }
 //нет описаний Серверов
 if (arrSrv.busy<1) return CERRsrvNot;
 //Запускаем Сервера
 RegIter(arrSrv,pSrv) { mshJobStart(pSrv->thread,pthrSrv,pSrv); }
 return COutKodOk;
 }
/*
 * Обработка запросов к суперсерверу
 */
static int mshClntSup(void) {
 Tsrv *pSrv;
 int num,size;
 Terr err;
 char *p2,c2;
 //
 #ifdef DEB_LOG
  write(logflin,supSrv.buf.sbuf.psec,supSrv.buf.sbuf.size);
 #endif
 size=supSrv.buf.busy;
 if (size) {
  c2=(size>1)?supSrv.buf.psec[1]:'\0';
  p2=(size>1)?supSrv.buf.psec + 1:NULL;
  switch (supSrv.buf.psec[0]){
   //остановить сервера
   case CsrvCodStop :
    //остановить все сервера
    if ((size<2) || (c2==CsrvCodStopALL)){
     RegIter(arrSrv,pSrv) {pSrv->pfchangedstatSrv=NULL;stoptcpSrv(pSrv);}
     mshJobYield;
     viewSrv();
     RegIter(arrSrv,pSrv) pSrv->pfchangedstatSrv=changedstatSrv;
     }
    //остановить один сервер
    else{
     //номер сервера
     num=strtoint(p2);
     if (num < arrSrv.busy){
      pSrv=RegOffpD(arrSrv,num);
      stoptcpSrv(pSrv);
      }
     }
    break;
   //рестарт серверов 
   case CsrvCodReStart :
    if ((size<2) || (c2==CsrvCodStopALL) ){
     //останавливаем запущенные сервера
     RegIter(arrSrv,pSrv) {
      pSrv->pfchangedstatSrv=NULL;
      if (pSrv->stat!=CsrvStatStop) stoptcpSrv(pSrv);
      }
     restarttcpSrv();
     mshJobYield;
     viewSrv();
     statChangeSrvs(pSrv);
     }
    //остановить один сервер
    else{
     //перезагрузить Ini файл
     if ((err=iniReadFL(supSrv.pflIni,&(supSrv.iniPipe)))<CERR) return err;
     //номер сервера
     num=strtoint(p2);
     if (num < arrSrv.busy){
      pSrv=RegOffpD(arrSrv,num);
      //остановить сервер
      if (pSrv->stat!=CsrvStatStop) stoptcpSrv(pSrv);
      //найти порт в ini файле
      if ((err=getPortSrv(pSrv))<CERR) return err;
      //запускаем сервер
      sockpSrvInit((Tsrv*)pSrv,&err);
      if (err>CERR) mshJobStart(pSrv->thread,pthrSrv,pSrv);
      statChangeSrvs(pSrv);
      }
     }
    break;
   case CsrvCodGuideSrv :
    statusviewSrv;
    break;
   //Получить страницу клиентов сервера 
   case CsrvCodGuideClnt :
    //номер сервера
    num=strtoint(p2);
    if (num < arrSrv.busy){
     pSrv=RegOffpD(arrSrv,num);
     supSrv.viewCmd=CsrvCodGuideSrv;
     supSrv.pviewSrv=pSrv;
     viewClnt(pSrv);
     }
    break;
   //Обновить страницу клиентов сервера 
   case CsrvCodViewClnt :
    if ((supSrv.viewCmd==CsrvCodGuideSrv)&&(supSrv.pviewSrv) )
     viewClnt(pSrv);
    break;
   //Обновить страницу клиентов сервера 
   case CsrvCodStopClnt :
    if ((supSrv.viewCmd==CsrvCodGuideSrv)&&(supSrv.pviewSrv) ){
     pSrv=supSrv.pviewSrv;
     //остановить всех клиентов текущего сервера
     if ((size<2) || (c2==CsrvCodStopALL) ){
      pSrv->pfchangedstatSrv=NULL;
      stopClntALL(pSrv);
      mshJobYield;
      viewClnt(pSrv);
      pSrv->pfchangedstatSrv=changedstatSrv;
      }
     //остановить одного клиента текущего сервера
     else{
      //номер сервера
      num=strtoint(p2);
      if (num < arrSrv.busy){
       TsrvClnt *pClnt=pRegOffpD(pSrv,num);
       stopClnt(pSrv,pClnt);
       }
      }
     }
    break;
   case CsrvCodExit:
    //остановить сервера
    RegIter(arrSrv,pSrv) {pSrv->pfchangedstatSrv=NULL;stoptcpSrv(pSrv);}
    mshJobYield;
    //Exit
    arrSrv.codOut=CsrvStatStop;
    break;
   }
  }
 return COutKodOk;
 }
/*
 */
static int supSrvInit(void){
 struct sockaddr_in addr;
 socklen_t sa;
 __SOCKADDR_ARG addrArg;
 char *pstat;
// char s[64];
// GIOChannel *pChannel;
// GIOStatus st;
// GIOCondition con;
// gboolean isbuf;
 Terr err=COutKodOk;
 //суперсервер не описан запускаем по умолчанию
 //запускаем суперсервер
 BufInit(supSrv.buf);
 // sockpSrvInit((Tsrv*)(&supSrv),&err);   supSrv.protocol=CprotoNet;
 supSrv.stat=CsrvStatSocket;
 if ((supSrv.sctlisten=netSocket(CprotoNet) )<0) {
  supSrv.stat=CsrvStatSocketerr;
  return CERRScktFiled;
  }
 else {
  if ( supSrv.hostport.size==0) {
   addr.sin_addr.s_addr=htonl( INADDR_ANY );
   addr.sin_port=htons( CPortSupSrvDef );
   }
  else{
   err=getAdr(&(supSrv.hostport),&addr,pstat);
   if (err<CERR){
    return CERRScktFiled;
    }
   }
  }
 supSrv.stat=CsrvStatBind;
 if ((bind(supSrv.sctlisten,(struct sockaddr *)(&addr),sizeof(addr)))) {
  supSrv.stat=CsrvStatBinderr;
  return CERRPortFiled;
  }
 supSrv.stat=CsrvStatListen;
 if ((listen( supSrv.sctlisten,CsizeListen)) ) {
  supSrv.stat=CsrvStatListenerr;
  return CERRsrvListenFiled;
  }
 //
 supSrv.pviewSrv=NULL;
 supSrv.viewCmd=CsrvCodNULL;
 spinLockInit(supSrv.lock);
 //ждем соединениена этом порту
// struct sockaddr_in addr;
 while(arrSrv.codOut!=CsrvStatStop) {
  supSrv.sctClnt=accept(supSrv.sctlisten,(struct sockaddr *)(&addrArg),&sa);
  if (supSrv.sctClnt<0){
   err=errno;
   if ( (errno==EINTR)||(errno==ECONNABORTED)||(errno==EAGAIN) ) continue;
   err=CERRAccept;
   break;
   }
  //чтение команд суперсервера
 #ifdef DEB_LOG
 logflin=open("log/srvin.txt", O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
 logflout=open("log/srvout.txt", O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
 #endif
/*
  supSrv.pChannel=g_io_channel_unix_new(supSrv.sctClnt);
  isbuf=g_io_channel_get_buffered(supSrv.pChannel);
  g_io_channel_set_buffered(supSrv.pChannel,TRUE);
  g_io_channel_set_buffer_size(supSrv.pChannel,CbufSizeDef);
  con=g_io_channel_get_buffer_condition(supSrv.pChannel);
  */
  BufClear(supSrv.buf);
  //Отсылаем состояние серверов
  statusviewSrv;
  while (arrSrv.codOut!=CsrvStatStop) {
//   st=bufReadChannelMsg(supSrv.pChannel,&(supSrv.buf));  if (st==G_IO_STATUS_EOF) break;
   err=bufReadMsg(supSrv.sctClnt,&(supSrv.buf));
   if (err!=COutKodOk) break;
   if (supSrv.buf.busy){
    if ((err=mshClntSup())<CERR) {
 #ifdef DEB_LOG
  snprintf(s,63,"err=%i",err);
  write(logflin,s,strlen(s));
 #endif
     }
    }
   }
 #ifdef DEB_LOG
  snprintf(s,63,"err=%i",err);
  write(logflin,s,strlen(s));
 #endif
  supSrv.pviewSrv=NULL;
  supSrv.viewCmd=CsrvCodNULL;
  }
 spinLockFree(supSrv.lock);
 //канал потерян
 close(supSrv.sctClnt);
 supSrv.sctClnt=CsctNULL;
 #ifdef DEB_LOG
 close(logflin);
 close(logflout);
 #endif
 return err;
 }
//
static void supSrvFree(void){
 close(supSrv.sctlisten);
 supSrv.sctlisten=CsctNULL;
 close(supSrv.sctClnt);
 supSrv.sctClnt=CsctNULL;
 }
/*
 */
void vmAppFree();

void mshSrvFree(void){
 Tsrv *pSrv;
 //завершение приложения
 supSrvFree();
 RegIter(arrSrv,pSrv) tcpSrvFree(pSrv); 
 RegFree(arrSrv);
 vmAppFree();
 }

int mshSrv(char *pcname){
 int err=EXIT_SUCCESS;
 char *pnameApp="mSv";
 Tbstr bfl;
 char cfl[CsizePacthFl];
 //Инициализация Приложения
 vmAppInit(pnameApp);
 supSrv.pflIni=pcname;
 bfl.size=strlen(pcname);
 memCopy(cfl,pcname,bfl.size);
 bfl.psec=cfl;
 if ((err=iniDirInit(&bfl,pCmshDirIni,&(supSrv.iniPipe)))<CERR) return err;
 //Инициализация массива серверов
 RegInit(arrSrv,CsumSrvDef);
 if ((err=startalltcpSrv())>CERR) {
  //Инициализация sSrv
  arrSrv.codOut=CsrvStatRun;
  //запускаем суперсервер
  err=supSrvInit();
  }
 mshSrvFree();
 return err;
 }
