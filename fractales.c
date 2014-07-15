
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_primitives.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>
#include "estructuras.h"
#include "globals.h"
#include "botonera.h"
#include "fractales.h"

// constantes del programa
const double LARGOARTE=50;
const int LARGOFRACTAL=300;
const int CANTICOLORS=30;
const int SCREENW = 400; 
const int SCREENH = 576;

//default values
static double tol=1;// default tolerancia 1
static int order=6; // default orden 6
static unsigned char red=0, green=0, blue=0; //default values color de fondo

//vars para koch fractal
static double x_1; 
static double y_1;
static int c=0;//para variaciones artísticas
static bool variaciones=false;
static bool masvariaciones=false;
static ALLEGRO_FONT *japan=NULL;
static bool kbz=false;
const int CANTICIRC=30;

ALLEGRO_BITMAP *back=NULL;//fondo para celular
ALLEGRO_BITMAP *back1=NULL;
ALLEGRO_BITMAP *back2=NULL;
// colors
ALLEGRO_COLOR colors[10];


//función que dibuja un fractal de koch del largo, orden n, tolerancia ingresados y centrado en (x,y)
void koch(double largo, int n, double tolerancia, double x, double y)
{
    x_1=x-largo/2.0; //calculo las coordendas del punto extremo izquierdo del triángulo
    y_1=y-largo*0.2547627247;// partiendo del centro ingresado por parámetros
    lineafractal(largo,0.0 ,n,tolerancia); // dibujo el lado izquierdo del copo
    lineafractal(largo, 120.0 ,n, tolerancia); // dibujo el lado derecho
    lineafractal(largo, 240.0 ,n, tolerancia); // finalmente el lado inferior
}

void lineafractal(double largo, double ang, int n, double tolerancia)
{
    if(largo<tolerancia){ //si el largo de un lado es menor a la tolerancia, aviso y salgo con error
        printf("no se puede dibujar el orden pedido con esa tolerancia\n");
        exit(1);
    }
    if(n==0) polares(largo,ang); //si el orden es 0 dibujo una linea, partiendo del último punto hasta donde dibujé, con cierto largo y un ángulo ang
    else {
        lineafractal(largo/3,ang,n-1, tolerancia); // dibujo lineas de largo igual a un tercio del largo anterior
        lineafractal(largo/3,ang-60,n-1, tolerancia); // variando el angulo en 60 grados(triángulos equiláteros)
        lineafractal(largo/3,ang+60,n-1, tolerancia);
        lineafractal(largo/3,ang,n-1,tolerancia); // vuelvo al punto inicial
    }
        
}

// función que recibe un radio y un angulo y dibuja una línea partiendo del último punto dibujado con un ángulo ang desde el eje x y un radio
void polares(double radio, double ang)
{
    double radianes;
    double x2, y2;
    radianes=ang*3.1415926535/180; // paso a radianes 
    x2=x_1; // las coordenadas de destino (x2, y2)
    y2=y_1;// se consiguen sumandole a las anteriores (x_1,y_1) el vector que forma la distancia (ingresada por cierto radio y angulo)
    x2+=(radio*cos(radianes));// opero con polares
    y2+=(radio*sin(radianes));
    if(variaciones || masvariaciones){ // si las variaciones artísticas estan activadas
        al_draw_line(x_1,fabs(y_1-SCREENH),x2,fabs(y2-SCREENH),colors[c],16); //dibujo tres líneas de colores random 
        al_draw_line(x_1,fabs(y_1-SCREENH),x2,fabs(y2-SCREENH),colors[c+1],7); // para dar mayor expresión gráfica
        al_draw_line(x_1,fabs(y_1-SCREENH),x2,fabs(y2-SCREENH),colors[c+2],1);
        c+=5;// cambio de colores
        if(c>=CANTICOLORS) // si ya recorrí todos los colores, vuelvo a empezar
            c=0;
    }
    else
        al_draw_line(x_1,fabs(y_1-SCREENH),x2,fabs(y2-SCREENH),al_map_rgb(255,255,255),1); // dibujo una línea de (x_1,y_1) a (x2,y2)
    x_1=x2; // mis coordenadas nuevas pasan a ser las coordenadas viejas
    y_1=y2;
}

