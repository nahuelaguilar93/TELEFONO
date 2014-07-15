#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include "estructuras.h"
#include "globals.h"
#include "botonera.h"

void botones(boton *bot, unsigned int x, unsigned int y, unsigned int largo, unsigned int alto){
    bot->pto1.x=x;
    bot->pto1.y=y;
    bot->pto2.x=x+largo;
    bot->pto2.y=y;
    bot->pto3.x=x;
    bot->pto3.y=y+alto;
    bot->pto4.x=x+largo;
    bot->pto4.y=y+alto;
}

void presionobotones(ALLEGRO_EVENT event, boton item[], void (*p2f[]) (void), int canti){
    int i;
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        for(i=0;i<canti;i++)
              if (event.mouse.x >= item[i].pto1.x && event.mouse.x < item[i].pto2.x && event.mouse.y >= item[i].pto1.y && event.mouse.y < item[i].pto3.y){
                p2f[i]();
              }
    }
}

void drawbotones(boton item[], ALLEGRO_BITMAP **icons, int canti)
{
    int i;
    for(i=0;i<canti;i++)
        al_draw_bitmap(icons[i],item[i].pto1.x,item[i].pto1.y,0);
    
}

void inicio(ALLEGRO_EVENT event){
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        if (event.mouse.x >= 175 && event.mouse.x < 233 && event.mouse.y >= 622 && event.mouse.y < 638)
            exitmenu=true;  
    }
}

void salio(ALLEGRO_EVENT ev){
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            doexit=true; // si se cerro la ventana salgo
        
    if(ALLEGRO_KEY_ESCAPE==ev.keyboard.keycode) ;
          //  doexit=true; // la esta delirando el allegro
}