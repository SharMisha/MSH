/* 
 * File:   mwinFront.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 *
 * Created on 3 февраля 2019 г., 16:48
 */

#include <gtk/gtk.h>
#include "mshMem.h"
#include "mshErr.h"
#include "mshCnst.h"
#include "mshData.h"
#include "mshIni.h"
//
#include "mshvmCnst.h"
#include "mshvmVm.h"
//
#include "mwinCore.h"
#include "mwinWidget.h"
//внешняя функция из vm
Terr vmwingetModLabel(Tstr *psMod,Tstr *psLabel,void *pUser,Tdata *pdataModLabel);
Terr setParObj(GtkWidget *pObj,EqType qpar,Tdata *psetData);
//

GIOChannel* ClntGtkInitChannel(TClnt *pClnt,Tbstr *pbHostPort , int *perr ,int (*pfwork)(TClnt *pClnt)  );
//extern void winSetChannel(Tdiv tcp,TfsrvWrite fsrvWrite,void* pUser);
int bufSend(Tdiv div,Tbstr *pbsBf);
int mClntUsrDef(TClnt *pClnt);
int iniDirReadFL(Tbstr *pbfl,const char *pDir,TIni *pIni);
//
Terr mshSet(TDb *pDb,TarrData *pInds,Tdata *psetData);
void castbStr(Tdata *pData,Tbstr *pbstrOut);
Terr castDataInt(Tdata *pData);
Tstr* castId(Tdata *pData);
GObject* getObjAtrtype(TarrData *pArgs,EwinTypeObj *ptypeObj,EwinTypeAtr *ptypeAtr,Tdata **ppdname,Terr *perr);
GObject* getObj(Tdata *pdName,Terr *perr);
void fileDirPacht(Tbstr *pname,const char *pDir,char *pout);
void wingetPachFl(Tdata *pArg,const char *pDir,char *pout);
void castbStr(Tdata *pData,Tbstr *pbstrOut);
//
TwinDb winDb;

//GQuark qPack=1;
//
//GQuark qType=2;
//GQuark qParent=3;

int sizeBuf=1024;
//
Terr mshgetDb(TDb *pDb,Tstr *pName,const char *pDir,enum TisSet dbisSet,int sizePartGlb,enum TtypeDb typeDb);
void mshDbFree(TDb *pDb);
Terr mshPushArr(TDb *pDb,Tdata *pSetData);
char* strToBuf(Tdata *pArg,Tbstr *pbStr,char *pbuf,int sizebuf,char *pDef);
GObject* findObj(TDb *pDb,Tdata *pdataName);
Tdata* findatrObj(TDb *pDb,Tdata *pdataName);
Tdata* getParObj(GtkWidget *pObj,EqType qpar);
Terr setPardataObj(Tdata *pdName,EqType qpar,Tdata *psetData);
//Tstr* winnameMod(void* pUser);
//
GObject* winWinNew(char *ptitle);
GObject* winNbNew(GtkPositionType pos,gboolean show_tabs,gboolean scrollable);
GtkWidget* winLbNew(char *ptitle);
GtkWidget* winButNew(char *plabel,char *pfile,GtkPositionType position);
GtkWidget* winBoxNew(GtkOrientation  orientation,gint spacing);
GtkWidget* winBoxBNew(GtkOrientation  orientation,GtkButtonBoxStyle layout_style);
GObject* winmwinScrollNew(GtkPolicyType hscrollbar_policy,GtkPolicyType vscrollbar_policy);
GObject* winGridNew(GtkTreeModel *pmodel,int cols);
GObject* winTreeNew(GtkTreeModel *pmodel,int cols);
GtkWidget* winImgNew(char *pfile);
GtkWidget* winCalendarNew();
GObject* winNewStore(int cols,int isTree);
gint winNbBnaddPg(GtkNotebook *pNb,GtkWidget *pPg,char *pslab,char *pimgfl);
//
void winWinSetTitle(GtkWidget *pObj,char *ptitle);
char* winWinGetTitle(GtkWidget *pObj);
void winWinSetSize(GtkWidget *pObj,gint setWidth,gint setHeight);
void winWinGetSize(GtkWidget *pObj,gint *pwidth,gint *pheight);
void winWinGetPos(GtkWidget *pObj,gint *pX,gint *pY);
void winLbSetTitle(GtkWidget *pObj,char *ptitle);
void winRowToNblabel(GtkNotebook *pNb,Tbstr *pbstrBuf);
void winLbSetTitle(GtkWidget *pObj,char *ptitle);
void winButSetTitle(GtkWidget *pObj,char *plabel);
void winButSetImage(GtkWidget *pObj,char *pfile);
void winButSetPosition(GtkWidget *pObj,GtkPositionType position);
void winViewSetHeder(GtkTreeView *pView,Tbstr *pbstrBuf);
void winViewGetColHeder(GtkTreeView *pView,char **ppcBuf,int col);
void winNbSetTitle(GtkWidget *pObj,Tbstr *psbf);
void winNbSetFocus(GtkNotebook *pObj,gint nPg);
void winViewRowsTreeStore(GtkTreeStore *pStore,Tbstr *pbstrBuf);
void winViewRowsTreeStoreAdd(GtkTreeStore *pStore,Tbstr *pbstrBuf);
void winViewRowsListStore(GtkListStore *pStore,Tbstr *pbstrBuf);
void winViewRowsListStoreAdd(GtkListStore *pStore,Tbstr *pbstrBuf);
void winViewGridUpDateRow(GtkListStore *pStore,Tbstr *pbstrBuf,int row);
void winViewGridUpDateCol(GtkListStore *pStore,Tbstr *pbstrBuf,int row,int col);
void winViewGridGetCol(GtkTreeModel *pStore,char **ppcBuf,int row,int col);
void winViewGridGetRow(GtkTreeModel *pStore,Tarea *pArea,int row);
void winViewGridGet(GtkTreeModel *pStore,Tarea *pArea);
void winCalendarSetData(GtkWidget *pObj,Tbstr *psbf);
void winCalendarGetData(GtkWidget *pObj,Tstr *pstr);
char* winLbGetTitle(GtkWidget *pObj);
char* winButGetTitle(GtkWidget *pObj);
GtkPositionType winButGetPosition(GtkWidget *pObj);
Terr winSetOn(GtkWidget *pObj,Tdata *pdnameObj,char *pcOn,Tstr *psLabel,Tstr *psMod,void* pUser);
Terr winSetOnInit(GtkWidget *pObj,Tdata *pdnameObj,Tstr *psLabel,Tstr *psMod,EqType qpar,void* pUser);
char* selfocusCol(GtkTreeView *pView,int col);
void winBoxSetSpacing(GtkBox *pObj,gint spacing);
gint winBoxGetSpacing(GtkBox *pObj);
void winImgSetSpacing(GtkBox *pObj,gint spacing);
void winBoxPack(GtkBox *pObj,GtkWidget *child,EwinPackChild typePack,gboolean expand,gboolean fill,guint padding);
Terr DbCreatDb(TDb *pDb,char *patch,TdbHead *phdr);
Terr openDir(char *patchDir);
/*
 * инициализация библиотеки Win
 */
