/* 
 * File:   mshArea.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 20 Август 2014 г., 11:55
 */

#ifndef MSHAREA_H
#define	MSHAREA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mshCnst.h"
#include "mshMem.h"
#include "mshArf.h"
/*
 * непрерывная Область Данных
 */
#define _contArea(Type) Type* psec;Toff size;Toff busy

#define	pareaInit(pArea,Size) (pArea)->psec=memNew( (Size) * sizeof(*((pArea)->psec)));(pArea)->size=(Size);(pArea)->busy=0
#define	areaInit(Area,Size) (Area).psec=memNew((Size) * sizeof(*((Area).psec)));(Area).size=Size;(Area).busy=0

#define	pareaFree(pArea) if ((pArea!=NULL)&&((pArea)->psec!=NULL)) ({memFree((pArea)->psec);memNULL(pArea);})
#define	areaFree(Area) ({memFree((Area).psec);memNULL(&(Area));})

#define	pareaClear(pArea) (pArea)->busy=0
#define	areaClear(Area) Area.busy=0

#define pareaOff(pArea,Off) ((pArea)->psec)+(Off)
#define	areaOff(Area,Off) ((Area).psec)+(Off)

#define	pareaWide(pArea,Count) if ((((pArea)->busy)+(Count))>((pArea)->size)) {(pArea)->size=(pArea)->busy+(Count);\
 (pArea)->psec=memChange((pArea)->psec,(pArea)->size * sizeof(*((pArea)->psec)));}
#define	areaWide(Area,Count) if ((((Area).busy)+(Count))>((Area).size)) {(Area).size=(Area).busy+(Count);\
 (Area).psec=memChange((Area).psec,(Area).size * sizeof(*((Area).psec)));}

#define	pareaTop(pArea) (pArea)->psec + (pArea)->busy
#define	areaTop(Area) (Area).psec + (Area).busy

#define	pareaCharBusy(pArea,Count) pareaWide(pArea,Count) (pArea)->busy+= Count
#define	areaCharBusy(Area,Count) areaWide(Area,Count) (Area).busy+= Count

#define	pareaCharBusyTop(pArea,Count,pTop) pareaWide(pArea,Count) pTop=pareaTop(pArea);(pArea)->busy+= Count
#define	areaCharBusyTop(Area,Count,pTop) areaWide(Area,Count); pTop=areaTop(Area); (Area).busy+= Count

#define	pareaBusy(pArea,Count) Count=mshDivUp(Count,sizeof(*((pArea)->psec)) );pareaCharBusy(pArea,Count)
#define	areaBusy(Area,Count) Count=mshDivUp(Count,sizeof(*((Area).psec)) );areaCharBusy(Area,Count)

#define	pareaBusyTop(pArea,Count,pTop) Count=mshDivUp(Count,sizeof(*((pArea)->psec)) );pareaCharBusyTop(pArea,Count,pTop)
#define	areaBusyTop(Area,Count,pTop) Count=mshDivUp(Count,sizeof(*((Area).psec)));areaCharBusyTop(Area,Count,pTop)

#define	pareaBusyStr(pArea,Count,pTop) Count=mshDivUp( offsetTstr + Count , sizeof(*((pArea)->psec)));pareaCharBusyTop(pArea,Count,pTop)
#define	areaBusyStr(Area,Count,pTop) Count=mshDivUp( offsetTstr + Count , sizeof(*((Area).psec)));areaCharBusyTop(Area,Count,pTop)

#define	pareaPush(pArea,pDan,Count,Top) pareaWide(pArea,Count) Top=(pArea)->busy;\
        memCopy((((pArea)->psec))+((pArea)->busy),pDan,Count * sizeof(*((pArea)->psec)) );(pArea)->busy+= Count
#define	areaPush(Area,pDan,Count,Top) areaWide(Area,Count) Top=(Area).busy;\
        memCopy((((Area).psec))+((Area).busy),pDan,Count * sizeof(*((Area).psec)) );(Area).busy+= Count

    
#define	pareaFirst(pArea,pDan,Count) *((pArea)->psec)=*pDan;(pArea)->busy=Count
#define	areaFirst(Area,pDan,Count) *((Area).psec)=*pDan;(Area).busy=Count
//
#define	pareaAddStr(pArea,pStr,Top,Count) ({Tstr *_pS; _pS=(Tstr*)((pArea)->psec + (pArea)->busy); Top=(pArea)->busy;\
 _pS->size=(pStr)->size; memCopy(_pS->psec,(pStr)->psec,(pStr)->size ); (pArea)->busy +=Count;})
 
/*
#define	pareaAddStr(pArea,pStr,Top,Count) Top=(pArea)->busy;\
 memCopy((((pArea)->psec))+Top,&((pStr)->size),sizeof((pStr)->size) );\
 if ((pStr)->size) {memCopy((((pArea)->psec))+Top + offsetof(Tstr,psec),(pStr)->psec,(pStr)->size );}\
 (pArea)->busy+= Count
//
#define	areaAddStr(Area,pStr,Top,Count) Top=(Area).busy;\
 memCopy(&(((((Area).psec))+Top)->size),&((pStr)->size),sizeof((pStr)->size) );\
 if ((pStr)->size) {memCopy(&(((((Area).psec))+Top)->psec),(pStr)->psec,(pStr)->size );}\
 (Area).busy+= Count
*/
//
#define	areaPushStr(Area,pStr,Top) ({int _Count=mshDivUp(pStrSize(pStr),sizeof(*((Area).psec)));\
 areaWide(Area,_Count) pareaAddStr(&(Area),pStr,Top,_Count);})

#define	pareaPushStr(pArea,pStr,Top) ({int _Count=mshDivUp(pStrSize(pStr),sizeof(*((pArea)->psec)) );\
 pareaWide(pArea,_Count) pareaAddStr(pArea,pStr,Top,_Count);})

#define pareaStrOff(pArea,Off) (Tstr*)(pareaOff(pArea,Off))
#define areaStrOff(Area,Off) (Tstr*)(areaOff(Area,Off))

#define pareaStrNext(pArea,Off) ({Tstr *_pStr=pareaOff(pArea,Off); _pStr += pStrSize( _pStr ); })
/*
 * Массив данных
 */
#define _contArr(Type) Type *psec; Type *ptop; Tusize busy; Tusize size; Tusize capa

