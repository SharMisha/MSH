
/* 
 * File:   mshGuide.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 13 октября 2016 г., 11:49
 */

#include "mshMonC.h"
//
/*
 * Управление через сигналы
 */
//
int bufSend(Tdiv div,Tbstr *pBuf);
//GIOStatus sendMsg(GIOChannel   *pChannel,Tbstr *pbsBf);
/*
*/
//GIOStatus bufReadChannelMsg(GIOChannel *pChannel,Tbuf *pBf);
int bufReadMsg(Tdiv div,Tbuf *pBf);
void winViewGridUpDateRow(GtkListStore *pStore,Tbstr *pbstrBuf,int y);
void winViewRowsListStore(GtkListStore *pStore,Tbstr *pbstrBuf);
int iniDirInit(Tbstr *pbfl,const char *pDir,TIni *pIni);
GObject* winWinNew(char *ptitle);
void winWinSetSize(GtkWidget *pObj,gint setWidth,gint setHeight);
void winViewSetHeder(GtkTreeView *pView,Tbstr *pbstrBuf);
int iniDirInit(Tbstr *pbfl,const char *pDir,TIni *pIni);
GObject* winGridNew(GtkTreeModel *pmodel,int cols);
/**/
static int guideNetSend(char *pMsg){
 Tbstr Buf;
// GIOStatus st;
 //
 Buf.psec=pMsg;
 Buf.size=strlen(pMsg);
// Buf.size=strlen(pMsg)+1;
// return sendMsg(Guide.pIOChannel,&Buf);
 return bufSend(Guide.Clnt.sctrw,&Buf);
 }

gboolean readBufClnt(GIOChannel *pChannel, GIOCondition condition, gpointer data){
 int err;
 int y;
 char *pC;
 int numP;
 Tbstr bsBf;
// GIOStatus st;
 Tdiv div;
 //
 div=g_io_channel_unix_get_fd (pChannel);
 err=bufReadMsg(div,&(Guide.Clnt.buf));
 if (err!=COutKodOk) return FALSE;
// st=bufReadChannelMsg(pChannel,&(Guide.Clnt.buf));
// if (st==G_IO_STATUS_EOF) return FALSE;
// if (Guide.Clnt.buf.busy==0) return FALSE; //TRUE;
 pC=Guide.Clnt.buf.psec;
 AreaTobStr(Guide.Clnt.buf,bsBf);
 bsBf.psec++;
 bsBf.size--;
 numP=((*pC==CsrvCodGuideClnt)||(*pC==CsrvCodGuideClnt1))?1:0 ;
 switch (*pC){
  //пришла таблица описания серверов
  case CsrvCodGuideSrv :
  //пришла таблица описания клиентов
  case CsrvCodGuideClnt :
   if (Guide.numPg==numP) {
    winViewRowsListStore(Guide.pStore[Guide.numPg],&bsBf);
    }
   break;
  //пришла строка описания сервера
  case CsrvCodGuideSrv1 :
  //пришла строка описания клиента
  case CsrvCodGuideClnt1 :
   if (Guide.numPg==numP) {
    y=atoi(bsBf.psec);
    //начало описания сервера
    if ((pC=memFindChar(bsBf.psec+1,0,bsBf.size-1))){
     ++pC;
     bsBf.size -= (pC - bsBf.psec);
     bsBf.psec=pC;
     winViewGridUpDateRow(Guide.pStore[Guide.numPg],&bsBf,y);
     }
    }
   break;
  }
 return TRUE;
 }

//----------------------------------
//
static int getIdClnt(gchar *pmsg,gchar **ppsId,gchar **ppsNm){
 GtkTreeSelection *psel;
 GtkTreeIter iter;
 //
 psel=gtk_tree_view_get_selection( (GtkTreeView*)(Guide.pPage[0]) );
 //ничего не выбрано
 if ( gtk_tree_selection_count_selected_rows(psel) ) {
  int w;
  //
  gtk_tree_selection_get_selected(psel,(GtkTreeModel**)(&(Guide.pStore[0])),&iter); //
  //
  gtk_tree_model_get((GtkTreeModel*)(Guide.pStore[0]), &iter, 0, ppsId,-1);
  if (ppsNm) gtk_tree_model_get((GtkTreeModel*)(Guide.pStore[0]), &iter, 1,ppsNm,-1);
  w=strlen(*ppsId);
  memCopy(pmsg+1,*ppsId,w);
  pmsg[w+1]='\0';
  return COutKodOk;
  }
 return CERRguideNotSel;
 }
