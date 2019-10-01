/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstDbWrite.c
 * Author: misha
 *
 * Created on 16 декабря 2018 г., 10:51
 */


#include "mshData.h"
#include "mshvmCnst.h"

/*
 * Simple C Test Suite
 */
void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);
void vmtstWriteSecLongs(TmapLong *pmapLong,TdivStream *pdiv);
//Terr DbOpenDiv(TDb *pDb,char* patch,enum TisPg dbisPg,enum TisArr dbisArr,enum TisLong dbisLong);
Terr DbOpenDiv(TDb *pDb,char* patch,enum TtypeDb typeDb);

void test1() {
 int err;
 char patch[256],*pc;
 TDb Db;
 //
// pc="mvm/sys/sysCnst/cnt/";
 pc="gdb/d1/d1/";
 memCopy(patch,pc,strlen(pc)+1);
 //
// err=DbOpenDiv(&Db,patch,EisPg,EisArrNo,EisLong);
 err=DbOpenDiv(&Db,patch,EtypeDb);
 printf("\nКод завершения Базы %d\n",err);
 vmtstWriteTree(&Db,NULL);
 if (Db.mlong.div!=CdivRetErr) vmtstWriteSecLongs(&(Db.mlong),NULL);
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