#define	parrInit(pArr,Size) (pArr)->psec=memNew((Size) * sizeof(*((pArr)->psec)));\
 (pArr)->size=(pArr)->capa=(Size);(pArr)->ptop=NULL;(pArr)->busy=0
#define	arrInit(Arr,Size) (Arr).psec=memNew((Size) * sizeof(*((Arr).psec)));\
 (Arr).size=(Arr).capa=(Size);(Arr).ptop=NULL;(Arr).busy=0

#define	parrFree(pArr) if (pArr) ({memFree((pArr)->psec);memNULL(pArr);})
#define	arrFree(Arr) ({memFree((Arr).psec);memNULL(&(Arr));})

#define parrIsNull(pArr) (pArr)->busy==0
#define arrIsNull(Arr) (Arr).busy==0

#define parrIsNoNull(pArr) (pArr)->busy
#define arrIsNoNull(Arr) (Arr).busy

#define	parrClear(pArr) (pArr)->ptop=NULL;(pArr)->busy=0
#define	arrClear(Arr) (Arr).ptop=NULL;(Arr).busy=0

#define	parrpValToOff(pArr,pSec) (pSec)- (pArr)->psec
#define	arrpValToOff(Arr,pSec) (pSec)- (Arr).psec

#define	parrOffpD(pArr,Off) ((pArr)->psec)+(Off)
#define	arrOffpD(Arr,Off) ((Arr).psec)+(Off)

#define	parrOffD(pArr,Off) (pArr)->psec[Off]
#define	arrOffD(Arr,Off) (Arr).psec[Off]

#define	parrOffpData(pArr,Off) (Tdata*)((pArr)->psec)+(Off)
#define	arrOffpData(Arr,Off) (Tdata*)((Arr).psec) + (Off)

#define	parrOffData(pArr,Off) *(parrOffpData(pArr,Off))
#define	arrOffData(Arr,Off) *(arrOffpData(Arr,Off))

#define	parrptop(pArr) (pArr)->ptop
#define	arrptop(Arr) (Arr).ptop
#define	parrtop(pArr) *((pArr)->ptop)
#define	arrtop(Arr) *((Arr).ptop)
 
#define	parrsetptop(pArr) (pArr)->ptop=(pArr)->psec + (pArr)->busy-1
#define	arrsetptop(Arr) (Arr).ptop=(Arr).psec + (Arr).busy - 1

#define	parrsetpTop(pArr) (pArr)->ptop=((pArr)->busy)?(pArr)->psec + (pArr)->busy -1 : NULL
#define	arrsetpTop(Arr) (Arr).ptop=((Arr).busy)?(Arr).psec + (Arr).busy-1 : NULL

#define	arrsec(Arr) (Arr).psec[0]

#define	parrAddVal(pArr) (pArr)->busy++;parrsetptop(pArr)
#define	arrAddVal(Arr) (Arr).busy++;arrsetptop(Arr)

#define	parrExtSize(pArr,Size) (pArr)->size = Size;\
 (pArr)->psec=memChange((pArr)->psec,sizeof(*((pArr)->psec)) * ((pArr)->size))
#define	arrExtSize(Arr,Size) (Arr).size = Size;\
 (Arr).psec=memChange((Arr).psec,sizeof(*((Arr).psec)) * ((Arr).size))

#define	parrExt(pArr) (pArr)->size += (pArr)->capa;\
 (pArr)->psec=memChange((pArr)->psec,sizeof(*((pArr)->psec)) * ((pArr)->size))
#define	arrExt(Arr) (Arr).size += (Arr).capa;\
 (Arr).psec=memChange((Arr).psec,sizeof(*((Arr).psec)) * ((Arr).size))

#define	parrWideSize(pArr,Size) if (((pArr)->busy +Size)>((pArr)->size)) {parrExtSize(pArr,(pArr)->busy +Size);}
#define	arrWideSize(Arr,Size) if (((Arr).busy +Size)>((Arr).size)) {arrExtSize(Arr,(Arr).busy +Size);}

#define	parrWide(pArr) if (((pArr)->busy) >= ((pArr)->size)) {parrExt(pArr);}
#define	arrWide(Arr) if (((Arr).busy) >= ((Arr).size)) {arrExt(Arr);}

#define	parrBusy(pArr) parrWide(pArr) parrAddVal(pArr)
#define	arrBusy(Arr) arrWide(Arr) arrAddVal(Arr)

#define	parrBusyNull(pArr) parrBusy(pArr);memNULL((pArr)->ptop)
#define	arrBusyNull(Arr) arrBusy(Arr);memNULL((Arr).ptop)

#define	parrBusy1(pArr) (pArr)->busy=1;(pArr)->ptop=(pArr)->psec
#define	arrBusy1(Arr) (Arr).busy=1;(Arr).ptop=(Arr).psec

#define	parrBusyFirst(pArr) parrBusy1(pArr);memNULL((pArr)->psec)
#define	arrBusyFirst(Arr) arrBusy1(Arr);memNULL((Arr).psec)

#define	parrPushDan(pArr,Dan) parrBusy(pArr); *(pArr->ptop)=Dan
#define	arrPushDan(Arr,Dan) arrBusy(Arr); *(Arr.ptop)=Dan
#define	parrPushpDan(pArr,pDan) parrPushDan(pArr,*pDan)
#define	arrPushpDan(Arr,pDan) arrPushDan(Arr,*pDan)

#define	parrPop(pArr) (pArr)->busy=((pArr)->busy)?(pArr)->busy-1:0; parrsetpTop(pArr)
#define	arrPop(Arr) (Arr).busy=((Arr).busy)?(Arr).busy-1:0; arrsetpTop(Arr)

#define	parrSetDan(pArr,Dan,Off) if ((Off)>=((pArr)->size)) {parrWideSize(pArr,(Off)+1);}\
 if ((pArr)->busy <= (Off) ) {(pArr)->busy=(Off)+1; (pArr)->ptop=(pArr)->psec + (Off);} parrOffD(pArr,Off)=Dan
#define	arrSetDan(Arr,Dan,Off) parrSetDan(&(Arr),Dan,Off)
#define	parrSetpDan(pArr,pDan,Off) parrSetDan(pArr,*pDan,Off)
#define	arrSetpDan(Arr,pDan,Off) arrSetDan(Arr,*pDan,Off)

