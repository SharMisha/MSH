/* 
 * File:   mshDiv.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 20 Август 2014 г., 11:51
 */

#ifndef MSHDIV_H
#define	MSHDIV_H

#ifdef	__cplusplus
extern "C" {
#endif
 
#include "mshHsys.h"
#include "mshMem.h"

/* дискриптор файла */
typedef int Tdiv;

#define Tstream FILE
    
typedef struct {
 char *psec;
 char *pbuf;
 Tstream *pdiv;
 } TdivStream;
#define CdivRetErr -1
#define CdivMod S_IWUSR | S_IRUSR
// | S_IRGRP | S_IWGRP | S_IROTH
/**/
#define divOpen(name) (Tdiv)open(name, O_RDWR, CdivMod)
/**/
#define divOpenR(name) (Tdiv)open(name, O_RDONLY, S_IRUSR)
/* */
#define divCreat(name) (Tdiv)open(name, O_RDWR | O_CREAT | O_TRUNC, CdivMod)

#define divCreatTemp(Template) mkstemp (Template)
/**/
#define divOpenCreate(name) (Tdiv)open(name, O_RDWR | O_CREAT, CdivMod)
/**/
#define divClose(div) close(div)
/**/
#define divRead(div,pbuf,count) read(div, pbuf, count)
/**/
#define divWrite(div,pbuf,count) write(div, pbuf, count)
/* установить позицию в файле */
 #define divPos(div,pos) lseek(div, pos, SEEK_SET)
/* найти текущую позицию */
 #define divPosCur(div) lseek(div, 0, SEEK_CUR)
/*найти конец файла */
#define divPosEnd(div) lseek(div, 0, SEEK_END)
//
#define flush(div) fdatasync(div); 
/*
 * чтение файла с определенной позиции
 */
#define divReadPos(div,buf,count,pos) pread(div, buf, count, pos)
/*
 * запись файла с определенной позиции
 */
#define divWritePos(div,buf,count,pos) pwrite(div, buf, count, pos)
/*
 * Установить длинну файла
 */
#define divSetSize(div,count) ftruncate(div, count)
/* размер файла */
#define divSize(div,count) count=divPosEnd(div);divPos(div,0)
/* работа с директориями */
#define divOpenDir(patchDir) opendir(patchDir)
#define divCloseDir(pDir) closedir(pDir)
#define divMakeDir(patchDir) mkdir(patchDir,0777)
#define divReadDir(pDir) readdir(pDir)
#define divDeleteDir(pDir) rmdir(pDir)
#define divDeleteFile(pFile) unlink(pFile)
/*
 *  Работа с файловым потоком
 */
#define streamOpen(pname,mode) fopen(pname, mode)
#define streamOpenR(pname) fopen(pname,"r")
#define streamOpenA(pname) fopen(pname,"a+")
#define streamCreate(pname) fopen(pname,"w+")
#define streamClose(fd) fclose(fd)
#define streamReadStr(fd,pbuf,sizeStrMAX) fgets(pbuf,sizeStrMAX,fd)
#define streamReadChar(fd) getc(fd)
#define streamReadUnChar(fd,c) ungetc(c,fd)
#define streamWriteBuf(fd,pbuf) fputs(pbuf,fd)
#define streamWritepStr(fd,pStr) fwrite((pStr)->psec,1,(pStr)->size,fd)
#define streamWriteStr(fd,Str) fwrite((Str).psec,1,(Str).size,fd)
#define streamWriteChar(fd,Char) fputc(Char,fd)
#define streamWriteBK(fd) fputc(CprsEndStr,fd)
#define streamEof(fd) (feof(fd))? COutEOF : CERRdivReadBad
/* заменить расщирение файла */
#define mshExtaddtoFL(psExt,pfl) memCopy(pfl+strlen(pfl),psExt,strlen(psExt)+1)
#define mshaddExtToFL(psExt,pfl,wp) pfl[wp]='\0'; mshExtaddtoFL(psExt,pfl)
/*
 * обмен по сети
 */
#define netReadN(div,pb,N) recv(div,pb,N,0)
#define netSendN(fd,pbeg,N) send(fd,pbeg,N,0)

#ifdef	__cplusplus
}
#endif

#endif	/* MSHDIV_H */

