#include"arkread.h"
#include"arkwrite.h"

void escolheFuncao(int funcao, char* arquivo, char* campo, char* dado, int quantidade){
    if(funcao == 1){
        lerCSV(arquivo);
    }
    else if (funcao == 2){
        lerBin(arquivo);
    }
    else if (funcao == 3){
        procuraRegistros(arquivo, campo, dado, NULL, NULL, funcao);
    }
    else if(funcao == 4 || funcao == 6){
        char* aux = malloc(sizeof(char)*1000);
        char* campoAtualizar = malloc(sizeof(char)*60); // dado que armazena o campo a ser atualizado pela funcao de Atualizacao
        char* dadoAtualizar = malloc(sizeof(char)*200); // dado que armazena o novo Valor do Campo a ser atualizado pela funcao de Atualizacao
        int j;
        for(int i = 0; i < quantidade; i++){
            dado = malloc(sizeof(char)*60);
            scanf("%60s ",campo);
            fgets(aux,1000,stdin);
            if(funcao == 4){    
                if(aux[0] ==  '"'){
                    for(j=1; aux[j] != '"';j++){
                        dado[j-1] = aux[j];
                    }
                    dado[j] = '\0';
                }
                else{
                    for(j=0; aux[j] != '\0' && aux[j]!= '\n';j++){
                            dado[j] = aux[j];
                    }
                    dado[j] = '\0';
                }
            }
            else if(funcao == 6){
                //ler dado
                int k;
                if(strcmp(campo, "telefoneServidor") == 0){
                    dado = malloc(sizeof(char)*14);
                }
                if(aux[0] ==  '"'){
                    for(k=0,j=1; aux[j] != '"' && aux[j] != '\0';j++,k++){
                        dado[k] = aux[j];
                    }
                    if(strcmp(campo, "telefoneServidor") != 0)
                        dado[k] = '\0';
                    j++;
                }
                else{
                    for(k=0,j=0; aux[j] != ' ';j++,k++){
                            dado[k] = aux[j];
                    }
                    dado[k] = '\0';
                }
                j++; // Pula espaco

                // ler campoAtualizar
                for(k=0,j=j; aux[j] != ' ';j++,k++){
                    campoAtualizar[k] = aux[j];
                }
                campoAtualizar[k] = '\0';
                j++; // Pula espaco

                //ler dadoAtualizar
                if(aux[j] ==  '"'){
                    for(k=0,j=j+1; aux[j] != '"';j++,k++){
                        dadoAtualizar[k] = aux[j];
                    }
                    dadoAtualizar[k] = '\0';
                }
                else{
                    for(k=0,j=j; aux[j] != ' ' && aux[j] != '\n';j++,k++){
                            dadoAtualizar[k] = aux[j];
                    }
                    dadoAtualizar[k] = '\0';
                }

            }
            procuraRegistros(arquivo, campo, dado, campoAtualizar, dadoAtualizar, funcao);
        }
        // FILE* ark = fopen(arquivo,"rb");
        // long int offsetAnterior;
        // long int offsetProximo;
        // int atualRegTam;
        // int proximoRegTam;
        // long int topoLista;
        // fseek(ark,1,SEEK_SET);
        // fread(&topoLista,sizeof(long int),1,ark);
		// offsetAnterior = topoLista;
		// fseek(ark,offsetAnterior+1,SEEK_SET); // vejo o tamanho do proximo registro da lista encadeada
        // fread(&atualRegTam,sizeof(int),1,ark);
        // fread(&offsetProximo,sizeof(long int),1,ark);
		// fseek(ark,offsetProximo+1,SEEK_SET);
		// fread(&proximoRegTam,sizeof(int),1,ark); 
		// while (offsetProximo != -1){
		// 	printf("atual Offset: %ld Atual tam: %d ||| Proximo Offset: %ld Proximo Tam; %d\n",offsetAnterior,atualRegTam,offsetProximo,proximoRegTam);
		// 	offsetAnterior = offsetProximo;
		// 	atualRegTam = proximoRegTam;
		// 	fread(&offsetProximo,sizeof(long int),1,ark);
		// 	fseek(ark,offsetProximo+1,SEEK_SET);
		// 	fread(&proximoRegTam,sizeof(int),1,ark); 
		// }
        // fclose(ark);
        free(aux);
        binarioNaTela2(arquivo);    
    }
    else if(funcao == 5){
        Registro r;
        FILE* ark = fopen(arquivo,"r+b");
        if(ark == NULL){
            printf(ERRO1);
            exit(0);
        }
        Cabecalho c = lerCabecalho(ark);
        if(c.status == '0'){
            printf(ERRO1);
            exit(0);
        }
        else{
            c.status = '0';
            addCabecalho(c,ark);
        }
        for(int i = 0; i < quantidade; i++){
            r = lerRegistroEntrada();
            insercaoLogica(ark,r);
        }
        c.status = '1';
        fseek(ark,0,SEEK_SET);
        fwrite(&c.status,sizeof(char),1,ark);
        
        binarioNaTela1(ark);
        fclose(ark);
    }

}

