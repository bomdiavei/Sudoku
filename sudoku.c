#include <stdio.h>
#include <stdlib.h>
#include <time.h>//USADA PARA GERAR NOMES DE ARQUIVOS ALEATORIOS DIFERENTES A CADA EXECUÇÃO
#include <string.h>//ESSA BIBLIOTECA FOI ADICIONADA P/ TRABALHAR COM OS NOMES DOS ARQUIVOS

#define ERROR_FILE_MSG	"Nao foi possivel abrir o arquivo!\n"
#define INVALID_OPTION	"Opcao invalida! Tente novamente!"

enum Boolean {
	FALSO=0, VERDADEIRO=1
};
/* Prototipos */
FILE* carregar(char quadro[9][9],int *qtd_jogadas, char *nome_arquivo);//CASSIO
FILE* carregar_continuar_jogo (char quadro[9][9], char *nome_arquivo, int *qtd_jogadas);//CASSIO
void carregar_novo_jogo(char quadro[9][9], char *nome_arquivo);//CARLOS
int determinarQuadrante(int x, int y);
FILE* criar_arquivo_binario(char quadro[9][9], char *nome_arquivo);//CASSIO
int eValido(const char quadro[9][9], int x, int y, int valor);//CARLOS
int eValidoNaColuna(const char quadro[9][9], int y, int valor);//CASSIO
int eValidoNoQuadro3x3(const char quadro[9][9], int x, int y, int valor);//CASSIO
int eValidoNaLinha(const char quadro[9][9], int x, int valor);//CARLOS
int existeCampoVazio(const char quadro[9][9]);
void imprimir(const char quadro[9][9]);//CASSIO
void jogar();//CARLOS
void resolver(FILE *fb, char quadro[9][9], char *nome_arquivo, int *qtd_jogadas);//CASSIO
void resolverUmPasso(char quadro[9][9]);//CARLOS
void salvar_jogada_bin(FILE *fb, char quadro[9][9], char *nome_arquivo, int *qtd_jogadas);//CARLOS

/* Funcoes auxiliares */
int fimX(int quadr);
int fimY(int quadr);
int le_opcao();
void gen_random(char *s, const int len);
int iniX(int quadr);
int iniY(int quadr);
void menu();
void menu_arquivo();


/* -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * MAIN
 * /////////////////////////////////////////////////////////////////////////////
 */
int main() {

	// inicia o jogo
	jogar();

	return 0;
}

/* -----------------------------------------------------------------------------
 * CARREGAR
 * Inicializa o SUDOKU a partir de um novo jogo ou estado de jogo anterior
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE* carregar(char quadro[9][9],int *qtd_jogadas, char *nome_arquivo){//CASSIO
	int opcao,qtd_jogadas1;
	/*para evitar passar por parametro a variavel "nome_arquivo" que ja esta sendo
	recebida como parametro, criei esta variavel, achei mais facil implementar assim*/
	char nome_arquivo1[20];
	FILE *f;
	f=NULL;
	menu_arquivo();
	opcao = le_opcao();

	// TODO Função incompleta

	switch(opcao) {

		// carregar novo sudoku
		case 1:
            puts("Insira o nome do arquivo: ");
            scanf("%s",nome_arquivo1);
            fflush(stdin);
            carregar_novo_jogo(quadro,nome_arquivo1);
            *qtd_jogadas=0;//JÁ QUE VAI SER UM NOVO SUDOKU, ENTÃO O NUMERO DE JOGADAS TEM QUE SER ZERO
			break;

		// continuar jogo
		case 2:
            puts("Digite o nome do arquivo do jogo salvo que deseja continuar: ");
            scanf("%s",nome_arquivo1);
            fflush(stdin);
            qtd_jogadas1=*qtd_jogadas;
            f=carregar_continuar_jogo(quadro,nome_arquivo1,&qtd_jogadas1);
            *qtd_jogadas=qtd_jogadas1;
            strcpy(nome_arquivo,nome_arquivo1);//AGORA É SÓ COPIAR O CONTEÚDO PARA "nome_arquivo"...ACHEI MAIS FACIL A IMPLEMENTAÇÃO USANDO ESTE MÉTODO
			break;

		// retornar ao menu anterior
		case 9:
			break;

		default:
			puts(INVALID_OPTION);
			break;
	}
    return f;
}

