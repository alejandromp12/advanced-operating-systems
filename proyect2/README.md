# advanced-operating-systems

*****************************************************

Tecnologico de Costa Rica

Sistemas Operativos Avanzados

Proyecto 2: Memoria Compartida - Productor Consumidor

*****************************************************


Guia para usar el programa:

1. Buscar los archivos ejecutables llamados creator, producer, consumer y terminator en el directorio src/bin/
	
	#instalar dependencias
	sudo apt-get install libgtk-3-dev
	
	#compilar el programa
	make clean; make

	cd src/bin/

2. Definir los parametros del programa en el siguiente orden:

	creator: <bufferID> <numeroEspacios> , i.e. creator 0 5 significa crear el buffer 0 con 5 espacios de mensaje.

	producer: <bufferID> <tiempoPromedioEnviar> , i.e. producer 0 3 significa enviar mensajes al buffer 0 cada 3 segundos en promedio con distribucion exponencial.

	consumer: <bufferID> <tiempoPromedioLeer> , i.e. consumer 0 3 significa leer mensajes del buffer 0 cada 3 segundos en promedio con distribucion exponencial.

	destructor: <bufferID> , i.e. destructor 0 significa destruir el buffer 0 y terminar los productores y consumidores asociados. 
	
	
3. Correr el programa con el siguiente comando en varias consolas (por ejemplo):
	
	./src/bin/creator 0 5

	./src/bin/producer 0 3

	./src/bin/consumer 0 4

	./src/bin/terminator 0
	
	
*********************** SALIDA DEL PROGRAMA *********************

El programa identifica cada buffer con un numero (por ejemplo: Buffer 1)

La salida del programa es una interfaz de usuario minimalista en la cual la parte superior muestra una tabla donde cada columna es un espacio del buffer y cada fila despliega un dato del mensaje.

Cada fila se compone de:
	* Index
	* Process ID
	* Message Date
	* Message Key

La parte inferior de la GUI muestra una bitacora de tipo srollable en la cual se despliega el log con eventos y estadisticas del buffer y sus procesos relacionados.

Los programas finalizan cuando el usuario llama al terminator, el cual termina los demas procesos asociados al buffer indicado dejando solo el creador vivo que actualmente mostraria la interfaz de usuario. 


