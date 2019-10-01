/* 
 * File:   winWidget.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 14 Август 2015 г., 16:37
 */

#ifndef WIDGET_H
#define	WIDGET_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "Net.h"
#include "mshIni.h"
    
typedef struct {
 TDb Db;
 TClnt Clnt;
 GIOChannel *pIOChannel;
 Tarea Area;
 TIni Ini;
 } TwinDb;


#define mwinstartMain(pwin) gtk_widget_show_all(pwin); gtk_main()
 
#define mwinstartMainChannel(pwin,pIOChannel,readBufClnt,pData) (void)g_io_add_watch(pIOChannel, G_IO_IN , readBufClnt , pData);  mwinstartMain(pwin);   g_io_channel_shutdown(pIOChannel,TRUE,NULL)


#define MnbButLab(but,lab) but=gtk_button_new_with_label(lab)
    
#define MnbButImg(but,imfl) gtk_button_set_image((GtkButton*)but,gtk_image_new_from_file(imfl));\
gtk_button_set_image_position((GtkButton*)but,GTK_POS_LEFT);\
gtk_button_set_always_show_image((GtkButton*)but,TRUE)
    
#define MnbBut(but,lab,imfl) MnbButLab(but,lab); if (imfl) {MnbButImg(but,imfl);}

#define MnbButToggleLab(but,lab) but=gtk_toggle_button_new_with_label(lab);\
 gtk_toggle_button_set_mode((GtkToggleButton*)but,FALSE);\
 gtk_toggle_button_set_active((GtkToggleButton*)but,FALSE);\
 gtk_toggle_button_set_inconsistent((GtkToggleButton*)but,FALSE)
    
#define MnbButToggle(but,lab,imfl) MnbButToggleLab(but,lab); if (imfl) {MnbButImg(but,imfl);}

#define MnbLb(slab) (GtkLabel*)gtk_label_new(slab)

#define	smbSpaceSkip(pcIn,pcEnd) while ((*(pcIn)==' ')&&((trSmbNext(pcIn))<pcEnd))

#define getModel(pView) (GtkTreeModel*)gtk_tree_view_get_model(pView)

#define typeCells(cols,types) ({int j;for (j=0;j<cols;j++) types[j]=G_TYPE_STRING;})
#define modelListStore(cols,types) gtk_list_store_newv(cols,types)
#define modelTreeStore(cols,types) gtk_tree_store_newv(cols,types)

#define mwinNbLbaddPg(pNb,pPg,slab) (void)gtk_notebook_append_page( pNb,pPg,gtk_label_new(slab) )

#define vmwinNbImgaddPg(pNb,pPg,pimgfl) (void)gtk_notebook_append_page(pNb,pPg,gtk_image_new_from_file(pimgfl) )
/*
 */
#define AtrToDataSet(Data,sAtr) DataSetStr(Data); (Data).str.size=strlen(sAtr); memCopy((Data).str.psec,sAtr,(Data).str.size)
#define AtrTopDataSet(pData,sAtr) pDataSetStr(pData); (pData)->str.size=strlen(sAtr); memCopy((pData)->str.psec,sAtr,(pData)->str.size)
//
#define AtrTopData(pData,sAtr) pDataSetbStr(pData); (pData)->bstr.psec=sAtr; (pData)->bstr.size=strlen(sAtr)
#define AtrToData(Data,sAtr) DataSetbStr(Data); (Data).bstr.psec=sAtr; (Data).bstr.size=strlen(sAtr)
#define winnameMod(pUser) &( ((TvmDo*)(pUser))->pmod->name )

#ifdef	__cplusplus
}
#endif

#endif	/* WIDGET_H */

