#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include "estructuras.h"
#include "globals.h"
#include "wallpaper.h"
#include "musica.h"


ALLEGRO_SAMPLE *soundtrack=NULL;
ALLEGRO_SAMPLE_ID id;

void pajaros(void){
    if(soundtrack){
        al_stop_sample(&id);
        al_destroy_sample(soundtrack);
        soundtrack=NULL;
    }
    soundtrack = al_load_sample("pajaros.wav"); // pájaros de motoneta
    if (!soundtrack)
    {
        printf( "Audio clip sample not loaded!\n" );
    }
    al_play_sample(soundtrack, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,&id); // le doy play a la canción
}

void clorofila(void){    
    if(soundtrack){
        al_stop_sample(&id);
        al_destroy_sample(soundtrack);
        soundtrack=NULL;
    }
    soundtrack = al_load_sample("clorofila.wav"); // pájaros de motoneta
    if (!soundtrack)
    {
        printf( "Audio clip sample not loaded!\n" );
    }
    al_play_sample(soundtrack, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,&id); // le doy play a la canción
}

void graciasalfred(void){    
    if(soundtrack){
        al_stop_sample(&id);
        al_destroy_sample(soundtrack);
        soundtrack=NULL;
    }
    soundtrack = al_load_sample("graciasalfred.wav"); // pájaros de motoneta
    if (!soundtrack)
    {
        printf( "Audio clip sample not loaded!\n" );
    }
    al_play_sample(soundtrack, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,&id); // le doy play a la canción
}

void brisa(void){    
    if(soundtrack){
        al_stop_sample(&id);
        al_destroy_sample(soundtrack);
        soundtrack=NULL;
    }
    soundtrack = al_load_sample("brisa.wav"); // pájaros de motoneta
    if (!soundtrack)
    {
        printf( "Audio clip sample not loaded!\n" );
    }
    al_play_sample(soundtrack, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,&id); // le doy play a la canción
}

void mute(void){
    if(soundtrack){
    al_stop_sample(&id);
    al_destroy_sample(soundtrack);
    soundtrack=NULL;
    }
}


void corrermusica(void){
    const int CANTICANCIONES=5;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT_QUEUE *queue=NULL;
    ALLEGRO_BITMAP *icons[CANTICANCIONES];
    boton items[CANTICANCIONES];
    void (*p2buttonf[CANTICANCIONES])(void);
    ALLEGRO_FONT *font=NULL;
    
    queue = al_create_event_queue(); 
    timer = al_create_timer(1.0/FPS); //genero un timer para trabajar en 60 fps
    bool redraw=false;
    
    //pajaros
    icons[0]=al_load_bitmap("pajaros.png");
    botones(&items[0],30,70, 150, 200);
    p2buttonf[0]=pajaros;
    
    //clorofila
    icons[1]=al_load_bitmap("clorofila.png");
    botones(&items[1],230,70, 150, 200);
    p2buttonf[1]=clorofila;
    
    //brisa
    icons[2]=al_load_bitmap("brisa.png");
    botones(&items[2],30,330, 150, 200);
    p2buttonf[2]=brisa;
    
    //alfred
    icons[3]=al_load_bitmap("graciasalfred.png");
    botones(&items[3],230,330, 150, 200);
    p2buttonf[3]=graciasalfred;

    //mute
    icons[4]=al_load_bitmap("mute.png");
    botones(&items[4],340,OFFSETDOWN-70, 64, 64);
    p2buttonf[4]=mute;
    
    //font 
    font=al_load_font("font.ttf",30,0);
    
    //register events
    al_register_event_source(queue, al_get_display_event_source(display));
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
        presionobotones(ev, items, p2buttonf,CANTICANCIONES);
        if(redraw && !exitmenu){ // si hay que redibujar
            drawfondo();
            al_draw_text(font, al_map_rgb(171,130,255), 10, OFFSETDOWN-55, ALLEGRO_ALIGN_LEFT,"Musica por Motoneta");
            drawbotones(items, icons,CANTICANCIONES);
            redraw=false;
            al_flip_display(); //flipeo 
        }
    }
    
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_bitmap(icons[0]);
    al_destroy_bitmap(icons[1]);
    al_destroy_bitmap(icons[2]);
    al_destroy_bitmap(icons[3]);
    
    exitmenu=false;
}
