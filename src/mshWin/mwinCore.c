/* 
 * File:   mwinCore.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 5 февраля 2019 г., 15:26
 */
#include <gtk/gtk.h>
#include "mshMem.h"
#include "mshErr.h"
#include "mshCnst.h"
#include "mshData.h"
#include "mshSmb.h"
#include "mwinCore.h"
#include "mwinWidget.h"
//
TwinDb winDb;
//GQuark qPack;
//GQuark qType;
//GQuark qParent;
extern TwinDb winDb;
extern int sizeBuf;

//
void castbStr(Tdata *pData,Tbstr *pbstrOut);
Tstr* castId(Tdata *pData);
Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
Terr vmwingetModLabel(Tstr *psMod,Tstr *psLabel,void *pUser,Tdata *pdataModLabel);
void winCalendarGetData(GtkWidget *pObj,Tbstr *psbf);
void winViewGridGet(GtkTreeView *pView,Tarea *pArea);
Terr castDataInt(Tdata *pData);
void winWinSetTitle(GtkWidget *pObj,char *ptitle);
void winRowToNblabel(GtkNotebook *pNb,Tbstr *pbstrBuf);
void winLbSetTitle(GtkWidget *pObj,char *ptitle);
void winButSetTitle(GtkWidget *pObj,char *plabel);
void winViewRowsListStore(GtkListStore *pStore,Tbstr *pbstrBuf);
void winViewRowsTreeStore(GtkTreeStore *pStore,Tbstr *pbstrBuf);
void winCalendarSetData(GtkWidget *pObj,Tbstr *psbf);
char* winWinGetTitle(GtkWidget *pObj);
char* winLbGetTitle(GtkWidget *pObj);
char* winButGetTitle(GtkWidget *pObj);
void fileDirPacht(Tbstr *pname,const char *pDir,char *pout);
/*
 * найти имя объекта
 */
void getdataNameObj(GtkWidget *pObj,Tdata *pdName){
 pDataSetbStr(pdName);
 pdName->bstr.psec=gtk_widget_get_name(pObj);
 pdName->bstr.size=strlen(pdName->bstr.psec);
 }
//
Terr setPardataObj(Tdata *pdName,EqType qpar,Tdata *psetData){
 TarrData Inds;
 Tdata arData[2];
 //
 Inds.busy=Inds.size=2;
 Inds.psec=arData;
 Inds.ptop=arData+1;
 arData[0]=*pdName;
 pDataSetAlt(arData);
 pDataSetInt(Inds.ptop);
 Inds.ptop->intgr=qpar;
 return mshSet(&(winDb.Db),&Inds,psetData);
 }
Terr setParObj(GtkWidget *pObj,EqType qpar,Tdata *psetData){
 Tdata dName;
 //
 getdataNameObj(pObj,&dName);
 return setPardataObj(&dName,qpar,psetData);
 }
Tdata* getParObj(GtkWidget *pObj,EqType qpar){
 TarrData Inds;
 Tdata arData[2];
 Tval *pVal;
 Terr err;
 //
 getdataNameObj(pObj,arData);
 Inds.busy=Inds.size=2;
 Inds.psec=arData;
 Inds.ptop=arData+1;
 pDataSetAlt(arData);
 pDataSetInt(Inds.ptop);
 Inds.ptop->intgr=qpar;
 pVal=mshGet(&(winDb.Db),&Inds,&err);
 return (pVal==NULL)?NULL:&(pValData(pVal));
 }
/*
 * строковый аргумент преобразовать в Си строку
 */
char* strToBuf(Tdata *pArg,Tbstr *pbStr,char *pbuf,int sizebuf,char *pDef){
 if (pArg==NULL) return pDef;
 castbStr(pArg,pbStr);
 if (pbStr->size<1) return pDef;
 if (pbStr->psec[pbStr->size]=='\0') return pbStr->psec;
 if ( (pbStr->size+1)>=sizebuf ) pbStr->size=sizebuf-1;
 memCopy(pbuf,pbStr->psec,pbStr->size);
 pbuf[pbStr->size]='\0';
 return pbuf;
 }
/*
 */
