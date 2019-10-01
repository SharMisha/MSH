/* 
 * File:   mwinImg.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 12:51
 */
#include <gtk/gtk.h>
#include "mwinCore.h"
#include "mwinWidget.h"
/*
 * Создать GtkWidget
 */
GtkWidget* winImgNew(char *pfile){
 return gtk_image_new_from_file(pfile);
 }
/*
 * Атрибуты GtkWidget
 */


