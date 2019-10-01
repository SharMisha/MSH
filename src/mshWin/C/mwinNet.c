/* 
 * File:   mwinNet.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 19 февраля 2019 г., 9:52
 */
#include <gtk/gtk.h>
#include "mshErr.h"
#include "mshMem.h"
//
#include "mwinCore.h"
#include "mshData.h"
#include "mwinWidget.h"
//
extern TwinDb winDb;

/*
 */
Tdata* findatrObj(TDb *pDb,Tdata *pdataName);
void winViewRowsListStore(GtkListStore *pListStore,Tbstr *pbstrBuf);
void winViewRowsTreeStore(GtkTreeStore *pStore,Tbstr *pbstrBuf);
int bufSend(Tdiv div,Tbstr *pbsBf);
int bufReadMsg(Tdiv div,Tbuf *pBf);
//
void winWinSetTitle(GtkWidget *pObj,char *ptitle);
void winRowToNblabel(GtkNotebook *pNb,Tbstr *pbstrBuf);
void winLbSetTitle(GtkWidget *pObj,char *ptitle);
void winButSetTitle(GtkWidget *pObj,char *plabel);
void winCalendarSetData(GtkWidget *pObj,Tbstr *psbf);
void winViewGridGet(GtkTreeView *pView,Tarea *pArea);
char* winWinGetTitle(GtkWidget *pObj);
char* winLbGetTitle(GtkWidget *pObj);
char* winButGetTitle(GtkWidget *pObj);
void winCalendarGetData(GtkWidget *pObj,Tbstr *psbf);
//
int mClntUsrDef(TClnt *pClnt){
 char *pC;
 Tbstr bf;
 Terr err;
 Tdata dataName,*pdObj;
 GObject *pObj;
 TwinDb *pwinDb;
 EwinTypeObj typeObj;
 Tstr *pStr;
 GtkListStore *pListStore;
 //
 if (pClnt->buf.busy<2) return CWARdataNotFound;
 //найти имя объекта
 bf.psec=pClnt->buf.psec+1;
 bf.size=pClnt->buf.busy-1;
 dataName.bstr=bf;
 pwinDb=(TwinDb*)(pClnt->pData);
 pdObj=findatrObj(&(pwinDb->Db),&dataName);
 if (pdObj==NULL) return CERRwinObjNotFind;
 pObj=pdObj->voidtype2.void1;
 if (pObj==NULL) return CERRwinObjNotFind;
 typeObj=(EwinTypeObj)(pdObj->voidtype2.type1);
 //Объект найден
 pC=memFindChar(bf.psec,CsysfrRou,bf.size);
 if (pC) bf.size=bf.psec-pC;
 if (bf.size<=0) return  CWARdataNotFound;
 //
 switch( pClnt->buf.psec[0] ){
  //получить с сервера данные
  case CnetcomSet :
   if (pC){
    bf.size = bf.psec +  bf.size - pC-1;
    bf.psec=pC+1;
    }
   else{
    bf.psec=bf.psec +  bf.size;
    bf.size =0;
    }
   if (bf.size<1) return err;
   //
   switch(typeObj){
    case EwinGrid :
     pListStore=GTK_LIST_STORE( gtk_tree_view_get_model(GTK_TREE_VIEW(pObj)) );
     winViewRowsListStore(pListStore,&bf);
     break;
    case EwinTree :
     winViewRowsTreeStore(GTK_TREE_STORE( gtk_tree_view_get_model(GTK_TREE_VIEW(pObj)) ),&bf);
     break;
    case EwinWin :
     winWinSetTitle((GtkWidget*)pObj,bf.psec);
     break;
    case EwinNb :
     winRowToNblabel(GTK_NOTEBOOK(pObj),&bf);
     break;
    case EwinLb :
     winLbSetTitle(GTK_WIDGET(pObj),bf.psec);
     break;
    case EwinBut :
     winButSetTitle(GTK_WIDGET(pObj),bf.psec);
     break;
    case EwinCalendar :
     winCalendarSetData(GTK_WIDGET(pObj),&bf);
     break;
    default:
     break;
    }
   break;
  case CnetcomGet :
   switch(typeObj){
    case EwinGrid :
     winViewGridGet(GTK_TREE_VIEW(pObj),(Tarea*)(&(pClnt->buf)));
     pStr=(Tstr*)(pClnt->buf.psec);
     pStrTobStr(pStr,bf);
     break;
    case EwinTree :
     break;
    case EwinWin :
     bf.psec=winWinGetTitle((GtkWidget*)pObj);
     bf.size=strlen(bf.psec);
     break;
    case EwinNb :
     winRowToNblabel(GTK_NOTEBOOK(pObj),&bf);
     break;
    case EwinLb :
     bf.psec=winLbGetTitle( GTK_WIDGET(pObj) );
     bf.size=strlen(bf.psec);
     break;
    case EwinBut :
     bf.psec=winButGetTitle( GTK_WIDGET(pObj) );
     bf.size=strlen(bf.psec);
     break;
    case EwinCalendar :
     winCalendarGetData(GTK_WIDGET(pObj),&bf);
     break;
    default:
     break;
    }
   //отправить сообщение на сервер
   areaClear(pClnt->buf);
   areaCharBusy(pClnt->buf,bf.size+2);
   pClnt->buf.psec[0]=CnetcomGet;
   pClnt->buf.psec[bf.size+1]=Cvm0;
   memMove(pClnt->buf.psec+1,bf.psec,bf.size);
   bf.psec=pClnt->buf.psec;
   bf.size++;
   err=bufSend(winDb.Clnt.sctrw, &bf );
   break;
  }
 return err;
 }
