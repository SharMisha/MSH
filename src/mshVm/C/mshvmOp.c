/*
 * File:   mshVmOp.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:51
 */
#include "mshvmVm.h"
#include "mshvmMacro.h"

void pDataKillLongArr(TmapLong *pmLong,Tdata *pDataIn);
Terr castDataArf(Tdata *pData);
Terr castDataInt(Tdata *pData);
Terr castDataFloat(Tdata *pData);
Terr castDataToStr(Tdata *pData);
Terr castNorm(Tdata *pData);
void castbStr(Tdata *pData,Tbstr *pbstrOut);
void bStrGet(Tdata *pData,Tbstr* pbstrOut);
/*
 * 2 строка следует за 1 ?
 */
static TintNum OpStrLarge(Tbstr *pbStr1,Tbstr *pbStr2) {
 char *pc1,*pc2,*pce;
 //
 if (pbStr2->size<1) return 1;
 pce=pbStr2->size < pbStr1->size ? pbStr1->psec + pbStr2->size : pbStr1->psec + pbStr1->size;
 for (pc1=pbStr1->psec,pc2=pbStr2->psec;pc1<pce;++pc1,++pc2){
  if ((*pc1) > (*pc2)) return 1;
  if ((*pc1) < (*pc2)) return 0;
  }
 return ((pbStr1->size) > (pbStr2->size))?1:0;
 }
/*
 * 1 ая строка содержит 2 ую
 *  кодировка UTF-8
 */
static int OpStrInclFsub(char *pc1,char *pc2,char *pce){
 for (;pc2<pce;++pc1,++pc2)
  if (*pc1!=*pc2) return 0;
 return 1;
 }

static TintNum OpStrInclF(Tbstr *pbStr1,Tbstr *pbStr2) {
 char *pc1,*pc2,*pce11,*pce2;
 if (pbStr2->size<1) return 1;
 if ((pbStr2->size) > (pbStr1->size)) return 0;
 pc1=pbStr1->psec;
 pc2=pbStr2->psec;
 pce2=pc2+pbStr2->size;
 if ((pbStr2->size) == (pbStr1->size))
  return OpStrInclFsub(pc1,pc2,pce2);
 pce11=pc1+pbStr1->size-pbStr2->size+1;
 while(pc1<pce11){
  if (OpStrInclFsub(pc1,pc2,pce2)) return 1;
  trSmbNext(pc1);
  }
 return 0;
 }
/*
 *    соединить строки
 */
static int StrConStr(Tdata *pAr1,Tbstr *pbStr1,Tbstr *pbStr2, Tarea *pArea) {
 int out=COutKodOk;
 Toff offB,n;
 Tstr *pStr;
 char *pB;

 if (pbStr2->size<1) return out;
 n=pbStr1->size + pbStr2->size;
 //строка помещается в данные
 if (n <= CsizeStr) {
  if (pDataIsNoStr(pAr1)) {
   pStrTopData(pbStr1,pAr1);
   }
  memCopy(&(pAr1->str.psec[pAr1->str.size]),pbStr2->psec,pbStr2->size);
  pAr1->str.size=n;
  return out;
  }
 //строку размещаем в области pArea
// pareaPush(pArea,pbStr1->psec,pbStr1->size,off);
 if ((pDataIsArea(pAr1)) && (pAr1->areaOff.parea==pArea) ){
  offB=pAr1->areaOff.off;
  }
 else{
  pareaPushStr(pArea,pbStr1,offB);
  pDataSetArea(pAr1);
  pAr1->areaOff.parea=pArea;
  pAr1->areaOff.off=offB;
  }
 n=pbStr2->size;
 pareaBusy(pArea,n);
 pB=(char*)(pArea->psec + offB) +  pStrSize(pbStr1);
 memMove(pB,pbStr2->psec,pbStr2->size);
 pStr=pareaStrOff(pArea,offB);
 pStr->size += pbStr2->size;
 pArea->busy=mshDivUp(pStrSize(pStr),sizeof(*((pArea)->psec)) ) + offB;
 return out;
 }