//función que realiza variaciones artísticas
void art(int *a, int *b, double *largo){
    int i=*a, j=*b, p;
    double  l=*largo;
    if(j<4){
        if(i<7){
            koch(l*(i+1),i,0.1,SCREENW/2,SCREENH/2); // fractal que cambia de orden y tamaño
                i++;
            }
        else { // reinicio i, aumento j
            i=0; 
            j++;
            l-=10;
            if(l<60)
                l=LARGOARTE;
            } 
        }
    else {// reinicio variables
        j=0;
        i=0;
    }
    *a=i;
    *b=j;
    *largo=l;
}

//función que realiza más variaciones artísticas
void morevariations(int *a){
    int i=*a; static int p=15;
    if(i<order){ // dibujo fractales cada vez más chicos con colores random
        koch(LARGOFRACTAL-p,6,0.1,SCREENW/2,SCREENH/2);
        i++;
        p+=15;
        }
        else {
            i=0;
        }
    if(p>(LARGOFRACTAL-100)) // para no llegar al límite de tolerancia
        p=5;
    *a=i;
}

// función que dibuja el copo aumentando su orden progresivamente
void normal(int *a, ALLEGRO_FONT *font){ 
    int i=*a;
    if(i<order){
        al_draw_bitmap(back,0, OFFSETUP,0); // color de fondo electo por usuario
        al_draw_textf(font, al_map_rgb(255,255,255), 60, SCREENH-50 ,ALLEGRO_ALIGN_LEFT, "presionar barra, enter o touch ");
        al_draw_textf(font, al_map_rgb(255,255,255), 60, SCREENH-17 ,ALLEGRO_ALIGN_LEFT, " para variaciones artisticas");
        koch(LARGOFRACTAL,i,tol,SCREENW/2,SCREENH/2);//dibujo el copo
        i++;//aumento orden
        }
        else {
            i=0; //vuelvo a empezar
        }
    *a=i;
}

void changefractal(void){
    if(!variaciones && !masvariaciones){
        variaciones=true;
        al_draw_bitmap(back2,0, OFFSETUP,0); // color verde de fondo
        al_draw_text(japan, al_map_rgb(0,0,0), 15, SCREENH-50 ,ALLEGRO_ALIGN_LEFT, "psychedelic fractal art");
        kbz=true;
    }
    else if(variaciones){
        masvariaciones=true;
        variaciones=false;
        al_draw_bitmap(back1,0, OFFSETUP,0); // color rojo
        kbz=true;
        al_draw_text(japan, al_map_rgb(0,0,0), 35, SCREENH-50 ,ALLEGRO_ALIGN_LEFT, "infinite fractal art");
    }
    else if(masvariaciones){
        masvariaciones=false;
        variaciones=false;
        al_draw_bitmap(back,0, OFFSETUP,0);
    }
    
}