#define	parrSetEnd(pArr,Dan) parrtop(pArr)=Dan
#define	arrSetEnd(Arr,Dan) arrtop(Arr)=Dan
#define	parrSetpEnd(pArr,pDan) parrtop(pArr)=*pDan
#define	arrSetpEnd(Arr,pDan) arrtop(Arr)=*pDan
//итератор
#define	parrIter(pArr,pData) if (parrIsNoNull(pArr))\
 for (pData=(pArr)->psec;(pData) <= (pArr)->ptop; (pData)++)
#define	arrIter(Arr,pData) for (pData=(Arr).psec;(pData) <= (Arr).ptop; (pData)++)

#define	parrbegIter(pArr,pData,Beg) if (Beg < (pArr)->busy)\
 for (pData=parrOffpD(pArr,Beg);pData <= (pArr)->ptop; pData++)
#define	arrbegIter(Arr,pData,Beg) parrbegIter(&(Arr),pData,Beg)
//копирование массивов
#define	parrCopy(pto,pfrom) if ((pto)->size<(pfrom)->busy) {parrWideSize(pto,(pfrom)->busy);}\
   memCopy((pto)->psec,(pfrom)->psec,(pfrom)->busy * sizeof(*((pfrom)->psec)));\
   (pto)->busy=(pfrom)->busy; parrsetptop(pto)
//добавление массива в конец данного
#define	parrExpand(pto,pfrom) if ((pto)->size < ((pfrom)->busy + (pto)->busy))\
  {parrWideSize(pto,(pfrom)->busy + (pto)->busy );}\
  memCopy((pto)->psec + (pto)->busy,(pfrom)->psec,(pfrom)->busy * sizeof(*((pfrom)->psec)));\
  (pto)->busy += (pfrom)->busy; parrsetptop(pto)
#define	arrExpand(to,from) if ((to).size < ((from).busy + (to).busy))\
  {arrExtSize(to,(from).busy + (to).busy );}\
  memCopy((to).psec + (to).busy,(from).psec,(from).busy * sizeof(*((from).psec)));\
  (to).busy += (from).busy; arrsetptop(to)
//сдвинуть массив назад
#define	parrShift(pArr,Shift) parrWideSize(pArr,(pArr)->busy+Shift);\
  memMove((pArr)->psec+Shift,(pArr)->psec,(pArr)->busy * sizeof(*((pArr)->psec)));\
  (pArr)->busy+=Shift;parrsetptop(pArr)
//расширить массив назад
#define	parrExtNull(pArr,Shift) parrWideSize(pArr,(pArr)->busy+Shift);memNULLvoid((pArr)->ptop+1 ,(Shift) * sizeof(*((pArr)->psec)) );\
  (pArr)->busy+=Shift;parrsetptop(pArr)
#define	arrExtNull(Arr,Shift) arrWideSize(Arr,Shift);memNULLvoid((Arr).ptop+1 ,(Shift) * sizeof(*((Arr).psec)) );\
  (Arr).busy+=Shift;arrsetptop(Arr)
//вставить данные в определенную позицию
#define	parrInsOffpData(pArr,Off) parrWideSize(pArr,(pArr)->busy+1);\
  memMove(parrOffpD(pArr,Off)+1,parrOffpD(pArr,Off),((pArr)->busy -Off) * sizeof(*((pArr)->psec)));\
  (pArr)->busy++;parrsetptop(pArr)
//вставить данные в определенную позицию
#define	parrInspData(pArr,pSec) parrWideSize(pArr,(pArr)->busy+1);\
  memMove(pSec+1,pSec,((pArr)->busy - (parrpValToOff(pArr,pSec)) ) * sizeof(*((pArr)->psec)));\
  (pArr)->busy++;parrsetptop(pArr)
//заменить голову pto на pfrom
#define	parrInsert(pto,pfrom) if ((pto)->busy<(pfrom)->busy) (pto)->busy=(pfrom)->busy;\
 memCopy((pto)->psec,(pfrom)->psec,(pfrom)->busy * sizeof(*((pfrom)->psec)));parrsetptop(pto)
//вставить pfrom в начало pto
#define	parrInsertBeg(pto,pfrom) parrShift(pto,(pfrom)->busy);parrInsert(pto,pfrom)
//заменить голову pto на pfrom
#define	parrInsertOff(pto,pfrom,Off) parrWideSize(pto,(pto)->busy+(pfrom)->busy);\
 if (Off>(pto)->busy) Off=(pto)->busy;\
 memMove((pto)->psec+Off+(pfrom)->busy,(pto)->psec+Off,((pto)->busy - Off ) * sizeof(*((pfrom)->psec)));\
 memCopy((pto)->psec+Off,(pfrom)->psec,(pfrom)->busy * sizeof(*((pfrom)->psec)));\
 (pto)->busy += (pfrom)->busy; parrsetptop(pto)

#define	parrDel(pArr,pDan) if (pDan < ((pArr)->ptop)) {\
 memMove(pDan,pDan+1,((pArr)->busy - (pDan - (pArr)->psec) - 1) * sizeof(*pDan) );}\
 (pArr)->busy--;parrsetptop(pArr)
//
#define StrToArrData(Str,Arr) (Arr).size=(Arr).busy=(Str).size / sizeof(Tdata); (Arr).psec=(Tdata*)((Str).psec); arrsetpTop(Arr)
#define pStrToArrData(pStr,Arr) (Arr).size=(Arr).busy=(pStr)->size / sizeof(Tdata); (Arr).psec=(Tdata*)((pStr)->psec); arrsetpTop(Arr)
/*
 * Массив Аргументов
 */
/*
 * структура индекса Области однородных секций
 */
