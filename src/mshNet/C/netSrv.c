
/* 
 * File:   netSrv.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 26 октября 2016 г., 10:28
 */

#include "Net.h"

TsrvBuf* bufGet(TsecsBuf *psecsBuf);
//int bufReadMsg(Tdiv div,Tbuf *pBuf);
//GIOStatus bufReadChannelMsg(GIOChannel *pChannel,Tbuf *pBf);
int bufReadMsg(Tdiv div,Tbuf *pBf);
void secsBufInit(TsecsBuf *psecsBuf);
void secsBufClear(TsecsBuf *psecsBuf);
void secsBufFree(TsecsBuf *psecsBuf);
/*
 * серверный Клиент
 */
//
/*
typedef struct {
 Tdiv sctrw;
 pthread_t thread;
 char stat;
 TsrvBuf *pbuf; //ссылка на полученный буфер обмена
 long idClnt; //идентификатор клиента
 struct stTsrv *pSrv; //ссылка на сервер
 void *pData; //ссылка на данные конкретного приложения
 uint isbusy; //свободно=0 или используется=1
 } TsrvClnt;
 */
void sockpSrvInit(Tsrv *pSrv,Terr *perr) {
 struct sockaddr_in addr;
 //
 *perr=COutKodOk;
 pSrv->stat=CsrvStatSocket;
 if ((pSrv->sctlisten=netSocket(pSrv->protocol) )<0) {
  *perr=CERRScktFiled;
  pSrv->stat=CsrvStatSocketerr;
  }
 else {
  bzero(&addr,sizeof(addr));
  addr.sin_family=PF_INET;
  addr.sin_port=htons( pSrv->port );
  addr.sin_addr.s_addr=htonl( INADDR_ANY );
  pSrv->stat=CsrvStatBind;
  if ((bind(pSrv->sctlisten,(struct sockaddr *)(&addr),sizeof(addr)))) {
   *perr=CERRPortFiled;
   pSrv->stat=CsrvStatBinderr;
   }
  else {
   pSrv->stat=CsrvStatListen;
   if ((listen( pSrv->sctlisten,CsizeListen)) ) {
    *perr=CERRsrvListenFiled;
    pSrv->stat=CsrvStatListenerr;
    }
   }
  }
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
 return NULL;
 }
/*
 * Инициализация клиента
 * 
 */
#define srvClntBeg(pClnt,pSrv) (pClnt)->sctrw=(pSrv)->sctClnt;(pClnt)->stat=CsrvStatInit;(pClnt)->pbuf=NULL

static void srvClntInit(Tsrv *pSrv,TsrvClnt *pClnt,long numClnt){
 objSetBusy(pClnt);
 srvClntBeg(pClnt,pSrv);
 pClnt->pSrv=pSrv;
 pClnt->idClnt=numClnt;
 pClnt->pData=NULL;
 if (pSrv->pfinit) pSrv->pfinit(pClnt);
 }
/**/
static void srvClntClear(TsrvClnt *pClnt){
 objSetBusy(pClnt);
 srvClntBeg(pClnt,pClnt->pSrv);
 if (pClnt->pSrv->pfclear) pClnt->pSrv->pfclear(pClnt);
 }
/**/
static void srvClntFree(TsrvClnt *pClnt){
 if (pClnt->pSrv->pffree) pClnt->pSrv->pffree(pClnt);
 //клиент обслуживает канал
 if ( (objIsBusy(pClnt))  && (pClnt->sctrw!=CsctNULL) ) close(pClnt->sctrw);
 }
/*
 * Сервер
 */
Terr srvInit(Tsrv *pSrv, int  Port
  ,int (*pfwork)(TsrvClnt *pClnt),int (*pfinit)(TsrvClnt *pClnt),int (*pffree)(TsrvClnt *pClnt),int (*pfclear)(TsrvClnt *pClnt)
  ,void (*pfchangedstatSrv)(Tsrv *pSrv,TsrvClnt *pClnt)){
 Terr err;
 //
 pSrv->port=Port;
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
 * Сервер
 */
Terr srvWWWInit(Tsrv *pSrv, int  Port
  ,int (*pfwork)(TsrvClnt *pClnt),int (*pfinit)(TsrvClnt *pClnt),int (*pffree)(TsrvClnt *pClnt),int (*pfclear)(TsrvClnt *pClnt)
  ,void (*pfchangedstatSrv)(Tsrv *pSrv,TsrvClnt *pClnt)){
 char pach[32];
 //
 snprintf(pach,31,"%c,%i,%c",CdelemiterHostPort,Port,Cvm0);
 pSrv->port=Port;
 FCGX_Init(); //инициализация библилиотеки
 printf("Lib is inited\n");        
 pSrv->sctlisten = FCGX_OpenSocket(pach, 20); //открываем новый сокет 
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
 return COutKodOk;
 }
//
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
/**/
void stopClnt(Tsrv *pSrv,TsrvClnt *pClnt){
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
void stopClntALL(Tsrv *pSrv){
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
void stopSrv(Tsrv *pSrv){
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
//
void srvFree(Tsrv *pSrv){
 if (pSrv->stat!=CsrvStatFree){
  stopSrv(pSrv);
  secsBufFree( &(pSrv->arrBuf) );
  pRegFree(pSrv);
  pSrv->stat=CsrvStatFree;
  }
 }
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
 //
 sa=sizeof(addr);
 while(pSrv->stat!=CsrvStatstop) {
  pSrv->stat=CsrvStatAccept;
  if (pSrv->pfchangedstatSrv) pSrv->pfchangedstatSrv(pSrv,NULL);
  pSrv->sctClnt=accept(pSrv->sctlisten,(struct sockaddr *)(&addr),&sa);
  if (pSrv->stat==CsrvStatstop) break;
  if (pSrv->sctClnt<0){
   err=errno;
   if ( (errno==EINTR)||(errno==ECONNABORTED)||(errno==EAGAIN) ) continue;
   pSrv->stat=CsrvStatAccepterr;
   err=CERRAccept;
   break;
   }
  pSrv->stat=CsrvStatRead;
  if (pSrv->pfchangedstatSrv) pSrv->pfchangedstatSrv(pSrv,NULL);
  numClnt=(int long)(addr.sin_addr.s_addr);
  if ((pClnt=srvClntGet(pSrv,numClnt))==NULL) return COutKodOk;
  // новый клиент запустить его в новом потоке
  mshJobStart(pClnt->thread,pthrsrvClnt,pClnt);
  }
 if (pSrv->sctlisten!=CsctNULL) {close(pSrv->sctlisten);pSrv->sctlisten=CsctNULL;}
 //связь прослушивающего клиента потеряна 
 stopSrv(pSrv);
 pSrv->stat=CsrvStatStop;
 if (pSrv->pfchangedstatSrv) pSrv->pfchangedstatSrv(pSrv,NULL);
 return err;
 }