/* -----------------------------------------------------------------------------
 * CARREGAR CONTINUAR JOGO
 * Carrega um estado de jogo a partir de um arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE* carregar_continuar_jogo (char quadro[9][9], char *nome_arquivo, int *qtd_jogadas) {//CASSIO
	// TODO
	int i,j;
    FILE *f;
    strcat(nome_arquivo,".bin");//ACRESCENTA .BIN PARA O NOME DIGITADO DO USUARIO
    f=fopen(nome_arquivo,"rb");
    if(f==NULL){
        puts(ERROR_FILE_MSG);
    }
    else{
      fseek(f,0,SEEK_SET);//fseek(ARQUIVO,BITS A PARTIR DO PONTO INDICADO, PONTO INDICADO)
      fread(qtd_jogadas,sizeof(int),1,f); //Lê do arquivo 1 item e transfere para a variavel de tamanho int
      /*fseek(ARQUIVO,(ESPAÇO P/ GUARDAR A QUANTIDADE DE JOGADAS)+(ESPAÇO P/ GUARDAR O QUADRO ORIGINAL)+(ESPAÇO P/
      GUARDAR QUADROS MODIFICADOS DE CADA JOGADA)-(ESPAÇO P/ GUARDAR O ULTIMO QUADRO),PONTO INDICADO)*/
      fseek(f,sizeof(int)+81+((*qtd_jogadas-1) *81),SEEK_SET);
      for(i=0;i<9;i++){
         for(j=0;j<9;j++){
            fread(quadro,sizeof(char),81,f);
         }
      }
    }
    fclose(f);
    return f;
}

