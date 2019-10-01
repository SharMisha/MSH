/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstGridBuf.c
 * Author: misha
 *
 * Created on 17 марта 2019 г., 18:36
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "mshData.h"
#include "mwinCore.h"
#include "mwinWidget.h"
/*
 * Simple C Test Suite
 */
gint eventDelete(GtkWidget *pwin,GdkEvent *pevn,gpointer pdata);
gint eventDestroy(GtkWidget *pwin,GdkEvent *pevn,gpointer pdata);
//
GObject* winWinNew(char *ptitle);
GtkWidget* winGridNew(GtkTreeModel *pmodel,int cols);
void winViewSetHeder(GtkTreeView *pView,Tbstr *pbstrBuf);
void winViewRowsListStore(GtkListStore *pStore,Tbstr *pbstrBuf);
GtkWidget* winBoxNew(GtkOrientation  orientation,gint spacing);
void winBoxPack(GtkBox *pObj,GtkWidget *child,EwinPackChild typePack,gboolean expand,gboolean fill,guint padding);
GObject* winNbNew(GtkPositionType pos,gboolean show_tabs,gboolean scrollable);
gint winNbBnaddPg(GtkNotebook *pNb,GtkWidget *pPg,char *pslab,char *pimgfl);
/*
 * Simple C Test Suite
 */

void test1(){
 printf("mshwinGridBuf test 1\n");
 GtkWidget *pwin,*pnb;
 GtkTreeView *pTree;
 GtkBox *pBox;
 gint cols=3;
 Tbstr bBuf,bHeder;
 GtkListStore *pListStore;
 gchar lab[]="<Заголовок1\tЗаголовок2\tЗаголовок3>";
//
// gchar pBuf[]="<[0],0\t[0],1\t[0],2<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2>[1],0\t[1],1\t[1],2<[1,0],0\t[1,0],1\t[1,0],2\n[1,1],0\t[1,1],1\t[1,1],2>>";
// gchar pBuf[]="<[0],0\t[0],1\t[0],2<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2>[1],0\t[1],1<[1,0],0\t[1,0],1\t[1,0],2\n[1,1],0\t[1,1],1\t[1,1],2>>";
// gchar pBuf[]="<[0],0\t[0],1\t[0],2<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2>[1],0\t[1],1\t[1],2\n[2],0\t[2],1\t[2],2\n[5],0\t[5],1<[5,0],0\t[5,0],1\t[5,0],2\n[5,1],0\t[5,1],1\t[5,1],2>>";
// gchar pBuf[]="<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2\n[0,2],0\t[0,2],1\t[0,2],2>";
 gchar pBuf[]="[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2\n[0,2],0\t[0,2],1\t[0,2],2";
// gchar pBuf[]="<<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2>[1],0\t[1],1\t[1],2<[1,0],0\t[1,0],1\t[1,0],2\n[1,1],0\t[1,1],1\t[1,1],2>>";
 //
 pwin= GTK_WINDOW (winWinNew("mshwinGridBuf"));
 pnb=winNbNew(GTK_POS_TOP,FALSE,TRUE);
 gtk_container_add(GTK_CONTAINER(pwin),pnb );
 pBox=(GtkBox*)winBoxNew(GTK_ORIENTATION_VERTICAL,4);
 (void)winNbBnaddPg(GTK_NOTEBOOK(pnb),(GtkWidget*)pBox,NULL,NULL);
 pTree=(GtkTreeView*)winGridNew(NULL,cols);
 winBoxPack(pBox,(GtkWidget*)pTree,EwinPackStart,FALSE,FALSE,4);
 bHeder.psec=lab;
 bHeder.size=strlen(lab);
 winViewSetHeder(pTree,&bHeder);
 bBuf.psec=pBuf;
 bBuf.size=strlen(pBuf);
 pListStore=GTK_LIST_STORE( gtk_tree_view_get_model(pTree) );
 winViewRowsListStore(pListStore,&bBuf);
//
 mwinstartMain(pwin);
 }

void test2() {
 printf("tstwinGridBuf test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstwinGridBuf) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstwinGridBuf\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstwinGridBuf)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstwinGridBuf) \n");
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
