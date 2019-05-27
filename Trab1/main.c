//Yan Crisóstomo Rohwedder 9779263  
#include"arkread.h"
#include"arkwrite.h"


int main(){

    int funcao;
    char* arquivo = malloc(sizeof(char)*60);
    char* campo = malloc(sizeof(char)*60);
    char* dado = malloc(sizeof(char)*60);
    scanf("%d %60s",&funcao, arquivo);
    if(funcao == 3){
        char* aux = malloc(sizeof(char)*60);
        scanf("%60s ",campo);
        fgets(aux,60,stdin);
        int i;
        for(i=0; aux[i] != '\0' && aux[i]!= '\n';i++){
            dado[i] = aux[i];
        }      
    }
    escolheFuncao(funcao, arquivo, campo, dado);
    free(arquivo);
    free(campo);
    free(dado);

return 0;
} 