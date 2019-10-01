/* 
 * File:   mshvmCnst.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 21 Август 2014 г., 14:33
 */

#ifndef MSHVMCNST_H
#define	MSHVMCNST_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef CDBL_EPSILON
#define	CDBL_EPSILON ((double)2.22044605e-16L)
#endif
    
static u_int16_t CvmArrOffNull= -1;

//
#define CidirDbLoc 0
#define CidirDbGlb 1
#define CidirDbTmp 2
//
#define	CvmpTCPNULL 0
#define	CvmpThreadNULL 0
#define	CvmpTCP -2
 /*
 * Коды операций
 */ 
#define	CvmOpPlus '+'
#define	CvmOpMinus '-'
#define	CvmOpMulti '*'
#define	CvmOpDiv '/'
#define	CvmOpDivInt '\\'
#define	CvmOpMod '#'
#define	CvmOpStr '_'
#define	CvmOpOr '|'
#define	CvmOpAnd '&'
#define	CvmOpNot '~'
#define	CvmOpNotAnd '^'
#define	CvmOpEqu '='
#define	CvmOpHight '>'
#define	CvmOpLow '<'
#define	CvmOpUsr '`'
/* 
 * основные терминальные символы
 */
//входной буфер пуст
//код кавычки
#define	CCtrKodKav1 '\''
//код кавычки
#define	CCtrKodKav2 '"'
//признак метки
#define	CCtrKodLabel ':'
//признак условия в команде
#define	CtrCmdCond '?'
#define	CtrCmdCondFalse '!'
//признак операции расширения
#define	CtrKodUsr '`'
//признак начала массива
#define	CtrKodArr '$'
//признак Аргументов
//#define	CtrKodArg '@'
// --- признак данных Job ---
#define	CtrTypeDbTemp '%'
#define	CtrTypeDbGlb '^'
//#define	CtrKodTypeDoLast '!'
//#define	CtrKodTypeSys '%'
//признак внутренней функции и свойства
#define	CtrKodTypeInner '%'
//скобки ()
#define	CtrKodBktBeg '('
#define	CtrKodBktEnd ')'
//индекс []
#define	CtrKodIndBeg '['
#define	CtrKodIndEnd ']'
//параметры функции []
#define	CtrKodFunBeg '['
#define	CtrKodFunEnd ']'
//список {}
#define	CtrKodListBeg '{'
#define	CtrKodListEnd '}'
//разделитель списка 
#define	CtrKodDlmList ','
//конец команды
#define	CtrKodEndCom ';'
//Код команды отделяется от аргументов 
#define	CtrKodDlmCom ' '
//
#define	CtrKodDlmSubArg ':'
//разделитель в присвоении
#define	CCtrKodEqu '='
const static char CtrKodEqu=CCtrKodEqu;
//разделитель алтернативного списка
#define	CtrKodDlmAlt ':'
//разделитель списка 
#define	CtrKodDlmArg ','
//
//разделитель свойств
#define	CtrKodMod '.'
#define	CtrKodArg 'a'
//
#define	CtrKodCmt '/'
#define	CtrKodAst '*'
//
#define	CvmPsevdoArg 'A'
#define	CvmPsevdoJob 'J'
#define	CvmPsevdoBuf 'B'
//
#define	CctrTrmCnstEnd CtrKodSpace,CCtrKodLabel,CCvmGT,CCtrKodEqu,CtrKodDlmAlt,CtrCmdCond\
,CtrKodArr,CtrTypeDbTemp,CtrTypeDbGlb,CtrKodMod\
,CtrKodBktBeg,CtrKodBktEnd,CtrKodIndBeg,CtrKodIndEnd,CtrKodListBeg,CtrKodListEnd,CtrKodDlmList,CtrKodEndCom,CtrCmdCondFalse,CCvmEndStr,CCvmBK,Cvm0

//,CtrKodArr,CtrKodTypeDb,CtrKodMod,CtrKodTypeDoLast\

//,CtrKodArr,CtrKodArg,CtrKodTypeDb,CtrKodMod,CtrKodTypeDoLast\

