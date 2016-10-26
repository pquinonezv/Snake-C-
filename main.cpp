#include <windows.h>
#include <conio.h>
#include <stdio.h>

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define ESC 27

int cuerpo[200][2];  //cuerpo de la serpiente posicion 0: x, posicion 1: y
char tecla;  //tecla que se recibe del teclado
int n = 1;
int tam = 15; //tamaño de la serptiente
int x = 10; int y = 12; //posicion actual de la cabeza de la serpiente
int dir = 3; //direccion de la serpiente. 1.arriba,2.abajo,3.derecha,4.izquierda
int xc = 30; //posicion x de la comida
int yc = 15; //posicion y de la comida
int velocidad = 100, h=1;
int score = 0;

//pinta en pantalla con coordenadas utilizando el HANDLE y la estructura COORD
void gotoxy(int x, int y)
{
    HANDLE hCon;
    COORD dwPos;

    dwPos.X = x;
    dwPos.Y = y;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon, dwPos);
}

//pinta las paredes o limites
void pintar()
{
    //pinta las lineas horizontales superior e inferior
    for( int i=2; i<78; i++ )
    {
        gotoxy(i,3); printf("%c", 205);
        gotoxy(i,23); printf("%c", 205);
    }

    //pinta las lineas verticales izquierda y derecha
    for( int i=4; i<23; i++ )
    {
        gotoxy(2,i); printf("%c", 186);
        gotoxy(77,i); printf("%c", 186);
    }

    //pinta las 4 esquinas para darle efecto de esquina
    gotoxy(2,3); printf("%c", 201);
    gotoxy(2,23); printf("%c", 200);
    gotoxy(77,3); printf("%c", 187);
    gotoxy(77,23); printf("%c", 188);
}

//va pintando el cuerpo de la serpiente punto a punto
void guardar_posicion()
{
    cuerpo[n][0] = x;
    cuerpo[n][1] = y;
    n++;
    if( n==tam )
    {
        n = 1;
    }
}

//pinta la serpiente
void pintar_cuerpo()
{
    //hace un ciclo por el tamaño de la serpiente
    for( int i=1; i<tam; i++ )
    {
        gotoxy(cuerpo[i][0], cuerpo[i][1]); //obtiene las coordenadas de la matriz cuerpo
        printf("*");
    }
}

//imprimir puntos
void puntos()
{
    gotoxy(3,1);
    printf("Score: %d", score);
}

//aumenta la velocidad del juego de 10 en 10 cada 20 puntos
void camdiar_velocidad()
{
    if(score==h*20)
    {
        velocidad -= 10;
        h++;
    }
}
//borra una bolita de la serpiente dependiendo del valor de n, dando efecto de caminado
void borrar_cuerpo()
{
    gotoxy(cuerpo[n][0], cuerpo[n][1]); //
    printf(" ");
}

//recibe la tecla presionada por el usuario y asigna una direccion a la serpiente
void teclear()
{
    if(kbhit())
    {
        tecla = getch();
        switch(tecla)
        {
        case ARRIBA:
            if( dir!=2 )//validar que no este yendo hacia abajo porque no se puede ese movimiento
                dir = 1; break;
        case ABAJO:
            if( dir!=1 )//validamos que no este yendo hacia arriba porque no se puede ese movimiento
                dir = 2; break;
        case DERECHA:
            if( dir!=4 ) //validamos que no este yendo hacia izquierda porque no se puede ese movimiento
                dir = 3; break;
        case IZQUIERDA:
            if( dir!=3 ) //validamos que no este yendo hacia derecha porque no se puede ese movimiento
                dir = 4; break;
        }
    }
}

//validamos si serpiente toca comida
void comida()
{
    if(x==xc && y==yc) //si la posicion de la cabeza de la serpiente es igual a la comida
    {
        //redibujamos la comida en una posicion aleatoria dentro de los limites
        xc = (rand()%73)+4; //[rand()%73] nos da valor entre 0 y 72
        yc = (rand()%19)+4; //[rand()%19] nos da valor entre 0 y 18

        tam++;//aumentamos el tamaño de la serpiente
        score+=10;
        gotoxy(xc,yc); printf("%c",4); //colocamos y pintamos la comida

        camdiar_velocidad(); //valida si es necesario aumentar la velocidad del juego
    }
}

//valida si chocamos con las paredes
bool game_over()
{
    if( y==3 || y==23 || x==2 || x==77 ) //si chocamos con el margen de las paredes
    {
        gotoxy(30,13);
        printf("G A M E   O V E R  !!");
        return true;
    }

    //Recorrremos todo el cuerpo de la serpiente
    for(int j=tam-1; j>0; j--)
    {
        //validamos si la posicion de la cabeza de la serpiente no es igual alguna posicion de su cuerpo
        //si es asi entonces choco contra su propio cuerpo y regresamos falso porque se termina el juego
        if( cuerpo[j][0]==x && cuerpo[j][1]==y )
        {
            gotoxy(30,13);
            printf("G A M E   O V E R  !!");
            return true;
        }
    }

    return false;
}

int main()
{
    pintar(); //pintamos la pared
    gotoxy(xc,yc); printf("%c",4); //colocamos y pintamos la comida

    //bucle que sale con la tecla ESC o al validar el game_over
    while( tecla!=ESC && !game_over() )
    {
        borrar_cuerpo(); //borramos el cuerpo de la serpiente para despues redibujarla
        guardar_posicion(); //guardamos la posicion de la serpiente en cada uno de sus bolitas
        pintar_cuerpo(); //pintamos el cuerpo de la serpiente
        comida(); //validamos si serpiente se come comida

        puntos();
        teclear();  //Lee alguna tecla del teclado
        teclear();  //Otra vez lee por si la primera vez no alcanzo a leer

        if( dir==1 ) y--;        //hacia arriba
        if( dir==2 ) y++;       //hacia abajo
        if( dir==3 ) x++;       //hacia derecha
        if( dir==4 ) x--;       //hacia izquierda

        //Ajusta la velocidad del juego
        Sleep(velocidad);
    }

    pintar(); //pintamos la pared de nuevo ya que la serpiente la puede borrar
    return 0;
}
