
/* 
 * File:   netCore.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 5 ноября 2016 г., 11:51
 */

#include "Net.h"

/*
 * отправка сообщений
 */
/*
GIOStatus sendMsg(GIOChannel   *pChannel,Tbstr *pbsBf){
 GIOStatus st;
 GIOCondition con;
 gsize w=0;
 GError *perror=NULL;
 // 
 con=g_io_channel_get_buffer_condition(pChannel);
 if (con && G_IO_OUT){
  st=g_io_channel_write_chars(pChannel,pbsBf->psec,pbsBf->size,&w,&perror);
  if (st==G_IO_STATUS_NORMAL) st=g_io_channel_flush(pChannel,&perror);
  }
 return st;
 }
*/
//
int bufSend(Tdiv div,Tbstr *pbsBf){
 //отправляем само сообщение
 if ( netSendN(div,pbsBf->psec,pbsBf->size) ) {
  flush(div);
  return COutKodOk;
  }
 return CERRSendFiled;
 }
int srvOut(TdivSrv div,Tbstr *pbsBf){
 //отправляем само сообщение
// if ( netSendN(div.TCP,pbsBf->psec,pbsBf->size) ) {  flush(div.TCP);  return COutKodOk;  }
 if (FCGX_PutStr(pbsBf->psec,pbsBf->size, div.pWWW)>0) return COutKodOk;
 return CERRSendFiled;
 }
/*
 * Чтение буфера
 */
/*
int bufReadMsg(Tdiv div,Tbuf *pBf){
 int rt,n=511;
 char Bf[n+1],*pC;
 //
 pBufClear(pBf);
 pC=pBf->psec;
 do{
  rt = (ReadN(div,Bf ,n) );
  if (rt>0){
   pareaCharBusyTop(pBf,rt,pC);
   memCopy(pC,Bf,rt);
   }
  } while(rt>=n);
 pBf->psec[pBf->busy]='\0';
 return COutKodOk;
 }
*/
///*
int bufReadMsg(Tdiv div,Tbuf *pBf){
 int rt,n=CsizeBuf,w=n-1;
 gchar *pC;
 //
 pBufClear(pBf);
 do{
  pareaWide(pBf,n)
  pC=pareaTop(pBf);
  rt = (netReadN(div,pC ,w) );
  if (rt>0) pBf->busy += rt;
  else
   if (pBf->busy)
    break;
   else
    return COutEOF;
  } while(rt>=w);
 if (pBf->psec[pBf->busy-1]==Cvm0)
  pBf->busy--;
 else
  pBf->psec[pBf->busy]=Cvm0;
 return COutKodOk;
 }
/*
int bufReadMsg(Tdiv div,Tbuf *pBf){
 int rt,n=513,w=n-1;
 gchar *pC;
 //
 pBufClear(pBf);
 do{
  pareaWide(pBf,n)
  pC=pareaTop(pBf);
  rt = (ReadN(div,pC ,w) );
  if (rt>0) pBf->busy += rt;
  else
   if (pBf->busy)
    break;
   else
    return COutEOF;
  } while(rt>=w);
 pBf->psec[pBf->busy]='\0';
 return COutKodOk;
 }
*/
/*
int bufReadChannelMsg(GIOChannel *pChannel,Tbuf *pBf){
 GError *perror;
 GIOStatus st;
 gsize rt,n=511;
 char Bf[n+1],*pC;
 //
 pBufClear(pBf);
 pC=pBf->psec;
 do{
  st=g_io_channel_read_chars(pChannel,Bf,n,&rt,&perror);
  if ((st == G_IO_STATUS_NORMAL)&&(rt>0)){
   pareaCharBusyTop(pBf,rt,pC);
   memCopy(pC,pBf,rt);
   }
  } while(st != G_IO_STATUS_EOF);
 pBf->psec[pBf->busy]='\0';
 return COutKodOk;
 }
*/
GIOStatus bufReadChannelMsg(GIOChannel *pChannel,Tbuf *pBf){
 GError *perror=NULL;
 GIOStatus st;
 gsize rt,n=513,w=n-1;
 gchar *pC;
 //
 pBufClear(pBf);
 do{
  pareaWide(pBf,n)
  pC=pareaTop(pBf);
  st=g_io_channel_read_chars(pChannel,pC,w,&rt,&perror);
  if (st == G_IO_STATUS_EOF) return st;
  if ((st == G_IO_STATUS_NORMAL)&&(rt>0)) {
   pBf->busy += rt;
   }
  } while(rt>=w);
 pBf->psec[pBf->busy]='\0';
 return st;
 }
