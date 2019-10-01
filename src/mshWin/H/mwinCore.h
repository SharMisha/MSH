
/* 
 * File:   mwinCore.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 26 марта 2017 г., 13:54
 */

#ifndef MWINCORE_H
#define MWINCORE_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "mshCnst.h"

#define CwinTitleDef "Вы не задали заголовок приложения"
#define CwinwidthDef 600
#define CwinheightDef 800

#define	CwinsizeIndDef 4
#define	CwinsizePaddingDef 4
#define	CwinsizeAreaDef 512

#define	CwinAtrInt0Def -1
#define	CwinNbTabIsDef 1
#define	CwinPosDef 0
#define	CwinScPolDef GTK_POLICY_AUTOMATIC
#define	CwinBoxStyleDef 3
#define	CwinSpacingDef 4
#define	CwinCellDef 1
#define	CwinNbScrollDef TRUE
#define	CwinVisDef 1
#define	CwinIsTreeDef 0
#define	CwinPackEndDef 0
//расположение объектов в массиве tmpArr
#define	CwinoffMain 1
#define	CwinoffVmWin 2

#define CsizewinDbPgDef 24*50

/*
 * создание объектов Gtk
 * Win - главное окно
 * Nb - notebook
 * Lb - label
 * But - кнопки
 * Boxb - button_box
 * Grid - таблица
 * Tree - дерево
 * ---------------------------
 *  title -заголовок
 * width, height - размеры
 * name имя
 */
//имена объектов 
#define CwinWin "winWin"
#define CwinNb "winNb"
#define CwinLb "winLb"
#define CwinBut "winBut"
#define CwinBox "winBox"
#define CwinBoxBut "winBoxB"
#define CwinScroll "winScroll"
#define CwinGrid "winGrid"
#define CwinTree "winTree"
#define CwinListStore "winListStore"
#define CwinTreeStore "winTreeStore"
#define CwinImg "winImg"
#define CwinCalendar "winCalendar"
#define CwinTab "winTab"
 
typedef enum{
 Ewin0,
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
 EwinCalendar,
 EwinTab,
 EwinEnd
 } EwinTypeObj;
 //имена Атрибутов
#define CwinAtrName "name"
#define CwinAtrWidth "width"
#define CwinAtrHeight "height"
#define CwinAtrNbTabIs "nbtabis"
#define CwinAtrPos "pos"
#define CwinAtrhScPol "hscpol"
#define CwinAtrvScPol "vscpol"
#define CwinAtrBoxStyle "boxstyle"
#define CwinAtrCell "cell"
#define CwinAtrSpacing "spac"
#define CwinAtrNbScroll "nbscroll"
#define CwinAtrVisible "vis"
#define CwinAtrImg "img"
#define CwinAtrModel "model"
#define CwinAtrTitle "title"
#define CwinAtrPackEnd "packend"
//
static char* CwinNameMain=".main";
 
#define CwindataHeder 'H'

#define CwintrgWinInit "oninit"
#define CwintrgWinChanel "onchl"
#define CwintrgWinClicked "on"

static char* CwintrgWin="on";
 
typedef enum{
 EwinAtr0,
 EwinAtrName,
 EwinAtrSize,
 EwinAtrNbTabIs,
 EwinAtrPos,
 EwinAtrhScPol,
 EwinAtrvScPol,
 EwinAtrBoxStyle,
 EwinAtrCell,
 EwinAtrSpacing,
 EwinAtrNbScroll, //10
 EwinAtrVisible,
 EwinAtrImg,
 EwinAtrModel,
 EwinAtrTitle,
 EwinAtrData,
 EwinAtrDataAdd,
 EwinAtrFocus,
 EwinAtrTabAdd,
 EwintrgWinOn,
 EwintrgWinOnInit,
// EwintrgWinChanel,
 EwinAtrEnd
 } EwinTypeAtr;
 
typedef enum{
 EwinPackStart,
 EwinPackEnd,
 EwinPackCentr
 } EwinPackChild;
/* 
typedef struct {
 GtkWidget *pwinObj;
 Tuint32 idParent;
 EwinTypeObj type;
 } TwinObj;
 */
/* 
typedef struct {
 GtkNotebook *pNb;
 gint idPg;
 } TmwinNbBpg;
*/
/* 
#define qPack 1
//
#define qType 2
#define qParent 3
// 
#define qOnMod 4
#define qOnLab 5
// 
#define qOnModInit 6
#define qOnLabInit 7
*/ 
// 
typedef enum {
 qParent=1,
 qPack,
 qOnModLab,
 qOnModLabInit,
 qinPg
 } EqType;
/*
 * Команды обмена с сервером
 */
#define CnetcomInit 'I'
#define CnetcomValue 'V'
#define CnetcomSet 'S'
#define CnetcomGet 's'
#define CnetcomFocusSet 'F'
#define CnetcomFocusGet 'f'
 
#define CsysfrRou '\n'
 
#ifdef __cplusplus
}
#endif

#endif /* MWINCORE_H */

