/* 
 * File:   mshmData.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 20 Август 2014 г., 12:01
 */

#ifndef MSMDATA_H
#define	MSMDATA_H

#ifdef	__cplusplus
extern "C" {
#endif
//     
#include "mshMap.h"
#include "mshArea.h"
#include "mshAreaData.h"
//    
/*
 * строка символов
 */
typedef struct {
 Tsize size; /* длинна строки */
 char  psec[CCsizeStr]; /* строка */
 } Tstr;
typedef struct {
 Tsize size; /* длинна строки */
 char  psec[CCsizeStrShort]; /* строка */
 } Tstrshr;
 /*
 * строка с внешним расположением поля длины
 */
#define _contBstr  Tsize size; char *psec

typedef struct { _contBstr; } Tbstr;
 
#define	bstrInit(bStr,Size) (bStr).psec=memNew(Size);(bStr).size=Size
#define	bstrWide(bStr,Size) if ((bStr).size<Size) {(bStr).psec=memChange((bStr).psec,Size);(bStr).size=Size;}
#define	pbstrWide(pbStr,Size) if ((pbStr)->size<Size) {(pbStr)->psec=memChange((pbStr)->psec,Size);(pbStr)->size=Size;}
#define	bstrFree(bStr) (bStr).psec=memNew(Size);(bStr).size=0

#define	bstrClear(bStr) (bStr).psec=NULL;(bStr).size=0
#define	pbstrClear(pbStr) (pbStr)->psec=NULL;(pbStr)->size=0

#define offsetTstr offsetof(Tstr,psec)
//
#define pStrSize(pstr) (pstr)->size + offsetTstr
#define StrSize(str) (str).size + offsetTstr
//преобразование Tstr в Tbstr 
#define pStrTopbStr(pStr,pbStr) (pbStr)->size=(pStr)->size;(pbStr)->psec=(pStr)->psec
#define pStrTobStr(pStr,bStr) (bStr).size=(pStr)->size;(bStr).psec=(pStr)->psec
#define StrTobStr(Str,bStr) (bStr).size=(Str).size;(bStr).psec=(Str).psec

#define pStrTopStr(pStrTo,pStrFrom) (pStrTo)->size=(pStrFrom)->size; memCopy((pStrTo)->psec,(pStrFrom)->psec,(pStrTo)->size)
#define StrTopStr(pStrTo,StrFrom) (pStrTo)->size=(StrFrom).size; memCopy((pStrTo)->psec,(StrFrom).psec,(StrFrom).size)
#define StrToStr(StrTo,StrFrom) (StrTo).size=(StrFrom).size; memCopy((StrTo).psec,(StrFrom).psec,(StrTo).size)
#define pStrToStr(StrTo,pStrFrom) (StrTo).size=(pStrFrom)->size; memCopy((StrTo).psec,(pStrFrom)->psec,(StrTo).size)
 
#define pStrTopStrID(pStrTo,pStrFrom) (pStrTo)->size=((pStrFrom)->size>CCsizeStr)?CCsizeStr:(pStrFrom)->size;\
 memCopy((pStrTo)->psec,(pStrFrom)->psec,(pStrTo)->size)
#define StrToStrID(StrTo,StrFrom) (StrTo).size=((StrFrom).size>CCsizeStr)?CCsizeStr:(StrFrom).size;\
 memCopy((StrTo).psec,(StrFrom).psec,(StrTo).size)
//
#define pCharTopStr(pBuf,pStr) pStr->size=strlen(pBuf); if ((pStr->size+1)>CCsizeStr) pStr->size=CCsizeStr-1; memCopy(pStr->psec,pBuf,pStr->size+1)
#define pCharToStr(pBuf,Str) Str.size=strlen(pBuf); if ((Str.size+1)>CCsizeStr) Str.size=CCsizeStr-1; memCopy(Str.psec,pBuf,Str.size+1)

#define pStrnextpStr(pStr) (Tstr*)({Tsize _l=mshOkrUp((pStr)->size,sizeof(Tsize) ); (pStr)->psec + _l; })

#define AreaTobStr(Area,bStr) (bStr).psec=(Area).psec; (bStr).size=(Area).busy
#define pAreaTopbStr(pArea,bStr) (bStr).psec=(pArea)->psec; (bStr).size=(pArea)->busy
/* Строку поместить в данные */ 
#define pStrTopData(pStr,pData) memMove((pData)->str.psec,(pStr)->psec,(pStr)->size);(pData)->str.size=(pStr)->size;pDataSetStr(pData)
#define pStrToData(pStr,Data) memMove(&((Data).str.psec),(pStr)->psec,(pStr)->size);(Data).str.size=(pStr)->size;DataSetStr(Data)
#define StrTopData(Str,pData) memMove((pData)->str.psec,(Str).psec,(Str).size);(pData)->str.size=(Str).size;pDataSetStr(pData)
/*
 */
#define pStrShortTopData(pData,pStr) if (((pStr) != NULL) && ((pStr)->size)){\
 (pData)->str.size=(((pStr)->size) > CCsizeStr)?CCsizeStr:(pStr)->size;\
 memMove((pData)->str.psec,(pStr)->psec,(pData)->str.size);pDataSetStr(pData);} else pDataSetNULL(pData)
 