static void OpRunstrOp(Tdata *pAr,Tbstr *pbStr){
 switch(pDataSight(pAr)){
  case CDataFloat :
   pDataSetInt(pAr);
   pAr->intgr = pAr->fnum;
  case CDataInt :
   pbStr->size=sizeof(pAr->intgr);
   pbStr->psec=(char*)(&(pAr->intgr));
   break;
  default :
   bStrGet(pAr,pbStr);
   break;
  }
 }
/*
 * Операции сравнения
 */
int OpCompStr(Tbstr *pbAr1,Tbstr *pbAr2){
 int n,out;
 n=(pbAr1->size<pbAr2->size)?pbAr1->size:pbAr2->size;
 out=memCompare(pbAr1->psec,pbAr2->psec,n);
 return (out>0)?1:(pbAr1->size > pbAr2->size)?1:0;
 }
//
Terr OpCompare(Tdata *pAr1,Tdata *pAr2,char op1,char op2) {
 Terr err=COutKodOk;
 int isArf,out=0;
 Tbstr bAr1,bAr2;
 //операции сравнения
 castNorm(pAr2);
 castNorm(pAr1);
 if ( (pDataIsNum(pAr1))&&(pDataIsNum(pAr2)) )
  isArf=1;
 else{
  isArf=0;
  castbStr(pAr1,&bAr1);
  castbStr(pAr2,&bAr2);
  }
 //
 switch (op1) {
  case CvmOpNot :
   switch (op2) {
    case CvmOpEqu :
     if (isArf) {vmopArf3(pAr1,!=,pAr2,out);}
     else{
      if (bAr1.size!=bAr2.size) out=1;
      else
       if (memCompare(bAr1.psec,bAr2.psec,bAr1.size)) out=1;     
      }
     break;
    case CvmOpHight :
     if (isArf) {vmopArf3(pAr1,<=,pAr2,out);}
     else{
      out= !(OpCompStr(&bAr1,&bAr2));
      }
     break;
    case CvmOpLow :
     if (isArf) {vmopArf3(pAr1,>=,pAr2,out);}
     else{
      out= !(OpCompStr(&bAr2,&bAr1));
      }
     break;
    default  :
     err=CERRvmNotOp;
     break;
    }
   break;
  case CvmOpHight :
   switch (op2) {
    case CvmOpEqu :
     if (isArf) {vmopArf3(pAr1,>=,pAr2,out);}
     else{
      out= !(OpCompStr(&bAr2,&bAr1));
      }
     break;
    case CvmOpNULL :
     if (isArf) {vmopArf3(pAr1,>,pAr2,out);}
     else{
      out= OpCompStr(&bAr1,&bAr2);
      }
     break;
    default :
     err=CERRvmNotOp;
     break;
    }
   break;
  case CvmOpLow :
   switch (op2) {
    case CvmOpEqu :
     if (isArf) {vmopArf3(pAr1,<=,pAr2,out);}
     else{
      out= !(OpCompStr(&bAr1,&bAr2));
      }
     break;
    case CvmOpHight :
     if (isArf) {vmopArf3(pAr1,!=,pAr2,out);}
     else{
      if (bAr1.size!=bAr2.size) out=1;
      else
       if (memCompare(bAr1.psec,bAr2.psec,bAr1.size)) out=1;     
      }
     break;
    case CvmOpNULL :
     if (isArf) {vmopArf3(pAr1,<,pAr2,out);}
     else{
      out= OpCompStr(&bAr2,&bAr1);
      }
     break;
    default  :
     err=CERRvmNotOp;
    }
   break;
  case CvmOpEqu :
   if (isArf) {vmopArf3(pAr1,==,pAr2,out);}
   else{
    if ((bAr1.size==bAr2.size)&&( (bAr1.size==0) || (memCompare(bAr1.psec,bAr2.psec,bAr1.size)==0) ) ) out=1;
    }
   break;
  }
finish: 
 pDataSetInt(pAr1);
 pAr1->intgr=out;
 return err;
 }
