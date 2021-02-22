#include "clock.h"

void init(unsigned int time_left) {
	/* Default date format */
	strncpy(clock.option.format, "%F", sizeof (clock.option.format));
	clock.date.time_left = time_left;
	
	/* Default color */
	clock.option.color = COLOR_GREEN; /* COLOR_GREEN = 2 */
	clock.bg = COLOR_BLACK;
	
	initscr();
	start_color();

	/* Init default terminal color */
	if(use_default_colors() == OK)
		clock.bg = -1;

	/* Init color pair */
	init_pair(0, clock.bg, clock.bg);
	init_pair(1, clock.bg, clock.option.color);
	init_pair(2, clock.option.color, clock.bg);

	refresh();

	clock.geo.x = 0;
	clock.geo.y = 0;
	clock.geo.w = MINFRAMEW;
	clock.geo.h = 7;

	if(time_left >= 3600) {
		clock.option.hour = true;
		clock.geo.w = HOURFRAMEW;
	}

	/* Create clock win */
	clock.framewin = newwin(clock.geo.h,
							clock.geo.w,
							clock.geo.x,
							clock.geo.y);


	box(clock.framewin, 0, 0);

	// nodelay(stdscr, true);
	wrefresh(clock.framewin);
}

void update_time() {
	int h = clock.date.time_left / 3600;
	int m = (clock.date.time_left % 3600) / 60;
	int s = clock.date.time_left % 60;

	clock.date.hour[0] = h / 10;
	clock.date.hour[1] = h % 10;

	clock.date.minute[0] = m / 10;
	clock.date.minute[1] = m % 10;

	clock.date.second[0] = s / 10;
	clock.date.second[1] = s % 10;

	clock.date.time_left--;
}

void draw_number(int n, int x, int & y) {
	int i, sy = y;

	for(i = 0; i < 30; ++i, ++sy) {
		if(sy == y + 6) {
			sy = y;
			++x;
		}

		wattroff(clock.framewin, A_BLINK);

		wbkgdset(clock.framewin, COLOR_PAIR(number[n][i/2]));
		mvwaddch(clock.framewin, x, sy, ' ');
	}

	wrefresh(clock.framewin);
	y += 7;
}

void draw_dots(int & y) {
	chtype dotcolor = COLOR_PAIR(1);
	/* 2 dot for number separation */
	wbkgdset(clock.framewin, dotcolor);
	y ++;
	mvwaddstr(clock.framewin, 2, y, "  ");
	mvwaddstr(clock.framewin, 4, y, "  ");
	y += 4;
}

void draw_clock() {
	int y = 1;

	if(clock.option.hour) {
		/* Draw hour numbers */
		draw_number(clock.date.hour[0], 1, y);
		draw_number(clock.date.hour[1], 1, y);
		/* Draw dots */
		draw_dots(y);
	}

	/* Draw minute numbers */
	draw_number(clock.date.minute[0], 1, y);
	draw_number(clock.date.minute[1], 1, y);
	/* Draw dots */
	draw_dots(y);

	/* Draw second numbers */
	draw_number(clock.date.second[0], 1, y);
	draw_number(clock.date.second[1], 1, y);

	wrefresh(clock.framewin);
}

int main(int argc, const char ** argv) {
	/* Alloc clock */
	memset(&clock, 0, sizeof(clock_t));

	init(3600);

	while(clock.date.time_left != 0) {
		update_time();
		draw_clock();
		sleep(1);
	}

	endwin();

	return 0;
}