//
static void butgetpgClient(GtkButton *pbut){
 gchar *psId,*psNm,msg[2];
 //
 if (getIdClnt(msg,&psId,&psNm)<CERR) return;
 msg[0]=CsrvCodGuideClnt;
 msg[1]=Cvm0;
 // guideNetChannelSend(msg);
 guideNetSend(msg);
 gtk_label_set_text(Guide.plbIdSrv,psId );
 gtk_label_set_text(Guide.plbNameSrv,psNm);
 g_free(psId);
 g_free(psNm);
 //
 Guide.numPg=1;
 gtk_notebook_set_current_page(Guide.pNb,Guide.numPg);
 }
static void msgSrvId(char cCom){
 char *psId,msg[2];
 if (getIdClnt(msg,&psId,NULL)<CERR) return;
 msg[0]=cCom;
 msg[1]=Cvm0;
 guideNetSend(msg);
 }
static void msgSrvAll(char cCom){
 gchar msg[3];
 msg[0]=cCom;
 msg[1]=CsrvCodStopALL;
 msg[2]=Cvm0;
 guideNetSend(msg);
 }
//
static void butstopSrv(GtkButton *pbut){
 msgSrvId(CsrvCodStop);
 }
static void butstopSrvAll(GtkButton *pbut){
 msgSrvAll(CsrvCodStop);
 }
static void butstartSrv(GtkButton *pbut){
 msgSrvId(CsrvCodReStart);
 }
static void butstartSrvAll(GtkButton *pbut){
 msgSrvAll(CsrvCodReStart);
 }
//
static void butCmd(char cCom){
 char msg[2];
 msg[0]=cCom;
 msg[1]=Cvm0;
 //подключение клиента к сети
 guideNetSend(msg);
 }
//
static void butViewServer(GtkButton *pbut){
 butCmd(CsrvCodGuideSrv);
 }
//
static void butgetpgServer(GtkButton *pbut){
 butCmd(CsrvCodGuideSrv);
 Guide.numPg=0;
 gtk_notebook_set_current_page(Guide.pNb,Guide.numPg);
 }
static void butstopClnt(GtkButton *pbut){
 msgSrvId(CsrvCodStopClnt);
 }
static void butstopClntAll(GtkButton *pbut){
 msgSrvAll(CsrvCodStopClnt);
 }
//
static void butViewClnt(GtkButton *pbut){
 butCmd(CsrvCodViewClnt);
 }
//
void ClntGtkFree(TClnt *pClnt);
//
static void butexitServer(GtkButton *pbut){
 msgSrvAll(CsrvCodExit);
 gtk_main_quit();
 ClntGtkFree(&(Guide.Clnt));
 exit (EXIT_SUCCESS);
 }

