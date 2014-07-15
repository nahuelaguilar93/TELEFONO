#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include "estructuras.h"
#include "botonera.h"
#include "globals.h"
#include "mensaje.h"
#include "diccionario.h"

static int errores[20][2];
static int cantierrors;

void teclado(ALLEGRO_EVENT ev, int *letras, unsigned char bigbuffer[])
{
    int i;
    static bool mayus=false;
    static bool tilde=false;
    if(ev.type == ALLEGRO_EVENT_KEY_UP) { // verifico que el evento haya sido una tecla levantada
        for(i=0;i<26;i++){
            if((i+ALLEGRO_KEY_A)== ev.keyboard.keycode){ // si se presionaron teclas del teclado de a to z
                bigbuffer[*letras] ='a'+i;
                if(mayus) bigbuffer[*letras]-=0x20; // si mayus esta activida resto 20 hexa para pasar de a to A etc
                if (tilde)
                    switch(bigbuffer[*letras]){// si la tilde esta activada y luego se ingresa una vocal, cambio la vocal sin acentuar por la acentuada
                        case 'a': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0xA1;break; //estan en UTF8 y les corresponden 2 bytes a cada letra acentuada
                        case 'e': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0xA9;break;
                        case 'i': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0xAD;break;
                        case 'o': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0xB3;break;
                        case 'u': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0xBA;break;
                        case 'A': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0x81;break;
                        case 'E': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0x89;break;
                        case 'I': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0x8D;break;
                        case 'O': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0x93;break;
                        case 'U': bigbuffer[*letras]=0xC3; bigbuffer[(++*letras)]=0x9A;break;
                    }
                bigbuffer[(++*letras)]=0x00; // aumento tamaño total buffer
            }
        }

        for(i=0;i<=9;i++){// si se ingresaron números
            if((i+ALLEGRO_KEY_0)== ev.keyboard.keycode){
                bigbuffer[*letras] ='0'+i;
                bigbuffer[(++*letras)]=0x00;
            }
        }
        // coma
        if(ALLEGRO_KEY_COMMA==ev.keyboard.keycode){
            bigbuffer[*letras] =',';
            bigbuffer[++(*letras)]=0x00;
        }
        //punto
        if(ALLEGRO_KEY_FULLSTOP==ev.keyboard.keycode){
            bigbuffer[*letras] ='.';
            bigbuffer[++(*letras)]=0x00;
        }
        //barra espaciadora
        if(ALLEGRO_KEY_SPACE==ev.keyboard.keycode){
            bigbuffer[*letras] =' ';
            bigbuffer[++(*letras)]=0x00;
        }
         //mayus
        if(ALLEGRO_KEY_CAPSLOCK==ev.keyboard.keycode)
            mayus=!mayus;
        // tilde
        if(ALLEGRO_KEY_INSERT==ev.keyboard.keycode)
            tilde=true;
        else 
            tilde=false; // se desactiva la tilde una vez que se agregó una letra después de la tilde
        if(*letras)
			cantierrors=marcar_errores(bigbuffer,errores);
    }
}

void drawmsg(ALLEGRO_FONT *font){
    int i,j,k=0;
    unsigned char rtabuffer[200]; // string donde cargo el mensaje
    char aux[3]; // para las vocales acentuadas en UTF8
    int largo=al_get_text_width(font, "a");// monospaced font, el largo es el mismo para todas las letras
    if(get_msgs(rtabuffer,destination)){// cargo el ultimo mensaje, si no hay ninguno da 0 e imprimo mensaje default
        msgactual=al_create_bitmap(250, 100);
        al_set_target_bitmap(msgactual); // escribo sobre msgactual
        al_clear_to_color(al_map_rgb(50,200,0)); //verde clarito 
            for(j=0;j<4;j++){// hasta 4 lineas de texto
                for(i=0;i<24;i++){ // hasta 24 caracteres por linea
                    if(!rtabuffer[k])// si llegue al final salgo
                        break;
                    if(rtabuffer[k]==0xC3){  // si es una vocal acentuada que ocupa 2 bytes
                        aux[0]=rtabuffer[k++]; // cargo esa vocal como string en aux
                        aux[1]=rtabuffer[k];
                        aux[2]=0x00;// y la imprimo
                        al_draw_textf(font, al_map_rgb(0,0,0),  2+i*largo, j*al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT,"%s",aux);
                    }else// si no es una vocal acentuada imprimo el caracter suelto
                         al_draw_textf(font, al_map_rgb(0,0,0), 2+i*largo, j*al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT,"%c",rtabuffer[k]);
                    k++;
                }
                if(!rtabuffer[k])
                    break;
            }
            al_set_target_bitmap(al_get_backbuffer(display)); // le devuelvo el control al display
        al_draw_bitmap(msgactual, 130, OFFSETUP+30, 0);// dibujo el mensaje de respuesta
    }
    else
        al_draw_bitmap(msgactual, 15, OFFSETUP+30, 0); //dibuja el mensaje default
}

