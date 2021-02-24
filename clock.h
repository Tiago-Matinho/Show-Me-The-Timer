#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#include <ncurses.h>

/* Macros */
#define ITOA(i) (i-'0')
#define HOURFRAMEW  54
#define MINFRAMEW  34
#define HOUR_IN_SEC 3600
#define BG_COLOR COLOR_BLACK
#define HOUR_COLOR COLOR_YELLOW
#define MINUTE_COLOR COLOR_YELLOW
#define SECOND_COLOR COLOR_YELLOW
#define HOUR_DOT_COLOR COLOR_YELLOW
#define MIN_DOT_COLOR COLOR_YELLOW
#define FINISH_MSG "~~~ Timer Finished! ~~~\n"

/* Number matrix */
const bool number[][15] = {
	{1,1,1,1,0,1,1,0,1,1,0,1,1,1,1},	/* 0 */
	{0,0,1,0,0,1,0,0,1,0,0,1,0,0,1},	/* 1 */
	{1,1,1,0,0,1,1,1,1,1,0,0,1,1,1},	/* 2 */
	{1,1,1,0,0,1,1,1,1,0,0,1,1,1,1},	/* 3 */
	{1,0,1,1,0,1,1,1,1,0,0,1,0,0,1},	/* 4 */
	{1,1,1,1,0,0,1,1,1,0,0,1,1,1,1},	/* 5 */
	{1,1,1,1,0,0,1,1,1,1,0,1,1,1,1},	/* 6 */
	{1,1,1,0,0,1,0,0,1,0,0,1,0,0,1},	/* 7 */
	{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1},	/* 8 */
	{1,1,1,1,0,1,1,1,1,0,0,1,1,1,1},	/* 9 */
};

/* Clock class */
class Clock {
	public:
		Clock(u_int32_t time_left) {
			this->readConfigs();
			this->newTimer(time_left);

			/* Init color pair	*/
			/* 0 	Empty		*/
			/* 1 	Hour		*/
			/* 2 	Minute		*/
			/* 3 	Second		*/
			/* 4 	Hour dot	*/
			/* 5 	Min dot		*/
			init_pair(0, options.bg_color, options.bg_color);
			init_pair(1, options.hour_color, options.hour_color);
			init_pair(2, options.minute_color, options.minute_color);
			init_pair(3, options.second_color, options.second_color);
			init_pair(4, options.hour_dot_color, options.hour_dot_color);
			init_pair(5, options.min_dot_color, options.min_dot_color);
			refresh();

			this->width = MINFRAMEW;

			if(time_left >= HOUR_IN_SEC) {
				this->options.hour = true;
				width = HOURFRAMEW;
			}
			
			this->height = 7;
			this->cur_x = 1;
			this->cur_y = 1;
			
			/* Create clock win */
			this->win = newwin(this->height, this->width,
			(LINES - this->height) / 2,(COLS - this->width) / 2);

			/* Draw box */
			if(options.box)
				box(this->win, 0, 0);

			wrefresh(this->win);
		}

		~Clock() {}

		u_int8_t getHour() {return this->timer.hour;}
		u_int8_t getMin() {return this->timer.minute;}
		u_int8_t getSec() {return this->timer.second;}
		u_int8_t getTimeleft() {return this->timer.time_left;}
		std::string getFinishMsg() {return this->options.finish_msg;}
		std::string getFinishSound() {return this->options.finish_sound;}
		bool finished() {return !this->timer.time_left;}

		void updateTime();
		void drawClock();
		void showMessage();

	private:
		/* Options */
		struct {
			bool box;
			bool hour;
			int bg_color;
			int hour_color;
			int minute_color;
			int second_color;
			int hour_dot_color;
			int min_dot_color;
			std::string finish_msg;
			std::string finish_sound;
		} options;

		/* Timer */
		struct {
			u_int32_t time_left;
			u_int8_t hour;
			u_int8_t minute;
			u_int8_t second;		
		} timer;

		/* Ncurses */
		int height, width, cur_x, cur_y; /* x -> vertical y -> horizontal */
		WINDOW* win;
		
		void readConfigs() { // TODO	add the ability to read config file
			this->options.box = true;
			this->options.hour = false;
			this->options.bg_color = BG_COLOR;
			this->options.hour_color = HOUR_COLOR;
			this->options.minute_color = MINUTE_COLOR;
			this->options.second_color = SECOND_COLOR;
			this->options.hour_dot_color = HOUR_DOT_COLOR;
			this->options.min_dot_color = MIN_DOT_COLOR;
			this->options.finish_msg = FINISH_MSG;
			this->options.finish_sound = "";
		}
		
		void newTimer(u_int32_t time_left) {
			this->timer.time_left = time_left;
			this->timer.hour = 0;
			this->timer.minute = 0;
			this->timer.second = 0;
		}

		void drawNumber(u_int8_t n) { // FIXME	Change so that color is also an arg
			int i, sx = this->cur_x, sy = this->cur_y;

			for(i = 0; i < 30; ++i, ++sy) {
				if(sy == this->cur_y + 6) {
					sy = this->cur_y;
					++sx;
				}

				wattroff(this->win, A_BLINK);

				wbkgdset(this->win, COLOR_PAIR(number[n][i/2]));
				mvwaddch(this->win, sx, sy, ' ');
			}

			wrefresh(this->win);
			this->cur_y += 7;
		}

		void drawDots() {	 // FIXME	Change so that color is also an arg
			chtype dotcolor = COLOR_PAIR(1);
			/* 2 dot for number separation */
			wbkgdset(this->win, dotcolor);
			this->cur_y++;
			mvwaddstr(this->win, 2, this->cur_y, "  ");
			mvwaddstr(this->win, 4, this->cur_y, "  ");
			this->cur_y += 4;
		}
};

/* Prototypes */

#endif