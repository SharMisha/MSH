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
 * Created on 9 декабря 2018 г., 10:33
 */

#include <stdio.h>
#include <stdlib.h>

int mshClnt(char *pcName);
/*
 * 
 */
int main(int argc, char** argv) {
 int err;
 //
 if (argc<2) return EXIT_FAILURE;
 err= mshClnt(argv[1]);
 printf ("\nerrCode=%i\n",err);
 return err;
 }

