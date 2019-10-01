/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstGridBuf.c
 * Author: misha
 *
 * Created on 6 февраля 2019 г., 19:15
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "mshData.h"
#include "mwinWidget.h"
/*
 * Simple C Test Suite
 */
gint eventDelete(GtkWidget *pwin,GdkEvent *pevn,gpointer pdata);
gint eventDestroy(GtkWidget *pwin,GdkEvent *pevn,gpointer pdata);
//
GtkWidget* winGridNew(GtkTreeModel *pmodel,int cols);
void winViewSetHeder(GtkTreeView *pView,Tbstr *pbstrBuf);
void winViewRowsListStore(GtkListStore *pListStore,Tbstr *pbstrBuf);
/*
 * Simple C Test Suite
 */

void test1(){
 printf("mshwinGridBuf test 1\n");
 GtkWidget *pwin;
 GtkWidget *pTree;
 GtkListStore *pListStore;
// gchar *plab[]={"Заголовок1","Заголовок2","Заголовок3"};
 gchar lab[]="<Заголовок1\tЗаголовок2\tЗаголовок3>";
 gint cols=3;
 Tbstr bBuf,bHeder;
//
// gchar pBuf[]="<[0],0\t[0],1\t[0],2<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2>[1],0\t[1],1\t[1],2<[1,0],0\t[1,0],1\t[1,0],2\n[1,1],0\t[1,1],1\t[1,1],2>>";
// gchar pBuf[]="<[0],0\t[0],1\t[0],2<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2>[1],0\t[1],1<[1,0],0\t[1,0],1\t[1,0],2\n[1,1],0\t[1,1],1\t[1,1],2>>";
// gchar pBuf[]="<[0],0\t[0],1\t[0],2<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2>[1],0\t[1],1\t[1],2\n[2],0\t[2],1\t[2],2\n[5],0\t[5],1<[5,0],0\t[5,0],1\t[5,0],2\n[5,1],0\t[5,1],1\t[5,1],2>>";
// gchar pBuf[]="<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2\n[0,2],0\t[0,2],1\t[0,2],2>";
 gchar pBuf[]="[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2\n[0,2],0\t[0,2],1\t[0,2],2";
// gchar pBuf[]="<<[0,0],0\t[0,0],1\t[0,0],2\n[0,1],0\t[0,1],1\t[0,1],2>[1],0\t[1],1\t[1],2<[1,0],0\t[1,0],1\t[1,0],2\n[1,1],0\t[1,1],1\t[1,1],2>>";
 //
 pwin= GTK_WINDOW (winWinNew("mshwinGridBuf"));
 bHeder.psec=lab;
 bHeder.size=strlen(lab);
 pTree=winGridNew(NULL,cols);
 winViewSetHeder((GtkTreeView*)pTree,&bHeder);
 bBuf.psec=pBuf;
 bBuf.size=strlen(pBuf);
 pListStore=GTK_LIST_STORE( gtk_tree_view_get_model(pTree) );
 winViewRowsListStore((GtkTreeView*)pTree,&bBuf);
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
