# advanced-operating-systems

*****************************************************

Tecnologico de Costa Rica

Sistemas Operativos Avanzados

Tarea 1: Sincronizacion de threads

*****************************************************


Guia para usar el programa:

1. Buscar el archivo ejecutable llamado shortTask1 en el directorio shortTask1/bin/

	En caso de no existir este archivo, ejecutar en consola:

	#crear directorio bin
	mkdir -p shortTask1/bin/ 
	
	#compilar el programa
	make

	cd shortTask1/bin/

2. Definir los parametros del programa en el siguiente orden:

	parametro1: cantidad de carros de Este a Oeste
	
	parametro2: cantidad de carros de Oeste a Este
	
3. Correr el programa con el siguiente comando en consola:
	
	./shortTask1 parametro1 parametro2
	
	
// SALIDA DEL PROGRAMA

El programa identifica cada carro con un numero (por ejemplo: Car 1)

La salida del programa es una serie de mensajes para ilustrar lo que esta pasando.

El numero maximo de carros en el puente es: 3 carros

Tipos de mensaje:

	* La creacion de un carro y su respectiva direccion
	* Cuando un carro llega al puente
	* Cuando un carro espera para entrar al puente
	* Cuando un carro entra al puente
	* El numero actual de carros cruzando el puente
	* Cuando un carro sale del puente
	* El final del programa


