/*
 * File:   mshVmDebag.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 17 Сентябрь 2014 г., 9:09
 */
#include "mshvmDebag.h"
#include "mshvmTr.h"
#include "mshvmMacro.h"

Tstr* mshpAreaNextpStr(Tarea *pArea,Tstr *pstr);
void vmtstWriteDbArr(TDb *pDb,TdivStream *pdiv);
void vmtstWriteStr(Tstr* pstr,TdivStream *pdiv);
void vmtstWriteData(Tdata *pData,TdivStream *pdiv);
void vmtstWriteKeyData(Tval *pVal,TdivStream *pdiv);
void vmtstWriteArrData(TarrData *pArr,TdivStream *pdiv);
void vmtstWriteArrInt(TarrInt *pArr,TdivStream *pdiv);
void vmtstWriteArrpVoid(TarrpVoid *pArr,TdivStream *pdiv);
void vmtstWriteAreaInt(Tarea *parea,TdivStream *pdiv);
void vmtstWriteQuery(TIterQuery *pcurQuery,TdivStream *pdiv);
void vmtstWriteTree(TDb *pDb,TdivStream *pdiv);
void vmtstWriteArrInt(TarrInt *pArr,TdivStream *pdiv);
void vmtstWriteBuf(char *pC,int len,TdivStream *pdiv);
void vmtstWriteSecLongs(TmapLong *pmapLong,TdivStream *pdiv);
int cmbFindInStr(const char *pStrCnst,char C);

#define vmtstWriteStrNoSize(pstr,pdiv) ({ char *_pC=(pstr)->psec,*_pE=_pC+(pstr)->size;\
 for (;_pC<_pE;_pC++) {vmPrnt(pdiv,"%c",*_pC); } })

void vmprnTtrErr(TerrTr *perr,TdivStream *pdiv) {
 vmPrnt(pdiv,"err=%d #str=%d #pos=%d",perr->err,perr->numstr,perr->numpos);
 vmPrntBK(pdiv);
 }
static void vmprnArrStr(Tstr *parrStr,int Size,TdivStream *pdiv) {
 int j;
 for (j=0;j<Size;j++){
  vmtstWriteStr(parrStr + j,pdiv);
  }
 }

/*
 * рапспечатка заголовка
 * TvmMod *pmod
void vmprnModHead(Tdata *phead,TdivOut *pdiv){
 vmPrnt(pdiv,"__Заголовок модуля___");
 vmPrntBK(pdiv);
 vmPrnt(pdiv," Общая длинна %d Количество секций %d Расширения %d Константы %d Метки %d Команды %d Область %d"
         ,phead->aunum[CvmPackSize],phead->aunum[CvmPackSum]
         ,phead->aunum[2]/sizeof(Tdata),phead->aunum[3]/sizeof(Tdata),phead->aunum[4]/sizeof(TvmLabel)
         ,phead->aunum[5]/sizeof(TvmCmd),phead->aunum[6]/sizeof(Tdata));
 vmPrntBK(pdiv);
 }
*/
void vmprnModHead(TvmMod *pmod,TdivStream *pdiv){
 vmPrnt(pdiv,"__Заголовок модуля___");
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"Расширения %d [%p] Константы %d [%p] Метки %d [%p] Команды %d [%p] Область %d [%p]"
         ,pmod->sumext,pmod->pext
         ,pmod->sumdata,pmod->pdata
         ,pmod->labels.busy,pmod->labels.psec
         ,pmod->sumcmd,pmod->pbegcmds
         ,pmod->areaString.busy,pmod->areaString.psec
         );
 vmPrntBK(pdiv);
 }
/*
 * Распечатка Раздела Констант
 */
void vmprnDatas(TDb *pDb,Tdata *pData,Tsize size,TdivStream *pdiv){
 int j;
 for (j=0;j<size;j++,pData++) {
  vmPrnt(pdiv,"%6d ",j);
  vmtstWriteData(pData,pdiv);
//  if (( j % 5 == 0)&& j)
   vmPrntBK(pdiv);
  }
 }
/*
 * Распечатка Раздела Меток
 */
static void vmprnLabel(TLabel *plabel,TdivStream *pdiv){
 ToffMod j;
 for (j=0;j<plabel->label.size;j++){
  vmPrnt(pdiv,"%c",plabel->label.psec[j]);
  }
 vmPrnt(pdiv," ");
 if (plabel->label.size < 8)
  for (j=plabel->label.size;j<9;j++){
   vmPrnt(pdiv," ");
   }
 }
/*
*/
static void vmprnFindLabel(Tarr2Label *parr2Label,Tsize off,TdivStream *pdiv){
 TLabel *plabel=NULL;
 TarrTLabel *psecLabel=NULL;
 //
 parrIter(parr2Label,psecLabel){
  parrIter(psecLabel,plabel){
   if (plabel->off==off) {
    vmprnLabel(plabel,pdiv);
    return;
    }
   }
  }
 vmPrnt(pdiv,"%10c ",' ');
 }
 /**/
