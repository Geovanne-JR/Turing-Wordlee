//Universidade de Brasília
//Departamento de Ciência da Computação/IE
//Semestre: 2/2025
//Disciplina: Algoritmos e Programação de Computadores
//Aluno : Geovanne Junio Menezes Lima
//Matrícula: 252001154
//Projeto: Turing Wordlee

#include <stdio.h> //PARA INPUT E OUTPUT
#include <stdlib.h> //PARA SYSTEM CLEAR
#include <string.h> //PARA STRCMP E STRCPY
#include <ctype.h> //DEIXAR AS LETRAS MAIUSCULAS

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define MAX_PALAVRAS 10 //MUDE AQUI PARA AUMENTAR O NUMERO MAXIMO DE PALAVRAS POR FASE
#define TAM_PALAVRA 15      //MUDE AQUI PARA AUMENTAR O TAMANHO MAXIMO DE CADA PALAVRA
#define TAM_PLAYERNAME 20           
#define ARQUIVO_FASE "fase.txt"
#define ARQ_RANKING "ranking.bin"

#define VERDE_BG    "\x1b[42m" //cor do fundo
#define RESET       "\x1b[0m"
#define VERMELHO    "\x1b[31m"
#define VERDE       "\x1b[32m"
#define AMARELO     "\x1b[33m" //CORES PARA O TERMINAL
#define AZUL        "\x1b[34m"
#define ROXO        "\x1b[35m"
#define CIANO       "\x1b[36m"
#define BRANCO      "\x1b[37m"

typedef struct {
    char PlayerName[TAM_PLAYERNAME]; //jogador e sua pontuacao
    int pontuacao;
} Player;

// PRA JOGAR OUTRAS FASES, EDITE O ARQUIVO "fase.txt" SEGUINDO AS INSTRUCOES. 
FILE *fp_fase;
char playername_atual[TAM_PLAYERNAME];
int pontuacaoTotal = 0;      

char letrasDafase[50];      
char lista_palavras[MAX_PALAVRAS][TAM_PALAVRA];
int palavras_encontradas[MAX_PALAVRAS];        
int qtdPalavrasFase;      


void salvarPontuacaoRanking();
void menuPrincipal();
void limparTela();
int carregarFaseDoArquivo(); 
void pontuacoesDoJogo(); //funcoes do menu principal, declaradas antes do main e as funcoes no final, para melhorar a visibilidade da main
void instrucoes();      
void configuracoes();    
void verRanking();       
void sairDoJogo();       


int main() {
    fp_fase = fopen(ARQUIVO_FASE, "r");
   
    if (fp_fase == NULL) {
        printf(VERMELHO "ERRO : O arquivo '%s' nao conseguiu abrir com sucesso." RESET "\n", ARQUIVO_FASE);
        printf(VERMELHO "Arquivo deve estar na mesma pasta do programa." RESET "\n");
        return 1;
    }

    limparTela();
    //cabecalhho lindo demaizi
    printf(CIANO "////////////////////////////////////////////////////////////////////////////////////////////////////////////////" RESET "\n\n");
    printf(BRANCO " //                                             ----TURING WORDLEE----                                       //  " RESET "\n");
    printf(AMARELO " //                                             BY: Geovanne/252001154                                            "            RESET "\n");
    printf(VERMELHO " //                                     ----------------VERSION 1.0-----------------                         //  " RESET "\n\n");
    printf(CIANO "////////////////////////////////////////////////////////////////////////////////////////////////////////////////" RESET "\n\n");
    
    printf("Informe seu playername: ");
    fgets(playername_atual, TAM_PLAYERNAME, stdin);
    playername_atual[strcspn(playername_atual, "\n")] = 0; //le o nome do jogador e tira o \n do final

    menuPrincipal();

    if (fp_fase != NULL) { //fecha antes de sair pra nao dar merda no windows
        fclose(fp_fase);
    }

    return 0;
}


void limparTela() {
    system(CLEAR);
}

int carregarFaseDoArquivo(){
 
    if (fscanf(fp_fase, "%s", letrasDafase) == EOF) {
        return 0; 
    }


    if(strcmp(letrasDafase, "-") == 0){                     //SE A LINHA FOR UM HIFEN, PULA PARA A PROXIMA LINHA
        if (fscanf(fp_fase, "%s", letrasDafase) == EOF) {
            return 0;
        }
    }

    fscanf(fp_fase, "%d", &qtdPalavrasFase);

    for(int i = 0; i < qtdPalavrasFase; i++){   //CARREGA AS PALAVRAS DA FASE
        fscanf(fp_fase, "%s", lista_palavras[i]);
        palavras_encontradas[i] = 0;
    }
    return 1; 
}

