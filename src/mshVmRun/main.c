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
 * Created on 6 декабря 2018 г., 16:37
 */

#include <stdio.h>
#include <stdlib.h>

int mshVMrunPos(char *pcName,int *pnumstr,int *pnumpos);
/*
 * 
 */
int main(int argc, char** argv) {
 int err,numstr,numpos;
 //
 if (argc<2) {
  printf("Не задано имя исходного файла\n");
  return EXIT_FAILURE;
  }
 printf("Выполнение pi coda модуля %s\n",argv[1]);
 err=mshVMrunPos(argv[1],&numstr,&numpos);
 printf("Код завершения err=%i #Str=%i #Pos=%i\nExit\n",err,numstr,numpos);
 return (err==0)?(EXIT_SUCCESS):EXIT_FAILURE;
 }