#define StrShortTopData(pData,Str) ({ if ((Str).size){\
 (pData)->str.size=((Str).size > CCsizeStr)?CCsizeStr:(Str).size;\
 memMove((pData)->str.psec,(Str).psec,(pData)->str.size);pDataSetStr(pData);}\
 else {(pData)->str.size=0;pDataSetNULL(pData);} })
/*
 * Добавить строку в конец области
 */

//#define pStrTopDatapArea(pStr,pArea,pData) pareaPushStr(pArea,pStr,(pData)->areaOff.off); (pData)->areaOff.parea=pArea; pDataSightSet(pData,CDataArea)

//#define StrTopDataArea(Str,Area,pData) areaPushStr(Area,&Str,(pData)->areaOff.off); (pData)->areaOff.parea=&Area; pDataSightSet(pData,CDataArea)

#define pStrTopArea(pbStr,pArea,pData) (pData)->areaOff.parea=pArea; pareaPushStr(pArea,pbStr,(pData)->areaOff.off); pDataSetArea(pData)

#define StrToArea(bStr,Area,pData) (pData)->areaOff.parea=&(Area); areaPushStr(Area,&(bStr),(pData)->areaOff.off); pDataSetArea(pData)

#define StrTopDatapArea(Str,pArea,pData) pStrTopArea(&Str,pArea,pData)

#define pStrTopDataOrpArea(pStr,pArea,pData) ({if (((pStr)->size) <= CCsizeStr){ (pData)->str.size=(pStr)->size;\
 if ((pStr)->size) {memCopy((pData)->str.psec,(pStr)->psec,(pStr)->size);pDataSetStr(pData);} else pDataSetNULL(pData); }\
 else {pStrTopArea(pStr,pArea,pData);} })

#define	_contAdrOff Tadr adr; Toff off

typedef struct TsadrOff {
 _contAdrOff;
 } TadrOff;
 
typedef struct{
 Tarea *parea;
 Toff off;
 } TareaOff;
// 
typedef struct{
 void *pvoid;
 u_int64_t off;
 } TvoidOff;
 
#define	pAdrSetNULL(pAdr) *pAdr=CnotAdr
#define	AdrSetNULL(Adr) Adr=CnotAdr
#define	pAdrIsNULL(pAdr) *pAdr==CnotAdr
#define	AdrIsNULL(Adr) Adr==CnotAdr
#define	pAdrIsNoNULL(pAdr) *pAdr
#define	AdrIsNoNULL(Adr) Adr
 
#define	pAdrOffSetNULL(padrOff) memNULL(padrOff)
#define	AdrOffSetNULL(adrOff) memNULL(&adrOff)
/*
 * по адресу найти страницу и наоборот
 */