void desenhaAsLetrasEspalhadasPorAi(char *letras) {
    int len = strlen(letras);

    printf("\n");
    printf(AMARELO);

    if (len == 5) {
        printf("       %c           %c     \n", letras[0], letras[1]);
        printf("             %c           \n", letras[2]);             //formato de X quando tem 5 letras
        printf("       %c           %c     \n", letras[3], letras[4]);
    } 
    else if (len == 6) {
        printf("         %c       %c       \n", letras[0], letras[1]);
        printf("     %c               %c   \n", letras[2], letras[3]); //formato circular quando tem 6 letras
        printf("         %c       %c       \n", letras[4], letras[5]);
    } 
    else {
        int largura = 3;
        for (int i = 0; i < len; i++) {
            printf("   %c   ", letras[i]);                             //formato diferente se forem 7 ou mais letras
            if ((i + 1) % largura == 0) printf("\n\n");
        }
    }
    
    printf(RESET "\n"); 
}

void desenhaQuadradinhoBonito(char *palavra, int encontrada) { 
    int len = strlen(palavra);
    
    printf("   ");
    for(int i=0; i<len; i++) printf(".---."); 
    printf("\n");

    printf("   ");
    for(int i=0; i<len; i++) {
        if (encontrada) {
            printf("|" VERDE_BG BRANCO " %c " RESET "|", palavra[i]); //faz os quadrados verdes se a palavra foi encontrada
        } else {
            printf("|   |"); 
        }
    }
    printf("\n");

    printf("   ");
    for(int i=0; i<len; i++) printf("'---'");
    printf("\n");
}

void jogarFase() {
    char tentativa[TAM_PALAVRA];
    int palavrasQueSobra;
    int faseAtual = 1;

    rewind(fp_fase);  //se quiser jogr a mesma fase de novo, volta o ponteiro do arquivo pro inicio
    pontuacaoTotal = 0; 

    while (carregarFaseDoArquivo()) {
        palavrasQueSobra = qtdPalavrasFase;

        while (palavrasQueSobra > 0) {
            limparTela();
            
            printf(BRANCO "########################################\n" RESET);
            printf(CIANO  "   FASE %d    |    PONTUACAO: %d\n" RESET, faseAtual, pontuacaoTotal); //MENU DO RANKING
            printf(BRANCO "########################################\n" RESET);
            desenhaAsLetrasEspalhadasPorAi(letrasDafase); 
            printf(BRANCO "----------------------------------------\n" RESET);

            printf("Decifre %d palavras:\n\n", qtdPalavrasFase);
            
            for (int i = 0; i < qtdPalavrasFase; i++) {
                desenhaQuadradinhoBonito(lista_palavras[i], palavras_encontradas[i]); //desenha,pegando a palavra e se ja foi encontrada
            }
            printf("\n");

            printf(AMARELO "Digite a palavra ou 'SAIR': " RESET);
            scanf("%s", tentativa);
            getchar(); 

            for(int i = 0; tentativa[i]; i++){
             tentativa[i] = toupper(tentativa[i]);
            }
            if (strcmp(tentativa, "SAIR") == 0){
                salvarPontuacaoRanking();
                pontuacaoTotal = 0;
                return;
            }
   
            int acertou = 0;
            for (int i = 0; i < qtdPalavrasFase; i++) {
                if (strcmp(tentativa, lista_palavras[i]) == 0) { //compara a palavra
                    if (palavras_encontradas[i] == 1) {
                        printf(AMARELO "\n>> Ja foi encontrada mane! -10 pts.\n" RESET);
                        pontuacaoTotal -= 10;
                    } else {
                        printf(VERDE "\n>> ACERTOU! +100 pts!\n" RESET);
                        palavras_encontradas[i] = 1;
                        pontuacaoTotal += 100; //soma a pontuacao
                        palavrasQueSobra--; //diminui a quantidade de palavras que faltam
                    }
                    acertou = 1;
                    break;
                }
            }

            if (acertou == 0) {
                printf(VERMELHO "\n>> EROOOUUU! -20 pts.\n" RESET); // 
                pontuacaoTotal -= 20;
            }

            printf("Tecle <Enter>...");
            getchar();
        }

        limparTela();
        printf(VERDE "\n\n      PARABENS! FASE CONCLUIDA!      \n\n" RESET);
        printf("       Bonus de fase: +25 pontos       \n");
        pontuacaoTotal += 25;
        faseAtual++; //vai pra proxima fase
        printf("\nTecle <Enter> para a proxima fase...");
        getchar();
    }
    
    printf(VERMELHO "\nFIM DE JOGO! Voce decifrou a enigma!\n" RESET);
    printf("Pontuacao Final: %d\n", pontuacaoTotal);
    salvarPontuacaoRanking(); //salva a pontuacao no ranking
    
    
    pontuacaoTotal = 0;
    
    printf("Tecle <Enter> para voltar...");
    getchar();
}

