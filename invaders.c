#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include "invaderstructs.h"
#include <sys/sem.h>
#include <sys/ipc.h>
#include <pthread.h>
#define MAX_BOMBS 1000
#define SHSIZE 2

  /*---SEMAFOROS--------*/
  key_t Llave; 
  int semaforo; 
  int cantidadJugadores;
  
  struct sembuf Proceso; 
  struct sembuf Proceso2; 

  /*-----MEMORIA COMPARTIDA------*/       
	int *shm = 0;
	int *s = 0;
        int shmid;
  /*-----GLOBALES JUEGO----------*/
  void menu(struct options *settings);
  void gameover(int win);
  void *reloj(void *msg);
  int defensor();
  int invasor();
  

union semun{ 
  int val; 
  struct semid_ds * buf; 
  short int *array; 
  struct seminfo *__buf; 
}; 


int main() {
  char jugador;
  char player;
  cantidadJugadores = 0;

  //semaforo

	Llave = ftok ("/bin/ls", 313); 
 	semaforo = semget (Llave, 10, 0600 | IPC_CREAT);
 	semctl (semaforo, 0, SETVAL, 0); 
 
  //end semafo

		//compartiendo memoria
	 		key_t key;
		
			key = ftok ("/bin/ls", 125);;
		
			shmid = shmget(key, sizeof(int)*3, IPC_CREAT | 0777);
			
			if(shmid < 0)
			{
			   perror("shmget");
				exit(1);
			}
			shm = (int *)shmat(shmid, NULL, 0);

			if(shm == (int *) -1)
			{
				perror("shmat");
					exit(1);
			}
	
					

			 s = shm;
		// [0] : inicio
		// [1] : score
		// [3] : timer
	 	//fin share memory


  initscr();

  mvaddstr(0,1, "-------------------------------------------------------------------------------");
  refresh();
  mvaddstr(1,1,   ":::::::::::::::::::::::SPACE::::::::::::::::::INVADERS:::::::::::::::::::::::::");
  refresh();

  mvaddstr(2,1,   "-------------------------------------------------------------------------------");
  refresh();

  mvaddstr(3,(COLS/2)-12, "SELECCIONE: A. Defensor");
  refresh();

  mvaddstr(4,(COLS/2), "B. Invasor");
  refresh();

  mvaddstr(5,1,"------------------------------------------------------------------------------");
  refresh();

  mvaddstr(7,1,  "------------------------------------------------------------------------------");
  refresh();
 
  move(12,0);
  refresh();
 
        player = getch();
	refresh();    

		  if(player == 'A'){


					*shm = shm[0] + 1;

		 			//printf("%d",shm[0]);
					//refresh();
					defensor();
			
		  }
		  if(player == 'B'){

					*shm = shm[0] + 1;
		 			//printf("%d",shm[0]);
					//refresh();
					invasor();
		  }
		 
  
  	endwin();		

	shmdt ((char *)shm);
	shmctl (shmid, IPC_RMID, 0); //se limpia la memoria compartida


  return 0;
 
}

int invasor(){

   clear();
   echo();
   nocbreak();
   nodelay(stdscr,0);
	

	       //compartiendo memoria para semaforo inicial
		       //printf("VALOR DEL Shm INVASOR : %d", *shm);
               		
			
			if(*shm==1){
 			
				mvaddstr(9,1,"Invasor, esperando al oponente defensor...");
			 	refresh();
			
				Proceso.sem_num = 0; 
	 			Proceso.sem_op  = -1;
	 			Proceso.sem_flg = 0;
	    			
		 		semop (semaforo, &Proceso, 1);
				
			}
			Proceso.sem_num = 0; 
 			Proceso.sem_op  = 1;
 			Proceso.sem_flg = 0;
    			
	 		semop (semaforo, &Proceso, 1);
		//fin share memory and semaphore
	 	
		move(10,1);
		refresh();
		
	    	printf("   En sus marcas..!! \n");
		sleep(1);
		printf("   Listos..?! \n");
		sleep(1);
		printf("   A JUGAR!!! \n");
		sleep(1);

/*-------SCREEN SETTINGS--------*/
   initscr();
   clear();
   noecho();
   cbreak();
   nodelay(stdscr,1);
   keypad(stdscr,1);
   srand(time(NULL)); 
refresh();

char tellscore[30];
char telltimer[30];
/*-------SCREEN-------------------*/
   mvaddstr(0,(COLS/2)-9, "Space Invaders Sopes 1");
	
   move (0,1);
   addstr("SCORE OPONENT: ");
   move(0,COLS-19);
   addstr("Q = Salir");
   refresh();

	while(1){//PARA SALIR DEL JUEGO
		
	      sprintf(tellscore, "%d", shm[1]);  //SCORE
	      mvaddstr(0,15, tellscore);
	      		refresh();

	      sprintf(telltimer, "%d", shm[2]); // TIMER
	      mvaddstr(0,(COLS/2)+15, telltimer);
	      move(2,0);		
	      refresh();
	      
	     
		if(*shm==3)
		{
			break;
		}
	      }


	
 
return 0;
}