void drawletras(ALLEGRO_FONT *font,unsigned char bigbuffer[], int letras){ //dibuja el texto que todavía no se mando
    int i,j,k=0,t;// igual a drawmsg pero para el texto que todavía no se mando
    char aux[3]; bool rojo=false; static int tilt=0;
    int largo=al_get_text_width(font, "a");
    for(j=0;j<4;j++){ // por las cuatro lineas
        for(i=0;i<24;i++){ //de 0 a 24 caracteres monospaced por linea
            if(!bigbuffer[k]) // si se llego al final del buffer salgo
                break;
            for(t=0;t<cantierrors;t++) // checkeo errores ortograficos
                if(errores[t][0]==k)
                    rojo=true;// si estoy dentro de un error dibujo en rojo
                else if(errores[t][1]==k)
                    rojo=false;// si no estoy dentro de un error dibujo en negro
            if(bigbuffer[k]==0xC3){// para vocales con tilde
                aux[0]=bigbuffer[k++];
                aux[1]=bigbuffer[k];
                aux[2]=0x00;// escribo sobre el display blanco del celular                    
                if(rojo)
                    al_draw_textf(font, al_map_rgb(255,0,0), 58+i*largo, OFFSETDOWN-370+j*al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT,"%s",aux);
                else
                    al_draw_textf(font, al_map_rgb(0,0,0), 58+i*largo, OFFSETDOWN-370+j*al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT,"%s",aux);
            }
            else// escribo sobre el display blanco del celular
                if(rojo)
                    al_draw_textf(font, al_map_rgb(255,0,0), 58+i*largo, OFFSETDOWN-370+j*al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT,"%c",bigbuffer[k]);
                else 
                    al_draw_textf(font, al_map_rgb(0,0,0), 58+i*largo, OFFSETDOWN-370+j*al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT,"%c",bigbuffer[k]);
            k++;
        }
        if(!bigbuffer[k])
            break;
    }
    if(tilt>5){ // para lograr un efecto de titileo
        al_draw_text(font, al_map_rgb(0,0,0), 58+i*largo, OFFSETDOWN-370+j*al_get_font_line_height(font), ALLEGRO_ALIGN_LEFT,"|");
        tilt++;
        if(tilt>10)
            tilt=0;
    }
    else
        tilt++;
        
}

void drawpiano(ALLEGRO_BITMAP *piano,ALLEGRO_BITMAP *piano2, bool abcde)
{
    if(abcde)// si esta activado el teclado alfabético lo dibujo
        al_draw_bitmap(piano,1, OFFSETDOWN-390,0);
    else// si esta activado el teclado numérico lo dibujo
        al_draw_bitmap(piano2,1, OFFSETDOWN-390,0);
}

void changepiano(ALLEGRO_EVENT event, boton item, bool *abcde)
{    // cambio de teclado, item corresponde al boton de cambio de teclado
    int i;
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){ // si se clickeo en el boton de cambio de teclado
        if (event.mouse.x >= item.pto1.x && event.mouse.x < item.pto2.x && event.mouse.y >= item.pto1.y && event.mouse.y < item.pto3.y){
            (*abcde)=!(*abcde);   // cambio el estado de teclado true->alfabético false->numérico
            }
    }
}

void presionoenter(ALLEGRO_EVENT event, boton item, char bigbuffer[],int *letras){ // función que verifica si se presiono enter por teclado o qwerty
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){// por qwerty
        if (event.mouse.x >= item.pto1.x && event.mouse.x < item.pto2.x && event.mouse.y >= item.pto1.y && event.mouse.y < item.pto3.y){
            entermsg(bigbuffer, letras);   // si se presiono enter voy a entermsg
            }
    }
    else if(event.type==ALLEGRO_EVENT_KEY_DOWN) // por teclado
            if(ALLEGRO_KEY_ENTER==event.keyboard.keycode){
                 entermsg(bigbuffer, letras); 
    }
}

