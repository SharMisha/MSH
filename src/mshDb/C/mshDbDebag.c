/*
 * File:   mshDebag.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 20 Август 2014 г., 12:18
 */
#include "mshDebag.h"
#include "mshData.h"
#include "mshDb.h"
#include "mshDiv.h"
#include "mshPgLong.h"

void roundPgDbAltRoot(TDb *pDb, void (*pfbeg)(TuPg *puPg, int Level,void *pUser), void (*pfend)(TuPg *puPg, int Level,void *pUser),void *pUser );
void roundPgDbAlt(TDb *pDb, Tadr Adr, void (*pfbeg)(TuPg *puPg, int Level,void *pUser), void (*pfend)(TuPg *puPg, int Level,void *pUser), int *pLevel,void *pUser);
void bStrGet(Tdata *pData,Tbstr* pbstrOut);
void pdataLongRef(TmapLong *pmLong,Tdata *pData);
//создать тестовую строку
void vmtstStrNew(Tstr *pstr,char *pc){
 int j;
 char c='A';
 if (pc==NULL) pc=&c;
 for (j=0;j<pstr->size;j++) pstr->psec[j]=(*pc)++;
 }
/*
 * Печать строки
 */
void vmtstWriteBuf(char *pC,int len,TdivStream *pdiv){
 char *pE=pC+len;
 for (; pC < pE ; ++pC)
  if (*pC) {vmPrnt(pdiv,"%c",*pC);}
 }
void vmtstWriteStr(Tstr* pstr,TdivStream *pdiv){
 //
 if ((pstr!=NULL)&&(pstr->size)) {
  char out[pstr->size+1];
  memCopy(out,pstr->psec,pstr->size);
  out[pstr->size]='\0';
  vmPrnt(pdiv,"size=%u %s",pstr->size,out);
  }
 }
/*
 */
void vmtstNewWriteStr(Tstr *pstr,TdivStream *pdiv){
 vmPrnt(pdiv,"Тест Строк");
 vmPrntBK(pdiv);
 vmtstStrNew(pstr,NULL);
 vmtstWriteStr(pstr,pdiv);
 vmPrntBK(pdiv);
 }
/**/
void vmtstWriteAdr(Tadr adr,TdivStream *pdiv){
 vmPrnt(pdiv,"%lu ",adr);
 }
void vmtstWriteData(Tdata *pData,TdivStream *pdiv);

 void vmtstWriteArrDataSec(Tdata *pData,TdivStream *pdiv){
  if (pData==NULL) {
   vmPrnt(pdiv,"NULL");
   vmPrntBK(pdiv);
   }
  else vmtstWriteData((Tdata*)pData,pdiv);
  }
 void vmtstWriteArrInt(TarrInt *pArr,TdivStream *pdiv){
  vmtstWriteArrHead(pArr,pdiv);
  if ((pArr==NULL)||(parrIsNull(pArr))) return;
  int *pD,i=0;
  parrIter(pArr,pD) {
   vmPrnt(pdiv," %d",*pD);
   if (( i % 10 == 0)&& i) {vmPrntBK(pdiv);}
   ++i;
   }
  vmPrntBK(pdiv);
  }
 
 void vmtstWriteArrpVoid(TarrpVoid *pArr,TdivStream *pdiv){
  if ((pArr==NULL)||(parrIsNull(pArr))) return;
  void **ppD;
  int i=0;
  vmPrntBK(pdiv);
  parrIter(pArr,ppD) {
   vmPrnt(pdiv," %p",*ppD);
   if (( i % 10 == 0)&& i) {vmPrntBK(pdiv);}
   ++i;
   }
  vmPrntBK(pdiv);
  }
 void vmtstWriteDataArr(TarrData *pArr,TdivStream *pdiv){
  if ( (pArr==NULL)||(parrIsNull(pArr)) ) return;
  int i;
  Tdata *pD;
  i=0;
  parrIter(pArr,pD) {
   vmPrnt(pdiv," [%d]",i);
   vmtstWriteArrDataSec(pD,pdiv);
   if (( i % 5 == 0)&& i) {
    vmPrntBK(pdiv);
    }
   i++;
   }
  vmPrntBK(pdiv);
  }
 void vmtstWriteArrData(TarrData *pArr,TdivStream *pdiv){
  if (pArr){
   vmtstWriteArrHead(pArr,pdiv);
   vmtstWriteDataArr(pArr,pdiv);
   }
  }
