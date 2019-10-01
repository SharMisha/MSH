/*
 * File:   mshVmMod.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 21 Август 2014 г., 18:50
 */
#include "mshvmVm.h"
#include "mshvmCnst.h"
//
void getPachtLibDir(Tstr *pname,const char *pRas,int sizeDir,char *pout,int sizeout);
Terr mshProgReadToMap(char *pfl,const char *pCmshMag,Tmap *pMap);
void mshDbFree(TDb *pDb);


/*
typedef struct {
 TPacks modpack; //буфер модуля
 Tmap mapmod; //отображение файла модуля в память
 Tarr2Label labels; //метки
 Tarea areaString; //область размещения длинных строк
 Tstr  *pext;   // начало области родителей объекта
 Tdata *pdata;   // начало области данных
 TCmd *pbegcmds; // начало команд
// TflowCmd flowCmd;
 Tsize sumext;
 Tsize sumdata;
 Tsize sumcmd;
 Tstr name;   // имя модуля
 } TvmMod;
 */
/*
void ModInit(TvmMod *pMod){
 memNULL(pMod);
 }
*/
/*
 *   запаковать программный модуль
 */
static void PackpMod(TvmMod *pMod){
 Tbstr bsOut;
 //имена модулей расширения
 PackClear((pMod)->modpack);
 bsOut.size=sizeof(Tstr) * pMod->sumext;
 bsOut.psec=(char*)(pMod->pext);
 packVoid(&(pMod->modpack),&(bsOut) );
 //область комманд
 bsOut.size=sizeof(TCmd) * pMod->sumcmd;
 bsOut.psec=(char*)(pMod->pbegcmds);
 packVoid(&(pMod->modpack),&(bsOut) );
 //область Длинныхстрок
 bsOut.size=sizeof(*(pMod->areaString.psec)) * pMod->areaString.busy;
 bsOut.psec=(char*)(pMod->areaString.psec);
 packVoid(&(pMod->modpack),&(bsOut) );
 //область констант
 bsOut.size=sizeof(Tdata) * pMod->sumdata;
 bsOut.psec=(char*)(pMod->pdata);
 packVoid(&(pMod->modpack),&(bsOut) );
 //область меток
 if (pMod->labels.busy){
  TarrTLabel *psecLabels;
  //
  arrIter(pMod->labels,psecLabels){
   bsOut.size=sizeof(*(psecLabels->psec)) * psecLabels->busy;
   bsOut.psec=(char*)(psecLabels->psec);
   packVoid(&(pMod->modpack),&(bsOut) );
   }
  }
 }
/*
 *   распаковать программный модуль
 */
void PackUnpMod(TvmMod *pMod){
 Tdata *pD;
 char *pE;
 TarrTLabel *psecLabels;
 //
 PackPtr0(pMod->modpack);
 pE=(char*)(pMod->modpack.psec + pMod->modpack.size);
 //имена модулей расширения
 pMod->pext=(Tstr*)(pMod->modpack.Ptr.pPtrStr->psec);
 pMod->sumext=pMod->modpack.Ptr.pPtrStr->size / sizeof(Tstr);
 //область комманд
 PackNextVoid(pMod->modpack);
 pMod->sumcmd=pMod->modpack.Ptr.pPtrStr->size / sizeof(TCmd);
 pMod->pbegcmds=(TCmd*)(pMod->modpack.Ptr.pPtrStr->psec);
 //область Длинныхстрок
 PackNextVoid(pMod->modpack);
 pMod->areaString.psec=(Tsize*)(pMod->modpack.Ptr.pPtrStr->psec);
 pMod->areaString.busy=pMod->areaString.size=pMod->modpack.Ptr.pPtrStr->size / sizeof(*(pMod->areaString.psec));
 //область констант
 PackNextVoid(pMod->modpack);
 pMod->pdata=(Tdata*)(pMod->modpack.Ptr.pPtrStr->psec);
 pMod->sumdata=pMod->modpack.Ptr.pPtrStr->size / sizeof(Tdata);
 //настроить ссылки на область
 for (pD=pMod->pdata;pD < pMod->pdata + pMod->sumdata ;++pD) if (pDataIsArea(pD)) (pD)->areaOff.parea=&(pMod->areaString);
 //область меток
 arr2Init(pMod->labels,CtrSum2LabelDef,CtrSizeLabelDef);
// arr2Clear(pMod->labels);
 if (pMod->modpack.Ptr.pPtrChar>=pE) return;
 while(1){
  PackNextVoid(pMod->modpack);
  if (pMod->modpack.Ptr.pPtrChar>=pE) break;
  arr2Push(pMod->labels);
  psecLabels=pMod->labels.ptop;
  psecLabels->busy=pMod->modpack.Ptr.pPtrStr->size / sizeof(*(psecLabels->psec));
  psecLabels->psec=(TLabel*)(pMod->modpack.Ptr.pPtrStr->psec);
  parrsetptop(psecLabels);
  }
 }
/*
 * загрузка модуля с устройства
 */
