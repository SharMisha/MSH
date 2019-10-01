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
 * Created on 9 декабря 2018 г., 10:20
 */

#include "Srv.h"
//
int mshSrv(char *pflIni);
/*
 * 
 */
int main(int argc, char** argv) {
 char *pflIni="mapPipe.ini";
 int err;
 //
 err=(argc<2)? mshSrv(pflIni) : mshSrv(argv[1]);
 printf ("\nerrCode=%i\n",err);
 }
