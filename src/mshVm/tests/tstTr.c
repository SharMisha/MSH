/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   tstTr.c
 * Author: misha
 *
 * Created on 26 ноября 2018 г., 19:16
 */
#include "mshvmDebag.h"

void trCodamshCmdBuf(Ttr *pTr,Tbstr *pbufPrg,Tstr *pName);
void tstvmprnTr(Ttr *pTr,TdivStream *pdiv);
void TrInit(Ttr *pTr);
void TrFree(Ttr *pTr);
/*
 * Simple C Test Suite
 */

void test1() {
 Ttr Tr;
 TdivStream *pdiv=NULL;
 Tbstr bufM;
 char *pMcmd,bcmdM[1024];
 Terr err;
 int i;
 //
 printf("tstTr test 1\n");
 pMcmd=NULL;
// pMcmd=" S $1={3,2};";
// pMcmd=" S [1]=3+2;";
// pMcmd=" COPY lk[]=$;";
// pMcmd=" S Abc[1]=3;";
// pMcmd=" S [Abc][1]=3;";
// pMcmd=" S [10,20]=1;";
// pMcmd=" S [10:20]=1;";
// pMcmd=" S [10,20]={1,2,3};";
// pMcmd=" S $1=Ab;";
// pMcmd=" S A$1=Ab;";
// pMcmd=" S $1=Ab,[2]=Cd,$abc=123,[gfr]=321;";
// pMcmd=" S %md$1=3;";
// pMcmd=" S ^%d$1=3;";
// pMcmd=" S %d[1]=3;";
// pMcmd=" S %lb[1+4,[7]]=3;";
// pMcmd=" S $1=48/((3*2)+(9-6))/5;";
// pMcmd=" S lb$(m$1)=3;";
// pMcmd=" S lb$m$1=3;";
// pMcmd=" S ^mdbg[7]=Ab;";
// pMcmd=" S ^[7]=Ab;"; //Ошибка -3044
// pMcmd=" S ^%[7]=Ab;";
// pMcmd=" S job$1=3;";
// pMcmd=" S job$$1=3;";
// pMcmd=" S A$$1=3;";
// pMcmd=" S $1=3,$$1=25;";
// pMcmd=" S $1=$;";
// pMcmd=" S ^gb$(2)=32;";
// pMcmd=" S $2=%[7+8];";
// pMcmd=" S $2=gb[7+8];";
// pMcmd=" S $2=%[7,[3+2]];";
// pMcmd=" S $3=Lb;";
// pMcmd=" S $3=.Lb;";
// pMcmd=" S .Lb=3;";
// pMcmd=" S Lb=3;";
// pMcmd=" S $2=md.lb(2);";
// pMcmd=" S $2=md.lb(2,5);";
// pMcmd=" S $2=[2**5,7];";
// pMcmd=" S $2=[[2,5],7+6];";
// pMcmd=" S $2=[$2,7];";
// pMcmd=" S $2=[md.lb(2,5),7];";
// pMcmd=" S $2=[md.lb,1];";
// pMcmd=" S $2=gl[md.lb];";
// pMcmd=" S $2=gl$md.lb;";
// pMcmd=" S $2=gl$(md.lb);";
// pMcmd=" S $3=[md.lb[2,5],[8]];";
// pMcmd=" S $3=[md.lb(2,5),[8]];";
// pMcmd=" S [md.lb(2,5),[8]]=32;";
// pMcmd=" S $5=md.lb([2,9],7+6);";
// pMcmd=" S md.lb[[2,9],7+6]=32;";
// pMcmd=" S md.lb([2,9],7+6)=32;";
// pMcmd=" set $1=%sysReadDir('ini','%[1]');";
// pMcmd=" S %[7]=[Ab].$3_(8+7);";
// pMcmd=" s ^^^$10='%%['_A$2_']';"; //ошибка -3030
// pMcmd=" S $5=md.[lb](2,9);";
// pMcmd=" S $5=[md].(lb_1)(2,9);";
// pMcmd=" s $1=A$1_A$2,$11='%%['_$12_']';";
// pMcmd=" s $1=(,);"; //err
// pMcmd=" S [AB]=[15968];";
// pMcmd=" S %[AB]=%[15968];";
// pMcmd=" S [8,9][A]=25;";
// pMcmd=" KD [7,8];";
// pMcmd=" KDC $5;";
// pMcmd=" KDP Md.Lb(5);";
// pMcmd=" S ^$5[3]=45;"; //err
// pMcmd=" S ldb[$3,2]=5;";
// pMcmd=" S ldb[$3]=5;";
// pMcmd=" S $A[$3]=5;";
// pMcmd=" S $6=A,glb[$3]=5;";
// pMcmd=" S $A=5+(8*3)-4;";
// pMcmd=" S $A=5 _^[4];";
// pMcmd=" S $A=5 _ ^[4];";
// pMcmd=" S %%[3]=2;";
// pMcmd=" S A$$3=2;";
// pMcmd=" S A$B$3=2;";
// pMcmd=" S A$(B$3)=2;";
// pMcmd=" S A$$(B$3)=2;";
// pMcmd=" S %$[3]=2;";
// pMcmd=" S ^[Abc]=7;"; //err
// pMcmd=" S $(4+5)=25";
// pMcmd=" S [(4+5),7]=25";
// pMcmd=" S [4+5,dr]=25";
// pMcmd=" S $2=$5[7];";
// pMcmd=" S $2=md.lb.14;";
// pMcmd=" S $2=45.lb.14;";
// pMcmd=" Const C4='\n',C21='\t';";
// pMcmd=" Const C4=5,C21=Abc;";
// pMcmd=" Const C4,C21;";
// pMcmd=" Const C21=Abc; S $4=C21;";
// pMcmd=" Const C21=45.14; S $4=C21;";
// pMcmd=" Const C21=45.ab.14; S $4=C21;"; //err
// pMcmd=" Const C4=5,C21=Abc; S $C4=C21;";
// pMcmd=" Const C4=5,C21=Abc; S C21$C4=121;";
// pMcmd=" S $4=21,$1=4,$2=$$1;";
// pMcmd=" S $45=($21+5-8)*[A];";
// pMcmd=" S $45=($21+5-8)*9;";
// pMcmd=" S $(A==45)=%$21==8;";
// pMcmd=" S $(A=45)=%$21=8;";
// pMcmd=" S %$45=%$21 , $2=$7 ;";
// pMcmd=" S $2={7,6};";
// pMcmd=" S $2={7,md.lb};";
// pMcmd=" S $2=[8,{7,md.lb},g];";
// pMcmd=" S $2=md.lb[{7,8}];";
// pMcmd=" S [8,{7,6}]=3;";
// pMcmd=" G Mod.Lab;";
// pMcmd=" G Mod.Lab(7,8);";
// pMcmd=" G Mod.Lab.7.a8;";
// pMcmd=" G Mod.Lab.7.'8';";
// pMcmd=" D Lb;";
// pMcmd=" G Lab;";
// pMcmd=" J ($1).[2](1,2);";
// pMcmd=" J $1.[2](1,2);";
// pMcmd=" J $1.^[2].1.a;";
// pMcmd=" Set Md.Lab=8;";
// pMcmd=" Set Md.Lab=8+5;";
// pMcmd=" Set Md.Lab=(8+5);"; //надо так
// pMcmd=" Set Md.Lab()=8;";
// pMcmd=" Set Md.Lb(pr+1)=8;";
// pMcmd=" Set Md.Lab=(8*3);";
// pMcmd=" Set Md.Lab.pr=(8*3);";
// pMcmd=" Set Md.Lab(pr,1)=(8*3);";
// pMcmd=" Set (Md_1).Lab=8;";
// pMcmd=" Set Md.Lab(1,2)=(8*3);";
// pMcmd=" Do Md.Lb.pr.7;";
// pMcmd=" Do Md.Lb(pr.7);";
// pMcmd=" Do [MOD].[LAB](1+5*7,2);";
// pMcmd=" Do [MOD].[LAB]((1+$7)*9,2);";
// pMcmd=" Do LaB();";
// pMcmd=" Do namemod.LB(1,2,3,4);";
// pMcmd=" Do nm.(LB_1)(1,2,3,4);";
// pMcmd="do %win('ini','[1]');";
// pMcmd="do %sysReadDir('ini','[1]');";
// pMcmd="do %sysReadIni($1);";
// pMcmd="s $2=%sysReadIni(%sysReadDir('ini','[1]')_A)+8;";
// pMcmd=" Do (LB_PR)();";
// pMcmd=" Do LB(1,2),m.l(A,s);";
// pMcmd=" Do LB,m.l(A,s);";
// pMcmd=" Do M.LB,m.l(A,s);";
// pMcmd=" Do (MD_7).(LB_8);";
// pMcmd=" Job md.LB(1,2,3,4);";
// pMcmd=" S gl$2=gl;";
// pMcmd=" K Lb;";
// pMcmd=" K gd[]";
// pMcmd=" K ^g$";
// pMcmd=" K gl$2;";
// pMcmd=" K ^g$";
// pMcmd=" K gl[7];";
// pMcmd=" KDC [3],$[2];";
// pMcmd=" KDP Lb;";
// pMcmd=" KDP Md.Lb;";
// pMcmd=" KDP .Lb;";
// pMcmd=" KDP Lb();";
// pMcmd=" KD Lb();";
// pMcmd=" Copy [8,2]=^gb[9];";
// pMcmd=" Copy [8]=^gl[7];";
// pMcmd=" Copy []=^gb[];";
// pMcmd=" Copy %[]=^gb[];";
// pMcmd=" Copy []=%[];";
// pMcmd=" MOVE To[12]=gl[7];";
// pMcmd=" MOVE %[3]=[5];";
// pMcmd=" MOVE %[]=[];";
// pMcmd=" MOVE to[]=from[];";
// pMcmd=" Copy [$3,7]=%[5,$28];";
// pMcmd=" S $1=2?1!0;";
// pMcmd=" S $1=2?1?11!10!0;";
// pMcmd=" S $1=2+3?1!0;";
// pMcmd=" S $1=(2+3)?1!0;";
// pMcmd=" S $15=14?15!B==18?28!17;";
// pMcmd=" S $15=(A==14)?15!B==18?28!17;";
// pMcmd=" S $15=(A==14)?B==18?28!29!17;";
// pMcmd=" S $15=A==14?15+35!B==18?28**38!17/37;";
// pMcmd=" S $15=A==14?B==18?28**38!29+39!17/37;";
// pMcmd=" If?0 S $2=101; E S $2=104; END;";
// pMcmd=" If?$1==25 S $2=101; E?$1==23 ; S $2=103; E S $2=104; END; S $3=33;";
// pMcmd=" If?5; S $1=11; E?3 S $1=13; END;";
// pMcmd=" If?5 Br?7 ; S $1=11; E?3 S $1=13; END;";
// pMcmd=" If?5 If?7 Br?7 S $1=11; END E?3 S $1=13; END;";
// pMcmd=" S $1=18; If?$1==15 S $2=101; E?$1==13 S $2=103; E S $2=104; END S $3=33;";
// pMcmd=" If?$1=15 S $2=101; E?$1=13 S $2=103; E S $2=104; END;"; //ошибка $1=15 знака = быть не может
// pMcmd="\n If?5\n  If?50 S $1=11;\n  E S $51=151;\n  END;\n E?3 S $1=13;\n END;";
// pMcmd="\n If?5\n  If?50 Br?7 S $1=11;\n  E S $51=151;\n  END;\n E?3 S $1=13;\n END;";
 pMcmd="S $1=B; CASE?A S $2=4 ; \nA: S $2=1 ; \nB: S $2=2;\nC: S $2=3;\n endCase;";
// pMcmd=" S $1=2;\n G?1 A;";
// pMcmd="CASE?$1;\n  S $2=4;\nA: S $2=1;\nB: S $2=2;\nC: S $2=3;\n endCase;";
// pMcmd="\n CASE?$1;\nA: S $2=1;\nB: S $2=2;\nC: S $2=3;\n endCase;";
// pMcmd="\n CASE?$1\n  S $2=4;\nA: S $2=1;\nB: S $2=2;\nC: S $2=3;\n endCase;";
// pMcmd="\n CASE?$1;\n  S $2=4;\nA: Br?7 S $2=1;\nB: S $2=2;\nC: S $2=3;\n endCase;";
// pMcmd=" CASE?$1;\n  S $2=4;\n A1: S $10=B;\n B1: CASE?$21;\n  A1: S $22=1;\n  B1: S $24=2;\n  endCASE;\n C1: S $11=3;\n endCASE;";
// pMcmd=" CASE?$1;\n  S $2=4;\n A1: S $10=B;\n B1: CASE?$21;\n  A1: S $22=1;\n  B1: S $24=2;\n  endCASE ; \n C1: S $11=3;\n endCASE;";
// pMcmd="CASE?$1 \n  S $2=4; \n A1: S $10=B;\n B1: CASE?$21;\n  A1: Br?7 S $22=1;\n  B1: S $24=2;\n  endCASE;\n C1: S $11=3;\n endCASE;";
// pMcmd=" CASE?$1;\n  S $2=4;\nA1: Br?8; S $10=B;\n B1: CASE?$21;\n  A1: Br?7 S $22=1;\n  B1: S $24=2;\n  endCASE;\n C1: S $11=3;\n endCASE;";
// pMcmd="S $1=C1,$4=C2;\nCASE?$1;\nA1: S $2=20;\nB1: CASE?$4;\nA2: Br?7 \nB2: S $5=41;\n endCASE;\nC1: S $2=22;\n endCASE;\n S $3=7;\n";
// pMcmd="\n CASE?1;\n  If?2 S $2=4;Br?8\n  E S $2=14;\n  endIf\n  S $2=24;\n A1: S $10=B;\n B1: CASE?$21;\n  A2: S $22=1;\n  B2: S $24=2;\n  endCASE;\n  S $24=45; \n C1: S $11=3;\n endCASE;";
// pMcmd="\n CASE?1;\n  If?2 S $2=4;\n  E S $2=14;Br?8\n  endIf\n  S $2=24;\n A1: S $10=B;\n B1: CASE?$21;\n  A2: S $22=1;\n  B2: S $24=2;\n  endCASE;\n  S $24=45; \n C1: S $11=3;\n endCASE;";
// pMcmd="\n CASE?1\n  If?2 S $2=4;\n   If?4 Br?1 \n   E S $2=104; \n   endIf;\n  E S $2=14;Br?8\n  endIf\n  S $2=24;\n A1: S $10=B;\n B1: CASE?$21;\n  A2: S $22=1;\n  B2: S $24=2;\n  endCASE;\n  S $24=45; \n C1: S $11=3;\n endCASE;";
// pMcmd=" CASE?1;\n  If?2 S $2=4;\n  E S $2=14;\n A1: S $10=B;\n B1: CASE?$21;\n  A2: S $22=1;\n  B2: S $24=2;\n  endCASE;\n C1: S $11=3;\n endCASE;";
// pMcmd=" LU $2_5;";
// pMcmd=" LW Nm;";
// pMcmd=" TO 200;";
// pMcmd=" TO $4 + [7,2] ** $6 - 9;";
// pMcmd=" EvC ev=Md.Lb;";
// pMcmd=" EvC ev=Md.Lb(1503);";
// pMcmd=" EvC ev=Md.(Lb_1);";
// pMcmd=" EvC ev=Md.Lb_1;"; //допустимая трактовка
// pMcmd=" EvC ev=Md.(Lb_1)(1503);";
// pMcmd=" EvC ev_8=Lb_7;";
// pMcmd=" EvC ev=Lb_1;";
// pMcmd=" EvC ev=(Lb_1)(1503,25);";
// pMcmd=" EvC ev=Lb;";
// pMcmd=" EvC ev=Lb(1503,25);";
// pMcmd=" EvC ev_1=(Md_2).(Lb_3);";
// pMcmd=" EvC [ev]=[Md].$2;";
// pMcmd=" Do Lb([1]);";
// pMcmd=" Do Lb();";
// pMcmd=" Do Lb;";
// pMcmd=" Do (Lb_7)();";
// pMcmd=" Do ev(A,2);";
// pMcmd=" S $7=70+(8+(1*6));";
// pMcmd=" Go Lb_7;";
// pMcmd=" EvC (Lb_7);"; //должна быть ошибка
// pMcmd=" EvC md.lb=Md.Lb;";
// pMcmd=" EvC ev=Md.Lb;";
// pMcmd=" EvC ev_8=Lb_7;";
// pMcmd=" EvT ev(A,2);";
// pMcmd=" EvT md.ev(2);"; //неверно md-будет проигнорировано
// pMcmd=" EvT (md.ev)(2);";
// pMcmd=" EvT (md.ev)(2,5);";
// pMcmd=" EvT ev_1;";
// pMcmd=" EvT ev;";
// pMcmd=" EvT ev[A,54];";
// pMcmd=" EvW ev;";
// pMcmd=" EvW ev(1503,5);";
// pMcmd=" EvW ev[A];";
// pMcmd=" EvW ev(A);";
// pMcmd=" Ret $2;";
// pMcmd=" Ret 25;";
// pMcmd=" Ret;";
// pMcmd=" S?5 $15=17;";
// pMcmd=" S $2=$1+100;";
// pMcmd=" S $$1=$1+100;";
// pMcmd=" S $1=10; While?$1<19 ; S $$1=$1+100; S $1=$1+1; END;";
// pMcmd=" S $1=10; While?$1<19 ; S $$1=$1+100; Br?7 S $1=$1+1; END;";
// pMcmd=" S $1=10; While?$1<19 S $$1=$1+100; END;";
// pMcmd=" S $1=10; While ; S $$1=$1+100; END?$1>19;";
// pMcmd=" S $1=0; While?$1<9 ; S $2=A; END?$1<10;";
// pMcmd=" Next1 [1],$2;";
// pMcmd=" S $2=1; Next1 ^gb[$2],$2;";
// pMcmd=" Next [1+2],$(2+5); S $2=3; END;";
// pMcmd=" Next [1+2],$2;\n Break?1;\n S $2=3;\n END;";
// pMcmd=" Next1 lb[D];"; //ошибка нет индекса сохранения
// pMcmd=" Next1?8 [D,2],[1,3];";
// pMcmd=" Next1 (gl_1)[D,2],[1,3];";
// pMcmd=" Next?5 [1]; S $2=3; END;";
// pMcmd=" Next?5 [1]; Br?8 S $2=3; END;";
//pMcmd=" Next ^d1[],$4; Break?($4.%fieldGet(CsysfrDate,1,2) ~= $3); End;";
// pMcmd=" S $2=1; Next ^gb[],$1; S $2=$2+1,$$2=$1; END;";
// pMcmd=" S $2=2; Next ^gb[],$$2; S $2=$2+1; END;";
// pMcmd=" Query [1],[3]; S $2=3; END;";
// pMcmd=" Query1 [1],[3];";
// pMcmd=" S [2].PropUsr=Value;";
// pMcmd=" S [2].%type=Ur;"; //???
// pMcmd=" S [2,5].%type=Ur;"; //???
// pMcmd=" S [2,5].%type=Ur_1;";
// pMcmd=" S $3=[2,5].%type;";
// pMcmd=" S $3=[2].%size;";
// pMcmd=" Do [2].%iniRead(fl);";
// pMcmd=" S $2=$2.%strGet(5,6);";
// pMcmd=" Do [2].%winNew('%[2]');";
// pMcmd=" Do winObj.New(ar$);";
// pMcmd=" S $2=%strGet(5);"; //должна быть ошибка
// pMcmd=" S $2=[$2.%strGet(5),8];";
// pMcmd=" S $2=[$2.%strGet(5)];";
// pMcmd=" S $2=[2,5].%strGet(5,3);";
// pMcmd=" S [2]=%statData;";
// pMcmd=" S $2=%statData;";
// pMcmd=" S [%this,8,5]=3;";
// pMcmd=" S [%this]=3;";
// pMcmd=" S [5,3]=8,$2=[5],$1=%statData;";
// pMcmd=" S [5,3]=538,[5]=58,$2=[5].%stat;";
// pMcmd=" Set ^win[1,1,3,1,2].%objNew=winScroll;";
// pMcmd=" S [5,3]='0123456789',$2=[5,3].%strGet(2);";
// pMcmd=" S $3='1234567890',$2=$3.%strGet(2,3);";
// pMcmd=" S $3='1234567890',$2=$3.%strCode(2);";
// pMcmd=" S $3='1234567890',$2=$3.%strPop(2);";
// pMcmd=" S $3='1234567890',$3.%strIns(2)='AB';";
// pMcmd=" S $3='1234567890',$2=$3.%strIns(2,AB);";
// pMcmd=" S $3='1234567890',$3.%strSet(2,3)='AB';";
// pMcmd=" S $3='1234567890',$2=$3.%strDel(2,3);";
// pMcmd=" S $3='1234567890',$2=$3.%strFind(78,2);";
// pMcmd=" S $3='1234567890',$2=$3.%strRepl(78,ABC);";
// pMcmd=" S $3=^gb$3,$2=$3.%fieldPop(',');";
// pMcmd=" S $1=$2.%size;";
// pMcmd=" S 1+$1,$2=A;";
// pMcmd=" Const C1=19; Set 1+$1,B$$1=(($1>2)?C1!'') _ %Key _ C1 _ $4 _ C1 _ %Data;";
// pMcmd=" S [2:8,5]=3;";
// pMcmd=" S $2=3\n,$1=5;\n";
// pMcmd=" Set?($2~='' _& ($1~='')) [%this,$2]=$1;";
// pMcmd=" S [2:]=3;";
// pMcmd=" S []=3;";
// pMcmd=" S [%this:5]=3;";
// pMcmd=" W fl;";
// pMcmd=" W 'fl/fldir/fl.txt'(a1,a2);";
// pMcmd=" W 'fl/fldir/fl.txt'(a1,a2);";
// pMcmd=" W 'fl/fldir/fl.txt';";
// pMcmd=" W ('fl/fldir/'_'fl.txt');";
// pMcmd=" W 'fl/fldir/'_'fl.txt';";
// pMcmd=" W $ac(a1,a2);";
// pMcmd=" W $1(a1,a2);";
// pMcmd=" W ($1)(a1,a2);";
// pMcmd=" W 'fl/fldir/fl.txt';";
// pMcmd=" Rd 'fl/fldir/fl.txt';";
// pMcmd=" While?$2<$5;";
// pMcmd=" Set $3+1;";
// pMcmd=" KillAll ^gb;";
// pMcmd=" Write OUT;";
// pMcmd=" S $1=$3$2;";
// pMcmd=" S $1=[$3,$2];";
// pMcmd=" S $1=[$3:$2];";
// pMcmd=" S $1=[$3[$2]];";
// pMcmd=" S $1=$3?$2!$1;";
// pMcmd=" S $2=2,$1=3; Next ^gb[],$1; S $2=$2+1,$$2=$1; END?$1>7;";
// pMcmd=" D [5].%objNew(objUsr);";
// pMcmd=" Set $1=Md.Lab.%findLabel;";
// pMcmd=" If?cm$3~='';\n Kill us[cm$3];\n While?(cm$1~='');\n Set $2=cm$1.%fieldPop(CsysfrPole),$3=$2.%fieldPop(CsysfrCom);\n Set?$3~='' us[cm$3,$3]=$2; Endwhile; EndIf;";
// pMcmd=" S $1=$3`i2;";
// pMcmd=" S $3`i2=$1;";
// pMcmd=" S $1=$3.%v32(1);";
// pMcmd=" S $1.%v64(1)=641;";
// pMcmd="Do %winNew(CwinTypeWinBox,030box,,CwinAtrPosV,CwinBoxStart,4);";

 if (pMcmd) {
  TrInit(&Tr);
  bufM.size=strlen(pMcmd);
  memCopy(bcmdM,pMcmd,bufM.size);
  bufM.psec=bcmdM;
  trCodamshCmdBuf(&Tr,&bufM,NULL);
  vmPrnt(pdiv,"\nИсходный текст \n%s\n\n err=%d offStr=%d offPos=%d \n",pMcmd,Tr.errTr.err,Tr.errTr.numstr,Tr.errTr.numpos);
  tstvmprnTr(&Tr,pdiv);
  TrFree(&Tr);
  }
 printf("tstTr test 1\n");
 }
//
void test2() {
 printf("tstTr test 2\n");
 printf("%%TEST_FAILED%% time=0 testname=test2 (tstTr) message=error message sample\n");
}

int main(int argc, char** argv) {
 printf("%%SUITE_STARTING%% tstTr\n");
 printf("%%SUITE_STARTED%%\n");

 printf("%%TEST_STARTED%% test1 (tstTr)\n");
 test1();
 printf("%%TEST_FINISHED%% time=0 test1 (tstTr) \n");
/*
 printf("%%TEST_STARTED%% test2 (tstTr)\n");
 test2();
 printf("%%TEST_FINISHED%% time=0 test2 (tstTr) \n");
*/
 printf("%%SUITE_FINISHED%% time=0\n");

 return (EXIT_SUCCESS);
}