#define	AdrToPg(pDb,Adr) mapgetSec( (pDb)->mpg,Adr)
#define	PgToAdr(pDb,Pg) mapgetOff((pDb)->mpg,Pg)

#define	pgOffSecAdrOffData(pDb,AdrOff,uPg,Val) uPg.pvoid=AdrToPg(pDb,AdrOff.adr);\
   Val=*(pgOffpSec(uPg.pgD,AdrOff.off))
 
#define	_contAdrOfferr _contAdrOff;u_char isalt;Terr err

typedef struct {
 _contAdrOfferr;
 } TAdrOfferr;

typedef struct {_contArr(TAdrOfferr);} TarrAdrOfferr;
 
#define AdrOfferrSetNULL(AdrOfferr) memNULL(&AdrOfferr)
#define pAdrOfferrSetNULL(pAdrOfferr) memNULL(pAdrOfferr)
 
typedef struct {
 TmapLong *pmlong;
 Tadr adrLong;
 Tuint32 Off;
 } TDbdrLong;
 
typedef struct {
 void *void1;
 void *void2;
 Tuint16 type1;
 Tuint16 type2;
 } TVoidTip;
 
//#define dataLongpDb dbAdrLong.pdb
/*
 * Хранение данных размер 24 байта
 */
//struct TstvmDb;
 
typedef union Tudata {
 TDbdrLong dbAdrLong;
 TareaOff areaOff;
 TfNum fnum;
 TintNum intgr;
 Tstr str;
 Tstr *pstr;
 Tbstr bstr;
 Tbstr *pbstr;
 TVoidTip voidtype2;
 void *pointer;
 TvoidOff voidOff;
 struct TstvmDb *prefdb;
 Tint64 int64[CcountInt]; /* целое число -8 */
 Tuint64 uint64[CcountInt]; /* целое число -8 */
 Tint32 int32[CcountInt32]; /* массив целых чисел */
 Tuint32 uint32[CcountInt32]; /* массив целых положительных чисел */
 Tint16 int16[CcountInt16]; /* массив целых чисел */
 Tuint16 uint16[CcountInt16]; /* массив целых положительных чисел */
 Tint8 int8[CcountInt8]; /* массив целых чисел */
 Tint8 uint8[CcountInt8]; /* массив целых положительных чисел */
 u_char st[CsizeData];
 } Tdata;
 /*
  * st[CsizeData-1] - тип данных
  * 8-й бит обозначает ветвь индекса 0-основная ветвь, 1-альтернативная ветвь
  */
/*
 * Установить тип данных
 */
#define DataType(Data) ((Data).st[ChightData])
#define pDataType(pData) ((pData)->st[ChightData])
 
#define DataSight(Data) ((DataType(Data)) & CDataSightMask)
#define pDataSight(pData) ( (pDataType(pData)) & CDataSightMask )
// 
//   ---Is ------
// 
#define DataIsType(Data,St) DataSight(Data) == St
#define pDataIsType(pData,St) pDataSight(pData) == St
 
#define DataIsNULL(Data) DataType(Data) == CDataNull
#define pDataIsNULL(pData) pDataType(pData) == CDataNull
 
#define DataIsNoNULL(Data) DataType(Data) != CDataNull
#define pDataIsNoNULL(pData) pDataType(pData) != CDataNull
 
#define TypeIsInt(Type) (Type <= CDataMaskInt)&&(Type > CDataFloat)
#define TypeIsFloat(Type) Type == CDataFloat

#define DataIsInt(Data) TypeIsInt(DataSight(Data))
#define pDataIsInt(pData) TypeIsInt(pDataSight(pData))

#define DataIsFloat(Data) DataIsType(Data,CDataFloat)
#define pDataIsFloat(pData) pDataIsType(pData,CDataFloat)

#define DataIsPointer(Data) DataIsType(Data,CDataPointer)
#define pDataIsPointer(pData) pDataIsType(pData,CDataPointer)

#define DataIsNoPointer(Data) (DataSight(Data))!=CDataPointer
#define pDataIsNoPointer(pData) (pDataSight(pData))!=CDataPointer

