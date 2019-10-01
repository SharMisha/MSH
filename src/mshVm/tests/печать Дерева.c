/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   печать Дерева.c
 * Author: misha
 *
 * Created on 3 марта 2019 г., 10:40
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Simple C Test Suite
 */

void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);
Terr DbOpenDiv(TDb *pDb,char* patch,enum TisPg dbisPg,enum TisArr dbisArr,enum TisLong dbisLong);

void test1() {
 int err;
 char patch[256],*pc;
 TDb Db;
 //
 pc="./mvm/sysWin/sysWinCnst/cnt/";
// pc="gdb/win/win/";
 memCopy(patch,pc,strlen(pc)+1);
 //
 err=DbOpenDiv(&Db,patch,EisPg,EisArrNo,EisLong);
 printf("\nКод завершения Базы %d\n",err);
 vmtstWriteTree(&Db,NULL);
 //
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
