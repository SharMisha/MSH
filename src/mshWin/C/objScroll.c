/* 
 * File:   mwinScroll.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 10:57
 */
#include <gtk/gtk.h>
#include "mwinCore.h"
#include "mwinWidget.h"
/*
 * Создать GtkWidget
 */
GObject* winmwinScrollNew(GtkPolicyType hscrollbar_policy,GtkPolicyType vscrollbar_policy){
 GtkWidget *pWidget;
 //
 pWidget=gtk_scrolled_window_new( NULL , NULL );
 gtk_scrolled_window_set_policy((GtkScrolledWindow*)pWidget,hscrollbar_policy,vscrollbar_policy);
 return (GObject*)pWidget;
 }
/*
 * Атрибуты GtkWidget
 */
/*
 * Упаковка потомков
 */
void winmwinScrollSetPolicy(GtkBox *pObj,GtkPolicyType hscrollbar_policy,GtkPolicyType vscrollbar_policy){
 gtk_scrolled_window_set_policy((GtkScrolledWindow*)pObj,hscrollbar_policy,vscrollbar_policy);
 }
//
void winmwinScrollGetPolicy(GtkBox *pObj,GtkPolicyType *phscrollbar_policy,GtkPolicyType *pvscrollbar_policy){
 gtk_scrolled_window_get_policy((GtkScrolledWindow*)pObj,phscrollbar_policy,pvscrollbar_policy);
 }
/**/
void winmwinScrollSetPlacement(GtkBox *pObj,GtkCornerType window_placement){
 gtk_scrolled_window_set_placement((GtkScrolledWindow*)pObj,window_placement);
 }
//
GtkCornerType winmwinScrollGetPlacement(GtkBox *pObj){
 return gtk_scrolled_window_get_placement((GtkScrolledWindow*)pObj);
 }


