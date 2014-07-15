/* 
 * File:   fractales.h
 * Author: julian
 *
 * Created on July 13, 2014, 12:09 PM
 */

#ifndef FRACTALES_H
#define	FRACTALES_H

void correrfractal(void);
void koch(double largo, int n, double tolerancia, double x, double y);
void lineafractal(double largo, double ang, int n, double tolerancia);
void polares(double radio, double ang);
void art(int *a, int *b, double *largo);
void morevariations(int *a);
void normal(int *a, ALLEGRO_FONT *font);
void changefractal(void);
void correrfractal(void);

#endif	/* FRACTALES_H */

