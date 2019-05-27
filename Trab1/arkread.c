#include"arkread.h"
#include"arkwrite.h"

void escolheFuncao(int funcao, char* arquivo, char* campo, char* dado){
    if(funcao == 1){
        lerCSV(arquivo);
    }
    else if (funcao == 2){
        lerBin(arquivo);
    }
    else if (funcao == 3){
        procuraRegistros(arquivo,campo,dado);
    }
}

void procuraRegistros(char* arquivo,char* campo, char* dado){
    FILE* ark = fopen(arquivo,"rb");
    if(ark == NULL){
        printf(ERRO1);
        exit(0);
    }
    Cabecalho c = lerCabecalho(ark);
    if(c.status == 0){
        printf(ERRO1);
        exit(0);
    }
    fseek(ark,PAGINA_DISCO-214,SEEK_CUR);
    if(strcmp(campo, "idServidor") == 0){
        int id = atoi(dado);
        buscaID(id,ark,c);
    }
    else if(strcmp(campo, "salarioServidor") == 0){
        double salario = atof(dado); 
        buscaSalario(salario,ark,c);
    }
    else if(strcmp(campo, "telefoneServidor") == 0){
        buscaTelefone(dado,ark,c);
    }
    else if(strcmp(campo, "nomeServidor") == 0){
        buscaNome(dado,ark,c);
    }
    else if(strcmp(campo, "cargoServidor") == 0){
        buscaCargo(dado,ark,c);
    }
    fclose(ark);
}

void buscaID(int id, FILE* ark,Cabecalho c){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    while(r.idServidor != id && r.tamanhoRegistro != 0){
        offset += r.tamanhoRegistro + 5;
        r = lerRegistroBin(ark);
    }
    offset += r.tamanhoRegistro;
    if(r.tamanhoRegistro == 0){
        printf(RI);
        exit(0);
    }
    else{
        imprimeRegistro(r,c);
    }
    paginasAcessadas = (offset / PAGINA_DISCO) + 1;
    printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
}

void buscaSalario(double salario, FILE* ark, Cabecalho c){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    int flag = 0;
    while(r.tamanhoRegistro != 0){
        offset += r.tamanhoRegistro + 5;
        if(r.salarioServidor == salario){
            if(flag == 0)
                flag = 1;
            imprimeRegistro(r,c);
        }
        r = lerRegistroBin(ark);
    }
    offset += r.tamanhoRegistro;
    if(flag == 0){
        printf(RI);
        exit(0);
    }
    paginasAcessadas = (offset / PAGINA_DISCO) + 1;
    printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
}

void buscaTelefone(char* telefone, FILE* ark, Cabecalho c){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    int flag = 0;
    while(r.tamanhoRegistro != 0){
        offset += r.tamanhoRegistro + 5;
        if(strcmp(r.telefoneServidor,telefone) == 0){
            if(flag == 0)
                flag = 1;
            imprimeRegistro(r,c);
        }
        r = lerRegistroBin(ark);
    }
    offset += r.tamanhoRegistro;
    if(flag == 0){
        printf(RI);
        exit(0);
    }
    paginasAcessadas = (offset / PAGINA_DISCO) + 1;
    printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
}

void buscaNome(char* nome, FILE* ark, Cabecalho c){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    int flag = 0;
    while(r.tamanhoRegistro != 0){
        offset += r.tamanhoRegistro + 5;
        if(r.tamNome > 0){
           if(strcmp(r.nomeServidor,nome) == 0){
                if(flag == 0)
                    flag = 1;
                imprimeRegistro(r,c);
            }
        }
        r = lerRegistroBin(ark);
    }
    offset += r.tamanhoRegistro;
    if(flag == 0){
        printf(RI);
        exit(0);
    }
    paginasAcessadas = (offset / PAGINA_DISCO) + 1;
    printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
}

void buscaCargo(char* cargo, FILE* ark, Cabecalho c){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    int flag = 0;
    while(r.tamanhoRegistro != 0){
        offset += r.tamanhoRegistro + 5;
        if(r.tamCargo > 0){
            if(strcmp(r.cargoServidor,cargo) == 0){
                if(flag == 0)
                    flag = 1;
                imprimeRegistro(r,c);
            }
        }
        r = lerRegistroBin(ark);        
    }

    offset += r.tamanhoRegistro;
    if(flag == 0){
        printf(RI);
        exit(0);
    }
    paginasAcessadas = (offset / PAGINA_DISCO) + 1;
    printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
}


