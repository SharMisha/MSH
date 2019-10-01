/*
 * File:   mshCore.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 20 Август 2014 г., 12:07
 */

#include "mshDb.h"
#include "mshData.h"
#include "mshPgLong.h"
#include "mshErr.h"
#include "mshSmb.h"

Terr LongDelInd(Tlong *pLong,Tsize Off);
Terr castNorm(Tdata *pData);
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
int roundDataDbAlt(TDb *pDb, TarrData *pInds,int isAlt, int (*pfdata)(TtreeData *ptreeData) , void *pUser);
/*
 * извлечь строку из страницы длинных строк
 */
Tstr* LongpStrOfpData(Tdata *pData) {
 Tlong *pLong;
 if (pData->dbAdrLong.pmlong && pData->dbAdrLong.pmlong->psec) {
  pLong=pData->dbAdrLong.pmlong->psec + pData->dbAdrLong.adrLong;
  if (pLong->psec[pData->dbAdrLong.Off] > CZERO) return  (Tstr*)(pLong->psec + (pLong->psec[pData->dbAdrLong.Off]) );
  }
 return NULL;
 }
/*
 */
void fileDirPacht(Tbstr *pname,const char *pDir,char *pout){
 pname->psec[pname->size]=Cvm0;
 snprintf(pout,CsizePacthFl,"%c%c%s%c%s",CdelimKat,CdelimFile,pDir,CdelimFile,pname->psec);
 }

/*
 * сформировать имя файла для обращения к файлам дерева 
*/
void getPachtLibDir(Tstr *pname,const char *pRas,int sizeDir,char *pout,int sizeout){
 //
 pout[0]=CdelimKat;
 pout[1]=CdelimFile;
 if (pname->size<sizeDir)
  snprintf(pout+2,sizeout,"%s%c%s%c%s%c",pRas,CdelimFile,pname->psec,CdelimFile,pname->psec,CdelimFile);
 else{
  char pdir[CsizePartDef+1];
  memCopy(pdir,pname->psec,sizeDir);
  pdir[sizeDir]='\0';
  snprintf(pout+2,sizeout,"%s%c%s%c%s%c",pRas,CdelimFile,pdir,CdelimFile,pname->psec,CdelimFile);
  }
 }

/*
 * найти место нахождения строки
 */
void bStrGet(Tdata *pData,Tbstr* pbstrOut){
 Tstr *pstr;
 //
 pbstrClear(pbstrOut);
 switch (pDataSight(pData)) {
  case CDataObj :
  case CDataStr :
   pbstrOut->size=pData->str.size;
   pbstrOut->psec=pData->str.psec;
   break;
  case CDatapStr :
   if (pData->pstr) {
    pbstrOut->size=pData->pstr->size;
    pbstrOut->psec=pData->pstr->psec;
    }
   break;
  case CDatabStr :
   *pbstrOut=pData->bstr;
   break;
  case CDatapbStr :
   if (pData->pbstr) *pbstrOut=*(pData->pbstr);
   break;
  case CDataArea :
   if ((pstr=pareaStrOff(pData->areaOff.parea,pData->areaOff.off))){pStrTopbStr(pstr,pbstrOut);}
   break;
  case CDataLong :
   if (pData->datapmLong){
    if ((pstr=LongpStrOfpData(pData))) {pStrTopbStr(pstr,pbstrOut);}
    }
   break;
  }
 }
/* +
 *     сравнение 2х полей данных
 *             key1== key2 то CMPeq
 *             key1 > key2 то  CMPup
 *             key1 < key2 то  CMPdown
 */
