# advanced-operating-systems

*****************************************************

Tecnologico de Costa Rica

Sistemas Operativos Avanzados

Tarea Corta 2: Rastreador de System Calls

*****************************************************


Guia para usar el programa:

1. Buscar el archivo ejecutable llamado shortTask2 en el directorio shortTask2/bin/
	
	#compilar el programa
	make clean; make

	cd shortTask2/bin/

2. Definir los parametros del programa en el siguiente orden:

	parameter1: <-v|-V> v minuscula para imprimir todos los logs e informacion de system calls sin pausas o V mayuscula para hacer pausas entre system calls
	
	parameter2: comando a monitorear y sus respectivos parametros separados por espacios, por ejemplo "ls -l"
	
	
3. Correr el programa con el siguiente comando en consola (por ejemplo):
	
	./shortTask2 -v ls -l
	
	
// SALIDA DEL PROGRAMA

El programa imprime un mensaje por cada system call que detecta, mostrando su respectivo codigo, nombre y cantidad de llamadas actuales para ese codigo.

Al finalizar imprime una tabla (tipo resumen) con todos los tipos de systemcalls detectados, su codigo y su cantidad de apariciones.  