Terr winInit(){
 Terr err;
 TdbHead hdr;
 char CwinDbKat[]="./dbw/win/\0\0\0\0";
 //
 memNULL(&(winDb));
 winDb.Clnt.sctrw=CdivRetErr;
 CwinDbKat[0]=CdelimKat;
 CwinDbKat[1]=CwinDbKat[5]=CwinDbKat[9]=CdelimFile;
// CwinDbKat[2]='d'; CwinDbKat[3]='b'; CwinDbKat[4]='w'; CwinDbKat[6]='w'; CwinDbKat[7]='i'; CwinDbKat[8]='n';
 CwinDbKat[10]=Cvm0;
 hdr.typeDb=EtypeDb;
 hdr.sizelong=0;
 hdr.sizepg=CsizewinDbPgDef;
 err=openDir(CwinDbKat);
 if (err>CERR)
  err=DbCreatDb(&(winDb.Db),CwinDbKat,&hdr);
 //
 if (err<CERR) {
  return err;
  }
 areaInit(winDb.Area,CwinsizeAreaDef);
 iniInit(winDb.Ini);
 return COutKodOk;
 }
/**/
Terr winFree(){
 if (winDb.Db.typeDb==EtypeDb){
  areaFree(winDb.Area);
  iniFree(winDb.Ini);
  mshDbFree(&(winDb.Db));
  }
 return COutKodOk;
 }
/*
 * Поместить Объект в предка
 */
static void ObjToParent(GtkWidget *pObj,GtkWidget *pParent,EwinTypeObj typeParent){
 EwinPackChild typePack;
 Tdata *pdPar,setData;
 int inPg;
 guint padding;
 //
 switch(typeParent){
   case EwinNb :
    inPg=winNbBnaddPg(GTK_NOTEBOOK(pParent),pObj,NULL,NULL);
    setData.voidtype2.void1=pParent;
    setData.voidtype2.type1=inPg;
    (void)setParObj(pObj,qinPg,&setData);
    break;
   case EwinBox :
    pdPar=getParObj(pParent,qPack);
    if (pdPar==NULL){
     padding=CwinsizePaddingDef;
     typePack=EwinPackStart;
     }
    else{
     typePack=pdPar->uint64[0];
     padding=pdPar->uint64[1];
     }
    winBoxPack(GTK_BOX(pParent),pObj,typePack,FALSE,FALSE,padding);
    break;
   default: 
    gtk_container_add(GTK_CONTAINER(pParent),pObj );
    break;
  } 
 }
/*
 * Создать новый объект
 */