/*
// 4- младших разряда = номер данных внутри секции, количество сдвигов для получения номера секции
#define CarSecNumSec 4
// количество элементов данных внутри секции
#define CarSecNumDataOfSec 16
// маска номера данных внутри секции
#define CarSecNumData 0x000f
//-----------------------------------
//
//     Область однородных секций с (16) данными
//
#define _contSecs(Type) _contArr(Type*);Type *psectop; Tusize secsize; Tusize secbusy

#define	pSecsInit(parSec,Size) parrInit(parSec,Size); (parSec)->psectop=NULL; (parSec)->secsize=(parSec)->secbusy=0
#define	SecsInit(arSec,Size) arrInit(arSec,Size); (arSec).psectop=NULL; (arSec).secsize=(arSec).secbusy=0

#define	pSecsFree(parSec) ({typeof((parSec)->psec) _psec;  parrIter(parSec,_psec) memFree(*_psec); memFree((parSec)->psec); })
#define	SecsFree(arSec) ({typeof((arSec).psec) _psec;  arrIter(arSec,_psec) memFree(*_psec); memFree((arSec).psec); })

#define	pSecsClear(parSec) (parSec)->psectop=NULL; (parSec)->secbusy=0
#define	SecsClear(arSec) (arSec).psectop=NULL; (arSec).secbusy=0

#define	pSecsIsData(parSec) (parSec)->secbusy

#define SecsNumSec(Off) (Off) >> CarSecNumSec
#define SecsNumData(Off) (Off) & CarSecNumData

#define	pSecsOffpD(parSec,Off)  ( (parSec)->psec[ SecsNumSec(Off) ] ) + ( SecsNumData(Off) )
#define	SecsOffpD(arSec,Off)  ( (arSec).psec[ SecsNumSec(Off) ] ) + ( SecsNumData(Off) )
#define	pSecsOffD(parSec,Off) *(pSecsOffpD(parSec,Off))
#define	SecsOffD(arSec,Off) *(SecsOffpD(arSec,Off))

#define	pSecsOffpDataIs(parSec,Off)  (pSecsIsData(parSec))? ( pSecsOffpD(parSec,Off) ) : NULL

#define	pSecsExt(parSec) parrBusy(parSec);*((parSec)->ptop)=memNewInit(sizeof(typeof(**((parSec)->psec))) *  CarSecNumDataOfSec);\
 (parSec)->secsize +=CarSecNumDataOfSec

#define	pSecsWide(parSec) if (((parSec)->secbusy) >= ((parSec)->secsize)) {pSecsExt(parSec);}

#define	pSecsAddVal(parSec) (parSec)->psectop=pSecsOffpD(parSec,(parSec)->secbusy); (parSec)->secbusy++

#define	pSecsBusy(parSec) pSecsWide(parSec) pSecsAddVal(parSec)
#define	pSecsBusyNull(parSec) pSecsBusy(parSec); memNULL((parSec)->psectop)

#define	pSecsPushDan(parSec,Dan) pSecsBusy(parSec); *((parSec)->psectop)=Dan

#define	pSecsPop(parSec) (parSec)->secbusy--; (parSec)->psectop=((parSec)->secbusy)?( pSecsOffpD(parSec,(parSec)->secbusy) ):NULL
#define	SecsPop(arSec) (arSec).secbusy--; (arSec).psectop=((arSec).secbusy)?( SecsOffpD(arSec,(arSec).secbusy) ):NULL
//итератор
#define	pSecsIter(parSec,pData) for (Tusize _secj=0;\
 ( (parSec)->secbusy) && ( pData=( pSecsOffpD(parSec,_secj) ) ) && ( ( ( SecsNumSec(_secj) )  < ( (parSec)->busy  -1) ) || (pData <= (parSec)->psectop ) );\
 ++_secj)
#define	SecsIter(arSec,pData) for (Tusize _secj=0;\
 ( (arSec).secbusy) && ( pData=( SecsOffpD(arSec,_secj) ) ) && ( ( ( SecsNumSec(_secj) ) < ( (arSec).busy  -1) ) || (pData <= (arSec).psectop ) ) ;\
 ++_secj)
//
// Box структуры на основе Secs
// 
#define	_contSecsBox(Type,secType) _contSecs(secType);Tsize init;void *pUser;\
  void (*pfinit)(secType *psec,void *pUser);void (*pfclear)(secType *psec,void *pUser); void (*pffree)(secType *psec,void *pUser)

#define	_structSecsBoxStruct(Type,secType) typedef struct stt##Type {_contSecsBox(stt##Type,secType);} Type

#define	pSecsBoxInit(pBox,Size,puser,pfInit,pfClear,pfFree) pSecsInit(pBox,Size);(pBox)->init=0;(pBox)->pUser=puser; \
 (pBox)->pfinit=(pfInit);(pBox)->pfclear=(pfClear);(pBox)->pffree=(pfFree)
#define	secsBoxInit(Box,Size,puser,pfInit,pfClear,pfFree) SecsInit(Box,Size);(Box).init=0;(Box).pUser=puser; \
 (Box).pfinit=(pfInit);(Box).pfclear=(pfClear);(Box).pffree=(pfFree)

#define	pSecsBoxNew(Type,secType,Size,puser,pfInit,pfClear,pfFree) ({Type* pBox=memType(Type);\
 pSecsBoxInit(pBox,Size,puser,pfInit,pfClear,pfFree);pBox;})

#define	pSecsBoxClear(pBox) pSecsClear(pBox)
#define	SecsBoxClear(Box) SecsClear(Box)

#define	pSecsBoxFree(pBox) if ( ((pBox)->pffree) ) { (pBox)->secbusy = (pBox)->init; \
  while ( (pBox)->secbusy ) { (pBox)->pffree( (pBox)->psectop) ;pSecsPop(pBox); } } (pBox)->secbusy = (pBox)->init; pSecsFree(pBox)
#define	SecsBoxFree(Box) pSecsBoxFree( &(Box) )

#define	pSecsBoxPush(pBox) ({ pSecsBusy(pBox);\
 if (((pBox)->secbusy) > (pBox)->init ){ (pBox)->init = (pBox)->secbusy; \
  if (((pBox)->pfinit)) {((pBox)->pfinit)((pBox)->psectop,pBox);} }\
 else if (((pBox)->pfclear)) ((pBox)->pfclear)((pBox)->psectop);})
//
#define	SecsBoxPush(Box) pSecsBoxPush(&(Box))

#define	pSecsBoxPop(pBox) pSecsPop(pBox)
#define	SecsBoxPop(Box) pSecsBoxPop(&(Box))

#define	pSecsBoxPopTop(pBox,pTop) if ((pBox)->psectop) { pTop=(pBox)->psectop; pSecsBoxPop(pBox);} else pTop=NULL
#define	SecsBoxPopTop(Box,pTop) pSecsBoxPopTop(&(Box),pTop)
 */