#define	CctrTrmNumEnd CctrTrmCnstEnd
#define	CctrTrmIDEnd CctrTrmCnstEnd
#define	CctrTrm CtrTypeDbTemp,CtrTypeDbGlb,CtrKodArr,CtrKodBktBeg,CtrKodBktEnd,CtrKodIndBeg,CtrKodIndEnd,CtrKodListBeg,CtrKodListEnd,CtrKodDlmList\
,CCtrKodLabel,CtrKodMod,CtrCmdCond,CtrCmdCondFalse,CtrKodEndCom,Cvm0

//
const static char CtrEndNum[]={CctrTrmNumEnd};
const static char CtrEndCnstEnd[]={CctrTrmCnstEnd};
const static char CtrEndID[]={CctrTrmIDEnd};
const static char CtrTrm[]={CctrTrm};
//

#define  CtrExpStatExp 'e'
#define  CtrExpStatRef 'r'
//#define  CtrExpStatFunName 'F'
#define  CtrExpStatFunMod 'M'
#define  CtrExpStatFunInd 'I'
//#define  CtrExpStatFunPar 'P'
#define  CtrExpStatBkt '('
#define  CtrExpStatInd '['
#define  CtrExpStatList '{'
#define  CtrExpStatArr '$'
//#define  CtrExpStatArg '@'
#define  CtrExpStatGlb '^'
 #define  CtrExpStatCondTrue CtrCmdCond
#define  CtrExpStatCondFalse CtrCmdCondFalse
#define  CtrExpStatSet 's'
/*
 * типы Термов
 */
#define	CtrTypeTerm0 0x0
//константа
#define	CtrTypeTermCnst 0x04
//идентификатор
#define	CtrTypeTermID 0x05
//идентификатор
#define	CtrTypeTermExtFun 0x06
//Целое число
#define	CtrTypeTermInt 0x010
//Дробное число
#define	CtrTypeTermFloat 0x11
//код операции
#define	CtrTypeTermOp '+'
// тип команды End
#define	CprsTypeItrDef '0'
// команда If
#define	CtrTypeItrIf 'I'
// команда Case
#define	CtrTypeItrCase 'C'
// команда For
#define	CtrTypeItrFor 'F'
// команда Next
#define	CtrTypeItrNext 'N'
// команда Query
#define	CtrTypeItrQuery 'Q'
//Операции в VM
const static char CvmOp[]={CvmOpPlus,CvmOpMinus,CvmOpMulti,CvmOpDiv,CvmOpDivInt,CvmOpMod,CvmOpStr,CvmOpOr,CvmOpAnd
 ,CvmOpNot,CvmOpEqu,CvmOpHight,CvmOpLow,CvmOpUsr,Cvm0};
const static char CvmOp2[]={CvmOpPlus,CvmOpMinus,CvmOpMulti,CvmOpDiv,CvmOpDivInt,CvmOpMod,CvmOpStr,CvmOpOr,CvmOpAnd
 ,CvmOpNot,CvmOpEqu,CvmOpHight,CvmOpLow,CvmOpUsr,CvmOpNotAnd,Cvm0};
const static char *pCvmOpUsrData="iIsSwWcC";
 /* идентификация файла модуля */
const static char* pCmshMagVM="MSHVM011";
 /* идентификация файла Исходного текста */
const static char* pCmshMagMsh="MSH01001";
 /* идентификация файла модуля */
const static char* pCmshDirMsh="msh";
const static char* pCmshDirVm="mvm";
const static char* pCmshDirAsm="mav";
//const static char* pCmshExtMsh="msh";
/* идентификация файла модуля */
const static char* pCmshExtVm="mvm";
//const static char* pCmshExtAsm="avm";
const static char* pCmshExtCnst="cnt";
/* файлы Ini */
const static char* pCmshExtIni="ini";
const static char* pCmshDirIni="ini";
/* файлы Img */
const static char* pCmshExtImg="img";
/* файлы Txt */
const static char* pCmshExtTxt="txt";
const static char* pCmshDirTxt="txt";
/* имя конструктора объекта */
const static char* pConstructor="New";
//
#define	CsizeCodeOpDef 4