/*
int bufReadMsg1(Tdiv div,Tbuf *pBuf){
 int64_t w,len;
 int err=COutKodOk;
 char *pb;
 //
 pBufClear(pBuf);
 w=pBuf->size;
 pBuf->size= (ReadN(div,pBuf->psec ,w) );
 if ( pBuf->size < w ) return err;
 if ( ioctl(div,FIONREAD,&len)<0 ) return CERRReadFiled;
 if (len){
  pBuf->size +=len;
  w=len+1;
  pBufBusyTop(pBuf,w,pb);
  w=len;
  while ( len>0 ) {
   w= ( ReadN(div,pb,len) );
   pb +=w;
   len -= w;
   }
  pBuf->psec[pBuf->size]='\0';
  }
 return COutKodOk;
 }
*/
/*
int bufReadMsg(Tdiv div,Tbuf *pBf){
 gsize len,w;
 int rt;
 //
 pBufClear(pBf);
 if ( (rt=ioctl(div,FIONREAD,&len))<0 ) return CERRReadFiled;
 if (len>0) {
  w=len + 1;
  pareaWide(pBf,w)
  pBf->size= (ReadN(div,pBf->psec ,len) );
  }
 pBf->psec[pBf->size]='\0';
 return COutKodOk;
 }

int bufReadChannelMsg1(GIOChannel *pChannel,Tbuf *pBf){
 int div;
 div=g_io_channel_unix_get_fd(pChannel);
 return bufReadMsg(div,pBf);
 }

int bufReadChannelMsg(GIOChannel *pChannel,Tbuf *pBf){
 gsize len,w;
 GError *perror;
 GIOStatus st;
 int div,rt;
 //
 pBufClear(pBf);
 div=g_io_channel_unix_get_fd(pChannel);
 if ( (rt=ioctl(div,FIONREAD,&len))<0 ) return CERRReadFiled;
 if (len>0) {
  w=len + 1;
  pareaWide(pBf,w)
  st=g_io_channel_read_chars(pChannel,pBf->psec,len,&w,&perror);
  if (st == G_IO_STATUS_EOF) return COutKodOk;
  if ((st != G_IO_STATUS_NORMAL)||(w<1))
   pBf->busy = 0;
  else
   pBf->busy=w;
  }
 pBf->psec[pBf->busy]='\0';
 return COutKodOk;
 }
*/
/*
 * Обработчик сигнала
 */
