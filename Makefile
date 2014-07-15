
all: .all-pre Cellphone

Cellphone: build/botonera.o build/contactos.o build/diccionario.o build/fractales.o build/iomain.o build/main.o build/mensaje.o build/musica.o build/save_file.o build/wallpaper.o
	gcc -o Cellphone build/botonera.o build/contactos.o build/diccionario.o build/fractales.o build/iomain.o build/main.o build/mensaje.o build/musica.o build/save_file.o build/wallpaper.o `pkg-config --libs allegro-5.0 allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0`

build/botonera.o: botonera.c botonera.h estructuras.h globals.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/botonera.o botonera.c

build/contactos.o: contactos.c contactos.h mensaje.h estructuras.h globals.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/contactos.o contactos.c

build/diccionario.o: diccionario.c diccionario.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/diccionario.o diccionario.c

build/fractales.o: fractales.c fractales.h botonera.h estructuras.h globals.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/fractales.o fractales.c

build/iomain.o: iomain.c iomain.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/iomain.o iomain.c

build/main.o: main.c main.h botonera.h contactos.h fractales.h iomain.h musica.h wallpaper.h estructuras.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/main.o main.c

build/mensaje.o: mensaje.c mensaje.h botonera.h diccionario.h estructuras.h globals.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/mensaje.o mensaje.c

build/musica.o: musica.c musica.h wallpaper.h estructuras.h globals.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/musica.o musica.c

build/save_file.o: save_file.c save_file.h globals.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/save_file.o save_file.c

build/wallpaper.o: wallpaper.c wallpaper.h botonera.h estructuras.h globals.h
	gcc    -c -g -I../../Documents/allegro-5.0.10/include/allegro5 `pkg-config --cflags allegro_ttf-5.0 allegro_acodec-5.0 allegro_font-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro-5.0` -o build/wallpaper.o wallpaper.c


clean:	reset
	rm -f build/*.o Cellphone
	rmdir build

.all-pre:
	@mkdir build -p
reset:
	rm -f *_msgs.txt wallp.txt
