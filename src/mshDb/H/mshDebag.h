/* 
 * File:   mshDebag.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 * Created on 20 Август 2014 г., 12:06
 */

#ifndef MSHDEBAG_H
#define	MSHDEBAG_H

#include <stdio.h>
#include <stdlib.h>
#include "mshData.h"
#include "mshDb.h"
#include "mshErr.h"
#include "mshPgLong.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
#define	MSHDEBAG
 //Печать на устройство
#define	vmPrnt(pdiv,...) if (pdiv==NULL) printf(__VA_ARGS__); else\
 if (pdiv->psec!=NULL) {snprintf(pdiv->pbuf,CsizePacthFl,__VA_ARGS__);pdiv->psec=strcat(pdiv->psec,pdiv->pbuf);}\
 else if (pdiv->pdiv!=NULL) fprintf(pdiv->pdiv,__VA_ARGS__)

#define	vmPrntBK(pdiv) if (pdiv==NULL) printf("\n"); else\
 if (pdiv->psec!=NULL) pdiv->psec=strcat(pdiv->psec,"<br>");\
 else if (pdiv->pdiv!=NULL) fprintf(pdiv->pdiv,"\n")

 #define vmtstWriteAreaHeadA(parea,pdiv) vmPrntBK(pdiv);\
  vmPrnt(pdiv,"Area %p psec %p size %u busy %u",parea,(parea)->psec,(parea)->size,(parea)->busy);
 #define	vmtstWriteAreaHead(parea,pdiv) vmtstWriteAreaHeadA(parea,pdiv);vmPrntBK(pdiv)
 
#define	vmtstWriteArrHeadA(pArr,pdiv) vmPrnt(pdiv,"Arr %p psec %p ptop %p size %u busy %u capa %u"\
    ,pArr,(pArr)->psec,(pArr)->ptop,(pArr)->size,(pArr)->busy,(pArr)->capa)
#define	vmtstWriteArrHead(pArr,pdiv) vmtstWriteArrHeadA(pArr,pdiv); vmPrntBK(pdiv)
    
#define	vmtstWriteSecsHead(pReg,pdiv) vmPrnt(pdiv,"Reg._ct0 "); vmtstWriteArrHeadA( &((pReg)->_ct0),pdiv);\
 vmPrnt(pdiv," [Reg psec=%p ptop=%p size=%u busy=%u]",(pReg)->psec,(pReg)->ptop,(pReg)->size,(pReg)->busy); vmPrntBK(pdiv)

//#define vmtstWriteStrNoSize(pstr,pdiv) for (char *_pC=(pstr)->psec,_pE=_pC+(pstr)->size;_pC<_pE;_pC++) vmPrnt(pdiv,"%c",*_pC)
  
#ifdef	__cplusplus
}
#endif

#endif	/* MSHDEBAG_H */

