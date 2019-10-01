
/* 
 * File:   mshErr.h
 * Author: Шарымов Михаил Алексеевич
 * Copyright (C)
 * misha_shar53@mail.ru misha_shar53@yahoo.com misha_shar53@gmail.com
 *
 * Created on 1 декабря 2016 г., 7:45
 */

#ifndef MSHERR_H
#define MSHERR_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Коды завершения и ошибок
 */
#define	COutKodOk 0
//
#define CoutRefUpdate 1
/**/
/*
 * Ошибки обращения к данным
 */
/* коды возврата */
/* нет индекса */
#define CMPnull -1
/* ключ перед страницами */
#define CkeyFirst -1
#define CkeyFirstFull -2
#define	COutFound -3
#define	COutLabelNotFound -4
#define	COutExpNot -5
#define	COutEOF -6
#define	COutCR -7
#define	COutEndInd -8
/* ключ > страничных */
#define CMPup -12
/* ключ == страничному */
#define CMPeq -13
/* ключ < страничного */
#define CMPdown -14
/* ключ '= страничному */
#define CMPnoEq -15
//способ продолжения обхода дерева 
#define CroundBreak -20
#define CroundContinue -21
#define CroundNoAlt -22
// **Предупреждения
//страница не найдена
#define CWARPgFull -101
//данные не найдены
#define	CWARdataNotFound -102
/* Глобаль не найдена */
#define	CWARglbnotFound -103
//данные не найдены
#define	CWARdataNotDown -105
//страница не найдена
#define CWARchanelFailed -110
//нет признака конца команды
#define CWARendCmd -120
//не комментарий
#define CWARnoComm -121
//#define	CWARvmDbNameEmpty -128
/* не преобразуется в число */
#define	CWARunNum -140
//метка не найдена
#define	CWARLabelNotFound -141
// **Ошибки** 
#define	CERR -1000
//данные не помещаются на странице
#define	CERRPgFull -1001
//данные не того типа
#define	CERRdataNotType -1003
//Индекс задан не верно
#define	CERRIndFiled -1004
//ссылка на несуществующий адрес
#define	CERRAdrFiled -1005
//итератор не найден
#define	CERRiteratorNotFound -1006
//не смог создать страницу
//файл слишком короткий
#define	CERRvmDivSmall -1007
//сбой при отображении файла в память
#define	CERRvmmMapFiled -1008
//нет пустой страницы
#define	CERRpgNewEmptyNot -1010
//не найдены страницы pgLink
#define	CERRnotFoundPgLink -1011
//не найдены страницы pgData
#define	CERRnotFoundPgData -1012
// нарушена структура страниц pgLink
#define	CERRerrorPgLink -1013
//разрушен заголовочный файл
#define	CERRvmFileFiled -1014
//не получен доступ к разделяемому ресурсу
#define	CERRvmLockNot -1015
//не смог создать обращение к данным
#define	CERRDbNotCreat -1016
//на стеке данных не обнаружено
#define	CERRvmDataNo -1017
// нарушена структура данных
#define	CERRerrorTree -1020
//стек Данных пуст
#define	CERRvmDataNotFound -1030
//стек Курсоров пуст
#define	CERRvmNotcurQuery -1031
//Не найден блок Do
#define	CERRvmDoFiled -1035
//не смог создать Job
#define	CERRvmNotCreateJob -1036
//не смог создать Do
#define	CERRvmNotCreateDo -1037
//не смог создать Event
#define	CERRvmNotCreateEvnt -1038
//не смог создать Lock
#define	CERRvmNotCreateLock -1039
// не удалось создать блок Job
#define	CERRjobFiled -1040
// не удалось создать блок Do
#define	CERRdoFiled -1041
//малый размер модуля
#define	CERRvmShortSizeMod -1042
//не смог сформировать Индекс
#define	CERRvmNotIndCreate -1043
//не удалось инициализировать SpinLock
#define	CERRvmSpinLockFiled -1044
//нет возможности выделить память
#define	CERRvmNotMem -1045
//не смог найти такой модуль
//#define	CERRvmNotMod -1046
//ошибка запуска модуля
#define	CERRvmModFiled -1047
//переход за пределы области команд
#define	CERRvmJmpFiled -1050
//стек MSH команд неверен
#define	CERRvmStkMshCmdFiled -1051
// обращение по пустому ключу
#define	CERRnullKey -1100
// Объект не найден
#define	CERRisNotObj -1101
// при записи объект должен быть единственным
#define	CERRSetObjNotSingle -1102
/* ошибка создания устройства */
#define	CERRdivCreateBad -1900
/* ошибка открытия устройства */
#define	CERRdivOpenBad -1901
/* ошибка чтения с устройства */
#define	CERRdivReadBad -1902
/* не полное чтение с устройства */
#define	CERRdivReadPart -1903
/* ошибка записи на устройство */
#define	CERRdivWriteBad -1904
/* директория не создана */
#define	CERRdivDirFiled -1905
//сбой при обращении к файлу
#define	CERRvmDivFiled -1906
//не смог создать файл
#define	CERRvmNotFileCreate -1907
/* Не создан Map */
#define	CERRmapFiled -1910
/* Глобаль не может быть создана */
#define	CERRglbFailed -1930
/* Глобаль не корректна */
#define	CERRglbFiled -1932
//не опознана  Магическая последовательность
#define	CERRdivMagic -1933
/* не корректен заголовок Глобали */
#define	CERRglbHead -1932
/* не преобразуется в число */
#define	CERRunNum -1940
// неверное имя
#define	CERRnamenotFound -1941
//нет описание Глобалей
#define	CWARvmIniNot -200
//Адресат не установлен
#define	CWARvmIstPrNot -201
//Аргументы не заданы
#define	CWARvmArgNot -202
//не подходящий символ
#define	CWARtrIsNoSmb -203
//нет команд к выполнению
#define	CWARtrCmdEmpty -204
//нет выделенной строки
#define	CWARvmSelNo -206
//имя индекса пусто не удалять используется в спец.целях
#define	CWARvmDbIndsEmpty -210
/* Ошибки выполнения  */
//нарушен формат данных
#define	CERRvmdataUnFormat -2001
//данные не найдены
#define	CERRvmdataNotFound -2002
//данные не найдены
#define	CERRdataNotFound -2003
//обращение к данным не верно
#define	CERRvmdataFiled -2004
//тип внутренней функции не верен
#define	CERRnameInFunFiled -2005
//нет такой Win функции
#define	CERRnameInWinNo -2006
//имя модуля или метки не задано
#define	CERRnameModNotFound -2009
//модуль не найден
#define	CERRvmModNotFound -2010
//метка не найдена
#define	CERRvmLabelNotFound -2011
//имя модуля или метки не идентификатор
#define	CERRnameModNoIdn -2015
//имя не может быть массивом
#define	CERRNameFiled -2022
//неизвестный стиль Списка
#define	CERRvmStylListFiled -2023
//индекс неверный
#define	CERRvmIndFiled -2024
//индекс не пустой
#define	CERRvmIndNotEmpty -2025
//Не задан индекс сохранения
#define	CERRvmIndSaveFiled -2026
//Не найдено дерево
#define	CERRvmDbFiled -2027
//Не задано имя глобали
#define	CERRvmDbNameNotFound -2028
//имя Данных не верно
#define	CERRvmDbNameFiled -2029
//Буфер может быть только массивом
#define	CERRvmDbNameBufFiled -2030
//Не заданы данные
#define	CERRvmDanFiled -2031
// ссылка на дерево не может быть использована в этом контексте
#define	CERRrefDbNo -2038
//источник деревом не является
#define	CERRfromDbFiled -2040
//приемник деревом не является
#define	CERRtoDbFiled -2041
//не нашел куда поместить результат
#define	CERRDataToFiled -2042
//Объект не найден
#define	CERRObjNotFind -2050
//имя Объекта не задано
#define	CERRObjNotFound -2051
//аргумент не того типа
#define	CERRvmArgNoType -2055
//мало аргументов
#define	CERRvmArgLittle -2056
//нет аргументов
#define	CERRvmNotArg -2057
//такого типа не существует
#define	CERRTypeNo -2060
//такого Атрибута не существует
#define	CERRAtrNotFound -2061
/*
 * сообщения об Ошибках
 */
