#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define MAPA_Y 100
#define MAPA_X 25
#define alturaMapa 0.043

int imagemMapa1[MAPA_Y][MAPA_X];
int imagemMapa2[MAPA_Y][MAPA_X];

// JOGADOR
int teclaEsquerda, teclaDireita, teclaCima, teclaAbaixo;
float xJogador, yJogador;

// MAPA
float posicaoMapa1 = 0.0;
float posicaoMapa2 = MAPA_Y * alturaMapa;
float larguraMapa  = 0.056;
float limiteX = 0.60;
float limiteY = 0.84;

// Tema dos mapas
int temaMapa1 = 0;
int temaMapa2 = 1;

//IMAGEM DO CARRO JOGADORORR
int imagemCarro[20][10] = {
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
};

//COR DO CARRO JOGADOR
void desenharCarro(){
     for(int l = 0; l < 20; l++){
        for(int c = 0; c < 10; c++){
              if(imagemCarro[l][c] != 0){
                 if(imagemCarro[l][c] == 1) glColor3ub(0, 0, 0); if(imagemCarro[l][c] == 2) glColor3ub(255,0,0); if(imagemCarro[l][c] == 3) glColor3ub(255,255,255);
            
                 float x = c * 0.011 - 0.045; float y = -l * 0.011;

                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + 0.011, y);
                    glVertex2f(x + 0.011, y + 0.011);
                    glVertex2f(x, y + 0.011);
                glEnd();
            }
        }
    }
}

// MAPAS

void inicializarMapas(int tipoMapa) {
    for(int l = 0; l < MAPA_Y; l++) {
        int borda = (l / 4) % 2 == 0 ? 1 : 2; // condição ? valor_se_verdadeiro : valor_se_falso;
        for(int c = 0; c < MAPA_X; c++) {
            if(tipoMapa == 0){
                if(c == 0 || c == MAPA_X - 1){
                    imagemMapa1[l][c] = borda;
                }
                else {
                    imagemMapa1[l][c] = 0;
                }
            }
            if(tipoMapa == 1){
                if(c == 0 || c == MAPA_X - 1){
                    imagemMapa2[l][c] = borda;
                }
                else if((c == 8 && (l / 4) % 2 == 0) || (c == 16 && (l / 4) % 2 == 0)){
                    imagemMapa2[l][c] = 1;
                }
                else{
                    imagemMapa2[l][c] = 0;
                }
            }
        }   
    }
}

void gerarPedrasAleatorias(){

    for(int l = 4; l < MAPA_Y; l += 4){

        int coluna = rand() % (MAPA_X - 2);

        if(coluna > 1 && coluna < MAPA_X - 3){

            if(imagemMapa2[l][coluna] == 0){

                imagemMapa2[l][coluna] = 3;
                imagemMapa2[l][coluna + 1] = 3;

                imagemMapa2[l + 1][coluna] = 3;
                imagemMapa2[l + 1][coluna + 1] = 3;
            }
        }
    }
}

// Desenho do mapa 1

void desenharMapa1(){

    for(int l = 0; l < MAPA_Y; l++){
        for(int c = 0; c < MAPA_X; c++){
            if(temaMapa1 == 0){
                if(imagemMapa1[l][c] == 0) glColor3ub(60, 60, 60);
                if(imagemMapa1[l][c] == 1) glColor3ub(255, 255, 255);
                if(imagemMapa1[l][c] == 2) glColor3ub(0, 0, 0);
            }
            if(temaMapa1 == 1){
                if(imagemMapa2[l][c] == 0) glColor3ub(160, 160, 160);
                if(imagemMapa2[l][c] == 1) glColor3ub(255, 255, 255);
                if(imagemMapa2[l][c] == 2) glColor3ub(0, 0, 0);
                if(imagemMapa2[l][c] == 3) glColor3ub(72, 72, 72);
                if(imagemMapa2[l][c] == 4) glColor3ub(139, 139, 139);
            }
            float x = -0.7 + c * larguraMapa;
            float y = -1.0 + l * alturaMapa + posicaoMapa1;

            glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + larguraMapa, y);
                glVertex2f(x + larguraMapa, y + alturaMapa);
                glVertex2f(x, y + alturaMapa);
            glEnd();
        }
    }
}

