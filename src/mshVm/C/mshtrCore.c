/*
 * File:   mshvmTrCore.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 14:54
 */
#include "mshvmTr.h"
#include "mshvmCnst.h"
#include "mshvmMacro.h"

int cmbFindInStr(const char *pStrCnst,char C);
/*
 * Пропустить пробелы, с учетом конца строки
 */
Terr trSpaceSkip(Ttr *pTr){
 //
 pTr->bufIn.cTerm=CtrTypeTerm0;
 while ( (pTr->bufIn.pcur<pTr->bufIn.pcurEnd) ){
  if (*(pTr->bufIn.pcur)==CCvmEndStr){
   addNumStr(pTr);
   pTr->bufIn.cTerm=CCvmEndStr;
   trSmbNext(pTr->bufIn.pcur);
   break;
   }
  if (!( trisSpaceBas(*(pTr->bufIn.pcur) ) ) ) break;
  pTr->bufIn.cTerm=CtrKodSpace;
  trSmbNext(pTr->bufIn.pcur);
  }
 return (pTr->bufIn.pcur<pTr->bufIn.pcurEnd)?COutKodOk:CWARendCmd;
 }
/*
Terr trSpaceSkip(Ttr *pTr){
 pTr->bufIn.cTerm=CtrTypeTerm0;
 while ( (pTr->bufIn.pcur<pTr->bufIn.pcurEnd)&&( trIsSpace(*(pTr->bufIn.pcur) )) ) {
  if (pTr->bufIn.cTerm==CtrTypeTerm0) pTr->bufIn.cTerm=CtrKodSpace;
  if (*(pTr->bufIn.pcur)==CCvmEndStr){
   addNumStr(pTr);
   pTr->bufIn.cTerm=CCvmEndStr;
   }
  trSmbNext(pTr->bufIn.pcur);
  }
 return (pTr->bufIn.pcur<pTr->bufIn.pcurEnd)?COutKodOk:CWARendCmd;
 }
*/
/*
 * найти искомый символ во входном буфере
 */
Terr trFindSmb(Ttr *pTr,char cRet1) {
 while(1) {
  trSmbNext(pTr->bufIn.pcur);
  if (pTr->bufIn.pcur>=pTr->bufIn.pcurEnd) return CWARendCmd;
  if (*(pTr->bufIn.pcur)==CCvmEndStr){
   addNumStr(pTr);
   }
  if (*(pTr->bufIn.pcur)==cRet1) {
   pTr->bufIn.pcur++;
   return COutKodOk;
   }
  }
 }

/*
 */
void skipComment(Ttr *pTr){
 //пропускаем комментарии
 //однострочный
 if (*(pTr->bufIn.pcur+1)==CvmCommentSlash){
  pTr->bufIn.pcur++;
  trSkipCommentSlash(pTr->bufIn.pcur,pTr->bufIn.pcurEnd);
  if (pTr->bufIn.pcur>=pTr->bufIn.pcurEnd) {
   pTr->errTr.err=CWARendCmd;
   return;
   }
  addNumStr(pTr);
  pTr->bufIn.pcur++;
  pTr->errTr.err=COutKodOk;
  pTr->bufIn.cTerm=CCvmEndStr;
  return;
  }
 //многострочный
 if (*(pTr->bufIn.pcur+1)==CvmCommentAs){
  pTr->bufIn.pcur+=2;
  while (( pTr->errTr.err=trFindSmb(pTr,CvmCommentAs) )==COutKodOk){
   if (*(pTr->bufIn.pcur)==CvmCommentSlash ) {
    pTr->bufIn.pcur++;
    pTr->bufIn.cTerm=CvmCommentSlash;
    goto finish;
    }
   }
  pTr->errTr.err=CERRtrParseCommentNotFinished;
  return;
  }
 pTr->errTr.err=CWARnoComm;
 // комментарии не опознаны
 return;
finish: 
 pTr->errTr.err=COutKodOk;
 }

/*
 * выделить терм из буфера
 */
/*
static Terr trFindTerm(Ttr *pTr,const char *psmbsEnd) {
 while((pTr->bufIn.pcur < pTr->bufIn.pcurEnd)&&((cmbFindInStr(psmbsEnd,*(pTr->bufIn.pcur)))==0)&&(smbIsNoCntrl(*(pTr->bufIn.pcur))) ) {trSmbNext(pTr->bufIn.pcur);}
 if (pTr->bufIn.pcur >= pTr->bufIn.pcurEnd) return CWARendCmd;
 return COutKodOk;
 }
*/
void trBufToTr(Ttr *pTr,Tbstr *pbufPrg,Tstr *pName){
 //
 if (pName==NULL) pTr->bufIn.name.size=0; else pTr->bufIn.name=*pName;
 pTr->errTr.numstr=1;
 pTr->errTr.numpos=0;
 pTr->bufIn.pBuf=pTr->bufIn.pcur=pTr->bufIn.pcurOld=pTr->bufIn.pbegStr=pbufPrg->psec;
 pTr->bufIn.pcurEnd=pTr->bufIn.pBuf + pbufPrg->size;
 pTr->bufIn.cTerm=CtrTypeTerm0;
 }