#define DataIsNum(Data) DataSight(Data) < CDataStr
#define pDataIsNum(pData) pDataSight(pData) < CDataStr

#define DataIsNoNum(Data) DataSight(Data) >= CDataStr
#define pDataIsNoNum(pData) pDataSight(pData) >= CDataStr
// 
#define DataIsStr(Data) (DataSight(Data) & CDataStrMask) == CDataStr
#define pDataIsStr(pData) (pDataSight(pData) & CDataStrMask) == CDataStr
#define DataIsNoStr(Data) !(DataIsStr(Data))
#define pDataIsNoStr(pData) !(pDataIsStr(pData))
// 
#define DataIsObj(Data) (DataSight(Data) & CDataStrMask) == CDataObj
#define pDataIsObj(pData) (pDataSight(pData) & CDataStrMask) == CDataObj
//
#define pDataIsArea(pData) pDataSight(pData)==CDataArea
#define pDataIsNoArea(pData) pDataSight(pData)!=CDataArea
//  Является ли страницей длинных строк
#define DataIsLong(Data) DataSight(Data) == CDataLong
#define pDataIsLong(pData) pDataSight(pData) == CDataLong
#define DataIsNoLong(Data) DataSight(Data) != CDataLong
#define pDataIsNoLong(pData) pDataSight(pData) != CDataLong
//   Является ли внешними данными
#define DataIsOut(Data) DataSight(Data) & CDataMaskOut
#define pDataIsOut(pData) pDataSight(pData) & CDataMaskOut
//   Является ли списком
#define DataIsList(Data) DataType(Data) & CDataList
#define pDataIsList(pData) pDataType(pData) & CDataList
//  субстатус строки
#define DataTypeSub(Data) ((Data).st[ChightData1])
#define pDataTypeSub(pData) ((pData)->st[ChightData1])
//
#define DataTypeSubSetNULL(Data) DataTypeSub(Data)= '\0'
#define pDataTypeSubSetNULL(pData) pDataTypeSub(pData)= '\0'
//   ---Set 
#define DataSightSet(Data,St) DataTypeSubSetNULL(Data);DataType(Data)= ((DataType(Data) & CDataSightUnMask) | (St))
#define pDataSightSet(pData,St) pDataTypeSubSetNULL(pData);pDataType(pData)=((pDataType(pData) & CDataSightUnMask) | (St))
 
#define DataSetNULL(Data) memNULL(&Data)
#define pDataSetNULL(pData) memNULL(pData)

#define pDataSetInt(pData) pDataSightSet(pData,CDataInt)
#define DataSetInt(Data) DataSightSet(Data,CDataInt)

#define pDataSetFloat(pData) pDataSightSet(pData,CDataFloat)
#define DataSetFloat(Data) DataSightSet(Data,CDataFloat)

#define pDataSetStr(pData) pDataSightSet(pData,CDataStr)
#define DataSetStr(Data) DataSightSet(Data,CDataStr)
//  данные объект
#define DataSetObj(Data) DataSightSet(Data,CDataObj)
#define pDataSetObj(pData) pDataSightSet(pData,CDataObj)

#define pDataSetbStr(pData) pDataSightSet(pData,CDatabStr)
#define DataSetbStr(Data) DataSightSet(Data,CDatabStr)

#define pDataSetpStr(pData) pDataSightSet(pData,CDatapStr)
#define DataSetpStr(Data) DataSightSet(Data,CDatapStr)

#define pDataSetpbStr(pData) pDataSightSet(pData,CDatapbStr)
#define DataSetpbStr(Data) DataSightSet(Data,CDatapbStr)

#define pDataSetPointer(pData) pDataSightSet(pData, CDataPointer)
#define DataSetPointer(Data) DataSightSet(Data,CDataPointer)

