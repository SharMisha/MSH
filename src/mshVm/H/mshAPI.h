/* 
 * File:   mshAPI.h
 * Author: misha
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 6 ноября 2015 г., 8:58
 */

#ifndef MSHAPI_H
#define	MSHAPI_H

#ifdef	__cplusplus
extern "C" {
#endif
/*
typedef struct {
 int numstr; //транслируемая строка
 int numpos; //позиция трансляции
 Terr err;
 } TerrTr;
*/
/*
 * M программу оттранслировать в Pi код
 *  int numstr; //транслируемая строка
 int numpos; //позиция трансляции
 Tstr name; // имя модуля
 Tstr nameInclude; //имя Include
 Terr err;

 */
int vmAppInit(TApp *pAppIn,char *pnameApp);

void vmAppFree(TApp* pApp);

int mshMtoVM(char *pcName,int *pnumstr,int *pnumpos);
/*
 * выполнить модуль VM
 */
int mshVMrun(char *pcName,TerrTr *perrTr);
/*
 * M программу оттранслировать в Pi код и выполнить
 */
int mshMrun(char *pcName,char *pcLabel,TerrTr *perrTr);
/*
 * Удалить файлы из каталога
 */
int deleteFileFromDir(char *pDir);

#ifdef	__cplusplus
}
#endif

#endif	/* MSHAPI_H */