/*
 */
Terr OpVec(Tdata *pAr1,Tdata *pAr2,char op) {
 Terr out;
 char *pc;
 //
 if ((out=castDataInt(pAr2))<CERR) return out;
 pDataSetInt(pAr1);
 pc=strFindChar(pCvmOpUsrData,op);
 if (pc==NULL) return CERRvmNotOpUsr;
 switch (pc-pCvmOpUsrData ) {
  //int64
//  case pCvmOpUsrData[0] :
  case 0 :
   if (pAr2->intgr >= CcountInt) return CERRvmcoorVekFiled;
   if (pAr2->intgr)
    pAr1->intgr=pAr1->int64[pAr2->intgr];
   break;
  //uint64 
//  case pCvmOpUsrData[1] :
  case 1 :
   if (pAr2->intgr >= CcountInt) return CERRvmcoorVekFiled;
   if (pAr2->intgr)
    pAr1->uint64[0]=pAr1->uint64[pAr2->intgr];
   break;
  //int32
//  case pCvmOpUsrData[2] :
  case 2 :
   if (pAr2->intgr >= CcountInt32) return CERRvmcoorVekFiled;
   pAr1->intgr=pAr1->int32[pAr2->intgr];
   break;
  //uint32 
//  case pCvmOpUsrData[3] :
  case 3 :
   if (pAr2->intgr >= CcountInt32) return CERRvmcoorVekFiled;
   pAr1->uint64[0]=pAr1->uint32[pAr2->intgr];
   break;
  //int16
//  case pCvmOpUsrData[4] :
  case 4 :
   if (pAr2->intgr >= CcountInt16) return CERRvmcoorVekFiled;
   pAr1->intgr=pAr1->int16[pAr2->intgr];
   break;
  //uint16 
//  case pCvmOpUsrData[5] :
  case 5 :
   if (pAr2->intgr >= CcountInt16) return CERRvmcoorVekFiled;
   pAr1->uint64[0]=pAr1->uint16[pAr2->intgr];
   break;
  //int16
//  case pCvmOpUsrData[6] :
  case 6 :
   if (pAr2->intgr >= CcountInt8) return CERRvmcoorVekFiled;
   pAr1->intgr=pAr1->int8[pAr2->intgr];
   break;
  //uint16 
//  case pCvmOpUsrData[7] :
  case 7 :
   if (pAr2->intgr >= CcountInt8) return CERRvmcoorVekFiled;
   pAr1->uint64[0]=pAr1->uint8[pAr2->intgr];
   break;
  default :
   return CERRvmNotOpUsr;
  }
 return COutKodOk;
 }
/*
 * Унарные операции
 */
#define OpStrLog(Op) OpRunstrOp(pAr1,&bstr1); OpRunstrOp(pAr2,&bstr2);\
    if (bstr2.size) { if (bstr1.size){ n=(bstr1.size < bstr2.size)?bstr1.size:bstr2.size;\
      for (j=0;j<n;++j) bstr1.psec[j] = bstr1.psec[j] Op bstr2.psec[j];} else{*pAr1=*pAr2;} }

Terr OpRunUno(Tdata *pAr,char op,char op1) {
 Tbstr bstr;
 int j;
 Terr out;
 //
 switch (op) {
  case CvmOpMinus :
   if ((out=castDataArf(pAr))<CERR) return out;
   vmopArfUn(pAr,-);
   //
   return COutKodOk;
  case CvmOpNot :
   if ((out=castDataInt(pAr))<CERR) return out;
   pAr->intgr=(pAr->intgr)?0:1;
  case CvmOpStr :
   if (op1==CvmOpNot) {
    OpRunstrOp(pAr,&bstr);
    if (bstr.size)
     for (j=0;j < bstr.size;++j) bstr.psec[j] = ~bstr.psec[j];
    return COutKodOk;
    }
   else
    if (!op1) return castDataToStr(pAr);
   return CERRvmNotOp;
  case CvmOpPlus :
   return castDataArf(pAr);
  }
 return CERRvmNotOp;
 }
