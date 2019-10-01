/* 
 * File:   mwinNb.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 8:25
 */
#include <gtk/gtk.h>
#include "mwinCore.h"
#include "mwinWidget.h"
#include "mshMem.h"
#include "mshSmb.h"
#include "mshData.h"

//GQuark qinPg=1;
const gchar *pkey="Nb";

/*
 * Создать GtkWidget
 */
GObject* winNbNew(GtkPositionType pos,gboolean show_tabs,gboolean scrollable){
 GtkNotebook *pNb;
 //
 pNb=(GtkNotebook*)gtk_notebook_new();
 gtk_notebook_set_show_tabs(pNb,show_tabs);
 if (show_tabs) gtk_notebook_set_tab_pos(pNb,pos);
 gtk_notebook_set_scrollable(pNb,scrollable);
 return (GObject*)pNb;
 }
/*
 * Атрибуты  GtkWidget
 */
void winNbSetTitle(GtkWidget *pObj,gint nPg,char *ptitle){
 GtkWidget *pPg,*pTitle;
 const char *sType;
 GtkNotebook *pNb;
 //
 pNb=(GtkNotebook*)pObj;
 pPg=gtk_notebook_get_nth_page(pNb,nPg);
 pTitle=gtk_notebook_get_tab_label(pNb,pPg);
 sType=gtk_widget_get_name(pTitle);
 if (strlen(sType)>3){
  switch( sType[3] ){
   case 'B' :
    gtk_button_set_label( (GtkButton*)pTitle,ptitle);
    break;
   case 'L' :
    gtk_label_set_text((GtkLabel*)pTitle,ptitle);
    break;
   }
  }
 }
//
char* winNbGetTitle(GtkWidget *pObj,gint nPg){
 GtkWidget *pPg,*pTitle;
 GtkNotebook *pNb;
 //
 pNb=(GtkNotebook*)pObj;
 pPg=gtk_notebook_get_nth_page(pNb,nPg);
 pTitle=gtk_notebook_get_tab_label(pNb,pPg);
 if (GTK_IS_LABEL(pTitle))
  return (char*)gtk_label_get_text((GtkLabel*)pTitle);
 else
  if (GTK_IS_BUTTON(pTitle))
   return (char*)gtk_button_get_label( (GtkButton*)pTitle);
 return NULL;
 }

static void DestroymwinNb(gpointer ptr){memFree(ptr); }

Tdata* getParObj(GtkWidget *pObj,EqType qpar);

static void butNbClick(GtkButton *pBut,gpointer user_data){
 GtkNotebook *pNb;
 gulong inPg;
 Tdata *pdinPg;
 //
 pdinPg=getParObj((GtkWidget*)pBut,qinPg);
 if (pdinPg!=NULL){
  pNb=pdinPg->voidtype2.void1;
  inPg=pdinPg->voidtype2.type1;
  gtk_notebook_set_current_page(pNb,inPg);
  }
 }
//
void getdataNameObj(GtkWidget *pObj,Tdata *pdName);
Terr setPardataObj(Tdata *pdName,EqType qpar,Tdata *psetData);
//
gint winNbBnaddPg(GtkNotebook *pNb,GtkWidget *pPg,char *pslab,char *pimgfl){
 GtkWidget *pBut;
 gulong inPg;
 Tdata setData,dName;
 //
 if (pslab==NULL){
  if ( (pimgfl)&&(strlen(pimgfl)) )
   return gtk_notebook_append_page(pNb,pPg,gtk_image_new_from_file(pimgfl) );
  return gtk_notebook_append_page(pNb,pPg,NULL);
  }
 if ( (pimgfl==NULL)||(strlen(pimgfl)==0) )
  return gtk_notebook_append_page(pNb,pPg,gtk_label_new(pslab));
 //
 MnbBut(pBut,pslab,pimgfl)
 inPg=gtk_notebook_append_page(pNb,pPg,pBut);
 setData.voidtype2.void1=pNb;
 setData.voidtype2.type1=inPg;
 getdataNameObj((GtkWidget*)pNb,&dName);
 dName.str.psec[dName.str.size]='B';
 dName.str.size++;
 (void)setPardataObj(&dName,qinPg,&setData);
 g_signal_connect(pBut, "clicked", G_CALLBACK(butNbClick),pNb);
 return inPg;
 }
//
static void CellsToTitle(GtkNotebook *pNb,gint j,char *pC,char *pB){
 GtkWidget *pPg,*pTitle;
 //
 pPg=gtk_notebook_get_nth_page(pNb,j);
 pTitle=gtk_notebook_get_tab_label(pNb,pPg);
// sType=gtk_widget_get_name(pTitle);
 *pC=Cvm0;
 if (GTK_IS_LABEL(pTitle))
  gtk_label_set_text((GtkLabel*)pTitle,pB);
 else
  gtk_button_set_label( (GtkButton*)pTitle,pB);
 }
//
void selectRow(Tbstr *pbstrBuf,char **ppB,char **ppE);
//
void winRowToNblabel(GtkNotebook *pNb,Tbstr *pbstrBuf){
 gint j,n;
 char *pC,*pB,*pE;
 //Выделяем строку
 selectRow(pbstrBuf,&pB,&pE);
 n=gtk_notebook_get_n_pages(pNb);
 //разнести строку в заголовок
 for (pC=pB,j=0;(pC<pE)&&(j<n);trSmbNext(pC)){
  if (*pC==CrazCol) {
   CellsToTitle(pNb,j,pC,pB);
   trSmbNext(pC); 
   pB=pC;
   ++j;
   }
  }
 if (pB<pC) CellsToTitle(pNb,j,pC,pB);
 }
/**/
void winNbSetFocus(GtkNotebook *pObj,gint nPg){
 gtk_notebook_set_current_page ( pObj,nPg);
 }