Registro lerRegistroEntrada(){ // Le registro vindo da entrada padrao
    Registro r;
    // inicio registro
    r.removido = '-';
    r.encadeamentoLista = -1;
    r.tamanhoRegistro = 34; //inicia com campos de tamanho fixo
    r.tagCampo4 = 'n';
    r.tagCampo5 = 'c';
    r.tamNome = 0;
    r.nomeServidor = NULL;
    r.tamCargo = 0;
    r.cargoServidor = NULL;
    char aux;
    
    
    //leio registro
    scanf("%d ",&r.idServidor);
    
    //Ler salario
    aux = getc(stdin);
    if(aux == 'N'){
        r.salarioServidor = -1;
        for(int i = 0; i<4; i++){
            aux = getc(stdin);
        }
    }
    else{
        ungetc(aux,stdin);
        scanf("%lf ",&r.salarioServidor);
    }
    //ler telefone
    aux = getc(stdin);
    if(aux == 'N'){
        r.telefoneServidor[0] = '\0';
        for(int i = 1;i<14;i++)
            r.telefoneServidor[i] = LIXO;
        for(int i = 0; i<4; i++){
            aux = getc(stdin);
        }
    }
    else{
        fread(r.telefoneServidor,sizeof(char),14,stdin);
        aux = getc(stdin); //coleta ultima aspa
        aux = getc(stdin); // coleta espaco
    }
    //ler nome
    aux = getc(stdin);
     if(aux == 'N'){
        for(int i = 0; i<4; i++){
            aux = getc(stdin);
        }
    }
    else{
        int i = 0;
        aux = getc(stdin);
        while(aux != '"'){
            r.nomeServidor = realloc(r.nomeServidor, sizeof(char)*(i+1));    
            r.nomeServidor[i] = aux;
            i++;
            aux = getc(stdin);
        }
        r.nomeServidor = realloc(r.nomeServidor, sizeof(char)*(i+1));    
        r.nomeServidor[i] = '\0';
        r.tamNome += i+2;
        r.tamanhoRegistro += r.tamNome + 4;// mais campo de indicador de tamanho mais tag e \0
        aux = getc(stdin);// coleta espaco
    }
    //ler cargo
    aux = getc(stdin);
     if(aux == 'N'){
        for(int i = 0; i<4; i++){
            aux = getc(stdin);
        }
    }
    else{
        int i = 0;
        aux = getc(stdin);
        while(aux != '"'){
            r.cargoServidor = realloc(r.cargoServidor, sizeof(char)*(i+1));    
            r.cargoServidor[i] = aux;
            i++;
            aux = getc(stdin);
        }
        r.cargoServidor = realloc(r.cargoServidor, sizeof(char)*(i+1));    
        r.cargoServidor[i] = '\0';
        r.tamCargo += i+2;
        r.tamanhoRegistro += r.tamCargo + 4;// mais campo de indicador de tamanho mais tag e \0
        aux = getc(stdin);// coleta \n
    }
    return r;
}

