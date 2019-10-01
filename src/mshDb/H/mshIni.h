
/* 
 * File:   mshIni.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 28 октября 2016 г., 16:50
 */

#ifndef MSHINI_H
#define MSHINI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mshData.h"
 /*
 * Ini файлы
 */
typedef struct {
 Tbstr key;
 Tbstr val;
 } TbKeyVal;

typedef struct {
 _contArr(TbKeyVal);
 Tbstr Buf;
 } TIni;
//
#define	iniInit(Ini)   arrInit(Ini,CvmasumIniMapDef);bstrInit((Ini).Buf,CvmasumIniBufDef)
#define	piniInit(pIni) parrInit(pIni,CvmasumIniMapDef);bstrInit((pIni)->Buf,CvmasumIniBufDef)
//
#define	iniFree(Ini) memFree((Ini).Buf.psec)
#define	piniFree(pIni) memFree((pIni)->Buf.psec); parrFree(pIni);
 

#ifdef __cplusplus
}
#endif

#endif /* MSHINI_H */