//---------------------------------------
/*
 * структура индекса Области однородных секций
 */
// 4- младших разряда = номер данных внутри секции, количество сдвигов для получения номера секции
#define CarRegNumReg 4
// количество элементов данных внутри секции
#define CarRegNumDataOfReg 16
// маска номера данных внутри секции
#define CarRegNumData 0x000f
/*
 *    Область однородных секций с (16) данными
 */
#define _contReg(Type) Type *psec; Type *ptop; Tusize busy; Tusize size;Tusize _secj; struct { _contArr(Type*); } _ct0

#define	pRegExt(pRegs) arrBusy((pRegs)->_ct0);*((pRegs)->_ct0.ptop)=memNewInit(sizeof(typeof(*((pRegs)->psec))) *  CarRegNumDataOfReg);\
 (pRegs)->size +=CarRegNumDataOfReg
#define	RegExt(Regs) arrBusy((Regs)._ct0);*((Regs)._ct0.ptop)=memNewInit(sizeof(typeof(*((Regs).psec))) *  CarRegNumDataOfReg);\
 (Regs).size +=CarRegNumDataOfReg

#define	pRegInit(pRegs,Size) arrInit((pRegs)->_ct0,Size);(pRegs)->size=0;pRegExt(pRegs);(pRegs)->psec=*((pRegs)->_ct0.ptop);\
 (pRegs)->ptop=NULL; (pRegs)->busy=0
#define	RegInit(Regs,Size) arrInit((Regs)._ct0,Size);(Regs).size=0;RegExt(Regs);(Regs).psec=*((Regs)._ct0.ptop);\
 (Regs).ptop=NULL; (Regs).busy=0

#define	pRegFree(pRegs) ({typeof((pRegs)->_ct0.psec) _preg; if ((pRegs)->_ct0.busy) arrIter((pRegs)->_ct0,_preg) memFree(*_preg); memFree(((pRegs)->_ct0.psec)); })
#define	RegFree(Regs) ({typeof((Regs)._ct0.psec) _preg; if ((Regs)._ct0.busy) arrIter((Regs)._ct0,_preg) memFree(*_preg); memFree(((Regs)._ct0.psec)); })

#define	pRegClear(pRegs) (pRegs)->ptop=NULL; (pRegs)->busy=0
#define	RegClear(Regs) (Regs).ptop=NULL; (Regs).busy=0

#define	pRegIsData(pRegs) (pRegs)->busy

#define arRegNumReg(Off) (Off) >> CarRegNumReg
#define arRegNumData(Off) (Off) & CarRegNumData

#define	pRegOffpD(pRegs,Off) (Off<(pRegs)->busy)?(( (pRegs)->_ct0.psec[ arRegNumReg(Off) ] ) + ( arRegNumData(Off) )):NULL
#define	RegOffpD(Regs,Off) (Off<(Regs).busy)?(( (Regs)._ct0.psec[ arRegNumReg(Off) ] ) + ( arRegNumData(Off) )):NULL

#define	pRegOffpDataIs(pRegs,Off)  (pRegIsData(pRegs))? ( pRegOffpD(pRegs,Off) ) : NULL

#define	pRegWide(pRegs) if (((pRegs)->busy) >= ((pRegs)->size)) {pRegExt(pRegs);}

#define	pRegAddVal(pRegs) (pRegs)->ptop=( (pRegs)->_ct0.psec[ arRegNumReg((pRegs)->busy) ] ) + ( arRegNumData((pRegs)->busy) ); (pRegs)->busy++

#define	pRegBusy(pRegs) pRegWide(pRegs) pRegAddVal(pRegs)
#define	pRegBusyNull(pRegs) pRegBusy(pRegs); memNULL((pRegs)->ptop)

#define	pRegPushDan(pRegs,Dan) pRegBusy(pRegs); *((pRegs)->ptop)=Dan

#define	pRegPop(pRegs) ({if ((pRegs)->busy) {(pRegs)->busy--; (pRegs)->ptop=((pRegs)->busy)?( pRegOffpD(pRegs,((pRegs)->busy-1)) ):NULL;} })
#define	RegPop(Regs) ({if ((Regs).busy) {(Regs).busy--; (Regs).ptop=((Regs).busy)?( RegOffpD(Regs,((Regs).busy-1)) ):NULL;} })
//итератор
#define	pRegIter(pRegs,pData) for ((pRegs)->_secj=0;(((pRegs)->_secj < (pRegs)->busy)&& ( pData=( pRegOffpD(pRegs,(pRegs)->_secj) ) )) ; ++(pRegs)->_secj)
#define	RegIter(Regs,pData) for ((Regs)._secj=0;(((Regs)._secj < (Regs).busy)&& ( pData=( RegOffpD(Regs,(Regs)._secj) ) )) ; ++(Regs)._secj)
/*
 * Box структуры на основе Reg
 */
#define	_contRegBox(secType) _contReg(secType);Tsize init;void *pUser;\
  void (*pfinit)(secType *psec,void *pUser);void (*pfclear)(secType *psec,void *pUser); void (*pffree)(secType *psec,void *pUser)

#define	_structRegBoxStruct(Type,secType) typedef struct {_contRegBox(secType);} Type

#define	pRegBoxInit(pBox,Size,puser,pfInit,pfClear,pfFree) pRegInit(pBox,Size);(pBox)->init=0;(pBox)->pUser=puser; \
 (pBox)->pfinit=(pfInit);(pBox)->pfclear=(pfClear);(pBox)->pffree=(pfFree)
#define	RegBoxInit(Box,Size,puser,pfInit,pfClear,pfFree) RegInit(Box,Size);(Box).init=0;(Box).pUser=puser; \
 (Box).pfinit=(pfInit);(Box).pfclear=(pfClear);(Box).pffree=(pfFree)

#define	pRegBoxNew(Type,secType,Size,puser,pfInit,pfClear,pfFree) ({Type* pBox=memType(Type);\
 pRegBoxInit(pBox,Size,puser,pfInit,pfClear,pfFree);pBox;})

#define	pRegBoxClear(pBox) pRegClear(pBox)
#define	RegBoxClear(Box) RegClear(Box)

