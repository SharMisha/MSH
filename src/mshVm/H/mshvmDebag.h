/* 
 * File:   mshvmDebag.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 17 Сентябрь 2014 г., 9:11
 */
#ifndef MSHVMDEBAG_H
#define	MSHVMDEBAG_H

#ifdef	__cplusplus
extern "C" {
#endif
 
#include "../../mshDb/H/mshDebag.h"
#include "../../mshDb/H/mshDiv.h"
#include "../../mshDb/H/mshArea.h"
#include "../../mshDb/H/mshAreaData.h"

#include "mshvmVm.h"
#include "mshvmTr.h"
#include "mshvmCnst.h"
 
#define	vmprnMarrCmdSub(pvmCmd,div) if ((pCmdSub(pvmCmd) >= ' ')&&((pCmdSub(pvmCmd)) <= 'z')) {vmPrnt(div,"%c",pCmdSub(pvmCmd));}\
 else if (pCmdSub(pvmCmd)) {vmPrnt(div,"%d",pCmdSub(pvmCmd));} else vmPrnt(div," ")
//
#define vmprnBoxHead(pBox,pdiv) vmtstWriteArrHead(pBox,pdiv); vmPrnt(pdiv," init=%d",(pBox)->init); vmPrntBK(pdiv)
//
#define vmprnSecsHead(pBox,pdiv) vmtstWriteSecsHead(pBox,pdiv)
 
#define vmprnArr2Head(pBox,pdiv) vmtstWriteArrHead(pBox,pdiv);\
 vmPrnt(pdiv," init=%d sizearr=%d",(pBox)->init,(pBox)->sizearr); vmPrntBK(pdiv)
//
#define vmprnArr2Data(pBox,pdiv) vmprnArr2Head(pBox,pdiv); if ((pBox)->busy) vmtstWriteArrData((TarrData*)((pBox)->psec),pdiv)
//
//#define vmprnAreaHead(parea,pdiv) vmtstWriteAreaHead(parea,pdiv); vmprnArea((parea)->psec,(parea)->busy,pdiv)
#define  vmtstWriteArriDisBusy(Arr,pdiv) ({if (arrIsNoNull(Arr)) { typeof((Arr).psec) psec;  vmPrntBK(pdiv);\
  RegIter(Arr,psec) { vmPrnt(pdiv," %p iD=%i isbusy=%i",psec,psec->iD,psec->isbusy); vmPrntBK(pdiv);  }}  })

void vmprnAreaIter(Tarea *pArea,TdivStream *pdiv);
void tstvmprnMod(TvmMod *pmod,TdivStream *pdiv);
void tstvmprnBoxcurQuery(TBoxIterQuery *pBox,TdivStream *pdiv);
void tstvmprnDo(TvmDo *pDo,TCmd *pcurCmd,TdivStream *pdiv);

#ifdef	__cplusplus
}
#endif

#endif	/* MSHVMDEBAG_H */

