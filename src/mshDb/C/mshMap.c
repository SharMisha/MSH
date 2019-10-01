/* 
 * File:   mshMap.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 22 декабря 2016 г., 21:08
 */
#include "mshMap.h"
#include "mshDiv.h"
#include "mshErr.h"

Terr checkDivMag(Tdiv ff,const char *pmag);
/**/
void mapInit(Tmap *pMap,size_t sizeSec){
 if (pMap->div){
  pMap->sizesec=sizeSec;
  pMap->size=divPosEnd(pMap->div);
  if (pMap->size < pMap->sizesec){
   (void)divSetSize(pMap->div,pMap->sizesec);
   pMap->size=pMap->sizesec;
   }
  pMap->psec=FileToMap(pMap->div,pMap->size);
  }
 }
/*
 * загрузить в буфер файл
 */
Terr mshFileReadToMap(char *pfl,TmapFixed *pMap){
 Tdiv ff;
 size_t sizemap;
 //
 if ((ff=divOpen(pfl))==CdivRetErr) return CERRdivReadBad;
 sizemap=divPosEnd(ff);
 TpmapFixedNew(ff,sizemap,pMap);
 return (pMap->psec==MAP_FAILED)?CERRvmmMapFiled:COutKodOk;
 }
/*
 * загрузить в буфер программу
 */
Terr mshProgReadToMapFixed(char *pfl,const char *pCmshMag,TmapFixed *pMap){
 Terr err=COutKodOk;
 Tdiv ff;
 size_t sizemap;
 //
 if ((ff=divOpen(pfl))==CdivRetErr) return CERRdivReadBad;
 if ((err=checkDivMag(ff,pCmshMag))<CERR) {
  divClose(ff);
  return err;
  }
 sizemap=divPosEnd(ff);
 TpmapFixedNew(ff,sizemap,pMap);
 return (pMap->psec==MAP_FAILED)?CERRvmmMapFiled:err;
 }
/**/
Terr mshProgReadToMap(char *pfl,const char *pCmshMag,Tmap *pMap){
 Terr err=COutKodOk;
 Tdiv ff;
 size_t sizemap;
 //
 if ((ff=divOpen(pfl))==CdivRetErr) return CERRdivReadBad;
 if ((err=checkDivMag(ff,pCmshMag))<CERR) {
  divClose(ff);
  return err;
  }
 sizemap=divPosEnd(ff);
 TpMapNew(ff,sizemap,pMap);
 return (pMap->psec==MAP_FAILED)?CERRvmmMapFiled:err;
 }
/*
 */
Terr areaMapDivInit(Tmap *pMap,size_t sizeSec){
 Tdiv Div;
 size_t Size;
 //
 if ( (Div=divCreatTemp("mapXXXXXX") )==CdivRetErr ) return CERRdivOpenBad;
// if (sys_Map_size_page==0) sys_Map_size_page=sysconf(_SC_PAGESIZE); Size=sys_Map_size_page;
 Size=sysconf(_SC_PAGESIZE);
 TpMapNew(Div,Size,pMap);
 return COutKodOk;
 }
