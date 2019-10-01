/* 
 * File:   mshType.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 20 Август 2014 г., 11:49
 */

#ifndef MSHTYPE_H
#define	MSHTYPE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mshHsys.h"
 
typedef int8_t   Tint8;
typedef u_int8_t Tuint8;
typedef int16_t   Tint16;
typedef u_int16_t Tuint16;
typedef int32_t   Tint32;
typedef u_int32_t Tuint32;
typedef int64_t   Tint64;
typedef u_int64_t Tuint64;

typedef int16_t Tsize;
typedef u_int16_t Tusize;
typedef int16_t Terr;
typedef u_int16_t ToffMod;
typedef u_int32_t Toff;

typedef long double TfNum2;
typedef double TfNum;
typedef int64_t TintNum;
typedef u_int64_t ToffData;
typedef u_int64_t Tadr;
typedef u_int64_t TsizePack;
typedef u_int64_t TsizeVoid;
//поиск половинным делением, если количество ключей меньше этого порога, ищем подряд
#define CsumFindKey 4
/*
 *  типы данных
 */
#define  CDataNull 0x0
/*  действительные */
#define  CDataFloat 0x01
/*  целые */
#define  CDataInt 0x02
//в поле длинна массива
//#define  CTypeDataSizeArr 0x03
/*  указатель */
#define  CDataPointer 0x06
/*  ссылка на дерево данных */
#define  CDatanameDb 0x07
//маска Int
#define  CDataMaskInt 0x06
//в поле хранится строка
#define  CDataStr 0x08
//в поле хранится имя Объекта
#define  CDataObj 0x09
//маска Str
#define  CDataStrMask 0x08
//данные хранящиеся вне поля данных
//маска внешних данных
#define  CDataMaskOut 0x10
#define  CDataMaskNoOut (~CDataMaskOut)
//строка pstr
#define  CDatapStr 0x10
//строка TbStr
#define  CDatabStr 0x11
//строка TpbStr
#define  CDatapbStr 0x12
//ссылка на строку в области
#define  CDataArea 0x13
//#define  CTypeDataAreaSize 0x14
//#define  CTypeDatamapArea 0x15
//строка в Long
#define  CDataLong 0x18
//тип данных
#define  CDataSightMask 0x1f
#define  CDataSightUnMask (~CDataSightMask)
//тип данных
//#define  CDataSightAltMask 0x9f
//#define  CDataSightAltUnMask ~CDataSightMask
//маска массива
//#define  CDataTypeArr 0x20
#define  CDataList 0x20
//маска не массива df
#define  CDataNoList ~CDataList
//#define  CDataTypeNoArr ~CDataTypeArr
//маска альтернативного индекса
#define  CDatasubAlt 0x80
//маска Не альтернативного индекса0x7f
#define  CDatasubNoAlt ~CDatasubAlt
//-----------------------
#define	CstyleIndNone 0x0
#define	CstyleIndInd 0x0
#define	CstyleIndArr 0x40
#define	CstyleIndMod 0xc0
#define	CstyleIndMask 0xc0
#define	CstyleIndUnMask ~CstyleIndMask

#define styleGet(St) St & CstyleIndMask
#define styleIsArr(St) (styleGet(St))==CstyleIndArr

//
//#define	CvmIniComment '#'
#define	CvmIniEqu '='
#define	CvmIniendStr ';'
#define	CvmCommentSlash '/'
#define	CvmCommentAs '*'
/*
*  статусы вершины
*  вершины нет
*/
#define  CstatDan0 0
/* данные есть потомков нет */
#define  CstatDanD 1
/* данных нет потомки есть */
#define  CstatDanC 2
/* данные есть потомки есть */
#define  CstatDanCD 3
/* данных нет потомков нет объекты есть */
#define  CstatDanA 4
/* данные есть потомков нет объекты есть */
#define  CstatDanAD 5
/* данных нет потомки есть объекты есть */
#define  CstatDanAC 6
/* данные естьпотомки есть объекты есть */
#define  CstatDanACD 7
/* Маска есть данные */
#define  CstatDanmaskD 1
/* маска есть потомки */
#define  CstatDanmaskC 2
/* маска есть объекты */
#define  CstatDanmaskA 4
/*
 *  тип страницы 
 */
#define  CTypePgNone 0x0
#define  CTypePgFree 0x1
#define  CTypePgLink 0x2
#define  CTypePgData 0x4
/*
 * что удалять
 */
#define CtypeKiLL 0x70
#define CtypeKiLLData 0x10
#define CtypeKiLLDown 0x20
#define CtypeKiLLAlt 0x40
#define CtypeKiLLAll 0x80
//
#define CtypesKiLL 'A'
#define CtypesKiLLData 'D'
#define CtypesKiLLDown 'C'
#define CtypesKiLLAlt 'P'
#define CtypesKiLLAll '*'
/*
 */
#define CcmdNext 'N'
#define CcmdBack 'B'
#define CcmdQuery 'Q'
    
