/* 
 * File:   mshLong.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 23 декабря 2016 г., 13:39
 */
#include "mshData.h"
#include "mshPgLong.h"
#include "mshMap.h"
#include "mshCnst.h"
#include "mshErr.h"

void bStrGet(Tdata *pData,Tbstr* pbstrOut);
/*
 * место занимаемое строкой на странице

 */

static Tsize LongSize(Tlong *pLong,Tsize Off){
 Tsize offLast,offEnd;
 offEnd=pLongOff(pLong,Off);
 if (offEnd==CZERO) return 0;
 offLast=Off;
 while (offLast-- > 0)
  if (pLong->psec[offLast]!=CZERO) return ((pLongOff(pLong,offLast)) - offEnd);
 return pLong->size - offEnd;
 }
//тестовый вызов
Tsize tstLongSize(Tlong *pLong,Tsize Off){return LongSize(pLong,Off);}
/*
 *  Удаить со страницы строку
 */
static Terr LongClear(Tlong *pLong,Tsize Off){
 Tsize *pb,*ps,size;
 //
 pgLongClearNULL(pLong);
 if (Off>=pLong->busy) return CWARdataNotFound;
 if (pLong->psec[Off]==CZERO) return COutKodOk;
 if (pLong->psec[Off]<CZERO) pLong->psec[Off]=-pLong->psec[Off];
 //строка последняя удалить ее
 if (Off==(pLong->busy-1)) {
  if (pLong->busy<2) {
   pLongClear(pLong);
   return COutKodOk;
   }
  pLong->busy--;
  }
 else {
  //начало области строк
  pb=pgLongpStr(pLong,pLong->busy-1);
  ps=pgLongpStr(pLong,Off);
  size=LongSize(pLong,Off);
  //удалить строку
  memMove(pb+size,pb,(ps-pb) * sizeof(Tsize));
  //удалить индекс
  pLong->psec[Off]=CZERO;
  //откорректировать смещение строк
  while (++Off < pLong->busy)
   if (pLong->psec[Off]!=CZERO)
    pLong->psec[Off]=(pLong->psec[Off]>CZERO)?pLong->psec[Off] +size:pLong->psec[Off] -size;
  }
 pgLongClearNULL(pLong);
 return COutKodOk;
 }
// версия для тестирования
Terr tstLongClear(Tlong *pLong,Tsize ind){return LongClear(pLong,ind);}
/*
 * сжать страницу удалив пустые области
 */
static Terr LongCompressed(Tlong *pLong){
 Terr err=COutFound;
 Tsize j;
 //
 if (pLong->busy<1) return err;
 for (j=pLong->busy-1;j>=0;--j)
  if (pLong->psec[j]<CZERO) {(void)LongClear(pLong,j);err=COutKodOk;}
 return err;
 }
// версия для тестирования
Terr tstLongCompressed(Tlong *pLong){return LongCompressed(pLong);}
/*
 *  Отметить строку как Удаленную
 */
Terr LongDelInd(Tlong *pLong,Tsize Off){
 if (Off>=pLong->busy) return CWARdataNotFound;
 if (pLong->psec[Off]<CZERO) return COutKodOk;
 pLong->psec[Off]=-pLong->psec[Off];
 pgLongClearNULL(pLong);
 return COutKodOk;
 }
/*
 *  Добавить строку в конец страницы
 */
static Tstr* LongAddEnd(Tlong *pLong,Tsize Size,Tuint32 *pOff,Terr *perr){
 Tstr *pStr=NULL;
 Tuint32 Off;
 //
 *perr=COutKodOk;
// if ((*perr=LongCompressed(pDb,pLong))<CERR) return NULL;
 if ((Size+1) > pLong->free) {*perr=CERRPgFull;return NULL;}
 //добавляем строку в конец
 Off=pLongOffBeg(pLong) - Size;
 pLong->psec[pLong->busy]=Off;
 pStr=(Tstr*)(pLong->psec + Off);
 *pOff=pLong->busy;
 pLong->busy++;
 pLong->free=Off - pLong->busy;
 return pStr;
 }
/*
 * вставить строку в определенную позицию
 */