#define	CsizePartModDef 6
//
#define	CsizePartGlbDef 6
#define	CsizePartLocDef 4
//размер стека страниц для записи
#define	CvmIndsDef 64
#define	CvmIndsct0Def 16
//#define	CvmThisDef 16
//#define	CvmThisct0Def 4
#define	CvmIndsct1Def 4
#define	CvmrefDbDef 4
#define	CvmsizeSteckIndDef 8
#define	CvmsumIterQueryIndDef 8
#define	CvmsizeIterQueryDef 8
#define	CvmsizeArrRunDef 32
#define	CvmsizeAreaTempsDef 512
#define	CvmsizetemparrDataDef 16
//
#define	CvmsizeboxDoDef 8

#define	CvmsizeBufPckDef 512

#define	CvmsizeArrJobDef 32
//количество загруженных Модулей
#define	CvmsumModDef 4
//
#define	CvmsumarrinFuncDef 4
//
#define	CvmsizeboxAppDef 4
//
#define	CvmsumLockDef 4
#define	CvmsumLockResDef 256
//размер таблицы Глобалей
#define	CarrDbsizeDef 8
//
//количество curQuery в блоке выполнения
#define	CvmsizeQueryDef 4
//количество индексов внутри curQuery
#define	CvmsumQueryDef 4
//размер области curQuery
#define	CvmsizeAreaDef 512
//размер области curQuery
#define	CvmsizeAreaModDef 64
//размер временной области
#define	CvmsizeTempDef CsizepgLongDef
//количество страниц данных в блоке буфера
#define	CvmcapaPgDef 4
//количество длинных страниц в блоке буфера
#define	CvmcapaLongDef 4
//лимит страниц данных в блоке буфера
#define	CvmlimitPgDef 128
//лимит длинных страниц в блоке буфера
#define	CvmlimitLongDef 128
//
#define	CvmsumArgsDef 8
#define	CvmsumArgsBoxDef 8
//
#define	CvmsumObjsDef 8
//
#define	CvmsumArrTempDef 8
//количество Значений внутри стека Данных
#define	CvmsumDanDef 4
//количество предков у модуля
#define	CvmsumExtDef 4
//количество локальных секций меток
#define	CvmsizeLabSecDef 4
//количество Значений внутри стека Данных
#define	CvmsumRblDef 32
//количество событий приложения
#define	CvmsumEvntDef 4
#define	CvmsumEvntBackDef 8
#define	CvmsumEvntWaitDef 8
#define	CvmsizeEvntWaitDef 8
#define	CvmsumBoxEvntDef 8
#define	CvmsizeBoxEvntDef 8
#define	CvmsizeEvntArgDef 8
#define	CvmsumextFunDef 4
//размер таблицы файлов
#define	CvmsumFileDef 4
//количество зарезервированых ID
#define	CminIdFile 100
//
#define	CsizeFileNamesDef 512
//
#define	CsizeWWWbufDef 1024

#define	CsizeLabelDef 32
#define	CsizeCtrlLabelDef 4

#define CsizeIterDef 4
#define CsizeCtrlIterDef 32

#ifndef __DBL_EPSILON__
#define	__DBL_EPSILON__=((double)2.22044604925031308085e-16L)
#endif	/* __DBL_EPSILON__ */

//количество Tsize во временной области Do
#define	CsizeAreaTempDef 512

#define	CountVoidDef 4
//количество заданий
#define	CountJobDef 8
//количество блоков Run
#define	CountRunDef 16

#define	CtrsizetrOpDef 8
#define	CtrsizetrStstkCnstDef 8
#define	CtrsizeTtrStDef 32
#define	CtrsizeTtrStCtrlDef 8
#define	CsizeBoxIterDef 8
#define	CsizeArrIfCaseDef 8
#define	CsizeBoxIterCmdDef 64
#define	CsizeBoxIterSubCmdDef 8
//количество меток в программе
#define	CtrSizeLabelDef 8
//количество меток в программе
#define	CtrSum2LabelDef 4
 //количество модулей расширения в программе
#define	CtrSizeExtDef 4
 //количество данных в программе
#define	CtrSizeDataDef 128
//размер области длинных строк
#define	CtrSizeAreaLongDef CsizeLongDef
//глубина вложений инструкций Include
#define	CtrSumIncludeDef 4
 //количество констант в программе
