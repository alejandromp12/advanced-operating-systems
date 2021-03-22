# advanced-operating-systems

*****************************************************

Tecnologico de Costa Rica

Sistemas Operativos Avanzados

Proyecto 1: Loteria de Threads

*****************************************************


Guia para usar el programa:

1. Buscar el archivo ejecutable llamado lottery_threads en el directorio src/bin/
	
	#compilar el programa
	make clean; make

	cd src/bin/

2. Definir los parametros del programa en el siguiente orden:

	parameter1: <-m|--mode> <expropriation_type> , 0 for expropriation or 1 for non-expropriation.
	
	parameter2: <-t|--threads> <num_threads:int> , integer of the total number of threads, greater than 5.
	
	parameter3: -t1 <num_ticket:int> <workload:int> <quantum:int> , thread 1: number of tickets, workload units (1 unit = 50 terms), quantum.
	
			quantum: if the program is in expropriation mode the quantum must be in milliseconds
					 if the program is in non-expropriation mode, the quantum must be a percentage of the total work
					 
	parameterN: -tN <num_ticket:int> <workload:int> <quantum:int>, rest of the threads. 
	
	
3. Correr el programa con el siguiente comando en consola (por ejemplo):
	
	./src/bin/lottery_threads -m 0 -t 5 t1 4 200 4  t2 5 300 4 t3 5 100 4 t4 2 70 2 t5 2 50 1
	
	
// SALIDA DEL PROGRAMA

El programa identifica cada thread con un numero (por ejemplo: Thread 1)

La salida del programa es una interfaz de usuario minimalista en la cual la primera fila corresponde al progreso total y el resto de filas son para threads individuales.

Cada fila se compone de:
	* Titulo del thread "Thread N"
	* Approximacion de Pi mas reciente
	* Barra de progreso del thread N
	* Porcentaje del progreso del thread N

El programa finaliza cuando el usuario cierra la interfaz de usuario.