/*
 * Бинарные операции
 */
Terr OpRun(TvmDo *pDo,Tdata *pAr1,Tdata *pAr2,char op1,char op2) {
 Terr out=COutKodOk;
 //
 switch (op1) {
  case CvmOpMulti :
   if (op2==CvmOpMulti) {
    if (((out=castDataFloat(pAr1))<CERR)||((out=castDataFloat(pAr2))<CERR)) return out;
    pAr1->fnum=(TfNum)(pow(pAr1->fnum,pAr2->fnum));
    return out;
    }
   vmopArf1(pAr1,*=,pAr2,out)
   return out;
  case CvmOpMod :
   if (((out=castDataInt(pAr1))>CERR)&&((out=castDataInt(pAr2))>CERR)) (pAr1)->intgr %=(pAr2)->intgr;
   return out;
  case CvmOpPlus :
   vmopArf1(pAr1,+=,pAr2,out)
   return out;
  case CvmOpMinus :
   vmopArf1(pAr1,-=,pAr2,out)
   return out;
  case CvmOpDiv :
   vmopArf1(pAr1,/=,pAr2,out)
   return out;
  case CvmOpOr :
   vmopInt2(pAr1,&,pAr2,out);
   return out;
  case CvmOpAnd :
   vmopInt2(pAr1,|,pAr2,out);
   return out;
   //деление нацело
  case CvmOpDivInt :
   vmopArf1(pAr1,/=,pAr2,out)
   if (pDataIsFloat(pAr1)){
    pAr1->intgr=pAr1->fnum;
    pDataSetInt(pAr1);
    }
   return COutKodOk;
  //операции сравнения
  case CvmOpNot :
  case CvmOpHight :
  case CvmOpLow :
  case CvmOpEqu :
   return OpCompare(pAr1,pAr2,op1,op2);
  /*
Побитовые операции над строками :
           '_>' операция следует за
           '_<' операция содержит
           '_' операция соединения строк
           '_&' операция И
           '_|' операция ИЛИ
           '_^' операция исключающее ИЛИ
           '_~' операция НЕ
   */ 
  case CvmOpUsr :
   return OpVec(pAr1,pAr2,op2);
  }
 if (op1==CvmOpStr){
  Tdata Ar1;
  Tbstr bstr1,bstr2;
  int n,j;
  //
  Ar1=*pAr1;
  switch (op2) {
   //следует за '_>'
   case CvmOpHight :
    castbStr(pAr1,&bstr1);
    castbStr(pAr2,&bstr2);
    pAr1->intgr=OpStrLarge(&bstr1,&bstr2);
    pDataSetInt(pAr1);
    break;
   //содержит '_<'
//   case 'C' : return vmOpStrIncl(ptree,ar1,ar2); case 'W' : return vmOpStrInclW(ptree,ar1,ar2);
   case CvmOpLow :
    castbStr(pAr1,&bstr1);
    castbStr(pAr2,&bstr2);
    pAr1->intgr=OpStrInclF(&bstr1,&bstr2);
    pDataSetInt(pAr1);
    break;
   case Cvm0 : //нет второго поля в операции, операция соединения строк
    castbStr(pAr1,&bstr1);
    castbStr(pAr2,&bstr2);
    out=StrConStr(pAr1,&bstr1,&bstr2,&(pDo->Temps));
    break;
   //логические операции 
   case CvmOpOr :
    OpStrLog(|)
    break;
   case CvmOpAnd :
    OpStrLog(&)
    break;
   case CvmOpNotAnd :
    OpStrLog(^)
    break;
   case CvmOpUsr :
    castDataInt(pAr2);
    pDataSetInt(pAr1);
    break;
   }
  if (DataIsLong(Ar1)) pDataKillLongArr(Ar1.dbAdrLong.pmlong, &Ar1);
  if ( (pAr2)&&(pDataIsLong(pAr2)) ) pDataKillLongArr(pAr2->dbAdrLong.pmlong,pAr2);
  return out;
  }
 else
  return CERRvmNotOp;
 }
