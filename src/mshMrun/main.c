/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 6 декабря 2018 г., 16:23
 */

#include <stdio.h>
#include <stdlib.h>

int mshMrunPos(char *pcName,char *pcLabel,int *pnumstr,int *pnumpos);
/*
 * 
 */
int main(int argc, char** argv) {
 int numstr,numpos,err;
 //
 if (argc<2) {
  printf("Не задано имя исходного файла\n");
  return EXIT_FAILURE;
  }
 if (argc<3) {
  printf("Выполнить модуль %s\n",argv[1]);
  err=mshMrunPos(argv[1],NULL,&numstr,&numpos);
  }
 else{
  printf("Выполнить модуль %s с метки %s\n",argv[1],argv[2]);
  err=mshMrunPos(argv[1],argv[2],&numstr,&numpos);
  }
 printf("Код завершения %d str=%d pos=%d\n",err,numstr,numpos);
 return (err==0)?(EXIT_SUCCESS):EXIT_FAILURE;
 }
