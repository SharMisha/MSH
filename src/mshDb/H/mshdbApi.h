/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mshdbApi.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 5 декабря 2018 г., 14:30
 */

#ifndef MSHDBAPI_H
#define MSHDBAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mshDb.h"

Tval* mshGet(TDb *pDb,TarrData *pInds,Terr *perr);
Terr mshStat(TDb *pDb,TarrData *pInds,Tdata *pDataOut);
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
Terr mshSetArr(TDb *pDb,Toff Off,Tdata *pSetData);
Terr mshKill(TDb *pDb,TarrData *pInds,char tpK);
Terr mshKillDbDALL(TDb *pDb);
Terr mshNextInit(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey);
Terr mshNext(TIterNext *pIterNext);
Terr mshBack(TIterNext *pIterNext);
Terr mshNext1(TIterNext *pIterNext,TarrData *pInds,Tdata *pKey);
Terr mshQuery(TIterQuery *pQuery);
Terr mshQuery1(TIterQuery *pQuery);
Tdata* mshGetArr(TDb *pDb,Tusize Off,Terr *perr);
Terr mshStatArr(TDb *pDb,Tusize Off,Tdata *pStatOut);
Terr mshSetArr(TDb *pDb,Toff Off,Tdata *pSetData);
Terr mshPushArr(TDb *pDb,Tdata *pSetData);
Terr mshKillArr(TDb *pDb,Tusize Off);
void mshKillArrALL(TDb *pDb);
Terr mshArrNextBack(TIterNext *pNext);
Terr mshArrNextBack1(TIterNext *pNext);

#ifdef __cplusplus
}
#endif

#endif /* MSHDBAPI_H */

