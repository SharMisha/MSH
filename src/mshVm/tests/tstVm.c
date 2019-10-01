/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstVm.c
 * Author: misha
 *
 * Created on 26 ноября 2018 г., 19:14
 */

#include "mshvmDebag.h"

void trCodamshCmdBuf(Ttr *pTr,Tbstr *pbufPrg,Tstr *pName);
void tstvmprnTr(Ttr *pTr,TdivStream *pdiv);
void TrInit(Ttr *pTr);
void TrFree(Ttr *pTr);
void mshTrRunpDo(Ttr *pTr,TApp *pApp,TvmDo **ppDo,u_char flag, TdivStream *pdiv);
Terr vmAppInit(char *pnameApp);
void vmAppFree();
void vmprnTtrErr(TerrTr *perr,TdivStream *pdiv);
void tstvmprnJob(TvmDo *pDo,TdivStream *pdiv);
void tstvmprnApp(TvmDo *pDo,TdivStream *pdiv);
void tstvmprnDoDb(TvmDo *pDo,TdivStream *pdiv);
/*
 * Simple C Test Suite
 */

void test1() {
 Ttr Tr;
 TdivStream *pdiv=NULL;
// TApp App;
 TvmDo *pDo;
 Tbstr bufM;
 char *pMcmd;
// char bcmdM[1024];
 Terr err;
 int i;
 //
 printf("tstVm test 1\n");
 pMcmd=NULL;
// pMcmd=" S [9]=Ab;";
// pMcmd=" S [9]=Ab,[1]=123.5;";
// pMcmd=" S ^gb[2]=GB;";
// pMcmd=" S %t[2]=TMP;";
// pMcmd=" S $2=9;";
// pMcmd=" S $2=9.7;";
// pMcmd=" S ^gb$2=GB;";
// pMcmd=" S $2={1,2};";
// pMcmd=" S ^gb$2={1,2};";
// pMcmd=" S [9]=A12345678901234567890123456789;";
// pMcmd=" S [9]=A1234567890123456789012003456789,[1]=[9]; K [9];";
// pMcmd=" S [9]=A1234567890123456789012003456789,[1]=[9]; Kill [9];";
// pMcmd=" S [9]=A12345678901234567890123456789,[1,[9]]=2;";
// pMcmd=" S [9+1]=Ab_1;";
// pMcmd=" S [9,5]=Ab;";
// pMcmd=" S $3=Ab;";
// pMcmd=" S ld[9]=Ab;";
// pMcmd=" S ld[9]=Ab,$1=ld[9];";
// pMcmd=" S Is[9]=Ab,Is[10]=Is[9];";
// pMcmd=" S Is[9]=Ab,Is[10]=Is[9]; COPY Pr[]=Is[];";
// pMcmd=" S Is[1,9]=Ab,Is[1,10]=Is[1,9]; COPY Pr[2]=Is[1];";
// pMcmd=" S Is[1,9]=Ab,Is[1,10]=Is[1,9]; COPY Pr[2]=Is[];";
// pMcmd=" S Is[1,9]=Ab,Is[1,10]=Is[1,9]; COPY Pr[]=Is[1];";
// pMcmd=" S ld$3=Ab;";
// pMcmd=" S %j[9,5]=Ab;";
// pMcmd=" S %j$3=Ab;";
// pMcmd=" S %%[9,5]=Ab;"; // % - спецсимвол и принимается только 1 раз имя будет %
// pMcmd=" S %%ld[9,5]=Ab;"; 
// pMcmd=" S ^gb[9]=Ab;";
// pMcmd=" S ^gb[9,5]=Cd;";
// pMcmd=" S ^%$2=35;"; //ошибка глобаль без имени
// pMcmd=" S %^$2=35;"; //временная переменная %
// pMcmd=" S ^%tmp[9]=Ab;";
// pMcmd=" S Ab$1=Cd;";
// pMcmd=" S Ab$1=Cd";//Нет конца команды
// pMcmd=" S bA$(1+1)=25";
//pMcmd=" S bA$(1+1)=25;";//Нет конца команды
// pMcmd=" S $1=J$1;";
// pMcmd=" S $1='bA',($1)$1=78;";
// pMcmd=" S $1=bA,$1$1=78;";
// pMcmd=" S %Ab$2=89,$1=%Ab$2;";
// pMcmd=" S %$2=89;";
// pMcmd=" S $2=^gb[8];";
// pMcmd=" S $2=121[8];";//Ошибка, имя глобали может быть только идентификатором
// pMcmd=" S [10,7]=8,$1=[10,[10,7]];";
// pMcmd=" S [10,7]=7,$1=[10,[10,7]];";
// pMcmd=" S [9+3]=100+7;";
// pMcmd=" S $1=4,$$1=44,$2=$0;";
// pMcmd=" S $(2+1)=32;";
// pMcmd=" S $2+1=32;"; //ошибка -2024
// pMcmd=" S $(2+1)=32;";
// pMcmd=" S $1=2,$$1=32;";
// pMcmd=" S ^$2=32;";
// pMcmd=" S ^gb[11]=Ab; KD ^gb[11];";
// pMcmd=" S ^gb[10]=Ab,^gb[10,1]=2; KC ^gb[10];";
// pMcmd=" S ^gb[10]=Ab,^gb[10,1]=2; K ^gb[10];";
// pMcmd=" S %timePrimer1[3]=1,%timePrimer1$3=2;";
// pMcmd=" S %timePrimer1$3=2;";
// pMcmd=" S %timePrimer1[3]=1;";
// pMcmd=" S timePrimer1[3]=1;";
// pMcmd=" S [1]=11,[1,2]=12,[1,2,3]=13; KD [1,2]";
// pMcmd=" S [1,2]=12,[1,2,3]=13; K [1,2]";
// pMcmd=" S [1]=11,[1,2]=12,[1,2,3]=13; KC [1,2];";
// pMcmd=" S [1]=11,[1,2]=12,[1,2,3]=13,[1,2:4]=14;";
// pMcmd=" S [1]=11,[1,2]=12,[1,2,3]=13,[1,2:4]=14,[1,2,5]=15;";
// pMcmd=" S [1]=11,[1,2]=12,[1,2,3]=13,[1,2:4]=14; KP [1,2];";
// pMcmd=" S [1]=11,[1,2]=12,[1,2,3]=13,[1,2:4]=14; K [1,2];";
// pMcmd=" S $5=55;K $5;";
// pMcmd=" S $5=55,$2=22,$7=77;K $;";
// pMcmd=" S $3=.Lb;";//ошибка модуль не найден
// pMcmd=" S Lb=3;";//ошибка модуль не найден
// pMcmd=" S (Md_1).Lb=3;";
// pMcmd=" Do Md.Lb.pr.7;";
// pMcmd=" Do Md.Lb(pr.lb.arg);";
// pMcmd=" Do LB(1,2);";
// pMcmd=" Do LB_1;";
// pMcmd=" Do (MD_7).(LB_8);";
// pMcmd=" Do ^gb[9,5].^gb[9,5](42,2);";
// pMcmd=" Do [MOD].[LAB](1+5*7,2);";
// pMcmd=" Do LB;";
// pMcmd=" Do MD.;";
// pMcmd=" Go Lb;";
// pMcmd=" S $a=100;";
// pMcmd=" S a[1,10]=100,a[1,20]=102,a$1=201,a$2=202;";
// pMcmd=" S a[1,10]=100,a[1,20]=102,a$1=201,a$2=202; K a;";
// pMcmd=" S a[1,10]=100,a[1,20]=102,a$1=201,a$2=202; K a[];";
// pMcmd=" S a[1,10]=100,a[1,20]=102,a$1=201,a$2=202; K a$;";
// pMcmd=" S [1,10]=100,[1,20]=102; K [];";
// pMcmd=" KDP ^gb[9].Lb;";
// pMcmd=" KDP Lb;";
// pMcmd=" KDP .Lb;";
// pMcmd=" KDP MD.Lb;";
// pMcmd=" S gb[9].Lb=45;";
// pMcmd=" S $1=i1[1].I1;";
// pMcmd=" D i1[1].%this;";
// pMcmd=" S i1[1].I1=5;";
// pMcmd=" S pr[11,[12].P2,M.L,$1].Pr=i1[1].I1+ i2.I2+g[3];";
// pMcmd=" S %[7]=[Ab].$3_(8+7);";
// pMcmd=" S $(4+5)=25;";
// pMcmd=" S [4+5,7]=25;";
// pMcmd=" Const C4=5,C21=Abc;";
// pMcmd=" Const C4=5,C21=Abc; S $C4=C21;";
// pMcmd=" S $4=21,$1=4,$2=$$1;";
// pMcmd=" S $4=($21+5-8)*[A];";
// pMcmd=" S $4=($21+5-8)*9;";
// pMcmd=" S $(A==45)=%$21==8;";
// pMcmd=" S $4=40,$1=10,$2=20; K $2;";
// pMcmd=" S $4=40,$1=10,$2=20; K $;";
// pMcmd=" S $4=40,$1=10,$2=20; COPY lk$=$;";
 pMcmd=" S A$1=10,A$2=20,A$3=30; COPY $=A$;";
// pMcmd=" S $4=40,$1=10,$2=20; COPY lk[]=$;";
// pMcmd=" S k[4]=40,k[1]=10,k[2]=20; COPY $=k[];"; //err ссылка на массив не может быть использована в данном контексте Время выполнения
// pMcmd=" S ^gb[9]=9,^gb[9,1]=91;";
// pMcmd=" Copy [8,2]=^gb[9];";
// pMcmd=" Copy []=^gb[];";
// pMcmd=" Copy [8]=^gb[];";
// pMcmd=" Copy [8]=^gb[]; Move b[12]=[8];";
// pMcmd=" S $1=2?1!0;";
// pMcmd=" S $1=0?1!0;";
// pMcmd=" S $1=2?1?11!10!0;";
// pMcmd=" S $1=0?1?11!10!20;";
// pMcmd=" S $1=1?0?11!21!20;";
// pMcmd=" If?0 S $2=101; E S $2=104; END;";
// pMcmd=" S $1=28; If?$1==25 S $2=101; E?$1==23 S $2=103; E S $2=104; END; S $3=33;";
// pMcmd=" S $1=28; If?$1==28 S $2=101; E?$1==23 S $2=103; E S $2=104; END; S $3=33;";
// pMcmd=" S $1=23; If?$1==28 S $2=101; E?$1==23 S $2=103; E S $2=104; END; S $3=33;";
// pMcmd="\n S $1=A;\n CASE?$1;\n   S $2=4;\nA: S $2=1;\nB: S $2=2;\nC: S $2=3;\n endCase;\n S $3=7;\n";
// pMcmd="\n CASE?A;\n   S $2=4;\nA: S $2=1;\nB: S $2=2;\nC: S $2=3;\n endCase;\n S $3=7;\n";
// pMcmd="\n Const CA=A;\n S $1=A;\nCASE?$1;\n S $2=0;\nCA: S $2=1;\nB: S $2=2;\nendCase;\n S $3=7;\n";
// pMcmd="\n Const CA=A;\n S $1=CA;\nCASE?$1;\n S $2=0;\nCA: S $2=1;\nA: S $2=4;\nB: S $2=2;\nendCase;\n S $3=7;\nCASE?$1;\n S $4=0;\nCA: S $4=1;\nA: S $4=4;\nB: S $4=2;\nendCase;\n";
// pMcmd="\n S $1=1,$4=C2;\nCASE?$1;S $2=200\nA1: S $2=20;\nB1: CASE?$4;\nA2: S $5=40;\nB2: S $5=41;\n endCASE;\nC1: S $2=22;\n endCASE;\n S $3=7;\n";
// pMcmd="\n S $1=D1,$2=4;\n CASE?$1;\n  If?$2==3 S $3=4;\n  E S $3=14;\n  endIf\n  S $2=24;\n A1: S $3=B;\n B1: CASE?$2;\n  A2: S $3=34;\n  B2: S $3=35;\n  endCASE;\n  S $4=45; \n C1: S $4=55;\n endCASE; S $5=7;\n";
// pMcmd=" LW Nm;";
// pMcmd=" LW Nm;LU Nm;";
// pMcmd=" LU $2_5;";
// pMcmd=" TO 200;";
// pMcmd=" EvC ev=winObj.New;";
// pMcmd=" EvC ev=Md.(Lb_1);";
// pMcmd=" EvC ev=Lb_1;";
// pMcmd=" EvC ev=Lb;";
// pMcmd=" EvC ev_1=(Md_2).(Lb_3);";
// pMcmd=" EvC [ev]=[Md].$2;";
// pMcmd=" Do (Lb_7);";
// pMcmd=" Go Lb_7;";
// pMcmd=" EvC (Lb_7);"; //должна быть ошибка
// pMcmd=" EvC md.lb=Md.Lb;";
// pMcmd=" EvC ev_8=Lb_7;";
// pMcmd=" EvT ev(A,2);";
// pMcmd=" EvT ev_1;";
// pMcmd=" EvT ev;";
// pMcmd=" EvW ev;";
// pMcmd=" Do lb(A,2);";
// pMcmd=" Ret 25+1;";
// pMcmd=" Ret 25;";
// pMcmd=" Ret $2;";
// pMcmd=" Ret;";
// pMcmd=" S $1=10; While?$1<19 ; S $$1=$1+100; S $1=$1+1; END; s $2=20;";
// pMcmd=" S $1=5; While; S $$1=$1+100; S $1=$1+1; ENDfor?$1>19; s $2=20;";
// pMcmd=" S $1=5; While?$1<19; S $1=$1+1; S $3=A; ENDfor?$1>10; s $2=20;";
// pMcmd=" K ^gb[]; S ^gb[1]=121,^gb[2]=122,^gb[4]=124,^gb[6]=126,^gb[8]=128,^gb[9]=129,^gb[1,10]=101,^gb[1,12]=102,^gb[1,13]=103,^gb[1,14]=104,^gb[1,15]=105,^gb[1,20]=220;";
// pMcmd=" S ^gb[1,10]=101,^gb[1,12]=102,^gb[1,13]=103,^gb[1,14]=104,^gb[1,15]=105;";
// pMcmd=" K ^gb[5]; S ^gb[7]=127;";
// pMcmd=" S ^gb[1,20]=220;";
// pMcmd=" S %t[1,10]=101,%t[1,12]=102,%t[1,13]=103,%t[1,14]=104,%t[1,15]=105;";
// pMcmd=" K $2; Next1 ^gb[],$2;";
// pMcmd=" S $2=1; Next1 ^gb[],$2;";
// pMcmd=" S $2=1; Next1 ^gb[1],$2;";
// pMcmd=" S $2=30; Next1 ^gb[1],$2;"; //None
// pMcmd=" S $2=10; Next1 ^gb[1],$2;";
// pMcmd=" S $2=11; Next1 ^gb[1],$2;";
// pMcmd=" S $2=15; Next1 ^gb[1],$2;";
// pMcmd=" S $2=20; Next1 ^gb[1],$2;"; //None
// pMcmd=" S $2=17; Next1 ^gb[1],$2;";
// pMcmd=" S $2=1; Next1 ^gb[$2],$2;";
// pMcmd=" K $2; Next1 ^gb[],$2;Next1 ^gb[],$2;";
// pMcmd=" K ^gb$; S ^gb$1=11,^gb$2=12,^gb$3=13,^gb$4=14,^gb$5=15,^gb$9=19;";
// pMcmd=" K $2; Next1 ^gb$,$2;";
// pMcmd=" K $2; Qery1 ^gb[],$2;";
// pMcmd=" S $2=5; Qery1 ^gb[],$2;";
// pMcmd=" S $2=1; Qery1 ^gb[],$2;";
// pMcmd=" S $2={1,15}; Qery1 ^gb[],$2;";
// pMcmd=" S $2={1,17}; Qery1 ^gb[],$2;";
// pMcmd=" S $2={1,11}; Qery1 ^gb[],$2;";
// pMcmd=" S $2=1; Qery1 ^gb[$2],$2;";
// pMcmd=" S $2=11; Qery1 ^gb[1],$2;";
// pMcmd=" S $2=11; Qery1 ^gb[$2],$2;"; //None
// pMcmd=" S $2=26; Qery1 ^gb[1],$2;"; //None
// pMcmd=" S ^gb[1,10,100]=301,^gb[1,10,105]=302,^gb[1,10,110]=310,^gb[1,10,112]=312,^gb[1,10,120]=320;";
// pMcmd=" S $2={10,130}; Qery1 ^gb[1],$2;";
// pMcmd=" KD ^gb[1,12]; S ^gb[1,12,200]=400;";
// pMcmd=" K $2; Qery1 ^gb[1],$2;";
// pMcmd=" S $2=100; Qery1 ^gb[1],$2;"; //None
// pMcmd=" S $2=1; Qery1 ^gb[1],$2;";
// pMcmd=" S $2={1,2}; Qery1 ^gb[1],$2;";
// pMcmd=" S $2={11,2}; Qery1 ^gb[1],$2;";
// pMcmd=" S $2={10,2}; Qery1 ^gb[1],$2;";
// pMcmd=" S $2={16,2}; Qery1 ^gb[1],$2;";
// pMcmd=" K $2; Qery1 ^gb[],$2;Qery1 ^gb[],$2;";
// pMcmd=" S $2=2,$1=3; Next ^gb[],$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S $2=2,$1=2; While Next1 ^gb[],$1; Break?%eof ; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S $2=2,$1=2; While Qery1 ^gb[],$1; Break?%eof ; S $2=$2+1,$$2=$1; END;";
// pMcmd=" Next ^gb[]; END;";
// pMcmd=" S $2=3; Next ^gb[],$$2; S $2=$2+1; END;";
// pMcmd=" S $2=2,$1=3; Next ^gb[],$1; S $2=$2+1,$$2=$1; END?$1>7;";
// pMcmd=" S $2=2,$1=3; Next ^gb[],$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" K ^g$; S ^g$2=12,^g$4=14,^g$5=15,^g$8=18,^g$9=19;";
// pMcmd=" S ^g$1=11,^g$2=12,^g$13=23,^g$14=24,^g$15=25;";
// pMcmd=" K ^g$14;";
// pMcmd=" S $2=2,$1=10; Next ^g$,$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S ^gb[11]=21,^gb[12]=22,^gb[13]=23,^gb[14]=24,^gb[15]=25;";
// pMcmd=" S $2=2,$1=8; Next ^gb[],$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S $2=2,$1=8; Back ^gb[],$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S $2=2; Next ^gb[1],$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S $2=3; Query ^gb[]; S $2=$2+1,$$2=%queryKey; END;";
// pMcmd=" K $1; S $2=3; Query ^gb[],$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S $1=2; Query1 ^gb[],$1;";
// pMcmd=" S $2=3,$1=2; Query ^gb[],$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S $2=3,$1={1,10}; Query ^gb[],$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S $2=0; Query ^gb[],$1; S $2=$2+1,gi$$2=$1; END;";
// pMcmd=" S $2=0; Query ^gb[]; S $2=$2+1,gi$$2=%queryKey; END;";
// pMcmd=" K $1; S $2=0; Query ^g$,$1; S $2=$2+1,gi$$2=$1; END;";
// pMcmd=" K $1; S $2=0; Next ^g$,$1; S $2=$2+1,gi$$2=$1; END;";
// pMcmd=" S [25]='Привет789второй789третий789';";
// pMcmd=" S $2='Привет789второй789третий789';";
// pMcmd=" S gb[1]=2,gb$3=4; K gb[];";
// pMcmd=" S gb[1]=2,gb$3=4; K gb$;";
// pMcmd=" S gb[1]=2,gb$3=4; K gb;";
// pMcmd=" K ^gb;";
// pMcmd=" S $1=^gb$1;";
// pMcmd=" S ^gb$2='Привет789второй789третий789';";
// pMcmd=" S ^gb$3='Привет789,второй789,третий789,четвертый,пятый';";
// pMcmd=" S ^gb[25]='Привет789второй789третий789';";
// pMcmd=" set $1=%sysReadDir('ldb');";
// pMcmd=" set $1=%sysReadDirFile('ldb');";
// pMcmd=" set $1=%sysReadDirR('ldb');";
// pMcmd=" set %sysReadDirR('ldb');";
// pMcmd=" set $1=%sysReadDirFileR('ldb');";
// pMcmd=" set %sysReadDir('ini');";
// pMcmd=" Do [2].%iniRead('1_BA');";
// pMcmd=" Do [2].%iniRead('1_BA'),[2].%iniWrite('1_BC');";
// pMcmd=" Do [].%iniRead('1_BA');";
//----------------------- 
// pMcmd=" Do [2].%winNew('^%[2]');";
// pMcmd=" Do winObj.New(ar$);";
// pMcmd=" Set win[1].%objNew('pos=800','X=2')=winWin;";
//-----------------------
 /*
  * GetStr,CodeStr,PopStr,InsStr,SetStr,DelStr,FindStr,Repl
  * 
  * GetField, PushField, PopField, InsField, SetField, DelField
  * 
  * GetList, PushList, PopList, InsList, SetList, DelList
  * ----------------------
 */
// pMcmd=" S $1=^gb$2;";
// pMcmd=" K ^gb$; S ^gb$2='Привет789,второй789,третий789,четвертый,пятый',^gb$3=^gb$2;";
// pMcmd=" S ^gb$2='Привет789,второй789,третий789,четвертый,пятый';";
// pMcmd=" Set $1=Lab.%findLabel;";
// pMcmd=" S $1=^gb$2.%strGet(3,4);";
// pMcmd=" S $2=^gb$2,$2.%strGet(3,2);";
// pMcmd=" S $2=^gb$2.%strGet(3);";
// pMcmd=" S $2=^gb$2.%strCode(3);";
// pMcmd=" S $2=$1.%strPop(3);";
// pMcmd=" S $1=^gb$2,$2=$1.%strPop(4);";
// pMcmd=" S $3=^gb$2,$3.%strPop(4);";
// pMcmd=" S $1=^gb$2.%strIns(3,'AB');";
// pMcmd=" S $3=^gb$2,$3.%strIns(3)='AB';";
// pMcmd=" S $1=^gb$2.%strSet(3,2,'AB');";
// pMcmd=" S $2=^gb$2,$2.%strSet(3,4)='AB';";
// pMcmd=" S $2=^gb$2.%strDel(3,2);";
// pMcmd=" S $2=^gb$2,$2.%strDel(3,2);";
// pMcmd=" S $2=^gb$2.%strFind('ив');";
// pMcmd=" S $2=^gb$2.%strFind('вт',2);";
// pMcmd=" S $2=^gb$2.%strFind('9',11);";
// pMcmd=" S $2=^gb$2.%strFind('нету',2);";
// pMcmd=" S $2=^gb$2.%strRepl('ив',ABC);";
// pMcmd=" S $2=^gb$2,$2.%strRepl('ив')=ABC;";
// pMcmd=" S $2=^gb$2.%strRepl('ив');";
// pMcmd=" S $2=^gb$2,$2.%strRepl('ив');";
// pMcmd=" S $2=^gb$3.%fieldGet(',',3,2);";
// pMcmd=" S $1=1,$2=10,$1+$2+100;"; //сокращенная команда Set
// pMcmd=" S $2='2017.02.22',$3=$2.%fieldGet('.',1,2);";
// pMcmd=" S $4='2017.02.',$3='2017.05'; Next ^d1[],$4; Break?($4.%fieldGet('.',1,2) ~= $3); S [%queryKey]=%queryData; END;";
// pMcmd=" S $4='2017.02.',$5='BR'; Next ^d1[],$4; Next ^d1[$4,$5]; S [$4,%queryKey]=%queryData; END; END;";
// pMcmd=" S $2=^gb$3.%fieldPush(',','добавить');";
// pMcmd=" S $3=^gb$3,$3.%fieldPush(',')='добавить';";
// pMcmd=" S $2=^gb$3.%fieldPush(',');";
// pMcmd=" S $3=^gb$3,$2=$3.%fieldPop(',',2);";
// pMcmd=" S $3=^gb$3,$2=$3.%fieldPop(',');";
// pMcmd=" S $2=^gb$3.%fieldIns(',',3,'вставка');";
// pMcmd=" S $2=^gb$3,$2.%fieldIns(',',3)='вставка';";
// pMcmd=" S $2=^gb$3.%fieldSet(',',3,1,замена);";
// pMcmd=" S $2=^gb$3.%fieldSet(',',3,2,'замена');";
// pMcmd=" S $2=^gb$3,$2.%fieldSet(',',3,1)=замена;";
// pMcmd=" S $2=^gb$3,$2.%fieldSet(',',3,2)=замена;";
// pMcmd=" S $2=^gb$3,$2.%fieldSet(',',3,2);";
// pMcmd=" S $2=^gb$3.%fieldDel(',',3);";
// pMcmd=" S $2=^gb$3,$2.%fieldDel(',',3);";
// pMcmd=" S $2=^gb$3.%fieldDel(',',3,2);";
// pMcmd=" S $2=^gb$3,$2.%fieldDel(',',3,2);";
// pMcmd=" S ^gb$4={1,2,3,4};";
// pMcmd=" S $2=^gb$4.%listGet(3,2);";
// pMcmd=" S $1={6,7},$2=^gb$4.%listPush($1);";
// pMcmd=" S $2=^gb$4.%listPush(6,7,8);";
// pMcmd=" S $1=^gb$4,$2=$1.%listPop(2);";
// pMcmd=" S $2=^gb$4,$2.%listPop(2);";
// pMcmd=" S $2=^gb$4.%listIns(2,15,16);";
// pMcmd=" S $2=^gb$4,$2.%listIns(2)={15,16};";
// pMcmd=" S $2=^gb$4.%listSet(2,1,15,16);";
// pMcmd=" S $2=^gb$4,$2.%listSet(2,1)={15,16};";
// pMcmd=" S $2=^gb$4.%listDel(2,1);";
// pMcmd=" S $2=^gb$4,$2.%listDel(2,1);";
// pMcmd=" S $2=^gb$4.%listDel(2,2);";
// pMcmd=" S $2=^gb$4.%listDel(3,2);";
// pMcmd=" S $2=^gb$4,$2.%listDel(2,2);";
// pMcmd=" S $2=^gb$4.%listDel(2);";
// pMcmd=" S $2=^gb$4,$2.%listDel(2);";
// pMcmd=" S $.%arrPush=2;";
// pMcmd=" S m$0=0; S $5=m$.%arrPush(2);";
// pMcmd=" S m$0=0,m$.%arrPush=10,m$.%arrPush=20,m$.%arrPush=30,$3=m$.%arrPop;";
// pMcmd=" S $.%arrPush(10,20,30);";
// pMcmd=" S $.%arrPush(10,20,30),$5=$.%arrPop;";
// pMcmd=" S $.%arrPush(10,20,30),$.%arrPop;";
// pMcmd=" S [%this,5]=15;";
// pMcmd=" S $1=^gb$2,$2=$1.%size;";
//------ 
// pMcmd=" S $2=^gb[1],$1=%statData;";
// pMcmd=" Set $2=1,$5=5; While?$2<$5; Set $2=$2+1,$($5+$2)=$2; End;";
// pMcmd=" Set $1=1,$2=10,$1+$2+3;"; //$1=1, $2=14;
// pMcmd=" Set B$1=1,B$2=2; Write ST;";
// pMcmd=" Set cm$3=I1,$3=I2,$2=D,us[cm$3,$3]=$2;";
// pMcmd=" S $2=^gb[1,5],$1=%statData;";
// pMcmd=" S $2=^gb[1,12],$1=%statData;";
// pMcmd=" S [5,3]=8,$2=[5,3],$1=%Key;";
// pMcmd=" S [5,3]=8,$2=[5,3],$1=%Data;";
// pMcmd=" S [2:8,5]=3;";
// pMcmd=" S [2:]=3;";
// pMcmd=" S []=3;"; //err??
// pMcmd=" S [5].[%this,7]=3;";
// pMcmd=" S [%this:5]=3;";
// pMcmd=" S $2=ab,$1=$2.%size;";
// pMcmd=" S $2=%idDo;";
// pMcmd=" Do tstDo.sub(11,12);";
// pMcmd=" Do tstDo.sub(1,2,3,4,5,6,7,8,9);";
// pMcmd=" job tstJob.sub(11,12);set $1=job@0;";
// pMcmd=" job tstevCall.ev;evT usrCall(11,12);";
// pMcmd=" job tstevWait.ev;evT usrCall(11,12);";
// pMcmd=" S $2=J$0;";
//------------------------ 
// pMcmd=" D [5].%objNew(objUsr);";
// pMcmd=" S [5].%objNew(A1,A2)=ObjUsr;";
// pMcmd=" D [5].%objNew(A1,A2,objUsr);";
// pMcmd="Set [%this].%objNew($)=winObj;";
// pMcmd=" Set win[1].%objNew()=winWin;";
// pMcmd=" Do win[1].%objNew(winWin);";
// pMcmd=" Set $1=%findLabel(Lb,Md);";
// pMcmd=" Set $1=%findLabel(Lb);";
// pMcmd=" Set $1=winObj.New.%findLabel;";
// pMcmd=" S $3=9223372036854775807,$1=$3`s1;";
// pMcmd=" S $3.%v32(1)=2147483647,$1=$3`s1;";
// pMcmd=" S $3.%v32(1)=2147483647;";
// pMcmd=" S $3.%v32(1)=2147483647,$1=$3.%v32(1);";
// pMcmd=" S $2=641,$1.%v64(1)=$2;";
// pMcmd=" S $1.%v64(1)=$2;";
// pMcmd=" do %winNew();";
// pMcmd=" Do par[1].%iniRead('byxPar.ini');\n Do par[1].%iniRead('byxData.ini');\n Do par[1].%iniRead('byxCfg.ini');";
// pMcmd=" Do pr[1].%iniRead('byxPar.ini','byxData.ini','byxCfg.ini');";
 if (pMcmd==NULL) return;
 TrInit(&Tr);
 bufM.size=strlen(pMcmd);
 bufM.psec=pMcmd;
// memCopy(bcmdM,pMcmd,bufM.size);
// bufM.psec=bcmdM;
// bufM.psec[bufM.size]='\0';
 trCodamshCmdBuf(&Tr,&bufM,NULL);
 vmPrnt(pdiv,"\nИсходный текст %s\n",pMcmd);
 vmprnTtrErr(&(Tr.errTr),pdiv);
 tstvmprnTr(&Tr,pdiv);
 if (Tr.errTr.err==COutKodOk) {
//*
  vmPrnt(pdiv,"\nВыполнение \n");
  if ((err=vmAppInit("Dbg"))<CERR) {
   vmPrnt(pdiv," Ошибка Инициализации=%d\n",err);
   }
  else{
   mshTrRunpDo(&Tr,&App,&pDo,CvmSubCmdDODo,NULL);
   if (Tr.errTr.err>CERR) {
    tstvmprnDo(pDo,NULL,NULL);
    tstvmprnJob(pDo,pdiv);
    tstvmprnApp(pDo,pdiv);
    sleep(1);
    tstvmprnDoDb(pDo,pdiv);
    vmprnTtrErr(&(Tr.errTr),pdiv);
    vmAppFree();
    }
   else{
    vmPrnt(pdiv," Ошибка выполнения =%d\n",Tr.errTr.err);
    }
   }
  }
 TrFree(&Tr);
 //*/
 printf("tstVm test 1\n");
 }
//
void test2() {
 printf("tstVm test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstVm) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstVm\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstVm)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstVm) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstVm)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstVm) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
