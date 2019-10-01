/* 
 * File:   mshArf.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 27 октября 2016 г., 10:00
 */

#ifndef MSHARF_H
#define MSHARF_H

#define AbsInt(vInt) abs(vInt)
#define AbsLInt(vInt) labs(vInt)

/* Деление нацело в большую сторону */
#define mshDivUp(val,okr) (( ( (val)/(okr)*(okr) ) < (val) )?((val)/(okr)+1):(val)/(okr))
/* Округление */
#define mshOkrUp(val,okr) (mshDivUp(val,okr)) * (okr)

#define	Swap(Obj1,Obj2) {typeof(Obj1) _Obj; _Obj=Obj1;Obj1=Obj2;Obj2=_Obj;}
/*
 * Проверка магических символов
 */
#define CompareMag(pbuf,pmag) (memCompare(pbuf,pmag,strlen(pmag)))?CERRdivMagic:COutKodOk
/*
 * Сравнение строк
 */
#define StrCmrStr(skey1,skey2) ({int i; (skey1.size==0)?((skey2.size) ? CMPdown :CMPeq ):(skey2.size==0)?CMPup\
 : (skey1.size > skey2.size) ? ((i=memCompare(skey1.psec, skey2.psec, skey2.size))<0 ? CMPdown : CMPup )\
 : (skey1.size < skey2.size) ? ((i=memCompare(skey1.psec, skey2.psec, skey1.size))>0 ? CMPup : CMPdown  )\
 : ((i=memCompare(skey1.psec, skey2.psec, skey1.size))<0) ? CMPdown : (i>0)?CMPup : CMPeq ; })
 
#define StrCmrpStr(skey1,pskey2) ({int i; i=((pskey2==NULL)||(pskey2->size==0));(skey1.size==0)?(i?CMPeq:CMPdown):i?CMPup\
 : (skey1.size > pskey2->size) ? ((memCompare(skey1.psec, pskey2->psec, pskey2->size)<0 ? CMPdown : CMPup))\
 : (skey1.size < pskey2->size) ? ((memCompare(skey1.psec, pskey2->psec, skey1.size)>0 ? CMPup : CMPdown ))\
 : ((i=memCompare(skey1.psec, pskey2->psec, skey1.size))<0) ? CMPdown : (i>0)?CMPup : CMPeq ; })
 
#define pStrCmrStr(pskey1,skey2) ({int i; ((pskey1==NULL)||(pskey1->size==0))?(skey2.size ? CMPdown :CMPeq ):(skey2.size==0)?CMPup\
 : (pskey1->size > skey2.size) ? ((memCompare(pskey1->psec, skey2.psec, skey2.size)<0 ? CMPdown : CMPup))\
 : (pskey1->size < skey2.size) ? ((memCompare(pskey1->psec, skey2.psec, pskey1->size)>0 ? CMPup : CMPdown ))\
 : ((i=memCompare(pskey1->psec, skey2.psec, skey2.size))<0) ? CMPdown : (i>0)?CMPup : CMPeq ; })
 
#define pStrCmrpStr(pskey1,pskey2) ({int i; i=((pskey2==NULL)||(pskey2->size==0));((pskey1==NULL)||(pskey1->size==0))?(i?CMPeq:CMPdown):i?CMPup\
 : (pskey1->size > pskey2->size) ? ((memCompare(pskey1->psec, pskey2->psec, pskey2->size)<0 ? CMPdown : CMPup ))\
 : (pskey1->size < pskey2->size) ? ((memCompare(pskey1->psec, pskey2->psec, pskey1->size)>0 ? CMPup : CMPdown ))\
 : ((i=memCompare(pskey1->psec, pskey2->psec, pskey1->size))<0) ? CMPdown : (i>0)?CMPup : CMPeq ; })
/*
 *     "Эквивалентность строк
 */
#define	pStrEqu(pskey1,pskey2) ( ((pskey1) == NULL)?(((pskey2) == NULL)?CMPeq : CMPnoEq)\
 :((pskey1)->size != (pskey2)->size)?CMPnoEq:(memCompare((pskey1)->psec,(pskey2)->psec, (pskey1)->size))?CMPnoEq:CMPeq )
#define	StrEqu(skey1,skey2) ( ((skey1).size != (skey2).size)?CMPnoEq\
 :(memCompare((skey1).psec,(skey2).psec, (skey1).size))?CMPnoEq:CMPeq )
#define	StrpEqu(skey1,pskey2) ( ((pskey2) == NULL)?CMPnoEq\
 :((skey1).size != (pskey2)->size)?CMPnoEq:(memCompare((skey1).psec,(pskey2)->psec,(skey1).size))?CMPnoEq:CMPeq )
#define	StrNpEqu(skey1,pskey2,N) ( (memCompare((skey1).psec,(pskey2)->psec,N))?CMPnoEq:CMPeq )
// получить строку
#define StrGetpArea(pArea,Off) (Tstr*)((pArea)->psec+(Off))
#define StrGetArea(Area,Off) (Tstr*)((Area).psec+(Off))

#define	 StrTocharInt(pstr,pbuf)({ int w,size=0; char *pc,*pb=pbuf;\
 w=(pstr->size<CsizeStr)? pstr->size : CsizeStr-1; pc=pstr->psec;\
 if (*pc==CvmOpMinus) {*pb=*pc;pb++;pc++;w--;} else  if (*pc==CvmOpPlus) {pc++;w--;}\
 while (size<w) { if ((*pc>'9')||(*pc<'0')) break; *pb=*pc; pb++;pc++;size++; } size; })

// преобразование данных
#define StrToFloat(buf,fval) fval=atof(buf);fval=(fval>0)?fval + __DBL_EPSILON__ :fval - __DBL_EPSILON__
#define StrToInt(pbuf) atol(pbuf)
#define strtoint(pbuf) atoi(pbuf)
//#define msChartoInt(psChar) strtoq((char*)&(psChar), NULL, 10);

#define IntTopBuf(intnum,pbuf) snprintf(pbuf,CsizeStr,"%-ji",intnum)
#define IntTopStr(intnum,pstr) pstr->size=IntTopBuf(intnum,(char*)&(pstr->psec))
#define IntToStr(intnum,str) str.size=IntTopBuf(intnum,(char*)&(str.psec))
//Копирование строк
#define	strCopy(to,from) strcpy(to,from)
// преобразовать к целому числу
#define	mshMVmCastInt(pData) (pDataIsInt(pData))?pData->intgr:(vmCastInt(NULL,pData)<CERR)?0:pData->intgr

#endif	// MSHARF_H