Terr winNewObj(TarrData *pArgs){
 EwinTypeObj type;
 Tbstr bStr1;
 char buf1[sizeBuf],buf2[CsizePacthFl];
 char *pC1=NULL,*pC2=NULL;
 TarrData Inds;
 Tdata setData,*pdataParent,*pdataName,dataParent;
 GtkWidget *pParent=NULL;
// Terr err=COutKodOk;
 GObject *pObj=NULL,*pO;
 char *ptitle="Главное окно";
 EwinPackChild typePack;
 //
 GtkPositionType pos=GTK_POS_TOP;
 gboolean show_tabs=TRUE;
 gboolean scrollable=TRUE;
 GtkOrientation  orientation;
 gint intpar;
 GtkButtonBoxStyle layout_style;
 GtkPolicyType hscrollbar_policy,vscrollbar_policy;
 int narg1=3,narg2=narg1+1,narg3=narg1+2,narg4=narg1+3;
// Terr err;
 //
 if (winDb.Db.typeDb==EtypeNo) return CERRwinInitNo;
 if ((pArgs==NULL)||(pArgs->busy<2)) return CERRwinArgLittle;
 //Первым аргументом должно идти тип создаваемого объекта
 castDataInt(pArgs->psec);
 type=(EwinTypeObj)(pArgs->psec[0].intgr);
 if ( (type<=Ewin0)||(type>=EwinEnd) ) return CERRwinTypeNo;
 //Вторым аргументом должно идти имя
 pdataName=pArgs->psec+1;
 if ((pdataName==NULL)||(pDataIsNoStr(pdataName)) ) return CERRwinObjNotFound;
 (void)castId(pdataName);
 if (pdataName->str.size<1) return CERRwinObjNotFound;
 pdataName->str.psec[pdataName->str.size]=Cvm0;
 memNULL(&setData);
 //Третьим аргументом должен идти имя предка, имя может быть пустой строкой
 if (pArgs->busy>2){
  pdataParent=findatrObj(&(winDb.Db),pArgs->psec+2);
  if (pdataParent!=NULL){
   pParent=pdataParent->voidtype2.void1;
   dataParent=*pdataParent;
   }
  }
 else
  pParent=NULL;
 switch(type){
  case EwinWin :
   if (pArgs->busy>narg1)
    pC1=strToBuf(pArgs->psec+narg1,&bStr1,buf1,sizeBuf,ptitle);
   pObj=winWinNew(pC1);
   break;
  case EwinNb :
   if (pArgs->busy>narg1){ castDataInt(pArgs->psec+narg1);  pos=pArgs->psec[narg1].intgr; 
    if (pArgs->busy>narg2){ castDataInt(pArgs->psec+narg2);  show_tabs=pArgs->psec[narg2].intgr;
     if (pArgs->busy>narg3){ castDataInt(pArgs->psec+narg3);  scrollable=pArgs->psec[narg3].intgr; }
     }
    }
   pObj=winNbNew(pos,show_tabs,scrollable);
   break;
  case EwinLb :
   if (pArgs->busy>narg1)
    pC1=strToBuf(pArgs->psec+narg1,&bStr1,buf1,sizeBuf,NULL);
   pObj=(GObject*)winLbNew(pC1);
   break;
  case EwinBut :
   pos=GTK_POS_RIGHT;
   if (pArgs->busy>narg1){
    pC1=strToBuf(pArgs->psec+narg1,&bStr1,buf1,sizeBuf,NULL);
    if (pArgs->busy>narg2){
     wingetPachFl(pArgs->psec+narg2,pCmshExtImg,buf2);
     pC2=buf2;
     if (pArgs->busy>narg3){ castDataInt(pArgs->psec+narg3);  pos=pArgs->psec[narg3].intgr;}
     }
    }
   pObj=(GObject*)winButNew(pC1,pC2,pos);
   break;
  case EwinBox :
   if (pArgs->busy<=narg1) return CERRwinArgLittle;
   typePack=EwinPackStart;
   intpar=4;
   castDataInt(pArgs->psec+narg1);
   orientation=pArgs->psec[narg1].intgr;
   if (pArgs->busy>narg2){
    castDataInt(pArgs->psec+narg2);
    typePack=pArgs->psec[narg2].intgr;
    if ((typePack<EwinPackStart)||(typePack>EwinPackCentr)) return CERRwinArgNotType;
    if (pArgs->busy>narg3){
     castDataInt(pArgs->psec+narg3);
     intpar=pArgs->psec[narg3].intgr;
     }
    }
   pObj=(GObject*)winBoxNew(orientation,intpar);
   if (pObj!=NULL){
    setData.uint64[0]=typePack;
    setData.uint64[1]=intpar;
    (void)setPardataObj(pdataName,qPack,&setData);
    }
   break;
  case EwinBoxBut :
   orientation=GTK_ORIENTATION_HORIZONTAL;
   layout_style=GTK_BUTTONBOX_START;
   if (pArgs->busy>narg1){ castDataInt(pArgs->psec+narg1);  orientation=pArgs->psec[narg1].intgr; 
    if (pArgs->busy>narg2){
     castDataInt(pArgs->psec+narg2);
     layout_style=pArgs->psec[narg2].intgr;
     if ((layout_style<GTK_BUTTONBOX_SPREAD)||(layout_style>GTK_BUTTONBOX_EXPAND)) return CERRwinArgNotType;
     }
    }
   pObj=(GObject*)winBoxBNew(orientation,layout_style);
   break;
  case EwinSc :
   hscrollbar_policy=GTK_POLICY_AUTOMATIC;
   vscrollbar_policy=GTK_POLICY_AUTOMATIC;
   if (pArgs->busy>narg1){ castDataInt(pArgs->psec+narg1);  hscrollbar_policy=pArgs->psec[narg1].intgr; 
    if (pArgs->busy>narg2){
     castDataInt(pArgs->psec+narg2);
     vscrollbar_policy=pArgs->psec[narg2].intgr;
     }
    }
   pObj=(GObject*)winmwinScrollNew(hscrollbar_policy,vscrollbar_policy);
   break;
  case EwinGrid :
   intpar=4;
   pO=NULL;
   if (pArgs->busy>narg1){
    castDataInt(pArgs->psec+narg1);
    intpar=pArgs->psec[narg1].intgr;
    if (pArgs->busy>narg2){
     pO=findObj(&(winDb.Db), pArgs->psec+narg2);
     }
    }
//   pObj=(pO==NULL)?winGridNew(NULL,intpar):winGridNew((GtkTreeModel*)pO,intpar);
   pObj=winGridNew((GtkTreeModel*)pO,intpar);
   break;
  case EwinTree :
   intpar=4;
   pO=NULL;
   if (pArgs->busy>narg1){
    castDataInt(pArgs->psec+narg1);
    intpar=pArgs->psec[narg1].intgr;
    if (pArgs->busy>narg2){
     pO=findObj(&(winDb.Db),pArgs->psec+narg2);
     }
    }
//   pObj=(pO==NULL)?winTreeNew(NULL,intpar):winTreeNew((GtkTreeModel*)pO,intpar);
   pObj=winTreeNew((GtkTreeModel*)pO,intpar);
   break;
  case EwinListStore :
   if (pArgs->busy>narg1){
    castDataInt(pArgs->psec+narg1);
    intpar=pArgs->psec[narg1].intgr;
    if (pArgs->psec[narg1].intgr>0){
     pObj=winNewStore(pArgs->psec[narg1].intgr,0);
     }
    }
   pParent=NULL;
   break;
  case EwinTreeStore :
   if (pArgs->busy>narg1){
    castDataInt(pArgs->psec+narg1);
    intpar=pArgs->psec[narg1].intgr;
    if (pArgs->psec[narg1].intgr>0){
     pObj=winNewStore(pArgs->psec[narg1].intgr,1);
     }
    }
   pParent=NULL;
   break;
  case EwinImg :
   if (pArgs->busy>narg1){
    wingetPachFl(pArgs->psec+narg1,pCmshExtImg,buf2);
    pC1=buf2;
    }
   pObj=(GObject*)winImgNew(pC1);
   break;
  case EwinCalendar :
   pObj=(GObject*)winCalendarNew();
   break;
  default :
   return CERRTypeNo;
  } 
 memNULL(&setData);
 Inds.capa=Inds.size=1;
 Inds.busy=1;
 Inds.psec=Inds.ptop=pdataName;
 setData.voidtype2.void1=pObj;
 setData.voidtype2.type1=type;
 //если предок есть, то объект поместить в его потомки
 if (pParent!=NULL){
  setData.voidtype2.void2=pParent;
  setData.voidtype2.type2=dataParent.voidtype2.type1;
  //поместить объект в родителя
  ObjToParent((GtkWidget*)pObj,pParent,dataParent.voidtype2.type1);
  }
 //если имя задано   
 if ( GTK_IS_WIDGET(pObj) ) {
  gtk_widget_set_name((GtkWidget*)pObj,pdataName->str.psec);
//  gtk_widget_show ((GtkWidget*)pObj);
  }
 return mshSet(&(winDb.Db),&Inds,&setData);
 }

