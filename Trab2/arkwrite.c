#include"arkwrite.h"
#include"arkread.h"

//Adiciona cabecalho ao arquivo binario 
void addCabecalho(Cabecalho c, FILE* saida){
	rewind(saida);

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

//Adiciona um registro ao arquivo binario
void addRegistro(Registro r, FILE* saida){
    fwrite(&r.removido,sizeof(char),1,saida);
    fwrite(&r.tamanhoRegistro,sizeof(int),1,saida);
    fwrite(&r.encadeamentoLista,sizeof(double),1,saida);
    fwrite(&r.idServidor,sizeof(int),1,saida);
    fwrite(&r.salarioServidor,sizeof(double),1,saida);
    fwrite(&r.telefoneServidor,sizeof(char),14,saida);
    if(r.tamNome > 0){
        fwrite(&r.tamNome,sizeof(int),1,saida);
        fwrite(&r.tagCampo4,sizeof(char),1,saida);
        fwrite(r.nomeServidor,sizeof(char),r.tamNome-1,saida);
    }
    if(r.tamCargo > 0){
        fwrite(&r.tamCargo,sizeof(int),1,saida);
        fwrite(&r.tagCampo5,sizeof(char),1,saida);
        fwrite(r.cargoServidor,sizeof(char),r.tamCargo-1,saida);
    }
}

//imprime registro na saida padrao
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

// Tamanho do registro continua igual
// regTam = tamanho do registro menos campos removido,tamReg
// remove registro que acabou de ser lido de um arquivo binario
void remocaoLogica(FILE* ark, int regTam){
    int regTam_semEncadamentoLista = regTam - 8;
    int regTam_Completo = regTam + 5; // inclui campos removido e regTam
    char removido = '*';
    long int topoLista;
    char lixo = LIXO;
    fseek(ark,-regTam_Completo,SEEK_CUR); // volto o para o inicio do registro lido
    long int regPos = ftell(ark); // gravo a posicao do registro
    fwrite(&removido,sizeof(char),1,ark); // escrevo que o arquivo foi removido
    fseek(ark,12,SEEK_CUR); // Pulo campo regTam e EncadLista
    for(int i = 0; i < (regTam_semEncadamentoLista);i++){
        fwrite(&lixo,sizeof(char),1,ark);
    }
    fseek(ark,sizeof(char),SEEK_SET); // volto para o inicio do arquivo pulando status
    fread(&topoLista,sizeof(long int),1,ark); // leio TopoLista
    if(topoLista == -1){
        fseek(ark,-sizeof(long int),SEEK_CUR);
        fwrite(&regPos,sizeof(long int),1,ark); // escrevo novo topo lista com a posicao do arquivo
        fseek(ark,regPos+5,SEEK_SET); // volto para o registro e pulo campos removido e tamanho do registro
        fwrite(&topoLista,sizeof(long int),1,ark); // escrevo no encad. lista o que havia em topoLista.
        fseek(ark,regTam_semEncadamentoLista,SEEK_CUR); // continuo do proximo registro
    }
    else{
        int proximoRegTam;
        long int offsetProximo;
        long int offsetAtual;
        fseek(ark,(topoLista)+1,SEEK_SET); // vejo o tamanho do primeiro registro da lista encadeada
        fread(&proximoRegTam,sizeof(int),1,ark);
		if(proximoRegTam >= regTam){
			offsetAtual = 1; // TopoLista no cabecalho
			offsetProximo = topoLista;
            fseek(ark,offsetAtual,SEEK_SET);
            fwrite(&regPos,sizeof(long int),1,ark);
            fseek(ark,regPos+5,SEEK_SET);
            fwrite(&offsetProximo,sizeof(long int),1,ark);
		}
		else{
			offsetAtual = topoLista; // TopoLista no cabecalho
			fseek(ark,topoLista+5,SEEK_SET);
			fread(&offsetProximo,sizeof(long int),1,ark);
			fseek(ark,offsetProximo+1,SEEK_SET); // vejo o tamanho do proximo registro da lista encadeada
			fread(&proximoRegTam,sizeof(int),1,ark);
			while(proximoRegTam < regTam ){ // se o tamanho do registro for maior que o proximo da lista ou se e o fim da lista
				offsetAtual = offsetProximo;
				fseek(ark,offsetAtual+5,SEEK_SET); // vejo o offset do proximo registro da lista encadeada
				fread(&offsetProximo,sizeof(long int),1,ark);
				if(offsetProximo == -1)
					break;
				fseek(ark,offsetProximo+1,SEEK_SET); // vejo o tamanho do proximo registro da lista encadeada
				fread(&proximoRegTam,sizeof(int),1,ark);
			}
			fseek(ark,offsetAtual+5,SEEK_SET);
			fwrite(&regPos,sizeof(long int),1,ark);
			fseek(ark,regPos+5,SEEK_SET);
			fwrite(&offsetProximo,sizeof(long int),1,ark);
		}

		// int atualRegTam;
		// offsetAtual = topoLista;
		// fseek(ark,offsetAtual+1,SEEK_SET); // vejo o tamanho do proximo registro da lista encadeada
        // fread(&atualRegTam,sizeof(int),1,ark);
        // fread(&offsetProximo,sizeof(long int),1,ark);
		// fseek(ark,offsetProximo+1,SEEK_SET);
		// fread(&proximoRegTam,sizeof(int),1,ark);
		// printf("Primeiro da lista =%ld Atual tam: %d\n",offsetAtual,atualRegTam); 
		// while (offsetProximo != -1){
		// 	printf("atual Offset: %ld Atual tam: %d ||| Proximo Offset: %ld Proximo Tam; %d\n",offsetAtual,atualRegTam,offsetProximo,proximoRegTam);
		// 	offsetAtual = offsetProximo;
		// 	atualRegTam = proximoRegTam;
		// 	fread(&offsetProximo,sizeof(long int),1,ark);
		// 	fseek(ark,offsetProximo+1,SEEK_SET);
		// 	fread(&proximoRegTam,sizeof(int),1,ark); 
		// }
		//fseek(ark,regPos+13,SEEK_SET);
		fseek(ark,regTam_semEncadamentoLista,SEEK_CUR);
    }
}

void insercaoLogica(FILE* ark, Registro r){
	long int topoLista;
	long int offsetAtual;
	long int offsetAnterior;
	int atualRegTam;
	long int offset;
	char lixo = LIXO;
	int i;
	char aux;
	Registro registroAnterior;
	long int offsetUltimoRegistro;
	fseek(ark,PAGINA_DISCO,SEEK_SET);
	registroAnterior = lerRegistroBin(ark);
	aux = getc(ark);
	while(aux != EOF){
		ungetc(aux,ark);
		registroAnterior = lerRegistroBin(ark);
		aux = getc(ark);
	}
	// registroAnterior = Ultimo registro do Arquivo
	ungetc(aux,ark);
	fseek(ark,-(registroAnterior.tamanhoRegistro),SEEK_CUR);
	offsetUltimoRegistro = ftell(ark);

	fseek(ark,sizeof(char),SEEK_SET);
	fread(&topoLista,sizeof(long int),1,ark);
	offsetAtual = topoLista;
	
	if(topoLista != -1){
		offsetAnterior = 1; //topolista no cabecalho
		fseek(ark,offsetAtual+1,SEEK_SET); // vejo o tamanho do registro da lista encadeada
		fread(&atualRegTam,sizeof(int),1,ark);
		while (offsetAtual != -1 && atualRegTam < r.tamanhoRegistro){ // Procuro registro com tamanho ideal para "r"
			offsetAnterior = offsetAtual;
			fread(&offsetAtual,sizeof(long int),1,ark);
			fseek(ark,offsetAtual+1,SEEK_SET);
			fread(&atualRegTam,sizeof(int),1,ark);
		}
	}
	if(topoLista == -1 || offsetAtual == -1){ // nao achei registro na lista encadeada
		fseek(ark,0,SEEK_END);
		offset = ftell(ark);
		offset = offset % PAGINA_DISCO;
		offset += r.tamanhoRegistro+5;
		if(offset > PAGINA_DISCO){
			offset -= (r.tamanhoRegistro+5);
			for(i=0; i<(PAGINA_DISCO-offset); i++){
				fwrite(&lixo,sizeof(char),1,ark);
			}
			addRegistro(r,ark);
			registroAnterior.tamanhoRegistro += i;
			fseek(ark,offsetUltimoRegistro,SEEK_SET);
			addRegistro(registroAnterior,ark);
		}
		else{
			addRegistro(r,ark);
		}
	}
	else{ // achei registro na lista encadeada
		//printf("acheiu na lista encadeada com TOPO = %ld\n",topoLista);
		// Corrigo lista encadeada
		long int offsetProximo;
		fread(&offsetProximo,sizeof(long int),1,ark);
		if(offsetAnterior == 1){
			
			fseek(ark,offsetAnterior,SEEK_SET);
		}
		else{
			fseek(ark,offsetAnterior+5,SEEK_SET);
		}
		fwrite(&offsetProximo,sizeof(long int),1,ark);

		//Insiro novo registro
		fseek(ark,offsetAtual,SEEK_SET);
		int tamanhoRegistro = r.tamanhoRegistro;
		r.tamanhoRegistro = atualRegTam;
		addRegistro(r,ark);
		for(int i = 0; i < (atualRegTam - tamanhoRegistro);i++){
			fwrite(&lixo,sizeof(char),1,ark);
		}
	}
}

void atualizacaoLogica(FILE* ark, Registro r, char* campoAtualiza, char* dadoAtualiza){
	// Campos de tamanho fixo, sem necessidade de remover e inserir registro
	if(strcmp(campoAtualiza, "idServidor") == 0){
        int id = atoi(dadoAtualiza);
		fseek(ark,-(r.tamanhoRegistro+5),SEEK_CUR);
		r.idServidor = id;
		addRegistro(r,ark);
        
    }
    else if(strcmp(campoAtualiza, "salarioServidor") == 0){
        double salario;
        if(strcmp(dadoAtualiza, "NULO") == 0){
            salario = -1;
        }
        else{
            salario = atof(dadoAtualiza); 
        }
		fseek(ark,-(r.tamanhoRegistro+5),SEEK_CUR);
		r.salarioServidor = salario;
		addRegistro(r,ark);
    }
    else if(strcmp(campoAtualiza, "telefoneServidor") == 0){
        if(strcmp(dadoAtualiza, "NULO") == 0){
            dadoAtualiza = "\0@@@@@@@@@@@@@";
        }
		fseek(ark,-(r.tamanhoRegistro+5),SEEK_CUR);
		for(int i=0; i<14; i++){
			r.telefoneServidor[i] = dadoAtualiza[i];
		}
		addRegistro(r,ark);
    }

	//Campos de tamanho Variavel
    else if(strcmp(campoAtualiza, "nomeServidor") == 0){
		char lixo = LIXO;
		long int offsetProximoRegistro = ftell(ark);
		if(strcmp(dadoAtualiza,"NULO") == 0){
			if(r.tamNome > 0){
				fseek(ark, -(r.tamanhoRegistro+5), SEEK_CUR);
				int tamanhoNome = r.tamNome + 4; // inclui indicador de tamanho
				r.tamNome = 0;
				addRegistro(r,ark);
				for(int i=0;i<tamanhoNome;i++){
					fwrite(&lixo,sizeof(char),1,ark);
				}
			}
		}
		else{
			if(r.tamNome > 0){
				if(strcmp(dadoAtualiza, r.nomeServidor) != 0){
					int diferenca = strlen(dadoAtualiza) - strlen(r.nomeServidor);
					int tamanhoRegistroReal = 34;
					tamanhoRegistroReal += (r.tamNome + 4);
					r.tamNome = strlen(dadoAtualiza) + 2;
					r.tagCampo4 = 'n';
					r.nomeServidor = malloc(sizeof(char)*(r.tamNome-1));
					strcpy(r.nomeServidor,dadoAtualiza);
					if(r.tamCargo > 0)
						tamanhoRegistroReal += (r.tamCargo + 4);
					int novoTamanho = tamanhoRegistroReal + diferenca;
					if(r.tamanhoRegistro < novoTamanho){ //Se registro nao tem espaco disponivel para alteracao
						remocaoLogica(ark,r.tamanhoRegistro);
						r.tamanhoRegistro = novoTamanho;
						insercaoLogica(ark,r);
					}
					else{//Se registro tem espaco disponivel para alteracao
						fseek(ark,-(r.tamanhoRegistro+5),SEEK_CUR);
						addRegistro(r,ark);
						for(int i = 0; i<(r.tamanhoRegistro - novoTamanho); i++){ //Completa registro com lixo
							fwrite(&lixo,sizeof(char),1,ark);
						}
					}
				}
			}
			else{
				int diferenca = strlen(dadoAtualiza)+1; // inclui '\0'
				int novoTamanho = 34;
				r.tamNome = diferenca + 1; // inclui tag
				r.tagCampo4 = 'n';
				r.nomeServidor = malloc(sizeof(char)*(r.tamNome-1));
				strcpy(r.nomeServidor,dadoAtualiza);
				novoTamanho += (r.tamNome + 4);
				if(r.tamCargo > 0)
					novoTamanho += (r.tamCargo + 4);
				if(r.tamanhoRegistro < novoTamanho){ //Se registro nao tem espaco disponivel para alteracao
					remocaoLogica(ark,r.tamanhoRegistro);
					r.tamanhoRegistro = novoTamanho;
					insercaoLogica(ark,r);
				}
				else{//Se registro tem espaco disponivel para alteracao
					fseek(ark,-(r.tamanhoRegistro+5),SEEK_CUR);
					addRegistro(r,ark);
					for(int i = 0; i<(r.tamanhoRegistro - novoTamanho); i++){ //Completa registro com lixo
						fwrite(&lixo,sizeof(char),1,ark);
					}
				}
			}
		}
		fseek(ark,offsetProximoRegistro,SEEK_SET);
	}
    else if(strcmp(campoAtualiza, "cargoServidor") == 0){
		long int offsetProximoRegistro = ftell(ark);
    	char lixo = LIXO;
		if(strcmp(dadoAtualiza,"NULO") == 0){
			if(r.tamCargo > 0){
				fseek(ark, -(r.tamanhoRegistro+5), SEEK_CUR);
				int tamanhoCargo = r.tamCargo + 4; // inclui indicador de tamanho
				r.tamCargo = 0;
				addRegistro(r,ark);
				for(int i=0;i<tamanhoCargo;i++){
					fwrite(&lixo,sizeof(char),1,ark);
				}
			}
		}
		else{
			if(r.tamCargo > 0){
				if(strcmp(dadoAtualiza, r.cargoServidor) != 0){
					int diferenca = strlen(dadoAtualiza) - strlen(r.cargoServidor);
					int tamanhoRegistroReal = 34;
					tamanhoRegistroReal += (r.tamCargo + 4);
					r.tamCargo = strlen(dadoAtualiza) + 2;
					r.tagCampo5 = 'c';
					r.cargoServidor = malloc(sizeof(char)*(r.tamCargo-1));
					strcpy(r.cargoServidor,dadoAtualiza);
					if(r.tamNome > 0)
						tamanhoRegistroReal += (r.tamNome + 4);
					int novoTamanho = tamanhoRegistroReal + diferenca;
					if(r.tamanhoRegistro < novoTamanho){ //Se registro nao tem espaco disponivel para alteracao
						remocaoLogica(ark,r.tamanhoRegistro);
						r.tamanhoRegistro = novoTamanho;
						insercaoLogica(ark,r);
					}
					else{//Se registro tem espaco disponivel para alteracao
						fseek(ark,-(r.tamanhoRegistro+5),SEEK_CUR);
						addRegistro(r,ark);
						for(int i = 0; i<(r.tamanhoRegistro - novoTamanho); i++){ //Completa registro com lixo
							fwrite(&lixo,sizeof(char),1,ark);
						}
					}
				}
			}
			else{
				int diferenca = strlen(dadoAtualiza)+1; // inclui '\0'
				int novoTamanho = 34;
				r.tamCargo = diferenca + 1; // inclui tag
				r.cargoServidor = malloc(sizeof(char)*(r.tamCargo-1));
				r.tagCampo5 = 'c';
				strcpy(r.cargoServidor,dadoAtualiza);
				novoTamanho += (r.tamCargo + 4);
				if(r.tamNome > 0)
					novoTamanho += (r.tamNome + 4);
				if(r.tamanhoRegistro < novoTamanho){ //Se registro nao tem espaco disponivel para alteracao
					remocaoLogica(ark,r.tamanhoRegistro);
					r.tamanhoRegistro = novoTamanho;
					insercaoLogica(ark,r);
				}
				else{//Se registro tem espaco disponivel para alteracao
					fseek(ark,-(r.tamanhoRegistro+5),SEEK_CUR);
					addRegistro(r,ark);
					for(int i = 0; i<(r.tamanhoRegistro - novoTamanho); i++){ //Completa registro com lixo
						fwrite(&lixo,sizeof(char),1,ark);
					}
				}
			}
		}
		fseek(ark,offsetProximoRegistro,SEEK_SET);
	}
}


// ESCREVE TELA BINARIO CODIGO MONITOR ---------------------------------------------

void binarioNaTela1(FILE *ponteiroArquivoBinario) {

	/* Escolha essa função se você ainda tem o ponteiro de arquivo 'FILE *' aberto.
	*  Lembrando que você tem que ter aberto ele no fopen para leitura também pra funcionar (exemplo: rb, rb+, wb+, ...) */

	unsigned char *mb;
	unsigned long i;
	size_t fl;
	fseek(ponteiroArquivoBinario, 0, SEEK_END);
	fl = ftell(ponteiroArquivoBinario);
	fseek(ponteiroArquivoBinario, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, ponteiroArquivoBinario);
	for(i = 0; i < fl; i += sizeof(unsigned char)) {
		printf("%02X ", mb[i]);
		if((i + 1) % 16 == 0)	printf("\n");
	}
	free(mb);
}

void binarioNaTela2(char *nomeArquivoBinario) {

	/* Escolha essa função se você já fechou o ponteiro de arquivo 'FILE *'.
	*  Ela vai abrir de novo para leitura e depois fechar. */

	unsigned char *mb;
	unsigned long i;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela2): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar? Se você não fechou ele, pode usar a outra função, binarioNaTela1, ou pode fechar ele antes de chamar essa função!\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);
	for(i = 0; i < fl; i += sizeof(unsigned char)) {
		printf("%02X ", mb[i]);
		if((i + 1) % 16 == 0)	printf("\n");
	}
	free(mb);
	fclose(fs);
}

void trim(char *str) {

	/*
	*	Essa função arruma uma string de entrada "str".
	*	Manda pra ela uma string que tem '\r' e ela retorna sem.
	*	Ela remove do início e do fim da string todo tipo de espaçamento (\r, \n, \t, espaço, ...).
	*	Por exemplo:
	*
	*	char minhaString[] = "    \t TESTE  DE STRING COM BARRA R     \t  \r\n ";
	*	trim(minhaString);
	*	printf("[%s]", minhaString); // vai imprimir "[TESTE  DE STRING COM BARRA R]"
	*
	*/

	size_t len;
	char *p;

	for(len = strlen(str); len > 0 && isspace(str[len - 1]); len--); // remove espaçamentos do fim
	str[len] = '\0';
	for(p = str; *p != '\0' && isspace(*p); p++); // remove espaçamentos do começo
	len = strlen(p);
	memmove(str, p, sizeof(char) * (len + 1));
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}