Cabecalho lerCabecalho(FILE* ark){
    Cabecalho c;
    fread(&c.status,sizeof(char),1,ark);
    fread(&c.topolista,sizeof(long int),1,ark);
    fread(&c.tagCampo1,sizeof(char),1,ark);
    fread(&c.desCampo1,sizeof(char),40,ark);
    fread(&c.tagCampo2,sizeof(char),1,ark);
    fread(&c.desCampo2,sizeof(char),40,ark);
    fread(&c.tagCampo3,sizeof(char),1,ark);
    fread(&c.desCampo3,sizeof(char),40,ark);
    fread(&c.tagCampo4,sizeof(char),1,ark);
    fread(&c.desCampo4,sizeof(char),40,ark);
    fread(&c.tagCampo5,sizeof(char),1,ark);
    fread(&c.desCampo5,sizeof(char),40,ark);
    return c;
}

void lerBin(char* arquivo){
    FILE* bin = fopen(arquivo,"rb");
    if(bin == NULL){
        printf(ERRO1);
        exit(0);
    }
    Registro r;
    int fcomeco = 1;
    char aux;
    int offset = 0;
    int paginasAcessadas;
    aux = getc(bin);
    if(aux == '0'){
        printf(ERRO1);
        exit(0);
    }
    ungetc(aux,bin);
    fseek(bin,PAGINA_DISCO,SEEK_CUR); // pula pagina de cabecalho
    offset = PAGINA_DISCO;
    r = lerRegistroBin(bin);
    if(r.tamanhoRegistro == 0 && fcomeco == 1){
        printf(RI);
        exit(0);
    }
    else if (r.tamanhoRegistro > 0 && fcomeco == 1)
    {
        fcomeco = 0;
    }
    while(r.tamanhoRegistro > 0){
        offset += r.tamanhoRegistro+5;
        printf("%d ",r.idServidor);
        if(r.salarioServidor >= 0){
            printf("%.2lf ",r.salarioServidor);

        }
        else{
            printf("         ");
        }
        
        if(r.telefoneServidor[0] != '\0'){
            for(int i = 0; i<14;i++){
                printf("%c",r.telefoneServidor[i]);
            }
        }
        else
        {
            printf("              ");
        }
        if(r.tamNome > 0){
            printf(" ");
            printf("%d ",r.tamNome-2);
            printf("%s",r.nomeServidor);
        }
        if(r.tamCargo > 0){
            printf(" ");
            printf("%d ",r.tamCargo-2);
            printf("%s",r.cargoServidor);
        }
        printf("\n");
        r = lerRegistroBin(bin);
    }
    paginasAcessadas = (offset / PAGINA_DISCO) + 1; // Add 1 para contar a pagina atual
    printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
    fclose(bin);
}
Registro lerRegistroBin(FILE* bin){
    Registro r;
    char aux = getc(bin);
    if(aux == EOF){
        r.tamanhoRegistro = 0;
        return r;
    }
    else if (aux == LIXO){ //le todo o lixo de final de pagina
        while(aux == LIXO){
            aux = getc(bin);
        }
    }
    ungetc(aux,bin);
    fscanf(bin,"%c",&r.removido);
    fread(&r.tamanhoRegistro,sizeof(int),1,bin);
    fread(&r.encadeamentoLista,sizeof(long int),1,bin);
    fread(&r.idServidor,sizeof(int),1,bin);
    fread(&r.salarioServidor,sizeof(double),1,bin);
    fread(&r.telefoneServidor,sizeof(char),14,bin);
    fseek(bin,4,SEEK_CUR);// procura tag
    fscanf(bin,"%c",&r.tagCampo4);
    if(r.tagCampo4 == 'n'){
        fseek(bin,-5,SEEK_CUR);//retorna 5 bytes e le o tamanho do nome
        fread(&r.tamNome,sizeof(int),1,bin);
        fseek(bin,1,SEEK_CUR);// pula tag ja lida
        r.nomeServidor = malloc(sizeof(char)*(r.tamNome-1));    
        fread(r.nomeServidor,sizeof(char),r.tamNome-1,bin);

        fseek(bin,4,SEEK_CUR);// procura tag
        fscanf(bin,"%c",&r.tagCampo5);
        if(r.tagCampo5 == 'c'){
        fseek(bin,-5,SEEK_CUR);//retorna 5 bytes e le o tamanho do cargo
        fread(&r.tamCargo,sizeof(int),1,bin);
        fseek(bin,1,SEEK_CUR);// pula tag ja lida
        r.cargoServidor = malloc(sizeof(char)*(r.tamCargo-1));
        fread(r.cargoServidor,sizeof(char),r.tamCargo-1,bin);
            
        }
        else{
            r.tamCargo = 0;
            fseek(bin,-5,SEEK_CUR);
        }
    }
    else if (r.tagCampo4 == 'c'){
        r.tagCampo5 = r.tagCampo4;
        r.tamNome = 0;
        fseek(bin,-5,SEEK_CUR);//retorna 5 bytes e le o tamanho do cargo
        fread(&r.tamCargo,sizeof(int),1,bin);
        fseek(bin,1,SEEK_CUR);// pula tag ja lida
        r.cargoServidor = malloc(sizeof(char)*(r.tamCargo-1));
        fread(r.cargoServidor,sizeof(char),r.tamCargo-1,bin);
    }
    else{
        r.tamNome =  0;
        r.tamCargo = 0;
        fseek(bin,-5,SEEK_CUR);
    }
    return r;
}
void lerCSV(char* arquivo){
    
    
    FILE* CSV = fopen(arquivo,"rb");
    if(CSV == NULL){
        printf(ERRO0);
        exit(0);
    }
        
    FILE* saida = fopen("arquivoTrab1.bin","wb");
    //printf("arquivoTrab1.bin\n");
    if(saida == NULL){
        printf(ERRO0);
        exit(0);
    }
    //Inicializar cabeçalho
    Cabecalho c = initCabecalho(CSV);
    addCabecalho(c,saida);
    lerArquivo(CSV,saida);
    
    fclose(CSV);
    fclose(saida);
}

