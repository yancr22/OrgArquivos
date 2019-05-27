#include"arkwrite.h"
#include"arkread.h"

void addCabecalho(Cabecalho c, FILE* saida){
    //escreve o cabecalho no arquivo de saida
    fwrite(&c.status,sizeof(char),1,saida);
    fwrite(&c.topolista,sizeof(long int),1,saida);

    fwrite(&c.tagCampo1,sizeof(char),1,saida);
    fwrite(&c.desCampo1,sizeof(char),40,saida);

    fwrite(&c.tagCampo2,sizeof(char),1,saida);
    fwrite(&c.desCampo2,sizeof(char),40,saida);

    fwrite(&c.tagCampo3,sizeof(char),1,saida);
    fwrite(&c.desCampo3,sizeof(char),40,saida);

    fwrite(&c.tagCampo4,sizeof(char),1,saida);
    fwrite(&c.desCampo4,sizeof(char),40,saida);

    fwrite(&c.tagCampo5,sizeof(char),1,saida);
    fwrite(&c.desCampo5,sizeof(char),40,saida);
    //completa a primeira pagina de disco com lixo
    char lixo = LIXO;
    for(int i = 0; i < (PAGINA_DISCO - TAM_CABECALHO);i++){
        fwrite(&lixo,sizeof(char),1,saida);
    }
}

void addRegistro(Registro r, FILE* saida){
    fwrite(&r.removido,sizeof(char),1,saida);
    fwrite(&r.tamanhoRegistro,sizeof(int),1,saida);
    fwrite(&r.encadeamentoLista,sizeof(double),1,saida);
    fwrite(&r.idServidor,sizeof(int),1,saida);
    fwrite(&r.salarioServidor,sizeof(double),1,saida);
    fwrite(&r.telefoneServidor,sizeof(char),14,saida);
    if(r.tamNome != 0){
        fwrite(&r.tamNome,sizeof(int),1,saida);
        fwrite(&r.tagCampo4,sizeof(char),1,saida);
        fwrite(r.nomeServidor,sizeof(char),r.tamNome-1,saida);
    }
    if(r.tamCargo != 0){
        fwrite(&r.tamCargo,sizeof(int),1,saida);
        fwrite(&r.tagCampo5,sizeof(char),1,saida);
        fwrite(r.cargoServidor,sizeof(char),r.tamCargo-1,saida);
    }
}

void imprimeRegistro(Registro r,Cabecalho c){
    printf("%s: %d\n",c.desCampo1,r.idServidor);
    printf("%s: ",c.desCampo2);
    if(r.salarioServidor >= 0){
        printf("%.2lf\n",r.salarioServidor);
    }
    else{
        printf(ND);
    }
    printf("%s: ",c.desCampo3);
    if(r.telefoneServidor[0] != '\0'){
        for(int i = 0; i<14;i++){
            printf("%c",r.telefoneServidor[i]);
        }
        printf("\n");
    }
    else{
        printf(ND); 
    }
     printf("%s: ",c.desCampo4);
    if(r.tamNome > 0)
        printf("%s\n",r.nomeServidor);
    else{
        printf(ND);
    }
    printf("%s: ",c.desCampo5);
    if(r.tamCargo > 0)
        printf("%s\n",r.cargoServidor);
    else{
        printf(ND);
    }
    printf("\n");
}