/*
 */
static Terr getLabelMod(Tdata *pdataLabel,Tdata *pdataMod,void* pUser,Tstr **ppsLabel,Tstr **ppsMod){
 *ppsLabel=castId(pdataLabel);
 if (*ppsLabel==NULL) return CERRvmLabelNotFound;
 if (pdataMod!=NULL){
  *ppsMod=castId(pdataMod);
  if ((*ppsMod==NULL)||((*ppsMod)->size<1)) {
   *ppsMod=winnameMod(pUser);
   }
  }
 else
  *ppsMod=winnameMod(pUser);
 return COutKodOk;
 }
Terr winSetAtrObj(TarrData *pArgs,Tdata *pOut,void* pUser){
 GtkListStore *pListStore;
 Tdata *pdataSet,*pdnameObj;
 Terr err=COutKodOk;
 GObject *pParent,*pObj=NULL;
 EwinTypeObj typeObj;
 EwinTypeAtr typeAtr;
 Tbstr sbAtr;
 Tstr *psLabel=NULL,*psMod=NULL;
 char *pcAtr=NULL;
 char bufAtr[sizeBuf];
 int row;
 char *pC1,*pC2;
 Tbstr bStr1;
 char buf1[sizeBuf],buf2[CsizePacthFl];
 int narg1=3,narg2=narg1+1;
 //
 if ((pArgs==NULL)||(pArgs->busy<3)) return CERRwinArgLittle;
 pObj=getObjAtrtype(pArgs,&typeObj,&typeAtr,&pdnameObj,&err);
 if (pObj==NULL) return err;
 //данные на запись
 pdataSet=pArgs->psec+2;
 //добавить Tab в Nb pcName - содержит имя добавляемой страницы страницы
 if (typeAtr==EwinAtrTabAdd){
  //находим Nb
  pParent=findObj(&(winDb.Db),pdataSet);
  if (pParent==NULL) return CERRwinParentNotFind;
  if (!(GTK_IS_NOTEBOOK(pParent))) return CERRwinParentNoType;
  pC1=pC2=NULL;
  if (pArgs->busy>narg1){
   pC1=strToBuf(pArgs->psec+narg1,&bStr1,buf1,sizeBuf,NULL);
   if (pArgs->busy>narg2){
    wingetPachFl(pArgs->psec+narg2,pCmshExtImg,buf2);
    pC2=buf2;
    }
   }
  (void)winNbBnaddPg((GtkNotebook*)pParent,GTK_WIDGET(pObj),pC1,pC2);
  return err;
  }
 //
 switch(typeObj){
  //Главное окно   
  case EwinWin :
   //Атрибуты
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrTitle :
     pcAtr=strToBuf(pdataSet,&sbAtr,bufAtr,sizeBuf,NULL);
     if (pcAtr) winWinSetTitle((GtkWidget*)pObj,pcAtr);
     break;
    //Размеры окна
    case EwinAtrSize :
     if (pDataIsInt(pdataSet))
      winWinSetSize((GtkWidget*)pObj,pdataSet->int64[0],pdataSet->int64[1]);
     else
      return CERRwinArgNotType;
     break;
    case EwintrgWinOnInit :
     err=getLabelMod(pdataSet,pArgs->psec+3,pUser,&psLabel,&psMod);
     if (err<CERR) return err;
     err=winSetOnInit(GTK_WIDGET(pObj),pdnameObj,psLabel,psMod,qOnModLabInit,pUser);
     break;
    default :
     break;
    }
   break;
  //Notebook
  case EwinNb :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrTitle :
     castbStr(pdataSet,&sbAtr);
     if (sbAtr.size) winRowToNblabel(GTK_NOTEBOOK(pObj),&sbAtr);
     break;
    case EwinAtrFocus :
     castDataInt(pdataSet);
     winNbSetFocus(GTK_NOTEBOOK(pObj),pdataSet->intgr);
     break;
    default :
     break;
    }   
   break;
  //Label 
  case EwinLb :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrData :
    case EwinAtrTitle :
     pcAtr=strToBuf(pdataSet,&sbAtr,bufAtr,sizeBuf,NULL);
     if (pcAtr) winLbSetTitle(GTK_WIDGET(pObj),pcAtr);
     break;
    default :
     break;
    }   
   break;
  //кнопка 
  case EwinBut :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrData :
    case EwinAtrTitle :
     pcAtr=strToBuf(pdataSet,&sbAtr,bufAtr,sizeBuf,NULL);
     if (pcAtr) winButSetTitle(GTK_WIDGET(pObj),pcAtr);
     break;
    case EwinAtrImg :
     wingetPachFl(pdataSet,pCmshExtImg,buf2);
     pcAtr=buf2;
     if (pcAtr) winButSetImage(GTK_WIDGET(pObj),pcAtr);
     break;
    //Позиция Img
    case EwinAtrPos :
     castDataInt(pdataSet);
     winButSetPosition(GTK_WIDGET(pObj),(GtkPositionType)pdataSet->intgr);
     break;
    //событие 
    case EwintrgWinOn :
     err=getLabelMod(pdataSet,pArgs->psec+3,pUser,&psLabel,&psMod);
     if (err<CERR) return err;
     err=winSetOn(GTK_WIDGET(pObj),pdnameObj,"clicked",psLabel,psMod,pUser);
     break;
    default :
     break;
    }   
   break;
  //Box 
  case EwinBox :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrSpacing :
     castDataInt(pdataSet);
     winBoxSetSpacing((GtkBox*)pObj,pdataSet->intgr);
     break;
    default :
     break;
    }   
   break;
  // BoxBut
  case EwinBoxBut :
   break;
  // Scroll
  case EwinSc :
   break;
  // Grid
  case EwinGrid :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrTitle :
     castbStr(pdataSet,&sbAtr);
     if (sbAtr.size) winViewSetHeder(GTK_TREE_VIEW(pObj),&sbAtr);
     break;
    case EwinAtrData :
     castbStr(pdataSet,&sbAtr);
     pListStore=GTK_LIST_STORE( gtk_tree_view_get_model(GTK_TREE_VIEW(pObj)) );
     if (sbAtr.size){
      switch(pArgs->busy){
       //заменить все данные   
       case 3 :
        winViewRowsListStore(pListStore,&sbAtr);
        break;
       //заменить строку
       case 4 :
        castDataInt(pArgs->psec+3);
        winViewGridUpDateRow(pListStore,&sbAtr,pArgs->psec[3].intgr);
        break;
       //заменить ячейку в таблице
       default : 
        castDataInt(pArgs->psec+3);
        row=pArgs->psec[3].intgr;
        castDataInt(pArgs->psec+4);
        winViewGridUpDateCol(pListStore,&sbAtr,row,pArgs->psec[4].intgr);
        break;
       }
      }
     break;
    case EwinAtrDataAdd :
     castbStr(pdataSet,&sbAtr);
     pListStore=GTK_LIST_STORE( gtk_tree_view_get_model(GTK_TREE_VIEW(pObj)) );
     if (sbAtr.size){
      winViewRowsListStoreAdd(pListStore,&sbAtr);
      }
     break;
    case EwinAtrFocus :
     break;
    default :
     break;
    }
   break;
  // Tree 
  case EwinTree :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrTitle :
     castbStr(pdataSet,&sbAtr);
     if (sbAtr.size) winViewSetHeder(GTK_TREE_VIEW(pObj),&sbAtr);
     break;
    case EwinAtrData :
     castbStr(pdataSet,&sbAtr);
     if (sbAtr.size)
      winViewRowsTreeStore(GTK_TREE_STORE( gtk_tree_view_get_model(GTK_TREE_VIEW(pObj)) ),&sbAtr);
     break;
    case EwinAtrDataAdd :
     castbStr(pdataSet,&sbAtr);
     if (sbAtr.size){
      winViewRowsTreeStoreAdd(GTK_TREE_STORE( gtk_tree_view_get_model(GTK_TREE_VIEW(pObj)) ),&sbAtr);
      }
     break;
    default :
     break;
    }
   break;
  // ListStore
  case EwinListStore :
   break;
  // TreeStore
  case EwinTreeStore :
   break;
  //Image
  case EwinImg :
   break;
  // Calendar
  case EwinCalendar :
   switch(typeAtr){
    case EwinAtrTitle :
    case EwinAtrData :
     castbStr(pdataSet,&sbAtr);
     if (sbAtr.size) winCalendarSetData(GTK_WIDGET(pObj),&sbAtr);
     break;
    //событие 
    case EwintrgWinOn :
     err=getLabelMod(pdataSet,pArgs->psec+3,pUser,&psLabel,&psMod);
     if (err<CERR) return err;
     err=winSetOn(GTK_WIDGET(pObj),pdnameObj,"day_selected_double_click",psLabel,psMod,pUser);
     break;
    default :
     break;
    }
   break;
  default :
   return CERRTypeNo;
  } 
 // 
 return err;
 }