//Funcao procura registros em um arquivo binario, podendo realizar diferentes funcoes com os registros encontrados como Impressao,Remocao,Atualizacao
void procuraRegistros(char* arquivo, char* campo, char* dado, char* campoAtualizar, char* dadoAtualizar, int funcao){
    FILE* ark = fopen(arquivo,"r+b");
    if(ark == NULL){
        printf(ERRO1);
        exit(0);
    }
    Cabecalho c = lerCabecalho(ark);
    if(c.status == '0'){
        printf(ERRO1);
        exit(0);
    }
    else if(funcao > 3){
        char status = '0';
        rewind(ark);
        fwrite(&status,sizeof(char),1,ark);
        fseek(ark,PAGINA_DISCO-1,SEEK_CUR);
    }
    else{
        fseek(ark,PAGINA_DISCO-214,SEEK_CUR);
    }
    if(strcmp(campo, "idServidor") == 0){
        int id = atoi(dado);
        buscaID(id, ark, c, campoAtualizar, dadoAtualizar, funcao);
    }
    else if(strcmp(campo, "salarioServidor") == 0){
        double salario;
        if(strcmp(dado, "NULO") == 0){
            salario = -1;
        }            
        else{
            salario = atof(dado); 
        }
        buscaSalario(salario, ark, c, campoAtualizar, dadoAtualizar, funcao);
    }
    else if(strcmp(campo, "telefoneServidor") == 0){
        if(strcmp(dado, "NULO") == 0){
            dado = "\0@@@@@@@@@@@@@";
        }
        buscaTelefone(dado, ark, c, campoAtualizar, dadoAtualizar, funcao);
    }
    else if(strcmp(campo, "nomeServidor") == 0){
        buscaNome(dado, ark, c, campoAtualizar, dadoAtualizar, funcao);
    }
    else if(strcmp(campo, "cargoServidor") == 0){
        buscaCargo(dado, ark, c, campoAtualizar, dadoAtualizar, funcao);
    }
    if(funcao > 3){
        char status = '1';
        rewind(ark);
        fwrite(&status,sizeof(char),1,ark);
    }
    fclose(ark);
}
void buscaID(int id, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    while(r.tamanhoRegistro > 0){
        offset += r.tamanhoRegistro + 5; // + removido e Tamanho do Registro
        if(r.removido == '-'){
            if(r.idServidor == id)
                break;
        }
        r = lerRegistroBin(ark);
    }
    offset += r.tamanhoRegistro;
    if(funcao == 3){
        if(r.tamanhoRegistro == 0){
            printf(RI);
            exit(0);
        }
        else{
            imprimeRegistro(r,c);
            paginasAcessadas = (offset / PAGINA_DISCO) + 1;
            printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
        }
    }
    else if(funcao == 4){
        if(r.tamanhoRegistro > 0){
            remocaoLogica(ark,r.tamanhoRegistro);
        }
    }
    else if(funcao == 6){
        if(r.tamanhoRegistro > 0){
            atualizacaoLogica(ark, r, campoAtualizar, dadoAtualizar);
        }
    }
    
}

void buscaSalario(double salario, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    int flag = 0; // Flag que aponta se houve busca com sucesso
    while(r.tamanhoRegistro != 0){
        offset += r.tamanhoRegistro + 5;
        if(r.removido == '-'){
            if(r.salarioServidor == salario){
                if(funcao == 3){
                    if(flag == 0)
                        flag = 1;
                    imprimeRegistro(r,c);
                }
                else if(funcao == 4){
                    remocaoLogica(ark,r.tamanhoRegistro);
                }
                else if(funcao == 6){
                    atualizacaoLogica(ark, r, campoAtualizar, dadoAtualizar);
                 }
            }
        }
        r = lerRegistroBin(ark);
    }
    offset += r.tamanhoRegistro;
    if(funcao == 3){
        if(flag == 0){
            printf(RI);
            exit(0);
        }
        paginasAcessadas = (offset / PAGINA_DISCO) + 1;
        printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
    }
}

void buscaTelefone(char* telefone, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    int flag = 0; // Flag que aponta se houve busca com sucesso
    int acheiTelefone;
    while(r.tamanhoRegistro > 0){
        offset += r.tamanhoRegistro + 5;
        if(r.removido == '-'){
            acheiTelefone = 0;
            for(int i = 0; i<14; i++){
                if(r.telefoneServidor[i] != telefone[i]){
                    acheiTelefone = 1;
                }
            }
            if(acheiTelefone == 0){
                if(funcao == 3){
                    if(flag == 0)
                        flag = 1;
                    imprimeRegistro(r,c);
                }
                else if(funcao == 4){
                    remocaoLogica(ark,r.tamanhoRegistro);
                }
                else if(funcao == 6){
                    atualizacaoLogica(ark, r, campoAtualizar, dadoAtualizar);
                }
            }  
        }
        r = lerRegistroBin(ark);
    }
    offset += r.tamanhoRegistro;
    if(funcao == 3){
        if(flag == 0){
            printf(RI);
            exit(0);
        }
        paginasAcessadas = (offset / PAGINA_DISCO) + 1;
        printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
    }
}

void buscaNome(char* nome, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    int flag = 0; // Flag que aponta se houve busca com sucesso
    while(r.tamanhoRegistro != 0){
        offset += r.tamanhoRegistro + 5;
        if(r.removido == '-'){
            if(r.tamNome > 0){ // se existe nome
                if(strcmp(r.nomeServidor,nome) == 0){
                    if(funcao == 3){
                        if(flag == 0)
                            flag = 1;
                        imprimeRegistro(r,c);
                    }
                    else if(funcao == 4){
                        remocaoLogica(ark,r.tamanhoRegistro);
                    }
                    else if(funcao == 6){
                       atualizacaoLogica(ark, r, campoAtualizar, dadoAtualizar);
                    }
                }
            }
            else{ // se nao existe nome -> busca por nome NULO
                if(funcao == 4 && strcmp(nome,"NULO") == 0)
                    remocaoLogica(ark,r.tamanhoRegistro);
                else if(funcao == 6 && strcmp(nome,"NULO") == 0){
                    atualizacaoLogica(ark, r, campoAtualizar, dadoAtualizar);
                }
            }
        }
        r = lerRegistroBin(ark);
    }
    offset += r.tamanhoRegistro;
    if(funcao == 3){
        if(flag == 0){
            printf(RI);
            exit(0);
        }
        paginasAcessadas = (offset / PAGINA_DISCO) + 1;
        printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
    }
}

