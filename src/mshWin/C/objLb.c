/* 
 * File:   mwinLb.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 9:54
 */
#include <gtk/gtk.h>
#include "mwinCore.h"
#include "mwinWidget.h"
/*
 * Создать GtkWidget
 */
GtkWidget* winLbNew(char *ptitle){
 return gtk_label_new(ptitle);
 }
/*
 * Атрибуты GtkWidget
 */
void winLbSetTitle(GtkWidget *pObj,char *ptitle){
 gtk_label_set_text(GTK_LABEL(pObj),ptitle);
 }
//
char* winLbGetTitle(GtkWidget *pObj){
 return (char*)gtk_label_get_text(GTK_LABEL(pObj));
 }