#define	pRegBoxFree(pBox) if ( ((pBox)->pffree) ) { (pBox)->busy = (pBox)->init; \
  while ( (pBox)->busy ) { (pBox)->pffree( (pBox)->ptop,(pBox)->pUser ) ;pRegPop(pBox); } } (pBox)->busy = (pBox)->init; pRegFree(pBox)
#define	RegBoxFree(Box) pRegBoxFree( &(Box) )

#define	pRegBoxPush(pBox) ({ pRegBusy(pBox);\
 if (((pBox)->busy) > (pBox)->init ){ (pBox)->init = (pBox)->busy; \
  if (((pBox)->pfinit)) {((pBox)->pfinit)((pBox)->ptop,(pBox)->pUser);} }\
 else if (((pBox)->pfclear)) ((pBox)->pfclear)((pBox)->ptop,(pBox)->pUser);})
//
#define	RegBoxPush(Box) pRegBoxPush(&(Box))

#define	pRegBoxPop(pBox) pRegPop(pBox)
#define	RegBoxPop(Box) pRegBoxPop(&(Box))

#define	pRegBoxPopTop(pBox,pTop) if ((pBox)->ptop) { pTop=(pBox)->ptop; pRegBoxPop(pBox);} else pTop=NULL
#define	RegBoxPopTop(Box,pTop) pSecsBoxPopTop(&(Box),pTop)
/*
 * Box структуры на основе Arr
 */
#define	_contArrBox(secType) _contArr(secType);Tsize init;void *pUser;\
  void (*pfinit)(secType *psec,void *pUser);void (*pfclear)(secType *psec,void *pUser);\
  void (*pffree)(secType *psec,void *pUser)

#define	_structArrBoxStruct(Type,secType) typedef struct stt##Type {_contArrBox(secType);} Type

#define	parrBoxInit(pBox,Size,puser,pfInit,pfClear,pfFree) parrInit(pBox,Size);(pBox)->init=0;(pBox)->pUser=puser;\
 (pBox)->pfinit=(pfInit);(pBox)->pfclear=(pfClear);(pBox)->pffree=(pfFree)
#define	arrBoxInit(Box,Size,puser,pfInit,pfClear,pfFree) arrInit(Box,Size);(Box).init=0;(Box).pUser=puser;\
 (Box).pfinit=(pfInit);(Box).pfclear=(pfClear);(Box).pffree=(pfFree)

#define	parrBoxNew(Type,secType,Size,pUser,pfInit,pfClear,pfFree) ({Type* pBox=memType(Type);\
 parrBoxInit(pBox,Size,pUser,pfInit,pfClear,pfFree);pBox;})

#define	parrBoxClear(pBox) parrClear(pBox)
#define	arrBoxClear(Box) arrClear(Box)

#define	arrBoxFree(Box) if ((((Box).pffree))&&((Box).init)){(Box).ptop=((Box).psec)+((Box).init);\
 do{(Box).ptop--;(Box).pffree((Box).ptop,(Box).pUser);}while((Box).ptop > (Box).psec);\
 } arrFree(Box)

#define	parrBoxPush(pBox) parrBusy(pBox);\
 if (((pBox)->busy) > (pBox)->init ){\
 (pBox)->init = (pBox)->busy; if (((pBox)->pfinit)) {((pBox)->pfinit)((pBox)->ptop,(pBox)->pUser);}}\
 else if (((pBox)->pfclear)) ((pBox)->pfclear)((pBox)->ptop,(pBox)->pUser)
//
#define	arrBoxPush(Box) parrBoxPush(&(Box))

#define	parrBoxPop(pBox) parrPop(pBox)
#define	arrBoxPop(Box) arrPop(Box)

#define	parrBoxPopTop(pBox,pTop) if ((pBox)->ptop) {pTop=(pBox)->ptop; parrBoxPop(pBox);} else pTop=NULL
#define	arrBoxPopTop(Box,pTop) parrBoxPopTop(&(Box),pTop)

#define	parrBoxCopy(pto,pfrom) parrCopy(pto,pfrom);(pto)->init=(pfrom)->init;(pto)->pUser=(pfrom)->pUser
#define arrBoxCopy(to,from) parrBoxCopy( &(to),&(from) )

/*-------------------------------------------------------
 *                   2-х мерный стек
 * массив _ct0 содержит конец кадра массива _core
 */
//#define	_contStk2(Type) struct { _contArr(Type); } _core;struct { _contArr(int); } _ct0;struct { _contArr(Type); } _arr
#define	_contStk2(Type) struct { _contArr(Type); } _core;struct { _contArr(int); } _ct0
    
#define	structStk2(Type,secType) typedef struct { _contStk2(secType); } Type

#define	pStk2Init(pStk,Size,ct0Size) arrInit((pStk)->_core,Size); arrInit((pStk)->_ct0,ct0Size)
#define	Stk2Init(Stk,Size,ct0Size) arrInit((Stk)._core,Size); arrInit((Stk)._ct0,ct0Size)

#define	pStk2New(Type,Size) ({Type* pStk=memType(Type);pStk2Init(pStk,Size);pStk;})

#define	pStk2Free(pStk) arrFree((pStk)->_ct0);arrFree((pStk)->_core)
#define	Stk2Free(Stk) arrFree((Stk)._ct0);arrFree((Stk)._core)

#define	Stk2AllClear(Stk) arrClear((Stk)._core);arrClear((Stk)._ct0)
//проверять выход за кадр
#define	Stk2_0(Stk) ((Stk)._core.busy==0) || ( (Stk)._ct0.ptop && (( *((Stk)._ct0.ptop) > (Stk)._core.busy) ) ) 

#define	Stk2Lev0Clear(Stk) if ((Stk)._ct0.busy) {\
  if ((Stk)._core.busy > *((Stk)._ct0.ptop)) {(Stk)._core.busy = *((Stk)._ct0.ptop);arrsetpTop((Stk)._core);}\
 } else {(Stk)._core.busy=0;(Stk)._core.ptop=NULL;}

#define	Stk2Lev0Up(Stk) arrBusy((Stk)._ct0);*((Stk)._ct0.ptop)=Stk._core.busy

#define	Stk2Lev0Pop(Stk) if ((Stk)._ct0.busy) {\
     if ((Stk)._core.busy > *((Stk)._ct0.ptop)) { (Stk)._core.busy = *((Stk)._ct0.ptop); arrsetpTop((Stk)._core);}\
     (Stk)._ct0.busy--;arrsetpTop((Stk)._ct0); }\
    else { (Stk)._core.busy=0;(Stk)._core.ptop=NULL; }
