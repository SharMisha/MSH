
/* 
 * File:   mshNet.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 26 октября 2016 г., 10:21
 */

#ifndef NET_H
#define NET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gdk/gdk.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcgiapp.h>
   
#include "mshArea.h"
#include "mshCnst.h"
#include "mshErr.h"
#include "mshTask.h"
#include "mshData.h"
    
#define	CmarkLen '#'

#define	ChostportSupSrvDef ":61960"
#define	CPortSupSrvDef 61960
#define	CbufSizeDef  512
    
#include "mshArea.h"
//состояние сервера
#define	CsrvStatWait  'W'
#define	CsrvStatSocket   'S'
#define	CsrvStatSocketerr 's'
#define	CsrvStatBind   'B'
#define	CsrvStatBinderr 'b'
#define	CsrvStatListen  'L'
#define	CsrvStatListenerr 'l'
#define	CsrvStatAccept 'A'
#define	CsrvStatAccepterr 'a'
#define	CsrvStatConnect  'C'
#define	CsrvStatConnecterr  'c'
#define CsrvStatRead 'R'
#define	CsrvStatRun  'U'
#define	CsrvStatstop 'p'
#define	CsrvStatStop 'P'
#define	CsrvStatReStart 'T'
#define	CsrvStatInit 'I'
#define	CsrvStatIniterr 'i'
#define	CsrvStatFree  'F'
#define	CsrvStatClear  'O'
#define	CsrvStatEnd 'E'

//команды суперсервера
 #define CsrvCodNULL  '0'
//стоп сервер
 #define CsrvCodStop  'P'
//стоп все сервера
  #define CsrvCodStopALL  '*'
 //стоп клиент
 #define CsrvCodStopClnt  'p'
 #define CsrvCodReStart  'R'
 #define CsrvCodGuideSrv 'S'
 #define CsrvCodGuideSrv1 's'
 #define CsrvCodGuideClnt 'C'
 #define CsrvCodGuideClnt1 'c'
 #define CsrvCodViewClnt 'K'
 #define CsrvCodExit  'X'
    
#define	CsizeListen 16
#define	CsumClnt 8

static char* CkeyHostPort="Srv";
#define CdelemiterHostPort ':'

//typedef FCGX_Stream *TpdivSrv;
    
typedef union uTdivSrv {
 Tdiv TCP; //основной порт связи по TCP
 FCGX_Stream *pWWW; //канал связи с браузером
 } TdivSrv;
 
typedef struct {
 _contbuf;
 uint isbusy; //свободно=0 или используется=1
 } TsrvBuf;
 
typedef struct {
 _contReg(TsrvBuf);
 TspinLock lock; // управление доступом к таблице заданий
 } TsecsBuf;
 
/*
 * Клиент клиентской программы
 */ 
#define _contClnt Tdiv sctrw;GIOChannel *pChannel; pthread_t thread ;void *pData; char stat
 
//#define _contClnt0 _contClnt; struct sockaddr_in addr; int (*pfwork)(struct stTClnt *pClnt)
 
typedef struct stTClnt {
 _contClnt;
 struct sockaddr_in addr;
 int (*pfwork)(struct stTClnt *pClnt);
 Tbuf buf; //буфер обмена
 } TClnt;

#define	CsctNULL -1
//#define	CprotoNet IPPROTO_TCP
#define	CprotoNet IPPROTO_SCTP

#define netSocket(iprotoNet) socket(PF_INET,SOCK_STREAM,iprotoNet)
//#define netSocketPrt netSocket(IPPROTO_SCTP)
//#define netSocketPrt netSocket(IPPROTO_TCP)

/*
#define sockpSrvInit(pSrv,err) ({struct sockaddr_in addr; err=COutKodOk;(pSrv)->stat=CsrvStatSocket;\
 if (((pSrv)->sctlisten=netSocket )<0) {err=CERRScktFiled;(pSrv)->stat=CsrvStatSocketerr;}\
 else { bzero(&addr,sizeof(addr)); addr.sin_family=PF_INET; addr.sin_port=htons( (pSrv)->port ); addr.sin_addr.s_addr=htonl( INADDR_ANY );\
 (pSrv)->stat=CsrvStatBind; if ((bind((pSrv)->sctlisten,(struct sockaddr *)(&addr),sizeof(addr))))\
 {err=CERRPortFiled;(pSrv)->stat=CsrvStatBinderr;}\
 else {(pSrv)->stat=CsrvStatListen; if ((listen( (pSrv)->sctlisten,CsizeListen)) )\
 {err=CERRsrvListenFiled;(pSrv)->stat=CsrvStatListenerr; } } } })
*/
typedef struct {
 uint32_t len;
 char cmark;
 } Tmsglen;
         
//typedef void Sigfunc(int);
//Sigfunc* signal(int signo,Sigfunc* func);
 
//TsecsBuf arrBuf;
/*
 * отправка сообщений
 */
//#define ClntSend(pClnt,pBuf) ClntSend(pClnt->sctrw,pBuf)
 
#define mshSrvGetPort(psPort,port) ({int j;port=0;for (j=0;j<(psPort)->size;j++)\
 if (((psPort)->psec[j]<'0')||((psPort)->psec[j]>'9')) break; else port=(port) * 10 + (psPort)->psec[j]-'0';})

#ifdef __cplusplus
}
#endif

#endif /* NET_H */

