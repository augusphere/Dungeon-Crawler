#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <locale.h>

/* Definições de Limites Máximos */
#define MAX_ROWS 25
#define MAX_COLS 25

/* Estados do Jogo */
#define ESTADO_MENU     0
#define ESTADO_TUTORIAL 1
#define ESTADO_VILA     2
#define ESTADO_FASE1    3
#define ESTADO_FASE2    4
#define ESTADO_FASE3    5
#define ESTADO_VITORIA  6
#define ESTADO_GAMEOVER 7

/* Estruturas */
struct Jogador {
    int x;
    int y;
    char simbolo;
    int vidas;
    int arma;     /* 1: Espada, 2: Arco, 3: Cajado */
    int chaves;
};

struct Monstro {
    int x;
    int y;
    char tipo;         
    int ativo;          
    int vida_boss;      
    char sub_celula;    
    int stun;           
};

/* Variáveis Globais (Padrão C89) */
char mapa[MAX_ROWS][MAX_COLS];
int linhas_atuais = 10;
int colunas_atuais = 10;
int estado_atual = ESTADO_MENU;

struct Jogador player;
struct Monstro monstros[20];
int total_monstros = 0;

/* Protótipos das Funções */
void inicializar_jogo(void);
void carregar_fase(int fase);
void exibir_historia(int fase);
void desenhar_mapa(void);
void processar_entrada(char comando);
void atualizar_monstros(void);
void realizar_ataque(void);
void interagir(void);
void exibir_menu(void);
void exibir_tutorial(void);
void exibir_creditos(void);

int main(void) {
	setlocale(LC_ALL,"");
    char input;
    srand((unsigned int)time(NULL));
    
    inicializar_jogo();

    while (1) {
        if (estado_atual == ESTADO_MENU) {
            exibir_menu();
        } else if (estado_atual == ESTADO_TUTORIAL) {
            exibir_tutorial();
        } else if (estado_atual == ESTADO_GAMEOVER) {
            system("cls");
            printf("\n===================================\n");
            printf("            GAME OVER              \n");
            printf("===================================\n");
            printf(" A escuridao consumiu sua alma e o\n");
            printf(" mundo caiu em ruinas eternas...\n\n");
            printf("Pressione qualquer tecla para voltar ao menu...");
            getch();
            estado_atual = ESTADO_MENU;
            inicializar_jogo();
        } else if (estado_atual == ESTADO_VITORIA) {
            system("cls");
            printf("\n===================================\n");
            printf("       PARABENS! VITORIA!          \n");
            printf("===================================\n");
            printf(" Com um golpe final, o Guardiao Z cai.\n");
            printf(" A energia maligna se dissipa da masmorra.\n");
            printf(" A paz finalmente retornou a vila!\n\n");
            printf("Pressione qualquer tecla para ver os creditos...");
            getch();
            exibir_creditos();
            break;
        } else {
            system("cls");
            desenhar_mapa();
            printf("\nVidas: %d | Chaves: %d | Arma: %s\n", 
                   player.vidas, player.chaves, 
                   (player.arma == 1 ? "Espada" : (player.arma == 2 ? "Arco e Flecha" : (player.arma == 3 ? "Cajado" : "Nenhuma"))));
            printf("Comandos: W,A,S,D (Mover) | I (Interagir) | O (Atacar)\n");
            printf("Escolha sua acao: ");
            
            input = (char)getch();
            processar_entrada(input);
        }
    }
    return 0;
}

void inicializar_jogo(void) {
    player.vidas = 3;
    player.chaves = 0;
    player.arma = 0; 
    player.simbolo = 'A'; /* Começa olhando para cima */
    estado_atual = ESTADO_MENU;
}

