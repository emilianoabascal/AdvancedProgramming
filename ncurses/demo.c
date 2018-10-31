/*
    Example from:
    https://www.linuxquestions.org/questions/programming-9/detecting-arrow-key-presses-in-c-c-166055/
*/

#include <ncurses.h>
#include <unistd.h>
#define DELAY 30000
int main()
{
    int ch;
    int x = 0, y = 0;
    int max_y = 0, max_x = 0;
    int next_x = 0;
    int next_y = 0;
    int directionX = 1;
    int directionY = 1;
    int xFlag = 0;
    int yFlag = 0;
    /* Curses Initialisations */
    initscr();
    raw();                  // Do not buffer inputs, but pass them immediately to be read. Do not react to signal inputs
    keypad(stdscr, TRUE);   // Allow reading special keys from the keyboard (Fx, and arrows)
    noecho();               // Do not show on the terminal the keys pressed

    printw("Welcome - Press # to Exit\n");



    initscr();
    noecho();
    curs_set(FALSE);

    // Global var `stdscr` is created by the call to `initscr()`
    getmaxyx(stdscr, max_y, max_x);

    while((ch = getch()) != '#') {
      clear();
      switch(ch){
          case KEY_UP:
              directionY *= -1;
              yFlag = 1;
              xFlag = 0;
          case KEY_DOWN:
              directionY *= -1;
              yFlag = 1;
              xFlag = 0;
          case KEY_LEFT:
              directionX *= -1;
              yFlag = 0;
              xFlag = 1;
          case KEY_RIGHT:
              directionX *= -1;
              yFlag = 0;
              xFlag = 1;
          default:
              printw("\nThe pressed key is ");
              attron(A_BOLD);             // Activate the attribute to when displaying characters
              printw("%c", ch);
              attroff(A_BOLD);
      }
      if(xFlag == 1 && yFlag == 0){
        next_x = x + directionX;

      }else if(yFlag == 1 && yFlag == 0){
        next_y = y + directionY;
      }
      mvprintw(y, x, "o");
      refresh();
      usleep(DELAY);


      if (next_x >= max_x || next_x < 0) {
        directionX*= -1;
      } else {
        x+= directionX;
      }
      if(next_y >= max_y || next_y < 0){
        directionY *= -1;
      }else{
        y += directionY;
      }
    }

    endwin();

    printw("\n\nBye Now!\n");

    refresh();          // Show on the terminal anything printed on the window
    getch();            // Wait for a final key press before finishing the program
    endwin();

    return 0;
}
