/* 
 * File:   mwinTreeView.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 11:15
 */
#include <gtk/gtk.h>
#include "mwinCore.h"
#include "mwinWidget.h"
#include "mshMem.h"
#include "mshErr.h"
#include "mshSmb.h"
#include "mshData.h"
//
void selectRow(Tbstr *pbstrBuf,char **ppB,char **ppE);
/*
 * Создать GtkWidget
 */
//
GObject* winNewStore(int cols,int isTree){
 GType types[cols];
 int j;
 //
 for (j=0;j<cols;j++) types[j]=G_TYPE_STRING;
 if (isTree)
  return (GObject*)gtk_tree_store_newv(cols,types);
 return (GObject*)gtk_list_store_newv(cols,types);
 }
//
static GtkTreeView* gettreeView(GtkTreeModel *pmodel,gint cols){
 GtkTreeView *ptreeView;
 char ptitle[]="title ";
 int j;
 GtkCellRenderer *prenderer;
 GtkTreeViewColumn *pcolumn;
 //создаем таблицу
 ptreeView = (GtkTreeView*)gtk_tree_view_new_with_model(pmodel);
 if (ptreeView==NULL) return NULL;
 for (j=0;j<cols;++j){
  prenderer = gtk_cell_renderer_text_new();
  ptitle[5]=j+'0';
  pcolumn = gtk_tree_view_column_new_with_attributes(ptitle,prenderer,"text",j,NULL);
  gtk_tree_view_append_column(ptreeView, pcolumn);
  }
 return ptreeView;
 }
//
static GObject* winNewView(GtkTreeModel *pmodel,gint cols,int isTree){
 GtkTreeView *ptreeView;
// GtkTreeIter itCur;
 //находим модель
 if (pmodel==NULL) {
   pmodel=(GtkTreeModel*)winNewStore(cols,isTree);
   }
  else
   cols=gtk_tree_model_get_n_columns(pmodel);
 //создаем таблицу
 ptreeView = gettreeView(pmodel,cols);
 /*
 memNULLvoid(&itCur, sizeof(GtkTreeIter) );
 if (isTree){
  gtk_tree_store_append( (GtkTreeStore*)pmodel, &itCur,NULL);
  gtk_tree_store_set((GtkTreeStore*)pmodel,&itCur,0,".",-1);
  }
 else{
  gtk_list_store_append((GtkListStore*)pmodel, &itCur);
  gtk_list_store_set((GtkListStore*)pmodel,&itCur,0,".",-1);
  }
 */
 return (GObject*)ptreeView;
 }
//
GObject* winGridNew(GtkTreeModel *pmodel,int cols){
 return winNewView(pmodel,cols,0);   
 }
//
GObject* winTreeNew(GtkTreeModel *pmodel,int cols){
 return winNewView(pmodel,cols,1);
 }
/*
 */
gint winViewAppendColumn(GtkWidget *ptree_view){
 GtkTreeViewColumn *pcolumn;
 GtkCellRenderer *prenderer;
 //
 prenderer = gtk_cell_renderer_text_new();
 pcolumn = gtk_tree_view_column_new_with_attributes(NULL,prenderer,"text",NULL);
 return gtk_tree_view_append_column((GtkTreeView*)ptree_view, pcolumn);
 }
//
gint winViewRemoveColumn(GtkWidget *ptree_view,gint position){
 GtkTreeViewColumn *pcolumn;
 //
 pcolumn=gtk_tree_view_get_column((GtkTreeView*)ptree_view,position);
 return gtk_tree_view_remove_column((GtkTreeView*)ptree_view,pcolumn);
 }
//
gint winViewInsertColumn(GtkWidget *ptree_view,gint position){
 GtkTreeViewColumn *pcolumn;
 GtkCellRenderer *prenderer;
 //
 prenderer = gtk_cell_renderer_text_new();
 pcolumn = gtk_tree_view_column_new_with_attributes(NULL,prenderer,"text",NULL);
 return gtk_tree_view_insert_column((GtkTreeView*)ptree_view,pcolumn,position);
 }
/*
 */
//
#define CellsToHeder(j,pcolumn) *pC=Cvm0; pcolumn =gtk_tree_view_get_column(pView,j); gtk_tree_view_column_set_title(pcolumn,pB)

