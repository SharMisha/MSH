
/* 
 * File:   mshTask.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 2 ноября 2016 г., 15:15
 */

#ifndef MSHTASK_H
#define MSHTASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../mshDb/H/mshHsys.h"
/*
 * Блокировки
 */
typedef pthread_rwlock_t TLock;
typedef pthread_spinlock_t TspinLock;
//
#define _contWait  pthread_mutex_t lock; pthread_cond_t wait

typedef struct { _contWait; } TWait;

//mutex блокировки
#define	pLockInit(pLock) pthread_rwlock_init(pLock,NULL)
#define	pLockFree(pLock) if (pLock) (void)pthread_rwlock_destroy (pLock)
//
#define	pLockR(pLock) pthread_rwlock_rdlock(pLock)
#define	pLockW(pLock) pthread_rwlock_wrlock(pLock)
#define	pLockRtime(pLock,time) pthread_rwlock_timedrdlock(pLock,time)
#define	pLockWtime(pLock,time) pthread_rwlock_timedwrlock(pLock,time)
#define	unpLock(pLock) pthread_rwlock_unlock(pLock)
/*
#define	LockROff(Locks,Off) if (Off<Locks.busy) pLockR((TLock*)arrOffToVal(Locks,Off))
#define	LockWOff(Locks,Off) if (Off<Locks.busy) pLockW((TLock*)arrOffToVal(Locks,Off))
#define	UnLockOff(Locks,Off) if (Off<Locks.busy) unpLock((TLock*)arrOffToVal(Locks,Off))
*/
//spin блокировки
#define	spinLockInit(Lock) pthread_spin_init(&(Lock),PTHREAD_PROCESS_PRIVATE)
#define	spinLockFree(Lock) (void)pthread_spin_destroy(&(Lock))
#define	spinLock(Lock) pthread_spin_lock(&(Lock))
#define	spinUnLock(Lock) pthread_spin_unlock(&(Lock))
//cond блокировки
#define	condLockInit(Lock) pthread_cond_init(&(Lock),NULL)
#define	condLockFree(Lock) pthread_cond_destroy(&(Lock))
// mutex
#define waitInit(pWait) pthread_mutex_init(&((pWait)->lock),NULL); pthread_cond_init(&((pWait)->wait),NULL)
#define waitFree(pWait) pthread_mutex_destroy(&((pWait)->lock)); pthread_cond_destroy(&((pWait)->wait))
#define waitLock(pWait)  pthread_mutex_lock(&((pWait)->lock))
#define waitUnLock(pWait)  pthread_mutex_unlock(&((pWait)->lock))
 
#define waitWait(pWait) waitLock(pWait);\
 pthread_cond_wait(&((pWait)->wait),&((pWait)->lock))
#define waitWaitTime(pWait,pTime,Out) waitLock(pWait);\
 Out=pthread_cond_timedwait(&((pWait)->wait),&((pWait)->lock),pTime);\
 if (Out) waitUnLock(pWait)
 
#define waitSignal(pWait) waitLock(pWait);\
 pthread_cond_broadcast(&((pWait)->wait));waitUnLock(pWait)
#define waitSignal1(pWait) waitLock(pWait);\
 pthread_cond_signal(&((pWait)->wait));waitUnLock(pWait)

extern int pthread_spin_init (pthread_spinlock_t *__lock, int __pshared)  __THROW __nonnull ((1));
//extern int pthread_tryjoin_np (pthread_t __th, void **__thread_return) __THROW;
extern int pthread_yield (void) __THROW;
extern int pthread_cancel (pthread_t __th);
extern pthread_t pthread_self (void) __THROW __attribute__ ((__const__));
//extern int pthread_join (pthread_t __th, void **__thread_return);

/*
 * Задания
 */
#define mshJobStart(idPth,Prog,pArg) pthread_create(&(idPth),NULL,Prog,pArg);pthread_detach(idPth)
//завершение потока из него самого 
#define mshJobExit(perr) pthread_exit(perr)
//завершение потока idPth из другого потока 
#define mshJobcancel(idPth) pthread_cancel(idPth)
//ожидание завершения потока 
#define mshJobWait(idPth,pOut) pthread_join(idPth , pOut)
#define mshJobWaitIs(idPth,pOut) pthread_tryjoin_np(idPth,pOut)
//передача выполнения другим потокам 
#define mshJobYield pthread_yield()
//Id собственного потока
#define mshJobthreadId  pthread_self()

#ifdef __cplusplus
}
#endif

#endif /* MSHTASK_H */