/*
 */
Terr winGetAtrObj(TarrData *pArgs,Tdata *pOut,Tarea *pArea){
 Terr err=COutKodOk;
 GObject *pObj=NULL;
 EwinTypeAtr typeAtr;
 EwinTypeObj typeObj;
 int row;
 char *pcBuf;
 Tbstr bStr;
 Tdata *pdnameObj;
 GtkTreeModel *pStore;
 //
 if (winDb.Db.typeDb==EtypeNo) return CERRwinInitNo;
 if (pOut==NULL) return COutKodOk;
 if ((pArgs==NULL)||(pArgs->busy<2)) return CERRwinArgLittle;
 pObj=getObjAtrtype(pArgs,&typeObj,&typeAtr,&pdnameObj,&err);
 if (pObj==NULL) return err;
 //
 switch(typeObj){
  //Главное окно   
  case EwinWin :
   //Атрибуты
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrData :
    case EwinAtrTitle :
     pOut->bstr.psec=winWinGetTitle((GtkWidget*)pObj);
     pOut->bstr.size=strlen(pOut->bstr.psec);
     pDataSetbStr(pOut);
     break;
    //Размеры окна
    case EwinAtrSize :
     pDataSetInt(pOut);
     winWinGetSize((GtkWidget*)pObj,(gint*)(&(pOut->int64[0])),(gint*)(&(pOut->int64[1])) );
     break;
    //Размеры окна
    case EwinAtrPos :
     pDataSetInt(pOut);
     winWinGetPos((GtkWidget*)pObj,(gint*)(&(pOut->int64[0])),(gint*)(&(pOut->int64[1])) );
     break;
    default :
     break;
    }
   break;
  //Notebook
  case EwinNb :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrTitle :
//     pOut->bstr.psec=winWinGetTitle( GTK_WIDGET(pObj) );
//     pOut->bstr.size=strlen(pOut->bstr.psec);
//     pDataSetbStr(pOut);
     break;
    default :
     break;
    }   
   break;
  //Label 
  case EwinLb :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrTitle :
     pOut->bstr.psec=winLbGetTitle( GTK_WIDGET(pObj) );
     pOut->bstr.size=strlen(pOut->bstr.psec);
     pDataSetbStr(pOut);
     break;
    default :
     break;
    }   
   break;
  //кнопка 
  case EwinBut :
   switch(typeAtr){
    //текст на кнопке
    case EwinAtrData :
    case EwinAtrTitle : 
     pOut->bstr.psec=winButGetTitle( GTK_WIDGET(pObj) );
     pOut->bstr.size=strlen(pOut->bstr.psec);
     pDataSetbStr(pOut);
     break;
    //Позиция Img
    case EwinAtrPos :
     pOut->intgr=winButGetPosition(GTK_WIDGET(pObj));
     pDataSetInt(pOut);
     break;
    default :
     break;
    }   
   break;
  //Box 
  case EwinBox :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrPos :
     pOut->intgr=winBoxGetSpacing((GtkBox*)pObj);
     pDataSetInt(pOut);
     break;
    default :
     break;
    }   
   break;
  // BoxBut
  case EwinBoxBut :
   break;
  // Scroll
  case EwinSc :
   break;
  // Grid
  case EwinGrid :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrTitle :
     if (pArgs->busy>3){
      castDataInt(pArgs->psec+3);
      winViewGetColHeder(GTK_TREE_VIEW(pObj),&pcBuf,pArgs->psec[3].intgr);
      bStr.psec=pcBuf;
      bStr.size=strlen(pcBuf);
      pStrTopDataOrpArea(&bStr,pArea,pOut);
      g_free(pcBuf);
      }
     break;
    case EwinAtrData :
     pStore=gtk_tree_view_get_model(GTK_TREE_VIEW(pObj));
     switch(pArgs->busy){
      //заменить все данные   
      case 3 :
       winViewGridGet(pStore,pArea);
       pOut->pstr=(Tstr*)(pArea->psec);
       pDataSetpStr(pOut);
       break;
      //получить строку из таблицы
      case 4 :
       castDataInt(pArgs->psec+3);
       winViewGridGetRow(pStore,pArea,pArgs->psec[3].intgr);
       pOut->pstr=(Tstr*)(pArea->psec);
       pDataSetpStr(pOut);
       break;
      //получить ячейку из таблицы
      default : 
       castDataInt(pArgs->psec+3);
       row=pArgs->psec[3].intgr;
       castDataInt(pArgs->psec+4);
       winViewGridGetCol(pStore,&pcBuf,row,pArgs->psec[4].intgr);
       bStr.psec=pcBuf;
       bStr.size=strlen(pcBuf);
       pStrTopDataOrpArea(&bStr,pArea,pOut);
       g_free(pcBuf);
       break;
      }
     break;
    case EwinAtrFocus :
     if (pArgs->busy>2){
      castDataInt(pArgs->psec+2);
      pcBuf=selfocusCol(GTK_TREE_VIEW(pObj),pArgs->psec[2].intgr);
      if (pcBuf){
       bStr.psec=pcBuf;
       bStr.size=strlen(pcBuf);
       pStrTopDataOrpArea(&bStr,pArea,pOut);
       g_free(pcBuf);
       }
      }
     break;
    default :
     break;
    }
   break;
