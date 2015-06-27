/*
  Autore: Giammarco René Casanova
  Classe: Fisica (L-30)
  Prof: L.M. Barone
  Data: 25/11/13
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Dimensioni dell'area di gioco
#define M 10
#define N 10
//Colpo intelligente 
#define AI 1
//Lunghezza navi
#define PORTAEREI 4
#define TORPEDINIERA 3
#define INCROCIATORE 1
//Numero di navi
#define NUM_INCR 1
#define NUM_PORT 1
#define NUM_TORP 1

//dichiarazioni di funzioni
	int win();
	void play();
	void begin(int, int);
	void print();
	int full();
	void hit(int, int);
	void write(FILE *);

//variabili globali
char fleet[N][M] = {0};
int TOT = NUM_INCR*INCROCIATORE + NUM_PORT*PORTAEREI + NUM_TORP*TORPEDINIERA;

int mains()
{	
	//dichiarazioni
	int p, q, n, m, l, ans, fires = 0, err = 0;
	FILE *fp;

	//random seed
	srand(time(0));

	printf("/nBATTAGLIA NAVALE IN C/n");

	//reset array
	for(p = 0; p<N; p++){
		for(q = 0; q<M; q++){
			fleet[p][q] = '~';
		}}

	//richiesta metodo di output all'utente
	do{
		if(err)
			printf("\n Errore! Inserire 1 per stampare il gioco su terminale o 0 per scriverlo sul file seawar.txt\n");
		else
			printf("\n Inserire 1 per stampare il gioco su terminale o 0 per scriverlo sul file seawar.txt\n");
		scanf("%d",&ans);
		err = 1;
	}while(ans != 1 && ans != 0);

	//Posizionamento delle navi
	for(n = 0; n<NUM_INCR; n++)
	begin(rand()%2, INCROCIATORE);

	for(m = 0; m<NUM_PORT; m++)
	begin(rand()%2, PORTAEREI);
	 
	for(l = 0; l<NUM_TORP; l++)
	begin(rand()%2, TORPEDINIERA);

	if(ans){
	 printf("\n Istruzioni: il simbolo ~ indica una porzione di mare vuota, il carattere O rappresenta una nave, il + raffigura un colpo andato a vuoto(acqua) mentre una X simboleggia un colpo andato a segno");
     printf("\n Posizionamento navi\n");
	 print();
	}else{
    fp = fopen("seawar.txt", "a");
	fprintf(fp,"\nNUOVA PARTITA\n------------------------------\n");
	fprintf(fp,"\n Istruzioni: il simbolo ~ indica una porzione di mare vuota, il carattere O rappresenta una nave, il + raffigura un colpo andato a vuoto(acqua) mentre una X simboleggia un colpo andato a segno");
	fprintf(fp, "\nPosizionamento Navi\n");
	write(fp);
	}

	if(ans)
	 printf("\n Inizio del gioco\n");
	else
	 fprintf(fp,"\n Inizio del gioco\n");

	//ciclo di gioco
	do{
		play();
		 fires++;

	   if(ans)
	  print();
	   else
	  write(fp);

	}while(fires < 50 && !win());
	
	if(ans){
	 printf("\n Fine del gioco\n");
	}
	else
	{
	 fprintf(fp,"\n Fine del gioco\n");
	 fclose(fp);
	}

	//Determina la vincita
	if(win()){

		if(ans)
		 printf("\n Vittoria\n");
		else
		 fprintf(fp,"\n Vittoria\n");

	}else{

		if(ans)
		 printf("\n Sconfitta\n");
		else
		 fprintf(fp,"\n Sconfitta\n");
	}

	return 0;
}
//determina se tutte le navi sono state abbattute
int win(){
	int p, q, count = 0;

	for(p = 0; p<N; p++){
		for(q = 0; q<M; q++){
			if(fleet[p][q] == 'X')
				count++;
		}}

	if(count == TOT)
		return 1;
	else
		return 0;
}
//'spara' il colpo
void play(){

	int X, Y;
	int times = 0;

	//colpo random
		do{
	X = rand()%N;
    Y = rand()%M;
		}while((fleet[X][Y] == 'X' || fleet[X][Y]== '+') && !win() && !full());

	   if(fleet[X][Y] == 'O'){
		   fleet[X][Y] = 'X';
#ifdef AI
		   hit(X,Y);
#endif
	     } else if(fleet[X][Y] == '~'){
		   fleet[X][Y] = '+';
	     }
}
//Colpo 'intelligente'
void hit(int X, int Y){
	//dichiarazioni
	int X_n = 0, Y_n = 0;
	
	if(fleet[X+1][Y] == '~' || fleet[X][Y+1] == '~' || fleet[X-1][Y] == '~' || fleet[X][Y-1] == '~' || fleet[X+1][Y] == 'O' || fleet[X][Y+1] == 'O' || fleet[X-1][Y] == 'O' || fleet[X][Y-1] == 'O'){
	  do{
		  printf("\n Colpo intelligente\n");
		  //Sleep(5000);

       switch(rand()%4){
		   case 0:
			   X_n = X+1;
			   Y_n = Y;
			   break;
		   case 1:
			   X_n = X-1;
			   Y_n = Y;
			   break;
		   case 2:
			   X_n = X;
			   Y_n = Y+1;
			   break;
		   case 3:
			   X_n = X;
			   Y_n = Y-1;
	   }	   
	 }while(fleet[X_n][Y_n] != '~' && fleet[X_n][Y_n] != 'O');

		  if(fleet[X_n][Y_n] == 'O'){
		   fleet[X_n][Y_n] = 'X';
		   hit(X_n,Y_n); //ricorsione
	     } else {
		   fleet[X_n][Y_n] = '+';
	     }
	}
}
//verifica se la tabella è piena
int full(){
    int p, q, count = 0;

	for(p = 0; p<N; p++){
		for(q = 0; q<M; q++){
			if(fleet[p][q] != '~')
				count++;
		}}

	if(count >= 50){
		printf("\n Colpi terminati!\n");
		return 1;
	}else{
	    return 0;
	}
}
//stampa la tabella
void print(){
	int p, q, i = 0;

	printf("\n  0    1    2    3    4    5    6    7    8    9 \n ________________________________________________\n\n");
	for(p = 0; p<N; p++){
		for(q = 0; q<M; q++){
			printf("  %c  ",fleet[p][q]);
		}
	printf(" |%d \n\n",i++);
	}
}
void write(FILE *fp){
	int p, q, i = 0;

	fprintf(fp, "\n  0    1    2    3    4    5    6    7    8    9 \n ________________________________________________\n\n");

	for(p = 0; p<N; p++){
		for(q = 0; q<M; q++){
			fprintf(fp, "  %c  ",fleet[p][q]);
		}
	fprintf(fp, " |%d \n\n", i++);
	}
}
//posiziona in modo pseudo-casuale le varie navi
void begin(int vertical, int lenght){
	int X, Y, k, i;
	int available = 1;

 if(vertical){ //posiziona le navi in verticale o in orizzontale in base ad una scelta randomica

	  do{
	  X = rand()%N;
      Y = rand()%M;
	    for(k = 0; k<lenght; k++){
			if(fleet[X+k][Y] == 'O')
				available = 0;
			else
				available = 1;
		}
	  } while(fleet[X][Y] != '~' || (N-X<lenght) || !available);

	  for(i = 0; i<lenght; i++) fleet[X+i][Y] = 'O';

 } else {

	  do{
	  X = rand()%N;
      Y = rand()%M;
	    for(k=0; k<lenght; k++) {

			if(fleet[X][Y+k] == 'O')
				available = 0;
			else
				available = 1;
		}

	  }while(fleet[X][Y] != '~' || (M-Y<lenght) || !available);

	  for(i = 0; i<lenght; i++) 
		  fleet[X][Y+i] = 'O';
	}
}