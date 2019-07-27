#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <jpeglib.h>

//read a file

#define ROWS 24
#define COLS 80

int main (int argc, char *argv[]) {
	int c;
	FILE *fp;

	int curs_x;
	int curs_y;
	WINDOW *win;

	int screen[ROWS][COLS];

	//initialize array with spaces
	for(int i=0; i<ROWS; i++)
		for(int j =0; j < COLS; j++)
			screen[i][j] = 32;

	if(argc !=2) {
		printf("Need a filename\n");
		exit(0);
	}
	if( fp = fopen(argv[1], "r") ) {
		printf("%s already exists\n", argv[1]);
		fclose(fp);
		exit(0);
	}
	if (  (fp = fopen(argv[1], "w")) == NULL ) {
		printf("Could not create %s\n", argv[1]);
		exit(0);
	}


	initscr();	
	cbreak(); 		//no return needed
	curs_set(TRUE);		//show the cursor
	keypad(stdscr, TRUE);	//allow arrow keys 
	noecho();		//don't show typed characters
	start_color();		//allow color
	
	win = newwin(ROWS, COLS, 0, 0);
	
	clear();
	curs_x = 0; //(COLS / 2)-1;
	curs_y = 0; //(ROWS / 2)-1;
	wmove(win, curs_y, curs_x);

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	int held = 0;
	while( ( (c = getch()) != 'q') && (c !=ERR)) {


		switch(c) {
			case KEY_UP:
				curs_y--;		
				wmove(win, curs_y, curs_x);
				break;
			case KEY_DOWN:
				curs_y++;
				wmove(win, curs_y, curs_x);
				break;
			case KEY_LEFT:
				curs_x--;
				wmove(win, curs_y, curs_x);
				break;
			case KEY_RIGHT:
				curs_x++;
				wmove(win, curs_y, curs_x);
				break;
			case 'x':
			case 'X':
				wattron(win, COLOR_PAIR(1));
				waddch(win, ACS_CKBOARD | A_BOLD );
				wattroff(win, COLOR_PAIR(1));
				screen[curs_y][curs_x] = 'X';
				wmove(win, curs_y, curs_x);
				break;
			case KEY_BACKSPACE:
				curs_x--;
				screen[curs_y][curs_x] = 32;
				wmove(win, curs_y, curs_x);
				wdelch(win);
				break;
		
			default:
				break;
		
		wrefresh(win);
		
		}
	}

	fseek(fp, 0L, SEEK_SET);

	//write the array to the file
	for( int i=0; i<ROWS; i++) {
		for( int j=0; j< COLS; j++) 
			putc(screen[i][j], fp);
		if(i != ROWS)
			putc('\n', fp);
	}
	fclose(fp);
	endwin();

	return 0;
}
	