void vmprnarrLabel(Tarr2Label *parr2Label,TdivStream *pdiv){
 TLabel *plabel;
 TarrTLabel *psecLabel=NULL;
 u_int i=0;
 //
 vmPrntBK(pdiv);
 if (parr2Label->busy==0) return;
 parrIter(parr2Label,psecLabel){
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"---Раздел Меток--# %d",i);
  parrIter(psecLabel,plabel){
   vmPrntBK(pdiv);
   vmprnLabel(plabel,pdiv);
   vmPrnt(pdiv," %u",plabel->off);
   }
  i++;
  }
 vmPrntBK(pdiv);
 }
/**/
/*
void vmprnArea(Tsize *parea,Tsize size,TdivStream *pdiv){
 char *p;
 //
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"Раздел Длинных строк");
 vmPrntBK(pdiv);
 p=(char*)parea;
 *(parea + size)=0;
 vmPrnt(pdiv,"%s",p);
 vmPrntBK(pdiv);
 }
*/
/**/
void vmprnAreaIter(Tarea *pArea,TdivStream *pdiv){
 Tstr *p,*pE;
 //
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"Раздел Длинных строк");
 vmPrntBK(pdiv);
 if (pArea->busy){
  pE=(Tstr*)(pareaTop(pArea));
  for(p=(Tstr*)(pArea->psec);p<pE;p=pStrnextpStr(p)){
   vmtstWriteStr(p,pdiv);
   vmPrntBK(pdiv);
  }
 }
 }
/**/
void vmtstWriteArrInds(TstkInd *pInds,TdivStream *pdiv){
 TarrInt *parrInt;
 int c0,c1;
 int j0;
 //
 c0=c1=0;
 vmPrnt(pdiv,"ind_core ");
 vmtstWriteArrHead(&(pInds->_core),pdiv);
 for(j0=0;j0<pInds->_core.busy;){
  vmPrnt(pdiv,"%5i ",j0);
  vmtstWriteData(arrOffpD(pInds->_core,j0),pdiv);
  if ( (pInds->_ct0.busy) &&(c0<pInds->_ct0.busy) &&(j0>=pInds->_ct0.psec[c0]) ) c0++;
  vmPrnt(pdiv,"  %i",c0);
  if ( (pInds->_ct1.busy)&&(c1<pInds->_ct1.busy)&&(c0>=pInds->_ct1.psec[c1]) ) c1++;
  vmPrnt(pdiv,"  %i",c1);
  vmPrntBK(pdiv);
  ++j0;
  }
 parrInt=(TarrInt*)(&(pInds->_ct0));
 vmPrnt(pdiv,"ind_ct0 ");
 vmtstWriteArrInt(parrInt,pdiv);
 vmPrnt(pdiv,"ind_ct1 ");
 parrInt=(TarrInt*)(&(pInds->_ct1));
 vmtstWriteArrInt(parrInt,pdiv);
 }

static void tstprnRegArg(TCmd *pCmd,TdivStream *pdiv){
 vmPrnt(pdiv," %5i",pCmdArg(pCmd));
 }
//
#define tstprnRegIst(Sub,par) par=regIst(Sub); par=((par==CregInd)?'i':(par==CregMod)?'m':(par==CregCom)?'c':(par==CregComChar)?'C':(par==CregNULL)?'0':' ')
/*
*/
static void tstprnCmdSub(TCmd *pCmd,TdivStream *pdiv){
 u_char st;
 //
 tstprnRegIst(pCmdSub(pCmd),st);
 vmPrnt(pdiv,"%c",st);
 if ((regIstCmd(pCmd))==CregComChar){
  if (pCmdi(pCmd,2)) {vmPrnt(pdiv,"  %c",pCmdi(pCmd,2));};
  if (pCmdi(pCmd,3)) {vmPrnt(pdiv,"%c",pCmdi(pCmd,3));};
  }
 else{
  tstprnRegArg(pCmd,pdiv);
  }
 }

static void tstprnRegSub(TCmd *pCmd,TdivStream *pdiv){
 u_char Out[]="  ",par;
 u_char Sub;
 //
 Sub=pCmdSub(pCmd);
 par=Sub & CvmSubCmdRegMask;
 Out[0]=(par==CvmSubCmdRegUp)?'u':(par==CvmSubCmdRegMove)?'m':(par==CvmSubCmdRegPop)?'p':(par==CvmSubCmdRegAlt)?':':' ';
 par=styleGet(Sub);
 Out[1]=(par==CstyleIndInd)?'I':(par==CstyleIndArr)?'A':(par==CstyleIndMod)?'M':' ';
 vmPrnt(pdiv," %c%c",Out[0],Out[1] );
 tstprnCmdSub(pCmd,pdiv);
 }
/*
*/
static void tstprnCmdNext(TCmd *pCmd,TdivStream *pdiv) {
 u_char st;
 //
 st=pCmdCmdSubNext(pCmd);
 vmPrnt(pdiv," %c",st==CvmCmdSubNext1?'1':st==CvmCmdSubInit?'I':st==CvmCmdSubNext?'N':' ');
 if (pCmdCmdSubNextSave(pCmd)) {vmPrnt(pdiv,"s");}
 tstprnRegArg(pCmd,pdiv);
 }
