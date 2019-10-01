/* 
 * File:   mwinBox.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 10:06
 */
#include <gtk/gtk.h>
#include "mwinCore.h"
#include "mwinWidget.h"
//
Terr setPardataObj(Tdata *pdName,EqType qpar,Tdata *psetData);
/*
 * Создать GtkWidget
 */
GtkWidget* winBoxNew(GtkOrientation  orientation,gint spacing){
 return gtk_box_new(orientation,spacing);
 }
/*
 * Атрибуты GtkWidget
 */
/*
 * Упаковка потомков
 */
void winBoxPack(GtkBox *pObj,GtkWidget *child,EwinPackChild typePack,gboolean expand,gboolean fill,guint padding){
 if (typePack == EwinPackCentr)
  gtk_box_set_center_widget(pObj,child);
 else
  if (typePack == EwinPackEnd)
   gtk_box_pack_end(pObj,child,expand,fill,padding );
  else
   gtk_box_pack_start(pObj,child,expand,fill,padding );
 }
/**/
void winBoxSetSpacing(GtkBox *pObj,gint spacing){
 gtk_box_set_spacing(pObj,spacing);
 }
gint winBoxGetSpacing(GtkBox *pObj){
 return gtk_box_get_spacing(pObj);
 }
/**/
void winBoxSetStyle(Tdata *pdnameObj,EwinPackChild typePack){
 Tdata setData;
 //
 DataSetInt(setData);
 setData.uint64[0]=typePack;
 (void)setPardataObj(pdnameObj,qPack,&setData);
 }

