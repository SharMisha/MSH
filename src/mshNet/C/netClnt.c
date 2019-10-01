
#include "Net.h"
/* 
 * File:   netClnt.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 2 ноября 2016 г., 20:15
 */
#include "mshMem.h"
#include "Net.h"
#include "mshIni.h"

Tbstr* iniKeygetVal(TIni *pIni,Tbstr *pbsKey);
void castbStr(Tdata *pData,Tbstr *pbstrOut);
/*
 * поток клиента
 */
/*
 */
static int ClntInit( TClnt *pClnt,char *phost, int  Port,int (*pfwork)(TClnt *pClnt) ){
 Terr err;
 memNULL(pClnt);
 BufInit(pClnt->buf);
 pClnt->pfwork=pfwork;
 bzero(&(pClnt->addr),sizeof(pClnt->addr));
 pClnt->addr.sin_family=PF_INET;
 pClnt->addr.sin_port=htons(Port);
 inet_aton(phost,&(pClnt->addr.sin_addr) );
 pClnt->stat=CsrvStatSocket;
 if (( pClnt->sctrw=netSocket(CprotoNet) )<0) {
  pClnt->stat=CsrvStatSocketerr;
  return CERRScktFiled;
  }
 //соединение установлено
 if ((pClnt->pChannel=g_io_channel_unix_new(pClnt->sctrw))==NULL)
  {
  pClnt->stat=CsrvStatSocketerr;
  err=CERRChannelError;
  goto err;
  }
 pClnt->stat=CsrvStatConnect;
 if ((connect(pClnt->sctrw,(__CONST_SOCKADDR_ARG)(&(pClnt->addr)),sizeof(pClnt->addr))))
  {
  pClnt->stat=CsrvStatConnecterr;
  err=CERRconnectFiled;
  goto err;
  }
 return COutKodOk;
err: 
 close(pClnt->sctrw);
 pClnt->sctrw=CsctNULL;
 return err;
 }
/*
 */
GIOChannel* ClntGtkInit(TClnt *pClnt,char *phost, int  Port , int *perr ,int (*pfwork)(TClnt *pClnt) ){
// GError *perror=NULL;
// GIOStatus st;
 //
 if ( ( *perr=ClntInit(pClnt,phost,Port, pfwork ) )<CERR ) return NULL;
 //установить неблокируемый сокет
// int flags=fcntl(pClnt->sctrw,F_GETFL,O_NONBLOCK); fcntl(pClnt->sctrw,F_SETFL,flags | O_NONBLOCK);
// st=g_io_channel_set_flags(pIOChannel,G_IO_FLAG_NONBLOCK,&perror);
// (void)g_io_channel_set_encoding(pIOChannel,NULL, &perror);
// g_io_channel_set_buffered(pIOChannel,FALSE);
// g_io_channel_set_buffered(pIOChannel,TRUE);
 return pClnt->pChannel;
 }
/*
 */
GIOChannel* ClntGtkInitChannel(TClnt *pClnt,Tbstr *pbHostPort , int *perr ,int (*pfwork)(TClnt *pClnt)  ){
 //
 char host[pbHostPort->size+1];
 char *pR;
 int port;
 memCopy(host,pbHostPort->psec,pbHostPort->size);
 host[pbHostPort->size]=Cvm0;
 if ((pR=strFindChar(host,CdelemiterHostPort))) {
  *pR=Cvm0;
  ++pR;
  port=StrToInt(pR);
  return ClntGtkInit(pClnt,host,port,perr,pfwork);
  }
 //
 *perr=CERRvmIniNot;
 return NULL;  
 }
/*
 */
void ClntFree(TClnt *pClnt){
 close(pClnt->sctrw);
 (void)g_io_channel_get_close_on_unref(pClnt->pChannel);
 pClnt->sctrw=CsctNULL;
 BufFree(pClnt->buf);
 }
/*
 * 
 */

void ClntGtkFree(TClnt *pClnt){ 
 ClntFree(pClnt); 
 }
/*
void* pthrClnt(void *pArg){
 TClnt* pClnt;
 int err;
 //
 pClnt=(TClnt*)pArg;
 do {
  pClnt->stat=CsrvStatRead;
  //читаем сообщение
  err=bufReadMsg(pClnt->sctrw,&(pClnt->buf));
  } while (err>CERR);
 //сеанс связи завершон 
 close(pClnt->sctrw);
 pClnt->sctrw=CsctNULL;
 pClnt->stat=CsrvStatFree;
 return NULL;
 }
int ClntInitjob(TClnt *pClnt,char *phost, int  Port,int (*pfwork)(TClnt *pClnt)){
 int err;
 if ( (err=ClntInit(pClnt,phost,Port,pfwork) )>CERR){
  //запускаем читающий процесс
  mshJobStart(pClnt->thread,pthrClnt,pClnt);
  }
 return err;
 }
 */
