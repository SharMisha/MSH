
/* 
 * File:   mshMonC.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 13 октября 2016 г., 10:18
 */

#ifndef MSHMON_H
#define MSHMON_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <gtk/gtk.h>
#include "mwinWidget.h"
#include "mshvmCnst.h"
    
#define CsumPage 2

typedef struct {
 GtkNotebook *pNb;
 GtkTreeView *pPage[CsumPage];
 GtkListStore *pStore[CsumPage];
 GtkLabel *plbIdSrv;
 GtkLabel *plbNameSrv;
 TClnt Clnt;
 GIOChannel *pIOChannel;
 TIni iniTxt;
 int numPg;
 } Tguide;

Tguide Guide;
 
#ifdef __cplusplus
}
#endif

#endif /* MSHMON_H */

