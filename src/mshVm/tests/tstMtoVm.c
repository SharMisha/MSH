/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstMtoVm.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 26 ноября 2018 г., 19:17
 */

#include "../../mshDb/H/mshData.h"

/*
 * Simple C Test Suite
 */
int mshMtoVM(char *pcName,int *pnumstr,int *pnumpos,char *pbuferr);
void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);

void test1() {
 int err,numstr,numpos;
 char bufErr[1024];
// TDb Db;
 //
 char *pName=
// "sysCstnt";
// "sysNetCnst";
// "sysNetUser";
// "byxSrvDkList";
// "byxSrvDkData";
// "sysWinCnst";
// "byxUsrUI";
 "sysNetExo";
// "mon";
// "mon1"
 ;
 //
 //инициализация библиотеки
// mshVmWinInit();
 printf("Трансляция файла %s\n",pName);
 bufErr[0]='\0';
 err=mshMtoVM(pName,&numstr,&numpos,bufErr);
// err=tstmshMtoVM(pName,&numstr,&numpos,&Db);
// vmtstWriteTree(&Db,NULL);
 //
 printf("Код завершения %d str=%d pos=%d buferr=%s\nExit\n" ,err,numstr,numpos,bufErr);
 }

void test2() {
 printf("tstMtoVm test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstMtoVm) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstMtoVm\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstMtoVm)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstMtoVm) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstMtoVm)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstMtoVm) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