static Tstr* LongIns(Tlong *pLong,Tsize ind,Tsize size,Tuint32 *pOff,Terr *perr){
 Tsize *pb,ib,i;
 Tsize *pStr=NULL;
 Tuint32 Off;
 int d;
 //
 if (size > pLong->size) {*perr=CWARdataNotFound;return NULL;}
 if (pOff==NULL) pOff=&Off;
 if (pLong->psec[ind]<CZERO)
  pLong->psec[ind]=-pLong->psec[ind];
 else
  if (pLong->psec[ind]==CZERO) {
   if (ind)
    for (i=ind-1;i>=0;--i)
     if (pLong->psec[i]!=CZERO) {
      pLong->psec[ind]=(pLong->psec[i]>CZERO)?pLong->psec[i]:-(pLong->psec[i]);
      }
   if (pLong->psec[ind]==CZERO) pLong->psec[ind]=pLong->size;
   }
 //позиция последняя строка должна добавиться
 if (ind>=pLong->busy) return LongAddEnd(pLong,size,pOff,perr);
 *pOff=ind;
 *perr=COutKodOk;
 pStr=pgLongpStr(pLong,ind);
 d=LongSize(pLong,ind) - size;
 //не хватает места на странице
 if (-d > pLong->free) {*perr=CWARdataNotFound;return NULL;}
 //если строки одинаковы
 if (d==0) return (Tstr*)pStr;
 //строка влазит
 //начало области строк
 ib=pLongOffBeg(pLong);
 pb=pLong->psec + ib;
 //раздвинуть строку
 if (ind<pLong->busy-1)
  memMove(pb+d,pb,(pLong->psec[ind]-ib)*sizeof(Tsize));
 pStr += d;
 //откорректировать смещение строк
 for (i=ind;i<pLong->busy;++i)
  if (pLong->psec[i]>CZERO)
   pLong->psec[i] +=d;
  else
   if (pLong->psec[i]<CZERO)
    pLong->psec[i] -=d;
 pLong->free+=d;
 return (Tstr*)pStr;
 }
//
Tstr* tstLongIns(Tlong *pLong,Tsize ind,Tsize size,Tuint32 *pOff,Terr *perr){return LongIns(pLong,ind,size,pOff,perr);}
/*
 *  Добавить на страницу строку
 */
static Tstr* LongAdd(Tlong *pLong,Tsize size,Tuint32 *pOff,Terr *perr){
 Tsize j,max,min,imax,imin,aj;
 Tstr *pStr=NULL;
 Tuint32 Off;
 //
 if (pOff==NULL) pOff=&Off;
 pgLongClearNULL(pLong);
 if (pLong->busy<1) return LongAddEnd(pLong,size,pOff,perr);
 *perr=COutKodOk;
 *pOff=0;
 // на странице нет места для новой строки
 max=pLong->busy;min=-pLong->busy;
 imax=imin=-1;
 //пытаемся вставить строку в середину в наиболее подходящее место
 for (j=0;j<pLong->busy;j++)
  if (pLong->psec[j]<CZERO) {
   //найдено идеальное место вставляем строку
   aj=LongSize(pLong,j)-size;
   if (aj == 0) {
    pLong->psec[j]=-pLong->psec[j];
    pStr=(Tstr*)(pLong->psec + pLong->psec[j]);
    pLong->free -=size;
    *pOff=j;
    return pStr;
    }
   if (aj > 0) {
    if (aj<max) {max=aj;imax=j;}
    }
   else
    if (aj>min) {min=aj;imin=j;}
   }
 if ((imin>=0)&&(-min < max))
  //строка поместилась
  if ((pStr=LongIns(pLong,imin,size,pOff,perr))!=NULL) return pStr;
 //строка поместилась
 if (imax>=0)
  if ((pStr=LongIns(pLong,imax,size,pOff,perr))!=NULL) return pStr;
 //строка не поместилась
 return LongAddEnd(pLong,size,pOff,perr);
 }
Tstr* tstLongAdd(Tlong *pLong,Tsize size,Tuint32 *pOff,Terr *perr){return LongAdd(pLong,size,pOff,perr);}
/*
 * вставить строку в списокстраниц
 */