#define	CtrSizeConstDef 128
//количество команд в программе
#define	CtrSizeCmdDef 512
#define	CtrSizeCmdTempDef 16
//количество вложений в операции ?
#define	CtrSizeYclDef 4

#define	CsizeSteckDef 8
//typedef int idSocket;
#define CalphaHighAlpha 122
#define CalphaLow 97
#define Calpha1 CalphaHighAlpha - CalphaLow
#define Calpha2 (Calpha1) * (Calpha1)

/*   ********************************
 *     типы VM
 */
#define iInt16max 32767
#define iInt16min -32767
/*
 *  Команды и субКоманды VM
 */
/*
 *  Команды и субКоманды VM
 * ABNQELRDCHGSKJX 0 cgfjitonblxyzqrp
 * TPZVMOWFUY adwtushkvem 12345678
 */
//Регистровые операции
#define CvmCmdReg 'r'
 //увеличить уровень стека
 #define  CvmSubCmdRegUp 0x01
 //переслать данные с регистров на регистр индекса   
 #define  CvmSubCmdRegMove 0x02
 //#define  CvmSubCmdRegClear 'c'
 //увеличить уровень стека 2-го уровня
 //#define  CvmSubCmdRegUp1 0x03
//уменьшить уровень стека
 #define  CvmSubCmdRegPop 0x03
 //увеличить уровень стека 2-го уровня
 //#define  CvmSubCmdRegPop1 0x05
 //последнему индексу присвоить признак альтернативного
 #define  CvmSubCmdRegAlt 0x04
    
#define  CvmSubCmdRegMask 0x07
#define  CvmSubCmdRegUnMask ~CvmSubCmdRegMask
//#define  CvmSubCmdRegPop 'p'
#define regCmdGet(Reg) (Reg) & CvmSubCmdRegMask
#define regCmdSet(Reg,Data) (Reg)=((Reg) & CvmSubCmdRegUnMask) | ((Data) & CvmSubCmdRegMask)
#define regCmdpCmd(pCmd) (pCmdSub(pCmd)) & CvmSubCmdRegMask
//Текущую ссылку на данные запомнить в массиве
#define CvmCmdRefToArr 'p'
 //маркер начала выражения
#define CvmCmdMarker 'e'
//загрузка констант из модуля
#define CvmCmdLoadConst 'c'
// Команды работы с Деревом Данных
/*  Get - 'g' */
#define CvmCmdGet 'g'
//получить данные из массива
//  команды исходной программы
/* KiLL - 'K' */
#define	CvmCmdKiLL 'K'
//Set -'S'
#define CvmCmdSet 'S'
//короткий Set -'s' без правой части
#define CvmCmdSetSh 's'
/*
#define CcmdNext 'N'
#define CcmdBack 'B'
#define CcmdQuery 'Q'
 */ 
 //субкоманда одной итерации
 #define CvmCmdSubNext1 0x01
 //субкоманда инициализации итератора
 #define CvmCmdSubInit 0x02
 //субкоманда выполнения итератора
 #define CvmCmdSubNext 0x04
 //
 #define CvmCmdSubNextMask 0x07
 //субкоманда выполнения итератора
 #define CvmCmdSubSaveInd 0x08

#define pCmdCmdSubNext(pCmd) pCmdSub(pCmd) & CvmCmdSubNextMask
#define pCmdCmdSubNextSave(pCmd) pCmdSub(pCmd) & CvmCmdSubSaveInd
#define pCmdCmdSubNextNoSave(pCmd) (pCmdSub(pCmd) & CvmCmdSubSaveInd)==0
 //Команда перехода
 #define CvmCmdGo 'G'
 //Команда Case
 #define CvmCmdCase 'J'
 //Команда TimeOut
 #define CvmCmdTimeOut 'H'
 //Команды COPY,MOVE
 #define CvmCmdCopy 'C'
 //субКоманды
 #define CvmSubCmdCopyDelTo 0x01
 #define CvmSubCmdCopyDelFrom 0x02
 //Команда выполнить строку как MSH команду
 #define CvmCmdExecute 'X'