// Tree 
  case EwinTree :
   switch(typeAtr){
    //заголовок окна 
    case EwinAtrTitle :
     break;
    case EwinAtrFocus :
     if (pArgs->busy>2){
      castDataInt(pArgs->psec+2);
      pcBuf=selfocusCol(GTK_TREE_VIEW(pObj),pArgs->psec[2].intgr);
      if (pcBuf){
       bStr.psec=pcBuf;
       bStr.size=strlen(pcBuf);
       pStrTopDataOrpArea(&bStr,pArea,pOut);
       g_free(pcBuf);
       }
      }
     break;
    default :
     break;
    }
   break;
  // ListStore
  case EwinListStore :
   break;
  // TreeStore
  case EwinTreeStore :
   break;
  //Image 
  case EwinImg :
   break;
  // Calendar
  case EwinCalendar :
   switch(typeAtr){
    case EwinAtrTitle :
    case EwinAtrData :
     winCalendarGetData(GTK_WIDGET(pObj),&(pOut->str));
     pDataSetStr(pOut);
     break;
    default :
     break;
    }
   break;
  //добавить Tab в Nb pcName - содержит имя добавляемой страницы страницы
  case EwinTab :
   break;
  case EwinEnd :
   return CERRTypeNo;
  default :
   break;
  } 
 return err;
 }
