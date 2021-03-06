#ifndef _ARKREAD_H
#define _ARKREAD_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define PAGINA_DISCO 32000
#define TAM_CABECALHO 214

#define ERRO0 "Falha no carregamento do arquivo.\n"
#define ERRO1 "Falha no processamento do arquivo.\n"
#define RI    "Registro inexistente.\n"

//Define estrutura do cabecalho
 
struct cabecalho
{
   char status;
   long int topolista;
   
   char tagCampo1;
   char desCampo1[40];
   
   char tagCampo2;
   char desCampo2[40];
   
   char tagCampo3;
   char desCampo3[40];
   
   char tagCampo4;
   char desCampo4[40];
   
   char tagCampo5;
   char desCampo5[40];
};
typedef struct cabecalho Cabecalho;

//Define estrutura do registro

struct registro
{
    //Para implementacao de remocao de registro
    char removido;
    int tamanhoRegistro;
    long int encadeamentoLista;
    // Campos de tamanho fixo 26 bytes
    int idServidor;
    double salarioServidor;
    char telefoneServidor[14];
    // Campos de tamanho variavel
    int tamNome;
    char tagCampo4;
    char* nomeServidor;
    
    int tamCargo;
    char tagCampo5;
    char* cargoServidor;
};
typedef struct registro Registro;

//-----------------Funcoes---------------------------
void escolheFuncao(int funcao, char* arquivo, char* campo, char* dado, int quantidade);

Cabecalho initCabecalho(FILE* CSV);

Cabecalho lerCabecalho(FILE* ark);

Registro lerRegistro(FILE* CSV);

Registro lerRegistroEntrada();

Registro lerRegistroBin(FILE* Bin);

void lerArquivo(FILE* CSV, FILE* saida);

void lerBin(char* Arquivo);

void lerCSV(char* arquivo);

void procuraRegistros(char* arquivo,char* campo, char* dado, char* campoAtualizar, char* dadoAtualizar, int funcao);

void buscaID(int id, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao);

void buscaSalario(double salario, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao);

void buscaTelefone(char* telefone, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao);

void buscaNome(char* nome, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao);

void buscaCargo(char* cargo, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao);

#endif