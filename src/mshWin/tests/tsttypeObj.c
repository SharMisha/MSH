/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tsttypeObj.c
 * Author: misha
 *
 * Created on 8 февраля 2019 г., 9:40
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * Simple C Test Suite
 */

void test1() {
 printf("tsttypeObj test 1\n");
 printf("typeWin= %lu\n",gtk_window_get_type());
 printf("typeNb= %lu\n",gtk_notebook_get_type());
 }

void test2() {
    printf("tsttypeObj test 2\n");
    printf("%%TEST_FAILED%% time=0 testname=test2 (tsttypeObj) message=error message sample\n");
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% tsttypeObj\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% test1 (tsttypeObj)\n");
    test1();
    printf("%%TEST_FINISHED%% time=0 test1 (tsttypeObj) \n");
/*
    printf("%%TEST_STARTED%% test2 (tsttypeObj)\n");
    test2();
    printf("%%TEST_FINISHED%% time=0 test2 (tsttypeObj) \n");
*/
    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
