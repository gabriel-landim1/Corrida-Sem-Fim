#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Controlam o movimento do jogador
int moveEsq, moveDir, moveCima, moveBaixo;

// Posição atual do jogador na tela
float xPOS, yPOS;

// Vetor usado para armazenar textos, exemplo: pontuação, game over, scoreboard
char texto[100];

// Quantidade de vidas do jogador
int vida = 3;

// Pontuação atual do jogador
int pontos = 0;

// Controla se o jogador está colidindo com algum obstáculo, isso evita perder várias vidas ao mesmo tempo
int emColisao = 0;

// Guarda o nome digitado pelo jogador
char nomeJogador[50];

// Impede salvar o score várias vezes seguidas
int podeSalvar = 0;

// Vetor que guarda as pontuações dos jogadores
int pontuacoes[100];

// Matriz que guarda os nomes dos jogadores
char nomesJogadores[100][50];

// Quantidade de jogadores lidos do arquivo
int quantidadeJogadores = 0;

// === POSIÇÃO DOS OBSTÁCULOS ===

// Obstáculo amarelo
float obstaculo1X = -0.46;
float obstaculo1Y = -1.5;

// Obstáculo verde
float obstaculo2Y = 1.0;
float obstaculo2X = -0.11;

// Obstáculo azul
float obstaculo3Y = -1.0;
float obstaculo3X = 0.20;

// === CONFIGURAÇÕES DO MAPA ====
float tamanhoX = 0.056;
float tamanhoY = 0.08;
float limiteX = 0.6;
float limiteY = 0.84;

// === SPRITE: JOGADOR (CARRO) ===
int carrinho[20][10] = {
    {0, 0, 0, 2, 2, 2, 2, 0, 0, 0},
    {0, 0, 2, 2, 2, 2, 2, 2, 0, 0},
    {0, 2, 3, 3, 2, 2, 3, 3, 2, 0},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 2, 1, 1, 1, 1, 2, 2, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 2, 1},
    {0, 2, 1, 1, 1, 1, 1, 1, 2, 0},
    {0, 2, 1, 3, 2, 2, 3, 1, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {1, 2, 2, 3, 2, 2, 3, 2, 2, 1},
    {1, 2, 2, 1, 1, 1, 1, 2, 2, 1},
    {1, 2, 2, 2, 1, 1, 2, 2, 2, 1},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {0, 0, 0, 2, 2, 2, 2, 0, 0, 0},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
};

// === RENDERIZA O JOGADOR NA TELA (CARRO) ===
void desenharCarrinho(){
     for(int l = 0; l < 20; l++){           // percorre linhas da matriz
        for(int c = 0; c < 10; c++){        // percorre colunas da matriz

            // só desenha se não for vazio
            if(carrinho[l][c] != 0){

                // define cor do pixel
                if(carrinho[l][c] == 1) glColor3ub(0, 0, 0); 
                if(carrinho[l][c] == 2) glColor3ub(255,0,0); 
                if(carrinho[l][c] == 3) glColor3ub(255,255,255);
            
                // converte matriz para coordenada na tela
                float x = c * 0.0090 - 0.045; 
                float y = -l * 0.0090 - 0.011;

                // desenha um quadrado (pixel)
                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + 0.0090, y);
                    glVertex2f(x + 0.0090, y + 0.0090);
                    glVertex2f(x, y + 0.0090);
                glEnd();
            }
        }
    }
}

