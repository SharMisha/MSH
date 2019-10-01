
/* 
 * File:   mshvmMacro.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 декабря 2016 г., 18:07
 */

#ifndef MSHVMMACRO_H
#define MSHVMMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mshvmCnst.h"
#include "../../mshDb/H/mshSmb.h"
#include "../../mshDb/H/mshData.h"
/*
 * пропустить пробелы
*/
    
 /*
 * Является ли код Операцией '#&*_+-=|/\\?><'
 */
#define smbIsOp(c) cmbFindInStr(CvmOp,c)
#define smbIsOp2(c) cmbFindInStr(CvmOp2,c)

//  преобразовать к целому числу
#define	castDataIntOff(pData) (pDataIsInt(pData))?pData->intgr:(castDataInt(pData)<CERR)?0:pData->intgr

/*   ********************************
 *     типы VM
 */
//выбрать один из Арифметических типов
#define	vmselArf(pAr) (pDataIsInt(pAr))?pAr->intgr:pAr->fnum
//выполнить унарную операцию
#define	vmopArfUn(pAr,op) if (pDataIsInt(pAr)) pAr->intgr= op ((pAr)->intgr); else pAr->fnum= op pAr->fnum
//выполнить бинарную операцию
#define	vmopArf(pAr1,op,pAr2) if (pDataIsInt(pAr1)) ((pAr1)->intgr) op  (vmselArf(pAr2));\
 else ((pAr1)->fnum) op (vmselArf(pAr2))

#define	vmopArf1(pAr1,op,pAr2,out) if (((out=castDataArf(pAr1))>CERR)&&((out=castDataArf(pAr2))>CERR)){vmopArf(pAr1,op,pAr2);}
//
#define	vmopArf2(pAr1,op,pAr2,out) if (((out=castDataArf(pAr1))>CERR)&&((out=castDataArf(pAr2))>CERR)){\
    if (pDataIsInt(pAr1)) ((pAr1)->intgr) = ((pAr1)->intgr) op (vmselArf(pAr2));\
    else ((pAr1)->fnum)=((pAr1)->fnum) op (vmselArf(pAr2));}
    
#define	vmopArf3(pAr1,op,pAr2,out) out = (vmselArf(pAr1)) op (vmselArf(pAr2))
//
#define	vmopInt2(pAr1,op,pAr2,out) if (((out=castDataInt(pAr1))>CERR)&&((out=castDataInt(pAr2))>CERR)) ((pAr1)->intgr) = ((pAr1)->intgr) op ((pAr2)->intgr);\
/*
 * работа с модулями
 */
#define	vmGetpDataFromMod(pDo,pCmd) pDo->pmod->pdata + MvmpCmdOff(pCmd) - 1
#define	vmGetDataFromMod(pDo,pCmd) pDo->pmod->pdata[MvmpCmdOff(pCmd) - 1]
    
#define	mshVmDLBpath(pini,pname) mshIniGetPath(pini,(char*)((pname)->str.sec),(pname)->str.size)

#define	mshVmGLBdDbInit(pApp,pglb,pname) pglb->name=*pname;pglb->db.lock=mshLockResNew(pApp)

#define mshVmModInit(pvmMod) mshPackInit(pvmMod->modpack,CvmasizeModPackDef)
#define mshVmModFree(pvmMod) mshPackFree(pvmMod->modpack)

#define mshVmRunMod(pDo,perr) mshVmRunModGet(pDo,pDo->pJob->args.ptop->psec,perr)
//
#define	mshVmStyleIsList(Style) ((Style)==CStyleRefArrDo)||((Style)==CStyleRefArg)||((Style)==CStyleRefArrJob)||((Style)==CStyleRefArrApp)

/*
 *  стек Аргументов
 */
// записать значения
#define	argSetNULL(Arg) (Arg).busy=2;memNULLsum((Arg).psec,(Arg).busy);arrTop(Arg)
#define	argSetpMod(Arg,pMod) argSetNULL(Arg);(Arg).psec[0]=*pMod
#define	argSetMod(Arg,Mod) argSetNULL(Arg);(Arg).psec[0]=Mod
#define	argSetpLab(Arg,pLab) (Arg).psec[1]=*pLab
#define	argAddpArg(Arg,pDan) arrPushpDan(Arg,pDan)
#define	argAddArg(Arg,Dan) arrPushDan(Arg,Dan)
//
#define IdJobGet(pJob) pJob->iD
#define IdJobGetDo(pDo) IdJobGet(pDo->pJob)
#define pAppgetpJob(Off) (RegOffpD(App.arrJob,Off))
#define IdDoGet(pDo) pDo->iD
//
#define	pCmdInit(pCmd) (pCmd)->c4=0
#define	CmdInit(Cmd) (Cmd).c4=0
// 
#define	pCmdCmd(pCmd) (pCmd)->c1[0]
#define	CmdCmd(Cmd) (Cmd).c1[0]
 
#define	pCmdSub(pCmd) (pCmd)->c1[1]
#define	CmdSub(Cmd) (Cmd).c1[1]

#define	pCmdArg(pCmd) (pCmd)->c2[1]
#define	CmdArg(Cmd) (Cmd).c2[1]

#define	pCmdC4(pCmd) (pCmd)->c4
#define	CmdC4(Cmd) (Cmd).c4
    
#define	pCmdi(pCmd,i) (pCmd)->c1[i]

#define pCmdSubIsDoRet(pCmd) cmdDoIsRet(pCmdSub(pCmd))
#define pCmdSubIsDoIn(pCmd) cmdDoIsIn(pCmdSub(pCmd))
 //