void buscaCargo(char* cargo, FILE* ark, Cabecalho c, char* campoAtualizar, char* dadoAtualizar, int funcao){
    Registro r;
    r = lerRegistroBin(ark);
    int offset = PAGINA_DISCO;
    int paginasAcessadas = 1;
    int flag = 0; // Flag que aponta se houve busca com sucesso
    while(r.tamanhoRegistro > 0){
        offset += r.tamanhoRegistro + 5;
        if(r.removido == '-'){
            if(r.tamCargo > 0){ // Se existe cargo
                if(strcmp(r.cargoServidor,cargo) == 0){
                    if(funcao == 3){
                        if(flag == 0)
                            flag = 1;
                        imprimeRegistro(r,c);
                    }
                    else if(funcao == 4){
                        remocaoLogica(ark,r.tamanhoRegistro);
                    }
                    else if(funcao == 6){
                        atualizacaoLogica(ark, r, campoAtualizar, dadoAtualizar);
                    }
                    
                }
            }
            else{ // Se nao existe cargo
                if(funcao == 4 && strcmp(cargo,"NULO") == 0){
                    remocaoLogica(ark,r.tamanhoRegistro);
                }
                else if(funcao == 6 && strcmp(cargo,"NULO") == 0){
                    atualizacaoLogica(ark, r, campoAtualizar, dadoAtualizar);
                }
            }
        }
        r = lerRegistroBin(ark);
    }
    offset += r.tamanhoRegistro;
    if(funcao == 3){
        if(flag == 0){
            printf(RI);
            exit(0);
        }
        paginasAcessadas = (offset / PAGINA_DISCO) + 1;
        printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
    }
}

// Le cabecalho de um arquivo de dados binarios
Cabecalho lerCabecalho(FILE* ark){
    Cabecalho c;
    rewind(ark);
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

// Funcao que le arquivo binario e imprime seus registros
void lerBin(char* arquivo){
    FILE* bin = fopen(arquivo,"rb");
    if(bin == NULL){
        printf(ERRO1);
        exit(0);
    }
    Registro r;
    char aux;
    long int offset = 0;
    int paginasAcessadas = 1; // Conta pagina do cabecalho

    aux = getc(bin);
    if(aux == '0'){
        printf(ERRO1);
        exit(0);
    }
    ungetc(aux,bin);

    fseek(bin,PAGINA_DISCO,SEEK_CUR); // pula pagina de cabecalho
    offset = PAGINA_DISCO;
    r = lerRegistroBin(bin);
    if(r.tamanhoRegistro == 0){
        printf(RI);
        exit(0);
    }
    while(r.tamanhoRegistro > 0){
        offset += r.tamanhoRegistro+5;
        if(r.removido == '-'){ // Se registro nao foi removido
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
        }
        r = lerRegistroBin(bin);
    }
    paginasAcessadas += (offset / PAGINA_DISCO); // Add 1 para contar a pagina atual
    printf("Número de páginas de disco acessadas: %d\n",paginasAcessadas);
    fclose(bin);
}

//Le registro de um Arquivo binario
Registro lerRegistroBin(FILE* bin){
    Registro r;
    char aux = getc(bin);
    if(aux == EOF){
        r.tamanhoRegistro = 0;
        return r;
    }
    ungetc(aux,bin);
    fscanf(bin,"%c",&r.removido);
    fread(&r.tamanhoRegistro,sizeof(int),1,bin);
    fread(&r.encadeamentoLista,sizeof(long int),1,bin);
    if(r.removido == '-'){      
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
    }
    else{
        fseek(bin,r.tamanhoRegistro-8,SEEK_CUR);
    }
    aux = getc(bin);
    if(aux == LIXO){ //le todo o lixo de final de pagina e de registro
        while(aux == LIXO){
            aux = getc(bin);
        }
    }
    ungetc(aux,bin);
    return r;
}
// Le arquivo CSV e cria um arquivo binario com os registros do arquivo CSV
void lerCSV(char* arquivo){
    
    FILE* CSV = fopen(arquivo,"rb");
    if(CSV == NULL){
        printf(ERRO0);
        exit(0);
    }
        
    FILE* saida = fopen("arquivoTrab1.bin","wb");
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

// Funcao suplementar de lerCSV
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
    rewind(saida);
    char status = '1';
    fwrite(&status,sizeof(char),1,saida);

}

//Le registro de um arquivo CSV
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

// inicia cabecalho do arquivo binario a partir de um arquivo CSV
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