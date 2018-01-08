// Jugador2.c 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h> 
#include <stdio.h> 
#include <unistd.h>
union semun{
 int val;
 struct semid_ds * buf;
 short int *array;
 struct seminfo *__buf;
 };
 int main(){
 key_t Llave;
 int semaforo;
 struct sembuf Proceso;
 int i = 0 ;
 Llave = ftok ("/bin/ls", 313);
 semaforo = semget (Llave, 10, 0600 | IPC_CREAT);
 semctl (semaforo, 0, SETVAL, 0);
 Proceso.sem_num = 0;
 Proceso.sem_op = 1;
 Proceso.sem_flg = 0;
 //for(i = 0 ; i < 11; i++) {
 printf("Oponente Invasor Listo..!  \n");
 semop(semaforo,&Proceso, 1);
sleep(3);
 semop(semaforo,&Proceso, 1);
	printf("   En sus marcas..!! \n");
	sleep(1);
	printf("   Listos..?! \n");
	sleep(1);
	printf("   JUEGUEN!! \n");
	sleep(1);



//}
return 0;
 }
 //fin jugador2
