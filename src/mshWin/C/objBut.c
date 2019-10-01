/* 
 * File:   mwinBut.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 10:02
 */
#include <gtk/gtk.h>
#include "mshErr.h"
#include "mshData.h"
#include "mwinWidget.h"
//
#include "mwinCore.h"
//
TDb _DbwinD;
//
//GQuark qMod=4;
//GQuark qLabOff=5;
//
Terr vmwingetModLabel(Tstr *psMod,Tstr *psLabel,void *pUser,Tdata *pdataModLabel);
/**/
/*
 * Создать GtkWidget
 */
GtkWidget* winButNew(char *plabel,char *pfile,GtkPositionType position){
 GtkWidget *pWidget;
 //
 pWidget=gtk_button_new();
 if ((plabel)&&(strlen(plabel)>0)) {
  gtk_button_set_label((GtkButton*)pWidget,plabel);
  }
 if ((pfile)&&(strlen(pfile)>0)) {
  gtk_button_set_image((GtkButton*)pWidget,gtk_image_new_from_file(pfile));
  gtk_button_set_always_show_image((GtkButton*)pWidget,TRUE);
  gtk_button_set_image_position((GtkButton*)pWidget,position);
  }
 return pWidget;
 }
/*
 * Атрибуты GtkWidget
 */
void winButSetTitle(GtkWidget *pObj,char *plabel){
 if ((plabel)&&(strlen(plabel)>0)) {
  gtk_button_set_label(GTK_BUTTON(pObj),plabel);
  }
 }
char* winButGetTitle(GtkWidget *pObj){
 return gtk_button_get_label(GTK_BUTTON(pObj));
 }
void winButSetImage(GtkWidget *pObj,char *pfile){
 if ((pfile)&&(strlen(pfile)>0)) {
  gtk_button_set_image(GTK_BUTTON(pObj),gtk_image_new_from_file(pfile));
  gtk_button_set_always_show_image(GTK_BUTTON(pObj),TRUE);
  }
 }
//
GtkWidget* winButGetImage(GtkWidget *pObj){
 return gtk_button_get_image(GTK_BUTTON(pObj));
 }
//
void winButSetPosition(GtkWidget *pObj,GtkPositionType position){
 gtk_button_set_image_position(GTK_BUTTON(pObj),position);
 }
//
GtkPositionType winButGetPosition(GtkWidget *pObj){
 return gtk_button_get_image_position(GTK_BUTTON(pObj));
 }
/*
 * Назначение события для кнопки
 */