int defensor(){
 /*---------varibles pantalla---*/
   clear();
   echo();
   nocbreak();
   nodelay(stdscr,0);
	

	       //compartiendo memoria para semaforo inicial
		 //  	printf("VALOR DEL Shm DEFENSOR : %d", *shm);
  //semaforo


	if(*shm==1){
	 	Proceso.sem_num = 0; 
	 	Proceso.sem_op = -1;
	 	Proceso.sem_flg = 0;
	 

	  	mvaddstr(9,1,"Defensor, esperando al oponente invasor....");
	  	refresh();
	       
	 	semop (semaforo, &Proceso, 1);
	}
	Proceso.sem_num = 0; 
 	Proceso.sem_op = 1;
 	Proceso.sem_flg = 0;
 	semop (semaforo, &Proceso, 1);
  //fin semaforo
	move(10,1);
	refresh();

    	printf("   En sus marcas..!! \n");
	sleep(1);

	printf("   Listos..?! \n");
	sleep(1);

	printf("   JUEGUEN!! \n");
	sleep(1);


	   //Creating share memory parameters


	   //Ending share memory paramet


   
  /*-------SCREEN--------*/
   initscr();
   clear();
   noecho();
   cbreak();
   nodelay(stdscr,1);
   keypad(stdscr,1);
   srand(time(NULL)); 


 /*------variables JUEGO------*/

   struct player tank;
   struct alien aliens[30];
   struct shoot shot[3];
   struct bomb bomb[MAX_BOMBS];
   struct options settings;
   unsigned int input, loops=0, i=0, j=0, currentshots=0,      currentbombs=0, currentaliens=30;
   int random=0, score=0, win=-1;
   int timer = 0;
   
   char punteo[5] = "0000";
   char tellscore[30];
   char telltimer[30];
   shm[1] = score;//compartiendo punteo
   shm[2] = timer;//compartiendo tiempo

   char *msg ="time";
  //----------------------------TIMER---------------------
	sleep(1);
	pthread_t thread1;
	pthread_create(&thread1, NULL, reloj, (void *) msg);
  //-----------------END TIMER-------------------------

  
   /* Set default settings */
   settings.overall = 15000;
   settings.alien = 12;
   settings.shots = 3;
   settings.bombs = 10;
   settings.bombchance = 1;

   /* Set tank settings */
   tank.r = LINES - 1;
   tank.c = COLS / 2;
   strcpy(tank.ch,"<--->");

   /* Set aliens settings */
   for (i=0; i<10; ++i) {
      aliens[i].r = 1;
      aliens[i].c = i*3;
      aliens[i].pr = 0;
      aliens[i].pc = 0;
      aliens[i].ch = 'V';
      aliens[i].alive = 1;
      aliens[i].direction = 'r';
   }
   for (i=10; i<20; ++i) {
      aliens[i].r = 2;
      aliens[i].c = (i-10)*3;
      aliens[i].pr = 0;
      aliens[i].pc = 0;
      aliens[i].ch = 'V';
      aliens[i].alive = 1;
      aliens[i].direction = 'r';
   }
   for (i=20; i<30; ++i) {
      aliens[i].r = 3;
      aliens[i].c = (i-20)*3;
      aliens[i].pr = 0;
      aliens[i].pc = 0;
      aliens[i].ch = 'V';
      aliens[i].alive = 1;
      aliens[i].direction = 'r';
   }
   
   /* Set shot settings */
   for (i=0; i<3; ++i) {
      shot[i].active = 0;
      shot[i].ch = '*';
   }
   
   /* Set bomb settings */
   for (i=0; i<MAX_BOMBS; ++i) {
      bomb[i].active = 0;
      bomb[i].ch = '*';
      bomb[i].loop = 0;
   }
   
   /* Display game title,score header,options */
  // move(0,(COLS/2)-9);
  // addstr("--SPACE INVADERS--");
   mvaddstr(0,(COLS/2)-9, "Space Invaders Sopes 1");
	
   move(0,1);
   addstr("SCORE: ");

   move(0,COLS-19);
   addstr("Q = Salir");
   
   
   		

   while(1) {
      /* Show score */
      sprintf(tellscore, "%d", score);
      move(0,8);
      addstr(tellscore);
      
      /* Move tank */
      move(tank.r,tank.c);
      addstr(tank.ch);
      
      /* Move bombs */
      if (loops % settings.bombs == 0)
      for (i=0; i<MAX_BOMBS; ++i) {
         if (bomb[i].active == 1) {
            if (bomb[i].r < LINES) {
               if (bomb[i].loop != 0) {
                  move(bomb[i].r-1,bomb[i].c);
                  addch(' ');
               }
               else
                  ++bomb[i].loop;
               
               move(bomb[i].r,bomb[i].c);
               addch(bomb[i].ch);
               
               ++bomb[i].r;
            }
            else {
               bomb[i].active = 0;
               bomb[i].loop = 0;
               --currentbombs;
               move(bomb[i].r - 1,bomb[i].c);
               addch(' ');
            }
         }
      }
      
      /* Move shots */
      if (loops % settings.shots == 0)
      for (i=0; i<3; ++i) {
         if (shot[i].active == 1) {
            if (shot[i].r > 0) {
               if (shot[i].r < LINES - 2) {
                  move(shot[i].r + 1,shot[i].c);
                  addch(' ');
               }
               
               for (j=0; j<30; ++j) {
                  if (aliens[j].alive == 1 && aliens[j].r == shot[i].r && aliens[j].pc == shot[i].c) {
                
		     score += 10; //se suman 2
		     shm[1] = score;	
                     aliens[j].alive = 0;
                     shot[i].active = 0;
                     --currentshots;
                     --currentaliens;
                     move(aliens[j].pr,aliens[j].pc);
                     addch(' ');
                     break;
                  }
               }
               
               if (shot[i].active == 1) {
                  move(shot[i].r,shot[i].c);
                  addch(shot[i].ch);
                  
                  --shot[i].r;
               }
            }
            else {
               shot[i].active = 0;
               --currentshots;
               move(shot[i].r + 1,shot[i].c);
               addch(' ');
            }
         }
      }     
      
      /* Move aliens */
      if (loops % settings.alien == 0)
      for (i=0; i<30; ++i) {
         if (aliens[i].alive == 1) {
            move(aliens[i].pr,aliens[i].pc);
            addch(' ');
            
            move(aliens[i].r,aliens[i].c);
            addch(aliens[i].ch);
            
            aliens[i].pr = aliens[i].r;
            aliens[i].pc = aliens[i].c;
            
            /* Check if alien should drop bomb */
            random = 1+(rand()%100);
            if ((settings.bombchance - random) >= 0 && currentbombs < MAX_BOMBS) {
               for (j=0; j<MAX_BOMBS; ++j) {
                  if (bomb[j].active == 0) {
                     bomb[j].active = 1;
                     ++currentbombs;
                     bomb[j].r = aliens[i].r + 1;
                     bomb[j].c = aliens[i].c;
                     break;
                  }
               }
            }
            
            /* Set alien's next position */
            if (aliens[i].direction == 'l')
               --aliens[i].c;
            else if (aliens[i].direction == 'r')
               ++aliens[i].c;
               
            /* Check alien's next positions */
            if (aliens[i].c == COLS - 2) {
               ++aliens[i].r;
               aliens[i].direction = 'l';
            }
            else if (aliens[i].c == 0) {
               ++aliens[i].r;
               aliens[i].direction = 'r';
            }
         }
      }
      
      
      /* See if game has been won or lost*/
      if (currentaliens == 0) {
         win = 1;
         break;
      }
	if (score == 100) {
         win = 1;
         break;
      }
      for (i=0; i<30; ++i) {
         if (aliens[i].r == LINES-1) {
            win = 0;
            break;
         }
      }
      for (i=0; i<MAX_BOMBS; ++i) {
         if (bomb[i].r == tank.r && bomb[i].c == tank.c) {
            win = 0;
            break;
         }
      }    
      
      move(0,COLS-1);
      refresh();
      usleep(settings.overall);
      ++loops;
      
      input = getch();
      move(tank.r,tank.c);
      addch(' ');
      
      /* Check input */
      if (input == 'q')
         win = 2;
      else if (input == KEY_LEFT)
         --tank.c;
      else if (input == KEY_RIGHT)
         ++tank.c;
      else if (input == ' ' && currentshots < 3) {
         for (i=0; i<3; ++i) {
            if (shot[i].active == 0) {
               shot[i].active = 1;
               ++currentshots;
              // --score;
               shot[i].r = LINES - 2;
               shot[i].c = tank.c;
               break;
            }
         }
      }
      
      
      if (win != -1)
         break;
      
      /* Check for valid tank position */
      if (tank.c > COLS-2)
         tank.c = COLS - 2;
      else if (tank.c < 0)
         tank.c = 0;     
   }
   *shm = shm[0] + 1;
   gameover(win);
   endwin();

   return 0;
}


/* This function handles displaying the win/lose screen */
void gameover(int win) {

   nodelay(stdscr, 0);
   
   if (win == 0) {
      clear();
      move((LINES/2)-1,(COLS/2)-5);
      addstr("YOU LOSE!");
      move((LINES/2),(COLS/2)-11);
      addstr("PRESS ANY KEY TO EXIT");
      move(0,COLS-1);
      refresh();
      getch();
   }
   
   else if (win == 1) {
      clear();
      move((LINES/2)-1,(COLS/2)-5);
      addstr("YOU WIN!");
      move((LINES/2),(COLS/2)-11);
      addstr("PRESS ANY KEY TO EXIT");
      move(0,COLS-1);
      refresh();
      getch();
   }
}
void *reloj(void *myvar){
   char telltimer[30];   

	for(int i = 0; i< 100; i++){
	shm[2] ++;

	sleep(1);
              sprintf(telltimer, "%d", shm[2]);
	      mvaddstr(0,(COLS/2)+15, telltimer);
	      refresh();
		if(*shm==3)
		{
			
		}
		
	}
   	
}




