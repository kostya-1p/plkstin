#include <stdio.h>
#include <curses.h>
#include <unistd.h>

void rectangle(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

struct Rectangle{
	int x, y;
	int w, h;
};

struct Digit{
	int x, y;
  int deltaX, deltaY;
	char num;
};

bool isLowLeftCorner(struct Rectangle* rect, struct Digit* digit){
  if((*digit).x == (*rect).x - 1 && (*digit).y == (*rect).y + (*rect).h + 1)
    return 1;
  return 0;
}

bool isLowRightCorner(struct Rectangle* rect, struct Digit* digit){
  if((*digit).x == (*rect).x + (*rect).w + 1 && (*digit).y == (*rect).y + (*rect).h + 1)
    return 1;
  return 0;
}

bool isHighLeftCorner(struct Rectangle* rect, struct Digit* digit){
  if((*digit).x == (*rect).x - 1 && (*digit).y == (*rect).y - 1)
    return 1;
  return 0;
}

bool isHighRightCorner(struct Rectangle* rect, struct Digit* digit){
  if((*digit).x == (*rect).x + (*rect).w + 1  && (*digit).y == (*rect).y - 1)
    return 1;
  return 0;
}

bool isDigitAtCorner(struct Rectangle* rect, struct Digit* digit, int delta){
	if(((isHighRightCorner(rect, digit) || isLowLeftCorner(rect, digit))||
     ((isHighLeftCorner(rect, digit) || isLowRightCorner(rect, digit)))))
		return 1;
	return 0;
}

int main(){
	initscr();
	nodelay(stdscr,true);
	nocbreak();

	struct Rectangle rect;
	rect.w = 50;
	rect.h = 20;
	rect.x = COLS/2 - rect.w/2;
	rect.y = LINES/2 - rect.h/2;

	struct Digit digitOne;
	digitOne.x = rect.x;
	digitOne.y = rect.y - 1;
	digitOne.num = '1';
  digitOne.deltaX = 1; digitOne.deltaY = 0;

	struct Digit digitTwo;
	digitTwo.x = rect.x + rect.w;
	digitTwo.y = rect.y + rect.h + 1;
	digitTwo.num = '2';
  digitTwo.deltaX = -1; digitTwo.deltaY = 0;

	int delta = 1;

	int c=32, x;
	while(c != 27){

		if((x = getch()) != ERR){
			do{
				if(x == 32){
					delta = -1 * delta;
          digitTwo.deltaX *= -1; digitTwo.deltaY *= -1;
          digitOne.deltaX *= -1; digitOne.deltaY *= -1;
        }
				if ( x == 27 )
					c=(char)x;
			}while((x=getch())!=ERR);
		}
		else{
			clear();

			if(isDigitAtCorner(&rect, &digitOne, delta) && delta > 0){
        int tmp = digitOne.deltaY;
        digitOne.deltaY = digitOne.deltaX;
        digitOne.deltaX = tmp;
        digitOne.deltaX *= -1;
        tmp = digitTwo.deltaY;
        digitTwo.deltaY = digitTwo.deltaX;
        digitTwo.deltaX = tmp;
        digitTwo.deltaX *= -1;
			}
      else if(isDigitAtCorner(&rect, &digitOne, delta) && delta < 0){
        digitOne.deltaX *= -1;
        int tmp = digitOne.deltaY;
        digitOne.deltaY = digitOne.deltaX;
        digitOne.deltaX = tmp;

        digitTwo.deltaX *= -1;
        tmp = digitTwo.deltaY;
        digitTwo.deltaY = digitTwo.deltaX;
        digitTwo.deltaX = tmp;
      }

      digitOne.y += digitOne.deltaY;
      digitTwo.y += digitTwo.deltaY;
      digitOne.x += digitOne.deltaX;
      digitTwo.x += digitTwo.deltaX;

			rectangle( rect.y, rect.x, rect.y + rect.h, rect.x + rect.w);
			mvprintw(digitOne.y, digitOne.x, "%c", digitOne.num);
			mvprintw(digitTwo.y, digitTwo.x, "%c", digitTwo.num);
			usleep(100000);
		}
	}

	nodelay(stdscr,false);
	endwin();
	return 0;
}