#define	pCmdBusy(parrCmd) parrBusyNull(parrCmd)
#define	CmdBusy(arrCmd) arrBusyNull(arrCmd)
//
#define cmdSet(Cmd,cCmd,csubCmd,Arg) CmdCmd(Cmd)=cCmd; CmdSub(Cmd)=csubCmd;CmdArg(Cmd)=Arg
#define pcmdSet(pCmd,cCmd,csubCmd,Arg) pCmdCmd(pCmd)=cCmd; pCmdSub(pCmd)=csubCmd;pCmdArg(pCmd)=Arg

#define cmdPush(arrCmds,cCmd,csubCmd,Arg) CmdBusy(arrCmds);pcmdSet((parrCmds)->ptop,cCmd,csubCmd,Arg)
#define pcmdPush(parrCmds,cCmd,csubCmd,Arg) pCmdBusy(parrCmds);pcmdSet((parrCmds)->ptop,cCmd,csubCmd,Arg)

#define cmdPush4(arrCmds,C4) CmdBusy(arrCmds);(arrCmds).ptop->c4=C4
#define pcmdPush4(parrCmds,C4) pCmdBusy(parrCmds);(parrCmds)->ptop->c4=C4
    
#define cmdPushCnst(pTr,cCmd,csubCmd) pcmdPush(pTr->pcmds,cCmd,csubCmd | pTr->ptopStat->_sub.cnstCmd , pTr->ptopStat->_sub.cnstArg);\
  pTr->ptopStat->_sub.cnstCmd=0; pTr->ptopStat->_sub.cnstArg=0


#define	cmdPop(arrCmds) arrPop(arrCmds)
#define	pcmdPop(parrCmds) parrPop(parrCmds)

/*    
//#define	CmdSet(Cmd,cCmd,csubCmd,cCmd2,cCmd2Off,cCmd3,cCmd3Off) CmdCmd(Cmd)=cCmd; CmdSub(Cmd)=csubCmd;CmdiCmd(Cmd,2)=cCmd2; CmdiArg(Cmd,2)=cCmd2Off; CmdiCmd(Cmd,3)=cCmd3; CmdiArg(Cmd,3)=cCmd3Off
//
//#define	pCmdSet(pCmd,cCmd,csubCmd,cCmd2,cCmd2Off,cCmd3,cCmd3Off)  pCmdCmd(pCmd)=cCmd; pCmdSub(pCmd)=csubCmd;pCmdiCmd(pCmd,2)=cCmd2; pCmdiArg(pCmd,2)=cCmd2Off; pCmdiCmd(pCmd,3)=cCmd3; pCmdiArg(pCmd,3)=cCmd3Off
//#define	pCmdPush(parrCmds,cCmd,csubCmd,cCmd2,cCmd2Off,cCmd3,cCmd3Off) pCmdBusy(parrCmds);pCmdSet((parrCmds)->ptop,cCmd,csubCmd,cCmd2,cCmd2Off,cCmd3,cCmd3Off)
//#define	CmdPush(arrCmds,cCmd,csubCmd,cCmd2,cCmd2Off,cCmd3,cCmd3Off) CmdBusy(arrCmds); pCmdSet((arrCmds).ptop,cCmd,csubCmd,cCmd2,cCmd2Off,cCmd3,cCmd3Off)
// 
//#define	pCmdPushVal(parrCmds,Val) pCmdBusy(parrCmds);parrCmds->ptop->val=Val
//#define	pCmdSetVal(parrCmds,Val) if (parrCmds->ptop) parrCmds->ptop->val=Val

//#define	pCmdPop(parrCmds) parrPop(parrCmds)
 */
//
//#define	 vmCmdData(pDo,pCmd,pData) if ((GetpCmdRegDan(pCmd))==CRegData) {arrPopTop(pDo->datas,pData);}\
// else pData=((GetpCmdRegDan(pCmd))==CRegMod)?pDo->pmod->pdata + pCmdIndOff(pCmd):NULL

//#define	vmCmdInd(pDo,pCmd,parrData,pDbArr) vmCmdpDbArr(pDo,pCmd,parrData,pDbArr,2)

//#define vmpCmdpDataSetAlt(SubCmd,pData) if ((subCmdIsAlt(SubCmd))&&(pData)) pDataSetAlt(pData)
//
//#define trEndArgCmd(pTr,cCmd,csubCmd) if ((pTr->ptopStat)&&(pTr->ptopStat->_sub.cnstCmd)){cmdPushCnst(pTr,cCmd,csubCmd);}
/*
#define pTrToflowCmd(pTr,flowCmd) (flowCmd).pbegcmds=(flowCmd).pcurcmd=(pTr)->Cmds.psec;\
 (flowCmd).endCmnds=(pTr)->Cmds.busy
#define TrToflowCmd(Tr,flowCmd) (flowCmd).pbegcmds=(flowCmd).pcurcmd=(Tr).Cmds.psec;\
 (flowCmd).endCmnds=(Tr).Cmds.busy
 */
/*
*/
#define regpDataPop(pDo) arrPop(pDo->datas)

#define DosetVal(pDo) ValALLSetNULL((pDo)->pJob->val)

#define addNumStr(pTr) pTr->errTr.numstr++;pTr->errTr.numpos=0; pTr->bufIn.pbegStr=pTr->bufIn.pcur
#define subNumStr(pTr) pTr->errTr.numstr--
#define curNumStr(pTr) pTr->bufIn.pbegStr=pTr->bufIn.pcur
//CvmKodApp
#ifdef __cplusplus
}
#endif

#endif /* MSHVMMACRO_H */

