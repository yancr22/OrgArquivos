#ifndef _ARKWRITE_H
#define _ARKWRITE_H

#include<stdio.h>
#include<stdlib.h>
#include"arkread.h"

#define LIXO '@'
#define ND "valor nao declarado\n"

void addCabecalho(Cabecalho c, FILE* saida);

void addRegistro(Registro r, FILE* saida);

void imprimeRegistro(Registro r, Cabecalho c);


#endif