// SPRITE: CARRO OBSTÁCULO 1 (AMARELO)
int obstaculoAmarelo[20][10] = {
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {1, 2, 2, 2, 1, 1, 2, 2, 2, 1},
    {1, 2, 2, 1, 1, 1, 1, 2, 2, 1},
    {1, 2, 2, 3, 2, 2, 3, 2, 2, 1},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 1, 3, 2, 2, 3, 1, 2, 0},
    {0, 2, 1, 1, 1, 1, 1, 1, 2, 0},
    {1, 2, 1, 1, 1, 1, 1, 1, 2, 1},
    {1, 2, 2, 1, 1, 1, 1, 2, 2, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {0, 2, 3, 3, 2, 2, 3, 3, 2, 0},
    {0, 0, 2, 2, 2, 2, 2, 2, 0, 0},
    {0, 0, 0, 2, 2, 2, 2, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// === RENDERIZA O CARRO AMARELO NA TELA ===
void desenharCarroAmarelo(){
     for(int l = 0; l < 20; l++){           // percorre linhas da matriz
        for(int c = 0; c < 10; c++){        // percorre colunas da matriz

            // só desenha se não for 0
            if(obstaculoAmarelo[l][c] != 0){

                // define cor do pixel
                if(obstaculoAmarelo[l][c] == 1) glColor3ub(0, 0, 0); 
                if(obstaculoAmarelo[l][c] == 2) glColor3ub(255,255,0); 
                if(obstaculoAmarelo[l][c] == 3) glColor3ub(255,255,255);
            
                // converte matriz para coordenada na tela
                float x = c * 0.0090 - 0.09; 
                float y = -l * 0.0090 - 0.011;

                // desenha um quadrado (pixel)
                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + 0.0090, y);
                    glVertex2f(x + 0.0090, y + 0.0090);
                    glVertex2f(x, y + 0.0090);
                glEnd();
            }
        }
    }
}

// === LÓGICA DE MOVIMENTO DO OBSTÁCULO AMARELO ===
void moverObstaculoAmarelo(int value){

    // Move o obstáculo para baixo a cada atualização
    obstaculo1Y -= 0.01;

    // Se o obstáculo sair da tela
    if(obstaculo1Y < -1.1){

        // Reseta posição vertical para o topo da tela
        obstaculo1Y = 1.5;
    }
    glutPostRedisplay();
    glutTimerFunc(10,moverObstaculoAmarelo,0);
}

// SPRITE: CARRO OBSTÁCULO 2 (VERDE)
int obstaculoVerde[20][10] = {
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {1, 2, 2, 2, 1, 1, 2, 2, 2, 1},
    {1, 2, 2, 1, 1, 1, 1, 2, 2, 1},
    {1, 2, 2, 3, 2, 2, 3, 2, 2, 1},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 1, 3, 2, 2, 3, 1, 2, 0},
    {0, 2, 1, 1, 1, 1, 1, 1, 2, 0},
    {1, 2, 1, 1, 1, 1, 1, 1, 2, 1},
    {1, 2, 2, 1, 1, 1, 1, 2, 2, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {0, 2, 3, 3, 2, 2, 3, 3, 2, 0},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {0, 0, 2, 2, 2, 2, 2, 2, 0, 0},
    {0, 0, 0, 2, 2, 2, 2, 0, 0, 0},
};

// === RENDERIZA O CARRO VERDE NA TELA ===
void desenharCarroVerde(){
     for(int l = 0; l < 20; l++){           // percorre linhas da matriz
        for(int c = 0; c < 10; c++){        // percorre colunas da matriz

            // só desenha se não for 0
            if(obstaculoVerde[l][c] != 0){

                // define cor do pixel
                if(obstaculoVerde[l][c] == 1) glColor3ub(0, 0, 0); 
                if(obstaculoVerde[l][c] == 2) glColor3ub(0,255,0); 
                if(obstaculoVerde[l][c] == 3) glColor3ub(255,255,255);
            
                // converte matriz para coordenada na tela
                float x = c * 0.0090 - 0.09; 
                float y = -l * 0.0090 - 0.011;

                // desenha um quadrado (pixel)
                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + 0.0090, y);
                    glVertex2f(x + 0.0090, y + 0.0090);
                    glVertex2f(x, y + 0.0090);
                glEnd();
            }
        }
    }
}

// === LÓGICA DE MOVIMENTO DO OBSTÁCULO VERDE ===
void moverObstaculoVerde(int value){
    
    // Move o obstáculo para baixo a cada atualização
    obstaculo2Y -= 0.01;

    // Se o obstáculo sair da tela
    if(obstaculo2Y < -1.1){

        // Reseta posição vertical para o topo da tela
        obstaculo2Y = 1.0;
    }
    glutPostRedisplay();
    glutTimerFunc(10,moverObstaculoVerde,0);
}

