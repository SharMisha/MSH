/*
 * File:   mshAPI.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 29 Октябрь 2014 г., 19:14
 */
#include <ftw.h>
#include "mshvmVm.h"
#include "mshvmTr.h"
#include "mshvmCnst.h"
//
void TrInit(Ttr *pTr);
void TrFree(Ttr *pTr);
void trCodamshCmd(Ttr *pTr);
void TrToMod(Ttr *pTr,TvmMod *pMod);
void PackpMod(TvmMod *pMod);
Terr writeDivMod(TvmMod *pMod,Ttr *pTr);
void loadDivMsh(Ttr *pTr);
Terr loadDivMod(Tstr *pname,TvmMod *pMod);
void mshVmDo(TvmDo *pDo, TdivStream *pdiv);
Terr vmAppInit(char *pnameApp);
void vmAppFree();
TvmDo* vmGetJobDo();
Tsize getLabelSecMod(TvmMod *pMod,Tstr *psLabel,Tsize iSec);
void tstvmprnTr(Ttr *pTr,TdivStream *pdiv);
void getProgNameFile(Tstr *pname,const char* pCmshDirMsh,int sizeDir,char* pfl);
//
//
//extern struct sttTarrextFunc *parrextFunc;

/*
 * Транслировать программу до модуля
 */
//static void trM(char *pcName,Ttr *pTr,TvmMod *pMod,char *pname,char *pnameInclude){
static void trM(char *pcName,Ttr *pTr,TvmMod *pMod){
 //инициализация библиотеки
 //загрузить из файла в буфер
 pCharToStr(pcName,pTr->bufIn.name);
 loadDivMsh(pTr);
 //трансляция буфера
 if (pTr->errTr.err>CERR){
  trCodamshCmd(pTr);
  if (pTr->errTr.err>CERR) {
   pvmModInit(pMod);
   TrToMod(pTr,pMod);
   }
  }
 //
 }
/*
 * M программу оттранслировать в Pi код
 *  int numstr; //транслируемая строка
 int numpos; //позиция трансляции
 Tstr name; // имя модуля
 Tstr nameInclude; //имя Include
 Terr err;

 */

Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
//int mshMtoVM(char *pcName,int *pnumstr,int *pnumpos,char *pname,char *pnameInclude){
int mshMtoVM(char *pcName,int *pnumstr,int *pnumpos,char *pbuferr){
 Ttr Tr;
 TvmMod Mod;
 TdivStream asmtxt;
 char pfl[CsizePacthFl];
 int err;
 //
 TrInit(&Tr);
 trM(pcName,&Tr,&Mod);
 *pnumstr=Tr.errTr.numstr;
 *pnumpos=Tr.errTr.numpos;
 if (Tr.errTr.err<CERR) {
  if (pbuferr){
   strCopy(pbuferr,Tr.cbufErr);
   }
  TrFree(&Tr);
  return Tr.errTr.err;
  }
 //записать модуль в файл Vm
 //
// areaInit(Mod.modpack,CvmsizeBufPckDef);
//#define	areaInit(Mod.modpack,Size)
 /*
 (Mod.modpack).psec
         =memNew(
         1024 //(CvmsizeBufPckDef)
//         * sizeof(*((Mod.modpack).psec))
          // sizeof(Tsize)
         );
 (Mod.modpack).size=CvmsizeBufPckDef;
 (Mod.modpack).busy=0;
*/ 
 PackInit(Mod.modpack);
 //
 err=writeDivMod(&Mod,&Tr);
 //записать модуль в файл Asm
#ifdef mshDEBAG
 getProgNameFile(&(Mod.name),pCmshDirAsm,CsizePartModDef,pfl);
 memNULL(&asmtxt);
 if ((asmtxt.pdiv=streamCreate(pfl))){
  tstvmprnTr(&Tr,&asmtxt);
  streamClose(asmtxt.pdiv);
  }
#endif
 //
 PackFree(Mod.modpack);
 TrFree(&Tr);
 return err;
 }
/*
 * выполнить модуль VM
 */