//не обнаружена Метка
#define	CERRvmNotLabel -2204
//не обнаружен Индекс
#define	CERRvmNotInd -2205
//Индекс пустой
#define	CERRvmIndEmpty -2207
//нет такого кода Sub операции
#define	CERRvmNotSubOp -2210
//в команде ( недопустима
#define	CERRvmGonoBkt -2211
//в команде Go , недопустима
#define	CERRvmGonoList -2212
//индекс не того типа
#define	CERRvmIndNoType -2214
//нет команд к выполнению
#define	CERRvmCmdFound -2222
//файл не того типа либо не той версии
#define	CERRvmNotMag -2223
//не смог найти Исходный текст
#define	CERRvmNotMsh -2224
//Исходный текст не является Msh программой
#define	CERRvmNotMshMod -2225
//нет имени Event
#define	CERRvmEvntNoName -2241
//повторная блокировка этого же задания
#define	CERRvmReLock -2246
//нет имени файла
#define	CERRvmNotFileName -2301
//нет такой глобали
#define	CERRvmFileNot -2310
//нет описание
#define	CERRvmIniNot -2311
//не назначено текущее устройство
#define	CERRvmUseFileNo -2330
//не назначено устройство WWW
#define	CERRvmWWWNo -2331
//
// команда не завершена   
#define	CERRtrEndCmd -3000
//ссылка в левой части не верна
#define	CERtrRefFiled -3001
//свойство в левой части = недопустимо
#define	CERRtrPropLeft -3002
//на уровне ссылки недопустим код операции
#define	CERRtrLeftFiled -3003
//не соответствие скобок
#define	CERRtrBktFiled -3004
//индекс неверен
#define	CERRtrIndFiled -3005
//условие выполнения команды не верно
#define	CERRtrCondFiled -3006
//потерян уровень вложений выражения
#define	CERRtrStatNULL -3007
//нет символа завершения команды
#define	CERRtrEndCmdNo -3008
//Выражение неверно
#define	CERRtrExpFiled -3010
//Выражение должно быть индексом
#define	CERRtrIndNo -3011
//Индекс пустой
#define	CERRtrIndEmpty -3012
//не найден символ =
#define	CERRprsNoEqu -3013
//Аргумент не задан
#define	CERRtrArgNotFound -3014
//Аргумент не того типа
#define	CERRtrArgFiled -3015
//не задан аргумент функции
//#define	CERRtrRasArgFiled -3043
//нет такого кода операции
#define	CERRvmNotOp -3016
//нет такого кода векторной операции
#define	CERRvmNotOpUsr -3017
//нет такого кода векторной операции
#define	CERRvmcoorVekFiled -3018
//формат операции ?
#define	CERRtrYclFormat -3020
//формат
#define	CERRtrFormat -3021
//не опознано имя команды
#define	CERRtrIDbad -3022
 //для команды Else нет соответствующей команды If