#define pDataSetArea(pData) pDataSightSet(pData,CDataArea)
#define DataSetArea(Data) DataSightSet(Data,CDataArea)
//  данные страницы длинных строк
#define DataSetLong(Data) DataSightSet(Data,CDataLong)
#define pDataSetLong(pData) pDataSightSet(pData,CDataLong)
//  Установить признак массива
#define DataListSet(Data) DataType(Data) |= CDataList
#define pDataListSet(pData) pDataType(pData) |=CDataList
#define pDataListSetpData(pDataTo,pDataFrom) pDataType(pDataTo) |=  pDataIsList(pDataFrom)
// * Установить признак Не массива
#define DataListNoSet(Data) DataType(Data) &= CDataNoList
#define pDataListNoSet(pData) pDataType(pData) &=CDataNoList
//
#define StSetList(St) St |= CDataList
// Установить признак Альтернативной ветви
#define DataSetAlt(Data) DataTypeSub(Data) |=CDatasubAlt
#define pDataSetAlt(pData) pDataTypeSub(pData) |=CDatasubAlt
// * Установить признак Не Альтернативной ветви
#define DataSetNoAlt(Data) DataTypeSub(Data) &= CDatasubNoAlt
#define pDataSetNoAlt(pData) pDataTypeSub(pData) &= CDatasubNoAlt
//  Является ли ветвь Альтернативной
#define DataIsAlt(Data) (DataTypeSub(Data) & CstyleIndMask) == CDatasubAlt
#define pDataIsAlt(pData) (pDataTypeSub(pData) & CstyleIndMask) == CDatasubAlt
 
#define DatastyleGet(Data) DataTypeSub(Data) & CstyleIndMask
#define pDatastyleGet(pData) pDataTypeSub(pData) & CstyleIndMask

#define DatastyleSet(Data,St) DataTypeSub(Data)= ((DataTypeSub(Data) & CstyleIndUnMask) | (St))
#define pDatastyleSet(pData,St) pDataTypeSub(pData)= ((pDataTypeSub(pData) & CstyleIndUnMask) | (St))

#define pDatastyleSetIndArr(pData) pDatastyleSet(pData,CstyleIndArr)
#define DatastyleSetIndArr(Data) DatastyleSet(Data,CstyleIndArr)

#define pDatastyleIsIndArr(pData) pDataTypeSub(pData) & CstyleIndArr
#define DatastyleIsIndArr(Data) DataTypeSub(Data) & CstyleIndArr
/*
 */
#define DataOffLast(Data) ((Data).uint16[CCcountArg1])
#define pDataOffLast(pData) ((pData)->uint16[CCcountArg1])

#define DataSubPar(Data) ((Data).uint16[CCcountArg2])
#define pDataSubPar(pData) ((pData)->uint16[CCcountArg2])
 /* -16 */
#define	contKey Tdata data;Tadr adr
 
typedef struct { contKey; } Tkey;
/* поле ключа или данных
*  описание ключа -56 -80
*/
typedef struct {
 contKey;
 Tkey val;
 } Tval;
/* поле ключа или данных
*  описание ключа -56 -80
*/
#define	ValKey(Val) (Val).data
#define	pValKey(pVal) (pVal)->data
 
#define	ValAdr(Val) (Val).adr
#define	pValAdr(pVal) (pVal)->adr
 
#define	ValData(Val) (Val).val.data
#define	pValData(pVal) (pVal)->val.data
 
#define	ValAdrAlt(Val) (Val).val.adr
#define	pValAdrAlt(pVal) (pVal)->val.adr
        
#define	pValStat(pVal,Stat) if (pVal) {if (pValDataIsNoNULL(pVal)) Stat =CstatDanmaskD;\
 if (pValAdrIsNoNULL(pVal)) Stat |=CstatDanmaskC; if (pValAdrAltIsNoNULL(pVal)) Stat |=CstatDanmaskA;}else Stat=CstatDan0
#define	ValStat(Val,Stat) Stat=CstatDan0; if (ValDataIsNoNULL(Val)) Stat =CstatDanmaskD;\
 if (ValAdrIsNoNULL(Val)) Stat |=CstatDanmaskC; if (ValAdrAltIsNoNULL(Val)) Stat |=CstatDanmaskA