Terr loadDivMod0(TvmMod *pMod){
 Terr err;
 char pfl[CsizePacthFl];
 void *p;
 int w;
 //
 pMod->mapmod.psec=NULL;
 getPachtLibDir(&(pMod->name),pCmshDirVm,CsizePartModDef,pfl,CsizePacthFl);
 w=strlen(pfl);
 //pfl[w++]=CdelimFile;
 mshaddExtToFL(pCmshExtVm,pfl,w);
 err=mshProgReadToMap(pfl,pCmshMagVM,&(pMod->mapmod));
 if (err <CERR) {
  err=CERRvmModNotFound;
  return err;
  }
 w=strlen(pCmshMagVM);
 p=pMod->mapmod.psec+w;
 pMod->modpack.psec=p;
 pMod->modpack.busy=pMod->modpack.size=mshDivUp(pMod->mapmod.size-w,sizeof(Tsize));
 PackUnpMod(pMod);
 divClose(pMod->mapmod.div);
 pMod->mapmod.div=CdivRetErr;
 return err;
 }
Terr loadDivMod(Tstr *pname,TvmMod *pMod){
 pMod->name=*pname;
 return loadDivMod0(pMod);
 }
/*
 * Загрузить модуль
 */
TvmMod* getMod(Tstr *pname,Terr *perr){
 TvmMod *pMod;
 //
 if (pname->size<1) { *perr=CERRnameModNotFound; return NULL; }
 *perr=COutKodOk;
 spinLock(App.Mods.lock);
 RegIter(App.Mods,pMod)
  if (StrpEqu(pMod->name,pname)==CMPeq){
   spinUnLock(App.Mods.lock);
   return pMod;
   }
 //такого модуля нет загрузить его из библиотеки
 pRegBusy(&(App.Mods));
 spinUnLock(App.Mods.lock);
 pvmModInit(App.Mods.ptop);
 *perr=loadDivMod(pname,App.Mods.ptop);
 if (*perr<CERR){
  pRegPop(&(App.Mods));
  return NULL;
  }
 return App.Mods.ptop;
 }
/* ToffMod
 * Найти в модуле метку
 */
Tsize getLabelSecMod(TvmMod *pMod,Tstr *psLabel,Tsize iSec){
 TLabel *pLab;
 TarrTLabel *psecLabels;
 //
 if (pMod->labels.busy>iSec){
  psecLabels=arrOffpD(pMod->labels,iSec);
  parrIter(psecLabels,pLab)
   if (StrpEqu(pLab->label,psLabel)==CMPeq)
    return pLab->off;
  }
 return COutLabelNotFound;
 }
/*
 * Найти Метку в модуле
 * с учетом Предков
 */
Terr getLabelApp(TrefProg *prefProg){
 Terr err;
 //
 prefProg->off=0;
 if (prefProg->label.size==0) return 0;
 //ищем метку в текущем модуле
 if ( (prefProg->off=getLabelSecMod(prefProg->pmod,&(prefProg->label),0))>=0 ) {
  return COutKodOk;
  }
 //метка не найдена ищем по наследованию
 int j;
 TvmMod *pModext=prefProg->pmod;
 for (j=0;j<pModext->sumext;j++)
  if ( ((prefProg->pmod=getMod(pModext->pext+j,&err))!=NULL )
      && ( (err=getLabelApp(prefProg))==COutKodOk ) )
   return err;
 return CERRvmLabelNotFound;
 }
/*
 *
 */
Terr DbCreatSize(TDb *pDb,char *patch,Tusize sizePg,Tusize sizeLong);
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
Terr openDir(char *patchDir);

Terr writeDivMod(TvmMod *pMod,Ttr *pTr){
 TDb Db;
 TConst *pConst;
 TarrData Inds;
 Tdata Key;
 Tdiv ff;
 int w,w1,wp;
 Terr err=COutKodOk;
 char pfl[CsizePacthFl];
 //записатьего в файл
 getPachtLibDir(&(pMod->name),pCmshDirVm,CsizePartModDef,pfl,CsizePacthFl);
 wp=strlen(pfl);
 openDir(pfl);
 mshExtaddtoFL(pCmshExtVm,pfl);
 if ((ff=divCreat(pfl))==CdivRetErr) return CERRvmNotFileCreate;
 //записать Mag
 divWrite(ff,pCmshMagVM,strlen(pCmshMagVM));
 //записать программу
 PackpMod(pMod);
 w=pMod->modpack.busy * sizeof(Tsize);
 if ((w1=divWrite(ff,pMod->modpack.psec,w))!=w) err=CERRdivWriteBad;
 divClose(ff);
 //константы сбросить в дерево
 mshaddExtToFL(pCmshExtCnst,pfl,wp);
 w=strlen(pfl);
 pfl[w]=CdelimFile;
 pfl[w+1]='\0';
 if ( (err=DbCreatSize((TDb*)(&Db),pfl,CsizeModPgDef,CsizeModLongDef) )<CERR ) return err;
 Inds.busy=Inds.size=Inds.capa=1;
 DataSetStr(Key);
 Inds.psec=Inds.ptop=&Key;
 arrIter(pTr->arrCnst,pConst){
  Key.str=pConst->name;
  if (DataIsNoNULL(pConst->val))
   err=mshSet(&Db,&Inds,&(pConst->val));
  }
 mshDbFree((TDb*)(&Db));
 return COutKodOk;
 }