//вызывается подпрограмма
#define	CvmCmdDO 'D'
/*    
 #define	CvmSubCmdDODo 0x0
 #define	CvmSubCmdDORet 0x01
 #define	CvmSubCmdDOJob 0x02
 #define	CvmSubCmdDOSet 0x04
 #define	CvmSubCmdDOMask 0x07
 #define	CvmSubCmdMaskNoRet 0xfe
*/
/*
#define CtypeKiLL 0x38
#define CtypeKiLLData 0x08
#define CtypeKiLLDown 0x10
#define CtypeKiLLAlt 0x20

#define CtypesKiLL 'A'
#define CtypesKiLLData 'D'
#define CtypesKiLLDown 'C'
#define CtypesKiLLAlt 'P'
 *
#define CtypeKiLL 0x70
#define CtypeKiLLData 0x10
#define CtypeKiLLDown 0x20
#define CtypeKiLLAlt 0x40
#define CtypeKiLLAll 0x80
 #define	CvmSubCmdDOKill CtypeKiLL
 #define	CvmSubCmdDOKillData CtypeKiLLData
 #define	CvmSubCmdDOKillDown CtypeKiLLDown
 #define	CvmSubCmdDOKillAlt CtypeKiLLAlt
*/    
//

 #define	CvmSubCmdDOInProp 0x10
 #define	CvmSubCmdDOInFun 0x20
 #define	CvmSubCmdDOInWin 0x30
 #define	CvmSubCmdDOInStat 0x40
 #define	CvmSubCmdDOInMetod 0x50

 #define	CvmSubCmdDOMaskIn 0xf0
//
 #define cmdDoIsIn(subCmd) (subCmd) & CvmSubCmdDOMaskIn
 //вызывается подпрограмма
//#define	CvmCmdDO 'D'
 #define	CvmSubCmdDODo 0x0
 #define	CvmSubCmdDOSet 0x01
 #define	CvmSubCmdDOKill 0x02
 #define	CvmSubCmdDOJob 0x03
 #define	CvmSubCmdDOthis 0x04
 #define	CvmSubCmdDORet 0x08
 #define	CvmSubCmdDOMask 0x0f
 
 #define cmdIsDo(subCmd) (subCmd) & CvmSubCmdDOMask
    
 #define cmdDoIsSet(subCmd) (cmdIsDo(subCmd)) == CvmSubCmdDOSet
 #define cmdDoIsKill(subCmd) (cmdIsDo(subCmd)) == CvmSubCmdDOKill
 #define cmdDoIsJob(subCmd) (cmdIsDo(subCmd)) == CvmSubCmdDOJob
 #define cmdDoIsThis(subCmd) (subCmd) & CvmSubCmdDOthis
 #define cmdDoIsRet(subCmd) (subCmd) & CvmSubCmdDORet
//
 #define cmdDoSetThis(Cmd) (CmdSub(Cmd)) |= CvmSubCmdDOthis
 //#define subcmdDoSetThis(subCmd) (subCmd) |= CvmSubCmdDOthis
 #define cmdDoSetRet0(Cmd) (CmdSub(Cmd)) &= (~CvmSubCmdDORet)
 #define subcmdDoSetRet0(subCmd) (subCmd) &= (~CvmSubCmdDORet)
// #define cmdDoSetSet(subCmd) (subCmd) | CvmSubCmdDOSet
// #define cmdDoSetKill(subCmd,St) (subCmd) | St
 //
//Команды обработки событий
//Возврат из функции
#define	CvmCmdReturn 'R'
//Команда Lock
#define CvmCmdLock 'L'
 #define CvmSubCmdLockW '\1'
 #define CvmSubCmdLockR '\2'
 #define CvmSubCmdLockU '\3'
 #define CvmSubCmdLockMask 0x03
 #define cmdLock(subCmd) (subCmd) & CvmSubCmdLockMask
 #define cmdSubpCmdLock(pCmd) cmdLock(pCmdSub(pCmd))
 //#define CvmSubCmdLockS 'S'
