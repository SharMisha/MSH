/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstVmtoRun.c
 * Author: misha
 *
 * Created on 16 декабря 2018 г., 10:48
 */

#include <stdio.h>
#include <stdlib.h>


/*
 * Simple C Test Suite
 */
int mshVMrunPos(char *pcName,int *pnumstr,int *pnumpos);

void test1() {
 int err,numstr,numpos;
// char *pcName="byxDk";
 char *pcName="sysCnst";
// char *pcName="byxDkData";
 //
 printf("tstVmtoRun test 1\n");
 //создать приложение и выполнить Vm программ
 err=mshVMrunPos(pcName,&numstr,&numpos);
 printf("Код завершения err=%i #Str=%i #Pos=%i\nExit\n",err,numstr,numpos);
 }
/*
void test2() {
 printf("tstVmtoRun test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstVmtoRun) message=error message sample\n");
}
*/
int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstVmtoRun\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstVmtoRun)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstVmtoRun) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstVmtoRun)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstVmtoRun) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