/*
 * Обработка сообщений сервера
 */
Terr setDataObj(GObject *pObj,EwinTypeObj typeObj,Tdata *pdataSet);
Terr getDataObj(GObject *pObj,EwinTypeObj typeObj,Tdata *pOut,Tarea *pArea);
//GIOStatus bufReadChannelMsg(GIOChannel *pChannel,Tbuf *pBf);

gboolean readChannel(GIOChannel *pChannel, GIOCondition condition, gpointer pData){
 Terr err;
// GIOStatus st;
 Tbstr bObj,bIn;
 char *pC;
 Tdata dataName,*pdObj;
 GObject *pObj;
 EwinTypeObj typeObj;
 Tdiv div;
 //
 div=g_io_channel_unix_get_fd (pChannel);
 err=bufReadMsg(div,&(winDb.Clnt.buf));
 if (err!=COutKodOk) return FALSE;
// st=bufReadChannelMsg(pChannel,&(pwinDb->Clnt.buf)); if (st==G_IO_STATUS_EOF) return FALSE;
 //
 if (winDb.Clnt.buf.busy<2) return TRUE;
 bObj.size=winDb.Clnt.buf.busy-1;
 bObj.psec=winDb.Clnt.buf.psec+1;
 bIn.size=0;
 bIn.psec=NULL;
 pC=memFindChar(bObj.psec,CsysfrRou,bObj.size);
 //ищем объект
 if (pC) {
  bIn.psec=pC+1;
  bIn.size=bObj.psec+bObj.size-bIn.psec;
  bObj.size -=bIn.size+1;
  }
 if (bObj.size<1) return TRUE;
 if (bObj.size>CCsizeStr) bObj.size=CCsizeStr;
 dataName.bstr=bObj;
 DataSetbStr(dataName);
 pdObj=findatrObj(&(winDb.Db),&dataName);
 if (pdObj==NULL) return CERRwinObjNotFind;
 pObj=pdObj->voidtype2.void1;
 if (pObj==NULL) return CERRwinObjNotFind;
 typeObj=(EwinTypeObj)(pdObj->voidtype2.type1);
 dataName.bstr=bIn;
 switch( winDb.Clnt.buf.psec[0] ){
  case CnetcomSet :
   err=setDataObj(pObj,typeObj,&dataName);
   break;
  case CnetcomGet :
   err=getDataObj(pObj,typeObj,&dataName,&(winDb.Area));
   break;
  case CnetcomFocusSet :
   break;
  case CnetcomFocusGet :
   break;
  }
 return TRUE;
 }
/*
 */