void lerArquivo(FILE* CSV, FILE* saida){

    Registro r;
    int bytes = 0;
    char lixo = LIXO;
    int tamanhoRegistroAnterior=0;
    while(1){
        r = lerRegistro(CSV);
        if(r.tamanhoRegistro == 0){
            break;
        }
        bytes += r.tamanhoRegistro + 5; // tamanho do registro incluindo os campos removido e indicador de tamanho do registro
        if(bytes <= PAGINA_DISCO){
            addRegistro(r,saida);
            tamanhoRegistroAnterior = r.tamanhoRegistro;
        }
        else{
            fseek(saida,-(tamanhoRegistroAnterior+4),SEEK_CUR); // volta para o indicador de tamanho do registro anterior
            bytes -= (r.tamanhoRegistro+5);
            tamanhoRegistroAnterior += (PAGINA_DISCO-bytes);
            fwrite(&tamanhoRegistroAnterior,sizeof(int),1,saida);
            tamanhoRegistroAnterior -= (PAGINA_DISCO-bytes);
            fseek(saida,(tamanhoRegistroAnterior),SEEK_CUR);
            for(int i=0; i<(PAGINA_DISCO-bytes);i++){
                fwrite(&lixo,sizeof(char),1,saida);
            }
            bytes = r.tamanhoRegistro + 5;
            tamanhoRegistroAnterior = r.tamanhoRegistro;
            addRegistro(r,saida);
        }
    }
    //printf("Acabou©\n");
    rewind(saida);
    char status = '1';
    fwrite(&status,sizeof(char),1,saida);

}

