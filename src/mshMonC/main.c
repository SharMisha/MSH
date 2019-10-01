
/* 
 * File:   main.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 28 февраля 2017 г., 11:03
 */

#include "mshMonC.h"

int mshmonInit(char *phost);

/*
 * 
 */
int main(int argc, char** argv) {
 int err=0;
 //*
 if (argc<2)
  err=mshmonInit(ChostportSupSrvDef);
 else{
  //подключение клиента к сети
  err=mshmonInit(argv[1]);
  }
 printf ("\nerrCode=%i\n",err);
 exit (EXIT_SUCCESS);
 }