void wingetPachFl(Tdata *pArg,const char *pDir,char *pout){
 Tbstr bStr;
 //
 castbStr(pArg,&bStr);
 fileDirPacht(&bStr,pDir,pout);
 }
/*
 */
void selectRow(Tbstr *pbstrBuf,char **ppB,char **ppE){
 char *pC;
 //
 *ppB=pbstrBuf->psec;
 *ppE=*ppB+pbstrBuf->size;
 smbSpaceSkip(*ppB,*ppE);
 for (pC=*ppB;pC<*ppE;){
  switch(*pC){
   //начало уровня
   case CbegDataTree :
    trSmbNext(pC);
    smbSpaceSkip(pC,*ppE);
    *ppB=pC;
    break;
   case CendDataTree :
    *ppE=pC;
    return;
   default : 
    trSmbNext(pC);
    break;
   }
  }
 }
/*
 * по Имени найти Объект
 */
Tdata* findatrObj(TDb *pDb,Tdata *pdataName){
 TarrData Inds;
 Tval* pVal;
 Terr err;
 //
 Inds.busy=Inds.capa=Inds.size=1;
 Inds.psec=Inds.ptop=pdataName;
 pVal=mshGet(pDb,&Inds,&err);
 return ( (pVal==NULL)||(err<CERR) )?NULL:&(pValData(pVal));
 }
/**/
GObject* findObj(TDb *pDb,Tdata *pdataName){
 Tdata *pdAtr;
 //
 pdAtr=findatrObj(pDb,pdataName);
 return (pdAtr==NULL)?NULL:pdAtr->voidtype2.void1;
 }
//
void butCallBack(GtkWidget *pbut,void* pUser);
/**/
Terr winSetOnInit(GtkWidget *pObj,Tdata *pdnameObj,Tstr *psLabel,Tstr *psMod,EqType qpar,void* pUser){
 Terr err;
 Tdata setData;
 //
 err=vmwingetModLabel(psMod,psLabel,pUser,&setData);
 if (err<CERR) return err;
 //запомнить ссылку на модуль и смещенеи метки
 err=setPardataObj(pdnameObj,qpar,&setData);
 //подключить программу обработки
 return COutKodOk;
 }
//
Terr winSetOn(GtkWidget *pObj,Tdata *pdnameObj,char *pcOn,Tstr *psLabel,Tstr *psMod,void* pUser){
 Terr err;
 //
 err=winSetOnInit(pObj,pdnameObj,psLabel,psMod,qOnModLab,pUser);
 //подключить программу обработки
 g_signal_connect(pObj,pcOn,G_CALLBACK(butCallBack), pUser);
 return COutKodOk;
 }
/**/
GObject* getObj(Tdata *pdName,Terr *perr){
 GObject *pObj;
 //
 if (winDb.Db.typeDb==EtypeNo) {*perr=CERRwinInitNo;return NULL;}
 (void)castId(pdName);
 pObj=findObj(&(winDb.Db),pdName);
 if (pObj==NULL) {*perr=CERRObjNotFind;return NULL;}
 return pObj;
 }
/**/
GObject* getObjAtrtype(TarrData *pArgs,EwinTypeObj *ptypeObj,EwinTypeAtr *ptypeAtr,Tdata **ppdname,Terr *perr){
 Tdata *pdName,*pdObj;
 Tstr *psname;
 //тип Атрибута
 *ptypeObj=Ewin0;
 castDataInt(pArgs->psec);
 *ptypeAtr=(EwinTypeAtr)(pArgs->psec[0].intgr);
 if ((*ptypeAtr<EwinAtrName)||(*ptypeAtr>=EwinAtrEnd) ) {*ptypeAtr=EwinAtr0;*perr=CERRAtrNotFound;return NULL;}
 //найти объект
 pdName=pArgs->psec+1;
 psname=castId(pdName);
 *ppdname=pdName;
 *perr=WARwinObjNotFound;
 if ((psname==NULL)||(psname->size<1)) return NULL;
 pdObj=findatrObj(&(winDb.Db),pdName);
 if ((pdObj==NULL)||(pdObj->voidtype2.void1==NULL)) return NULL;
 *ptypeObj=pdObj->voidtype2.type1;
 *perr=COutKodOk;
 return pdObj->voidtype2.void1;
 }
