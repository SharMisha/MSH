/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstNbBut.c
 * Author: misha
 *
 * Created on 6 февраля 2019 г., 8:42
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
/*
 * Simple C Test Suite
 */
GtkWidget* winNbNew(GtkPositionType pos,gboolean show_tabs,gboolean scrollable);
gint winNbBnaddPg(GtkNotebook *pNb,GtkWidget *pPg,char *pslab,char *pimgfl);
void winRowToNblabel(GtkNotebook *pNb,Tbstr *pbstrBuf);

void test1() {
 GtkNotebook *pNb;
 GtkWidget *pWin;
 char pTitle[]="<Документы\tОтчеты\tКлассификаторы\tНастройки\t2016.12.12>";
 char *pT="title";
 Tbstr bTitle;
 //
 printf("tstwinNbBut test 1\n");
 bTitle.size=strlen(pTitle);
 bTitle.psec=pTitle;
 mshwinInitTopWin(pWin,"tstwinNbBut test",600,800,NULL, NULL);
 pNb=(GtkNotebook*)winNbNew(GTK_POS_TOP,TRUE,FALSE);
 //
 winNbBnaddPg(pNb,gtk_label_new("Pg0"),pT,"./img/mn_GL0.PNG");
 winNbBnaddPg(pNb,gtk_label_new("Pg1"),pT,"./img/mn_GL1.PNG");
 winNbBnaddPg(pNb,gtk_label_new("Pg2"),pT,"./img/mn_GL2.PNG");
 winNbBnaddPg(pNb,gtk_label_new("Pg3"),pT,"./img/mn_GL3.PNG");
 winNbBnaddPg(pNb,gtk_label_new("Pg4"),pT,NULL);
 winRowToNblabel(pNb,&bTitle);
 gtk_container_add((GtkContainer*)pWin,(GtkWidget*)pNb);
 mwinstartMain(pWin);
 }

void test2() {
 printf("tstwinNbBut test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstwinNbBut) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstwinNbBut\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstwinNbBut)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstwinNbBut) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstwinNbBut)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstwinNbBut) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