/**/
static void tstvmprnarrCmd1(TCmd *pCmd,TdivStream *pdiv){
 u_char st;
 //код Команды
 switch (pCmdCmd(pCmd)) {
  case CvmCmdKiLL : vmPrnt(pdiv," Kill  ");
   st=pCmdSub(pCmd);
   if (st & CtypeKiLL) {
    vmPrnt(pdiv,"%c",CtypesKiLL);
    }
   else{
    if (st & CtypeKiLLData) {vmPrnt(pdiv,"%c",CtypesKiLLData);}
    if (st & CtypeKiLLDown) {vmPrnt(pdiv,"%c",CtypesKiLLDown);}
    if (st & CtypeKiLLAlt) {vmPrnt(pdiv,"%c",CtypesKiLLAlt);}
    if (st & CtypeKiLLAll) {vmPrnt(pdiv,"%c",CtypesKiLLAll);}
    }
   break;
  case CvmCmdGet : vmPrnt(pdiv," Get   ");
   if (pCmdSub(pCmd)>' '){
    vmPrnt(pdiv,"%c",pCmdSub(pCmd));
    }
   else{
    vmPrnt(pdiv," %i",pCmdSub(pCmd));
    }
   break;
  case CvmCmdSet : vmPrnt(pdiv," Set   ");
   tstprnCmdSub(pCmd,pdiv);
   break;
  case CvmCmdSetSh : vmPrnt(pdiv," setShort   ");
   tstprnCmdSub(pCmd,pdiv);
   break;
  case CcmdNext : vmPrnt(pdiv," Next  "); tstprnCmdNext(pCmd,pdiv);  break;
  case CcmdBack : vmPrnt(pdiv," Back  "); tstprnCmdNext(pCmd,pdiv);  break;
  case CcmdQuery : vmPrnt(pdiv," Query "); tstprnCmdNext(pCmd,pdiv); break;
  case CvmCmdCopy : vmPrnt(pdiv," Copy  ");
   vmprnMarrCmdSub(pCmd,pdiv); vmPrnt(pdiv,"  ");
   break;
  case CvmCmdReg : vmPrnt(pdiv," Registr ");
   tstprnRegSub(pCmd,pdiv);
   break;
  case CvmCmdLoadConst : vmPrnt(pdiv," CnstLoad ");
   tstprnCmdSub(pCmd,pdiv);
   break;
  case CvmCmdCase : vmPrnt(pdiv," Case  0x%x ",pCmdSub(pCmd));
   tstprnRegArg(pCmd,pdiv);
   break;
  case CvmCmdLock :
   st=cmdSubpCmdLock(pCmd);
   vmPrnt(pdiv," Lock  %c",st==CvmSubCmdLockW?'W':st==CvmSubCmdLockR?'R':'U' );
   tstprnCmdSub(pCmd,pdiv);
   break;
  case CvmCmdReturn : vmPrnt(pdiv," Return  ");
   tstprnCmdSub(pCmd,pdiv);
   break;
  case CvmCmdTimeOut : vmPrnt(pdiv," TimeOut ");
   tstprnCmdSub(pCmd,pdiv);
   break;
  case CvmCmdEvent : vmPrnt(pdiv," Event ");
   st=cmdSubpCmdEvent(pCmd);
   vmPrnt(pdiv,"%c",st==CvmSubCmdEventCall?'C':st==CvmSubCmdEventWait?'W':st==CvmSubCmdEventTrap?'T':st==CvmSubCmdEventDelete?'D':' ' );
   tstprnCmdSub(pCmd,pdiv);
   break;
  case CvmCmdDO : vmPrnt(pdiv," DO    0x%x ",pCmdSub(pCmd));
   tstprnRegArg(pCmd,pdiv);
   break;
  case CvmCmdGo : vmPrnt(pdiv," Go    0x%x ",pCmdSub(pCmd));
   tstprnRegArg(pCmd,pdiv);
   break;
  case CvmCmdJmp : vmPrnt(pdiv," jmp  %c ", (pCmdSub(pCmd)==CvmJmp0)?'0':(pCmdSub(pCmd)==CvmJmp1)?'1':'*' );
   tstprnRegArg(pCmd,pdiv);
   break;
  case CvmCmdFile :
   st=CcmdSubpCmdFile(pCmd);
   vmPrnt( pdiv," File  ");
   vmPrnt(pdiv,"%c",st==CvmSubCmdOpen?'O':st==CvmSubCmdNew?'N':st==CvmSubCmdClose?'C':st==CvmSubCmdUse?'U'
           :st==CvmSubCmdWrite?'W':st==CvmSubCmdRead?'R':st==CvmSubCmdWWW?'w':' ');
   tstprnCmdSub(pCmd,pdiv);
   break;
  //отладочные команды маркеры
  case CvmCmdMarker :
   vmPrnt(pdiv,"Marker  %i ",pCmdSub(pCmd));
   tstprnRegArg(pCmd,pdiv);
   break;
  default:
   if (smbIsOp(pCmdCmd(pCmd))){
    vmPrnt(pdiv," %c",pCmdCmd(pCmd));
    if (pCmdSub(pCmd)>' '){
     vmPrnt(pdiv,"%c",pCmdSub(pCmd));
     }
    break;
    }
   vmPrnt(pdiv," %c     %i",pCmdCmd(pCmd),pCmdSub(pCmd));
   tstprnRegArg(pCmd,pdiv);
   break;
  }
 vmPrntBK(pdiv);
 }
