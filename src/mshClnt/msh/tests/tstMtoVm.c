/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstMtoVm.c
 * Author: misha
 *
 * Created on 9 декабря 2018 г., 10:59
 */

#include "mshData.h"

/*
 * Simple C Test Suite
 */
int tstmshMtoVM(char *pcName,int *pnumstr,int *pnumpos,TDb *pDb);
int mshMtoVM(char *pcName,int *pnumstr,int *pnumpos,char *pbuferr);
void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);
void mshVmWinInit();


void test1() {
 int err,numstr,numpos;
 char *pName;
// TDb Db;
 //
// pName="sysCnst";
// pName="winCnst";
// pName="winObj";
// pName="winBox";
// pName="winBoxB";
// pName="winBut";
// pName="winScroll";
// pName="winGrid";
// pName="winTree";
// pName="winLb";
// pName="winNb";
// pName="winImg";
// pName="winWin";
// pName="winCalendar";
// pName="netCnst";
 pName="byxUI";
// pName="byxUI";
 //
 //инициализация библиотеки
 mshVmWinInit();
 printf("Трансляция файла %s\n",pName);
 err=mshMtoVM(pName,&numstr,&numpos,NULL);
// err=tstmshMtoVM(pName,&numstr,&numpos,&Db);
// vmtstWriteTree(&Db,NULL);
 //
 printf("Код завершения %d str=%d pos=%d\nExit\n",err,numstr,numpos);
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
