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
		this->drawNumber(this->timer.hour / 10);
		this->drawNumber(this->timer.hour % 10);
		/* Draw dots */
		this->drawDots();
	}

	/* Draw minute numbers */
	this->drawNumber(this->timer.minute / 10);
	this->drawNumber(this->timer.minute % 10);
	/* Draw dots */
	this->drawDots();

	/* Draw second numbers */
	this->drawNumber(this->timer.second / 10);
	this->drawNumber(this->timer.second % 10);

	wrefresh(this->win);
}

int main(int argc, const char** argv) {
	u_int32_t time_left = 0;

	std::cin >> time_left;

	Clock clock(time_left);

	while(!clock.finished()) {
		clock.updateTime();
		clock.drawClock();
		sleep(1);
	}

	endwin();
	
	return 0;
}