/* ------------------------------------------------------
-----------------------
 * CARREGAR NOVO JOGO
 * Carrega um novo jogo do Sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void carregar_novo_jogo(char quadro[9][9], char *nome_arquivo) {//CARLOS
	// TODO
	int i,j;
	FILE *f;
	strcat(nome_arquivo,".txt");//Realiza a concatenação ou ligação do conteúdo de uma variável a outra.
	f=fopen(nome_arquivo,"r");
	if(f==NULL){
		puts(ERROR_FILE_MSG);
	}
	else{
		for(i=0;i<9;i++){
			for(j=0;j<9;j++){
				fscanf(f,"%d",&quadro[i][j]);
			}
		}
	}
	fclose(f);
}

/* -----------------------------------------------------------------------------
 * CRIAR ARQUIVO BINARIO
 * Criar arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE* criar_arquivo_binario(char quadro[9][9], char *nome_arquivo) {//CASSIO
	// TODO
	FILE *f;
	int qtd_jogadas1;
	char nome_arquivo1[10];
	qtd_jogadas1=0;
	gen_random(nome_arquivo1,4);//GEROU UM NOME ALEATÓRIO P/ nome_arquivo1
	strcat(nome_arquivo1,".bin");//ADICIONANDO A EXTENÇÃO P/ O NOME DO ARQUIVO
	f=fopen(nome_arquivo1,"wb");
	if(f==NULL){
		puts(ERROR_FILE_MSG);
	}
	else{
        fwrite(&qtd_jogadas1,sizeof(int),1,f);
		fwrite(quadro,sizeof(char),81,f);
	}
    strcpy(nome_arquivo,nome_arquivo1);
    fclose(f);
	return f;
}

/* -----------------------------------------------------------------------------
 * DETERMINAR QUADRANTE
 * Dado as posicoes x e y, determina o quadrante do quadro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int determinarQuadrante(int x, int y) {
	if (x < 3 && y < 3)
		return 1;
	else if (x < 3 && y < 6)
		return 2;
	else if (x < 3 && y < 9)
		return 3;
	else if (x < 6 && y < 3)
		return 4;
	else if (x < 6 && y < 6)
		return 5;
	else if (x < 6 && y < 9)
		return 6;
	else if (x < 9 && y < 3)
		return 7;
	else if (x < 9 && y < 6)
		return 8;
	else
		return 9;
}

/* -----------------------------------------------------------------------------
 * E VALIDO
 * Determina se um valor na posicao x e y e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int eValido(const char quadro[9][9], int x, int y, int valor) {//CARLOS

	// verifica as condicoes
    if (quadro[x][y]!=0)//FALTOU VERIFICAR SE A POSIÇÃO JÁ ESTÁ OCUPADA
        return FALSO;
	if (!eValidoNaColuna(quadro, y, valor))
		return FALSO;
	if (!eValidoNaLinha(quadro, x, valor))
		return FALSO;
	if (!eValidoNoQuadro3x3(quadro, x, y, valor))
		return FALSO;

	return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA COLUNA
 * Verifica se um valor na coluna y e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int eValidoNaColuna(const char quadro[9][9], int y, int valor) {//CASSIO
	// TODO
	int i;
    for(i=0;i<9;i++){
        if(valor==quadro[i][y]){
            return FALSO;
        }
    }
    return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA LINHA
 * Verifica se um valor na linha x e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int eValidoNaLinha(const char quadro[9][9], int x, int valor) {//CARLOS
	// TODO
	int i;
    for(i=0;i<9;i++){
        if(valor==quadro[x][i]){
            return FALSO;
        }
    }
    return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NO QUADRO 3X3
 * Verifica se um valor e valido no quadrante da posicao x, y
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int eValidoNoQuadro3x3(const char quadro[9][9], int x, int y, int valor) {//CASSIO
	// TODO
	int i,j;
	int quadr,inicio_x,inicio_y,final_x,final_y;
	quadr=determinarQuadrante(x,y);
	inicio_x=iniX(quadr);
	inicio_y=iniY(quadr);
	final_x=fimX(quadr);
	final_y=fimY(quadr);
	for(i=inicio_x;i<=final_x;i++){
        for(j=inicio_y;j<=final_y;j++){
            if(quadro!=0 && valor==quadro[i][j]){//POSIÇÕES VAZIAS NÃO PRECISAM SER COMPARADAS
                return FALSO;
            }
        }
	}
    return VERDADEIRO;
}



/* -----------------------------------------------------------------------------
 * EXISTE CAMPO VAZIO
 * Verifica se existe um campo nao preenchido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int existeCampoVazio(const char quadro[9][9]) {
	int i, j;

	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			if (quadro[i][j] == 0)
				return VERDADEIRO;
		}
	}

	return FALSO;
}

/* -----------------------------------------------------------------------------
 * IMPRIMIR
 * Imprime o quadro inteiro do sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void imprimir(const char quadro[9][9]) {//CASSIO
	int i, j;

    puts("~~~~~~~~ SUDOKU ~~~~~~~~~");
	puts("    1 2 3   4 5 6   7 8 9");
	for (i = 0; i < 9; i++) {
		if (i % 3 == 0)
			puts("  -------------------------");
		for (j = 0; j < 9; j++) {

			if (j == 0)
				printf("%d | ", i+1);
			else if (j % 3 == 0)
				printf("| ");

			if (quadro[i][j] == 0)
				printf("- ");
			else
				printf("%d ", quadro[i][j]);
		}
		puts("|");
	}
	puts("  -------------------------");
}

/* -----------------------------------------------------------------------------
 * JOGAR
 * Realiza toda a logica do jogo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void jogar() {//CARLOS
	int qtd_jogadas,opcao;//"qtd_jogadas" SERÁ USADA PARA O MÉTODO DE IMPLEMENTAÇÃO DE SALVAMENTO DOS ARQUIVOS
	char nome_arquivo[16];
	char quadro[9][9] = { {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	FILE *fb = NULL;

	qtd_jogadas=0;
	opcao = 0;

	while (opcao != 9) {
		// imprime na tela o quadro atual
		imprimir(quadro);

		// apresenta um menu com as opcoes
		menu();
		opcao = le_opcao();

		switch (opcao) {

		// carregar sudoku
		case 1:
			fb = carregar(quadro,&qtd_jogadas,nome_arquivo);

			if (fb == NULL) {
				fb = criar_arquivo_binario(quadro,nome_arquivo);
			}
			break;

		// preencher quadro com um valor
		case 2: {
			int x, y, valor;

			printf("Entre com a posicao e o valor (linha, coluna, valor): ");
			scanf("%d%d%d", &x, &y, &valor);


			if (eValido(quadro, x, y, valor)) {
				quadro[x][y] = valor;
				salvar_jogada_bin(fb,quadro,nome_arquivo,&qtd_jogadas);
				qtd_jogadas++;
                printf("O nome do arquivo do jogo atual: %.4s\n",nome_arquivo);//IMPRIME O NOME DO ARQUIVO SEM ".bin", OU SEJA, SEM A EXTENSÃO
			}
			else
				puts("Valor ou posicao incorreta! Tente novamente!");
		}
			break;

		// resolver 1 passo
		case 3:
			resolverUmPasso(quadro);
			salvar_jogada_bin(fb,quadro,nome_arquivo,&qtd_jogadas);
			puts("Um passo resolvido!");
			qtd_jogadas++;//MAIS UM QUADRO ADICIONADO NO ARQUIVO. IMPORTANTE SEMPRE ATUALIZAR A VARIAVEL
			break;

		// resolver o sudoku
		case 4:
			resolver(fb, quadro, nome_arquivo, &qtd_jogadas);
			break;

		case 9:
			puts("Programa finalizado ..");
			break;

		default:
			puts(INVALID_OPTION);
			break;
		}
	}
}

/* -----------------------------------------------------------------------------
 * RESOLVER
 * Resolve o sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
 //P/ SALVAR JOGADA PRECISO DO NOME DO ARQUIVO E DA QUANTIDADE DE JOGADAS
void resolver(FILE *fb, char quadro[9][9], char *nome_arquivo, int *qtd_jogadas){//CASSIO

	// enquanto houver posicoes vazias no quadro
	while(existeCampoVazio(quadro)) {
		resolverUmPasso(quadro);
		salvar_jogada_bin(fb, quadro,nome_arquivo,qtd_jogadas);
		*qtd_jogadas=*qtd_jogadas+1;
		//A CADA PASSO RESOLVIDO, TEMOS DE ATUALIZAR A QUANTIDADE DE JOGADAS, PARA MANTER ORGANIZADO O ARQUIVO BINÁRIO
	}
}

/* -----------------------------------------------------------------------------
 * RESOLVER UM PASSO
 * Preenche apenas um campo vazio
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
 /*PROFESSOR DEU A DICA DE QUE SÓ É POSSÍVEL UMA POSIÇÃO P/ CADA NÚMERO
POIS É UM COMPORTAMENTO DO SUDOKU NO NIVEL FÁCIL, O QUAL ESTAMOS TRABALHANDO EMCIMA,
ESSA DICA AJUDOU MUITO NO RACIOCÍNIO DA SEGUINTE FUNÇÃO*/
void resolverUmPasso(char quadro[9][9]) {//CARLOS
	// TODO
    int i,j,k;
    int qtd,x,jogadas_possiveis;
    qtd=0;
    x=0;
    for(i=0;i<9;i++){
        if(qtd==0){
            for(j=0;j<9;j++){
                jogadas_possiveis=0;
                /*SE "qtd" FOR DIFERENTE DE ZERO, SIGNIFICA QUE FOI ENCONTRADO UMA POSIÇÃO EM QUE SÓ
                É POSSÍVEL UM VALOR, E TAMBÉM FOI ENCONTRADO TAL VALOR*/
                if(qtd==0 && quadro[i][j]==0){//SE (AINDA NÃO FOI ACHADO UMA JOGADA E A DETERMINADA POSIÇÃO ESTIVER VAZIA)
                    for(k=1;k<10;k++){
                        if(eValido(quadro,i,j,k)){//  eValido(quadro,x,y,valor)  VERIFICA A QUANTIDADE DE JOGADAS PERMITIDAS NA DETERMINADA POSIÇÃO
                            x=k;
                            jogadas_possiveis++;
                        }
                    }
                    if(jogadas_possiveis==1){//SE A CONDIÇÃO FOR SATISFEITA, SIGNIFICA QUE ACHAMOS UMA JOGADA
                        qtd++;
                        quadro[i][j]=x;
                    }
                }
            }
        }
    }
}