// SPRITE: CARRO OBSTÁCULO 3 (AZUL)
int obstaculoAzul[20][10] = {
    {0, 0, 0, 2, 2, 2, 2, 0, 0, 0},
    {0, 0, 2, 2, 2, 2, 2, 2, 0, 0},
    {0, 2, 3, 3, 2, 2, 3, 3, 2, 0},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 2, 1, 1, 1, 1, 2, 2, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 2, 1},
    {0, 2, 1, 1, 1, 1, 1, 1, 2, 0},
    {0, 2, 1, 3, 2, 2, 3, 1, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {0, 2, 2, 3, 2, 2, 3, 2, 2, 0},
    {1, 2, 2, 3, 2, 2, 3, 2, 2, 1},
    {1, 2, 2, 1, 1, 1, 1, 2, 2, 1},
    {1, 2, 2, 2, 1, 1, 2, 2, 2, 1},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 0},
};

// === RENDERIZA O CARRO AZUL NA TELA ===
void desenharCarroAzul(){
     for(int l = 0; l < 20; l++){           // percorre linhas da matriz
        for(int c = 0; c < 10; c++){        // percorre colunas da matriz

            // só desenha se não for 0
            if(obstaculoAzul[l][c] != 0){

                // define cor do pixel
                if(obstaculoAzul[l][c] == 1) glColor3ub(0, 0, 0); 
                if(obstaculoAzul[l][c] == 2) glColor3ub(0,255,255); 
                if(obstaculoAzul[l][c] == 3) glColor3ub(255,255,255);
            
                // converte matriz para coordenada na tela
                float x = c * 0.0090 - 0.09; 
                float y = -l * 0.0090 - 0.011;

                // desenha um quadrado (pixel)
                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + 0.0090, y);
                    glVertex2f(x + 0.0090, y + 0.0090);
                    glVertex2f(x, y + 0.0090);
                glEnd();
            }
        }
    }
}

// === LÓGICA DE MOVIMENTO DO OBSTÁCULO AZUL ===
void moverObstaculoAzul(int value){
    
    // Move o obstáculo para cima a cada atualização
    obstaculo3Y += 0.01;

    // Se o obstáculo sair da tela
    if(obstaculo3Y > 1.1){

        // Reseta posição vertical para o baixo da tela
        obstaculo3Y = -1.0;
    }
    glutPostRedisplay();
    glutTimerFunc(8,moverObstaculoAzul,0);
}

// === MAPA DA PISTA ===
int mapa[25][25] = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
};

// === RENDERIZA O MAPA NA TELA ===
void desenharMapa(){
    for(int l = 0; l < 25; l++){            // percorre linhas da matriz
        for(int c = 0; c  < 25; c++){       // percorre colunas da matriz

            // define cor do pixel
            if(mapa[l][c] == 0) glColor3ub(128, 128, 128); 
            if(mapa[l][c] == 1) glColor3ub(255, 255, 255); 
            if(mapa[l][c] == 3) glColor3ub(255, 0, 0);

            // converte matriz para coordenada na tela
            float x = -0.7 + c * tamanhoX; 
            float y = -1.0 + l * tamanhoY;

            // desenha um quadrado (pixel)
            glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + tamanhoX, y);
                glVertex2f(x + tamanhoX, y + tamanhoY);
                glVertex2f(x, y + tamanhoY);
            glEnd();
        }
    }
}

// === Quando tecla é pressionada ===
void teclaMovendo(unsigned char key, int a, int b){

    // Movimento contínuo enquanto tecla estiver pressionada
    if(key == 'a') moveEsq = 1;          // Esquerda
    if(key == 'd') moveDir = 1;          // Direita
    if(key == 'w') moveCima = 1;         // Cima
    if(key == 's') moveBaixo = 1;        // Baixo

    // Reset do jogo quando pressionar R (apenas se morreu)
    if(key == 'r' && vida <= 0){

        // Libera o salvamento de score novamente (para próxima partida)
        podeSalvar = 0;

        // Reinicia status do jogador
        vida = 3;       // volta com 3 vidas
        pontos = 0;     // zera a pontuação

        // Volta o jogador para posição inicial
        xPOS = 0.0;
        yPOS = 0.0;

        // Reseta obstáculos para posição inicial do jogo
        obstaculo1X = -0.46;
        obstaculo1Y = -1.5;

        obstaculo2X = -0.11;
        obstaculo2Y = 1.0;

        obstaculo3X = 0.20;
        obstaculo3Y = -1.0;

        // Restaura tamanho do mapa
        tamanhoX =  0.056;
    }
}