/**/
static void vmprnarrCmdMod(TCmd *parrvmCmd,Tsize size,Tarr2Label *parr2Label,TdivStream *pdiv){
 TCmd *pvmCmd;
 Tsize off;
 //
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"Раздел Команд");
 vmPrntBK(pdiv);
 for (off=0;off < size;off++) {
  pvmCmd=parrvmCmd+off;
  vmPrnt(pdiv,"%6d ",off);
  //вывести Метку
  vmprnFindLabel(parr2Label,off,pdiv);
  //значение аргумента команды
  tstvmprnarrCmd1(pvmCmd,pdiv);
  }
 vmPrntBK(pdiv);
 }
/*
 */
static void vmprnarrCmd(TCmd *parrvmCmd,Tsize size,Tarr2Label *parr2Label,TdivStream *pdiv){
 TCmd *pvmCmd;
 Tsize off;
 //
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"Раздел Команд");
 vmPrntBK(pdiv);
 for (off=0;off < size;off++) {
  pvmCmd=parrvmCmd+off;
  vmPrnt(pdiv,"%6d ",off);
  //вывести Метку
  vmprnFindLabel(parr2Label,off,pdiv);
  //значение аргумента команды
  tstvmprnarrCmd1(pvmCmd,pdiv);
  }
 vmPrntBK(pdiv);
 }
/*
 * Распечатка Модуля
 */
void tstvmprnMod(TvmMod *pmod,TdivStream *pdiv){
 vmPrnt(pdiv,"__Модуль_");
 if (pmod->name.size) vmtstWriteStr(&(pmod->name),pdiv);
 vmPrnt(pdiv,"___");
 vmPrntBK(pdiv);
 vmprnModHead(pmod,pdiv);
 if (pmod->sumext) {
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"Раздел Модулей расширения");
  vmPrntBK(pdiv);
  vmprnArrStr(pmod->pext,pmod->sumext,pdiv);
  }
 if (pmod->sumdata) {
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"Раздел Констант");
  vmPrntBK(pdiv);
  vmprnDatas(NULL,pmod->pdata,pmod->sumdata,pdiv);
  }
 //Раздел Меток
 vmprnarrLabel(&(pmod->labels),pdiv);
 //Раздел Команд
 if (pmod->sumcmd) {
  vmprnarrCmdMod(pmod->pbegcmds,pmod->sumcmd,&(pmod->labels),pdiv);
  }
 if (pmod->areaString.busy) {
  if (pmod->areaString.busy >= pmod->areaString.size){
   areaWide(pmod->areaString, pmod->areaString.busy + 1 );
   }
  vmprnAreaIter( &(pmod->areaString),pdiv);
  }
 }
//
void tstvmprnBoxcurQuery(TBoxIterQuery *pBox,TdivStream *pdiv){
 vmprnBoxHead(pBox,pdiv);
 if (pBox->ptop!=NULL) {vmtstWriteQuery((TIterQuery*)(pBox->ptop),pdiv);}
 }
/*
static void tstvprnDb0(TDb *pDb,TdivStream *pdiv){
 if (pDb->mpg.div>0) {
  vmPrntBK(pdiv);
  vmtstWriteTree(pDb,pdiv);
  }
 if (pDb->mpg.div>0) {
 vmPrnt(pdiv,"   ------Массив-----");
  vmtstWriteDbArr(pDb,pdiv);
  }
 if (pDb->mlong.div>0) vmtstWriteSecLongs(&(pDb->mlong),pdiv);
 }
*/
//
void tstvmprnDb(TvmDb *pDb,char *psname,TdivStream *pdiv){
 char sbuf[64],*psbuf;
 //
 if (pDb->name.size){
  psbuf=sbuf;
  memCopy(psbuf,pDb->name.psec,pDb->name.size);
  sbuf[pDb->name.size]='\0';
  }
 else
  psbuf=psname;
 vmPrntBK(pdiv);
 if (pDb->db.mpg.div>0) {
  if (pDb->db.typeDb==EtypeDb){ 
   vmPrnt(pdiv,"-----Дерево=%s plock=%p",psbuf,pDb->plock);
   vmPrntBK(pdiv);
   vmtstWriteTree((TDb*)(&(pDb->db)),pdiv);
   }
  else{
   vmPrnt(pdiv,"   ------Массив=%s plock=%p",psbuf,pDb->plock);
   vmPrntBK(pdiv);
   vmtstWriteDbArr(&(pDb->db),pdiv);
   }
  }
 if (pDb->db.mlong.div>0) vmtstWriteSecLongs(&(pDb->db.mlong),pdiv);
 }
//
typedef struct {
 int iD; //идентификатор
 uint isbusy; //свободно=0 или используется задание=1
 } TiDisBusy;
 
typedef struct {
 _contReg(TiDisBusy*);
 } TarrpiDisBusy;
 
 
typedef struct {
 Tstr name;
 } TstrName;
 
typedef struct {
 _contArr(TstrName*);
 } TarrpstrName;
 
typedef struct {
 _contReg(TstrName*);
 } TsecpstrName;
