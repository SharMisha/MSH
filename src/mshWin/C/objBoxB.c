/* 
 * File:   mwinBoxB.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 10:44
 */
#include <gtk/gtk.h>
#include "mwinCore.h"
#include "mwinWidget.h"
/*
 * Создать GtkWidget
 */

GtkWidget* winBoxBNew(GtkOrientation  orientation,GtkButtonBoxStyle layout_style){
 GtkWidget *pWidget;
 //
 pWidget=gtk_button_box_new(orientation);
 gtk_button_box_set_layout((GtkButtonBox*)pWidget,layout_style);
 return pWidget;
 }
/*
 * Атрибуты GtkWidget
 */
/*
 * Упаковка потомков
 */
void winBoxBSetLayout(GtkBox *pObj,GtkButtonBoxStyle layout_style){
 gtk_button_box_set_layout((GtkButtonBox*)pObj,layout_style);
 }
//
GtkButtonBoxStyle winBoxBGetLayout(GtkBox *pObj){
 return gtk_button_box_get_layout((GtkButtonBox*)pObj);
 }