/*
 * Записать данные в объект
 */
Terr setDataObj(GObject *pObj,EwinTypeObj typeObj,Tdata *pdataSet){
 Tbstr sbAtr;
 GtkListStore *pListStore;
 Terr err=COutKodOk;
 char *pcAtr=NULL;
 char bufAtr[sizeBuf];
 //
 //
 switch(typeObj){
  //Главное окно   
  case EwinWin :
   //Атрибуты
   if (pcAtr) winWinSetTitle((GtkWidget*)pObj,pcAtr);
   break;
  //Notebook
  case EwinNb :
   castbStr(pdataSet,&sbAtr);
   if (sbAtr.size) winRowToNblabel(GTK_NOTEBOOK(pObj),&sbAtr);
   break;
  //Label 
  case EwinLb :
   pcAtr=strToBuf(pdataSet,&sbAtr,bufAtr,sizeBuf,NULL);
   if (pcAtr) winLbSetTitle(GTK_WIDGET(pObj),pcAtr);
   break;
  //кнопка 
  case EwinBut :
   pcAtr=strToBuf(pdataSet,&sbAtr,bufAtr,sizeBuf,NULL);
   if (pcAtr) winButSetTitle(GTK_WIDGET(pObj),pcAtr);
   break;
  // Grid
  case EwinGrid :
   castbStr(pdataSet,&sbAtr);
   pListStore=GTK_LIST_STORE( gtk_tree_view_get_model(GTK_TREE_VIEW(pObj)) );
   if (sbAtr.size){
    winViewRowsListStore(pListStore,&sbAtr);
    }
   break;
  // Tree 
  case EwinTree :
   castbStr(pdataSet,&sbAtr);
   if (sbAtr.size){
    winViewRowsTreeStore(GTK_TREE_STORE( gtk_tree_view_get_model(GTK_TREE_VIEW(pObj)) ),&sbAtr);
    }
   break;
  // Calendar
  case EwinCalendar :
   castbStr(pdataSet,&sbAtr);
   if (sbAtr.size) winCalendarSetData(GTK_WIDGET(pObj),&sbAtr);
   break;
  default :
   return CERRTypeNo;
  } 
 return err;
 }
/*
 */
Terr getDataObj(GObject *pObj,EwinTypeObj typeObj,Tdata *pOut,Tarea *pArea){
 Terr err=COutKodOk;
 //
 if (winDb.Db.typeDb==EtypeNo) return CERRwinInitNo;
 if (pOut==NULL) return COutKodOk;
 //
 switch(typeObj){
  //Главное окно   
  case EwinWin :
   pOut->bstr.psec=winWinGetTitle((GtkWidget*)pObj);
   pOut->bstr.size=strlen(pOut->bstr.psec);
   pDataSetbStr(pOut);
   break;
  //Notebook
  case EwinNb :
//   pOut->bstr.psec=winWinGetTitle( GTK_WIDGET(pObj) );
//   pOut->bstr.size=strlen(pOut->bstr.psec);
//   pDataSetbStr(pOut);
   break;
  //Label 
  case EwinLb :
   pOut->bstr.psec=winLbGetTitle( GTK_WIDGET(pObj) );
   pOut->bstr.size=strlen(pOut->bstr.psec);
   pDataSetbStr(pOut);
   break;
  //кнопка 
  case EwinBut :
   pOut->bstr.psec=winButGetTitle( GTK_WIDGET(pObj) );
   pOut->bstr.size=strlen(pOut->bstr.psec);
   pDataSetbStr(pOut);
   break;
  // Grid
  case EwinGrid :
   winViewGridGet(GTK_TREE_VIEW(pObj),pArea);
   pOut->pstr=(Tstr*)(pArea->psec);
   pDataSetpStr(pOut);
   break;
// Tree 
  case EwinTree :
   break;
  // Calendar
  case EwinCalendar :
   winCalendarGetData(GTK_WIDGET(pObj),&(pOut->bstr));
   break;
  default :
   break;
  } 
 return err;
 }