void winViewSetHeder(GtkTreeView *pView,Tbstr *pbstrBuf){
 GtkTreeViewColumn *pcolumn;
 gint j=0,n;
 char *pC,*pB,*pE;
 //Выделяем строку
 selectRow(pbstrBuf,&pB,&pE);
 //разнести строку в заголовок
 n=gtk_tree_view_get_n_columns(pView);
 for (pC=pB;(pC<pE)&&(j<n);){
  if (*pC==CrazCol) {
   CellsToHeder(j,pcolumn);
   trSmbNext(pC); 
   pB=pC;
   ++j;
   }
  else{
   trSmbNext(pC);
   }
  }
 if (pB<pC){ CellsToHeder(j,pcolumn); }
 }

void winViewGetColHeder(GtkTreeView *pView,char **ppcBuf,int col){
 GtkTreeViewColumn *pcolumn;
 //Выделяем строку
 pcolumn =gtk_tree_view_get_column(pView,col);
 *ppcBuf=gtk_tree_view_column_get_title(pcolumn);
 }
/*
 */
#define CellsToListStore(pStore,pitCur,pB,pC,j) *pC=Cvm0; gtk_list_store_set(pStore,pitCur,j,pB,-1)
/*
static void rowSetListStore(GtkListStore *pStore,GtkTreeIter *pitCur,char *pB,char *pE,int n){
 char *pC;
 int j=0;
 //
 for (pC=pB;(pC<pE)&&(j<n);){
  if (*pC==CrazCol) {
   CellsToListStore(pStore,pitCur,pB,pC,j);
   trSmbNext(pC);
   pB=pC;
   ++j;
   }
  else{
   trSmbNext(pC);
   }
  }
 if (pB<pC){  CellsToListStore(pStore,pitCur,pB,pC,j); }
 }
*/
/*
 */
//void winViewRowsListStoreAdd(GtkTreeView *pView,Tbstr *pbstrBuf)
void winViewRowsListStoreAdd(GtkListStore *pStore,Tbstr *pbstrBuf)
 {
 GtkTreeIter itCur;
 char *pC,*pB,*pE,p0[]=".";
 int n,j=0;
 //
 pB=pbstrBuf->psec;
 pE=pB+pbstrBuf->size;
 if (*(pE-1) == Cvm0) --pE;
 n=gtk_tree_model_get_n_columns((GtkTreeModel*)pStore);
 memNULLvoid(&itCur, sizeof(GtkTreeIter) );
 if ((pE-pB)<3) {
  gtk_list_store_append(pStore, &itCur);
  gtk_list_store_set(pStore,&itCur,0,p0,-1);
  return;
  }
 for (pC=pB;pC<pE;){
  switch(*pC){
   //начало уровня
   case CrazRow :
    if (j<n) {
     CellsToListStore(pStore,&itCur,pB,pC,j);
     }
   case CbegDataTree :
    trSmbNext(pC);
    pB=pC;
    j=0;
    break;
   case CendDataTree :
    CellsToListStore(pStore,&itCur,pB,pC,j);
    return;
   case CrazCol :
    if (j==0){
     gtk_list_store_append(pStore, &itCur);
     CellsToListStore(pStore,&itCur,pB,pC,j);
     }
    else
     if (j<n) {
      CellsToListStore(pStore,&itCur,pB,pC,j);
      }
    trSmbNext(pC);
    pB=pC;
    ++j;
    break;
   default:
    trSmbNext(pC);
    break;
   }
  }
 if (pC>pB) {
  if (j==0){
   gtk_list_store_append(pStore, &itCur);
   CellsToListStore(pStore,&itCur,pB,pC,j);
   }
  else
   if (j<n) {
    CellsToListStore(pStore,&itCur,pB,pC,j);
    }
  }
 }
/*
*/
//void winViewRowsListStore(GtkTreeView *pView,Tbstr *pbstrBuf)
void winViewRowsListStore(GtkListStore *pStore,Tbstr *pbstrBuf)
 {
 gtk_list_store_clear(pStore);
 winViewRowsListStoreAdd(pStore,pbstrBuf);
 }
/*
 * Заменить строку
 */
