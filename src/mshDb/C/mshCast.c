/*
 * File:   mshVmOp.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:51
 */
#include "mshData.h"
#include "mshPgLong.h"
#include "mshErr.h"

void bStrGet(Tdata *pData,Tbstr* pbstrOut);
/*
 * Преобразование данных
 */
/*
 * преобразокание в строку +
 */
/*
 * Преобразование дробного числа в строку +
 */
void castFloatToStr(TfNum fin,Tstr *pbuf){
 char buf[64];
 int j,decpt,sign,i;
 pbuf->size=0;
 if (fin<0) {
  pbuf->size=1;
  pbuf->psec[0]='-';
  fin=-fin;
  }
 if (fin < __DBL_EPSILON__ ) {
  pbuf->size=0;
  return;
  }
 if ((qecvt_r(fin,ClongMantisa,&decpt,&sign,&buf[0],64))<0) return;
 // точка внутри числа
 if (decpt>0) {
  (void)memCopy(&(pbuf->psec[pbuf->size]),&(buf[0]),decpt);
  pbuf->size+=decpt;
  if (decpt<ClongMantisa)
   for (j=ClongMantisa-1;j>decpt;j--)
    if (buf[j]!='0') {
     pbuf->psec[pbuf->size]='.';
     pbuf->size++;
     i=j-decpt+1;
     (void)memCopy(&(pbuf->psec[pbuf->size]),&(buf[decpt]),i);
     pbuf->size+=i;
     return;
     }
  return;
  }
 // точка до числа
 pbuf->psec[pbuf->size]='0';
 pbuf->psec[pbuf->size+1]='.';
 pbuf->size+=2;
 if (decpt<0) {decpt=-decpt;
  for (j=0;j<decpt;j++)  pbuf->psec[pbuf->size+j]='0';
  pbuf->size+=decpt;
  }
 (void)memCopy(&(pbuf->psec[pbuf->size]),&(buf[0]),ClongMantisa);
 pbuf->size+=ClongMantisa;
 while (((pbuf->size)>0)&&((pbuf->psec[pbuf->size - 1])=='0')) --(pbuf->size);
 }
/*
 * Выполнить преобразование к строке
 */
void castbStr(Tdata *pData,Tbstr *pbstrOut) {
 switch (pDataSight(pData)) {
  case CDatapStr :
   pbstrOut->size=pData->pstr->size;
   pbstrOut->psec=pData->pstr->psec;
   break;
  case CDataObj :
  case CDataStr :
   pbstrOut->size=pData->str.size;
   pbstrOut->psec=pData->str.psec;
   break;
  case CDataInt :
   IntToStr(pData->intgr,pData->str);
   pDataSetStr(pData);
   //нет проверки на переполнение
   pbstrOut->size=pData->str.size;
   pbstrOut->psec=pData->str.psec;
   break;
  case CDataFloat :
   castFloatToStr(pData->fnum,&(pData->str));
   pDataSetStr(pData);
   //нет проверки на переполнение
   pbstrOut->size=pData->str.size;
   pbstrOut->psec=pData->str.psec;
   break;
  case CDatabStr :
   *pbstrOut=pData->bstr;   
   break;
  case CDatapbStr :
   *pbstrOut=*(pData->pbstr);   
   break;
  case CDataNull : pbstrClear(pbstrOut);
  default:
   bStrGet(pData,pbstrOut);
   break;
 }
}
//
Tstr* castId(Tdata *pData) {
 Tstr *pstr;
 Tbstr bstr;
 //
 switch (pDataSight(pData)) {
  case CDataNull:
   return NULL;
  case CDatapStr :
   if (pstr==&(pData->str)){
    pDataSetStr(pData);
    }
   else{
    pstr=pData->pstr;   
    pStrShortTopData(pData,pstr);
    }
   break;
  case CDataObj :
  case CDataStr :
   break;
  case CDataInt :
   IntToStr(pData->intgr,pData->str);
   pDataSetStr(pData);
   //нет проверки на переполнение
   break;
  case CDataFloat :
   castFloatToStr(pData->fnum,&(pData->str));
   pDataSetStr(pData);
   //нет проверки на переполнение
   break;
  case CDatabStr :
   bstr=pData->bstr;
   StrShortTopData(pData,bstr);
   break;
  case CDatapbStr :
   bstr=*(pData->pbstr);
   StrShortTopData(pData,bstr);
   break;
  default:
   bStrGet(pData,&bstr);
   StrShortTopData(pData,bstr);
   break;
  }
return &(pData->str);
}
/*
 * Приведение к Арифметическим типам
 */
/*
 * из строки перебросить в буфер только Арифметическое значение
 */