/* Nova função modular que insere a história sem mexer na estrutura do jogo */
void exibir_historia(int fase) {
    system("cls");
    printf("====================================================\n");
    printf("                  DIARIO DE JORNADA                 \n");
    printf("====================================================\n\n");

    if (fase == ESTADO_VILA) {
        printf(" CAPITULO 1: O Clamor de Eldoria\n\n");
        printf(" As terras outrora ferteis de Eldoria estao secas.\n");
        printf(" Sussurros profanos ecoam da masmorra ao norte.\n");
        printf(" Voce acorda no centro do vilarejo. Sua missao e clara:\n");
        printf(" Encontre o Anciao da Vila (N) para obter uma arma\n");
        printf(" e cruze o portal de luz (L) para a masmorra.\n");
    } 
    else if (fase == ESTADO_FASE1) {
        printf(" CAPITULO 2: Os Saguao dos Esquecidos\n\n");
        printf(" Voce cruza o portal e o ar se torna gélido.\n");
        printf(" As paredes de pedra (*) cercam seus passos.\n");
        printf(" Para avancar, voce deve encontrar chaves (@)\n");
        printf(" escondidas atras de velhas caixas de madeira (k)\n");
        printf(" e destrancar as pesadas portas de ferro (D).\n");
    } 
    else if (fase == ESTADO_FASE2) {
        printf(" CAPITULO 3: As Cavernas do Medo\n\n");
        printf(" O solo racha. O cheiro de enxofre denuncia o perigo.\n");
        printf(" Chao falso esconde espinhos mortais (#).\n");
        printf(" Uma Aberracao Errante (X) espreita este piso,\n");
        printf(" movendo-se de forma imprevisivel. Ative os\n");
        printf(" mecanismos corretos para abrir caminho.\n");
    } 
    else if (fase == ESTADO_FASE3) {
        printf(" CAPITULO FINAL: O Trono de Sangue\n\n");
        printf(" Voce alcancou o santuaria interno da colmeia.\n");
        printf(" O Predador Ortogonal (Y) ja sentiu seu cheiro.\n");
        printf(" No fundo da sala, o terrivel Guardiao da Cripta (Z)\n");
        printf(" empunha sua lamina diagonal fatal.\n");
        printf(" Nao ha mais volta. E a sua vida ou a deles!\n");
    }

    printf("\n====================================================\n");
    printf("Pressione qualquer tecla para entrar no cenário...");
    getch();
}

void carregar_fase(int fase) {
    int i, j;
    total_monstros = 0;

    /* Dispara a tela de história correspondente antes de gerar o mapa */
    exibir_historia(fase);

    if (fase == ESTADO_VILA) {
        linhas_atuais = 10; colunas_atuais = 10;
        for(i = 0; i < 10; i++) {
            for(j = 0; j < 10; j++) {
                if (i == 0 || i == 9 || j == 0 || j == 9) mapa[i][j] = '*';
                else mapa[i][j] = ' ';
            }
        }
        mapa[3][3] = 'N'; 
        mapa[1][8] = 'L'; 
        player.x = 5; player.y = 5;
    } 
    else if (fase == ESTADO_FASE1) {
        linhas_atuais = 10; colunas_atuais = 10;
        for(i = 0; i < 10; i++) {
            for(j = 0; j < 10; j++) {
                if (i == 0 || i == 9 || j == 0 || j == 9 || (i == 4 && j < 7)) mapa[i][j] = '*';
                else mapa[i][j] = ' ';
            }
        }
        mapa[2][2] = '@'; 
        mapa[4][7] = 'D'; 
        mapa[6][5] = 'k'; 
        mapa[6][6] = 'k'; 
        mapa[8][8] = 'L'; 
        player.x = 2; player.y = 7;
    }
    else if (fase == ESTADO_FASE2) {
        linhas_atuais = 15; colunas_atuais = 15;
        for(i = 0; i < 15; i++) {
            for(j = 0; j < 15; j++) {
                if (i == 0 || i == 14 || j == 0 || j == 14 || (i == 7 && j != 4 && j != 10)) mapa[i][j] = '*';
                else mapa[i][j] = ' ';
            }
        }
        mapa[3][3] = '@'; mapa[3][11] = '@'; 
        mapa[7][4] = 'D'; mapa[7][10] = 'D'; 
        mapa[5][5] = '#'; mapa[5][9] = '#';  
        mapa[2][7] = 'O';                    
        mapa[10][7] = '*';                   
        mapa[13][13] = 'L';                  
        
        monstros[0].x = 12; monstros[0].y = 3; monstros[0].tipo = 'X';
        monstros[0].ativo = 1; monstros[0].sub_celula = ' '; monstros[0].stun = 0;
        mapa[3][12] = 'X';
        total_monstros = 1;

        player.x = 2; player.y = 2;
    }
    else if (fase == ESTADO_FASE3) {
        linhas_atuais = 25; colunas_atuais = 25;
        for(i = 0; i < 25; i++) {
            for(j = 0; j < 25; j++) {
                if (i == 0 || i == 24 || j == 0 || j == 24 || (i == 12 && j > 5 && j < 20)) mapa[i][j] = '*';
                else mapa[i][j] = ' ';
            }
        }
        mapa[2][2] = '@'; mapa[2][22] = '@'; mapa[22][2] = '@'; 
        mapa[12][5] = 'D'; mapa[12][12] = 'D'; mapa[12][19] = 'D'; 
        
        monstros[0].x = 5; monstros[0].y = 15; monstros[0].tipo = 'Y';
        monstros[0].ativo = 1; monstros[0].sub_celula = ' '; monstros[0].stun = 0;
        mapa[15][5] = 'Y';
        
        monstros[1].x = 12; monstros[1].y = 20; monstros[1].tipo = 'Z';
        monstros[1].ativo = 1; monstros[1].vida_boss = 3; monstros[1].sub_celula = ' '; monstros[1].stun = 0;
        mapa[20][12] = 'Z';
        
        total_monstros = 2;
        player.x = 12; player.y = 2;
    }

    mapa[player.y][player.x] = player.simbolo;
}

