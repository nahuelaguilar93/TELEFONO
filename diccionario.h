/* 
 * File:   diccionario.h
 * Author: julian
 *
 * Created on July 14, 2014, 8:29 PM
 */

#ifndef DICCIONARIO_H
#define	DICCIONARIO_H

FILE * abrir_letra_dic(char *str);
int esletra(char *c);
int contilde(unsigned char *c);
char sacarmayus(unsigned char c);
int separarenpalabras(char bigbuffer[], char palabras[][15], int limites[][2]);
char sacartilde(unsigned char letra);
int errorpalabra(char word[15]);
int marcar_errores(char bigbuffer[], int errores[][2]);
        
#endif	/* DICCIONARIO_H */