Tstr* getStrOfLongPg(TmapLong *pmlong ,Tsize SizeStr,Tadr *pAdr,Tuint32 *pOff,Terr *perr){
 void *pLong;
 Tstr *pStr=NULL;
 Tsize Size;
 //
 SizeStr += offsetTstr;
 *pAdr=0; *pOff=0;
 //Ищем страницу для вставки
 Size=mshDivUp(SizeStr,sizeof(Tsize));
 //запрашиваемая длинна превышает размер страницы
 if (Size >=  ((Tlong*)(pmlong->psec))->size ) {*perr=CERRPgFull;return NULL;}
 if (pmlong->limitViewlong){
  int n=0;
  pmapiterPgBack(pmlong,pLong){
   if ((pStr=LongAdd((Tlong*)pLong,Size,pOff,perr))){
    *pAdr=(void*)(pLong) - pmlong->psec;
    return pStr;
    }
   if (++n > pmlong->limitViewlong) break;
   }
  }
 else
  pmapiterPgBack(pmlong,pLong){
   if ((pStr=LongAdd((Tlong*)pLong,Size,pOff,perr))){
    *pAdr=(void*)(pLong) - pmlong->psec;
    return pStr;
    }
   }
 //подходящая страница не найдена добавляем новую страницу
 pLongNew(pmlong,*pAdr,pLong);
 return LongAddEnd((Tlong*)pLong,Size,pOff,perr);
 }
/*
 * строку pStrIn разместить в pData , а длинную строку в pmLong
 * Terr StrToDataDb(TmapLong *pmLong,Tdata *pData,Tbstr *pStrIn) 

 */
Terr StrToDataOrLong(TmapLong *pmLong,Tdata *pDataSet,Tbstr *pStrIn) {
 Tstr *pStr;
 Terr err=COutKodOk;
 //
 if ((pStrSize(pStrIn))<=CsizeStr) {
  pStrTopData(pStrIn,pDataSet);
  }
 else {
  if ((pStr=getStrOfLongPg(pmLong,pStrIn->size,&(pDataSet->dataLongAdr),&(pDataSet->dataLongOff),&err))){
   pStrTopStr(pStr,pStrIn);
   pDataSet->datapmLong=pmLong;
   pDataSetLong(pDataSet);
   }
  else{
   pDataSetNULL(pDataSet);
   }
  }
 return err; 
 }
/*
 * поместить Массив в данные
 */
void pArrToLong(TmapLong *pmLong,TarrData *pArr);
/*
static Terr pArrTopDataLoclong(TmapLong *pmLong,TarrData *pArr,Tdata *pData,int isStrToLong){
 Tsize Size;
 Tstr *pStr;
 Terr err=COutKodOk;
 //
 if (parrIsNull(pArr)){  pDataSetNULL(pData);  return COutKodOk; }
 if (pArr->busy<2){ *pData=pArr->psec[0]; return COutKodOk; }
 Size=sizeof(Tdata) * (pArr->busy);
 if ((pStr=getStrOfLongPg(pmLong,Size,&(pData->dataLongAdr),&(pData->dataLongOff),&err))){
  pStr->size=Size;
  memCopy(pStr->psec,pArr->psec,Size);
  pData->datapmLong=pmLong;
  pDataSetLong(pData);
  pDataListSet(pData);
  //если в массиве хранятся длинные строки то их то же перенести
  if (isStrToLong) pArrToLong(pmLong,pArr);
  return err;
  }
 return CERRPgFull;
 }
*/
//Terr pArrTopDataLoc(TmapLong *pmLong,TarrData *pArr,Tdata *pData){return pArrTopDataLoclong(pmLong,pArr,pData,1); }
//Terr pArrTopDataLocnoStr(TmapLong *pmLong,TarrData *pArr,Tdata *pData){ return pArrTopDataLoclong(pmLong,pArr,pData,0); }
/*
 * Откорректировать длинную строку
 */
void pDataKillLongArr(TmapLong *pmLong,Tdata *pDataIn){
 Tbstr bStr;
 TarrData Arr;
 Tdata *pData;
 //
 if (pDataIsLong(pDataIn)) {
  if (pDataIsList(pDataIn)){
   pDataIn->datapmLong=pmLong;
   bStrGet(pDataIn,&bStr);
   StrToArrData(bStr,Arr);
   arrIter(Arr,pData)
    pDataKillLongArr(pmLong,pData);
   }
  longpDataDel(pmLong,pDataIn);
  }
 }
/*
 * Заменить данные в pgLong
 */