void desenhar_mapa(void) {
    int i, j;
    for (i = 0; i < linhas_atuais; i++) {
        for (j = 0; j < colunas_atuais; j++) {
            putchar(mapa[i][j]);
        }
        putchar('\n');
    }
}

void processar_entrada(char comando) {
    int proximo_x = player.x;
    int proximo_y = player.y;
    char prox_celula;

    switch (comando) {
        case 'w': case 'W': proximo_y--; player.simbolo = 'A'; break;
        case 's': case 'S': proximo_y++; player.simbolo = 'V'; break;
        case 'a': case 'A': proximo_x--; player.simbolo = '<'; break;
        case 'd': case 'D': proximo_x++; player.simbolo = '>'; break;
        case 'i': case 'I': interagir(); return;
        case 'o': case 'O': realizar_ataque(); atualizar_monstros(); return;
        default: return; 
    }

    if (proximo_x < 0 || proximo_x >= colunas_atuais || proximo_y < 0 || proximo_y >= linhas_atuais) {
        return;
    }

    prox_celula = mapa[proximo_y][proximo_x];

    if (prox_celula == '*' || prox_celula == 'k' || prox_celula == 'D' || prox_celula == 'N') {
        mapa[player.y][player.x] = player.simbolo;
        return; 
    }

    if (prox_celula == 'O') {
        if (estado_atual == ESTADO_FASE2) {
            mapa[10][7] = ' '; 
        }
    }

    if (prox_celula == '#') {
        player.vidas--;
        if (player.vidas <= 0) {
            estado_atual = ESTADO_GAMEOVER;
        } else {
            carregar_fase(estado_atual);
        }
        return;
    }

    if (prox_celula == '@') {
        player.chaves++;
    }

    if (prox_celula == 'L') {
        if (estado_atual == ESTADO_VILA) estado_atual = ESTADO_FASE1;
        else if (estado_atual == ESTADO_FASE1) estado_atual = ESTADO_FASE2;
        else if (estado_atual == ESTADO_FASE2) estado_atual = ESTADO_FASE3;
        carregar_fase(estado_atual);
        return;
    }

    mapa[player.y][player.x] = ' ';
    player.x = proximo_x;
    player.y = proximo_y;
    mapa[player.y][player.x] = player.simbolo;

    if (estado_atual != ESTADO_VILA) {
        atualizar_monstros();
    }
}

void atualizar_monstros(void) {
    int i, dir, m_prox_x, m_prox_y;
    int diff_x, diff_y;
    char destino;

    for (i = 0; i < total_monstros; i++) {

        if (!monstros[i].ativo)
            continue;

        if (monstros[i].stun > 0) {
            monstros[i].stun--;
            continue;
        }

        m_prox_x = monstros[i].x;
        m_prox_y = monstros[i].y;

        /* Tipo X: Movimento aleatório */
        if (monstros[i].tipo == 'X') {
            dir = rand() % 4;
            if (dir == 0) m_prox_y--;
            else if (dir == 1) m_prox_y++;
            else if (dir == 2) m_prox_x--;
            else m_prox_x++;
        }

        /* Tipo Y: Perseguição Ortogonal Simples */
        else if (monstros[i].tipo == 'Y') {
            diff_x = player.x - monstros[i].x;
            diff_y = player.y - monstros[i].y;

            if (abs(diff_x) >= abs(diff_y) && diff_x != 0)
                m_prox_x += (diff_x > 0) ? 1 : -1;
            else if (diff_y != 0)
                m_prox_y += (diff_y > 0) ? 1 : -1;
        }
        
        /* Tipo Z (Boss): Movimento Diagonal Letal */
        else if (monstros[i].tipo == 'Z') {
            diff_x = player.x - monstros[i].x;
            diff_y = player.y - monstros[i].y;

            if (diff_x != 0) m_prox_x += (diff_x > 0) ? 1 : -1;
            if (diff_y != 0) m_prox_y += (diff_y > 0) ? 1 : -1;
        }

        if (m_prox_x < 0 || m_prox_x >= colunas_atuais || m_prox_y < 0 || m_prox_y >= linhas_atuais)
            continue;

        destino = mapa[m_prox_y][m_prox_x];

        if (destino == '*' || destino == 'D' || destino == 'k' ||
            destino == '#' || destino == '@' || destino == 'O' || destino == 'L')
            continue;

        mapa[monstros[i].y][monstros[i].x] = monstros[i].sub_celula;

        monstros[i].x = m_prox_x;
        monstros[i].y = m_prox_y;

        if (monstros[i].x == player.x && monstros[i].y == player.y) {
            player.vidas--;

            if (player.vidas <= 0)
                estado_atual = ESTADO_GAMEOVER;
            else
                carregar_fase(estado_atual);

            return;
        }

        monstros[i].sub_celula = mapa[monstros[i].y][monstros[i].x];
        mapa[monstros[i].y][monstros[i].x] = monstros[i].tipo;
    }
}