/*
 void vmtstWriteArrstrName(TarrpstrName *pArr,TdivStream *pdiv){
  if ((pArr==NULL)||(parrIsNull(pArr))) return;
  TstrName **ppsec;
   vmPrntBK(pdiv);
  parrIter(pArr,ppsec) {
   vmPrnt(pdiv," %p Name=",*ppsec);
   vmtstWriteStrNoSize(&((*ppsec)->name),pdiv);
   vmPrntBK(pdiv);
   }
  }
*/
//
static void tstvmprnarrDb(TsecsvmDb *psecDb,TdivStream *pdiv){
 TvmDb *pvmDb;
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"----secDbD---- %p",psecDb);
 RegIter(psecDb->regDb,pvmDb){
  tstvmprnDb(pvmDb,NULL,pdiv);
  }
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"----secDbA---- %p",psecDb);
 RegIter(psecDb->regArr,pvmDb){
  tstvmprnDb(pvmDb,NULL,pdiv);
  }
 vmPrntBK(pdiv);
 }
/*
 TLock lock;
 Tsize ijob;
 Tstr name;
 */
static void tstvmprnLock(TctrlLock *pLock, TdivStream *pdiv){
 vmPrnt(pdiv,"pLock=%p ijob=%i pLock->name",pLock,pLock->ijob );
 vmtstWriteStr(&(pLock->name),pdiv);
 vmPrntBK(pdiv);
 }
static void tstvmprnarrLock(TvmDo *pDo,TdivStream *pdiv){
 TctrlLock *pLock;
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"----Блокировки Приложения----");
 vmPrntBK(pdiv);
 vmtstWriteSecsHead( &(App.Locks),pdiv);
 RegIter(App.Locks,pLock)
  tstvmprnLock(pLock,pdiv);
 }
/*
typedef struct {
 TarrWait arrWaitEvn; //массив ссылок на программу обработки ожидания события
 TarrrefProg arrCallEvn; //массив ссылок на CallBack программы обработки события
 TarrData args; //аргументы события
 Tstr name; //имя события
 pthread_mutex_t lock; //управление ожидающими событиями
 pthread_cond_t wait; //
 char stat; //состояние события
 } TvmEvent;
typedef struct {
 TvmMod *pmod;
 ToffMod off;
 Tstr namemod;
 Tstr label;
 } TrefProg;
 */
static void tstvmprnEvent(TEvent *pEvent,TdivStream *pdiv){
 TvmDo **ppDo;
 TrefProg *prefProg;
 //
 vmPrnt(pdiv,"pEvent=%p name=",pEvent);
 vmtstWriteStr(&(pEvent->name),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"----arrdataArg----");
 if (pEvent->args.busy)
  vmtstWriteArrData(&(pEvent->args),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"----WaitEvent----");
 vmtstWriteArrHead( &(pEvent->arrwaitEvn),pdiv);
 vmPrntBK(pdiv);
 arrIter(pEvent->arrwaitEvn,ppDo) {vmPrnt(pdiv,"%i",(*ppDo)->iD);}
 //
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"----CallEvent----");
 vmtstWriteArrHead( &(pEvent->arrCallEvn),pdiv);
 vmPrntBK(pdiv);
 arrIter(pEvent->arrCallEvn,prefProg) {
  vmPrnt(pdiv,"mod{");
  vmtstWriteStr(&(prefProg->pmod->name),pdiv);
  vmPrnt(pdiv,"} label{");
  vmtstWriteStr(&(prefProg->label),pdiv);
  vmPrnt(pdiv,"} pMod=%p offMod=%u ",prefProg->pmod,prefProg->off);
  vmPrntBK(pdiv);
  }
 
 }
static void tstvmprnarrEvent(TvmDo *pDo,TdivStream *pdiv){
 TEvent *pEvent;
 //
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"----Event Приложения----");
 vmPrntBK(pdiv);
 vmprnSecsHead( &(App.secsEvent),pdiv);
 //
 RegIter(App.secsEvent,pEvent) tstvmprnEvent(pEvent,pdiv);
 }
void tstvmprnDoDb(TvmDo *pDo,TdivStream *pdiv){
 //
 if ( (pDo==NULL)||(pDo->pJob==NULL) ) return;
 vmPrntBK(pdiv);
 vmPrnt(pdiv," ****буфер B$ ***  ");
 vmPrntBK(pdiv);
 tstvmprnDb(&(pDo->pJob->vmDbBuf),"arrBuf",pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv," ****локальное дерево dbDo***  ");
 tstvmprnDb((TvmDb*)(&(pDo->dbDdo)),"dbDdo",pdiv);
 tstvmprnDb(&(pDo->dbAdo),"dbAdo",pdiv);
 vmPrnt(pdiv," ***локальные деревья Job*** ");
 tstvmprnarrDb(&(pDo->pJob->secDbJob),pdiv);
 vmPrntBK(pdiv);
// vmPrnt(pdiv," ***Глобали*** ");
// tstvmprnarrDb(&(pDo->pJob->pApp->Glbs),pdiv);
// vmPrntBK(pdiv);
 //
// tstvmprnarrLock(pDo,pdiv);
// tstvmprnarrEvent(pDo,pdiv);
 }
/*
typedef struct {
 Tdata val;
 Tstr name;
 ToffMod offMod;
 char isData;
 } TConst;
 */
static void tstvmprnCnst(TConst *pConst,TdivStream *pdiv){
 vmtstWriteStrNoSize(&(pConst->name),pdiv);
 vmPrnt(pdiv,"(");
 vmtstWriteData(&(pConst->val),pdiv); 
 vmPrnt(pdiv,") Off=%u",pConst->offMod);
 vmPrntBK(pdiv);
 }
