/*
 * File:   mshVmLock.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:48
 */
#include "mshvmVm.h"
#include "mshvmMacro.h"
//
/*
 * Блокировки
 */
static void ctrlLockInit(TctrlLock *pLock,Tstr *pnameIn){
 memNULL(pLock);
 pLock->ijob=COutFound;
 pLock->name=*pnameIn;
 pLockInit(&(pLock->lock));
 }
#define ctrlLockFree(pLock) pLockFree(&(pLock->lock))
/*
 * Разблокировать пользователя
 */
void LockUn(Tstr *pnameIn){
 TctrlLock *plock;
 //в таблице есть имена
 if (App.Locks.busy) {
  spinLock(App.Locks.lock);
  RegIter(App.Locks,plock){
   //имя найдено в таблице
   if ((plock->ijob>=0)&&(plock->name.size)&&(StrpEqu(plock->name,pnameIn)==CMPeq)) {
    plock->ijob=COutFound;
    plock->name.size=0;
    unpLock(&(plock->lock));
    break;
    }
   }
  spinUnLock(App.Locks.lock);
  }
 }
/*
 * получить Блокировку пользователя
 */
void LockWR(TvmDo *pDo,Tstr *pnameIn,char type,int time){
 TctrlLock *plock,*plfree;
 struct timespec tm; 
 //
 plfree=NULL;
 if (App.Locks.busy) {
  spinLock(App.Locks.lock);
  RegIter(App.Locks,plock){
   //имя найдено в таблице
   if (plock->ijob >= 0) {
    if ((plock->name.size)&&(StrpEqu(plock->name,pnameIn)==CMPeq)) {
     if ( pAppgetpJob(plock->ijob) ==(pDo->pJob) ) {pDo->err=CERRvmReLock;goto err;}
     goto finish;
     }
    }
   else
    plfree=plock;
   }
  }
 //имя не найдено добавить имя
 if (plfree){
  plock=plfree;
  plock->name=*pnameIn;
  }
 else{
  pRegBusy(&(App.Locks));
  if ((plock=App.Locks.ptop)==NULL) { pDo->err=CERRvmNotCreateLock; goto err; }
  ctrlLockInit(plock,pnameIn);
  }
 plock->ijob=IdJobGetDo(pDo);
 // блокировка
finish:
  spinUnLock(App.Locks.lock);
 if (time==-1){
  switch (type) {
   case CvmSubCmdLockR :
    pLockR(&(plock->lock));
    break;
   case CvmSubCmdLockW :
    pLockW(&(plock->lock));
    break;
   }
  }
 else{
  tm.tv_sec=0;
  tm.tv_nsec=time*1000;
  switch (type) {
   case CvmSubCmdLockR :
    pLockRtime(&(plock->lock),&tm);
    break;
   case CvmSubCmdLockW :
    pLockWtime(&(plock->lock),&tm);
    break;
   }
  }
 return;
err: 
 spinUnLock(App.Locks.lock);
 }
/*
 * Статус блокировки
 */
int LockStatus(TvmDo *pDo,Tstr *pnameIn){
 TctrlLock *plock;
 int out=COutFound;
 //в таблице есть имена
 if (App.Locks.busy) {
  spinLock(App.Locks.lock);
  RegIter(App.Locks,plock){
   //имя найдено в таблице
   if ((plock->ijob>=0)&&(plock->name.size)&&(StrpEqu(plock->name,pnameIn)==CMPeq)) {
    if ( pAppgetpJob(plock->ijob) ==(pDo->pJob) ) {
     out=0;
     break;
     }
    out=plock->ijob;
    break;
    }
   }
  spinUnLock(App.Locks.lock);
  }
 return out;
 }