int mshVMrun(char *pcName,TerrTr *perrTr){
 TvmDo *pDo;
 Terr err;
 Tstr sName;
 TvmMod Mod;
// TApp App;
 char *pnameApp="rVm";
 //
 pCharToStr(pcName,sName);
 if ((err=vmAppInit(pnameApp))<CERR) return err;
 //создать блок выполнения pDo
 if ( (pDo=vmGetJobDo() )==NULL) return CERRdoFiled;
 pvmModInit(&Mod);
 pDo->pmod=&Mod;
 //загрузить модуль
 if ((err=loadDivMod(&sName,pDo->pmod))<CERR) return err;
 //выполнить блок pDo
 mshVmDo(pDo, NULL);
 if (Mod.labels.size) {arrFree(Mod.labels);}
 vmAppFree();
 *perrTr=pDo->pJob->errTr;
 return perrTr->err;
 }
int mshVMrunPos(char *pcName,int *pnumstr,int *pnumpos){
 TerrTr errTr;
 int err;
 //
 err=mshVMrun(pcName,&errTr);
 *pnumstr=errTr.numstr;
 *pnumpos=errTr.numpos;
 return err;
 }
/*
 * M программу оттранслировать в Pi код и выполнить
 */
int mshMrun(char *pcName,char *pcLabel,TerrTr *perrTr){
 Ttr Tr;
 TvmMod Mod;
 TvmDo *pDo;
 char *pnameApp="Msh";
 Tstr Lb;
// TApp App;
 //
 TrInit(&Tr);
 trM(pcName,&Tr,&Mod);
 *perrTr=Tr.errTr;
 if (Tr.errTr.err<CERR) goto err;
 if ((perrTr->err=vmAppInit(pnameApp))<CERR) goto err;
 //создать блок выполнения pDo
 if ( (pDo=vmGetJobDo() )==NULL) {
  perrTr->err=CERRdoFiled;
  goto err;
  }
 pDo->pmod=&Mod;
 if (pcLabel==NULL){
  pDo->curOff=0;
  }
 else{
  Lb.size=strlen(pcLabel);
  if (Lb.size>CCsizeStr) Lb.size=CCsizeStr;
  memCopy(Lb.psec,pcLabel,Lb.size);
  if ( (pDo->curOff=getLabelSecMod(&Mod,&Lb,0) )<0 ){
   perrTr->err=CERRvmLabelNotFound;
   pDo->curOff=0;
   goto err;
   }
  }
 //выполнить блок pDo
 mshVmDo(pDo, NULL);
 *perrTr=pDo->pJob->errTr;
 vmAppFree();
err: 
 TrFree(&Tr);
 return perrTr->err;
 }
/*
 int numstr; //транслируемая строка
 int numpos; //позиция трансляции
 Terr err;

 */
int mshMrunPos(char *pcName,char *pcLabel,int *pnumstr,int *pnumpos){
 TerrTr errTr;
 //
 errTr.err=mshMrun(pcName,pcLabel,&errTr);
 *pnumstr=errTr.numstr;
 *pnumpos=errTr.numpos;
 return errTr.err;
 }
/*
 * Подключение внешних библиотек
 */
/*
void msharrextFuncAdd(TextFun *pextFun){
 if (parrextFunc==NULL){
  parrextFunc=memType(struct sttTarrextFunc);
  parrInit(parrextFunc,CvmsumextFunDef);
  }
 parrPushpDan(parrextFunc,pextFun);
 }

void mshaddextLib(char *pnameExt,TpextFun mshExtFn){
 TextFun extFun;
 int n;
 //
 memNULL( &(extFun.pref) );
 n=strlen(pnameExt);
 if (n>lpref) n=lpref;
 extFun.pextfn=mshExtFn;
 memCopy(extFun.pref,pnameExt,n);
 msharrextFuncAdd(&extFun);
 if (parrextFunc==NULL){
  parrextFunc=memType(struct sttTarrextFunc);
  parrInit(parrextFunc,CvmsumextFunDef);
  }
 parrPushDan(parrextFunc,extFun);
 }
*/