void configuracoes() {
    limparTela();
    printf(VERMELHO "\n--- CONFIGURACOES ---\n\n" RESET);
    printf(AZUL"1. Zerar Ranking\n");
    printf(AZUL"2. Mudar dificuldade (NAO IMPLEMENTADO)\n");  //PREGUICA DE FAZER ESSA PARTE
    printf(AZUL"3. Editor de fases (NAO IMPLEMENTADO)\n"); //ESSA TAMBEM
    printf(AZUL"4. Voltar ao menu principal\n\n");
    printf(VERMELHO"Escolha uma opcao: ");
    
    int opcao;
    scanf("%d", &opcao);
    getchar();

    if (opcao == 1) {
        printf("\nTem certeza que deseja apagar todo o ranking? (S/N): ");
        char confirma;
        scanf("%c", &confirma);
        getchar();

        if (toupper(confirma) == 'S') {

            FILE *f = fopen(ARQ_RANKING, "wb");
            if (f) {
                fclose(f);
                printf(VERDE "\n NAO SOBROU NADA:( !\n" RESET);
            } else {
                printf(VERMELHO "\nNao deu pra zerar :(, algo deu errado).\n" RESET);
            }
        } else {
            printf("\nCANCELADO.\n");
        }
    } else {
        printf("\nOpcao em desenvolvimento...\n");
    }

    if(opcao == 4){
        return;
    }

}

void verRanking() {
    limparTela();
    
    FILE *arq = fopen(ARQ_RANKING, "rb"); 
    
    if (arq == NULL) {
        printf(VERMELHO "\nAinda nao tem ranking .\n" RESET); //ve se tem ranking
        printf("Jogue a primeira para atualizar o ranking\n");
        printf("\nPressione <Enter> para voltar...");
        getchar();
        return;
    }
//SE QUISER AUMENTAR O LIMITE DO RANKING, AUMENTE O TAMANAHO DO VETOR ABAIXO E TAMBEM NA IMPRESSAO DO CABEÇALHO
    Player lista[100]; 
    int n = 0; 

    while (fread(&lista[n], sizeof(Player), 1, arq) == 1) {
        n++;
        if (n >= 150) break; 
    }
    fclose(arq);

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (lista[j].pontuacao < lista[j+1].pontuacao) { //bubble sort do ranking
                Player temp = lista[j];
                lista[j] = lista[j+1];
                lista[j+1] = temp;
            }
        }
    }

    printf(BRANCO "========================================\n" RESET);
    printf(AMARELO "          HALL DA FAMA (TOP %d)        \n" RESET, n);
    printf(BRANCO "========================================\n" RESET);
    printf("   #   |   PONTOS   |   JOGADOR   \n");
    printf("-------+------------+-------------------\n");

    for (int i = 0; i < n; i++) {
        printf("   %02d  | " CIANO "  %5d   " RESET "  |   %-15s \n", i + 1, lista[i].pontuacao, lista[i].PlayerName);//pega os dados ja ordenados e printa bonitinho
    }
    printf("\n");
    
    printf("Pressione <Enter> para voltar...");
    getchar();
    getchar();
}

