#pragma once
#include <stdio.h>
#include <cstdlib>
#include <locale.h>
const int max_buff = 10000;
const int max_replace = 10000;
char s[max_buff + 1]; // буфер для считывания строки

//конец строки определяется по символу конца строки '\0'
int get_str_len(const char* str); // возвращает длину строки (без учета последнего символа \0)

int find_first(const char* sub_str, const char* str); // возвращает индекс первого вхождения строки sub_str в str
                                                      // если подстроки нет, то возвращаем -1

int get_substring_count(const char* sub_str, const char* str); // возвращает количество "полных" вхождений строки sub_str в str
                                                               // в строке "AAA" количество "полных" вхождений подстроки "AA" равно одному!

bool replace_first(const char* sub_str, const char* repl, char* str); // заменяет первое вхождение sub_str на repl в строке str
                                                                      // возвращает true если замена произошла, иначе false

void replace_all(const char* sub_str, const char* repl, char* str); // заменяет все вхождения sub_str на repl в строке str

char* get_str_copy(const char* str);  // выделяет память (get_str_len + 1) для копирования str, копирует str и возвращет указатель. в конце \0
