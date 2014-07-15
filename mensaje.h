/* 
 * File:   mensaje.h
 * Author: julian
 *
 * Created on July 12, 2014, 11:43 AM
 */

#ifndef MENSAJE_H
#define	MENSAJE_H

void teclado(ALLEGRO_EVENT ev, int *letras, unsigned char bigbuffer[]);
void drawmsg(ALLEGRO_FONT *font);
void drawletras(ALLEGRO_FONT *font,unsigned char bigbuffer[], int letras);
int divlin(int linesize,ALLEGRO_FONT *font, char bigbuffer[], char lines[][40], int letras);
void drawpiano(ALLEGRO_BITMAP *piano,ALLEGRO_BITMAP *piano2, bool abcde);
void changepiano(ALLEGRO_EVENT event, boton item, bool *abcde);
void presionoenter(ALLEGRO_EVENT event, boton item,char bigbuffer[],int *letras);
void presionobackspace(ALLEGRO_EVENT event, boton item, char bigbuffer[],int *letras);
void teclaqwerty(int i, char bigbuffer[], int *letras);
void presionoqwerty(ALLEGRO_EVENT event, boton item[], int inicial, int canti,char bigbuffer[], int *letras);
void entermsg(char bigbuffer[],int *letras);
void corrermessage(void);

#endif	/* MENSAJE_H */