void winViewGridUpDateRow(GtkListStore *pStore,Tbstr *pbstrBuf,int row){
 GtkTreeIter itCur;
 char *pC,*pB,*pE;
 int n,j=0;
 //
 memNULLvoid(&itCur, sizeof(GtkTreeIter) );
 //
 if ( !gtk_tree_model_iter_nth_child( (GtkTreeModel*)pStore, &itCur,NULL, row) ) return;
 //
 pB=pbstrBuf->psec;
 pE=pB+pbstrBuf->size;
 if (*(pE-1) == Cvm0) --pE;
 n=gtk_tree_model_get_n_columns((GtkTreeModel*)pStore);
 for (pC=pB;pC<pE;trSmbNext(pC)){
  switch(*pC){
   //начало уровня
   case CbegDataTree :
    trSmbNext(pC);
//    smbSpaceSkip(pC,pE);
    pB=pC;
    break;
   case CrazRow :
   case CendDataTree :
    if (j<n) {
     CellsToListStore(pStore,&itCur,pB,pC,j);
     }
    return;
   case CrazCol :
    if (j<n) {
     CellsToListStore(pStore,&itCur,pB,pC,j);
     }
    trSmbNext(pC);
    pB=pC;
    ++j;
    break;
   }  
  }
 if ((pC>pB)&&(j<n)) {CellsToListStore(pStore,&itCur,pB,pC,j);}
 }
void winViewGridUpDateCol(GtkListStore *pStore,Tbstr *pbstrBuf,int row,int col){
 GtkTreeIter itCur;
 int n;
 char *pB,*pC;
 //
 memNULLvoid(&itCur, sizeof(GtkTreeIter) );
 //
 if ( !gtk_tree_model_iter_nth_child( (GtkTreeModel*)pStore, &itCur,NULL, row) ) return;
 n=gtk_tree_model_get_n_columns((GtkTreeModel*)pStore);
 if (col<n){
  pB=pbstrBuf->psec;
  pC=pB+pbstrBuf->size;
  *pC=Cvm0;
  gtk_list_store_set(pStore,&itCur,col,pB,-1);
  }
 }
/*
 * Получить поле таблицы
 */
void winViewGridGetCol(GtkTreeModel *pStore,char **ppcBuf,int row,int col){
 GtkTreeIter itCur;
 int n;
 //
 memNULLvoid(&itCur, sizeof(GtkTreeIter) );
 if ( gtk_tree_model_iter_nth_child( pStore, &itCur,NULL, row) ) {
  n=gtk_tree_model_get_n_columns(pStore);
  if (col<n){
   gtk_tree_model_get(pStore,&itCur,col,ppcBuf,-1);
   }
  }
 }
/*
 * Получить строку таблицы
 */
static  void gridGetRow(GtkTreeModel *pStore,GtkTreeIter *pitCur,char *pC,int n,Toff *pOff,Tarea *pArea){
 int j,w;
 char *pcBuf;
 Toff Off,Busy;
 //
 gtk_tree_model_get(pStore,pitCur,0,&pcBuf,-1);
 w=strlen(pcBuf);
 Busy=w+1;
 pareaBusy(pArea,Busy);
 memCopy(pC,pcBuf,w);
 Off=w;
 g_free(pcBuf);
 for (j=1;j<n;){
  gtk_tree_model_get(pStore,pitCur,j,&pcBuf,-1);
  pC[Off]=CrazCol;
  Off++;
  w=strlen(pcBuf);
  Busy=w+1;
  pareaBusy(pArea,Busy);
  memCopy(pC+Off,pcBuf,w);
  Off+=w;
  g_free(pcBuf);
  }
 *pOff += Off;
 }
void winViewGridGetRow(GtkTreeModel *pStore,Tarea *pArea,int row){
 GtkTreeIter itCur;
 int n;
 Toff Off;
 char *pC;
 Tstr *pStr;
 //
 pareaClear(pArea);
 //
 if ( gtk_tree_model_iter_nth_child( (GtkTreeModel*)pStore, &itCur,NULL, row) ) {
  n=gtk_tree_model_get_n_columns((GtkTreeModel*)pStore);
  if (n){
   pStr=(Tstr*)(pArea->psec);
   pC=pStr->psec;
   gridGetRow(pStore,&itCur,pC,n,&Off,pArea);
   pStr->size=Off;
   }
  }
 }
/*
 * Получить все строки таблицы
 */
void winViewGridGet(GtkTreeModel *pStore,Tarea *pArea){
 GtkTreeIter itCur;
 int n;
 Toff Off;
 char *pC;
 Tstr *pStr;
 //
 pareaClear(pArea);
 if (!gtk_tree_model_get_iter_first (pStore, &itCur)) return;
 //
 n=gtk_tree_model_get_n_columns(pStore);
 if (n<1) return;
 pStr=(Tstr*)(pArea->psec);
 pC=pStr->psec;
 do{
  gridGetRow(pStore,&itCur,pC,n,&Off,pArea);
  pC[Off]=CrazRow;
  Off++;
  pC +=Off;
  }while(gtk_tree_model_iter_next(pStore, &itCur));
 pStr->size=Off;
 }
/*
 */
