/*
 * File:   mshFile.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 14:44
 */
#include "mshData.h"
#include "mshMap.h"
#include "mshDiv.h"
#include "mshCnst.h"
#include "mshErr.h"
/*
const static char CvmSTDOUT[]="OUT";

void AreaDelete(Tarea *pArea,Toff off);
void castbStr(Tdata *pData,Tbstr *pbstrOut);
Terr checkDivMag(Tdiv ff,const char *pmag);
*/
/*
 * Проверка магического числа в файле
 */
Terr checkDivMag(Tdiv ff,const char *pmag){
 int n;
 n=strlen(pmag);
 char buf[n];
 //
 divPos(ff,0);
 if ((divRead(ff,buf,n))<n) return CERRdivMagic;
 return CompareMag(buf,pmag);
 }
/*
 */
Terr openDir(char *patchDir){
 DIR *pDir=NULL;
 int w;
 char *pc,*pcend,ce='\0';
 int isDir=1;
 Terr err=COutKodOk;
 //
 w=strlen(patchDir)-1;
 ce=patchDir[w];
 if (patchDir[w]==CdelimFile) patchDir[w]='\0';
 //директория существует
 if ((pDir=divOpenDir(patchDir))) {
  divCloseDir(pDir);
  goto finish;
  }
 //директория НЕ существует создать ее
 pcend=patchDir + w -1;
 for (pc=patchDir;pc<pcend;++pc)
  if (*pc==CdelimFile) {
   *pc='\0';
   if (isDir){
    pDir=divOpenDir(patchDir);
    if ((pDir)){
     *pc=CdelimFile;
     divCloseDir(pDir);
     continue;
     }
    isDir=0;
    }
   if (divMakeDir(patchDir)) {
    *pc=CdelimFile;
    err=CERRdivDirFiled;
    goto finish;
    }
   *pc=CdelimFile;
   }
 divMakeDir(patchDir);
finish: 
 patchDir[w]=ce;
 return err;
 }
/*
 * загрузка исходного файла с устройства
 */
Terr mshProgReadToMap(char *pfl,const char *pCmshMag,Tmap *pMap);
Terr mshProgReadToMapFixed(char *pfl,const char *pCmshMag,TmapFixed *pMap);

void getPachtLibDir(Tstr *pname,const char *pRas,int sizeDir,char *pout,int sizeout);

void getProgNameFile(Tstr *pname,const char* pCmshDirMsh,int sizeDir,char* pfl){
 getPachtLibDir(pname,pCmshDirMsh,sizeDir,pfl,CsizePacthFl);
 pfl[strlen(pfl)-1]='\0';
 }
/*
Terr loadDivMapFixed(Tstr *pname,TmapFixed *pMap,const char* pCmshDirMsh,const char* pCmshMagMsh,int sizeDir){
 char pfl[CsizePacthFl];
 //
 getProgNameFile(pname,pCmshDirMsh,sizeDir,pfl);
 return mshProgReadToMapFixed(pfl,pCmshMagMsh,pMap);
 }
*/
/*
 * Удалить файлы из каталога
 */
static int deleteFileFromDirSub(const char *pfilename,const struct stat *pstatus, int flag,struct FTW *__info){
 if (flag==FTW_F){ divDeleteFile(pfilename);}
 return FTW_CONTINUE;
 }
//
int deleteFileFromDir(char *pDir){
 return nftw(pDir, deleteFileFromDirSub,20,FTW_PHYS);
 }
/*
 * Удалить каталог
*/

static int deleteDirSub(const char *pfilename,const struct stat *pstatus, int flag,struct FTW *__info){
 if (flag==FTW_F){
  divDeleteFile(pfilename);
  }
 else
  if ((flag==FTW_D)||(flag==FTW_DP)){divDeleteDir(pfilename);}
 return FTW_CONTINUE;
 }
//
int deleteDir(char *pDir){
 return nftw(pDir, deleteDirSub,20, FTW_DEPTH | FTW_PHYS);
 }
