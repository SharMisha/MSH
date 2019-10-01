/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstvmVmtoRun.c
 * Author: misha
 *
 * Created on 3 марта 2019 г., 13:39
 */

#include <stdio.h>
#include <stdlib.h>

#include "mshvmTr.h"

int mshVMrun(char *pcName,TerrTr *perrTr);

/*
 * Simple C Test Suite
 */

void test1() {
 int err;
 char *pcName=
 "byxUsrUI"
// "byxSrvDkData"
 ;
TerrTr errTr;
 //
 printf("tstvmVmtoRun test 1, name=%s\n",pcName);
 err=mshVMrun(pcName,&errTr);
 printf("err=%i\n",err);
 if (err){
  errTr.nameMod.psec[errTr.nameMod.size]='\0';
  printf("nameMod=%s nstr=%i, npos=%i\n",errTr.nameMod.psec,errTr.numstr,errTr.numpos);
  }
 }
/*
void test2() {
    printf("tstvmVmtoRun test 2\n");
    printf("%%TEST_FAILED%% time=0 testname=test2 (tstvmVmtoRun) message=error message sample\n");
}
*/
int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% tstvmVmtoRun\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% test1 (tstvmVmtoRun)\n");
    test1();
    printf("%%TEST_FINISHED%% time=0 test1 (tstvmVmtoRun) \n");
/*
    printf("%%TEST_STARTED%% test2 (tstvmVmtoRun)\n");
    test2();
    printf("%%TEST_FINISHED%% time=0 test2 (tstvmVmtoRun) \n");
*/
    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