static Terr bStrIsArf(Tdata *pData,int *piSize ,int *pisInt,int *pisNum,Tbstr *pbStr,int isCast){
 int isPnt,w;
 char *pc,*pb,buf[CCsizeStr+1] ;
 Terr err=COutKodOk;
 //
 *piSize=*pisInt=*pisNum=0;
 if (pDataIsNum(pData)) return COutKodOk;
 bStrGet(pData,pbStr);
 if ((pbStr->psec==NULL)||(pbStr->size<1)||(pDataIsList(pData))) return CWARunNum;
 pc=pbStr->psec;
 w=(pbStr->size>CCsizeStr)?CCsizeStr:pbStr->size ;
 pb=pc;
 *pisInt=*pisNum=1;
 isPnt=0;
 if (*pc==CvmOpMinus) {
  ++pc;++*piSize;
  }
 else
  if (*pc==CvmOpPlus) {pc++;pb++;w--;}
 while (*piSize<w) {
  if (*pc==CvmOpPoint) {
   if (isPnt) {
    *pisNum=0;
    err=CWARunNum;
    if (isCast) goto finish;
    return CWARunNum;
    }
   ++isPnt;
   *pisInt=0;
   }
  else {
   if (((*pc)>'9')||((*pc)<'0')) {
    *pisNum=0;
    err=CWARunNum;
    if (isCast) goto finish;
    return CWARunNum;
    }
   }
  ++*piSize;++pc;
  }
finish: 
 memCopy(buf,pb,*piSize);
 buf[*piSize]='\0';
 //преобразуется к числу
 if (*pisNum==0) return CWARunNum;
 if (*pisInt){
  pData->intgr = StrToInt(buf);
  pDataSetInt(pData);
  }
 //преобразуется к действительному числу
 else{
  StrToFloat(buf,pData->fnum);
  pDataSetFloat(pData);
  }
 return COutKodOk;
 }
/*
 * приведение тип Tdata к Целому типу
 */
Terr castDataInt(Tdata *pData){
 Tbstr bStr,*pbStr=&bStr;
 //
 switch (pDataSight(pData)) {
  case CDataNull :
   pData->intgr=0;
   pDataSetInt(pData);
   return CWARunNum;
  case CDataInt :
   return COutKodOk;
  case CDataFloat :
   pDataSetInt(pData);
   pData->intgr = pData->fnum;
   return COutKodOk;
  }
 Tdata Data;
 Data=*pData;
 bStrGet(pData,&bStr); 
 if (bStr.size>ClongMantisa) bStr.size=ClongMantisa;
 if((pData->str.size=StrTocharInt(pbStr,(char*)&(pData->str.psec)))>0) {
  //преобразуется к целому числу
  if (pData->str.psec[pData->str.size]!='\0') pData->str.psec[pData->str.size]='\0';
  pData->intgr = StrToInt(pData->str.psec);
  pDataSetInt(pData);
  return COutKodOk;
  }
 pDataSetNULL(pData);
 return CERRvmFormatIntFiled;
 }

long long int castbStrToInt(Tbstr *pbStr){
 if ((pbStr==NULL)||(pbStr->size==0)) return 0;
 if (pbStr->psec[pbStr->size]==Cvm0 ) return StrToInt(pbStr->psec);
 if (pbStr->size>ClongMantisa) pbStr->size=ClongMantisa;
 char bf[pbStr->size+1];
 memCopy(bf,pbStr->psec,pbStr->size);
 bf[pbStr->size]=Cvm0;
 return StrToInt(bf);
 }
/*
 * Преобразование Данных к наиболее удобному типу
 */
Terr castNorm(Tdata *pData){
 int isInt,isNum,iSize;
 Tbstr bStr;
 //
 if ((pDataIsList(pData))||(bStrIsArf(pData,&iSize ,&isInt,&isNum,&bStr,0)==COutKodOk)) return COutKodOk;
 if (pDataIsStr(pData)) return COutKodOk;
 if (bStr.psec==NULL) {pDataSetNULL(pData);return COutKodOk;}
 if (bStr.size<=CCsizeStr){  StrTopData(bStr,pData);  }
 return COutKodOk;
 }
/*
 * приведение тип Tdata к Арифметическоиу типу
 */
Terr castDataArf(Tdata *pData){
 int isInt,isNum,iSize;
 Tbstr bStr;
 //
 if (pDataIsList(pData)) {
  bStrGet(pData,&bStr);
  *pData=*((Tdata*)(bStr.psec));
  }
 switch (pDataSight(pData)) {
  case CDataNull :
   pData->intgr=0;
   pDataSetInt(pData);
   return CWARunNum;
  case CDataInt :
  case CDataFloat :
   return COutKodOk;
  }
 if (pDataIsNum(pData)) {
  pDataSetInt(pData);
  return CWARunNum;
  }
 if (bStrIsArf(pData,&iSize ,&isInt,&isNum,&bStr,1)==COutKodOk) return COutKodOk;
 pDataSetInt(pData);
 pData->intgr=0;
 return CWARunNum;
 }
/*
 * приведение тип Tdata к Действительному типу
 */
Terr castDataFloat(Tdata *pData){
 Terr out;
 if ((out=castDataArf(pData))<CERR) return out;
 if (pDataIsInt(pData)){
  pData->fnum=pData->intgr;
  pDataSetFloat(pData);
  }
 return COutKodOk;
 }
/*
 * приведение тип Tdata к строке
 */
Terr castDataToStr(Tdata *pData){
 switch (pDataSight(pData)) {
  case CDataNull:
   pData->str.size=0;
   pDataSetStr(pData);
   break;
  case CDataInt :
   IntToStr(pData->intgr,pData->str);
   pDataSetStr(pData);
   break;
  case CDataFloat :
   castFloatToStr(pData->fnum,&(pData->str));
   pDataSetStr(pData);
   break;
  }
 return COutKodOk;
 }