//
#define	Stk2Busy(Stk) arrBusyNull(Stk._core)
//проверять выход за кадр
#define	Stk2Pop(Stk) if ( !(Stk2_0(Stk)) ) {(Stk)._core.busy--;arrsetpTop((Stk)._core);}
// if ((Stk)._ct0.busy) { if ((Stk)._core.busy > *((Stk)._ct0.ptop)) {(Stk)._core.busy--;arrsetpTop((Stk)._core);} } else {arrPop((Stk)._core);}

#define Stk2psec(Stk) ((Stk)._core.busy==0)?NULL:((Stk)._ct0.busy==0) ? (Stk)._core.psec :\
  ((Stk)._core.busy > *((Stk)._ct0.ptop))?arrOffpD( (Stk)._core,*( (Stk)._ct0.ptop) ):NULL
  

#define Stk2ptop(Stk) (Stk2_0(Stk))?NULL:(Stk)._core.ptop

//#define Stk2toArr(Stk,Arr) Arr.ptop=Stk2ptop(Stk);Arr.psec=Stk2psec(Stk);Arr.busy=Arr.size=(Arr.ptop)?(Arr.ptop - Arr.psec + 1) : 0
#define Stk2toArr(Stk,Arr) ({if ( Stk2_0(Stk) ) { arrClear(Arr); }\
 else if ((Stk)._ct0.busy==0) {Arr.psec=(Stk)._core.psec;Arr.ptop=(Stk)._core.ptop;Arr.busy=Arr.size=(Stk)._core.busy;}\
 else {Arr.psec=arrOffpD( (Stk)._core,*( (Stk)._ct0.ptop) );Arr.ptop=(Stk)._core.ptop;Arr.busy=Arr.size=(Stk)._core.busy-*((Stk)._ct0.ptop);} })
//    
#define Stk2size(Stk) ((Stk)._core.busy==0)?0:((Stk)._ct0.busy==0)?(Stk)._core.busy:(Stk)._core.busy - *((Stk)._ct0.ptop)
    
/*-------------------------------------------------------
 *                   3-х мерный стек
 * массив _ct0 содержит конец кадра массива _core
 * массив _ct1 содержит конец кадра массива _ct0
 */
    

#define	_contStk3(arrType) _contStk2(arrType);struct { _contArr(int); } _ct1
    
#define	structStk3(Type,secType) typedef struct { _contStk3(secType); } Type
    
#define	Stk3Init(Stk,Size,ct0Size,ct1Size) Stk2Init(Stk,Size,ct0Size); arrInit((Stk)._ct1,ct1Size)
#define	Stk3Free(Stk) Stk2Free(Stk);arrFree((Stk)._ct1)
//есть ли индекс 3-го уровня
#define	Stk3_1(Stk) ((Stk)._ct1.busy) && ((Stk)._ct0.busy > *((Stk)._ct1.ptop))
//    
#define	Stk3AllClear(Stk) Stk2AllClear(Stk);arrClear((Stk)._ct1)
//
#define	Stk3Lev1Up(Stk) arrBusy((Stk)._ct1);*((Stk)._ct1.ptop)=Stk._ct0.busy; Stk2Lev0Up(Stk)

#define	Stk3Lev1Pop(Stk) if ((Stk)._ct1.busy) {if ((Stk)._ct0.busy > *((Stk)._ct1.ptop)){\
 Stk2Lev0Pop(Stk) (Stk)._ct0.busy = *( (Stk)._ct1.ptop ); arrsetpTop( (Stk)._ct0 );} arrPop((Stk)._ct1); } else {Stk2AllClear(Stk);}
    
#define	Stk3Lev1Clear(Stk) if ( ((Stk)._ct1.busy)&&((Stk)._ct0.busy > *((Stk)._ct1.ptop)) ) {\
    (Stk)._ct0.busy=(Stk)._ct0.psec[*((Stk)._ct1.ptop)];arrsetpTop( (Stk)._ct0 ); Stk2Lev0Clear(Stk) } else {Stk2AllClear(Stk);}

//#define	Stk3Lev1Pop(Stk) if (Stk3_1(Stk)) { stk3lev1cl(Stk) (Stk)._ct1.busy--; arrsetpTop( (Stk)._ct1 ); } else {Stk2AllClear(Stk);}
// 
#define	Stk3Lev0Up(Stk) Stk2Lev0Up(Stk)
//проверять выход за кадр
//#define	Stk3_0(Stk) ((Stk)._ct1.busy==0)||((Stk)._ct0.busy > *((Stk)._ct1.ptop))
//#define	Stk3Lev0Pop(Stk) if ( Stk3_0(Stk) ) { Stk2Lev0Pop(Stk) }
#define	Stk3Lev0Pop(Stk) Stk2Lev0Pop(Stk)
//проверять выход за кадр
//#define	Stk3Lev0Clear(Stk) if ( Stk3_0(Stk)  ) { Stk2Lev0Clear(Stk) }
#define	Stk3Lev0Clear(Stk) Stk2Lev0Clear(Stk)
//
#define	Stk3Busy(Stk) Stk2Busy(Stk)
#define	Stk3Pop(Stk) Stk2Pop(Stk)
//#define	Stk3Pop(Stk) if ( Stk3_0(Stk) ) {Stk2Pop(Stk)}
//проверять выход за кадр
//#define Stk3psec(Stk) ( Stk3_0(Stk) )?Stk2psec(Stk):NULL
#define Stk3psec(Stk) Stk2psec(Stk)
//#define Stk3ptop(Stk) ( Stk3_0(Stk) )?Stk2ptop(Stk):NULL
#define Stk3ptop(Stk) Stk2ptop(Stk)
//проверять выход за кадр
//#define Stk3toArr(Stk,Arr) if ( Stk2_0(Stk) ) { Stk2toArr(Stk,Arr); } else{ arrClear(Arr); }
//#define Stk3size(Stk) ( !(Stk2_0(Stk)) )?0:Stk2size(Stk)
#define Stk3toArr(Stk,Arr) Stk2toArr(Stk,Arr);
#define Stk3size(Stk) Stk2size(Stk)
//
 /*
 * 2-х мерный массив
 */
