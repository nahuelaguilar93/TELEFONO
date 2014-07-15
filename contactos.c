#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include "globals.h"
#include "estructuras.h"
#include "mensaje.h"
#include "contactos.h"

void messi(void){ // hay que cambiar if(loadsms(destinationmessi)==NULL)
     msgactual = al_load_bitmap("messtalk.png");
     destination=MESSI;
     corrermessage();
}
void batman(void){
     msgactual = al_load_bitmap("batmantalk.png");
     destination=BATMAN;
     corrermessage();
}
void bowie(void){
     msgactual = al_load_bitmap("bowietalk.png");
     destination=BOWIE;
     corrermessage();
}
void moore(void){
     msgactual = al_load_bitmap("mooretalk.png");
     destination=MOORE;
     corrermessage();
}
void newton(void){
     msgactual = al_load_bitmap("newtontalk.png");
     destination=NEWTON;
     corrermessage();
}
void fourier(void){
     msgactual = al_load_bitmap("fouriertalk.png");
     destination=FOURIER;
     corrermessage();
}

void corrercontactos(void){
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT_QUEUE *queue=NULL;
    ALLEGRO_BITMAP  *fondo=NULL;
    boton items[6];
    void (*p2buttonf[6])(void);
    
    queue = al_create_event_queue(); 
    timer = al_create_timer(1.0/FPS); //genero un timer para trabajar en 60 fps
    bool redraw=false;
    fondo=al_load_bitmap("list.png");
    
    //messi
    botones(&items[0],3,70+OFFSETUP, 400, 85);
    p2buttonf[0]=messi;

    //moore
    botones(&items[1],30,155+OFFSETUP, 400, 85);
    p2buttonf[1]=moore;

    //fourier
    botones(&items[2],0,240+OFFSETUP, 400, 85);
    p2buttonf[2]=fourier;
    
    //bowie
    botones(&items[3],0,325+OFFSETUP, 400, 85);
    p2buttonf[3]=bowie;
    
    //newton
    botones(&items[4],0,410+OFFSETUP, 400, 85);
    p2buttonf[4]=newton;
    
    //batman
    botones(&items[5],0,495+OFFSETUP, 400, 85);
    p2buttonf[5]=batman;

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
        presionobotones(ev, items, p2buttonf,6);
        if(redraw && !exitmenu){ // si hay que redibujar
            al_draw_bitmap(fondo, 0, OFFSETUP, 0);
            redraw=false;
            al_flip_display(); //flipeo 
        }
    }
    al_destroy_event_queue(queue);
    al_destroy_bitmap(fondo);
    al_destroy_timer(timer);
    exitmenu=false;
}