
/* 
 * File:   mshSmb.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 23 ноября 2018 г., 13:55
 */

#ifndef MSHSMB_H
#define MSHSMB_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "mshCnst.h"

/*
 * считать следующий UTF-8 символ
 */
#define trSmbType(sCur) (!((sCur) & 0x80))?1:(((sCur) & 0xFC)==0xFC)?6:(((sCur) & 0xF8)==0xF8)?5\
:(((sCur) & 0xF0)==0xF0)?4:(((sCur) & 0xE0)==0xE0)?3:(((sCur) & 0xC0)==0xC0)?2:1

#define trSmbNext(psCur) psCur += trSmbType(*(psCur))

#define trSmbIs1(sCur) (sCur & 0xC0)!=0x80
#define trSmbIsNo1(sCur) (sCur & 0xC0)==0x80

#define smbIsAnsi(sCur) !((sCur) & 0x80)
#define smbIsNoAnsi(sCur) ((sCur) & 0x80)

/* что есть пробел */
#define	trisSpaceBas(c) ((c)==CtrKodSpace)||((c)==CCvmGT)||((c)==CCvmBK)
#define	trIsSpace(c) trisSpaceBas(c)||((c)==CCvmEndStr)
//
#define trSmbIsFloatPoint(cCur) (cCur) == CtrKodFloatPoint
//
#define	smbIsNumbe(c) (c >= '0')&&(c <= '9')
#define	smbIsNoNumbe(c) (!(smbIsNumbe(c)))
#define	smbIsAlpha(c) ((c >= 'A')&&(c <= 'Z'))||((c >= 'a')&&(c <= 'z'))
#define	smbIsNoAlpha(c) (!(smbIsAlpha(c)))
#define	smbIsAlphaNumbe(c) (smbIsAlpha(c))||(smbIsNumbe(c))
#define	smbIsNoAlphaNumbe(c) (!(smbIsAlphaNumbe(c)))
#define	smbIsNoCntrl(c) (smbIsAlpha(c))||(smbIsNumbe(c)||(smbIsNoAnsi(c)) )
/*
 * пропустить пробелы
*/
#define	trSmbSpace(pcIn,pcEnd) if (pcIn<pcEnd) while (( trIsSpace(*(pcIn)) )&&((trSmbNext(pcIn))<pcEnd))
//
//#define smbIsEndNum(c) (cmbFindInStr(CtrEndNum,c))||smbIsOp(c)||(trIsSpace(c))
//
//#define smbIsEndID(c) (cmbFindInStr(CtrEndID,c))||smbIsOp(c)||(trIsSpace(c))
//
//#define smbIsEndCnst(c) cmbFindInStr(CtrEndCnstEnd ,c)
//
//#define	smbTrmIsNumbe(c) (smbIsNumbe(c))
//
//#define smbIsTerm(c) cmbFindInStr(CtrTrm,c)

//#define	trsmbconstEnd(pcIn,pcEnd,cEnd ) while(((trSmbNext(pcIn)) < pcEnd)&&(*pcIn!=cEnd))
//#define	trSmbConstEnd(pcIn,pcEnd,cEnd ) trsmbconstEnd(pcIn,pcEnd,cEnd ); if (*pcIn==cEnd) trSmbNext(pcIn)
/*
 *  считать символы до опреднленного символа
*/
#define	trSmbEnd(pcIn,pcEnd,cOut) while (((trSmbNext(pcIn))<pcEnd)&&(smbIsAnsi(*pcIn))&&((*pcIn) != cOut))

#define	trSkipCommentSlash(pcIn,pcEnd) while ((*(pcIn)!=CCvmEndStr)&&((trSmbNext(pcIn))<pcEnd))

//#define	trSkipCommentAs(pcIn,pcEnd) while ((*(++pcIn)!=CvmCommentSlash)&&((trSmbNext(pcIn))<pcEnd)) trSmbEnd(pcIn,pcEnd,CvmCommentAs)

#ifdef __cplusplus
}
#endif

#endif /* MSHSMB_H */