#define	_contArr2up(arrType) _contArr(arrType); Tusize sizearr;Tusize init

#define	structArr2(Type,secType) typedef struct {_contArr(secType);} Tarr ## secType;\
 typedef struct {_contArr2up(Tarr ## secType);} Type

#define	parr2Init(pArr2,Size,SizeArr) parrInit(pArr2,Size);\
  (pArr2)->sizearr=SizeArr;(pArr2)->init=0
#define	arr2Init(Arr2,Size,SizeArr) arrInit(Arr2,Size);\
  (Arr2).sizearr=SizeArr;(Arr2).init=0

#define	parr2Free(pArr2) if ((pArr2)->init){ (pArr2)->ptop=((pArr2)->psec)+((pArr2)->init);\
 while( ((pArr2)->ptop) >= (pArr2)->psec ){parrFree((pArr2)->ptop);(pArr2)->ptop--;} } parrFree(pArr2)
#define	arr2Free(Arr2) if ((Arr2).init){ (Arr2).ptop=((Arr2).psec)+((Arr2).init)-1;\
 while( ((Arr2).ptop) >= (Arr2).psec ){parrFree((Arr2).ptop);(Arr2).ptop--;} } arrFree(Arr2)

#define	parr2Clear(pArr2) parrClear(pArr2)
#define	arr2Clear(Arr2) arrClear(Arr2)

#define	parr2Push(pArr2) ({ if ((pArr2)->init <= (pArr2)->busy) {parrBusy(pArr2); \
  (pArr2)->init = (pArr2)->busy ; parrInit((pArr2)->ptop,(pArr2)->sizearr);}\
  else {(pArr2)->busy++;parrsetptop(pArr2); parrClear((pArr2)->ptop);} })
#define	arr2Push(Arr2) parr2Push(&(Arr2))

#define	parr2Pop(pArr2) parrPop(pArr2)
#define	arr2Pop(Arr2) arrPop(Arr2)

#define	parr2PushpSec(pArr2,pSec) ({ if ((pArr2)->ptop) {parrPushpDan((pArr2)->ptop,pSec);} })
#define	arr2PushpSec(Arr2,pSec) parr2PushpSec(&(Arr2),pSec)
#define	arr2PushSec(Arr2,Sec) parr2PushpSec(&(Arr2),*Sec)

#define	parr2BusypSec(pArr2) ({if ((pArr2)->ptop) {parrBusyNull((pArr2)->ptop);} })
#define	arr2BusypSec(Arr2) ({if ((Arr2).ptop) {parrBusyNull((Arr2).ptop);} })

#define	parr2PopSecTop(pArr2,pTop) if ((pArr2)->ptop) {pTop=(pArr2)->ptop;parrPop((pArr2)->ptop);}\
 else pTop=NULL
#define	arr2PopSecTop(Arr2,pTop) parr2PopSecTop(&(Arr2),pTop)

#define	parr2PopSec(pArr2) if ((pArr2)->ptop) {parrPop((pArr2)->ptop);}
#define	arr2PopSec(Arr2) parr2PopSec(&(Arr2))

#define	parr2ClearSec(pArr2) if (pArr2->ptop) {parrClear(pArr2->ptop);}
#define	arr2ClearSec(Arr2) if (Arr2.ptop) {parrClear(Arr2.ptop);}

 /*
 * 2-х мерный массив с дополнительным полем  _sub
 */
#define	_contArr2sub(secType,subType) _contArr2up(struct { _contArr(secType); subType _sub;})
    
//#define	structArr2sub(Type,secType,subType) typedef struct {_contArr2sub(secType,subType);} Type

#define	structArr2sub(Type,secType,subType) typedef struct {_contArr(secType);subType _sub;} Tarr ## secType;\
 typedef struct {_contArr2up(Tarr ## secType);} Type

#define	parr2subInit(pArr2,Size,SizeArr) parr2Init(pArr2,Size,SizeArr)
#define	arr2subInit(Arr2,Size,SizeArr) arr2Init(Arr2,Size,SizeArr)

//#define	parr2subNew(Type,Size,SizeArr) ({Type* pArr2=memType(Type);parr2subInit(pArr2,Size,SizeArr);pArr2;})

#define	parr2subFree(pArr2) parr2Free(pArr2)
#define	arr2subFree(Arr2) arr2Free(Arr2)

#define	parr2subClear(pArr2) parr2Clear(pArr2)
#define	arr2subClear(Arr2) arr2Clear(Arr2)

#define	parr2subPush(pArr2) parr2Push(pArr2); memNULL(&((pArr2)->ptop->_sub))
#define	arr2subPush(Arr2) parr2subPush(&(Arr2))

#define	parr2subPop(pArr2) parr2Pop(pArr2)
#define	arr2subPop(Arr2) arr2Pop(Arr2)

#define	parr2subPushpSec(pArr2,pSec) parr2PushpSec(pArr2,pSec)
#define	arr2subPushpSec(Arr2,pSec) arr2PushpSec(Arr2,pSec)
#define	arr2subPushSec(Arr2,Sec) arr2PushSec(Arr2,Sec)

#define	arr2subBusypSec(Arr2) arr2BusypSec(Arr2)

#define	parr2subPopSecTop(pArr2,pTop) parr2PopSecTop(pArr2,pTop)
#define	arr2subPopSecTop(Arr2,pTop) arr2PopSecTop(Arr2,pTop)

#define	parr2subPopSec(pArr2) parr2PopSec(pArr2)
#define	arr2subPopSec(Arr2) arr2PopSec(Arr2)

#define	parr2subClearSec(pArr2) if (pArr2->ptop) {parrClear(pArr2->ptop);memNULL(&((pArr2)->ptop->_sub));}
#define	arr2subClearSec(Arr2) if (Arr2.ptop) {parrClear(Arr2.ptop);memNULL(&((Arr2).ptop->_sub)); }

#define	parr2suSecbCopy(ptoSec,pfromSec) parrCopy(ptoSec,pfromSec);(ptoSec)->_sub=(pfromSec)->_sub
#define	arr2subSecCopy(toSec,fromSec) parrCopy(&toSec,&fromSec);(toSec)._sub=(fromSec)._sub

#ifdef	__cplusplus
}
#endif

#endif	/* MSHAREA_H */