/*
int bufReadGtkMsg3(GIOChannel *pChannel,TBuf *pBuf){
 GIOCondition Cond;
 GIOStatus st;
 gchar *pbf;
 gsize len;
 GError *perror;
 //
 pBufClear(pBuf);
 Cond=g_io_channel_get_buffer_condition(pChannel);
 if ( (Cond==G_IO_IN)||(Cond==G_IO_PRI) ) {
  st=g_io_channel_read_to_end(pChannel,&pbf,&len,&perror);
  if (st == G_IO_STATUS_ERROR) return CERRReadFiled;
  if (st == G_IO_STATUS_AGAIN) return COutKodOk;
  if (len){
   pBufBusy(pBuf,len);
   pBuf->sbuf.size=len;
   memCopy(pBuf->sbuf.psec,pbf,len);
   }
  g_free(pbf);
  }
 return COutKodOk;
 }
int bufReadGtkMsg1(GIOChannel *pChannel,TBuf *pBuf){
 gsize len,wb,free;
 GError *perror;
 GIOStatus st;
 char *pb;
 //
 pBufClear(pBuf);
 wb=free=pBuf->size * sizeof(*(pBuf->psec));
 pb=pBuf->sbuf.psec;
 do{
  if ( free < 10 ){
   wb +=128;
   free +=128;
   pBuf->sbuf.psec=memChange(pBuf->sbuf.psec,wb );
   }
  pb=pBuf->sbuf.psec + wb - free;
  st=g_io_channel_read_chars(pChannel,pb,free,&len,&perror);
  if (st == G_IO_STATUS_ERROR) return CERRReadFiled;
  if ( len > 0 ) {
   free -= len;
   }
  }while(st != G_IO_STATUS_EOF);
 pBuf->psec=(Tsize*)(pBuf->sbuf.psec);
 if (pBuf->sbuf.size<wb) {
  pBuf->sbuf.size=wb;
  pBuf->size=mshDivUp(wb,sizeof(*(pBuf->psec)) );
  }
 return COutKodOk;
 }
int bufReadGtkMsg4(GIOChannel *pChannel,TBuf *pBuf){
 gsize len,w;
 GError *perror;
 GIOStatus st;
 char *pb;
 int div;
 //
 w=pBuf->size * sizeof(*(pBuf->psec));
 st=g_io_channel_read_chars(pChannel,pBuf->sbuf.psec,w,&len,&perror);
 if (st != G_IO_STATUS_NORMAL) return CERRReadFiled;
 pBuf->sbuf.size=len;
 if (( len < w )||(st==G_IO_STATUS_EOF)) return COutKodOk;
 div=g_io_channel_unix_get_fd(pChannel);
 if ( ioctl(div,FIONREAD,&len)<0 ) return CERRReadFiled;
 if (len){
  pBuf->sbuf.size +=len;
  w=len+1;
  pBufBusyTop(pBuf,w,pb);
  w=len;
  while ( len>0 ) {
   st=g_io_channel_read_chars(pChannel,pb,len,&w,&perror);
   if ((st == G_IO_STATUS_NORMAL)||(st == G_IO_STATUS_EOF)) {
    if (w){
     pb +=w;
     len -= w;
     }
    if (st == G_IO_STATUS_EOF) return CWARsrvReadEnd;
    }
   }
  pBuf->sbuf.psec[pBuf->sbuf.size]='\0';
  }
 return COutKodOk;
 }
//
int bufReadGtkMsg5(GIOChannel *pChannel,TBuf *pBuf){
 gsize len,w,busy;
 GError *perror;
 GIOStatus st;
 int div;
 char *pb;
 //
 pBufClear(pBuf);
 div=g_io_channel_unix_get_fd(pChannel);
 pb=pBuf->sbuf.psec;
 while(1){
  if ( ioctl(div,FIONREAD,&len)<0 ) return CERRReadFiled;
  if (len>0) {
   busy=pBuf->sbuf.size + len + 1;
   pareaBusy(pBuf,busy);
   st=g_io_channel_read_chars(pChannel,pb,len,&w,&perror);
   if (st == G_IO_STATUS_EOF) return COutKodOk;
   if ((st == G_IO_STATUS_NORMAL)&&(w>0)) {
    pBuf->sbuf.size += w;
    pb += w;
    }
   }
  }
 pBuf->busy=mshDivUp(pBuf->sbuf.size,sizeof(*(pBuf->psec)) );
 pBuf->sbuf.psec[pBuf->sbuf.size]='\0';
 return COutKodOk;
 }

int bufReadGtkMsg(GIOChannel *pChannel,TBuf *pBuf){
 gsize len,w;
 GError *perror;
 GIOStatus st;
 int div,rt;
 //
 pBufClear(pBuf);
 div=g_io_channel_unix_get_fd(pChannel);
 if ( (rt=ioctl(div,FIONREAD,&len))<0 ) return CERRReadFiled;
 if (len>0) {
  w=len + 1;
  pBufBusy(pBuf,w);
  st=g_io_channel_read_chars(pChannel,pBuf->sbuf.psec,len,&w,&perror);
  if (st == G_IO_STATUS_EOF) return COutKodOk;
  if ((st == G_IO_STATUS_NORMAL)&&(w>0))
   pBuf->sbuf.size = w;
  else
   return CERRReadFiled;
  }
 pBuf->busy=mshDivUp(pBuf->sbuf.size,sizeof(*(pBuf->psec)) );
 pBuf->sbuf.psec[pBuf->sbuf.size]='\0';
 return COutKodOk;
 }
*/