#define	CERRtrElseNoIf -3023
//команда Break находится вне цикла 
#define	CERRtrBreakNoFo -3024
//не верный формат ID
#define	CERRtrFrmId -3025
//не задан обработчик события
#define	CERRtrEvntCall -3026
//не задан опорный индекс в Итераторе
#define	CERRtrIterIndNot -3027
//опорный индекс в Итераторе не верен
#define	CERRtrIterIndFiled -3028
//не задан индекс сохранения в Итераторе
#define	CERRtrIndSaveFiled -3029
//непонятная последовательность символов
#define	CERRtrCharFiled -3030
//не задано имя глобали
#define	CERRtrnameGlbNo -3040
//имя глобали может быть только идентификатором
#define	CERRtrNameDbNotId -3041
//имя не задано
#define	CERRtrNameFound -3044
//внутреннее свойство не привязано к переменной
#define	CERRtrInnerNoVal -3045
//имя модуля может быть только идентификатором
#define	CERRtrNameModNotId -3046
//разбор выражения не завершен
#define	CERRtrParseExpNotFinished -3047
//Аргумент не завершен
#define	CERRtrParseArgNotFinished -3048
//Многострочный комментарий не завершен
#define	CERRtrParseCommentNotFinished -3049
//формат числа нарушен
#define	CERRtrFormatNumFiled -3050
//допустимо только целое число
#define	CERRvmFormatIntFiled -3051
//В команде Case не задано выражение вычисления метки перехода
#define	CERRtrCaseNotExp -3052
//нет правой части
#define	CERRvmRpartNot -3055
//тип Объекта указан неверно
#define	CERRvmTypeObjFiled -3060
 
#define	CWARsrvEmpty -401
#define	CWARsrvReadEnd -402
#define	CWARsrvEquNo -403
#define	CWARsrvArgNo -404
#define	CERRsrv -4000

#define	CERRScktFiled -4001
#define	CERRPortFiled -4002
#define	CERRPortNotFound -4003
#define	CERRHostPortFiled -4004
#define	CERRsrvListenFiled -4010
#define	CERRconnectFiled -4011
#define	CERRReadFiled -4012
#define	CERRAccept -4013
#define	CERRSendFiled -4015
#define	CERRChannelError -4020
#define	CERRChannelFlagsError -4021
#define	CERRrequestFiled -4022

#define	CERRsrvNot -4101
#define	CERRsrvMetodNo -4102
#define	CERRsrvUserNo -4103
#define	CERRsrvClntFiled -4104
#define	CERRsrvFileHmhNoOpen -4105
//Аргументы не переданы
#define	CERRsrvNotArg -4110
//Аргументы не корректны
#define	CERRsrvFiled -4111

#define	CERRguideNotArg -4200
#define	CERRguideNotSel -4201
#define	CERRguideNotWin -4202
//Объект не задан
#define	WARwinObjNotFound -500

#define	CERRwinNotArg -5000
#define	CERRwinNotSel -5001
#define	CERRwinNotWin -5002
//аргумент не найден
#define	CERRwinArgNotFound -5003
//аргумент не того типа
#define	CERRwinArgNotType -5004
//Объект не задан
#define	CERRwinObjNotFound -5005
//объект не того типа
#define	CERRwinObjNotType -5006
//Объект не найден
#define	CERRwinObjNotFind -5007
//мало аргументов
#define	CERRwinArgLittle -5010
//Предок не найден
#define	CERRwinParentNotFind -5011
//родитель не того типа
#define	CERRwinParentNoType -3065
//такого типа не существует
#define	CERRwinTypeNo -5014
//   
#define	CERRwintempDbFiled -5030
//библиотека Win не инициализирована
#define	CERRwinInitNo -5040


#ifdef __cplusplus
}
#endif

#endif /* MSHERR_H */

