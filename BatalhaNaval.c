#include <stdio.h>
#include <stdlib.h>

#define TAM_TAB 10
#define TAM_NAVIO 3
#define TAM_HAB 5

#define AGUA 0
#define NAVIO 3
#define AREA 5

int tabuleiro[TAM_TAB][TAM_TAB];

// Inicializa tabuleiro com água (0)
void inicializaTabuleiro() {
    for(int i=0; i<TAM_TAB; i++) {
        for(int j=0; j<TAM_TAB; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Verifica se navio cabe no tabuleiro e não sobrepõe outros navios
// retorna 1 se válido, 0 se inválido
int podePosicionarNavio(int linha, int coluna, char orientacao, int diagonal, int tamanho) {
    for(int i=0; i<tamanho; i++) {
        int l = linha;
        int c = coluna;

        if(diagonal == 0) { // Horizontal ou vertical
            if(orientacao == 'H' || orientacao == 'h') {
                c = coluna + i;
            } else if(orientacao == 'V' || orientacao == 'v') {
                l = linha + i;
            } else {
                return 0; // orientação inválida
            }
        } else if(diagonal == 1) { // Diagonal principal (linha e coluna aumentam)
            l = linha + i;
            c = coluna + i;
        } else if(diagonal == 2) { // Diagonal secundária (linha aumenta, coluna diminui)
            l = linha + i;
            c = coluna - i;
        }

        if(l < 0 || l >= TAM_TAB || c < 0 || c >= TAM_TAB) {
            return 0; // fora do tabuleiro
        }

        if(tabuleiro[l][c] == NAVIO) {
            return 0; // sobreposição com outro navio
        }
    }
    return 1; // pode posicionar
}

// Posiciona navio no tabuleiro, já validado
void posicionaNavio(int linha, int coluna, char orientacao, int diagonal, int tamanho) {
    for(int i=0; i<tamanho; i++) {
        int l = linha;
        int c = coluna;

        if(diagonal == 0) {
            if(orientacao == 'H' || orientacao == 'h') {
                c = coluna + i;
            } else {
                l = linha + i;
            }
        } else if(diagonal == 1) {
            l = linha + i;
            c = coluna + i;
        } else if(diagonal == 2) {
            l = linha + i;
            c = coluna - i;
        }
        tabuleiro[l][c] = NAVIO;
    }
}

// Gera matriz cone 5x5 (forma de cone para baixo)
void geraMatrizCone(int cone[TAM_HAB][TAM_HAB]) {
    int centro = TAM_HAB / 2;
    for(int i=0; i<TAM_HAB; i++) {
        for(int j=0; j<TAM_HAB; j++) {
            if(j >= centro - i && j <= centro + i)
                cone[i][j] = 1;
            else
                cone[i][j] = 0;
        }
    }
}

// Gera matriz cruz 5x5
void geraMatrizCruz(int cruz[TAM_HAB][TAM_HAB]) {
    int centro = TAM_HAB / 2;
    for(int i=0; i<TAM_HAB; i++) {
        for(int j=0; j<TAM_HAB; j++) {
            cruz[i][j] = (i == centro || j == centro) ? 1 : 0;
        }
    }
}

// Gera matriz octaedro (losango) 5x5
void geraMatrizOctaedro(int octaedro[TAM_HAB][TAM_HAB]) {
    int centro = TAM_HAB / 2;
    for(int i=0; i<TAM_HAB; i++) {
        for(int j=0; j<TAM_HAB; j++) {
            octaedro[i][j] = (abs(i - centro) + abs(j - centro) <= centro) ? 1 : 0;
        }
    }
}

// Sobrepõe habilidade no tabuleiro (marca com 5)
void sobrepoeHabilidade(int habilidade[TAM_HAB][TAM_HAB], int linOrig, int colOrig) {
    int meio = TAM_HAB / 2;
    for(int i=0; i<TAM_HAB; i++) {
        for(int j=0; j<TAM_HAB; j++) {
            if(habilidade[i][j] == 1) {
                int linTab = linOrig - meio + i;
                int colTab = colOrig - meio + j;
                if(linTab >=0 && linTab < TAM_TAB && colTab >=0 && colTab < TAM_TAB) {
                    if(tabuleiro[linTab][colTab] != NAVIO) {
                        tabuleiro[linTab][colTab] = AREA;
                    }
                }
            }
        }
    }
}

// Exibe o tabuleiro no console
void exibeTabuleiro() {
    printf("\nTabuleiro:\n");
    for(int i=0; i<TAM_TAB; i++) {
        for(int j=0; j<TAM_TAB; j++) {
            if(tabuleiro[i][j] == AGUA)
                printf("0 ");
            else if(tabuleiro[i][j] == NAVIO)
                printf("3 ");
            else if(tabuleiro[i][j] == AREA)
                printf("5 ");
        }
        printf("\n");
    }
}

int main() {
    inicializaTabuleiro();

    int linha, coluna;
    char orientacao;
    int diagonal;

    printf("Posicione 2 navios (horizontal ou vertical):\n");
    for(int nav=1; nav<=2; nav++) {
        do {
            printf("Navio %d - Linha inicial (0-%d): ", nav, TAM_TAB-1);
            scanf("%d", &linha);
            printf("Navio %d - Coluna inicial (0-%d): ", nav, TAM_TAB-1);
            scanf("%d", &coluna);
            printf("Navio %d - Orientacao (H-horizontal / V-vertical): ", nav);
            scanf(" %c", &orientacao);
            diagonal = 0; // Não é diagonal

            if(!podePosicionarNavio(linha, coluna, orientacao, diagonal, TAM_NAVIO)) {
                printf("Posicao invalida ou sobreposicao! Tente novamente.\n");
            } else {
                posicionaNavio(linha, coluna, orientacao, diagonal, TAM_NAVIO);
                break;
            }
        } while(1);
    }

    printf("\nPosicione 2 navios na diagonal:\n");
    for(int nav=3; nav<=4; nav++) {
        do {
            printf("Navio %d - Linha inicial (0-%d): ", nav, TAM_TAB-1);
            scanf("%d", &linha);
            printf("Navio %d - Coluna inicial (0-%d): ", nav, TAM_TAB-1);
            scanf("%d", &coluna);
            printf("Navio %d - Tipo diagonal (1-diagonal principal / 2-diagonal secundaria): ", nav);
            scanf("%d", &diagonal);

            if(!podePosicionarNavio(linha, coluna, 'H', diagonal, TAM_NAVIO)) {
                printf("Posicao invalida ou sobreposicao! Tente novamente.\n");
            } else {
                posicionaNavio(linha, coluna, 'H', diagonal, TAM_NAVIO);
                break;
            }
        } while(1);
    }

    // Cria matrizes das habilidades
    int cone[TAM_HAB][TAM_HAB];
    int cruz[TAM_HAB][TAM_HAB];
    int octaedro[TAM_HAB][TAM_HAB];

    geraMatrizCone(cone);
    geraMatrizCruz(cruz);
    geraMatrizOctaedro(octaedro);

    // Ponto de origem das habilidades (lê do usuário)
    int origemConeLinha, origemConeColuna;
    int origemCruzLinha, origemCruzColuna;
    int origemOctaedroLinha, origemOctaedroColuna;

    printf("\nDefina origem da habilidade CONE (linha e coluna 0-%d): ", TAM_TAB-1);
    scanf("%d %d", &origemConeLinha, &origemConeColuna);

    printf("Defina origem da habilidade CRUZ (linha e coluna 0-%d): ", TAM_TAB-1);
    scanf("%d %d", &origemCruzLinha, &origemCruzColuna);

    printf("Defina origem da habilidade OCTAEDRO (linha e coluna 0-%d): ", TAM_TAB-1);
    scanf("%d %d", &origemOctaedroLinha, &origemOctaedroColuna);

    // Sobrepõe as habilidades
    sobrepoeHabilidade(cone, origemConeLinha, origemConeColuna);
    sobrepoeHabilidade(cruz, origemCruzLinha, origemCruzColuna);
    sobrepoeHabilidade(octaedro, origemOctaedroLinha, origemOctaedroColuna);

    // Exibe tabuleiro final
    exibeTabuleiro();

    return 0;
}