//Event
#define CvmCmdEvent 'E'
 #define CvmSubCmdEventCall '\1'
 #define CvmSubCmdEventWait '\2'
 #define CvmSubCmdEventTrap '\3'
 #define CvmSubCmdEventDelete '\4'
 #define CvmSubCmdEventMask 0x07
 #define cmdEvent(subCmd) (subCmd) & CvmSubCmdEventMask
 #define cmdSubpCmdEvent(pCmd) cmdEvent(pCmdSub(pCmd))
//команды управления Устройствами
#define CvmCmdFile 'f'
//Команда Open
#define CvmSubCmdNone 0x0
#define CvmSubCmdOpen 0x01
#define CvmSubCmdNew 0x02
#define CvmSubCmdClose 0x03
#define CvmSubCmdUse 0x04
#define CvmSubCmdWrite 0x05
#define CvmSubCmdRead 0x06
#define CvmSubCmdWWW 0x07
#define CvmSubCmdMask 0x07
#define CcmdFile(subCmd) (subCmd) & CvmSubCmdMask
#define CcmdSubpCmdFile(pCmd) CcmdFile(pCmdSub(pCmd))
/* Остальные Команды */
//перейти на команду при условии
#define CvmCmdJmp 'j'
//перейти при любом условии
#define	CvmJmp0 0x0
#define	CvmJmp1 0x01
#define	CvmJmpAny 0x02
/*
 *  встроенные свойства
 * idJob, idDo, idDiv, statDo, isTr, this,stat
 */
 static char *pCvmInnerProp[]={"err","this","queryKey","queryData","Data","statData","dmtIO","nameMod","idDiv","isTr","idJob","idDo","statDo","eof",""};
 //
 enum {
 CvmTypePropErr,
 CvmTypePropThis,
 CvmTypePropqueryKey,
 CvmTypePropqueryData,
 CvmTypePropData,
 CvmTypePropStat,
 CvmTypePropdelemIO,
 CvmTypePropnameMod,
 CvmTypePropIdDiv,
 CvmTypePropIsTr,
 CvmTypePropIdJob,
 CvmTypePropIdDo,
 CvmTypePropStatDo,
 CvmTypePropEof
 };
/*
 *  встроенные Функции
 */
 static char *pCvmInnerFunct[]={"findLabel","sysLockR","sysLockW","sysLockSt"
         ,"sysCreateDir","sysDeleteDir","sysReadDir","sysReadDirFile","sysReadDirR","sysReadDirFileR","sysDeleteFile"
         ,"sysJobStatus","sysJobErr","sysJobValue","sysCompile",""};
 //
  enum {
   CvmTypeFunctfindLabel,
   CvmTypeFunctLockR,
   CvmTypeFunctLockW,
   CvmTypeFunctLockSt,
   CvmTypeFunctCreateDir,
   CvmTypeFunctDeleteDir,
   CvmTypeFunctReadDir,
   CvmTypeFunctReadDirFile,
   CvmTypeFunctReadDirR,
   CvmTypeFunctReadDirFileR,
   CvmTypeFunctDeleteFile,
   CvmTypeFunctjobStatus,
   CvmTypeFunctjobErr,
   CvmTypeFunctjobValue,
   CvmTypeFunctCompile
   };
/*
 *  свойства состояния узла дерева
 */
 static char *pCvmInnerStat[]={"type", "stat", "byte", "size", ""};
 //
  enum {
   CvmTypeStatType,
   CvmTypeStatStat,
   CvmTypeStatByte,
   CvmTypeStatSize
   };
