
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>
#include "estructuras.h"
#include "botonera.h"
#include "globals.h"
#include "wallpaper.h"

void drawfondo(void)
{
    if(mainfondo)
        al_draw_bitmap(mainfondo,0, OFFSETUP,0);
}

void cambiarfondo1(void){
    ALLEGRO_BITMAP *fondo=NULL;
    static bool cargo=false;
    fondo=al_load_bitmap("wallp1.png");
    mainfondo=fondo;
    wallnumber=1;
    exitmenu=true;
}

void cambiarfondo2(void){
    ALLEGRO_BITMAP *fondo=NULL;
    fondo=al_load_bitmap("wallp2.png");
    mainfondo=fondo;
    wallnumber=2;
    exitmenu=true;
}

void cambiarfondo3(void){
    ALLEGRO_BITMAP *fondo=NULL;
    fondo=al_load_bitmap("wallp3.png");
    mainfondo=fondo;
    wallnumber=3;
    exitmenu=true;
}
void cambiarfondo4(void){
    ALLEGRO_BITMAP *fondo=NULL;
    fondo=al_load_bitmap("wallp4.png");
    mainfondo=fondo;
    wallnumber=4;
    exitmenu=true;
}

void correrwallp(void){
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT_QUEUE *queue=NULL;
    ALLEGRO_BITMAP *fondo=NULL, *icons[4];
    boton items[4];
    void (*p2buttonf[4])(void);
    
    queue = al_create_event_queue(); 
    timer = al_create_timer(1.0/FPS); //genero un timer para trabajar en 60 fps
    bool redraw=false;
    fondo=al_load_bitmap("wallfondo.png");
    
    //wally1
    icons[0]=al_load_bitmap("wally1.png");
    botones(&items[0],30,70, 150, 200);
    p2buttonf[0]=cambiarfondo1;
    
    //wally2
    icons[1]=al_load_bitmap("wally2.png");
    botones(&items[1],230,70, 150, 200);
    p2buttonf[1]=cambiarfondo2;
    
    //wally3
    icons[2]=al_load_bitmap("wally3.png");
    botones(&items[2],30,330, 150, 200);
    p2buttonf[2]=cambiarfondo3;
    
    //wally4
    icons[3]=al_load_bitmap("wally4.png");
    botones(&items[3],230,330, 150, 200);
    p2buttonf[3]=cambiarfondo4;
    
    //register events
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    //timer starts
    al_start_timer(timer);
    
    while(!doexit && !exitmenu) //  loop
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev); // espero a timer, salida o keyboard
        if(ev.type == ALLEGRO_EVENT_TIMER) 
            redraw = true; // si es el timer de refresco, redibujo
        salio(ev);
        inicio(ev);
        presionobotones(ev, items, p2buttonf,4);
        if(redraw){ // si hay que redibujar
            al_draw_bitmap(fondo, 0, OFFSETUP, 0);
            drawbotones(items, icons,4);
            redraw=false;
            al_flip_display(); //flipeo 
        }
    }
    al_destroy_event_queue(queue);
    al_destroy_bitmap(fondo);
    al_destroy_bitmap(icons[0]);
    al_destroy_bitmap(icons[1]);
    al_destroy_bitmap(icons[2]);
    al_destroy_bitmap(icons[3]);
    al_destroy_timer(timer);
    exitmenu=false;
}