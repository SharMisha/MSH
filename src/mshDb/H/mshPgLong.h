
/* 
 * File:   mshPgLong.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 1 декабря 2016 г., 8:06
 */

#ifndef MSHPGLONG_H
#define MSHPGLONG_H

#ifdef __cplusplus
extern "C" {
#endif

#define datapmLong dbAdrLong.pmlong
#define dataLongAdr dbAdrLong.adrLong
#define dataLongOff dbAdrLong.Off

#define	pLongClear(pLong) (pLong)->busy=0;(pLong)->free=(pLong)->size

#define	pLongClear(pLong) (pLong)->busy=0;(pLong)->free=(pLong)->size
 
#define	SizeLong(pmLong) (pmLong)->sizesec
#define	SizeLongToSizeChar(pmLong) SizeLong(pmLong) - offsetof(Tlong,psec)
#define	SizeLongToSize(pmLong) (SizeLongToSizeChar(pmLong))/sizeof(Tsize)
    
#define	pLongInit(pmLong,pLong) (pLong)->size=SizeLongToSize(pmLong);pLongClear(pLong)
//  Найти строку на странице
#define	getLongOfpmLong(pmLong,offPg) (Tlong*)((pmLong)->psec + (offPg))
#define	getLongOfmLong(mLong,offPg) (Tlong*)((mLong).psec + (offPg))
//
#define	pLongOff(pLong,Off) ((pLong)->psec[Off]>CZERO)?(pLong)->psec[Off]:((pLong)->psec[Off]<CZERO)?-((pLong)->psec[Off]):0
#define	pgLongpStr(pLong,Off)  (((pLong)->psec[Off]==CZERO)?NULL:(pLong)->psec + (pLongOff(pLong,Off)))
/*    
#define	pLongOff(pLong,Off) ((pLong)->psec[Off]>CZERO)?(pLong)->psec[Off]:((pLong)->psec[Off]<CZERO)?-((pLong)->psec[Off]):0
#define	LongOff(Long,Off) ((Long).psec[Off]>CZERO)?(Long).psec[Off]:((Long).psec[Off]<CZERO)?-((Long).psec[Off]):0
//
#define	pgLongStr(pLong,Off) ((pLong)->psec[Off]==CZERO)?NULL:(pLong)->psec[pLongOff(pLong,Off)]
#define	pgLongpStr(pLong,Off) ((pLong)->psec[Off]==CZERO)?NULL:(pLong)->psec + (pLongOff(pLong,Off))
*/
//
//освободить свободное пространство в конце страницы
#define	pLongFree(pLong) if ((pLong)->busy<1) {pLongClear(pLong);}else (pLong)->free=(pLong)->psec[(pLong)->busy-1] - (pLong)->busy
#define	pgLongClearNULL(pLong) while (((pLong)->busy)&&((pLong)->psec[(pLong)->busy-1]<=CZERO)) --((pLong)->busy);pLongFree(pLong)
#define	pLongOffBeg(pLong) ({pgLongClearNULL(pLong);((pLong)->busy)?(pLong)->psec[(pLong)->busy-1]:(pLong)->size;})

#define	pLongNew(pmLong,offPg,pLong) pmapsecNew(pmLong,offPg,pLong);pLongInit(pmLong,(Tlong*)pLong)

#define	LongDel(pmLong,adrPg,Off) LongDelInd(getLongOfpmLong(pmLong,adrPg),Off)
 
//#define LongpRefDel(pData) if ( (pData) && (pDataIsLong(pData)) ) {LongDel((pData)->datapmLong,(pData)->dataLongAdr,(pData)->dataLongOff);}pDataSetNULL(pData)
//#define LongRefDel(Data) if (DataIsLong(Data)) {LongDel((Data).datapmLong,(Data).dataLongAdr,(Data).dataLongOff);}DataSetNULL(Data)

#define longpDataDel(pmLong,pData) LongDel(pmLong,(pData)->dataLongAdr,(pData)->dataLongOff)
#define pKeyDelLong(pmLong,pKey) pDataKillLongArr(pmLong, &((pKey)->data) )
#define KeyDelLong(pmLong,Key) pDataKillLongArr(pmLong, &((Key).data) )
    
#define pValKeyDelLong(pmLong,pVal) pDataKillLongArr(pmLong, &(pValKey(pVal)) )
#define pValDataDelLong(pmLong,pVal) pDataKillLongArr(pmLong, &(pValData(pVal)) )
 
#define pValDelLong(pmLong,pVal) pValDataDelLong(pmLong,pVal);pValKeyDelLong(pmLong,pVal)
#ifdef __cplusplus
}
#endif

#endif /* MSHPGLONG_H */