Terr DataCmpNum(Tdata *pkey1, Tdata *pkey2) {
 char tp1, tp2;
 tp1 = pDataSight(pkey1);
 tp2 = pDataSight(pkey2);
//  * если хотя бы один из ключей число
 if (TypeIsInt(tp1)) {
  if (TypeIsInt(tp2))
   return ((pkey1->intgr) == (pkey2->intgr)) ? CMPeq : ((pkey1->intgr > pkey2->intgr) ? CMPup : CMPdown);
  if (tp2 == CDataFloat)
   return (pkey1->intgr == pkey2->fnum) ? CMPeq : (pkey1->intgr > pkey2->fnum) ? CMPup : CMPdown;
  return CMPdown;
  }
 if (tp1 == CDataFloat) {
  if (TypeIsInt(tp2)) return (pkey1->fnum == pkey2->intgr) ? CMPeq : (pkey1->fnum > pkey2->intgr) ? CMPup : CMPdown;
  if (tp2 == CDataFloat) return (pkey1->fnum == pkey2->fnum) ? CMPeq : (pkey1->fnum > pkey2->fnum) ? CMPup : CMPdown;
  return CMPdown;
  }
 if (tp2 <= CDataMaskInt) return CMPup;
 //  *   оба ключа строки ??? могут быть не форматные данные сравнивать побайтно ?
 return CMPnull;
 }
/**/
Terr DataCmp(Tdata *pkey1, Tdata *pkey2) {
 Terr err;
 if ((err=DataCmpNum(pkey1,pkey2)) != CMPnull) return err;
//  *   оба ключа строки ??? могут быть не форматные данные сравнивать побайтно ?
 Tbstr bkey1,bkey2;
 bStrGet(pkey1,&bkey1);
 bStrGet(pkey2,&bkey2);
 return (StrCmrStr(bkey1,bkey2));
 }
/*
 * сравнить строку с данным
 */
Terr DataCmpStr(Tstr *pskey1, Tdata *pkey2) {
 char tp2;
 Tbstr *pbkey2;
 //
 tp2 = pDataSight(pkey2);
 //  * если хотя бы один из ключей число
 if (tp2 < CDataStr) return CMPup;
 if ((tp2 == CDatabStr)||(tp2 == CDatapbStr)) {
  pbkey2=(tp2 == CDatabStr)?&(pkey2->bstr):pkey2->pbstr;
  return pStrCmrpStr(pskey1,pbkey2);
  }
 Tbstr bkey2;
 bStrGet(pkey2,&bkey2);
 return (pStrCmrStr(pskey1,bkey2));
 }
/*
 * сравниваются только однобайтные символы
 */
int cmbFindInStr(const char *pStrCnst,char C) {
 return ((strFindChar(pStrCnst,C))==NULL)?0:1;
 /*  --------
 int d;
 //
 for ( pC=(char*)pStrCnst;*pC; ){
  if (((d=trSmbType(*pC))==1)&&(*pC==C)) return 1;
  pC +=d;
  }
 return 0;
 */
 }
/*
 * выбрать строку из массива строк
 */
int equCnst(char *pcIn,char *pcEnd,char *parrStr[]){
 int n,j;
 const char *pCnst;
 //
 n=pcEnd-pcIn;
 for (j=0;strlen(parrStr[j]);j++){
  if (strlen(parrStr[j])==n) {
   pCnst=parrStr[j];
   if (memCompare(pcIn,pCnst,n)==0) return j;
   }
  }
 return COutFound;
 }
/*
 * Добавляем Интервал времени
 */
void vmAddTime(TintNum add,struct timespec *ptm){
 struct timeval now;
 gettimeofday(&now,NULL);
 ptm->tv_sec=now.tv_sec;
 ptm->tv_nsec=(now.tv_usec +  add) * 1000;
 if (ptm->tv_nsec>1000000) {++(ptm->tv_sec);ptm->tv_nsec-=1000000;}
 }
/*
 *  удалить директорию
 */
int mshDirDelete(char *pDir){ return (divDeleteDir(pDir)); }
/*
 * рекурсивный обход поля Tdata
 */
Terr StrToDataOrLong(TmapLong *pmLong,Tdata *pDataSet,Tbstr *pStrIn);

void recusionpData(Tdata *pData,void *pUser,void fp(Tdata *pD,void *pUser) );

void recusionpDataArr(Tdata *pData,void *pUser,void fp(Tdata *pD,void *pUser) ){
 Tbstr bStr;
 Tdata *pB,*pE;
 //
 if (pDataIsList(pData)){
  bStrGet(pData,&bStr);
  pB=(Tdata*)(bStr.psec);
  pE=pB+bStr.size;
  for(;pB<pE;++pB) recusionpData(pB,pUser,fp);
  }
 }

