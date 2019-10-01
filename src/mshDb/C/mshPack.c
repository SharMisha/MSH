/*
 * File:   mshPack.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 14:49
 */
#include "mshData.h"
#include "mshPack.h"
#include "mshMap.h"

void bStrGet(Tdata *pData,Tbstr* pbstrOut);
Terr StrToDataOrLong(TmapLong *pmLong,Tdata *pData,Tbstr *pStrIn);
/*
 * Запаковать pData
void DataToPack(TPacks *pPack,Tdata *pData,TareaChar *pArea)
 */
static void packDataLong(TPacks *pPack,Tdata *pData){
 Tbstr bBuf;
 Tdata *pB,*pE;
 //
 if (pDataIsOut(pData)){
  bStrGet(pData,&bBuf);
  packVoid(pPack,&bBuf);
  if ( (pDataIsList(pData))&&(bBuf.size) ) {
   pB=(Tdata*)(bBuf.psec);
   pE=pB+bBuf.size;
   for(;pB<pE;++pB) packDataLong(pPack,pB);
   }
  }
 }

void packData(TPacks *pPack,Tdata *pData){
 Tbstr bBuf;
 //запаковать поле данных
 bBuf.psec=(char*)pData;
 bBuf.size=sizeof(Tdata);
 packVoid(pPack,&bBuf);
 packDataLong(pPack,pData);
 }
/*
void ArrToPack(TPacks *pPack,TarrData *parrData,TareaChar *pArea){
 */
void packArrData(TPacks *pPack,TarrData *parrData){
 Tbstr bBuf;
 Tdata *pB;
 //
 bBuf.psec=(char*)(parrData->psec);
 bBuf.size=parrData->busy * sizeof(Tdata);
 packVoid(pPack,&bBuf);
 for (pB=parrData->psec;pB<parrData->ptop;++pB) packDataLong(pPack,pB);
 }
//
/*
 * распаковать длинные строки в pPack
 */
static void unpackDataToPack(TPacks *pPack,Tdata *pData){
 char Style;
 Tdata *pB,*pE;
 //
 if (pDataIsOut(pData)){
  Style=CDataArea;
  pData->areaOff.off=(Tsize*)(pPack->Ptr.pPtrStr) - pPack->psec ;
  (pData)->areaOff.parea=(Tarea*)pPack;
  if (pDataIsList(pData)){
   StSetList(Style);
   pB=(Tdata*)(pPack->Ptr.pPtrStr->psec);
   pE=pB+pPack->Ptr.pPtrStr->size;
   pPackNextVoid(pPack);
   for(;pB<pE;++pB) unpackDataToPack(pPack,pB);
   }
  pDataSightSet(pData,Style);
  }
 }

Tdata* unpackData(TPacks *pPack){
 Tdata *pData;
 //
 pData=(Tdata*)(pPack->Ptr.pPtrStr->psec);
 pPackNextVoid(pPack);
 unpackDataToPack(pPack,pData);
 return pData;
 }
/**/
void unpackArrData(TPacks *pPack,TarrData *parrData){
 TarrData arD;
 Tdata *pD;
 //
 arD.psec=(Tdata*)(pPack->Ptr.pPtrStr->psec);
 arD.busy=arD.capa=arD.size=pPack->Ptr.pPtrStr->size / sizeof(Tdata);
 arrsetptop(arD);
 parrCopy( parrData,&arD );
 for ( pD=arD.psec;pD <= arD.ptop;++pD ) unpackDataToPack(pPack,pD);
 }
