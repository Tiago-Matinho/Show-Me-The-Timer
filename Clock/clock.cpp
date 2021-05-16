#include "clock.h"

void Clock::updateTime() {
	this->timer.hour = this->timer.time_left / 3600;
	this->timer.minute = (this->timer.time_left % 3600) / 60;
	this->timer.second = this->timer.time_left % 60;

	this->timer.time_left--;
}

void Clock::drawClock() {
	this->cur_y = 1;
	
	if(this->options.hour) {
		/* Draw hour numbers */
		this->drawNumber(this->timer.hour / 10, 1);
		this->drawNumber(this->timer.hour % 10, 1);
		/* Draw dots */
		this->drawDots(4);
	}

	/* Draw minute numbers */
	this->drawNumber(this->timer.minute / 10, 2);
	this->drawNumber(this->timer.minute % 10, 2);
	/* Draw dots */
	this->drawDots(5);

	/* Draw second numbers */
	this->drawNumber(this->timer.second / 10, 3);
	this->drawNumber(this->timer.second % 10, 3);

	wrefresh(this->win);
}

void Clock::showMessage() {
	const char* str = this->options.finish_msg.c_str();

	clear();
	mvprintw(LINES / 2, (COLS - strlen(str))/2,"%s", str);
	refresh();
	getch();
}

/*  Private methods  */

void Clock::trim(std::string& str) {
	int i = 0, j = str.size() - 1;

	while(j > i) {
		if(str[i] == ' ')
			i++;
	
		if(str[j] == ' ')
			j--;
			
		if(str[i] != ' ' && str[j] != ' ') {
			str = str.substr(i, j + 1);
			return;
		}
	}
	
	str = str.substr(i, j);
}

int Clock::color(const std::string& str) {
	if(str.compare("black") == 0)
		return COLOR_BLACK;
	else if(str.compare("red") == 0)
		return COLOR_RED;
	else if(str.compare("green") == 0)
		return COLOR_GREEN;
	else if(str.compare("yellow") == 0)
		return COLOR_YELLOW;
	else if(str.compare("blue") == 0)
		return COLOR_BLUE;
	else if(str.compare("magenta") == 0)
		return COLOR_MAGENTA;
	else if(str.compare("cyan") == 0)
		return COLOR_CYAN;
	return 0;
}

void Clock::readConfigs() {
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

	char* file_path = strcat(getenv("HOME"), CONFIG_FILENAME);

	std::ifstream conf_file(file_path, std::ifstream::in);
	char delim = '=';
	std::string param, value;

	while(conf_file.good()) {
		std::getline(conf_file, param, delim);
		std::getline(conf_file, value);
		trim(param);
		trim(value);

		if(param.compare("box") == 0)
			this->options.box = STR_TO_BOOL(value);
		else if(param.compare("bg_color") == 0)
			this->options.bg_color = color(value);
		else if(param.compare("hour_color") == 0)
			this->options.hour_color = color(value);
		else if(param.compare("minute_color") == 0)
			this->options.minute_color = color(value);
		else if(param.compare("second_color") == 0)
			this->options.second_color = color(value);
		else if(param.compare("hour_dot_color") == 0)
			this->options.hour_dot_color = color(value);
		else if(param.compare("min_dot_color") == 0)
			this->options.min_dot_color = color(value);
		else if(param.compare("finish_msg") == 0)
			this->options.finish_msg = value;
		else if(param.compare("finish_sound") == 0)
			this->options.finish_sound = value;
	}
}

void Clock::newTimer(u_int32_t time_left) {
	this->timer.time_left = time_left;
	this->timer.hour = 0;
	this->timer.minute = 0;
	this->timer.second = 0;
}

void Clock::drawNumber(u_int8_t n, int color) {
	/* Change color */
	chtype dotcolor = COLOR_PAIR(color);
	wbkgdset(this->win, dotcolor);

	int i, sx = this->cur_x, sy = this->cur_y;

	for(i = 0; i < 30; ++i, ++sy) {
		if(sy == this->cur_y + 6) {
			sy = this->cur_y;
			++sx;
		}

		wattroff(this->win, A_BLINK);

		wbkgdset(this->win, number[n][i/2]? dotcolor : COLOR_PAIR(0));
		mvwaddch(this->win, sx, sy, ' ');
	}

	wrefresh(this->win);
	this->cur_y += 7;
}

void Clock::drawDots(int color) {
	/* Change color */
	chtype dotcolor = COLOR_PAIR(color);
	wbkgdset(this->win, dotcolor);

	/* 2 dot for number separation */
	this->cur_y++;
	mvwaddstr(this->win, 2, this->cur_y, "  ");
	mvwaddstr(this->win, 4, this->cur_y, "  ");
	this->cur_y += 4;
}