/*
 */
static Terr iniToAtr(TarrData *pArgs,Tdata *pdataIni,void* pUser){
 Tdata *pdataSet,*pdnameObj;
 Terr err=COutKodOk;
 Tbstr sbIni;
 TbKeyVal *pbKeyVal;
 //
 pdnameObj=pArgs->psec+1;
 pdataSet=pArgs->ptop;
 castbStr(pdataIni,&sbIni);
 err=iniDirReadFL(&sbIni,pCmshDirIni,&(winDb.Ini));
 parrIter(&(winDb.Ini),pbKeyVal){
  pdnameObj->bstr=pbKeyVal->key;
  pdataSet->bstr=pbKeyVal->val;
  pDataSetbStr(pdnameObj);
  pDataSetbStr(pdataSet);
  err=winSetAtrObj(pArgs,NULL,pUser);
  }
 //
 return err;
 }
/*
 * pArgs[0]=имя атрибута, дальше идут имена Ini файлов
 * Arg[0]- имя объекта, Arg[1]- имя атрибута, Arg[2]- значение атрибута 
 */
Terr winIniToAtr(TarrData *pArgs,Tdata *pOut,void* pUser){
 Tdata dataArr[3];
 TarrData Arg;
 Terr err=COutKodOk;
 Tusize j;
 //
 if ((pArgs==NULL)||(pArgs->busy<2)) return CERRwinArgLittle;
 //имя ини файла с атрибутами на запись
 memNULL(&Arg);
 Arg.psec=dataArr;
 Arg.size=Arg.busy=3;
 arrsetptop(Arg);
 dataArr[0]=pArgs->psec[0];
 DataSetbStr(dataArr[1]);
 DataSetbStr(dataArr[2]);
 for (j=1;j<pArgs->busy;++j){
  err=iniToAtr(&Arg,pArgs->psec + j,pUser);
  }
 return err;
 }