/**/
void vmtstWriteData(Tdata *pData,TdivStream *pdiv){
 char type;
 TarrData Arr;
 Tbstr bStr;
 //
 if (pData==NULL) return;
 type= pDataSight(pData);
 if (pDataIsAlt(pData)){vmPrnt(pdiv,":");}
 //распечатать массив
 if (pDataIsList(pData)){
  vmPrnt(pdiv,"{}");
  bStrGet(pData,&bStr);
  if (bStr.psec) {
   StrToArrData(bStr,Arr);
   vmPrntBK(pdiv);
   vmtstWriteDataArr(&Arr,pdiv);
   }
  return;
  }
 switch (type) {
  case CDataNull :
   vmPrnt(pdiv,"None");
   break;
  case CDataInt  :
   vmPrnt(pdiv,"Int 0=%ji , 1=%ji ",pData->intgr,pData->int64[1]);
   break;
  case CDataFloat  :
   vmPrnt(pdiv,"Float=%f ",pData->fnum);
   break;
  case CDataPointer  :
   vmPrnt(pdiv,"Pointer=%p [%u]",pData->pointer,pDataSubPar(pData));
   break;
  //строки
  case CDataArea :
   vmPrnt(pdiv,"AreaOut={parea=%p off=%d} ",pData->areaOff.parea,pData->areaOff.off);
   break;
  case CDataLong :
   vmPrnt(pdiv,"Long={datapmLong=%p adr=%lu off=%u}",pData->datapmLong,pData->dataLongAdr,pData->dataLongOff);
   break;
  case CDataObj :
   vmPrnt(pdiv,"Obj: ");
   vmtstWriteStr(&(pData->str),pdiv);
   break;
  case CDataStr :
   vmPrnt(pdiv,"Str: ");
   vmtstWriteStr(&(pData->str),pdiv);
   break;
  case CDatapStr :
   vmPrnt(pdiv,"pStr: ");
   vmtstWriteStr(pData->pstr,pdiv);
   break;
  default:
   bStrGet(pData,&bStr);
   vmtstWriteBuf(bStr.psec,bStr.size,pdiv);
   break;
  }
 }
/**/
/**/
 void vmtstWriteAreaInt(Tarea *parea,TdivStream *pdiv){
  Tsize *pB,*pend;
  vmtstWriteAreaHead(parea,pdiv);
  pB=(parea->psec);
  pend=pB+parea->busy;
  while(pB<pend){
   vmPrnt(pdiv,"[%p , %Ld] ",pB,(long long int)(pB - parea->psec));
   vmtstWriteStr((Tstr*)pB,pdiv);
   vmPrntBK(pdiv);
   pB += mshDivUp(((Tstr*)pB)->size,sizeof(Tsize))+1;
   }
  }
/**/
 void vmtstWriteAreaChar(TareaChar *parea,TdivStream *pdiv){
  char *pD,*pend;
  vmtstWriteAreaHead(parea,pdiv);
  pD=(parea->psec);
  pend=pD+parea->busy;
  while(pD<pend){
   vmPrnt(pdiv,"%c",*pD);
   pD++;
   }
  vmPrntBK(pdiv);
  }

 void vmtstWriteArrSec(int *pD,int *pbeg,TdivStream *pdiv){
  if (pD==NULL) {
   vmPrnt(pdiv,"NULL");
   }
  else {
   vmPrnt(pdiv,"[%p ",pD);
   if (pbeg!=NULL) {vmPrnt(pdiv," %Ld",(long long int)(pD-pbeg));}
   vmPrnt(pdiv,"]=%d ",*pD);
   }
  }
