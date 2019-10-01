/* 
 * File:   mwinWidget.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 03 февраля 2019 г., 11:14
 */
#include <gtk/gtk.h>
#include "mwinCore.h"
#include "mwinWidget.h"

extern TwinDb winDb;

gint eventDelete(GtkWidget *pwin,GdkEvent *pevn,gpointer pdata){
 return FALSE;
 }
gint eventDestroy(GtkWidget *pwin,GdkEvent *pevn,gpointer pdata){
 gtk_main_quit();
 return FALSE;
 }
/*
 * Главное окно приложения
 */
GObject* winWinNew(char *ptitle){
 GtkWidget *pwin;
 //
 gtk_init(NULL, NULL);
 pwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 if (ptitle!=NULL)
  gtk_window_set_title (GTK_WINDOW (pwin), ptitle);
 gtk_window_set_default_size (GTK_WINDOW (pwin), CwinwidthDef, CwinheightDef);
 g_signal_connect (pwin, "delete-event",G_CALLBACK(eventDelete), NULL);
 g_signal_connect (pwin, "destroy", G_CALLBACK(eventDestroy), NULL);
 return (GObject*)pwin;
 }
/*
 * Атрибуты окна
 */
void winWinSetTitle(GtkWidget *pObj,char *ptitle){
 gtk_window_set_title(GTK_WINDOW(pObj),ptitle);
 }
//
char* winWinGetTitle(GtkWidget *pObj){
 return (char*)gtk_window_get_title(GTK_WINDOW(pObj));   
 }
/**/
void winWinSetSize(GtkWidget *pObj,gint setWidth,gint setHeight){
 GtkWindow   *pWin;
 //
 pWin=GTK_WINDOW(pObj);
 gtk_window_set_default_size(pWin,setWidth,setHeight);
 }
//
void winWinGetSize(GtkWidget *pObj,gint *pwidth,gint *pheight){
 gtk_window_get_size(GTK_WINDOW(pObj),pwidth,pheight);
 }
//
void winWinGetPos(GtkWidget *pObj,gint *pX,gint *pY){
 gtk_window_get_position(GTK_WINDOW(pObj),pX,pY);
 }
/*
 */
void butCallBackOn(GtkWidget *pObj,EqType qpar);
Terr winFree();
gboolean readChannel(GIOChannel *pChannel, GIOCondition condition, gpointer pData);
int bufSend(Tdiv div,Tbstr *pbsBf);

Terr startWin(GtkWidget *pWin){
 Terr err;
 char cb[2];
 Tbstr bf;
 //программа предобработки окна
 //запускаем Главный цикл
 //канал связи с сервером открыт
 if (winDb.pIOChannel){
  //отправляем запрос инициализации на сервер
  cb[0]=CnetcomInit;
  cb[1]=Cvm0;
  bf.psec=cb;
  bf.size=1;
  err=bufSend(winDb.Clnt.sctrw, &bf);   
  //вызываем программу инициализации на клиенте
  butCallBackOn(pWin,qOnModLabInit);
  mwinstartMainChannel(pWin,winDb.pIOChannel,readChannel , &winDb);
//  g_io_add_watch(pIOChannel, G_IO_IN , readBufClnt , pData);
  }
 else {
//  mwinstartMain(pWidget);
//#define mwinstartMain(pwin) 
  gtk_widget_show_all(pWin);
  gtk_main();
  }
 (void)winFree();
 return err;
 }