static void tstvmprnarrCnst(Ttr *pTr,TdivStream *pdiv){
 TConst *pConst;
 arrIter(pTr->arrCnst,pConst) tstvmprnCnst(pConst,pdiv);
 vmPrntBK(pdiv);
 }
/**/
void tstvmprnTr(Ttr *pTr,TdivStream *pdiv){
 // 
 if (pTr->arrCnst.busy) {
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"Раздел Констант");
  vmPrntBK(pdiv);
  tstvmprnarrCnst(pTr,pdiv);
  }
 if (pTr->vmExt.busy) {
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"Раздел Предков");
  vmPrntBK(pdiv);
  vmprnArrStr(pTr->vmExt.psec,pTr->vmExt.busy,pdiv);
  }
 if (pTr->arrData.busy) {
  vmPrntBK(pdiv);
  vmPrnt(pdiv,"Раздел Данных");
  vmPrntBK(pdiv);
  vmprnDatas(NULL,pTr->arrData.psec,pTr->arrData.busy,pdiv);
  }
 //Раздел Меток
 vmprnarrLabel(&(pTr->arr2Label),pdiv);
 //раздел команд
 if (pTr->Cmds.busy) {
  vmprnarrCmd(pTr->Cmds.psec,pTr->Cmds.busy,&(pTr->arr2Label),pdiv);
  }
 //раздел длинных строк
 if (pTr->areaString.busy) {
  areaWide(pTr->areaString,1);
  vmprnAreaIter(&(pTr->areaString),pdiv);
  }
 }
/*
 static void vmtstWritesubInd(TsubInd *psubInd,TdivStream *pdiv){
  vmPrnt(pdiv,"name=");
  vmtstWriteStr(&(psubInd->name),pdiv);
  //u_char indtype;u_char subtype
  vmPrnt(pdiv," indtype=%u subtype=%u",psubInd->indtype,psubInd->subtype);
  vmPrntBK(pdiv);
  }
 */

void tstvmprnDbApp(TvmDo *pDo,TdivStream *pdiv){
 tstvmprnarrDb(&(App.Glbs),pdiv);
 }
  
#define vmtstWriteSecstrName(Arr,psec,pdiv) if (arrIsNoNull(Arr)) { vmPrntBK(pdiv);\
  RegIter(Arr,psec) { vmPrnt(pdiv," %p Name=",psec); vmtstWriteStrNoSize(&((psec)->name),pdiv); vmPrntBK(pdiv); }  }
//   Состояние VM
/*
//   Состояние App
typedef struct sttTApp {
 *TarrJob arrJob; //список заданий
 *TarrRun arrRun; //список блоков Run
 *TarrDo arrDo; //список блоков Do
 *TboxEvent boxEvent; //список пользовательских событий
 *TsecsMod Mods; // загруженные модули
 *TsecsLock Locks; //блокировки команды Lock
 *TLocksRes locksRes; //блокировки Разделяемых ресурсов
 *TsecsvmDb Glbs; //описание глобалей
 //
 TvmIni netDb; //список сетевых глобалей
 //
 TspinLock lockApp; // управление доступом к приложению
 TsecsFile arrFiles; //таблица файлов
 *uint maxId; //Максимальный идентификатор
 *char nameApp[lenNameApp+1]; //уникальный идентификатор приложения
 Tdata dataNULL; //пустое значение
 Tstrshr dlmIO;
 Tstr sunname; //имя безымянного модуля
 } TApp;
 */