/* -----------------------------------------------------------------------------
 * SALVAR JOGADA BINARIO
 * Salva o estado atual do quadro no arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void salvar_jogada_bin (FILE *fb, char quadro[9][9], char *nome_arquivo, int *qtd_jogadas) {//CARLOS
	// TODO
    fb = fopen(nome_arquivo,"wb");
    if(fb==NULL){
        puts(ERROR_FILE_MSG);
        exit(1);
    }
	fseek(fb,0,SEEK_SET);
	fwrite(qtd_jogadas,sizeof(int),1,fb);//PRIMEIROS BYTES = RESERVADOS PARA INFORMAR A QUANTIDADE DE JOGADAS
	fseek(fb,sizeof(int)+81+(*qtd_jogadas*81),SEEK_SET);//fseek(ARQUIVO, PRIMEIROS BYTES + QUADRO ORIGINAL + QUADROS DE JOGADAS ANTERIORES, INICIO DO ARQUIVO)
	fwrite(quadro,sizeof(char),81,fb);//ESCREVENDO O NOVO QUADRO NO ARQUIVO (NOVA JOGADA = ESCREVE UM NOVO QUADRO NO ARQUIVO COMO SOLICITADO NA DESCRIÇÃO DO TRABALHO)
    fclose(fb);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 							FUNCOES AUXILIARES
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -----------------------------------------------------------------------------
 * FIM X
 * Indice final da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fimX(int quadr) {
	switch(quadr) {
		case 1:
		case 2:
		case 3:
			return 2;

		case 4:
		case 5:
		case 6:
			return 5;

		default:
			return 8;
	}
}

/* -----------------------------------------------------------------------------
 * FIM Y
 * Indice final da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fimY(int quadr) {
	switch(quadr) {
		case 1:
		case 4:
		case 7:
			return 2;

		case 2:
		case 5:
		case 8:
			return 5;

		default:
			return 8;
	}
}

/* -----------------------------------------------------------------------------
 * GEN_RANDOM
 * Gera uma cadeira de caracteres randomica de tamanho len
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void gen_random(char *s, const int len) {
	int i;
	srand(time(NULL));
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

/* -----------------------------------------------------------------------------
 * INI X
 * Indice inicial da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int iniX(int quadr) {
	switch(quadr) {
		case 1:
		case 2:
		case 3:
			return 0;

		case 4:
		case 5:
		case 6:
			return 3;

		default:
			return 7;
	}
}

/* -----------------------------------------------------------------------------
 * INI Y
 * Indice inicial da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int iniY(int quadr) {
	switch(quadr) {
		case 1:
		case 4:
		case 7:
			return 0;

		case 2:
		case 5:
		case 8:
			return 3;

		default:
			return 7;
	}
}

/* -----------------------------------------------------------------------------
 * LE OPCAO
 * Imprime a mensagem a faz a leitura da opcao
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int le_opcao () {
	int opcao;

	printf("Opcao: ");
	scanf("%d", &opcao);

	return opcao;
}

/* -----------------------------------------------------------------------------
 * MENU
 * Imprime o menu de opcoes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu() {
	puts("\n~~~~~~~~ SUDOKU ~~~~~~~~");
	puts("[1] - Carregar jogo");
	puts("[2] - Jogar");
	puts("[3] - Resolver um passo");
	puts("[4] - Resolver");
	puts("[9] - Finalizar");
	puts("--------");
}

/* -----------------------------------------------------------------------------
 * MENU ARQUIVO
 * Imprime o menu de opcoes do arquivo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu_arquivo() {
	puts("\n~~~~~ MENU ARQUIVO ~~~~~");
	puts("[1] - Novo jogo");
	puts("[2] - Continuar jogo");
	puts("[9] - Retornar ao menu anterior");
	puts("--------");
}