/*
 * Печать длинной строки
 */
void vmtstWriteLongStr(Tlong *pg,Tsize i,TdivStream *pdiv){
 Tstr* pstr;
 pstr=(Tstr*)(pgLongpStr(pg,i));
// if (pstr!=NULL)
  vmtstWriteStr(pstr,pdiv);
 }
/*
 * печать Страницы длинных строк TpgLong
 */
void vmtstWritePgLong(Tlong *pg,TdivStream *pdiv){
 vmPrnt(pdiv,"Страница %p Заголовок size %u busy %u free %u",pg,pg->size,pg->busy,pg->free);
 vmPrntBK(pdiv);
 if (pg->busy){
  Tsize Off;
  Tstr *pstr;
  for (Off=0;Off<pg->busy;Off++) {
   vmPrnt(pdiv,"%u %d ",Off,pg->psec[Off]);
   pstr=(Tstr*)(pgLongpStr(pg,Off));
   vmtstWriteStr(pstr,pdiv);
   vmPrntBK(pdiv);
   }
  }
 }
//
void vmtstWriteSecLongs(TmapLong *pmapLong,TdivStream *pdiv){
 void *pLong;
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"ListLongs %p Long0 %p size %li sizesec %li",pmapLong,pmapLong->psec,pmapLong->size,pmapLong->sizesec);
 vmPrntBK(pdiv);
 for (pLong=pmapLong->psec;pLong < pmapLong->psec + pmapLong->size;pLong += pmapLong->sizesec)
  vmtstWritePgLong((Tlong*)pLong,pdiv);
 vmPrntBK(pdiv);
 }
char vmtstPgType(char type){
 switch(type){
  case CTypePgFree : return 'F';
  case CTypePgLink : return 'L';
  case CTypePgData : return 'D';
  }
 return '0';
 }
/*
 */
void vmtstWritePgHead(Tadr Adr,Tpg* pg,TdivStream *pdiv){
 if (pg==NULL){
  vmPrnt(pdiv," NULL");
  }
 else{
  vmPrnt(pdiv," Adr=%lu",Adr);
  vmPrnt(pdiv," pg=%p",pg);
  vmPrnt(pdiv," Тип %c size %u busy %u",vmtstPgType(pg->type),pg->size,pg->busy);
  }
 }
void vmtstWritePgHeadData(Tadr Adr,TpgData* pg,TdivStream *pdiv){
 if (pg==NULL) {
  vmPrnt(pdiv," NULL");
  return;
  }
 vmtstWritePgHead(Adr,(Tpg*)pg,pdiv);
 vmPrnt(pdiv," left=%lu right=%lu",pg->left,pg->right);
 }
/**/
void vmtstWriteKey(Tkey *pkey,TdivStream *pdiv){
 vmPrnt(pdiv,"(adr=");
 vmtstWriteAdr(pValAdr(pkey),pdiv);
 vmPrnt(pdiv,")");
 vmtstWriteData(&(pValKey(pkey)),pdiv);
 }
/*
 */
void vmtstWritePgLink(Tadr indpg,TpgLink *pg,TdivStream *pdiv){
 int i;
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"pgLink");
 vmtstWritePgHead(indpg,(Tpg*)pg,pdiv);
 vmPrntBK(pdiv);
 if ((pg->busy)>0)
  for (i=0;i<(pg->busy);i++) {
   vmPrnt(pdiv,"%u ",i);
   vmtstWriteKey(pg->psec + i,pdiv);
   vmPrntBK(pdiv);
   }
 }
/*
 */
