#include <stdio.h>
#include <stdlib.h>
#include "save_file.h"

int save_msgs(char *str,int contact)
{
    FILE *svfile;        /*File pointer to open the save file*/
    char *filename=get_filename(contact);   /*retrieves the save file name of the contact*/
    int ccount; /*Accumulator for character counting*/
    if((svfile=fopen(filename,"a"))==0)     /*creates or reopens the contact's save file*/
    {
        fprintf(stderr,"Failed to open save files from \"%s\".",filename);  /*Reports an error if it occurs*/
        return ERROR;
    }
    if((ccount=fprintf(svfile,"\n%c\n%s",SEPARADOR,str))<0) /*Writes in the message in the save file preceeded by a non-ASCII separator*/
    {
        fprintf(stderr,"Error writting on file \"%s\".",filename); /*Reports an error if it occurs*/
        return ERROR;
    }
    if(fclose(svfile))  /*Closes the file when done*/
    {
        fprintf(stderr,"Error closing file \"%s\".",filename); /*Reports an error if it occurrs*/
        return ERROR;
    }
    return ccount; /*if no errors occur, returns character count*/
}

int save_wallp(int wallpaper)   
{
    FILE *svfile;   /*Prepares a file pointer to open the save file*/
    char *filename="wallp.txt"; 
    if((svfile=fopen(filename,"w"))==0)  /*Always creates a new file if the wallpaper configuration changes*/
    {
       fprintf(stderr,"Failed to open save files from \"%s\".",filename);   /*In case of error reports it into stderr*/
        return ERROR;
    }
    if(fprintf(svfile,"Wallpaper: %d",wallpaper)<0) /*saves the new wallpaper number to the save file*/
    {
        fprintf(stderr,"Error writting on file \"%s\".",filename); /*If any error occurrs reports it*/
        return ERROR;
    }
    if(fclose(svfile))  /*Closes the new file when done*/
    {
        fprintf(stderr,"Error closing file \"%s\".",filename);   /*If the file can't be closed reports it*/
        return ERROR;
    }
    return TRUE; /*Returns a positive number upon success*/
}

int get_wallp(void)
{
    FILE *svfile;   /*Prepares a file pointer to open the save file*/
    char *filename="wallp.txt"; /*it is easier to write filename*/
    int wallpaper=DEFAULTW; /*Sets the wallpaper to default in case the file can't be read or it doesmn't exist*/
    if((svfile=fopen(filename,"r"))==0) /*Opens the save file for reading*/
    {
       fprintf(stderr,"Failed to open save files from \"%s\".",filename);  /*If an error ocurrs when opening file reports it*/
        return wallpaper;
    }
    fseek(svfile,BACKSPACE,SEEK_END);   /*Points at the wallpaper number, it is always at the end of the file, BACKSPACE is a -1 offset and SEEK_END is the origin at the End of File*/
    if(ferror(svfile))  /*If fseek fails, the ferror file flag is set to a non-zero number*/
    {
        fprintf(stderr,"Error reading on file \"%s\".",filename);   /*So if it fails, it reports it*/
        return wallpaper;
    }
    if(fscanf(svfile,"%d",&wallpaper)==EOF) /*Reads the wallpaper number as an interger and saves it into its variable*/
    {
        fprintf(stderr,"Error reading on file \"%s\".",filename);   /*Reports if a reading error occurs*/
        return (wallpaper=0);   /*Since a reading error doesn't guarantee the variables changing, resets the wallpaper to default before returning it*/
    }
    if(fclose(svfile))  /*Closes the file once done.*/
    {
        fprintf(stderr,"Error closing file \"%s\".",filename);  /*If an error ocurs reports it*/
        return wallpaper;
    }
    return wallpaper;   /*Returns the retrieved wallpaper number*/
}

int get_msgs(char str[MAX_MSG],int contact)
{
    FILE *svfile;        /*File pointer to open the save file*/
    char separador,*filename=get_filename(contact);   /*separator is a flag, retrieves the respective filename for the recieved contact number*/
    int backspace=0;  /*backspace is an offset*/
    if((svfile=fopen(filename,"r"))==0) /*opens the respective save file*/
    {
     //   fprintf(stderr,"Failed to open save files from \"%s\".",filename); /*If opening fails, reports it*/
        return ERROR;
    }
    fseek(svfile,backspace,SEEK_END);   /*Points at the end of the file*/
    if(ferror(svfile))  /*If fseek fails, the ferror file flag is set to a non-zero number*/
    {
    //    fprintf(stderr,"Error reading on file \"%s\".",filename);   /*So if it fails, it reports it*/
        return ERROR;
    }
    for(separador=FALSE,backspace=-1;separador==FALSE;backspace=-2) /*This loop reads the characters from the end towards the beggining of the file searching for the separator that divides each message saved*/
    {
        if(fseek(svfile,backspace,SEEK_CUR)||ferror(svfile)) /*Moves the pointer back two spaeaces each timeto read the previous letter and still go backwards another character after reading it*/
        {
       fprintf(stderr,"Error reading saved files from \"%s\".",filename);  /*If an error occurs while applying the offset reports it and returns*/
        return ERROR;
        }
        if(fgetc(svfile)==SEPARADOR)   /*After every offset of position, reads the current character and compares it to the separator*/ 
            separador=TRUE;    /*If the separator is found, turns the flag on to stop he loop*/          
    }
    if(fgetc(svfile)==EOF) /*After the separator is found, the position file pointer will point at the carriage return that leaves space for easier reading, here it is read to be ignored to prevent overflowing the max character count*/
    {
        fprintf(stderr,"Error reading saved files from \"%s\".",filename); /*If an error occurs reports it*/
        return ERROR;
    }
    if((fscanf(svfile,"%[^\a]s",str)==EOF)||ferror(svfile))    /*Reads all the way to the End of File and saves it into the string provided*/
    {							      /*\a is the audible bell in the ASCII character table, nowadays it is rarely useful and it cannot be written into a message, thus the scanf reading function will read until the end of file or the non-existent audible bell ASCII, making sure it reads until the end of file*/
        fprintf(stderr,"Error reading on file \"%s\".",filename);   /*If an error occurs reports it*/
        return ERROR;
    }
    if(fclose(svfile))  /*Closes the file after reading*/
    {
        fprintf(stderr,"Error closing file \"%s\".",filename); /*If an error occurs report it*/
        return ERROR;
    }
    return TRUE;    /*If the function succeeds returns a positive value*/
}

char *get_filename(int contact)
{
    char *filename; /*Prepares the pointer*/
    switch(contact) /*Switch case matches the file save name with the contact recieved*/
    {
            case MESSI:   filename="Messi_msgs.txt";   break;
            case MOORE:   filename="Moore_msgs.txt";   break;
            case FOURIER: filename="Fourier_msgs.txt"; break;
            case BOWIE:   filename="Bowie_msgs.txt";   break;
            case NEWTON:  filename="Newton_msgs.txt";  break;
            case BATMAN:  filename="Batman_msgs.txt";  break;
    }
    return filename; /*Returns the respective filename*/
}