void recusionpData(Tdata *pData,void *pUser,void fp(Tdata *pD,void *pUser) ){
 if (pDataIsNoNULL(pData)){
  fp(pData,pUser);
  recusionpDataArr(pData,pUser,fp );
  }
 }
/*
 * данные из pDataSet разместить в длинных страницах
 */
/*
static void moveTohisSub(Tdata *pDataSet,void *pUser){
 Tbstr bStr;
 TmapLong *pmLong=(TmapLong*)pUser;
 Tdata *pB,*pE;
 //
 if (pDataIsOut(pDataSet)) {
  if ((pDataIsLong(pDataSet))&&(pDataSet->datapmLong == pmLong)) return;
  bStrGet(pDataSet,&bStr);
  StrToDataOrLong(pmLong,pDataSet,&bStr);
  if (pDataIsList(pDataSet)) {
   pB=(Tdata*)(bStr.psec);
   pE=pB+bStr.size;
   for(;pB<pE;++pB) moveTohisSub(pB,pUser);
   }
  }
 }
//Terr setStrToLongPg(TmapLong *pmLong,Tdata *pDataSet){
void moveTohis(TmapLong *pmLong,Tdata *pDataSet){
 recusionpData(pDataSet,pmLong,moveTohisSub );
 }
*/
/*
 * Все данные перенести на страницу длинных строк
 */
void moveTohis(TmapLong *pmLong,Tdata *pData){
 Tbstr bStr;
 Tdata *pB,*pE;
 //
 if (pDataIsOut(pData)) {
//  if ((pDataIsLong(pData))&&(pData->datapmLong == pmLong)) return;
  bStrGet(pData,&bStr);
  StrToDataOrLong(pmLong,pData,&bStr);
  if (pDataIsList(pData)) {
   pB=(Tdata*)(bStr.psec);
   pE=pB+bStr.size;
   for(;pB<pE;++pB) moveTohis(pmLong,pB);
   }
  }
 }
/*
 * pArrToLong(pmLong,pArr);
*/
void pArrToLong(TmapLong *pmLong,TarrData *pArr){
 Tdata *pD;
 //
 parrIter(pArr,pD) moveTohis(pmLong,pD);
 }
/*
 * в данных установить ссылки на текущую базу
 */
void pdataLongRef(TmapLong *pmLong,Tdata *pData){
 Tstr *pStr;
 Tdata *pB,*pE;
 //
 if (pDataIsLong(pData)) {
  pData->datapmLong=pmLong;
  if (pDataIsList(pData)){
   pStr=LongpStrOfpData(pData);
   pB=(Tdata*)(pStr->psec);
   pE=pB+pStr->size;
   for(;pB<pE;++pB) pdataLongRef(pmLong,pB);
   }
  }
 }
/*
 * в массиве установить ссылки на текущую базу
 */
void pListdataLongRef(TmapLong *pmLong,TarrData *pArr){
 Tdata *pB;
 if (pArr->busy)
  for (pB=pArr->psec;pB <= pArr->ptop; ++pB) pdataLongRef(pmLong,pB);
 }
/*
 * в Tval установить ссылки на текущую базу данных
 */
void pValLongRef(TmapLong *pmLong,Tval *pVal){
 pdataLongRef(pmLong,&(pValKey(pVal)));
 pdataLongRef(pmLong,&(pValData(pVal)));
 }
/*
 * массив свернуть в данные или разместить в области pArea
 */
void pArrTopData(TarrData *pArr, Tdata *pDataOut, Tarea *pArea){
 Tbstr bsBf;
 if (pArr->busy<1){
  pDataSetNULL(pDataOut);
  return;
  }
 if (pArr->busy<2){
  *pDataOut=*(pArr->psec);
  return;
  }
 bsBf.psec=(char*)(pArr->psec);
 bsBf.size=pArr->busy * sizeof(Tdata);
 pareaPushStr(pArea,&bsBf,pDataOut->areaOff.off);
 pDataOut->areaOff.parea=pArea;
 pDataSetArea(pDataOut);
 pDataListSet(pDataOut);
 }
