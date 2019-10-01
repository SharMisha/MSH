/* 
 * File:   mwinCalendar.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 12:57
 */
#include <gtk/gtk.h>
#include "mwinCore.h"
#include "mwinWidget.h"
#include "mshData.h"
/*
 * Создать GtkWidget
 */
//
GtkWidget* winCalendarNew(){
 GtkWidget *pWidget;
 //
 pWidget=gtk_calendar_new();
 return pWidget;
 }
/*
 * Атрибуты GtkWidget
 */
void winCalendarSetData(GtkWidget *pObj,Tbstr *psbf){
 guint month, year,day ;
 //
 if (psbf->size==10){
  year=(psbf->psec[0]-'0') * 1000 + ( (psbf->psec[1]-'0') * 100)+ ( (psbf->psec[2]-'0') * 10)+ (psbf->psec[3]-'0');
  month=( (psbf->psec[5]-'0') * 10)+ (psbf->psec[6]-'0')-1;
  day=( (psbf->psec[8]-'0') * 10)+ (psbf->psec[9]-'0');
  gtk_calendar_select_month(GTK_CALENDAR(pObj),month,year);
  gtk_calendar_select_day(GTK_CALENDAR(pObj),day);
  }
 }
void winCalendarGetData(GtkWidget *pObj,Tstr *pstr){
 guint month, year,day ;
 //
 gtk_calendar_get_date((GtkCalendar*)pObj, &year,&month,&day);
 snprintf(pstr->psec,11,"%04u.%02u.%02u",year,month+1,day);
 pstr->size=10;
 }