// === TECLAS (KEY UP) - PARAR MOVIMENTO ===
void teclaSolta(unsigned char key, int a, int b){

    // Quando solta a tecla, para o movimento correspondente
    if(key == 'a') moveEsq = 0;
    if(key == 'd') moveDir = 0;
    if(key == 'w') moveCima = 0;
    if(key == 's') moveBaixo = 0;
}

// === MOVIMENTO DO JOGADOR ===
void moverJogador(int valor){

    // Velocidade do movimento do jogador
    float velocidadeCaminhar = 0.0050;

    // Movimento baseado nas teclas pressionadas
    if(moveEsq) xPOS -= velocidadeCaminhar;
    if(moveDir) xPOS += velocidadeCaminhar; 
    if(moveCima) yPOS += velocidadeCaminhar;
    if(moveBaixo) yPOS -= velocidadeCaminhar;

    // Limite do mapa (Impede sair da tela)
    if(xPOS > limiteX) xPOS = limiteX;
    if(xPOS < -limiteX) xPOS = -limiteX;
    if(yPOS < -limiteY) yPOS = -limiteY;
    if(yPOS > limiteY) yPOS = limiteY;

    // Se o jogador perdeu todas as vidas (game over)
    if(vida == 0){

        // Reduz o tamanho do mapa
        tamanhoX = 0;

        // Move o jogador para fora da tela (lado direito)
        xPOS = 5.0;

        // Move os obstáculos para fora da tela (lado esquerdo)
        obstaculo1X = -5.0;
        obstaculo2X = -5.0;
        obstaculo3X = -5.0;
    }
    glutTimerFunc(5, moverJogador, 0);
    glutPostRedisplay();
}

// === DETECÇÃO DE COLISÃO DO JOGADOR COM OBSTÁCULOS ===
void verificarColisao(){

    // distância entre jogador e obstáculo 1 - Amarelo
    float distXObst1 = xPOS - obstaculo1X;
    float distYObst1 = yPOS - obstaculo1Y;

    // distância entre jogador e obstáculo 2 - Verde
    float distXObst2 = xPOS - obstaculo2X;
    float distYObst2 = yPOS - obstaculo2Y;

    // distância entre jogador e obstáculo 3 - Azul
    float distXObst3 = xPOS - obstaculo3X;
    float distYObst3 = yPOS - obstaculo3Y;

    // === colisão com obstáculo 1 ===
    if(distXObst1 < 0.09 && distXObst1 > -0.09 && distYObst1 < 0.18 && distYObst1 > -0.18){

        // evita descontar vida várias vezes seguidas
        if(emColisao == 0){
            vida = vida - 1;        // perde uma vida
            emColisao = 1;          // ativa estado de colisão
            obstaculo1Y = 2.2;      // reseta posição do obstáculo
        } 
    else {
        // libera colisão quando sai do obstáculo
        emColisao = 0;
    }
}

    // === colisão com obstáculo 2 ===
    if(distXObst2 < 0.09 && distXObst2 > -0.09 && distYObst2 < 0.18 && distYObst2 > -0.18){

        if(emColisao == 0){
            vida = vida - 1;
            emColisao = 1;
            obstaculo2Y = 2.2;
        }
    } 
    else {
        emColisao = 0;
    }

    // === colisão com obstáculo 3 ===
    if(distXObst3 < 0.09 && distXObst3 > -0.09 && distYObst3 < 0.18 && distYObst3 > -0.18){

        if(emColisao == 0){
            vida = vida - 1;
            emColisao = 1;
            obstaculo3Y = -2.2;
        }
    } 
    else {
        emColisao = 0;
    }
}

void carregarScore(){

    quantidadeJogadores = 0;

    // Abre o arquivo de scoreboard em modo leitura ("r")
    FILE *arquivo = fopen("Scoreboard.txt", "r");

    // Verifica se o arquivo foi aberto corretamente
    if(arquivo != NULL){

        // Lê nome e pontuação do arquivo e passa para o próximo jogador
        while(fscanf(arquivo, "%s %d", nomesJogadores[quantidadeJogadores], &pontuacoes[quantidadeJogadores]) == 2 && quantidadeJogadores < 100){
            quantidadeJogadores++;  // passa para o próximo jogador
        }

        // Fecha o arquivo após a leitura
        fclose(arquivo);
    }

}

