#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
extern int wallnumber;

void inicializar(void){
    
    if(!al_init()) { //inicializo allegro
        fprintf(stderr, "failed to initialize allegro!\n");
        exit(1);
    }
    if(!al_install_mouse()) {
        fprintf(stderr, "failed to initialize the mouse!\n");
        exit(1);
    }
    if(!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize image addon!\n");
        exit(1);
    }
    if(!al_install_keyboard()) { //installo keyboard
        fprintf(stderr, "failed to initialize the keyboard!\n");
        exit(1);
    }
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        exit(1);
    }
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        exit(1);
    }
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        exit(1);
    }
    
    al_init_font_addon(); 
    al_init_ttf_addon();
}

void terminar(){
    save_wallp(wallnumber);
    al_shutdown_image_addon();
    al_uninstall_mouse();
    al_uninstall_keyboard();   
}