void presionobackspace(ALLEGRO_EVENT event, boton item, char bigbuffer[],int *letras){// función que verifica si se presiono backspace por teclado o qwerty
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){// por qwerty
        if (event.mouse.x >= item.pto1.x && event.mouse.x < item.pto2.x && event.mouse.y >= item.pto1.y && event.mouse.y < item.pto3.y)
            if((*letras)>0){
                bigbuffer[--*letras]=0x00;//disminuyo buffer
                if((unsigned char)bigbuffer[*letras]==0xC3)
                    bigbuffer[--*letras]=0x00;
            }
    }
    else if(event.type==ALLEGRO_EVENT_KEY_DOWN)// por teclado
        if(ALLEGRO_KEY_BACKSPACE==event.keyboard.keycode)
            if((*letras)>0){
                bigbuffer[--*letras]=0x00;//disminuyo buffer
                if((unsigned char)bigbuffer[*letras]==0xC3)
                    bigbuffer[--*letras]=0x00;
            }
}

void teclaqwerty(int i, char bigbuffer[400], int *letras){
    static bool mayus=false; // recibe una letra correspondida por un nro de tecla ordenada segun qwerty q=0, w=1, etc
    bool escribio=true; // y la coloca en el buffer
    switch(i){
            case 0: bigbuffer[*letras]='q';break;// teclado alfabetico
            case 1: bigbuffer[*letras]='w';break;
            case 2: bigbuffer[*letras]='e';break;
            case 3: bigbuffer[*letras]='r';break;
            case 4: bigbuffer[*letras]='t';break;
            case 5: bigbuffer[*letras]='y';break;
            case 6: bigbuffer[*letras]='u';break;
            case 7: bigbuffer[*letras]='i';break;
            case 8: bigbuffer[*letras]='o';break;
            case 9: bigbuffer[*letras]='p';break;
            case 10: bigbuffer[*letras]='a';break;
            case 11: bigbuffer[*letras]='s';break;
            case 12: bigbuffer[*letras]='d';break;
            case 13: bigbuffer[*letras]='f';break;
            case 14: bigbuffer[*letras]='g';break;
            case 15: bigbuffer[*letras]='h';break;
            case 16: bigbuffer[*letras]='j';break;
            case 17: bigbuffer[*letras]='k';break;
            case 18: bigbuffer[*letras]='l';break;
            case 19: bigbuffer[*letras]='z';break;
            case 20: bigbuffer[*letras]='x';break;
            case 21: bigbuffer[*letras]='c';break;
            case 22: bigbuffer[*letras]='v';break;
            case 23: bigbuffer[*letras]='b';break;
            case 24: bigbuffer[*letras]='n';break;            
            case 25: bigbuffer[*letras]='m';break;
            case 26: mayus=!mayus; escribio=false; break;// mayusculas
            case 27: bigbuffer[*letras]=' ';break;// teclado numerico
            case 37: bigbuffer[*letras]='0';break;
            case 38: bigbuffer[*letras]='-';break;
            case 39: bigbuffer[*letras]='/';break;
            case 40: bigbuffer[*letras]=':';break;
            case 41: bigbuffer[*letras]=';';break;
            case 42: bigbuffer[*letras]='(';break;
            case 43: bigbuffer[*letras]=')';break;
            case 44: bigbuffer[*letras]='$';break;
            case 45: bigbuffer[*letras]='&';break;
            case 46: bigbuffer[*letras]='@';break;
            case 47: bigbuffer[*letras]='"';break;
            case 48: bigbuffer[*letras]='.';break;
            case 49: bigbuffer[*letras]=',';break;
            case 50: bigbuffer[*letras]='?';break;
            case 51: bigbuffer[*letras]='!';break;
            case 52: bigbuffer[*letras]=' ';break; 
    }
    if(i>27 && i<37){ // 1 to 9
        bigbuffer[*letras]=i-28+'1';
    }
    
    if(escribio){ // si es un caracter más (no es el boton mayus)
        if(mayus)
            (bigbuffer[*letras])-=0x20; // si las mayusculas estan activadas paso a UCASE
        (*letras)++ ; // aumenta buffer
        bigbuffer[(*letras)+1]=0x00;//terminador
    }
    if(*letras)
		cantierrors=marcar_errores(bigbuffer,errores);
}

