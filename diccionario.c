#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diccionario.h"

FILE * abrir_letra_dic(char *str)
{
    FILE *fp;						/*Abre el diccionario en modo lectura.*/
    fp = fopen(str,"r"); // abre el archivo correspondiente a la letra con que empieza la palabra y el largo
    return fp;
}

int esletra(char *c)					/*Verifica si los caracteres son letras.*/
{
    if(('a'<=(*c) && (*c) <= 'z') ||('A'<=(*c) && (*c) <= 'Z'))
		return 1;
	else
		return 0;
}

int contilde(unsigned char *c)					/*Verifica si los caracteres son letras.*/
{	
    if(*c==0xC3)
        return 1;
    else 
        return 0;
}

char sacarmayus(unsigned char c) // quita las mayusculas,l devolviendo el char en ucase
{
    if('A'<=c && c <= 'Z')
    {
        c+=0x20;
    }
    if(c==0xC3){// si es una mayuscula acentuada
        switch(c){ 
            case 0x81: (c)=0xA1;break; //paso de mayus a minus
            case 0x89: (c)=0xA9;break;
            case 0x8D: (c)=0xAD;break;
            case 0x93: (c)=0xB3;break;
            case 0x9A: (c)=0xBA;break;
        }
    }
    return c;
}

int separarenpalabras(char bigbuffer[], char palabras[][15], int limites[][2])
{
    unsigned int inicio, i=0, j=0, k=0;
    int tilde=0;
    	while(bigbuffer[j] != '\0')						/*Hasta encontrar el terminador del mensaje se busca cargar todas las*/
	{									/*palabras del mismo en arreglos bidimensionales y para cada una se*/
            inicio = j;							/*obtienen posición de inicio y de fin de la palabra.*/
            while(esletra(&bigbuffer[j]) || (tilde=contilde(&bigbuffer[j])))
            {
                
                limites[i][0] = inicio;
                if(tilde){
                    palabras[i][k++] = sacarmayus(bigbuffer[j++]);
                    tilde=0;
                }
                    palabras[i][k++] = sacarmayus(bigbuffer[j]);
                    j++;
                    limites[i][1] = j;
                    palabras[i][k] = '\0';					/*La palabra debe finalizar en el terminador /0.*/
                    if(k>13)
                        break;
            }
            while(!esletra(&bigbuffer[j]) && (bigbuffer[j] != '\0'))	/*Si los caracteres no son letras se avanza la posición en el mensaje(j).*/
                j++;
            i++;
            k=0;
	}
    if(esletra(&bigbuffer[j]))
        j++;
    k=strlen(palabras[i-1]);
    if(sacarmayus(bigbuffer[j-1])==palabras[i-1][k-1])
        return i-1;
    else
        return i;
}

char sacartilde(unsigned char letra)
{
    switch(letra){// saco tilde de una vocal acentuada
        case 0xA1:letra='a';break; //paso de mayus a minus
            case 0xA9:letra='e';break;
            case 0xAD:letra='i';break;
            case 0xB3:letra='o';break;
            case 0xBA:letra='u';break;
    }
    return letra; // devuelvo letra sin tilde
}

int errorpalabra(char word[15])
{// encuentra si la palabra ingresada word esta en el diccionario
    char filename[10]; char aux[3]; char c; int i=0;
    FILE *archivo; int flag=0;int largo=strlen(word);
    char primerletra;
    while(word[i]){ // si tiene acentos resto el byte extra de UTF8
        if((unsigned char)word[i]==0xC3)
            largo--;
        i++;
    }
    if((unsigned char)word[0]==0xC3) // si tiene acentos saco la tilde de la primera letra
        primerletra=sacartilde(word[1]);
    else
        primerletra=word[0];
    
    i=0;
    sprintf(filename,"dic/%c%d.txt", primerletra, largo); // abro el archivo correspondiente a las palabras del español que empiezan con la letra y el largo de la palabra a analizar
    archivo=abrir_letra_dic(filename);// abro archivo
    if(archivo==NULL)
        return 1;// si no hay palabras de ese formato error
    while((c=fgetc(archivo))!= EOF){ // recorro el archivo con palabras de diccionario
        while(c!=0x0D){// hasta carriage feed
            if(word[i]!=c) // si la palabra es diferente
                flag=1;//guardo en flag esa diferencia
            i++;
            c=fgetc(archivo); // siguiente letra
        }
        fgetc(archivo); // me saco de encima el newline
        if(!flag){ // si se encontró la palabra en el dicc
            fclose(archivo);
            return 0;// para el espacio vacío
        }
        flag=0;
        i=0;
    }
    fclose(archivo);
    return 1; // si no se encontro en todas las palabras 
}

int marcar_errores(char bigbuffer[], int errores[][2])				/*La función devuelve el posición de letras en el mensajes entre*/
{										/*las cuales se encuentra un error.*/
    char  palabras[25][15];                                                 /*El arreglo palabras considera que el mensaje no contiene más de*/
    unsigned int cantidad, i=0, errors=0;
    int limites[25][2];
    cantidad=separarenpalabras(bigbuffer, palabras, limites);               /*Se guarda la cantidad de palabras. y separo bigbuffer en palabras*/
    for(i=0;i<cantidad;i++)
    {
        if(errorpalabra(palabras[i])){ // analizo todas las palabras
            errores[errors][0]=limites[i][0]; // devuelvo en que letra del bigbuffer empieza el error
            errores[errors][1]=limites[i][1]; // y en que letra termina
            errors++; 
        }
    }
    return errors; // devuelve cantidad de errores
}