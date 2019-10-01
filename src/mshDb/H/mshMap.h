
/* 
 * File:   mshMap.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 27 октября 2016 г., 10:05
 */

#ifndef MSHMAP_H
#define MSHMAP_H

#ifdef __cplusplus
extern "C" {
#endif
 
#include "mshCnst.h"
#include "mshDiv.h"
//
extern int sys_Map_size_page;
/*
 * Отображение файла в память MAP_PRIVATE
 */
#define FileToMap(Div,length) mmap(NULL,length,PROT_READ | PROT_WRITE, MAP_SHARED,Div,0)
#define FileToMapPRIVATE(Div,length) mmap(NULL,length,PROT_READ | PROT_WRITE, MAP_PRIVATE,Div,0)
    
#define FileToFixedMap(Div,length) mmap(NULL,length,PROT_READ , MAP_SHARED,Div,0)
// 
#define reMap(old_address,old_size,new_size) mremap(old_address,old_size,new_size,MREMAP_MAYMOVE)


#define unMap(addr,length) munmap(addr,length)
//#define wrireMap(addr,length) msync(addr,length, MS_ASYNC)
    
#define _contmapFixedDef void *psec; size_t size
    
typedef struct {
 _contmapFixedDef;
 } TmapFixed;
 
#define _contmapDef _contmapFixedDef; Tdiv div; size_t sizesec
typedef struct {
 _contmapDef;
 } Tmap;
typedef struct {
 _contmapDef;
 Tusize limitViewlong;
 } TmapLong;
//  Массив данных отображенных на файл
typedef struct {
 _contmapFixedDef;
 Tdiv div;
 Toff sizedata;
 } TmapArr;
//  Массив данных отображенных на файл
typedef struct {
 _contmapDef;
 size_t busy;
 } TmapArea;
/*
 * Массив данных отображенных на файл
 *
 */
#define maparrpDb0(pDb) ((Tdata*)((pDb)->mpg.psec))
#define maparrDb0(Db) ((Tdata*)((Db).mpg.psec))
#define mapArrpDataOff(pDb,Off) maparrpDb0(pDb) + Off
#define mapArrDataOff(pDb,Off) maparrpDb0(pDb)[Off]
#define maparrpDbBusy(pDb) maparrpDb0(pDb)->intgr
#define maparrDbBusy(Db) maparrDb0(Db)->intgr

#define mapSizeByte(Size) (Size) * sizeof(Tdata)
#define mapSizeData(Size) (Size) / sizeof(Tdata)
//#define	TmapBaseNew(div,Size,Map) ((TmapFixed)(Map)).size=Size;((TmapFixed)(Map)).psec=FileToFixedMap(div,Size)
//#define	TpmapBaseNew(div,Size,pMap) ((TmapFixed*)(pMap))->size=Size;((TmapFixed*)(pMap))->psec=FileToFixedMap(div,Size)
//#define	TmapBaseNew(div,Size,Map) ((TmapFixed)(Map)).size=Size;((TmapFixed)(Map)).psec=FileToMap(div,Size)
#define	TpmapBaseNew(Div,Size,pMap) ((pMap))->size=Size;((pMap))->psec=FileToMap(Div,Size)
 
//#define	TmapFixedNew(div,Size,Map) ((TmapFixed)(Map)).size=Size;((TmapFixed)(Map)).psec=FileToFixedMap(div,Size);divClose(div)
#define	TpmapFixedNew(Div,Size,pMap) ((pMap))->size=Size;((pMap))->psec=FileToFixedMap(Div,Size);divClose(Div)
#define	TmapFixedPRIVATENew(Div,Size,Map) ((Map)).size=Size;((Map)).psec=FileToMapPRIVATE(Div,Size);divClose(Div)
//#define	TpmapFixedWriteNew(div,Size,pMap) TpmapBaseNew(div,Size,pMap);divClose(div)
 
//#define	TmapFixedNew(div,Size,Map) TpmapBaseNew(div,Size,&Map);divClose(div)
//#define	TpmapFixedNew(div,Size,pMap) TpmapBaseNew(div,Size,pMap);divClose(div)
 
#define	TMapNew(Div,Size,Map) TpmapBaseNew(Div,Size,&Map);(Map).div=Div
#define	TpMapNew(Div,Size,pMap) TpmapBaseNew(Div,Size,pMap);(pMap)->div=Div
 
#define	mapFixedFree(Map) if (Map.psec) unMap((Map).psec,(Map).size)
#define	mapFree(Map) if ((Map).psec) unMap((Map).psec,(Map).size); if ((Map).div!=CdivRetErr) divClose((Map).div)
/*
#define	mapFixedClear(Map) (Map).psec=NULL;(Map).size=0
#define	mapFixedFreeClear(Map) if ((Map).psec) {unMap((Map).psec,(Map).size);mapFixedClear(Map);}
 */
/*
#define	mapReMap(Map,Size) unMap(Map.psec,Map.sizemap);(void)divSetSize(Map.div,Size); if (Map.init>Size) Map.init=Size;\
  Map.psec=FileToMap(Map.div,Size); Map.sizemap = Size
 */
 //*
#define	mapReMap(Map,Size) if (Size > (Map).size) {divSetSize((Map).div,Size); (Map).psec=reMap((Map).psec,(Map).size,Size);}\
 else{(Map).psec=reMap((Map).psec,(Map).size,Size);divSetSize((Map).div,Size); };(Map).size = Size
#define	mapRepMap(pMap,Size) if (Size > (pMap)->size) {divSetSize((pMap)->div,Size); (pMap)->psec=reMap((pMap)->psec,(pMap)->size,Size);}\
 else{(pMap)->psec=reMap((pMap)->psec,(pMap)->size,Size);divSetSize((pMap)->div,Size); };(pMap)->size = Size
//
#define	mapsecNew(Map,offSec,pSec) offSec=(Map).size;(Map).size += (Map).sizesec;\
        divSetSize((Map).div,(Map).size);(Map).psec=reMap((Map).psec,offSec,(Map).size);pSec=(Map).psec + offSec
#define	pmapsecNew(pMap,offSec,pSec) offSec=(pMap)->size;(pMap)->size += (pMap)->sizesec;\
        divSetSize((pMap)->div,(pMap)->size);(pMap)->psec=reMap((pMap)->psec,offSec,(pMap)->size);pSec=(pMap)->psec + offSec
/*
#define	mapSecNew(Map,offSec,pSec) if (Map.busy >= Map.mapf.size) {mapReMap(Map,Map.mapf.size + Map.sizesec);}\
 offSec=Map.busy; pSec=Map.mapf.psec + Map.busy; Map.busy += Map.sizesec
 */
 
#define	mapgetSec(Map,Off) (Map).psec + (Off)
 
#define	mapgetOff(Map,pSec) (pSec) - ((Map).psec

#define	mapiterPgBack(Map,pPg) for (pPg=(Map).psec + (Map).size - (Map).sizesec;pPg >= (Map).psec;pPg-=(Map).sizesec)
#define	pmapiterPgBack(pMap,pPg) for (pPg=(pMap)->psec + (pMap)->size - (pMap)->sizesec;pPg >= (pMap)->psec;pPg-=(pMap)->sizesec)

//#define mapLongInit(pmLong,sizeSec,limitView) mapInit((Tmap*)(pmLong),sizeSec); (pmLong)->limitViewlong=limitView
/*
 * Отображение файла на область Area
 */
#define	pareaMapInit(pareaMap,Size,Div) if (sys_Map_size_page==0) sys_Map_size_page=sysconf(_SC_PAGESIZE); if (Size<sys_Map_size_page) Size=sys_Map_size_page;\
 TpMapNew(Div,Size,pareaMap); (pareaMap)->busy=0

#define	areaMapPushStr(areaMap,pStr,Top) ({size_t size Size; int Count=mshOkrUp(pStrSize(pStr),sizeof( Tsize ) );Size=Count +  (areaMap).busy\
 if ( (areaMap).size < Size) {divSetSize((areaMap).div,Size); (areaMap).psec=reMap((areaMap).psec,(areaMap).size,Size); (areaMap).size = Size; }\
 areaAddStr(areaMap,pStr,Top,Count);})

#define	pareaMapPushStr(pareaMap,pStr,Top) ({size_t size Size; int Count=mshOkrUp(pStrSize(pStr),sizeof( Tsize ) );Size=Count +  (pareaMap)->busy\
 if ( (pareaMap)->size < Size) {divSetSize((pareaMap)->div,Size); (pareaMap)->psec=reMap((pareaMap)->psec,(pareaMap)->size,Size); (pareaMap)->size = Size; }\
 pareaAddStr(pareaMap,pStr,Top,Count);})

#ifdef __cplusplus
}
#endif

#endif /* MSHMAP_H */

