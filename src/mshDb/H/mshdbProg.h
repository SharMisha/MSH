/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mshdbProg.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 5 декабря 2018 г., 20:24
 */

#ifndef MSHDBPROG_H
#define MSHDBPROG_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "mshData.h"
    
void bStrGet(Tdata *pData,Tbstr* pbstrOut);

void castbStr(Tdata *pData,Tbstr *pbstrOut);
Tstr* castId(Tdata *pData);
Terr castNorm(Tdata *pData);
Terr castDataInt(Tdata *pData);
long long int castbStrToInt(Tbstr *pbStr);
Terr castDataArf(Tdata *pData);
Terr castDataFloat(Tdata *pData);
Terr castDataToStr(Tdata *pData);


int roundDataDbAlt(TDb *pDb, TarrData *pInds,int isAlt, int (*pfdata)(TtreeData *ptreeData) , void *pUser);

#ifdef __cplusplus
}
#endif

#endif /* MSHDBPROG_H */