void tstvmprnApp(TvmDo *pDo,TdivStream *pdiv){
 TvmMod *pmod;
 //
 if ( (pDo==NULL)||(pDo->pJob==NULL) ) return;
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  ***** ------------ ******");
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  ***** Состояние App MaxId=%i Name=******",App.maxId);
 vmtstWriteBuf(App.nameApp,lenNameApp,pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  ***** ------------ ******");
 vmPrnt(pdiv," список заданий Job ");
 vmtstWriteArriDisBusy(App.arrJob,pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv," список блоков Do ");
 vmtstWriteArriDisBusy(App.arrDo,pdiv);
 vmPrnt(pdiv," список модулей busy=%d",App.Mods.busy);
 vmtstWriteSecstrName(App.Mods,pmod,pdiv)
 vmPrntBK(pdiv);
 vmPrnt(pdiv," ****Глобали приложения****");
 tstvmprnDbApp(pDo,pdiv);
 vmPrnt(pdiv," список Событий ");
// vmtstWriteArrstrName((TarrpstrName*)(&(pDo->pJob->pApp->boxEvent)),pdiv);
 tstvmprnarrEvent(pDo,pdiv);
 vmPrnt(pdiv," блокировки команды Lock ");
// vmtstWriteArrstrName((TarrpstrName*)(&(pDo->pJob->pApp->Locks)),pdiv);
 tstvmprnarrLock(pDo,pdiv);
 vmPrnt(pdiv," блокировки Разделяемых ресурсов ");
 vmtstWriteArrpVoid((TarrpVoid*)(&(App.locksRes)),pdiv);
 vmPrnt(pdiv," TCP=%i pWWW=%p",pDo->pJob->divSrv.TCP,pDo->pJob->divSrv.pWWW);
 vmPrntBK(pdiv);
 }
/* 
 *   Состояние Job
//
typedef struct {
 *int iD; //идентифиватор задания
 struct sttTApp *pApp; //ссылка на приложение
 // Регистры
 *TstkInd  inds; //индексный регистр
 *TmapLong mlong;
 *TsecsvmDb secDbJob; //локальные деревья задания
 *struct sttTtr exeTr; //блок трансляции
 TPacks bufpack; //буфер сериализации
 *Tdiv TCP; //основной порт связи по TCP
 Tfile *pfile; //текущее устройство
 *pthread_t idPth; //идентификатор потока
 pthread_cond_t waitEvent; //Управление ожиданием событий
 Tdata val; //возвращаемое значение
 uint isbusy; //свободно=0 или используется задание=1
 *Terr err; //код возврата
 Tstrshr dlmIO;
 *Tstr nameJob;
 } TJob;
 */
void tstvmprnJob(TvmDo *pDo,TdivStream *pdiv){
 TvmDb *pDb;
 //
 if ( (pDo==NULL)||(pDo->pJob==NULL) ) return;
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  ***** ------------ ******");
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  ***** Состояние Job ****** err=%d errStr=%d errPos=%d iD=%i Name="
         ,pDo->pJob->errTr.err,pDo->pJob->errTr.numstr,pDo->pJob->errTr.numpos,pDo->pJob->iD);
 vmtstWriteStrNoSize(&(pDo->pJob->nameJob),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  ***** ------------ ******");
 vmPrnt(pdiv," индексный регистр ");
 vmPrntBK(pdiv);
 vmtstWriteArrInds(&(pDo->pJob->inds),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv," страница длинных строк ");
 vmtstWriteSecLongs(&(pDo->pJob->vmDbLong.db.mlong),pdiv);
 vmPrnt(pdiv," локальные деревья задания ");
 vmtstWriteSecstrName(pDo->pJob->secDbJob.regDb,pDb,pdiv)
 vmPrntBK(pdiv);
 vmPrnt(pdiv," локальные массивы задания ");
 vmtstWriteSecstrName(pDo->pJob->secDbJob.regArr,pDb,pdiv)
 vmPrnt(pdiv," idPth=%lu dlmIO=",pDo->pJob->idPth);
 vmtstWriteStrNoSize((Tstr*)(&(pDo->pJob->dlmIO)),pdiv);
 vmPrnt(pdiv," Val= ");
 vmtstWriteKeyData(&(pDo->pJob->val),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv," блок трансляции ");
 tstvmprnTr( &(pDo->pJob->exeTr),pdiv);
 vmPrntBK(pdiv);
 }
void vmprnrefDb( TrefDb *prefDb,TdivStream *pdiv){
 vmPrnt(pdiv,"pDb=%p off=%i pPath=",prefDb->pvmDb,prefDb->off);
 if (prefDb->pPath) {vmtstWriteStrNoSize(prefDb->pPath,pdiv);}
 vmPrnt(pdiv,"style=%x: ",prefDb->style);
 switch(prefDb->style){
  case CstyleDbArr :
   vmPrnt(pdiv,"%s: ","Arr");
   break;
  case CstyleDbArg :
   vmPrnt(pdiv,"%s: ","Arg");
   break;
  case CstyleDbArgJob :
   vmPrnt(pdiv,"%s: ","ArgJob");
   break;
  case CstyleDbDb :
   vmPrnt(pdiv,"%s: ","Db");
   break;
  case CstyleDbGlb :
   vmPrnt(pdiv,"%s: ","Glb");
   break;
  case CstyleDbNet :
   vmPrnt(pdiv,"%s: ","Net");
   break;
  } 
 if (prefDb->rInds.busy){
  vmPrntBK(pdiv);
  vmPrnt(pdiv," Индекс prefDb->Ind");
  vmtstWriteArrData(&(prefDb->rInds),pdiv);
  }
 vmPrntBK(pdiv);
 if ( prefDb &&(prefDb->fullInds.busy) ) {
  vmPrnt(pdiv," prefDb->fullInds");
  vmtstWriteArrData( &(prefDb->fullInds),pdiv);
  }
 vmPrntBK(pdiv);
 }
/*
void vmprnarrrefDb(TarrrefDb *parrrefDb,TdivStream *pdiv){
 TarrTdata *prefDb;
 //
 vmprnArr2Head(parrrefDb,pdiv);
 parrIter(parrrefDb,prefDb){
  if (prefDb->busy){
   vmPrntBK(pdiv);
   vmPrnt(pdiv," Индекс prefDb->Ind");
   vmPrntBK(pdiv);
   vmtstWriteArrData((TarrData*)prefDb,pdiv);
   }
  }
 vmPrntBK(pdiv);
 }
*/
/*
void vmprnarrrefDb(TstkThis *pInds,TdivStream *pdiv){
 TarrInt *parrInt;
 int c0,c1;
 int j0;
 //
 c0=c1=0;
 vmtstWriteArrHead(&(pInds->_core),pdiv);
 for(j0=0;j0<pInds->_core.busy; ++j0){
  vmPrnt(pdiv,"%5i ",j0);
  vmtstWriteData(arrOffpD(pInds->_core,j0),pdiv);
  if ( (pInds->_ct0.busy) &&(c0<pInds->_ct0.busy) &&(j0>=pInds->_ct0.psec[c0]) ) c0++;
  vmPrnt(pdiv,"  %i",c0);
  vmPrntBK(pdiv);
  }
 parrInt=(TarrInt*)(&(pInds->_ct0));
 vmPrnt(pdiv,"ind_ct0 ");
 vmtstWriteArrInt(parrInt,pdiv);
 }
*/
/*
 *   Состояние Do
 *int iD; //идентификатор блока
 *TRun *pRun; //ссылка на свое задание
 // Регистры
 *TarrData Args; //аргументы текущего блока Do
 *TarrInd iobjUp; //ссылка на объект
 *TvmDb dbDo; //локальное дерево блока
 *TBoxIterQuery curquery; //Регистр Итераторов
 *TarrInt arrJob; //стек запущеных из блока заданий
 *TarrData arInd; //текущий индекс блока Do
 *Tarea Temps; //Область промежуточных данных
 //
 TrefDb refDb; //ссылка на последний индекс по которому было обращение Get, Set, Kill
 *TarrInd *piobj; //индекс объекта текущего блока Do
 *TvmMod *pmod; // ссылка на описание модуля в App
 *TflowCmd *pflowCmd;
 *struct TvmsttDo *pDoParent;  //Блок Do который вызвал данный блок
 //
 *Terr err; //код возврата
 *Tstr nameDo; //имя блока
 char reserv[4]; //дополнительное место для продолжения строки
 uint isbusy; //свободно=0 или используется задание=1
 *char flag; //флаги блока выполнения
 *char stat; //статус выполнения Do
 */
void tstvmprnDo(TvmDo *pDo,TCmd *pcurCmd,TdivStream *pdiv){
 //
 if ((pDo==NULL)||(pDo->pmod==NULL)) return;
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  ***** ------------ ****** modName=");
 vmtstWriteStrNoSize(&(pDo->pmod->name),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  ***** Состояние Do ****** flag=%x stat=%x err=%d eof=%d errStr=%d errPos=%d iD=%i Name="
         ,pDo->flag,pDo->stat,pDo->err,pDo->eof,pDo->pJob->errTr.numstr,pDo->pJob->errTr.numpos,pDo->iD);
 vmtstWriteStrNoSize(&(pDo->nameDo),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv,"  ***** ------------ ******");
 vmPrntBK(pdiv);
 if (pcurCmd){
  vmPrnt(pdiv," команда #%d",pDo->curOff-1);
  tstvmprnarrCmd1(pcurCmd,pdiv);
  }
 vmPrntBK(pdiv);
 vmPrnt(pdiv," Возвращаемое значение=");
 vmtstWriteKeyData(&(pDo->pJob->val),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv," Данные Итератора= ");
 vmtstWriteKeyData(&(pDo->pJob->valquery),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv," ***индексный регистр*** ");
 vmPrntBK(pdiv);
 vmtstWriteArrInds(&(pDo->pJob->inds),pdiv);
 vmPrntBK(pdiv);
 return;
 if (pDo->rarInd.busy){
  vmPrnt(pdiv," ***текущий индекс блока Do*** ");
 vmPrntBK(pdiv);
  vmtstWriteArrData(&(pDo->rarInd),pdiv);
  }
 vmPrntBK(pdiv);
 vmPrnt(pdiv," ***аргументы текущего блока Do*** ");
 vmPrntBK(pdiv);
 vmtstWriteArrData(&(pDo->arArgs),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv," ***индекс объекта*** this=");
 vmtstWriteArrData(&(pDo->this),pdiv);
 vmPrntBK(pdiv);
 vmPrnt(pdiv," ***последнее обращение к Данным refDb *** ");
 vmPrntBK(pdiv);
 vmprnrefDb(&(pDo->refDb),pdiv);
 vmPrnt(pdiv," ***Job stkthis *** тип данных:");
 if (pDatastyleIsIndArr(pDo->pJob->thisObj.psec)){
  vmPrnt(pdiv," Arr ");
  }
 else{
  vmPrnt(pdiv," Db ");
  }
 vmPrntBK(pdiv);
// vmprnarrrefDb(&(pDo->pJob->stkthis),pdiv);
 vmtstWriteArrData(&(pDo->pJob->thisObj),pdiv);
 if (pDo->pmod) {vmPrnt(pdiv,"pMod=%p ",pDo->pmod);} else {vmPrnt(pdiv,"pMod=NULL ");
  vmPrnt(pdiv,"pbegcmds=%p curcmd=%u sumcmds=%d ",pDo->pmod->pbegcmds,pDo->curOff,pDo->pmod->sumcmd);
  }
 if (pDo->pDoParent) {vmPrnt(pdiv,"pParent=%p ",pDo->pDoParent);} else {vmPrnt(pdiv,"pParent=NULL ");}
 vmPrntBK(pdiv);
 if (pDo->Temps.busy) {
  vmPrnt(pdiv,"Area Temps");
  vmtstWriteAreaInt(&(pDo->Temps),pdiv);
  vmPrntBK(pdiv);
  }
 if (pDo->curquery.busy) {
  vmPrnt(pdiv," ***Регистр Итераторов***");
  tstvmprnBoxcurQuery(&(pDo->curquery),pdiv);
  }
 vmPrntBK(pdiv);
 }