GtkWidget* winGuideNew(int *argc, char ***argv){
 GtkWidget *pwin,*pnbox,*pbuts,*pbut;
 char *plabbut[]={"Выбор","Возврат","Остановить","Остановить все","Старт","Старт все","Обновить","Остановить сервер","ID Сервера","Сервер"};
 char plabTree0[]="<ID Сервера\tСервер\tСостояние\tПоток\tSoket\tПорт\tПрограмма>";
 char plabTree1[]="<ID Клиента\tМодуль\tСостояние\tПоток\tSoket";
 Tbstr bstrTree0,bstrTree1,bStr;
 char* ptitleWin="Монитор состояний серверов";
 char pfini[]="GuideTxt.ini";
 char *pname[]={"bnsSel","bnsRet","bnsStop","bnsStopALL","bnsStart","bnsStartALL","bnsUpdate","bnsExit","bnsIdSrv","bnsNameSrv"
         ,"hedSrv","hedClnt","titleWin"};
 int lname=sizeof(pname) / sizeof(char*);
 int j;
 TbKeyVal *pbKeyVal;
 //
 bstrTree0.psec=plabTree0;
 bstrTree0.size=strlen(plabTree0);
 bstrTree1.psec=plabTree1;
 bstrTree1.size=strlen(plabTree1);
 bStr.psec=pfini;
 bStr.size=strlen(pfini);
 if ( iniDirInit(&bStr,pCmshDirIni,&(Guide.iniTxt))>CERR ) {
  arrIter(Guide.iniTxt,pbKeyVal){
   for (j=0;j < lname;++j) {
    if ( (strlen(pname[j])==pbKeyVal->key.size )&&( (memCompare(pname[j],pbKeyVal->key.psec,pbKeyVal->key.size))==0 ) ){
     switch (j){
      //пришла таблица описания серверов
      case 0 ... 9 :
       plabbut[j]=pbKeyVal->val.psec;
       break;
      case 10 :
       bstrTree0=pbKeyVal->val;
       break;
      case 11 :
       bstrTree1=pbKeyVal->val;
       break;
      default : 
       ptitleWin=pbKeyVal->val.psec;
       break;
      }
     break;
     }
    } //for
   } //arrIter
  //
  }
 pwin=(GtkWidget*)winWinNew(ptitleWin);
 winWinSetSize(pwin,600,800);
 //страницы
 Guide.pNb=(GtkNotebook*)gtk_notebook_new();
 //Табы удалить
 gtk_notebook_set_show_tabs(Guide.pNb,FALSE);
 //страница сервера
 //На странице вертикальный контейнер
 pnbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
 gtk_container_add((GtkContainer*)Guide.pNb,pnbox);
 //панель кнопок
 pbuts=gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
 gtk_button_box_set_layout((GtkButtonBox*)pbuts,GTK_BUTTONBOX_START); 
 //добавить кнопки на панель
 gtk_container_add((GtkContainer*)pnbox,(GtkWidget*)pbuts );
 //кнопки
 MnbButLab(pbut,plabbut[0]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butgetpgClient), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //
 MnbButLab(pbut,plabbut[2]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butstopSrv), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //
 MnbButLab(pbut,plabbut[3]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butstopSrvAll), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //
 MnbButLab(pbut,plabbut[4]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butstartSrv), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //
 MnbButLab(pbut,plabbut[5]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butstartSrvAll), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //
 MnbButLab(pbut,plabbut[6]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butViewServer), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //
 MnbButLab(pbut,plabbut[7]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butexitServer), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //таблица с данными
 Guide.pPage[0]=(GtkTreeView*)winGridNew(NULL,7);
 winViewSetHeder(Guide.pPage[0],&bstrTree0);
 Guide.pStore[0]=(GtkListStore*)gtk_tree_view_get_model(Guide.pPage[0]);

 gtk_container_add((GtkContainer*)pnbox,(GtkWidget*)(Guide.pPage[0]));
 //страница клиента
 //На странице вертикальный контейнер
 pnbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
 //панель кнопок
 pbuts=gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
 gtk_button_box_set_layout((GtkButtonBox*)pbuts,GTK_BUTTONBOX_START); 
 //кнопки
 MnbButLab(pbut,plabbut[1]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butgetpgServer), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 
 Guide.plbIdSrv=(GtkLabel*)gtk_label_new(plabbut[8]);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)(Guide.plbIdSrv) );
 Guide.plbNameSrv=(GtkLabel*)gtk_label_new(plabbut[9]);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)(Guide.plbNameSrv) );
 //кнопки
 MnbButLab(pbut,plabbut[2]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butstopClnt), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //
 MnbButLab(pbut,plabbut[3]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butstopClntAll), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //
 MnbButLab(pbut,plabbut[6]);
 g_signal_connect (pbut,"clicked",G_CALLBACK(butViewClnt), NULL);
 gtk_container_add((GtkContainer*)pbuts,(GtkWidget*)pbut );
 //
 gtk_container_add((GtkContainer*)pnbox,pbuts);
 Guide.pPage[1]=(GtkTreeView*)winGridNew(NULL,5);
 winViewSetHeder(Guide.pPage[1],&bstrTree1);
Guide.pStore[1]=(GtkListStore*)gtk_tree_view_get_model(Guide.pPage[1]);
 gtk_container_add((GtkContainer*)pnbox,(GtkWidget*)(Guide.pPage[1]));
 gtk_container_add((GtkContainer*)Guide.pNb,pnbox);
 //  
 gtk_container_add((GtkContainer*)pwin,(GtkWidget*)(Guide.pNb));
 Guide.numPg=0;
 return  (GtkWidget*)pwin;
 }
/*
 * 
 */

GIOChannel* ClntGtkInit(TClnt *pClnt,char *phost, int  Port , int *perr ,int (*pfwork)(TClnt *pClnt) );

int mshmonInit(char *phostport){
 GtkWidget *pwin;
 int err;
 int  Port,w;
 char *pP=NULL,*pH=NULL;;
 char *phost="127.0.0.1";
 //создать окно
 Port=0;
 w=strlen(phostport);
 if (w<2)
  return CERRPortFiled;
 if (phostport[0]==CdelemiterHostPort){
  pP=phostport+1;
  pH=phost;
  }
 else{
  pP=memFindChar(phostport,CdelemiterHostPort,w);
  if (pP==NULL) {
   return CERRPortFiled;
   }
  pH=phostport;
  *pP=Cvm0;
  pP++;
  }
 if (pP==NULL) {
  return CERRPortFiled;
  }
 Port=StrToInt(pP);
 //
 if ((pwin=winGuideNew(NULL,NULL))==NULL) return CERRguideNotWin;
 //подключение клиента к сети
 Guide.pIOChannel=ClntGtkInit(&(Guide.Clnt),pH, Port , &err,NULL);
 if (Guide.pIOChannel==NULL) return CERRconnectFiled;
 //запуск главного цикла
 if ( err > CERR ) {
  mwinstartMainChannel(pwin,Guide.pIOChannel,readBufClnt,NULL);
  }
 //освобождение ресурсов
 iniFree(Guide.iniTxt);
 ClntGtkFree(&(Guide.Clnt));
 return err;
 }