void vmtstWritePgFree(ToffData indpg,TpgFree *pg,TdivStream *pdiv){
 int i;
 vmPrntBK(pdiv);
 vmPrnt(pdiv," TpgFree");
 vmtstWritePgHead(indpg,(Tpg*)pg,pdiv);
 vmPrntBK(pdiv);
 if ((pg->busy)>0)
  for (i=0;i<(pg->busy);i++) {
   vmPrnt(pdiv,"%u ",i);
   vmtstWriteAdr(pg->psec[i],pdiv);
   vmPrntBK(pdiv);
   }
 }
/**/
void vmtstWriteKeyData(Tval *pVal,TdivStream *pdiv){
 if (pVal) {
  vmPrnt(pdiv," [");
  vmtstWriteKey((Tkey*)pVal,pdiv);
  vmPrnt(pdiv,"] ");
  vmtstWriteKey(&(pVal->val),pdiv);
  }
 else{vmPrnt(pdiv,"NULL");}
 }
/*
 */
void vmtstWritePgData(Tadr adr,TpgData *pg,TdivStream *pdiv){
 int i;
 Tval *pVal;
 //
 vmPrntBK(pdiv);
 vmPrnt(pdiv," pgData");
 vmtstWritePgHeadData(adr,pg,pdiv);
 vmPrntBK(pdiv);
 if ((pg->busy)>0)
  for (i=0;i<(pg->busy);i++) {
   vmPrnt(pdiv,"%u ",i);
   pVal=pg->psec + i;
//   pdataLongRef(&(pDb->mlong),&(pValKey(pVal)) );
//   pdataLongRef(&(pDb->mlong),&(pValData(pVal)) ); 
   vmtstWriteKeyData(pVal,pdiv);
   vmPrntBK(pdiv);
   }
 }
/*
 */
void vmtstWriteAdrOff(TadrOff *pAdrOff,TdivStream *pdiv){
 vmtstWriteAdr(pAdrOff->adr,pdiv);
 vmPrnt(pdiv," off=%i",pAdrOff->off);
 vmPrntBK(pdiv);
 }
/*
 */
void vmtstWriteAdrOfferr(TAdrOfferr *pAdrOfferr,TdivStream *pdiv){
 vmtstWriteAdr(pAdrOfferr->adr,pdiv);
 vmPrnt(pdiv," indpg=%lu off=%i err=%d isalt=%d"
  ,pAdrOfferr->adr,pAdrOfferr->off,pAdrOfferr->err,pAdrOfferr->isalt);
 vmPrntBK(pdiv);
 }
void vmtstWriteArrAdrOfferr(TarrAdrOfferr *parrAdrOfferr,TdivStream *pdiv){
 TAdrOfferr *pAdrOfferr;
 vmtstWriteArrHead(parrAdrOfferr,pdiv);
 parrIter(parrAdrOfferr,pAdrOfferr) vmtstWriteAdrOfferr(pAdrOfferr,pdiv);
 vmPrntBK(pdiv);
 }
/*
TDb *pdb;Tval *pval;char type;
 */
void vmtstWriteNext(TIterNext *pNext,TdivStream *pdiv){
 vmtstWriteAdr(pNext->AdrOfferr.adr,pdiv);
 vmPrnt(pdiv," indpg=%lu off=%i err=%d",pNext->AdrOfferr.adr,pNext->AdrOfferr.off
         ,pNext->AdrOfferr.err);
 vmPrnt(pdiv," pdb=%p type=%c pval=",pNext->pdb,pNext->type);
 vmtstWriteKeyData( &(pNext->val),pdiv);
 vmPrntBK(pdiv);
 }
/**/
void vmtstWriteQuery(TIterQuery *pcurQuery,TdivStream *pdiv){
 vmtstWriteNext((TIterNext*)pcurQuery,pdiv);
 if ( arrIsNoNull(pcurQuery->addInd) ){
  vmPrnt(pdiv,"AddInd=");
  vmtstWriteArrData(&(pcurQuery->addInd),pdiv);
  }
 vmPrnt(pdiv,"узел=");
 vmtstWriteKeyData(&(pcurQuery->iterNext.val),pdiv);
 vmPrntBK(pdiv);
 }