/*
 *  встроенные Методы
 */
 
 static char *pCvmInnerMetod[]={"strGet","strCode","strIns","strSet","strDel","strFind","strRepl","strReplALL"
  ,"listGet","listPush","listIns","listSet","listDel","listCount"
  ,"fieldGet","fieldPush","fieldIns","fieldSet","fieldDel","fieldCount"
  ,"v64","vu64","v32","vu32","v16","vu16","v8","vu8"
  ,"strPop","listPop","fieldPop","iniRead","iniWrite","objNew","sysQuery","arrPush","arrPop"
  ,""};

  enum {
   CvmTypeMetodGetStr,
   CvmTypeMetodCodeStr,
   CvmTypeMetodInsStr,
   CvmTypeMetodSetStr,
   CvmTypeMetodDelStr,
   CvmTypeMetodFindStr,
   CvmTypeMetodReplStr,
   CvmTypeMetodReplALLStr,
   CvmTypeMetodGetList,
   CvmTypeMetodPushList,
   CvmTypeMetodInsList,
   CvmTypeMetodSetList,
   CvmTypeMetodDelList,
   CvmTypeMetodCountList,
   CvmTypeMetodGetField,
   CvmTypeMetodPushField,
   CvmTypeMetodInsField,
   CvmTypeMetodSetField,
   CvmTypeMetodDelField,
   CvmTypeMetodCountField,
   CvmTypeMetodv64,
   CvmTypeMetodvu64,
   CvmTypeMetodv32,
   CvmTypeMetodvu32,
   CvmTypeMetodv16,
   CvmTypeMetodvu16,
   CvmTypeMetodv8,
   CvmTypeMetodvu8,
   CvmTypeMetodPopStr,
   CvmTypeMetodPopList,
   CvmTypeMetodPopField,
   CvmTypeMetodiniRead,
   CvmTypeMetodiniWrite,
   CvmTypeMetodobjNew,
   CvmTypeMetodQuery,
   CvmTypeMetodarrPush,
   CvmTypeMetodarrPop
   };
/*
 *  функции библиотеки Win
 */
 static char *pCvmWin[]={"winInit","winFree","winNew","winSetAtr","winGetAtr","winIniToAtr","winAtrToIni","winOpenChannel","winStart"
         ,""};
 //
  enum {
   CvmTypeFunctWinInit,
   CvmTypeFunctWinFree,
   CvmTypeFunctWinNew,
   CvmTypeFunctWinSetAtr,
   CvmTypeFunctWinGetAtr,
   CvmTypeFunctWinIniToAtr,
   CvmTypeFunctWinAtrToIni,
   CvmTypeFunctWinOpenChannel,
   CvmTypeFunctWinStart,
   CvmTypeFunctWinEnd
   };
 
//Тип файла внутри директории
#define CvmTypeFileFile 'F'
#define CvmTypeFileDir 'D'
//
#define	CvmOpNULL '\0'
//
#define	CvmOpUno '1'
/*
 * статус событий
 */
//инициализировано
#define	CvmStatEvntInit 0x0
//программы обработки ждут возникновения события
#define	CvmStatEvntWait 0x1
//событие произошло
#define	CvmStatEvntExe 0x2
/*
 *   Описание блока выполнения
 * char flag; //флаги блока выполнения
 * flag=
 */
// блок Выполняется
#define	CvmStatDoRun 0x20
// блок ожидает
#define	CvmStatDoWait 0x0
// блок Завершен
#define	CvmStatDoEnd 0x10
//маска очистки старшего полубайта
#define	CvmStatDoNo 0x0f
//признак ссылки на псевдомассивы
//#define	CvmRefArg '@'
//признак ссылки на Job
#define	CvmRefJob '%'
 //признак массива
#define	CvmRefArr '$'
 //признак массива
//#define	CvmRefDoLast '!'
//признак Глобали
#define	CvmKodGlb '^'
#define	CvmKodApp '%'
 /* Имя Аргументов */
const static char* pCvmnameArgCALL="arg";
const static int pCvmnameArgSize=3;
/* префикс функций вызываемых слева от = */
const static char* pCvmprefSet=".";
//префикс методов Удаления
const static char* pCvmprefKill="..";

#define  CvmDataNull '-'
/*  действительные */
#define  CvmDataFloat 'f'
/*  целые */
#define  CvmDataInt 'i'
/*  указатель */
#define  CvmDataPointer 'p'
/*  ссылка на дерево данных*/
#define  CvmDatarefDb 'r'
/*  строка */
#define  CvmDataStr 's'
/*  объект */
#define  CvmDataObj 'o'
/*  массив */
#define  CvmDataList 'a'
/*
 */ 
#define	CretNone 0x0
#define	CretConstToCmd 0x01
#define	CretEndArr 0x02
#define	CretEndInd 0x03
#define	CretEndArg 0x04
#define	CretEndOp 0x05
#define	CretEndCond 0x06
#define	CretEndFun 0x07
/*
 * Что хранится в Индексном регистре
 */