void presionoqwerty(ALLEGRO_EVENT event, boton item[], int inicial, int canti,char bigbuffer[400], int *letras){
    int i;// recibo items correspondientes a todos los botones del qwerty
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){ // verifico en todos los items si se presiono el qwerty
        for(i=inicial;i<canti;i++)
              if (event.mouse.x >= item[i].pto1.x && event.mouse.x < item[i].pto2.x && event.mouse.y >= item[i].pto1.y && event.mouse.y < item[i].pto3.y){
                teclaqwerty(i, bigbuffer, letras);   //paso a teclaqwerty que numero de tecla se presiono, el buffer y la cantidad de letras totales
              }
    }
}

void entermsg(char bigbuffer[],int *letras){
    
    if(bigbuffer[0])//por si no se envía nada
        save_msgs(bigbuffer,destination);// guardo mensaje enviado
    bigbuffer[0]=0x00; // reseteo mi buffer
    (*letras)=0;// la cantidad de letras vuelve a 0
}

void corrermessage(void){
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT_QUEUE *queue=NULL;
    ALLEGRO_BITMAP *piano=NULL, *piano2=NULL;
    ALLEGRO_FONT *font=NULL;
    boton items[52], enter, cambiotec, backspace;
    char bigbuffer[400];
    char rtabuffer[50];
    int letras = 0;
    queue = al_create_event_queue(); 
    timer = al_create_timer(1.0/FPS); //genero un timer para trabajar en 60 fps
    bool redraw=false, abcde=true;
    
    
    //register events
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    
    //timer and font starts
    al_start_timer(timer);
    font=al_load_font("sans.ttf",17,0);
    
    //load visual keyboard
    piano=al_load_bitmap("piano1.png");
    piano2=al_load_bitmap("piano2.png");
    int i;
    //piano1 (piano=teclado)
    for(i=0;i<10;i++)// Q to P 
        botones(&items[i],3+33*i+8*i,OFFSETDOWN-390+139,33,48);
    for(i=0;i<9;i++) // A to L
        botones(&items[i+10],22+33*i+8*i,OFFSETDOWN-390+207,33,48);
    for(i=0;i<7;i++) // Z to M
        botones(&items[i+19],63+33*i+8*i,OFFSETDOWN-390+275,33,48);
    botones(&items[26],3,OFFSETDOWN-390+275,42,50); // uppercase
    botones(&items[27],104,OFFSETDOWN-390+342,195,50); // SPACE
    botones(&backspace,355,OFFSETDOWN-390+275,42,50); // backspace
    botones(&enter,317,OFFSETDOWN-390+83,73,34);//enter
    //piano2
    for(i=0;i<10;i++)// 1 to 0 
        botones(&items[i+28],3+33*i+8*i,OFFSETDOWN-390+139,33,48);
    for(i=0;i<10;i++)// - to "
        botones(&items[i+38],3+33*i+8*i,OFFSETDOWN-390+207,33,48);
    for(i=0;i<5;i++)// . to '
        botones(&items[i+48],63+50*i+6*i,OFFSETDOWN-390+278,50,50);
    botones(&cambiotec,3,OFFSETDOWN-390+347,44,48);//cambio teclado
    
    //clean big buffer
    bigbuffer[0]=0x00;
    
    while(!doexit && !exitmenu) //  loop
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev); // espero a timer, salida o keyboard
        if(ev.type == ALLEGRO_EVENT_TIMER) 
            redraw = true; // si es el timer de refresco, redibujo
        salio(ev);// cerro el celular
        inicio(ev);//  boton inicio
        changepiano(ev, cambiotec, &abcde);// cambio teclado
        if(abcde) // si es alfabetico
            presionoqwerty(ev, items, 0 , 28, bigbuffer, &letras);
        else //si es numerico
            presionoqwerty(ev, items, 27, 54, bigbuffer, &letras);
        presionobackspace(ev, backspace, bigbuffer, &letras);//si borro
        presionoenter(ev, enter, bigbuffer, &letras);// si mando mensaje
        teclado(ev, &letras, bigbuffer);// ingreso por teclado
        if(redraw){ // si hay que redibujar
            drawfondo();// fondo
            drawpiano(piano, piano2, abcde); // teclado
            drawletras(font, bigbuffer, letras); // mensaje en el display blanco
            drawmsg(font);//mensaje enviado
            redraw=false;
            al_flip_display(); //flipeo 
        }
    }
    
    al_destroy_bitmap(piano);
    al_destroy_bitmap(piano2);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
}
