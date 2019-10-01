
/* 
 * File:   mshPack.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 декабря 2016 г., 17:55
 */

#ifndef MSHPACK_H
#define MSHPACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mshData.h"

/*
 * Информация хранится в виде строк
 * упаковка распаковка объектов
 */ 
 typedef struct {
  _contArea(Tsize);
  union{
   Tstr *pPtrStr;
   char *pPtrChar;
   } Ptr;
  } TPacks;
/**/
#define packVoid(pPack,pbBuf) ({ Toff Top; pareaPushStr(pPack,pbBuf,Top); })
/**/
#define pPackNextVoid(pPack) pPack->Ptr.pPtrChar += pStrSize( pPack->Ptr.pPtrStr )
#define PackNextVoid(Pack) Pack.Ptr.pPtrChar += pStrSize( Pack.Ptr.pPtrStr )

#define	pPackPtr0(pPack) pPack->Ptr.pPtrChar=(char*)(pPack->psec)
#define	PackPtr0(Pack) Pack.Ptr.pPtrChar=(char*)(Pack.psec)
  
#define	pPackInit(pPack) pareaInit(pPack,CvmsizeBufPckDef);pPackPtr0(pPack)
#define	PackInit(Pack) areaInit(Pack,CvmsizeBufPckDef);PackPtr0(Pack)

#define	pPackClear(pPack) pareaClear(pPack);pPackPtr0(pPack)
#define	PackClear(Pack) areaClear(Pack);PackPtr0(Pack)

#define	PackFree(Pack) areaFree(Pack)

#ifdef __cplusplus
}
#endif

#endif /* MSHPACK_H */