void salvarScore(){
    carregarScore();

    // Variável de controle: começa como 0 (não encontrado)
    int encontrado = 0;

    // Percorre todos os jogadores já salvos procurando o nome do jogador atual
    for(int i = 0; i < quantidadeJogadores; i++){

        // Compara o nome salvo com o nome do jogador atual, 'strcmp' retorna 0 quando os dois nomes são iguais
        if(strcmp(nomesJogadores[i], nomeJogador) == 0){

            // Jogador já existe, só atualiza se a pontuação nova for maior que a anterior
            if(pontos > pontuacoes[i]){
                pontuacoes[i] = pontos;  // substitui pela pontuação maior
            }

            encontrado = 1;  // marca que o jogador foi encontrado
        }
    }

    // Se o jogador não foi encontrado, é a primeira vez que ele joga
    if(encontrado == 0){

        // Adiciona o nome e pontuação na próxima posição livre do array
        strcpy(nomesJogadores[quantidadeJogadores], nomeJogador);
        pontuacoes[quantidadeJogadores] = pontos;

        // Aumenta o contador pois agora tem mais um jogador
        quantidadeJogadores++;
    }

    // Abre o arquivo em modo escrita ("w")
    FILE *arquivo = fopen("Scoreboard.txt", "w");

    if(arquivo != NULL){

        // Reescreve todos os jogadores no arquivo, agora com os dados atualizados
        for(int i = 0; i < quantidadeJogadores; i++){
            fprintf(arquivo, "%s %d\n", nomesJogadores[i], pontuacoes[i]);
        }

        // Fecha o arquivo
        fclose(arquivo);
    }
}

// === MOSTRAR A PONTUAÇÃO DOS JOGADORES NA TELA ===
void mostrarScoreboard(){
    carregarScore();

    // Posição Y inicial do texto na tela
    float posicaoTextoY = 0.27;

    // Variável temporária usada na troca das pontuações
    int pontoTemporario;

    // Variável temporária usada na troca dos nomes
    char nomeTemporario[50];

    // ===== ORDENAÇÃO DO SCORE =====
    // Bubble Sort (maior pontuação primeiro)

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < (quantidadeJogadores - 1); j++){

            // Se a pontuação atual for menor que a próxima
            if(pontuacoes[j] < pontuacoes[j + 1]){

                // Troca as pontuações
                pontoTemporario = pontuacoes[j];
                pontuacoes[j] = pontuacoes[j + 1];
                pontuacoes[j + 1] = pontoTemporario;

                // Troca os nomes junto com as pontuações
                strcpy(nomeTemporario, nomesJogadores[j]);
                strcpy(nomesJogadores[j], nomesJogadores[j + 1]);
                strcpy(nomesJogadores[j + 1], nomeTemporario);

            }
        }
    }

    // === TÍTULO ===
    glColor3ub(255, 150, 0);
    glRasterPos2f(-0.28, 0.45);
    sprintf(texto, "=== SCOREBOARD ===");
    
    // Desenha letra por letra
    for(int i = 0; texto[i] != '\0'; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }

    // === PONTUAÇÃO DO JOGADOR ===
    glColor3ub(255, 255, 255);
    glRasterPos2f(-0.25, 0.35);
    sprintf(texto, "Sua pontuacao: %d pts", pontos);
    for(int i = 0; texto[i] != '\0'; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }

    // === MOSTRAR TOP 5 MELHORES ===
    for(int i = 0; i < quantidadeJogadores && i < 5; i++){
        glRasterPos2f(-0.12, posicaoTextoY);
        sprintf(texto, "%s - %d", nomesJogadores[i], pontuacoes[i]);

        for(int j = 0; texto[j] != '\0'; j++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[j]);
        }

        // Desce a posição do próximo texto
        posicaoTextoY -= 0.08;
    }

    // === INSTRUÇÃO PARA REINICIAR ===
    glColor3ub(255, 255, 255);
    glRasterPos2f(-0.35, -0.30);
    sprintf(texto, "Pressione R para jogar novamente");
    for(int i = 0; texto[i] != '\0'; i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }
}

// === SISTEMA DE PONTUAÇÃO ===
void atualizarPontos(int valor){

    // Se vida maior que 0
    if(vida > 0){
        pontos++;    // aumenta enquanto vivo
    }

    // Chame essa função a cada 100 milissegundo
    glutTimerFunc(100, atualizarPontos, 0);
}