void correrfractal(void) {
    // variables
    ALLEGRO_TIMER *timer = NULL, *kochtimer=NULL;
    ALLEGRO_EVENT_QUEUE *event_queue=NULL;
    ALLEGRO_FONT *font=NULL;
    bool redraw=false;
    double l=LARGOARTE; // variable para el largo de las variaciones artísticas del fractal
    int i=0, j=0, k=0;
    boton items[1];
    void (*p2buttonf[1])(void);
    
    //add change button
    botones(&items[0],0,OFFSETUP, SCREENW, SCREENH);
    p2buttonf[0]=changefractal;    
    
    //inicializo addons
    al_init_primitives_addon();
    back=al_create_bitmap(SCREENW, SCREENH);
    
    al_set_target_bitmap(back);
    
    al_clear_to_color(al_map_rgb(red,green,blue)); //pongo el display en el color ingresado
    al_flip_display(); //flipeo display
    
    back1=al_create_bitmap(SCREENW, SCREENH);
    al_set_target_bitmap(back1);
    al_clear_to_color(al_map_rgb(205,51,51)); //pongo el display en el color ingresado
    al_flip_display(); //flipeo display
    
    back2=al_create_bitmap(SCREENW, SCREENH);
    al_set_target_bitmap(back2);
    al_clear_to_color(al_map_rgb(124,252,0)); //pongo el display en el color ingresado
    al_flip_display(); //flipeo display
    
    
    al_set_target_bitmap(al_get_backbuffer(display));
    
    //timers
    timer = al_create_timer(1.0 / FPS); //genero un timer para trabajar en60fps
    if(!timer) {
    fprintf(stderr, "failed to create timer!\n");
    }
    kochtimer = al_create_timer(2); //genero un timer para trabajar en60fps
    if(!timer) {
        fprintf(stderr, "failed to create timer!\n");
    }
    
    //Genero una cola de eventos 
    event_queue = al_create_event_queue(); 
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
    }
    
    //Vision
    japan = al_load_ttf_font("japan.ttf",30,0 ); // cargo fonts
    if (!japan){
      fprintf(stderr, "Could not load 'japan.ttf'.\n");
    }
    font = al_load_ttf_font("font.ttf",15,0 );
    
    if (!font){
      fprintf(stderr, "Could not load 'font.ttf'.\n");
    }
    
    srand(time(NULL));// randomize
    for(k=0;k<CANTICOLORS;k++)
        colors[k]=al_map_rgb(rand()%255,rand()%255,rand()%255); // cargo CANTICOLORS de forma random
   
    // registro eventos
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(kochtimer));
    
    al_draw_bitmap(back,0, OFFSETUP,0);
    
    al_start_timer(timer); //empiezan timers
    al_start_timer(kochtimer);
    while(!doexit && !exitmenu) // main loop
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev); // espero a timer, salida o keyboard
        if(ev.timer.source == timer) 
            redraw = true; // si es el timer de refresco, redibujo
        salio(ev);
        inicio(ev);
        presionobotones(ev, items, p2buttonf,1);
        if(kbz){
            i=0; j=0; l=LARGOARTE;
            kbz=false;
        }
        if(ALLEGRO_KEY_SPACE==ev.keyboard.keycode) { // si se presionó la barra, activo/desactivo variaciones artísticas
            variaciones=!variaciones;
            i=0; j=0; l=LARGOARTE;
                if(variaciones){ // si se activaron las variaciones artísticas
                    al_draw_bitmap(back2,0, OFFSETUP,0); // color verde de fondo
                    al_draw_text(japan, al_map_rgb(0,0,0), 15, SCREENH-50 ,ALLEGRO_ALIGN_LEFT, "psychedelic fractal art");
                }
                else // si se volvió al modo normal
                    al_draw_bitmap(back,0, OFFSETUP,0);
            al_flip_display();
        }
        else if(ALLEGRO_KEY_ENTER==ev.keyboard.keycode) { // si se presionó la barra, activo/desactivo variaciones artísticas
            masvariaciones=!masvariaciones; 
            variaciones=false;
            i=0; j=0; l=LARGOARTE;
                if(masvariaciones){ // si se activaron más variaciones artísticas
                    al_draw_bitmap(back1,0, OFFSETUP,0); // color rojo
                    al_draw_text(japan, al_map_rgb(0,0,0), 35, SCREENH-50 ,ALLEGRO_ALIGN_LEFT, "infinite fractal art");
                }
                else // si se volvió al modo normal
                al_draw_bitmap(back,0, OFFSETUP,0);
            al_flip_display();
        }
        if(redraw){ // si hay que redibujar
            if(!al_is_event_queue_empty(event_queue)){ //me fijo si hay eventos del timer para redibujar el copo
                al_flush_event_queue(event_queue);// limpio la queue de eventos
                if(variaciones){ // si las variaciones 
                    art(&i,&j,&l);
                } else if(masvariaciones){
                    morevariations(&i);
                } else
                    normal(&i, font);
            }
                redraw=false;
                al_flip_display(); //flipeo 
        }
      }
    
    variaciones=false;
    masvariaciones=false;
    exitmenu=false;
    al_destroy_timer(timer);
    al_destroy_bitmap(back);
    al_destroy_bitmap(back1);
    al_destroy_bitmap(back2);
    al_destroy_timer(kochtimer);
    al_destroy_event_queue(event_queue);
    al_shutdown_primitives_addon();
}