//#define CvmIndTypeInd 0
//#define CvmIndTypeMod 0x08
//#define CvmIndTypeArg 0x10
//#define CvmIndTypeArr 0x18
//#define CvmIndTypeMask 0x18

//#define typeReg(Reg) (Reg) & CvmIndTypeMask
//#define typeRegCmd(pCmd,iCmd) typeReg(pCmdiCmd(pCmd,iCmd))
/*
 * к какой базе обращение
 */
/*
#define CvmIndDbDo 0x0
#define CvmIndDbRun 0x20
#define CvmIndDbRunA 0x40
#define CvmIndDbJob 0x60
#define CvmIndDbApp 0x80
#define CvmIndDbGlb 0xa0
#define CvmIndDbNet 0xe0
#define CvmIndDbMask 0xe0

#define typeDb(Reg) (Reg) & CvmIndDbMask
#define typeDbIs(Reg,regDb) (typeDb(Reg)) == (regDb)
//#define typeDbIsNo(Reg) typeDbIs(Reg,CvmIndDbNo)
#define typeDbIsNet(Reg) typeDbIs(Reg,CvmIndDbNet)
#define typeDbSet(Reg,regDb) (Reg)=( typeDb(Reg) ) | ( regDb )
#define typeDbSetCmd(pCmd,iCmd,regDb) typeDbSet(pCmdiCmd(pCmd,iCmd),regDb)
*/
/*
 *  ---Обмен между регистрами---
 */ 
#define	CregNone 0x0
#define	CregInd 0x08
#define	CregCom 0x10
#define	CregComChar 0x18
#define	CregMod 0x20
#define	CregNULL 0x38
//#define	CregObj 0x40
#define CregMask 0x38
#define CregunMask (~CregMask)
#define	CregunMaskInd 0xf7

#define regIst(Reg) (Reg) & CregMask
#define regIstCmd(pCmd) (regIst(pCmdSub(pCmd)))
#define regIstSet(Reg,St) (Reg)=((Reg) & CregunMask) | (St & CregMask)
#define regIstCmdSet(pCmd,St) (pCmdSub(pCmd))=((pCmdSub(pCmd)) & CregunMask) | (St & CregMask)
//--------------------------
// используется только внутри VM
#define	CstyleDbNone 0x0
#define	CstyleDbArr 0x01
#define	CstyleDbArg 0x03
#define	CstyleDbArgJob 0x07
#define	CstyleDbDb 0x08
#define	CstyleDbGlb 0x10
#define	CstyleDbNet 0x20
#define	CstyleDbMask 0x07
#define	CstyleDbMaskA 0x37

#define	styleDb(pData) pData & CstyleDbMaskA

#define typeRegIsNone(refDb) ((refDb).style & CstyleDbMask) == CstyleDbNone
#define typeRegIsNoNone(refDb) (refDb).style & CstyleDbMask
#define typeRegIsArr(refDb) (refDb).style & CstyleDbArr
#define typeRegIsarrJob(refDb) ((refDb).style & CstyleDbMask) == CstyleDbArgJob
#define typeRegIsArgJob(refDb) ((refDb).style & CstyleDbArg) == CstyleDbArg
#define typeRegIsNoArgJob(refDb) ((refDb).style & CstyleDbArg) != CstyleDbArg
#define typeRegIsNoArr(refDb) ((refDb).style & CstyleDbMask) != CstyleDbArr
#define typeRegIsArg(refDb) (refDb).style == CstyleDbArg
#define typepRegIsArr(prefDb) ((prefDb)->style & CstyleDbMask) == CstyleDbArr
#define typepRegIsArg(prefDb) (prefDb)->style == CstyleDbArg
#define typepRegIsInd(prefDb) (prefDb)->style & CstyleDbDb
#define typeRegIsInd(refDb) (refDb).style & CstyleDbDb
#define typepRegIsNet(prefDb) (prefDb)->style & CstyleDbNet

#ifdef	__cplusplus
}
#endif

#endif	/* MSHVMCNST_H */
