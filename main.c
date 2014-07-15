/* 
 * File:   main.c
 * Author: julian
 * Created on June 28, 2014, 2:40 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>
#include "wallpaper.h"
#include "estructuras.h"
#include "mensaje.h"
#include "contactos.h"
#include "musica.h"
#include "iomain.h"
#include "main.h"
#include "fractales.h"

//CONSTANTES GLOBALES
const int SCREEN_W = 400; 
const int SCREEN_H = 651;
const int OFFSETUP = 33;
const int OFFSETDOWN = 609;
const int FONDO_H=576;
const int FPS=10;
const int BUTTONSIZE= 64;
const int wallsize_W=400;
const int wallsize_H=600;

// VARIABLES GLOBALES
enum DESTINO {MESSI, MOORE, FOURIER, BOWIE, NEWTON, BATMAN};
int destination;
bool doexit=false;
bool exitmenu=false;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *msgactual = NULL;
ALLEGRO_BITMAP *mainfondo = NULL;
int wallnumber; 

int main(int argc, char** argv) {
    inicializar();
    correr();
    terminar();
    return (EXIT_SUCCESS);
}

void hora(ALLEGRO_FONT *font){
    time_t t; // dibuja la hora
    int hour, minutes;
    t = time(NULL);
    hour = localtime(&t)->tm_hour;
    minutes = localtime(&t)->tm_min;
    if(minutes<10)
        al_draw_textf(font, al_map_rgb(0,0,0), 200, 50, ALLEGRO_ALIGN_CENTRE,"%d:0%d",hour,minutes);
    else
        al_draw_textf(font, al_map_rgb(0,0,0), 200, 50, ALLEGRO_ALIGN_CENTRE,"%d:%d",hour,minutes);
}

void correr(void){
    const int CANTIBOTONES=4;
    ALLEGRO_TIMER *timer=NULL;
    ALLEGRO_EVENT_QUEUE *queue=NULL;
    ALLEGRO_BITMAP *main=NULL, *icons[CANTIBOTONES];
    bool redraw=false;
    ALLEGRO_FONT *font=NULL;
    boton items[CANTIBOTONES];
    void (*p2buttonf[CANTIBOTONES])(void);
    
    //create display, event queue and timer
    display = al_create_display(SCREEN_W, SCREEN_H);
    queue = al_create_event_queue(); 
    timer = al_create_timer(1.0/FPS); 

    //register events
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    
    //load bitmap
    main=al_load_bitmap("main.jpg");
    al_draw_bitmap(main,0,0,0);
    
    //mainfondo
    switch((wallnumber=get_wallp())){ // carga de wallpaper por archivo de configuracion
            case 1:cambiarfondo1(); break;
            case 2:cambiarfondo2(); break;
            case 3:cambiarfondo3(); break;
            case 4:cambiarfondo4(); break;
    }
    exitmenu=false;
    
    //wallpaper icon
    icons[0]=al_load_bitmap("wallpaper-icon.png");
    botones(&items[0], 30,OFFSETDOWN-BUTTONSIZE-25, BUTTONSIZE, BUTTONSIZE);
    p2buttonf[0]=correrwallp;
    
    //message icon
    icons[1]=al_load_bitmap("message-icon.png");
    botones(&items[1], 30*2+BUTTONSIZE*1,OFFSETDOWN-BUTTONSIZE-25, BUTTONSIZE, BUTTONSIZE);
    p2buttonf[1]=corrercontactos;
    
    //music icon
    icons[2]=al_load_bitmap("musica.png");
    botones(&items[2], 30*3+BUTTONSIZE*2,OFFSETDOWN-BUTTONSIZE-25, BUTTONSIZE, BUTTONSIZE);
    p2buttonf[2]=corrermusica;
    
    //fractal icon
    icons[3]=al_load_bitmap("fractal.png");
    botones(&items[3], 30*4+BUTTONSIZE*3,OFFSETDOWN-BUTTONSIZE-25, BUTTONSIZE, BUTTONSIZE);
    p2buttonf[3]=correrfractal;
    
    //start timer and font
    al_start_timer(timer);
    font=al_load_font("digital.ttf",72,0);
    
    while(!doexit) // main loop
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev); // espero a timer, salida o keyboard
            if(ev.type == ALLEGRO_EVENT_TIMER) 
                redraw = true; // si es el timer de refresco, redibujo
        salio(ev);// salio del celular=
        presionobotones(ev, items, p2buttonf,CANTIBOTONES); // presiono en los icons
        al_flush_event_queue(queue);
        if(redraw){ // si hay que redibujar
            drawfondo();// fondo
            drawbotones(items, icons,CANTIBOTONES);// dibujo icons
            hora(font);//horario
            redraw=false;
            al_flush_event_queue(queue);
            al_flip_display();//flipeo
        }
    }
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_bitmap(main);
    al_destroy_bitmap(mainfondo);
    al_destroy_event_queue(queue);
}