/*
 * выбрать следующую строку в pArea
 */
Tstr* mshpAreaNextpStr(Tarea *pArea,Tstr *pStr){
 int n;
 //
 if (pArea->busy<1) return NULL;
 if (pStr==NULL) return (Tstr*)(pArea->psec);
 n=mshDivUp(pStr->size,sizeof(Tsize));
 if ((Tsize*)(pStr->psec) + n >= pArea->psec + pArea->busy ) return NULL;
 return (Tstr*)(pStr->psec + n);
 }
/*
 * развернуть подмассивы в линейный массив
 */
void expandArrToArr(TarrData* parIn,TarrData* parTemp);

void expandDataToArr(Tdata *pD,TarrData* parTemp){
 TarrData arD;
 Tbstr bStr;
 //
 if (pDataIsList(pD)){
  bStrGet(pD,&bStr);
  StrToArrData(bStr,arD);
  expandArrToArr(&arD,parTemp);
  }
 else{
  parrPushpDan(parTemp,pD);
  }
 }
void expandArrToArr(TarrData* parIn,TarrData* parTemp){
 Tdata *pD;
 //
 if (parIn->busy) for(pD=parIn->psec;pD<=parIn->ptop;++pD) expandDataToArr(pD,parTemp);
 }
/*
//????????????????????
TarrData* expandArrData(TarrData* parIn,TarrData* parTemp){
 Tdata *pD;
 TarrData arD;
 //
 if (parIn->busy==0) return parIn;
 for(pD=parIn->psec;pD<=parIn->ptop;++pD)
  if (pDataIsList(pD)){
   arD=*parIn;
   arD.busy=pD-parIn->psec;
   arrsetptop(arD);
   parrCopy(parTemp,&arD);
   arD=*parIn;
   arD.psec=pD;
   arD.busy=parIn->ptop-pD+1;
   expandArrToArr(&arD,parTemp);
   return parTemp;
   }
 return parIn;
 }
*/
/*
 * массив pArr разместить в области pArea
 */
void pArrToArea(TarrData *pArr,Tarea *pArea){
 Tdata *pD;
 Tbstr bStr;
 TarrData arD;
 //
 if (pArr->ptop)
  for(pD=pArr->psec;pD<=pArr->ptop;++pD)
   if ( pDataIsList(pD) ) {
    bStrGet(pD,&bStr);
    StrToArrData(bStr,arD);
    pArrToArea(&arD,pArea);
    }
 }
 /**/
void pDataToList(Tdata *pData,Tdata *pPush,Tarea *pArea){
 Tbstr bStr;
 int Count;
 Tstr *pS;
 Tdata Data;
 //
 (void)castNorm(pPush);
 if (pData==NULL) return;
 if (pDataIsNULL(pData)){
  *pData=*pPush;
  return;
  }
 if (pDataIsList(pData)){
  bStrGet(pData,&bStr);
  }
 else{
  bStr.psec=(char*)(pData);
  bStr.size=sizeof(Tdata);
  }
 //-------------------------
 StrTopDatapArea(bStr,pArea,&Data);
 DataListSet(Data);
 Count=mshDivUp(sizeof(Tdata),sizeof(*(pArea->psec)) );
 pareaWide(pArea,Count);
 memCopy(pArea->psec + pArea->busy,pPush,sizeof(Tdata));
 pArea->busy += Count;
 pS=pareaStrOff(Data.areaOff.parea,Data.areaOff.off);
 pS->size +=sizeof(Tdata);
 *pData=Data;
 }
/*

void ListTopData(Tdata *pData,Tdata *pPop){
 Tbstr bStr;
 //
 if (pDataIsList(pData)){ //???
  bStrGet(pData,&bStr);
//  *pPop=*((Tdata*)(bStr.psec));
  pPop->bstr.psec=bStr.psec;
  pPop->bstr.size=bStr.size;
  pDataSetbStr(pPop);
  pDataListSet(pPop);//???
  }
 else
  *pPop=*pData;
 }
*/