/*
 *  size_t sizemap;
 size_t sizesec;
 Tdiv div;
 */
 void vmtstWriteDbArr(TDb *pDb,TdivStream *pdiv){
  int j,w;
  Tdata *pData;
  //
  if (pDb->mpg.psec==NULL) return;
  vmPrnt(pdiv,"TmapArr psec=%p sizemap=%lu sizedata=%lu busy=%li div=%i"
          ,pDb->mpg.psec,pDb->mpg.size, mapSizeData(pDb->mpg.size),((Tdata*)(pDb->mpg.psec))->intgr,pDb->mpg.div);
  vmPrntBK(pdiv);
  w=maparrpDbBusy(pDb);
  for(j=1;j<=w;++j){
   pData=mapArrpDataOff(pDb,j);
   vmPrnt(pdiv,"[%i]=",j);
   pdataLongRef(&(pDb->mlong),pData);   
   vmtstWriteData(pData,pdiv);
   vmPrntBK(pdiv);
   }
  }
 /*
  * вывод на печать заголовок Дерева
  */
 void vmtstWriteTreeHead(TDb *pDb,TdivStream *pdiv){
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"Tree %p CsizePgDef=%d CsizeLongDef=%d CsizeArrDef=%d sizePg=%lu siseLong=%lu" ,pDb,CsizeDbPgDef,CsizeDbLongDef,CnumArrDef,pDb->mpg.size,pDb->mlong.size);
  vmPrntBK(pdiv);
  if ( arrIsNoNull(pDb->trackdb) ) {
   vmPrnt(pdiv,"Трасса Link ");
   vmtstWriteArrAdrOfferr((TarrAdrOfferr*)(&(pDb->trackdb)),pdiv);
   }
  vmPrnt(pdiv," free=");
  vmtstWriteAdr(pgFree,pdiv);
  vmPrnt(pdiv,"root=");
  vmtstWriteAdr(pgRoot(pDb),pdiv);
  vmPrntBK(pdiv);
  }

typedef struct { 
 TDb *pDb;
 TdivStream *pdiv;
 } TpDbpdiv;

 
 void vmtstWriteTreePg(Tadr adr,TuPg *puPg,TdivStream *pdiv){
  if (puPg->pg->busy)
   switch (puPg->pg->type) {
    case CTypePgData : vmtstWritePgData(adr,puPg->pgD,pdiv); return;
    case CTypePgLink : vmtstWritePgLink(adr,puPg->pgL,pdiv); return;
    case CTypePgFree : vmtstWritePgFree(adr,puPg->pgF,pdiv); return;
    }
  }
 /* распечатка всех страниц дерева */
 void vmtstWriteTreeSub(TuPg *puPg, int Level,void *pUser){
  TpDbpdiv *pDbpdiv=(TpDbpdiv*)pUser;
  vmtstWriteTreePg(puPg->pvoid - ((TDb*)(pDbpdiv->pDb))->mpg.psec,puPg,pDbpdiv->pdiv);
  }

 void vmtstWriteTree(TDb *pDb,TdivStream *pdiv){
  int Level=0;
  TpDbpdiv Dbpdiv;
  //
  Dbpdiv.pDb=pDb;
  Dbpdiv.pdiv=pdiv;
  vmtstWriteTreeHead(pDb,pdiv);
  roundPgDbAltRoot(pDb, vmtstWriteTreeSub,NULL,&Dbpdiv );
  vmPrntBK(pdiv);
  Level=0;
  roundPgDbAlt(pDb, pgFree, vmtstWriteTreeSub, NULL, &Level,&Dbpdiv);
  }
