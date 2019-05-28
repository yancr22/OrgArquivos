#ifndef _ARKWRITE_H
#define _ARKWRITE_H

#include<stdio.h>
#include<stdlib.h>
#include"arkread.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define LIXO '@'
#define ND "valor nao declarado\n"

void addCabecalho(Cabecalho c, FILE* saida);

void addRegistro(Registro r, FILE* saida);

void imprimeRegistro(Registro r, Cabecalho c);

void remocaoLogica(FILE* ark, int regTam);

void insercaoLogica(FILE* ark, Registro r);

void atualizacaoLogica(FILE* ark, Registro r, char* campoAtualiza, char* dadoAtualiza);

//----------------CODIGO DO MONITOR--------------------------------------------

void binarioNaTela1(FILE *ponteiroArquivoBinario);

void binarioNaTela2(char *nomeArquivoBinario);

void trim(char *str);

void scan_quote_string(char *str);


#endif