/*
 */
typedef struct TsttarrData {_contArr(Tdata);} TarrData;
/*
 * страница длинных строк
 */
typedef struct TsttLong {
 Tusize size;
 Tusize busy;
 Tusize free;
 Tsize psec[];
 } Tlong;

/* поле ключа или данных
*  описание ключа -56 -80
*/
#define	pValAdrIsNULL(pVal) (AdrIsNULL(pValAdr(pVal)))
#define	pValAdrIsNoNULL(pVal) (AdrIsNoNULL(pValAdr(pVal)))
#define	ValAdrIsNULL(Val) (AdrIsNULL(ValAdr(Val)))
#define	ValAdrIsNoNULL(Val) (AdrIsNoNULL(ValAdr(Val)))

#define	pValAdrAltIsNULL(pVal) (AdrIsNULL(pValAdrAlt(pVal)))
#define	pValAdrAltIsNoNULL(pVal) (AdrIsNoNULL(pValAdrAlt(pVal)))
#define	ValAdrAltIsNULL(Val) (AdrIsNULL(ValAdrAlt(Val)))
#define	ValAdrAltIsNoNULL(Val) (AdrIsNoNULL(ValAdrAlt(Val)))

#define	pValAdrAltAdrIsNULL(pVal) (pValAdrIsNULL(pVal)) && (pValAdrAltIsNULL(pVal))
#define	pValAdrAltAdrIsNoNULL(pVal) (pValAdrIsNoNULL(pVal)) || (pValAdrAltIsNoNULL(pVal))

#define	pValKeyIsNULL(pVal) (DataIsNULL(pValKey(pVal)))
#define	pValKeyIsNoNULL(pVal) (DataIsNoNULL(pValKey(pVal)))

#define	pValDataIsNULL(pVal) (DataIsNULL(pValData(pVal)))
#define	pValDataIsNoNULL(pVal) (DataIsNoNULL(pValData(pVal)))
#define	ValDataIsNULL(Val) (DataIsNULL(ValData(Val)))
#define	ValDataIsNoNULL(Val) (DataIsNoNULL(ValData(Val)))

#define	pValALLIsNULL(pVal) pValDataIsNULL(pVal) && pValAdrAltAdrIsNULL(pVal)
#define	pValALLIsNoNULL(pVal) pValDataIsNoNULL(pVal) || pValAdrAltAdrIsNoNULL(pVal)

#define	pValAdrSetNULL(pVal) (AdrSetNULL(pValAdr(pVal)))
#define	pValAdrAltSetNULL(pVal) (AdrSetNULL(pValAdrAlt(pVal)))
#define	ValAdrSetNULL(Val) (AdrSetNULL(ValAdr(Val)))
#define	ValAdrAltSetNULL(Val) (AdrSetNULL(ValAdrAlt(Val)))

#define	pValKeySetNULL(pVal) DataSetNULL(pValKey(pVal))
#define	ValKeySetNULL(Val) DataSetNULL(ValKey(Val))
#define	pValDataSetNULL(pVal) DataSetNULL(pValData(pVal))
#define	ValDataSetNULL(Val) DataSetNULL(ValData(Val))

#define	pValDataAdrSetNULL(pVal) memNULL(&(pVal->val))
#define	ValDataAdrSetNULL(Val) memNULL(&(Val.val))
 
#define	pValALLSetNULL(pVal) memNULL(pVal)
#define	ValALLSetNULL(Val) memNULL(&(Val))
/*
 * Дерево
 * limitViewlong - количество просматриваемых страниц при поиске пустого места
 */ 
typedef struct {_contArr(TAdrOfferr);} TtrackDb;
/*
*/
#define _contDb enum TtypeDb typeDb; TmapLong mlong; Tmap mpg ;TtrackDb trackdb

typedef struct TstDb {
 _contDb;
 } TDb;
 
/**/
typedef struct {
 TDb *pDb;
 TarrData Inds;
 Tval *pkeydata;
 int Level;
 int isAlt;
 void *pUser;
 } TtreeData;
 