#define	CsizetreeDataDef  8
//размер стека спуска для обхода одного индекса дарева
#define	CountSecLayoutDef 8
//
#define	ClimitLongDef 200

#define CcountPgDef 2
#define CcountLongDef 1
    
//конец строки
#define	CCvmEndStr '\n'
#define	CCvmBK '\r'
#define	CCvmGT '\t'
#define	Cvm0 '\0'
//обмен таблицей по сети
#define CbegDataTree '<'
#define CendDataTree '>'
//
#define CrazRow '\n'
#define CrazCol '\t'
//разделитель аргументов команды
#define	CtrKodSpace ' '
#define	CtrKodGtab '\t'
//дробная часть числа
#define	CtrKodFloatPoint '.'
    
#define	CsizePartDef 6
//#define CdirGlb "mdb"
#define CsizePacthFl 256
//длинна мантисы
#define	ClongMantisa 16

//количество имен в таблице Глобальных файлов
#define	CvmasumIniMapDef 8
//количество имен в таблице Глобальных файлов
#define	CvmasumIniBufDef 512
//размер области путей в таблице Глобальных файлов
#define	CvmasizeIniMapDef 320
//количество Tdata элементов в буфере модуля
#define	CvmasizeModPackDef 64
//
#define	CsizeIterQueryrDef 4
//
#define	CsizeIterQueryrSaveDef 4
    
#define	CvmOpPlus '+'
#define	CvmOpMinus '-'
#define	CvmOpPoint '.'
 
#define	CsizeData 24
#define	CsizeDataBusy CsizeData - 2
#define ChightData CsizeData - 1
#define ChightData1 ChightData - 1
/* 18 байт строка в данных */
#define	CCsizeStr CsizeData-sizeof(Tsize)-2
const static uint CsizeStr=CCsizeStr;
const static int CsizeLabel=CCsizeStr - 1;

#define	CCsizeStrShort 4
/*  размеры полей данных */
/* 4 */
#define	CcountInt CsizeData/sizeof(TintNum) - 1
/* 16 */
#define CcountInt8 CsizeData/sizeof(Tint8) - 2
#define CcountInt16 CsizeData/sizeof(Tint16) - 1
#define CcountInt32 CsizeData/sizeof(Tint32) - 1
#define CCcountArg CsizeData/sizeof(Tint16)
#define CCcountArg1 CCcountArg-1
#define CCcountArg2 CCcountArg-2
const static uint CcountArg=CCcountArg;
    
/*
 *   работа с внешеими устройствами
 */
const static char CdelimFile='/';
const static char CdelimExt='.';
const static char CdelimKat='.';
const static char CprsEndStr='\n';
//def Ini
#define CiniDelemiter '='
#define CinibegRow '<'
#define CiniendRow '>'
#define CiniendCol ','
#define CiniendPar ';'
//debag 
#define	CDebagAdrTypeInd 'i'
#define	CDebagAdrTypePg 'p'
#define	CDebagAdrTypeArea 'a'

//разделитель алтернативного списка
#define	CKodDlmAlt ':'
//разделитель списка 
#define	CKodDlmArg ','

 
#define CinoutfOk 1
#define CinoutfNo 0

#define  CZERO '\x0000'
#define  CnotAdr 0
 
#define EroundNoAlt 0
#define EroundAlt 0x01

 #define CnumModPgDef 16
 #define CnumModLongDef 800

//#define mshDEBAG
    
//#undef mshDEBAG
#ifdef	mshDEBAG
 #define CnumPgDef 5
 #define CnumArrDef 10
 #define CnumLongDef 800
/*
// offsetof(Tstr,psec)
 #define CsizeDbPgDef 400
 #define CsizeDbLongDef 800
 #define CsizeArrPgDef CnumArrDef * sizeof(Tdata)
 #define CsizeArrLongDef CsizeDbLongDef
 #define CsizeLongDef CsizeDbLongDef
 #define CsizeModPgDef 400
 #define CsizeModLongDef 400
*/
#else
 #define CnumPgDef 128
 #define CnumArrDef 32
 #define CnumLongDef 1024 * 8
 /*
 #define CsizeArrDef 100

 #define CsizePgDef 1024*8
 #define CsizeDbLongDef 1024*8
 #define CsizeArrPgDef CsizeArrDef * sizeof(Tdata)
 #define CsizeArrLongDef CsizeDbLongDef
 #define CsizeLongDef CsizeDbLongDef
 #define CsizeModPgDef 10 * sizeof(Tval) + sizeof(TpgData)
 #define CsizeModLongDef 800
*/
#endif

enum TtypeDb{
 EtypeNo,
 EtypeDb,
 EtypeArr,
 EtypeLong
 };
 
enum TisGlb{
 EisGlbNo,
 EisGlb
 };
/*

enum TisArr{
 EisArrNo,
 EisArr
 };

enum TisLong{
 EisLongNo,
 EisLong
 };
*/
enum TisSet{
 EisSetNo,
 EisSet
 };

#ifdef	__cplusplus
}
#endif

#endif	/* MSHTYPE_H */