void instrucoes() {
    limparTela();
    printf(ROXO   "                                            Bem vindo ao Turing Wordlee!!.                                        "   RESET   "\n");
    printf(AMARELO "                                                Instrucoes do Jogo:                                                " RESET "\n\n");
    printf(BRANCO "                  1- O objetivo do jogo e encontrar palavras escondidas usando as letras fornecidas."    "\n" RESET);
    printf(BRANCO "                 2- Cada fase apresenta um conjunto de letras e uma lista de palavras para encontrar."   "\n" RESET);
    printf(BRANCO "                            3- Digite as palavras que voce acha que estao escondidas."  RESET   "\n" );
    printf(BRANCO "                4- Voce pode digitar 'SAIR' a qualquer momento para salvar seu progresso e sair do jogo." RESET "\n");
    printf(BRANCO "                 5- Cada palavra correta vale pontos, enquanto tentativas incorretas descontam pontos." RESET "\n");
    printf(BRANCO "                          6- Complete todas as palavras para passar para a proxima fase!" RESET "\n\n" );
    printf(CIANO  "                           Sera que conseguira salvar a humanidade e quebrar esse codigo?!                                            " RESET "\n\n");

    printf("                                                                \nPressione <Enter> para voltar...                                                                  ");
    getchar();
    getchar();
}

void sairDoJogo() {
    limparTela();
    printf("Saindo do jogo... adios :( !\n");
}
void fontes(){ 
    printf(VERMELHO "                                                Fontes utilizadas no projeto:\n" RESET                     );
    printf("        1. https://chrisyeh96-github-io.translate.goog/2020/03/28/terminal-colors.html?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=pt&_x_tr_pto=tc\n                  ");
    printf("                            2. https://www.youtube.com/watch?v=PkFSpFQ5l-g\n       ");
    printf("                             3. https://www.cprogressivo.net/2011/01/ponteiros-em-c.html\n                      ");
    printf("\nPressione <Enter> para voltar...");
    getchar();
    getchar();


}
void salvarPontuacaoRanking() {

    FILE *arq = fopen(ARQ_RANKING, "ab");

    if (arq == NULL) {
        printf(VERMELHO "\nErro ao salvar no ranking!\n" RESET); //ve se o arquivo abriu direito
        return;
    }

    Player p;
    strcpy(p.PlayerName, playername_atual); //coloca o player atual
    p.pontuacao = pontuacaoTotal;           //coloca a pontuacao atual

    fwrite(&p, sizeof(Player), 1, arq); //escreve o tamanho do player no arquivo

    fclose(arq); // fecha
    printf(VERDE "\nPontuacao salva no Ranking com sucesso!\n" RESET);
}

void pontuacoesDoJogo() {
    limparTela();
    printf(VERMELHO "------------------------------PONTUACOES DO JOGO------------------------------" RESET "\n\n");
    printf(VERDE "                      Cada palavra correta: +100 pontos                        " RESET "\n" );
    printf(VERMELHO "                    Palavra ja encontrada: -10 pontos                        " RESET  "\n" );
    printf(VERMELHO "                       Palavra errada: -20 pontos                          " RESET "\n" );
    printf(VERDE "                    Bonus de fase concluida: +25 pontos                      " RESET "\n\n" );

    printf("\nPressione <Enter> para voltar...");
    getchar();
    getchar();
}

void menuPrincipal() {
    int escolha;

    do {
        limparTela(); 

        printf("\n" VERMELHO "--- MENU DE OPCOES ---" RESET "\n");
        printf(AZUL "Ola, %s! Arquivo aberto e pronto para comecar." RESET  "\n", playername_atual);
        printf(AZUL "1. Jogar Fase"   RESET "\n");
        printf(AZUL "2. Configuracoes" RESET   "\n");
        printf(AZUL "3. Ver Ranking"   RESET "\n");
        printf(AZUL "4. Instrucoes" RESET "\n");
        printf(AZUL "5. Pontuacoes do jogo" RESET "\n");
        printf(AZUL "6. Fontes utilizadas no projeto" RESET "\n");
        printf(AZUL "7. Sair do Jogo" RESET "\n"); 
        
        printf(VERMELHO "Escolha uma opcao (1-7): " RESET);
        scanf("%d", &escolha);
        getchar(); 

        switch (escolha) {
            case 1:
                jogarFase();
                break;
            case 2:
                configuracoes();
                break;
            case 3:
                verRanking();
                break;
            case 4:
                instrucoes();
                break;                              //chama as funcoes conforme a escolha
            case 5:
                pontuacoesDoJogo();
                break;
            case 6:
                fontes();
                break;
            case 7:
                sairDoJogo();
                break;
            default:
                printf(VERMELHO "Ou voce nao leu ou a letra esta pequena. E de 1-7!!" RESET "\n");
                getchar();
                break;
        }

    } while (escolha != 7);
}