#define EroundIsAlt(Alt) Alt
/* 
#define	pdbArrFree(pdbArr) mapFree((pdbArr)->mlong);mapFree((pdbArr)->marr)
#define	dbArrFree(dbArr) mapFree((dbArr).mlong);mapFree((dbArr).marr)
 
#define	pDbFree(pDb) arrFree(pDb->trackdb);mapFree(pDb->mpg);pdbArrFree(pDb)
#define	DbFree(Db) arrFree(Db.trackdb);mapFree(Db.mpg);dbArrFree(Db)
*/
typedef struct TstadrOffDb {
 _contAdrOff;
 TDb *pDb;
 } TadrOffDb;
/*
 *   ******Заголовок файла Глобали********
 */ 
typedef struct {
 Tusize sizepg;
 Tusize sizelong;
 enum TtypeDb typeDb;
 } TdbHead;
 
/* Итераторы */ 
typedef struct {TAdrOfferr AdrOfferr;TDb *pdb;Tval val;char type;} TIterNext;

typedef struct {_contArr(TIterNext);} TarrIterNext;

//#define pValRefLong(pDb,pVal) pValLongRef(&(pDb->mlong),pVal)
//#define ValRefLong(pDb,Val) pValLongRef(&(pDb->mlong),&(Val))
 
/*
 * по Имени Глобали сформировать имя файла Глобали

 */
//#define getPachtDirExt(pname,pDir,Cext,pOut) snprintf(pOut,CsizePacthFl,"%s%c%s%c%s",pDir,CdelimFile,(pname)->psec,CdelimFile,Cext)
#define getPachtDir(pname,pDir,pOut) pOut[0]=CdelimKat; pOut[1]=CdelimFile;snprintf(pOut+2,CsizePacthFl-2,"%s%c%s%c",pDir,CdelimFile,(pname)->psec,CdelimFile)

#define CisFree 0
#define CisBusy 1

#define objIsFree(pObj) (pObj)->isbusy==CisFree
#define objIsBusy(pObj) (pObj)->isbusy
#define objSetFree(pObj) (pObj)->isbusy=CisFree
#define objSetBusy(pObj) (pObj)->isbusy=CisBusy
#define objAdd(pObj) (pObj)->isbusy++
#define objSub(pObj) if ((pObj)->isbusy) (pObj)->isbusy--
 
#define objGetSecs(Arr,pObj) pObj=NULL; RegIter(Arr,pObj) if ( objIsFree(pObj) ) break; if ((pObj)&&( objIsFree(pObj) )) {objSetBusy(pObj);} else pObj=NULL
#define objGetpSecs(pArr,pObj) pObj=NULL; pRegIter(pArr,pObj) if ( objIsFree(pObj) ) break; if ((pObj)&&( objIsFree(pObj) )) {objSetBusy(pObj);} else pObj=NULL
/*
 * Буфера
 */
#define _contbuf _contArea(char)
    
typedef struct {
 _contbuf;
 } Tbuf;

#define	CsizesecsBufDef 16
#define	CsizeBuf 512
 
#define pBufInit(pBuf)  pareaInit(pBuf,CsizeBuf)
#define BufInit(Buf)  areaInit(Buf,CsizeBuf)
 
#define BufFree(Buf)  areaFree(Buf)
#define pBufFree(pBuf)  pareaFree(pBuf)
 
#define BufClear(Buf)  areaClear(Buf)
#define pBufClear(pBuf)  pareaClear(pBuf)
#define pBufBusyTop(pBuf,Count,pTop) pareaCharBusyTop(pBuf,Count,pTop)
#define BufBusyTop(Buf,Count,pTop) areaCharBusyTop(Buf,Count,pTop)
 
#define pBufBusy(pBuf,Count) pareaCharBusy(pBuf,Count)
#define BufBusy(Buf,Count) areaCharBusy(Buf,Count)

#ifdef	__cplusplus
}
#endif

#endif	/* MSMDATA_H */

