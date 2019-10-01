/*
 * File:   mshIni.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 14:47
 */
#include "mshIni.h"
#include "mshData.h"
#include "mshDiv.h"
#include "mshSmb.h"
#include "mshErr.h"
//
void fileDirPacht(Tbstr *pname,const char *pDir,char *pout);

/*
 */
int iniRead(TIni *pIni){
 char *pend,*pbuf,*pbeg;
 //
 parrClear(pIni);
 pbuf=pIni->Buf.psec;
 pend=pbuf+pIni->Buf.size;
 while (pbuf<pend) {
  //пропустить пробелы
  trSmbSpace(pbuf,pend);
  if (pbuf>=pend) goto finish;
  //комментарии
  if ((*pbuf) == CvmCommentSlash){
   do{trSmbNext(pbuf);}while ((pbuf<pend)&&((*pbuf)!=CCvmEndStr));
   }
  else{
   //имя параметра
   parrBusyNull(pIni);
   pIni->ptop->key.psec=pbuf;
   //находим конец имени и помещаем его в поле name
   while ( (pbuf<pend)&&((*pbuf) != CCvmEndStr)&&((*pbuf) != CCvmBK)&&((*pbuf) != CiniDelemiter)&&(!(trIsSpace(*pbuf))) ) {    trSmbNext(pbuf);    }
   if (pbuf>=pend) goto finish;
   pIni->ptop->key.size=pbuf - pIni->ptop->key.psec;
   trSmbSpace(pbuf,pend);
   if (pbuf>=pend) goto finish;
   //если задано значение параметра
   if ( *pbuf==CiniDelemiter ) {
    // проходим этот символ
    trSmbNext(pbuf);
    trSmbSpace(pbuf,pend);
    if (pbuf>=pend) goto finish;
    //где находится строка со значением
    pIni->ptop->val.psec=pbuf;
    if ( (*pbuf=='\'')||(*pbuf=='"') ) {
     pbeg=pbuf;
     trSmbNext(pIni->ptop->val.psec);
     pbuf=pIni->ptop->val.psec;
     //
     while(((trSmbNext(pbuf)) < pend)&&(*pbuf!=*pbeg));
     if (pbuf>=pend) goto finish;
     if (pbuf-pIni->ptop->val.psec>1){
      pIni->ptop->val.size=pbuf-pIni->ptop->val.psec;
      if (*pbuf==*pbeg ) pIni->ptop->val.size--;
      trSmbNext(pbuf);
      }
     }
    else{
     while ( (pbuf<pend)&&( *pbuf != CiniendPar )) {    trSmbNext(pbuf);    }
     pIni->ptop->val.size=pbuf-pIni->ptop->val.psec;
     }
    if ( *pbuf == CiniendPar ) {    trSmbNext(pbuf);    }
    }
   }
  while ((pbuf<pend)&&((*pbuf==CCvmEndStr)||(*pbuf==CCvmBK)||(*pbuf==CiniendPar) ) ){  trSmbNext(pbuf); }
  }
finish:
 return COutKodOk;
 }

/**/
int iniReadFL(char *pflname,TIni *pIni){
 Tdiv div;
 size_t Size;
//
 if ((div=divOpen(pflname))==CdivRetErr) return COutKodOk;
 if ((Size=divPosEnd(div))<3) return COutKodOk;
 bstrWide(pIni->Buf,Size+1);
 pIni->Buf.size=Size;
 Size=divReadPos(div,pIni->Buf.psec,Size,0);
 divClose(div);
 pIni->Buf.psec[Size]=Cvm0;
 if (Size!=pIni->Buf.size) return CERRdivReadPart;
 return iniRead(pIni);
 }
/**/
static int iniInitReadFL(char *pflname,TIni *pIni){
 int err;
 //
 piniInit(pIni);
 err=iniReadFL(pflname,pIni);
 return err;
 }
/**/
int iniDirInit(Tbstr *pbfl,const char *pDir,TIni *pIni){
 char bf[CsizePacthFl];
 //
 fileDirPacht(pbfl,pDir,bf);
 return iniInitReadFL(bf,pIni);
 }

int iniDirReadFL(Tbstr *pbfl,const char *pDir,TIni *pIni){
 char bf[CsizePacthFl];
 //
 fileDirPacht(pbfl,pDir,bf);
 return iniReadFL(bf,pIni);
 }

/**/
Tbstr* iniKeygetVal(TIni *pIni,Tbstr *pbsKey){
 TbKeyVal *pbKeyVal;
 //
 parrIter(pIni,pbKeyVal)
  if ((StrpEqu(pbKeyVal->key,pbsKey))==CMPeq) return &(pbKeyVal->val);
 return NULL;
 }
/**/
Tbstr* iniChargetVal(TIni *pIni,char cKey){
 TbKeyVal *pbKeyVal;
 //
 parrIter(pIni,pbKeyVal) if (pbKeyVal->key.psec[0]==cKey) return &(pbKeyVal->val);
 return NULL;
 }