/*
 * Выбрать константу из входного потока
 */
Terr trTermConst(Ttr *pTr){
 pTr->bufIn.cTerm=CtrTypeTermCnst;
// return trFindTerm(pTr,CtrEndCnstEnd);
 while(1){
  if (pTr->bufIn.pcur >= pTr->bufIn.pcurEnd) return CWARendCmd;
  if ( ((cmbFindInStr(CtrEndCnstEnd,*(pTr->bufIn.pcur)))==1)||(!(smbIsNoCntrl(*(pTr->bufIn.pcur)))) ) return COutKodOk;
  trSmbNext(pTr->bufIn.pcur);
  }
 return COutKodOk;
 }
/*
 * выделить число
*/
static Terr trTermNum(Ttr *pTr){
 int isPoint=0;
 char *pcIn;
 //
 pcIn=pTr->bufIn.pcur;
 while(pTr->bufIn.pcur < pTr->bufIn.pcurEnd) {
  if (trSmbIsFloatPoint(*(pTr->bufIn.pcur))){
   if ( ((pTr->bufIn.pcur+1) < pTr->bufIn.pcurEnd)&&(smbIsNoNumbe(*(pTr->bufIn.pcur+1))) ) break;
   if (isPoint) return CERRtrFormatNumFiled;
   isPoint=1;
   }
  else
   if (smbIsNoNumbe(*(pTr->bufIn.pcur))) break;
  trSmbNext(pTr->bufIn.pcur);
  }
 pTr->bufIn.cTerm=(isPoint)?CtrTypeTermFloat:CtrTypeTermInt;
 if (pTr->bufIn.pcur >= pTr->bufIn.pcurEnd)
  return CWARendCmd;
 //то наверно это константа
 if (smbIsNoCntrl(*(pTr->bufIn.pcur))) {
  pTr->bufIn.pcur=pcIn;
  return trTermConst(pTr);
  }
 return COutKodOk;
 }
/*
 * выделить идентификатор
 */
static Terr trTermID(Ttr *pTr){
 char *pcIn;
 Terr err=COutKodOk;
 //
 pTr->bufIn.cTerm=CtrTypeTermID;
 pcIn=pTr->bufIn.pcur;
 while((pTr->bufIn.pcur < pTr->bufIn.pcurEnd)&&((cmbFindInStr(CtrEndID,*(pTr->bufIn.pcur)))==0) ) {
  if (smbIsNoAlphaNumbe(*(pTr->bufIn.pcur))) goto err;
  trSmbNext(pTr->bufIn.pcur);
  //длинна индефикатора превышена, наверно это константа
  if ((pTr->bufIn.pcur - pcIn)>CCsizeStr) goto err;
  }
 if (pTr->bufIn.pcur >= pTr->bufIn.pcurEnd)
  err=CWARendCmd;
 return err;
err: 
 if (smbIsNoCntrl(*(pTr->bufIn.pcur))) {
  pTr->bufIn.pcur=pcIn;
  pTr->bufIn.pcur=pcIn;
  return trTermConst(pTr);
  }
 return err;
 }
/*
 * пропустить код операции
 */
/*
 * выбрать из входного массива очередной терм
 * курсор будет находиться за последним символом терма
 */