// === PLACAR DE PONTOS ===
void placarPontos(){

    // Se o jogador morreu
    if(vida == 0){
        glRasterPos2f(5.0, 5.0);    // Manda para fora da tela o texto
    }
    else{
        glColor3ub(255, 255, 255);
        glRasterPos2f(0.75, 0.8);
        sprintf(texto, "Pontos: %d", pontos);
        for(int i = 0; texto[i] != '\0'; i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
        }
    }
}

// === PLACAR DE VIDA ===
void placarVida(){

    // Se o jogador morreu
    if(vida == 0){
        glRasterPos2f(5.0, 5.0);    // Manda para fora da tela o texto
    }
    else{
        glColor3ub(255, 255, 255);
        glRasterPos2f(-0.93, 0.8);
        sprintf(texto, "Vidas: %d", vida);
        for(int i = 0; texto[i] != '\0'; i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
        }   
    }
}

// === TELA DE GAME OVER ===
void gameOver(){

    // Só mostra scoreboard quando o jogador morre
    if(vida == 0){
        glColor3ub(25, 25, 25);
        glBegin(GL_QUADS);
            glVertex2f(-0.50, -0.45);
            glVertex2f(0.50, -0.45);
            glVertex2f(0.50, 0.65);
            glVertex2f(-0.50, 0.65);
        glEnd();


        // Borda
        glColor3ub(255, 220, 0);
        glBegin(GL_LINE_LOOP);
            glVertex2f(-0.50, -0.45);
            glVertex2f(0.50, -0.45);
            glVertex2f(0.50, 0.65);
            glVertex2f(-0.50, 0.65);
        glEnd();
        mostrarScoreboard();
    }
}

// === Função principal de desenho — chamada a cada frame ===
void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    // Salva o score apenas uma vez quando o jogador morre
    if(vida == 0 && podeSalvar == 0){

        // Salva o nome do jogador e a pontuação no arquivo
        salvarScore();

        // Marca que o score já foi salvo para evitar salvar várias vezes, isso impede que o display() salve o score repetidamente a cada frame
        podeSalvar = 1;
    }
    desenharMapa();

    // === JOGADOR ===
    glPushMatrix();
    glTranslated(xPOS, yPOS,0);
    desenharCarrinho();
    glPopMatrix();

    // === OBSTÁCULO 1 ===
    glPushMatrix();
    glTranslatef(obstaculo1X,obstaculo1Y,0);
    desenharCarroAmarelo();
    glPopMatrix();

    // === OBSTÁCULO 2 ===
    glPushMatrix();
    glTranslatef(obstaculo2X,obstaculo2Y,0);
    desenharCarroVerde();
    glPopMatrix();

    // === OBSTÁCULO 3 ===
    glPushMatrix();
    glTranslatef(obstaculo3X,obstaculo3Y,0);
    desenharCarroAzul();
    glPopMatrix();

    // === LÓGICA DO JOGO ===
    verificarColisao();
    gameOver();

    // === HUD ===
    placarVida();
    placarPontos();
    glFlush();
}

// === INICIALIZAÇÂO DO JOGO ===
int main(int argc, char** argv){

    // Mensagem inicial no terminal
    printf("=======================\n");
    printf(" BEM-VINDO AO JOGO!\n");
    printf("=======================\n");

    // Pede o nome do jogador e guarda na variável global
    printf("Digite seu nome: ");
    scanf("%s", nomeJogador);
    printf("Boa sorte, %s!\n", nomeJogador);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,600);
    glutCreateWindow("Jogo Dois");
    glClearColor(0.08, 0.08, 0.08, 1.0);

    // Funções de controle do teclado
    glutKeyboardFunc(teclaMovendo);     // tecla pressionada
    glutKeyboardUpFunc(teclaSolta);     // tecla solta

    glutTimerFunc(16, moverJogador, 0);             // movimento do jogador
    glutTimerFunc(16,moverObstaculoAzul,0);         // movimento obstáculo azul
    glutTimerFunc(16,moverObstaculoVerde,0);        // movimento obstáculo verde
    glutTimerFunc(16,moverObstaculoAmarelo,0);      // movimento obstáculo amarelo

    glutTimerFunc(100, atualizarPontos, 0);         // sistema de pontuação
    glutDisplayFunc(display);
    glutMainLoop();
}
