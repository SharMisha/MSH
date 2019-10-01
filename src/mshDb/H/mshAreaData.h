
/* 
 * File:   mshAreaData.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 23 ноября 2018 г., 13:18
 */

#ifndef MSHAREADATA_H
#define MSHAREADATA_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "mshArf.h"
 
#define _contbuf _contArea(char)

typedef struct {_contArr(void*);} TarrpVoid;
typedef struct {_contArea(Tsize);} Tarea;
typedef struct {_contArea(char);} TareaChar;
typedef struct { _contArr(int); } TarrInt;


#ifdef __cplusplus
}
#endif

#endif /* MSHAREADATA_H */

