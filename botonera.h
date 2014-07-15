/* 
 * File:   botonera.h
 * Author: julian
 *
 * Created on July 11, 2014, 3:21 PM
 */

#ifndef BOTONERA_H
#define	BOTONERA_H

void botones(boton *bot, unsigned int x, unsigned int y, unsigned int largo, unsigned int alto);
void presionobotones(ALLEGRO_EVENT event, boton item[], void (*p2f[]) (void), int canti);
void drawbotones(boton item[], ALLEGRO_BITMAP **icons, int canti);
void inicio(ALLEGRO_EVENT event);
void salio(ALLEGRO_EVENT ev);

#endif	/* BOTONERA_H */

