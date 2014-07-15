/* 
 * File:   save_file..h
 * Author: luio
 *
 * Created on July 13, 2014, 8:27 AM
 */

#ifndef SAVE_FILE__H
#define	SAVE_FILE__H

#define TRUE 1
#define FALSE 0
#define MAX_MSG 200
#define SEPARADOR 0xff
#define ERROR 0
#define BACKSPACE -1
#define DEFAULTW 1

#ifndef DESTINO
enum DESTINO {MESSI, MOORE, FOURIER, BOWIE, NEWTON, BATMAN};
#endif

int save_msgs(char *str,int contact);
int save_wallp(int wallpaper);
int get_msgs(char str[MAX_MSG],int contact);
int get_wallp(void);
char *get_filename(int contact);

/*********************************************************
 *  save_msgs is a function that recieves a string pointer to
 * a string message it must save and the contact to which that
 * message belongs to. It appends to the message history save
 * file. In case of an error returns 0 or a negative number, 
 * otherwise returns the character count.
 * ******************************************************/

/*********************************************************
 *  save_wallp recieves the number of the wallpaper it must
 * save to the configuration wallpaper save file. In case of 
 * an error returns 0, otherwise returns 1. 
 * ******************************************************/

/*********************************************************
 *  get_filename recieves the contact number and retrieves
 * its message history file's name for appending or creating. 
 * ******************************************************/

/*********************************************************
 *  get_wallp retrieves the wallpaper number from its
 * respective save file and returns it as an interger. 
 * In case of error returns the default wallpaper.
 * ******************************************************/

/*********************************************************
 *  get_msgs recieves the contact number and a free string
 * pointer to an array of the max message characters and
 * retrieves the last message from the respective message
 * history file. If an error occurrs returns a 0. Otherwise
 * returns a positive number.
 * ******************************************************/
/*NOTE: all errors in this functions are reported via stderr*/
#endif	/* SAVE_FILE__H */
