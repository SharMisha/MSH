
/* 
 * File:   mshSrv.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 7 ноября 2016 г., 9:33
 */

#ifndef MSHSRV_H
#define MSHSRV_H

#ifdef __cplusplus
extern "C" {
#endif
    
//#include <gdk/gdk.h>
/*    
#include "../mshCore/H/mshArea.h"
#include "../mshCore/H/mshTask.h"
#include "../mshCore/H/mshmData.h"
#include "../mshCore/H/mshNet.h"
#include "../mshCore/H/mshMod.h"
#include "../mshCore/H/mshIni.h"
*/
#include "Net.h" //"mshNet.h"
//#include "../mshCore/H/mshNet.h"
#include <fcgiapp.h>
    
#include "mshDb.h"
#include "mshSmb.h"
#include "mshvmCnst.h"
/*    
#include "../mshVm/H/mshVmType.h"
#include "../mshVm/H/mshVmCore.h"    
#include "../mshVm/H/mshTr.h"
*/
#include "mshvmVm.h"    

#define	CsumSrvDef  1
#define	CsumarrdataClntDef  2
#define CsumClntDoArgDef 8

#define	CsrvIsModNo  0
#define	CsrvIsMod    1
 
#define	CsrvModUnDef    '-'
#define	CCsrvEndArg '\n'
//
#define	CnameDbPar 'A'
//типы Серверов на вход поступают
/*
0sup=61960
1msh=61961
2vmm=61962
3pim=61964
4usr=61970
#5hmm=61965
#6dbm=61963
 */
//команды суперсервера
#define	CsrvTypeSup  '0'
//   команды Msh
#define	CsrvTypeMsh  '1'
//точка входа в программу VM  mod.label
#define	CsrvTypeVmm  '2'
//команды виртуальной машины Msh
#define	CsrvTypePim  '3'
//сообщение обрабатывается VM программой пользователя
#define	CsrvTypeUsr  '4'
//сообщение WWW обрабатывается VM программой пользователя
#define	CsrvTypeUsrWWW  '5'
//команды Msh при отладке
#define	CsrvTypeDbm  '6'
//специальный HTML файл
#define	CsrvTypeHmm  '7'

/*
 * Клиент серверной программы
 */ 
typedef struct {
 TvmDo *pDo;
 TvmMod Mod;
 } TdataClnt;
// 
typedef struct {
 _contReg(TdataClnt);
 } TsecsdataClnt;

struct stTsrv;

typedef struct {
 _contClnt;
 FCGX_Request request;
 TsrvBuf *pbuf; //ссылка на полученный буфер обмена
 long idClnt; //идентификатор клиента
 struct stTsrv *pSrv; //ссылка на сервер
 uint isbusy; //свободно=0 или используется=1
 int pid;
 int ppid;
 } TsrvClnt;
 
#define _contSrv0 Tdiv sctlisten; Tdiv sctClnt;GIOChannel *pChannel; Tstr  hostport;Terr err; char stat
 
#define _contSrv _contReg(TsrvClnt); _contSrv0;\
 int protocol ; TsecsBuf arrBuf;\
 int (*pfwork)(TsrvClnt *pClnt); int (*pfinit)(TsrvClnt *pClnt); int (*pffree)(TsrvClnt *pClnt); int (*pfclear)(TsrvClnt *pClnt);\
 void (*pfchangedstatSrv)(struct stTsrv *pSrv,TsrvClnt *pClnt)
 
typedef struct stTsrv {
 _contSrv;
 pthread_t thread;
 TsecsdataClnt arrdataClnt;
 TvmMod modTCP;
 Tbuf htmlheder;
 int modIs;
 int id;
 int pid;
 int ppid;
 int isWWW;
 char typeSrv;
 } Tsrv;

typedef struct {
 _contReg(Tsrv);
 char codOut; //код вызванного сервера
 } TsecsSrv;
 
typedef struct {
 _contSrv0;
 Tbuf buf;
 TIni iniPipe;
 char *pflIni;
 Tsrv *pviewSrv;
 char viewCmd;
 TspinLock lock; // управление доступом к таблице заданий
 } TsrvSup;

#define	srvBufSend(div,Buf,bsBf) AreaTobStr(Buf,bsBf); bufSend(div,&bsBf )
//#define	srvsendMsg(pChannel,Buf,bsBf) AreaTobStr(Buf,bsBf); sendMsg(pChannel,&bsBf)
#define srvClntBeg(pClnt,pSrv) (pClnt)->sctrw=(pSrv)->sctClnt;(pClnt)->stat=CsrvStatInit;(pClnt)->pbuf=NULL
 
TsrvSup supSrv; 
TsecsSrv arrSrv;
//TApp App;
// #define DEB_LOG
 #ifdef DEB_LOG
  Tdiv logflin,logflout;
 #endif
 
#define MisNum(c) (((c)>='0')&&((c)<='9'))||(((c)>='A')&&((c)<='F'))
#define MsetChar(c) ((c)<='9')?(c) - '0':(c) - 'A' + 10

#ifdef __cplusplus
}
#endif

#endif /* MSHSRV_H */
