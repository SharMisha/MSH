/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstSize.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 25 ноября 2018 г., 20:10
 */
#include "mshDb.h"

Tdata Data;

//#define pgLongOff int2A[sizeof(TadrPgLong) / sizeof(Tint2) ]

struct TstrInt{
 struct {int i;int j;};
 };
 typedef struct {
  _contArr(struct { _contArr(int);});
  } Tarr2T;
  
 typedef struct {
  Tstr Str;
  char resv;
  char type;
  } TstrSub;
 
typedef enum{
 EwinWin,
 EwinNb,
 EwinLb,
 EwinBut,
 EwinBox,
 EwinBoxBut,
 EwinSc,
 EwinGrid,
 EwinTree,
 EwinListStore,
 EwinTreeStore,
 EwinImg,
 EwinCalendar
 } EwinTypeObj;
  
typedef struct {
 void *pwinObj;
 Tuint32 idObj;
 Tuint32 idParent;
 EwinTypeObj type;
 } TwinObj;


void test1() {
 u_int32_t i01=-1,i02=-2147483647;
 int j;
 int pagesize=getpagesize();
 struct TstrInt a;
// 
 a.i=1;
 a.j=2;
 printf("tstSize test 1\n");
 printf("pagesize=%d Tarr2T=%d\n\n",pagesize,sizeof(Tarr2T));
 printf("ToffData %d TfNum2 %d TfNum %d TintNum %d\n",sizeof(ToffData),sizeof(TfNum2),sizeof(TfNum),sizeof(TintNum));
 printf("TpgLink %d TpgData %d\n",sizeof(TpgLink),sizeof(TpgData));
 printf("Tstr %d Tbstr %d Tkey %d Tval %d\n",sizeof(Tstr),sizeof(Tbstr),sizeof(Tkey),sizeof(Tval));
 printf("TadrOff %d {Tadr %d off %d}\n",sizeof(TadrOff),sizeof(Tadr),sizeof(TintNum));
 printf("TadrOffDb %d {Tadr %d off %d TDb* %d TadrPgLong %d}\n",sizeof(TadrOffDb),sizeof(Tadr),sizeof(Tsize),sizeof(TDb*),sizeof(TadrOff));
 printf("Tkey %d {Tadr %d Tdata %d}\n",sizeof(Tkey),sizeof(Tadr),sizeof(Tdata));
 printf("Tdata %d {TareaOff %d TfNum %d TintNum %d TadrPgLong %d}\n",sizeof(Tdata),sizeof(TareaOff),sizeof(TfNum)
         ,sizeof(TintNum),sizeof(TDbdrLong) );
 printf("TDbdrLong %d {Tadr=%d TmapLong=%d off=%d}\n",sizeof(TDbdrLong),sizeof(Tadr),sizeof(TmapLong*),sizeof(Tuint16));
 Tdata Data;
 memNULL(&Data);
 DataSightSet(Data,0xcc);
 DataTypeSub(Data)=0xdd;
 Data.dbAdrLong.adrLong=0xffffffffffffffff;
 Data.dbAdrLong.pmlong=0xaaaaaaaaaaaaaaaa;
 Data.dbAdrLong.Off=0xbbbbbbbb;
 printf("&Data %p {Tadr=%u TmapLong=%p off=%x sbSt=%x St=%x}\n",&Data,Data.dbAdrLong.adrLong,Data.dbAdrLong.pmlong,Data.dbAdrLong.Off,DataTypeSub(Data),DataType(Data));
 for (j=0;j<sizeof(Tdata);++j) printf("[%x]",Data.st[j]);
 printf("\n-1=%u -2147483647=%u\n",i01,i02);
 int i0=5,i1;
 double f;
 f=i0;
 i1=f;
 printf("i0=%d, f=%f ,i1=%d\n",i0,f,i1);
 Tstr Str;
 Tbstr bStr;
 char *p;
 Str.size=3;
 Str.psec[0]='1';
 Str.psec[1]='2';
 Str.psec[2]='3';
 Str.psec[3]='\0';
 StrTobStr(Str,bStr);
 p=(char*)(bStr.psec);
 printf("Str= %d %s bStr= %i %s \n",Str.size,p,bStr.size,p);
 printf("sizeStr= %d \n",StrSize(Str));
 printf("sizebStr= %d \n",StrSize(bStr));
 printf("sizeTstrSub= %d(sizeTstr %d) \n",sizeof(TstrSub),sizeof(Tstr) );
 printf("\n");
 printf("CsizePgDef= %d CsizeLongDef=%d\n",CsizeDbPgDef,CsizeDbLongDef);
 DataSetNULL(Data);
 //0x18
 DataSetLong(Data);
 DataTypeSub(Data)=0xdd;
 Data.dbAdrLong.pmlong=&Data;
 Data.dbAdrLong.adrLong=777;
 Data.dbAdrLong.Off=111;
 printf("&Data %p {Tadr=%u TmapLong=%p off=%u sbSt=%x St=%x}\n",&Data,Data.dbAdrLong.adrLong,Data.dbAdrLong.pmlong,Data.dbAdrLong.Off,DataTypeSub(Data),DataType(Data));
 printf("TwinObj %d (pwinObj=%d,idObj=%d,idParent=%d, EwinTypeObj= %d )\n",sizeof(TwinObj),sizeof(void*),sizeof(Tuint32),sizeof(Tuint32),sizeof(EwinTypeObj) );
 printf("CcountInt %d CcountInt32=%d,CcountInt16=%d,CcountInt8=%d\n",CcountInt,CcountInt32,CcountInt16,CcountInt8);
 }

void test2() {
 printf("tstSize test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstSize) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstSize\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstSize)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstSize) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstSize)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstSize) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