// Desenho do mapa 2

void desenharMapa2(){

    for(int l = 0; l < MAPA_Y; l++){
        for(int c = 0; c < MAPA_X; c++){

            if(temaMapa2 == 0){
                if(imagemMapa1[l][c] == 0) glColor3ub(60, 60, 60);
                if(imagemMapa1[l][c] == 1) glColor3ub(255, 255, 255);
                if(imagemMapa1[l][c] == 2) glColor3ub(0, 0, 0);
            }
            else if(temaMapa2 == 1){
                if(imagemMapa2[l][c] == 0) glColor3ub(160, 160, 160);
                if(imagemMapa2[l][c] == 1) glColor3ub(255, 255, 255);
                if(imagemMapa2[l][c] == 2) glColor3ub(0, 0, 0);
                if(imagemMapa2[l][c] == 3) glColor3ub(72, 72, 72);
                if(imagemMapa2[l][c] == 4) glColor3ub(139, 139, 139);

            }
                float x = -0.7 + c * larguraMapa;
                float y = -1.0 + l * alturaMapa + posicaoMapa2;

                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + larguraMapa, y);
                    glVertex2f(x + larguraMapa, y + alturaMapa);
                    glVertex2f(x, y + alturaMapa);
                glEnd();
        }
    }
}

// Move os mapas

void moverMapa(int valor){
    posicaoMapa1 -= 0.01;
    posicaoMapa2 -= 0.01;
    
    if(posicaoMapa1 <= -(MAPA_Y * alturaMapa)){
        posicaoMapa1 = posicaoMapa2 + MAPA_Y * alturaMapa;
        temaMapa1 = rand() % 2;        
    }
    if(posicaoMapa2 <= -(MAPA_Y * alturaMapa)){
        posicaoMapa2 = posicaoMapa1 + MAPA_Y * alturaMapa;
        temaMapa2 = rand() % 2;
}
    glutTimerFunc(10, moverMapa , 0);
    glutPostRedisplay();
}

// Tecla pressionada

void teclaMovendo(unsigned char key, int a, int b){
    if(key == 'a') teclaEsquerda = 1;
    if(key == 'd') teclaDireita = 1;
    if(key == 'w') teclaCima = 1;
    if(key == 's') teclaAbaixo = 1;
}

// Tecla solta

void teclaSolta(unsigned char key, int a, int b){
    if(key == 'a') teclaEsquerda = 0;
    if(key == 'd') teclaDireita = 0;
    if(key == 'w') teclaCima = 0;
    if(key == 's') teclaAbaixo = 0;
}

// Move jogador

void moverJogador(int valor){
    float velocidadeJogador = 0.0050;
    if(teclaEsquerda) xJogador -= velocidadeJogador;
    if(teclaDireita) xJogador += velocidadeJogador; 
    if(teclaCima) yJogador += velocidadeJogador;
    if(teclaAbaixo) yJogador -= velocidadeJogador;

    if(xJogador > limiteX) xJogador = limiteX;
    if(xJogador < -limiteX) xJogador = -limiteX;
    if(yJogador < -limiteY) yJogador = -limiteY;
    
    glutTimerFunc(2, moverJogador, 0);
    glutPostRedisplay();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    desenharMapa2();
    desenharMapa1();
    glPushMatrix();
    glTranslated(xJogador, yJogador,0);
    desenharCarro();
    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char** argv){
    srand(time(NULL));
    int sorteio = (rand() % 2);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,600);
    glutCreateWindow("Jogo Dois");
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glutKeyboardFunc(teclaMovendo);
    glutKeyboardUpFunc(teclaSolta);
    glutTimerFunc(16, moverJogador, 0);
    inicializarMapas(0);
    inicializarMapas(1);
    gerarPedrasAleatorias();
    glutTimerFunc(16, moverMapa , 0);
    glutDisplayFunc(display);
    glutMainLoop();
}