#define CellsToTreeStore *pC=Cvm0; gtk_tree_store_set(pStore,pitCur,j,pB,-1)

static void rowAddTreeStore(GtkTreeStore *pStore,GtkTreeIter *pitCur,GtkTreeIter *pitParent,char *pB,char *pE,int n){
 char *pC;
 int j;
 //
 gtk_tree_store_append (pStore, pitCur,pitParent);
 for (pC=pB,j=0;(pC<pE)&&(j<n);){
  if (*pC==CrazCol) {
   CellsToTreeStore;
   trSmbNext(pC);
   pB=pC;
   ++j;
   }
  else{
   trSmbNext(pC);
   }
  }
 pC=pE;
 if ((pB<pC)&&(j<n)) { CellsToTreeStore; }
 }

void winViewRowAddTreeStore(GtkTreeStore *pStore,GtkTreeIter *pitCur,GtkTreeIter *pitParent,char *pB,char *pE){
 int n;
 //
 n=gtk_tree_model_get_n_columns((GtkTreeModel*)pStore);
 rowAddTreeStore(pStore,pitCur,pitParent,pB,pE,n);
 }
/*
 */
void winViewRowsTreeStoreAdd(GtkTreeStore *pStore,Tbstr *pbstrBuf){
 struct TarrpGtkTreeIter {_contArr(GtkTreeIter);} treeIter;
 GtkTreeIter *pitCur,*pitParent;
 char *pC,*pB,*pE;
 int n,j=0;
 //
 n=gtk_tree_model_get_n_columns((GtkTreeModel*)pStore);
 arrInit(treeIter,4);
 pB=pbstrBuf->psec;
 pE=pB+pbstrBuf->size;
 pitCur=pitParent=NULL;
 for (pC=pB;pC<pE;){
  switch(*pC){
   //начало уровня
   case CbegDataTree :
    //строку поместить в дерево pitCur
//    if (pC>pB) rowAddTreeStore(pStore,pitCur,pitParent,pB,pC,n);
    if ((pC>pB)&&(j<n)) {
     CellsToTreeStore;
     }
    //итератор опустить на один уровень
    pitParent=pitCur;
    arrBusyNull(treeIter);
    pitCur=treeIter.ptop;
    trSmbNext(pC);
    pB=pC;
    j=0;
    break;
//    smbSpaceSkip(pC,pE);
   case CrazRow :
    if ((pC>pB)&& (j<n)) { CellsToTreeStore; }
    trSmbNext(pC);
    pB=pC;
    j=0;
    break;
   case CendDataTree :
//    if (pC>pB) rowAddTreeStore(pStore,pitCur,pitParent,pB,pC,n);
    //последнее поле
    if ((pC>pB)&& (j<n)) {
     CellsToTreeStore;
     }
    //подняться
    arrPop(treeIter);
    pitCur=pitParent;
    if (pitCur==NULL) goto finish;
    pitParent=(treeIter.busy>1)?treeIter.ptop - 1:NULL;
    trSmbNext(pC);
//    smbSpaceSkip(pC,pE);
    pB=pC;
    j=0;
    break;
   case CrazCol :
    if (j==0){
     gtk_tree_store_append (pStore, pitCur,pitParent);
     CellsToTreeStore;
     }
    else
     if (j<n) { CellsToTreeStore; }
    trSmbNext(pC);
    pB=pC;
    ++j;
    break;
   default : 
    trSmbNext(pC);
    break;
   }  
  }
 //освободить рессурсы итераторов
finish: 
 arrFree(treeIter);
 }
void winViewRowsTreeStore(GtkTreeStore *pStore,Tbstr *pbstrBuf){
 gtk_tree_store_clear(pStore);
 winViewRowsTreeStoreAdd(pStore,pbstrBuf);
 }
/*
 * Найти выделенную строку
 */
char* selfocusCol(GtkTreeView *pView,int col){
 GtkTreeSelection *psel;
 GtkTreeIter itCur;
 GtkTreeModel *pStore;
 char *pcBuf;
 int n;
 //
 if (pView==NULL) return NULL;
 psel=gtk_tree_view_get_selection(pView);
 gtk_tree_selection_set_mode(psel, GTK_SELECTION_SINGLE);
 if (! gtk_tree_selection_count_selected_rows(psel) ) return NULL;
 gtk_tree_selection_get_selected(psel,&pStore,&itCur);
 n=gtk_tree_model_get_n_columns((GtkTreeModel*)pStore);
 if (col>=n) return NULL;
 gtk_tree_model_get(pStore,&itCur,col,&pcBuf,-1);
 return pcBuf;
 }
