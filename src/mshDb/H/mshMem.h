/* 
 * File:   mshMem.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 20 Август 2014 г., 11:54
 */

#ifndef MSHMEM_H
#define	MSHMEM_H

#ifdef	__cplusplus
extern "C" {
#endif
/*
 *  Работа с памятью
 */
#define	memNew(size) malloc(size)
#define	memNewInit(size) calloc(1,size)
#define	memType(Type) (Type*)malloc(sizeof(Type))
#define	memTypeArr(Type,Count) (Type*)malloc(sizeof(Type)*Count)
#define	memFree(ptr) if (ptr) ({free(ptr);ptr=NULL;})
#define	memChange(ptr,size) realloc(ptr,size)
#define	memChangeFree(ptr,size) memFree(ptr);ptr=memNew(size)
#define	memCopy(to,from,n) memcpy(to,from,n)
#define	memMove(to,from, n) memmove((void*)(to),(void*)(from),n)
#define	strCopy(to,from) strcpy(to,from)
#define	memCompare(to,from,n) memcmp(to,from,n)
#define	memSet(to,val,n) memset(to,val,n)
#define	memFindChar(ptr,val,n) memchr(ptr,val,n)
#define	strFindChar(ptr,val) strchr(ptr,val)
//#define	memChr(pstr,chr,size) memchr(pstr,chr,size)
   
#define	memNULLvoid(Ptr,Size) bzero(Ptr,Size)
#define	memNULL(Ptr) bzero(Ptr,sizeof(*(Ptr)))
#define	memNULLsum(Ptr,Sum) (void)memset(Ptr,0,sizeof(*(Ptr))*Sum)
    
#ifdef	__cplusplus
}
#endif

#endif	/* MSHMEM_H */

