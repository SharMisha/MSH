
/* 
 * File:   netBuf.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 26 октября 2016 г., 10:37
 */
#include <pthread.h>

#include "Net.h"
/*
 * Инициализация массива буферов
 */
void secsBufInit(TsecsBuf *psecsBuf){
 pRegInit(psecsBuf,CsizesecsBufDef);
 spinLockInit(psecsBuf->lock);
 }
/* освободить буфера */
void secsBufFree(TsecsBuf *psecsBuf){
 pRegFree(psecsBuf);
 spinLockFree(psecsBuf->lock);
 }
/* очистить буфера */
void secsBufClear(TsecsBuf *psecsBuf){
 TsrvBuf *pBuf;
 //
 pRegIter(psecsBuf,pBuf) {
  pBufClear(pBuf);
  objSetFree(pBuf);
  }
 }
/*
 * получить пустой буфер
 */
TsrvBuf* bufGet(TsecsBuf *psecsBuf){
 TsrvBuf* pBuf;
 //
 spinLock(psecsBuf->lock);
 objGetpSecs(psecsBuf,pBuf);
 if (pBuf==NULL){
  pRegBusy( psecsBuf );
  pBuf=psecsBuf->ptop;
  pBufInit(pBuf);
  }
 else {pBufClear(pBuf);}
 objSetBusy(pBuf);
 spinUnLock(psecsBuf->lock);
 return pBuf;
 }