void realizar_ataque(void) {
    int i, range, ax, ay;
    int dir_x = 0, dir_y = 0;

    if (player.arma == 0) return;

    if (player.simbolo == 'A') dir_y = -1;
    else if (player.simbolo == 'V') dir_y = 1;
    else if (player.simbolo == '<') dir_x = -1;
    else if (player.simbolo == '>') dir_x = 1;

    /* ARMA 2: Arco e Flecha */
    if (player.arma == 2) {
        for (range = 1; range <= 4; range++) {
            ax = player.x + (dir_x * range);
            ay = player.y + (dir_y * range);

            if (ax < 0 || ax >= colunas_atuais || ay < 0 || ay >= linhas_atuais) break;

            if (mapa[ay][ax] == 'k') { mapa[ay][ax] = ' '; break; } 
            
            for (i = 0; i < total_monstros; i++) {
                if (monstros[i].ativo && monstros[i].x == ax && monstros[i].y == ay) {
                    if (monstros[i].tipo == 'Z') {
                        monstros[i].vida_boss--;
                        monstros[i].stun = 1; 
                        if (monstros[i].vida_boss <= 0) { 
                            monstros[i].ativo = 0; 
                            mapa[ay][ax] = monstros[i].sub_celula; 
                            estado_atual = ESTADO_VITORIA; 
                        }
                    } else {
                        monstros[i].ativo = 0;
                        mapa[ay][ax] = monstros[i].sub_celula;
                    }
                    return;
                }
            }
        }
    }
    /* ARMA 1: Espada */
    else if (player.arma == 1) {
        int r_row, r_col;
        for (r_row = 1; r_row <= 2; r_row++) {
            for (r_col = -1; r_col <= 1; r_col++) {
                if (dir_y != 0) {
                    ax = player.x + r_col; ay = player.y + (dir_y * r_row);
                } else {
                    ax = player.x + (dir_x * r_row); ay = player.y + r_col;
                }

                if (ax < 0 || ax >= colunas_atuais || ay < 0 || ay >= linhas_atuais) continue;

                if (mapa[ay][ax] == 'k') { mapa[ay][ax] = ' '; }
                for (i = 0; i < total_monstros; i++) {
                    if (monstros[i].ativo && monstros[i].x == ax && monstros[i].y == ay) {
                        if (monstros[i].tipo == 'Z') {
                            monstros[i].vida_boss--;
                            monstros[i].stun = 1; 
                            if (monstros[i].vida_boss <= 0) { 
                                monstros[i].ativo = 0; 
                                mapa[ay][ax] = monstros[i].sub_celula; 
                                estado_atual = ESTADO_VITORIA; 
                            }
                        } else {
                            monstros[i].ativo = 0; 
                            mapa[ay][ax] = monstros[i].sub_celula; 
                        }
                    }
                }
            }
        }
    }
    /* ARMA 3: Cajado Mágico */
    else if (player.arma == 3) {
        int ox, oy;
        for (ox = -1; ox <= 1; ox++) {
            for (oy = -1; oy <= 1; oy++){
                if (ox == 0 && oy == 0) continue;
                ax = player.x + ox; ay = player.y + oy;

                if (ax < 0 || ax >= colunas_atuais || ay < 0 || ay >= linhas_atuais) continue;

                if (mapa[ay][ax] == 'k') { mapa[ay][ax] = ' '; }
                for (i = 0; i < total_monstros; i++) {
                    if (monstros[i].ativo && monstros[i].x == ax && monstros[i].y == ay) {
                        if (monstros[i].tipo == 'Z') {
                            monstros[i].vida_boss--;
                            monstros[i].stun = 1; 
                            if (monstros[i].vida_boss <= 0) { 
                                monstros[i].ativo = 0; 
                                mapa[ay][ax] = monstros[i].sub_celula; 
                                estado_atual = ESTADO_VITORIA; 
                            }
                        } else {
                            monstros[i].ativo = 0; 
                            mapa[ay][ax] = monstros[i].sub_celula; 
                        }
                    }
                }
            }
        }
    }
}

