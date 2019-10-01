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
 * Created on 4 декабря 2018 г., 21:16
 */

#include <stdio.h>
#include <stdlib.h>

int mshMtoVM(char *pcName,int *pnumstr,int *pnumpos,char *pbuferr);
/*
 * 
 */
int main(int argc, char** argv) {
 int numstr,numpos,err;
 char bufErr[1024];
 //
 if (argc<2) {
  printf("Не задано имя исходного файла\n");
  return EXIT_FAILURE;
  }
 bufErr[0]='\0';
 printf("Трансляция файла %s\n",argv[1]);
 err=mshMtoVM(argv[1],&numstr,&numpos,bufErr);
 printf("Код завершения %d str=%d pos=%d buferr=%s\nExit\n" ,err,numstr,numpos,bufErr);
 return (err==0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

