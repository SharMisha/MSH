
/* 
 * File:   mshClnt.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 февраля 2017 г., 15:16
 */
//#include "vmWin.h"
#include "mshvmTr.h"

//void mshVmWinInit();
//void mshVmWinFree();
int mshVMrun(char *pcName,TerrTr *perrTr);

int mshClnt(char *pcName){
 int err;
 TerrTr errTr;
 //инициализация библиотеки
// mshVmWinInit();
 //создать приложение и выполнить Vm программу
 err=mshVMrun(pcName,&errTr);
 //
// mshVmWinFree();
 return err;
 }