void interagir(void) {
    int alvo_x = player.x;
    int alvo_y = player.y;
    char opc;

    if (player.simbolo == 'A') alvo_y--;
    else if (player.simbolo == 'V') alvo_y++;
    else if (player.simbolo == '<') alvo_x--;
    else if (player.simbolo == '>') alvo_x++;

    if (alvo_x < 0 || alvo_x >= colunas_atuais || alvo_y < 0 || alvo_y >= linhas_atuais) return;

    if (mapa[alvo_y][alvo_x] == 'N' && estado_atual == ESTADO_VILA) {
        system("cls");
        printf("===================================\n");
        printf("          ANCIAO DA VILA           \n");
        printf("===================================\n");
        printf(" Forasteiro, a profecia dizia que voce viria.\n");
        printf(" Pegue um destes tesouros de nossa linhagem:\n\n");
        printf(" 1. Espada (Ataque amplo 3x2 a frente)\n");
        printf(" 2. Arco e Flecha (Longo alcance retilineo)\n");
        printf(" 3. Cajado Magico (Explosao em 8 direcoes)\n\n");
        printf(" Digite o numero da opcao desejada: ");
        
        do {
            opc = (char)getch();
        } while(opc < '1' || opc > '3');
        
        player.arma = opc - '0';
        system("cls");
        printf("\n Que as divindades guiem seu braco. Va e purifique a masmorra!\n");
        printf("Pressione qualquer tecla para retornar ao mapa...");
        getch();
    }
    
    if (mapa[alvo_y][alvo_x] == 'D') {
        if (player.chaves > 0) {
            player.chaves--;
            mapa[alvo_y][alvo_x] = '='; 
        }
    }
}

void exibir_menu(void) {
    char opc;
    system("cls");
    printf("=======================================\n");
    printf("         DUNGEON CRAWLER C89           \n");
    printf("=======================================\n");
    printf("  1. Jogar\n");
    printf("  2. Tutorial\n");
    printf("  3. Sair / Creditos\n\n");
    printf(" Escolha uma opcao: ");
    
    opc = (char)getch();
    if (opc == '1') {
        estado_atual = ESTADO_VILA;
        carregar_fase(estado_atual);
    } else if (opc == '2') {
        estado_atual = ESTADO_TUTORIAL;
    } else if (opc == '3') {
        exibir_creditos();
        exit(0);
    }
}

void exibir_tutorial(void) {
    system("cls");
    printf("====================================================\n");
    printf("                 TUTORIAL DO JOGO                   \n");
    printf("====================================================\n");
    printf(" HISTORIA:\n Adentre a masmorra, recolha as chaves,\n");
    printf(" destrua os obstaculos e derrote o Boss Final!\n\n");
    printf(" CONTROLES:\n");
    printf("  W, A, S, D - Movimentar e direcionar o personagem\n");
    printf("  I          - Interagir com NPCs e trancas\n");
    printf("  O          - Desferir ataques com a arma equipada\n\n");
    printf(" SIMBOLOS DO MAPA:\n");
    printf("  * : Parede instransponivel     # : Espinho mortal\n");
    printf("  k : Caixa destruivel           @ : Chave de portoes\n");
    printf("  D : Porta trancada             = : Porta aberta\n");
    printf("  X, Y : Monstros perigosos      Z : Boss Final\n");
    printf("====================================================\n");
    printf("Pressione qualquer tecla para retornar ao Menu Principal...");
    getch();
    estado_atual = ESTADO_MENU;
}

void exibir_creditos(void) {
    system("cls");
    printf("===================================\n");
    printf("          CREDITOS DO JOGO         \n");
    printf("===================================\n");
    printf(" Desenvolvedor Principal:\n");
    printf(" Augusto Pereira Ribeiro Ferreira\n\n");
    printf(" Assistencia de IA (Balanceamento/Bugs):\n");
    printf(" Gemini (Google)\n");
    printf("\n Obrigado por jogar!\n");
    printf("===================================\n");
    printf("Pressione qualquer tecla para encerrar...");
    getch();
}