Registro lerRegistro(FILE* CSV){
    Registro r;
    r.tamanhoRegistro = 34; //Bytes do registro independentes dos campos variaveis
    r.removido = '-';
    r.encadeamentoLista = -1;
    r.salarioServidor = -1;
    r.telefoneServidor[0] = '\0';
    for(int i = 1;i<14;i++)
        r.telefoneServidor[i] = LIXO;
    r.tamNome = 0;
    r.tamCargo = 0;
    r.nomeServidor = NULL;
    r.cargoServidor = NULL;
    r.tagCampo4 = 'n';
    r.tagCampo5 = 'c';
    char check;
    fread(&check,sizeof(char),1,CSV);
    if(check == EOF || check == 0)
    {
        r.tamanhoRegistro = 0;
        return r;
    }
    fseek(CSV,-1,SEEK_CUR);
    fscanf(CSV,"%d",&r.idServidor);
    fseek(CSV,1,SEEK_CUR);// pular delimitador
    fread(&check,sizeof(char),1,CSV);
    if(check != ',' && check != '\n'){ //achou campo não nulo
        fseek(CSV,-1,SEEK_CUR); //retorna para ler ultimo byte
        fscanf(CSV,"%lf",&r.salarioServidor);
        fseek(CSV,1,SEEK_CUR);
    }
    fread(&check,sizeof(char),1,CSV);
    if(check != ',' && check != '\n'){ //achou campo não nulo
        fseek(CSV,-1,SEEK_CUR); //retorna para ler ultimo byte
        fread(&r.telefoneServidor,sizeof(char),14,CSV);
        fseek(CSV,1,SEEK_CUR);
    }
    fread(&check,sizeof(char),1,CSV);
    if(check != ',' && check != '\n'){ //achou campo não nulo
        fseek(CSV,-1,SEEK_CUR); //retorna para ler ultimo byte
        char aux;
        int i = 0;
        fread(&aux,sizeof(char),1,CSV);
        while(aux != ',' && aux != '\n'){
            r.nomeServidor = realloc(r.nomeServidor, sizeof(char)*(i+1));    
            r.nomeServidor[i] = aux;
            i++;
            fread(&aux,sizeof(char),1,CSV);
        }
        r.nomeServidor = realloc(r.nomeServidor, sizeof(char)*(i+1));
        r.nomeServidor[i] = '\0';   
        i++;
        r.tamNome = i+1; // add tag
        r.tamanhoRegistro = r.tamanhoRegistro + r.tamNome + 4;// tamanho do nome mais 4 bytes referentes ao indicador de tamanho
    }
    else{
        r.tamNome = 0;
    }
    fread(&check,sizeof(char),1,CSV);
    if(check != ',' && check != '\n'){ //achou campo não nulo
        fseek(CSV,-1,SEEK_CUR); //retorna para ler ultimo byte
        char aux;
        int i = 0;
        fread(&aux,sizeof(char),1,CSV);
        while(aux != ',' && aux != '\n'){
            r.cargoServidor = realloc(r.cargoServidor, sizeof(char)*(i+1));    
            r.cargoServidor[i] = aux;
            i++;
            fread(&aux,sizeof(char),1,CSV);
        }
        r.cargoServidor = realloc(r.cargoServidor, sizeof(char)*(i+1));
        r.cargoServidor[i] = '\0';   
        i++;
        r.tamCargo = i + 1; // add tag
        r.tamanhoRegistro = r.tamanhoRegistro + r.tamCargo + 4;// tamanho do nome mais 4 bytes referentes ao indicador de tamanho 
    }
    else{
        r.tamCargo = 0;
    }
    return r;
}

Cabecalho initCabecalho(FILE* CSV){
    //inicia o cabecalho com dados conhecidos e lidos do arquivo csv
    Cabecalho c;
    c.status = '0';
    c.topolista = -1;
    c.tagCampo1 = 'i';
    c.tagCampo2 = 's';
    c.tagCampo3 = 't';
    c.tagCampo4 = 'n';
    c.tagCampo5 = 'c';
    int i = 0;
    char aux;
    fread(&aux,sizeof(char),1,CSV);
    while(aux != ','){
        c.desCampo1[i] = aux;
        i++;
        fread(&aux,sizeof(char),1,CSV);
    }
    c.desCampo1[i] = '\0';
    i++;
    if(i<39){
        for(i=i;i<40;i++){
            c.desCampo1[i] = LIXO;
        }
    }


    i=0;
    fread(&aux,sizeof(char),1,CSV);
    while(aux != ','){
        c.desCampo2[i] = aux;
        i++;
        fread(&aux,sizeof(char),1,CSV);
    }
    c.desCampo2[i] = '\0';
    i++;
    if(i<39){
        for(i=i;i<40;i++){
            c.desCampo2[i] = LIXO;
        }
    }
    
    i=0;
    fread(&aux,sizeof(char),1,CSV);
    while(aux != ','){
        c.desCampo3[i] = aux;
        i++;
        fread(&aux,sizeof(char),1,CSV);
    }
    c.desCampo3[i] = '\0';
    i++;
    if(i<39){
        for(i=i;i<40;i++){
            c.desCampo3[i] = LIXO;
        }
    }


    i=0;
    fread(&aux,sizeof(char),1,CSV);
    while(aux != ','){
        c.desCampo4[i] = aux;
        i++;
        fread(&aux,sizeof(char),1,CSV);
    }
    c.desCampo4[i] = '\0';
    i++;
    if(i<39){
        for(i=i;i<40;i++){
            c.desCampo4[i] = LIXO;
        }
    }


    i=0;
    fread(&aux,sizeof(char),1,CSV);
    while(aux != '\n' && aux != '\r'){
        c.desCampo5[i] = aux;
        i++;
        fread(&aux,sizeof(char),1,CSV);
    }
    c.desCampo5[i] = '\0';
    i++;
    if(i<39){
        for(i=i;i<40;i++){
            c.desCampo5[i] = LIXO;
        }
    }
    return c;
}