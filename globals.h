/* 
 * File:   globals.h
 * Author: julian
 *
 * Created on July 11, 2014, 3:26 PM
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

extern const int SCREEN_W; 
extern const int SCREEN_H;
extern const int OFFSETUP;
extern const int OFFSETDOWN;
extern const int FONDO_H;
extern const int FPS;
extern const int BUTTONSIZE;
extern const int wallsize_W;
extern const int wallsize_H;

#ifndef DESTINO
enum DESTINO {MESSI, MOORE, FOURIER, BOWIE, NEWTON, BATMAN};
#endif

extern int destination;
extern bool people[6];
extern bool doexit;
extern bool paso;
extern bool exitmenu;
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_BITMAP *msgactual;
extern ALLEGRO_BITMAP *mainfondo;
extern int wallnumber;
#endif	/* GLOBALS_H */