Terr UpdateDataLong(TmapLong *pmLong,Tdata *pDataSet,Tdata *pDataDb) {
 Tstr *pStr;
 Tsize Size;
 Tlong *pLong;
 Tbstr bStr;
 Terr err = COutKodOk;
 //
 if ( pDataIsLong(pDataDb) ) {
  pDataDb->datapmLong=pmLong;
  if (( pDataIsLong(pDataSet) )
   && (pDataSet->datapmLong == pDataDb->datapmLong) ){
   //если строка не изменилась
   if ( (pDataSet->dataLongAdr==pDataDb->dataLongAdr)
      &&(pDataSet->dataLongOff==pDataDb->dataLongOff)) return err;
//   pDataKillLongArr(pmLong,pDataDb);   *pDataDb=*pDataNew;   return err;
   }
  //попытка вставить строку вместо старой
  if (pDataIsOut(pDataSet)) {
   //
   bStrGet(pDataSet,&bStr);
   if (bStr.psec) {
    //строка поместилась в данные
    if ((StrSize(bStr))<=CsizeStr) {
     pDataKillLongArr(pmLong,pDataDb);
//     if ( pDataIsLong(pDataNew) ) pDataKillLongArr(pDataNew->datapmLong,pDataNew);
     pStrTopData(&bStr,pDataDb);
     return COutKodOk;
     }
    pLong=getLongOfpmLong(pDataDb->datapmLong,pDataDb->dataLongAdr);
    Size=mshDivUp(StrSize(bStr),sizeof(Tsize));
    //массив переместить в текущую базу
//    moveTohisDataArr(pmLong,pDataNew);
    //строка НЕ поместилась вставить строку в новую страницу
    if ((pStr=LongIns(pLong,pDataDb->dataLongOff,Size,&(pDataDb->dataLongOff),&err))==NULL){
     pDataKillLongArr(pmLong,pDataDb);
     if ((pStr=getStrOfLongPg(pmLong,bStr.size,&(pDataDb->dataLongAdr),&(pDataDb->dataLongOff),&err))==NULL) return err;
     }
    StrTopStr(pStr,bStr);
    pDataDb->datapmLong=pmLong;
    pDataSetLong(pDataDb);
    pDataListSetpData(pDataDb,pDataSet);
//    if (pDataIsLong(pDataNew)) {longpDataDel(pDataNew->datapmLong,pDataNew);}
//    *pDataNew=*pDataDb;
    return err;
    }
   //удалить старое значение
   pDataKillLongArr(pmLong,pDataDb);
   pDataSetNULL(pDataDb);
   return err;
   }
  //удалить старое значение
  pDataKillLongArr(pmLong,pDataDb);
  *pDataDb=*pDataSet;
  return err;
  }
 //pDataDb не длинная строка
 if (pDataIsOut(pDataSet)) {
  bStrGet(pDataSet,&bStr);
  if (bStr.psec) {
   //строка поместилась в данные
   if ((StrSize(bStr))<=CsizeStr) {
//    if ( pDataIsLong(pDataNew) ) pDataKillLongArr(pDataNew->datapmLong,pDataNew);
    pStrTopData(&bStr,pDataDb);
    return COutKodOk;
    }
   if ((pStr=getStrOfLongPg(pmLong,bStr.size,&(pDataDb->dataLongAdr),&(pDataDb->dataLongOff),&err))==NULL) return err;
   StrTopStr(pStr,bStr);
   pDataDb->datapmLong=pmLong;
   pDataSetLong(pDataDb);
   pDataListSetpData(pDataDb,pDataSet);
//   if (pDataIsLong(pDataNew)) {longpDataDel(pDataNew->datapmLong,pDataNew);}
   }
  else{
   pDataSetNULL(pDataDb);
   }
  }
 else{
  *pDataDb=*pDataSet;
  }
 return err;
 }
/*
 * Очистить список страниц
 */
void ListLongClear(TmapLong *pmLong){
 void *pLong;
 size_t Size;
 //уменьшить список страниц
 if (pmLong->limitViewlong){
  Size=pmLong->limitViewlong * pmLong->sizesec;
  if ((pmLong->limitViewlong)&&(pmLong->size > Size )) { mapRepMap(pmLong,Size);}
  }
 pmapiterPgBack(pmLong,pLong){ pLongClear((Tlong*)pLong); }
 }
