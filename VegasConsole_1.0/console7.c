#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

/**
	init_terminal() and read_terminal_properties() are taken from https://github.com/fhd/least/blob/master/least.c
*/
#define ESC 27
#define SPINS_LAST 20
int spins[] = {830,283,182,131,44,713,440,248,49,252,220,342,330,36,764,190,104,994,265,250,310,57,529,294,34,837,153,156,663,299,246};
static const char* symbols="9XJQKA7";
static const int VAR=7;
void backspace(int s,int n);
int chaine();
int odds(int);
void fill(int);
void print_intro();
void fillWith(int n,char c);
struct timespec diff(struct timespec start, struct timespec end);
void print_symbol(char s, int offsetx, int offsety);
int terminal_rows, terminal_cols, display_rows;

int main(char* argv,int argc){
	
	initscr();
	cbreak;
	noecho();
	refresh();
	int turn = 1;
	
	print_intro();
	refresh();

	int i =0;
	char status = 'w';
	int score = 80;
	mvprintw(27,64,"score : %d credits   ",score);
	print_symbol('7',17,0);
	print_symbol('7',17,20);
	print_symbol('7',17,40);
	refresh();
	
	read(STDIN_FILENO,&status,1);
	
	srand((unsigned int) time(NULL));
	
	while(status!='q'&&score<500&&score>0){
		
		score-=1;
		int winning = 0;
		
		mvprintw(27,64,"score : %d credits   ",score);
		refresh();
		
		for(i=0;i<=SPINS_LAST;i++){

		
			int reel1 = odds(rand()%spins[i%70]);
			int reel2 = odds(rand()%spins[i%70]);
			int reel3 = odds(rand()%spins[i%70]);
			
			if(i==2){
				mvprintw(26,64,"                       ");
			}
			print_symbol(symbols[reel1],17,0);
			print_symbol(symbols[reel2],17,20);
			print_symbol(symbols[reel3],17,40);
			
			if(i==SPINS_LAST){
				if(symbols[reel1]==symbols[reel2] && symbols[reel2]==symbols[reel3%7]){
					switch(symbols[reel1%7]){case '9':	winning=8;	break;case 'X':	winning=16;	break;case 'J':	winning=32;	break;case 'Q':	winning=64;	break;case 'K':	winning=128;	break;case 'A':	winning=256;	break;case '7':	winning=512;	break;	}
				}else if(symbols[reel1]==symbols[reel2]){
					switch(symbols[reel1%7]){	case '9':	winning=0;	break;	case 'X':	winning=0;	break;	case 'J':	winning=1;	break;	case 'Q':	winning=2;	break;	case 'K':	winning=4;	break;	case 'A':	winning=8;	break;	case '7':	winning=16;	break;	}
				}else{	winning=0;	}
				score += winning;
				
				mvprintw(26,64,"Game pays %d (%d)",winning,turn);
				refresh();
				
				turn +=1;
				turn %=999;
				
			}else{
				refresh();
				struct timespec *start = malloc(sizeof(struct timespec));
				struct timespec *end = malloc(sizeof(struct timespec));
				struct timespec t1;
				clock_gettime(CLOCK_REALTIME,start);
				do{
					clock_gettime(CLOCK_REALTIME,end);
					t1 = diff(*start,*end);
					
				}while(t1.tv_nsec<50000000);
			}
		}
		read(STDIN_FILENO,&status,1);
		usleep(1000*300);
		
	}	
	endwin();
	printf("\n\nYour final score is %d\nThank you for playing\n",score);
	return 0;
}
int odds(int seed){
	
	int t = seed%70;
	if(t<=10){
		return 0;
	}
	if(t<=20){
		return 1;
	}
	if(t<=30){
		return 2;
	}
	if(t<=40){
		return 3;
	}
	if(t<=50){
		return 4;
	}
	if(t<=60){
		return 5;
	}
	if(t<=70){
		return 6;
	}
	return 0;
}
struct timespec diff(struct timespec start, struct timespec end){
	struct timespec temp;
	if ((end.tv_nsec - start.tv_nsec) < 0) 
	{
			temp.tv_sec = end.tv_sec - start.tv_sec - 1;
			temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	} 
	else 
	{
			temp.tv_sec = end.tv_sec - start.tv_sec;
			temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}
void print_intro(){
	printw("CONSOLE SEVENS : Las Vegas at Home\n\n");
	printw("7 7 7  512\n");
	printw("A A A  256\n");
	printw("K K K  128\n");
	printw("Q Q Q  64 \n");
	printw("J J J  32 \n");
	printw("X X X  16 \n");
	printw("7 7 -  16 \n");
	printw("9 9 9  8  \n");
	printw("A A -  8  \n");
	printw("K K -  4  \n");
	printw("Q Q -  2  \n");
	printw("J J -  1  \n");
	printw("press (q) to quit\n");
	printw("press any other key to spin\n");
	printw("\n");
}
void fill(int n){
	int g = 0;
	for(g=1;g<=n;g++)
	printw(" ");
}
void fillWith(int n,char c){
	int g = 0;
	for(g=1;g<=n;g++)
	printw("%c",c);
}	
void print_symbol(char s, int offsety, int offsetx){
	switch( s ){
		case '7':
			mvprintw(offsety+0,offsetx," .----------------.   ");
			mvprintw(offsety+1,offsetx,"| .--------------. |  ");
			mvprintw(offsety+2,offsetx,"| |   _______    | |  ");
			mvprintw(offsety+3,offsetx,"| |  |  ___  |   | |  ");
			mvprintw(offsety+4,offsetx,"| |  |_/  / /    | |  ");
			mvprintw(offsety+5,offsetx,"| |      / /     | |  ");
			mvprintw(offsety+6,offsetx,"| |     / /      | |  ");
			mvprintw(offsety+7,offsetx,"| |    /_/       | |  ");
			mvprintw(offsety+8,offsetx,"| |              | |  ");
			mvprintw(offsety+9,offsetx,"| '--------------' |  ");
			mvprintw(offsety+10,offsetx," '----------------'   ");
			break;
		case 'K':          
			mvprintw(offsety+0,offsetx," .----------------.   ");
			mvprintw(offsety+1,offsetx,"| .--------------. |  ");
			mvprintw(offsety+2,offsetx,"| |  ___  ____   | |  ");
			mvprintw(offsety+3,offsetx,"| | |_  ||_  _|  | |  ");
			mvprintw(offsety+4,offsetx,"| |   | |_/ /    | |  ");
			mvprintw(offsety+5,offsetx,"| |   |  __'.    | |  ");
			mvprintw(offsety+6,offsetx,"| |  _| |  \\ \\_  | |  ");
			mvprintw(offsety+7,offsetx,"| | |____||____| | |  ");
			mvprintw(offsety+8,offsetx,"| |              | |  ");
			mvprintw(offsety+9,offsetx,"| '--------------' |  ");
			mvprintw(offsety+10,offsetx," '----------------'   ");
			break;
		case 'A':
			mvprintw(offsety+0,offsetx," .----------------.   ");
			mvprintw(offsety+1,offsetx,"| .--------------. |  ");
			mvprintw(offsety+2,offsetx,"| |      __      | |  ");
			mvprintw(offsety+3,offsetx,"| |     /  \\     | |  ");
			mvprintw(offsety+4,offsetx,"| |    / /\\ \\    | |  ");
			mvprintw(offsety+5,offsetx,"| |   / ____ \\   | |  ");
			mvprintw(offsety+6,offsetx,"| | _/ /    \\ \\_ | |  ");
			mvprintw(offsety+7,offsetx,"| ||____|  |____|| |  ");
			mvprintw(offsety+8,offsetx,"| |              | |  ");
			mvprintw(offsety+9,offsetx,"| '--------------' |  ");
			mvprintw(offsety+10,offsetx," '----------------'   ");
		break;
		case 'Q':
			mvprintw(offsety+0,offsetx," .----------------.   ");
			mvprintw(offsety+1,offsetx,"| .--------------. |  ");
			mvprintw(offsety+2,offsetx,"| |    ___       | |  ");
			mvprintw(offsety+3,offsetx,"| |  .'   '.     | |  ");
			mvprintw(offsety+4,offsetx,"| | /  .-.  \\    | |  ");
			mvprintw(offsety+5,offsetx,"| | | |   | |    | |  ");
			mvprintw(offsety+6,offsetx,"| | \\  `-'  \\_   | |  ");
			mvprintw(offsety+7,offsetx,"| |  `.___.\\__|  | |  ");
			mvprintw(offsety+8,offsetx,"| |              | |  ");
			mvprintw(offsety+9,offsetx,"| '--------------' |  ");
			mvprintw(offsety+10,offsetx," '----------------'   ");
		break;
		case 'X':
			mvprintw(offsety+0,offsetx," .----------------.   ");
			mvprintw(offsety+1,offsetx,"| .--------------. |  ");
			mvprintw(offsety+2,offsetx,"| |  ____  ____  | |  ");
			mvprintw(offsety+3,offsetx,"| | |_  _||_  _| | |  ");
			mvprintw(offsety+4,offsetx,"| |   \\ \\  / /   | |  ");
			mvprintw(offsety+5,offsetx,"| |    > `' <    | |  ");
			mvprintw(offsety+6,offsetx,"| |  _/ /'`\\ \\_  | |  ");
			mvprintw(offsety+7,offsetx,"| | |____||____| | |  ");
			mvprintw(offsety+8,offsetx,"| |              | |  ");
			mvprintw(offsety+9,offsetx,"| '--------------' |  ");
 			mvprintw(offsety+10,offsetx," '----------------'   ");
		break;
		case 'J':
			mvprintw(offsety+0,offsetx," .----------------.   ");
			mvprintw(offsety+1,offsetx,"| .--------------. |  ");
			mvprintw(offsety+2,offsetx,"| |     _____    | |  ");
			mvprintw(offsety+3,offsetx,"| |    |_   _|   | |  ");
			mvprintw(offsety+4,offsetx,"| |      | |     | |  ");
			mvprintw(offsety+5,offsetx,"| |   _  | |     | |  ");
			mvprintw(offsety+6,offsetx,"| |  | |_' |     | |  ");
			mvprintw(offsety+7,offsetx,"| |  `.___.'     | |  ");
			mvprintw(offsety+8,offsetx,"| |              | |  ");
			mvprintw(offsety+9,offsetx,"| '--------------' |  ");
 			mvprintw(offsety+10,offsetx," '----------------'   ");
		break;
		case '9':
			mvprintw(offsety+0,offsetx," .----------------.   ");
			mvprintw(offsety+1,offsetx,"| .--------------. |  ");
			mvprintw(offsety+2,offsetx,"| |    ______    | |  ");
			mvprintw(offsety+3,offsetx,"| |  .' ____ '.  | |  ");
			mvprintw(offsety+4,offsetx,"| |  | (____) |  | |  ");
			mvprintw(offsety+5,offsetx,"| |  '_.____. |  | |  ");
			mvprintw(offsety+6,offsetx,"| |  | \\____| |  | |  ");
			mvprintw(offsety+7,offsetx,"| |   \\______,'  | |  ");
			mvprintw(offsety+8,offsetx,"| |              | |  ");
			mvprintw(offsety+9,offsetx,"| '--------------' |  ");
 			mvprintw(offsety+10,offsetx," '----------------'   ");
	}
}