/*
 */
Terr winAtrToIni(TarrData *pArgs,Tdata *pOut,Tarea *pArea){
 Tdata dataArr[2];
 TarrData Arg;
//    
 Tdata dataAtr,*pdnameObj;
 Terr err=COutKodOk;
 Tbstr sbIni,sbSet;
 TbKeyVal *pbKeyVal;
 Tdiv fFl;
 char bf[CsizePacthFl];
 char Eq=CvmIniEqu;
 char BK=CCvmEndStr;
 char Es=CvmIniendStr;
 int sum=0;
 //
 if ((pArgs==NULL)||(pArgs->busy<2)) return CERRwinArgLittle;
 //имя ини файла с атрибутами на запись
 castbStr(pArgs->psec+1,&sbIni);
 err=iniDirReadFL(&sbIni,pCmshDirIni,&(winDb.Ini));
 if (err<CERR) return err;
 fileDirPacht(&sbIni,pCmshDirIni,bf);
 if ((fFl=divCreat(bf))==CdivRetErr) return CERRvmDivFiled;
 memNULL(&Arg);
 Arg.psec=dataArr;
 Arg.size=Arg.busy=2;
 arrsetptop(Arg);
 dataArr[0]=pArgs->psec[0];
 DataSetpbStr(dataArr[1]);
 pdnameObj=pArgs->psec+1;
 parrIter(&(winDb.Ini),pbKeyVal){
  pdnameObj->pbstr=&(pbKeyVal->key);
  err=winGetAtrObj(&Arg,&dataAtr,pArea);
  if (err>CERR){
   castbStr(&dataAtr,&sbSet);
   if (sbSet.size){
    if ((divWrite(fFl,pbKeyVal->key.psec,pbKeyVal->key.size))<pbKeyVal->key.size) {err=CERRvmDivFiled;goto finish;}
    divWrite(fFl,&Eq,1);
    divWrite(fFl,sbSet.psec,sbSet.size);
    divWrite(fFl,&Es,1);
    divWrite(fFl,&BK,1);
    sum++;
    }
   }
  }
 if (pOut){
  pOut->intgr=sum;
  pDataSetInt(pOut);
  }
finish: 
 divClose(fFl);
 return err;
 }
/**/

Tdiv winChannelOpen(TarrData *pArgs,int *perr){
 Tbstr bStr;
 //
 if ((pArgs==NULL)||(pArgs->busy<1)) {*perr=CERRwinArgLittle;return CdivRetErr;}
 castbStr(pArgs->psec,&bStr);
 if (bStr.size<3) {*perr=CERRHostPortFiled;return CdivRetErr;}
 //pArgs->psec
 if (( winDb.pIOChannel=ClntGtkInitChannel( &(winDb.Clnt),&bStr , perr,mClntUsrDef))==NULL) {*perr=CWARchanelFailed;return CdivRetErr;}
 if (winDb.pIOChannel==NULL) {*perr=CERRconnectFiled; return CdivRetErr;}
 winDb.Clnt.pData=&winDb;
// winSetChannel(winDb.Clnt.sctrw,bufSend,pUser);
 *perr=COutKodOk;
 return winDb.Clnt.sctrw;
 }
/*
 */
Terr startWin(GtkWidget *pObj);

Terr winStart(TarrData *pArgs){
 GObject *pObj;
 Terr err;
 //
 if ((pArgs==NULL)||(pArgs->busy<1)) return CERRwinArgLittle;
 pObj=getObj(pArgs->psec,&err);
 if (pObj==NULL) return err;
 if (!(GTK_IS_WIDGET(pObj))) return CERRwinObjNotType;
 return startWin((GtkWidget*)pObj);
 }