void trSelTerm(Ttr *pTr,char cEnd){
 char cTerm;
 //
beg: 
 pTr->errTr.err=COutKodOk;
 //пропускаем пробелы
 if ( ( pTr->errTr.err=trSpaceSkip(pTr) )!=COutKodOk ) {
//  if (pTr->errTr.err==CWARendCmd) pTr->errTr.err=CERRtrEndCmd;
  return;
  }
 //конец строки
 if (pTr->bufIn.cTerm==CCvmEndStr) return;
 //код завершения
 if (pTr->bufIn.cTerm==cEnd) return;
 //пропускаем комментарии
 if (*(pTr->bufIn.pcur)==CvmCommentSlash){
  skipComment(pTr);
  //если комментарий закончен
  if (pTr->bufIn.cTerm==CCvmEndStr) return;
  if (pTr->errTr.err==COutKodOk) goto beg;
  }
 pTr->bufIn.pcurOld=pTr->bufIn.pcur;
 //символ является термом
 if ((cmbFindInStr(CtrTrm,*(pTr->bufIn.pcur)))==1){
  pTr->bufIn.cTerm=*(pTr->bufIn.pcur);
  goto finish;
  }
 cTerm=*(pTr->bufIn.pcur);
 //далее следует константа в кавычках
 if ((cTerm==CCtrKodKav1)||(cTerm==CCtrKodKav2)){
  if (( pTr->errTr.err=trFindSmb(pTr,cTerm) )!=COutKodOk) return;
  pTr->bufIn.cTerm=CCtrKodKav1;
  pTr->errTr.err=COutKodOk;
  return;
  }
 //код операции
 if (smbIsOp(cTerm)){
  if (cTerm == CtrKodUsr) {
   pTr->bufIn.cTerm=CtrKodUsr;
   if ((strFindChar(pCvmOpUsrData,pTr->bufIn.pcur[1]))==NULL){
    pTr->errTr.err=CERRvmNotOpUsr;
    return;
    }
   pTr->bufIn.pcur +=2;
   //pTr->errTr.err=trTermID(pTr);
   return;
   }
  //(pTr->bufIn.pcur < pTr->bufIn.pcurEnd)
  if (cTerm == CCtrKodEqu) {
   if ((pTr->bufIn.pcur+1) >= pTr->bufIn.pcurEnd){
    pTr->errTr.err=CERRtrEndCmd;
    return;
    }
   if (*(pTr->bufIn.pcur+1) != CCtrKodEqu) {
    pTr->bufIn.cTerm=CCtrKodEqu;
    goto finish;
    }
   }
  pTr->bufIn.cTerm=CtrTypeTermOp;
  do {
   if ((pTr->bufIn.pcur) >= pTr->bufIn.pcurEnd) break;
   if ( (smbIsNoAnsi(*(pTr->bufIn.pcur))) || (!(smbIsOp2(*(pTr->bufIn.pcur)))) ) break;
   }while((trSmbNext(pTr->bufIn.pcur)) < pTr->bufIn.pcurEnd);
  pTr->errTr.err=COutKodOk;
  return;
  }
 if (smbIsNumbe(cTerm)) {pTr->errTr.err=trTermNum(pTr);return;}
 if (smbIsAlpha(cTerm)) {pTr->errTr.err=trTermID(pTr);return;}
 if (smbIsNoCntrl(cTerm)) {pTr->errTr.err=trTermConst(pTr) ;return;}
 //терм не опознан, считаем что он и есть терм
 pTr->bufIn.cTerm=*(pTr->bufIn.pcur);
 trSmbNext(pTr->bufIn.pcur);
finish: 
 trSmbNext(pTr->bufIn.pcur);
 pTr->errTr.err=COutKodOk;
 }

/*
 * перенести данные из буфера в Tdata
 */
void trBufToData(char cTerm,char *pcIn,char *pcEnd,Tarea *pArea,Tdata *pData){
 int n=pcEnd-pcIn;
 Tbstr bStr;
 char buf[CsizeData];
 //
 pDataTypeSubSetNULL(pData);
 if (cTerm==CtrTypeTermInt){
  memCopy(buf,pcIn,n);
  buf[n]='\0';
  pDataSetInt(pData);
  pData->intgr=StrToInt(buf);
  return;
  }
 else
  if (cTerm==CtrTypeTermFloat){
   memCopy(buf,pcIn,n);
   buf[n]='\0';
   pDataSetFloat(pData);
   StrToFloat(buf,pData->fnum);
   return;
   }
 if (n <= CCsizeStr){
  pDataSetStr(pData);
  pData->str.size=n;
  memCopy(pData->str.psec,pcIn,pData->str.size);
  return;
  }
 bStr.size=n;
 bStr.psec=pcIn;
 pStrTopArea(&bStr,pArea,pData);
 }
/*
 */
void trBufToID(char *pcIn,char *pcEnd,Tstr *pStr){
 pStr->size=pcEnd-pcIn;
 if (pStr->size > CCsizeStr) pStr->size=CCsizeStr;
 memCopy(pStr->psec,pcIn,pStr->size);
 }
/*
 * Найти Константу в таблице Констант
 */
TConst* trFindConst(Ttr *pTr,Tstr *pname){
 TConst *pcnst;
 //
 arrIter(pTr->arrCnst,pcnst) if (StrpEqu(pcnst->name,pname)==CMPeq) return pcnst;
 return NULL;
 }
/*
 * загрузка исходного файла с устройства
 */
//Terr loadDivMapFixed(Tstr *pname,TmapFixed *pMap,const char* pCmshDirMsh,const char* pCmshMagMsh,int sizeDir);
void getProgNameFile(Tstr *pname,const char* pCmshDirMsh,int sizeDir,char* pfl);
Terr mshProgReadToMapFixed(char *pfl,const char *pCmshMag,TmapFixed *pMap);

void loadDivMsh(Ttr *pTr){
// char pfl[CsizePacthFl];
 //
 getProgNameFile(&(pTr->bufIn.name),pCmshDirMsh,CsizePartModDef,pTr->cbufErr);
 pTr->errTr.err=mshProgReadToMapFixed(pTr->cbufErr,pCmshMagMsh,&(pTr->bufIn.map));
// if (pTr->errTr.err<CERR) {  strCopy(pTr->cbufErr,pfl);  }
 }
