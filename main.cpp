#include "clock.h"

u_int32_t menu() {
	WINDOW* menuwin = newwin(7, 20, (LINES - 7) / 2, (COLS - 20) / 2);
	box(menuwin, 0, 0);
	refresh();
	wrefresh(menuwin);
	
	keypad(menuwin, true);
	
	u_int8_t timer[] = {0,0,0};
	u_int8_t cur_y;
	int choice;
	int highlight = 0;
	
	while(1) {
		cur_y = (20 - 8) / 2;

		for(int i = 0; i < 3; i++) {
			if(i == highlight)
				wattron(menuwin, A_REVERSE);

			mvwprintw(menuwin, 3, cur_y, "%02d", timer[i]);
			cur_y += 2;

			wattroff(menuwin, A_REVERSE);

			if(i < 2) {
				mvwprintw(menuwin, 3, cur_y, ":");
				cur_y++;
			}
		}

		choice = wgetch(menuwin);
		refresh();

		switch(choice) {
			case KEY_LEFT:
				highlight--;

				if(highlight == -1)
					highlight = 0;
				break;

			case KEY_RIGHT:
				highlight++;

				if(highlight == 3)
					highlight = 2;
				break;

			case KEY_DOWN:
				timer[highlight]--;
				if(timer[highlight] == UINT8_MAX)
					timer[highlight] = 0;
				break;

			case KEY_UP:
				timer[highlight]++;
				if(highlight == 0 && timer[highlight] == 25)
					timer[highlight] = 24;

				else if(timer[highlight] == 61)
					timer[highlight] = 60;
				break;

			default:
				break;
		}
		if(choice == 10)
			break;
	}

	clear();
	return timer[0] * 3600 + timer[1] * 60 + timer[2];
}

void cleanup() { endwin(); }

void stop_timer() {
	char c = 0;
	
	while(1) {
		c = getch();

		if(c == 'q' || c == 'Q')
			exit(0);
	}
}

void signal_handler(int signal) {
	switch(signal) {
		case SIGINT:
	 	case SIGTERM:
		 	running = false;
			break;

		/* Segmentation fault signal */
		case SIGSEGV:
			endwin();
			fprintf(stderr, "Segmentation fault.\n");
			exit(1);
			break;
		default:
		break;
	}
}

int main(int argc, const char** argv) {
	atexit(cleanup);

	/* Init signal handler */
	struct sigaction sig;
	sig.sa_handler = signal_handler;
	sig.sa_flags = 0;
	sigaction(SIGTERM, &sig, NULL);
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGSEGV, &sig, NULL);

	/* Start ncurses screen */
	initscr();
	/* cbreak() mode makes sure that control characters like suspend (CTRL-Z),
	interrupt and quit (CTRL-C) are passed as a signal */
	cbreak();
	noecho(); /* switches off echoing */
	start_color();
	curs_set(false);
	clear();

	u_int32_t time_left = menu();
	Clock clock(time_left);

	std::thread read_input(stop_timer);
	while(!clock.finished() && running) {
		clock.updateTime();
		clock.drawClock();
		sleep(1);
	}

	if(running)
		clock.showMessage();
	
	endwin();
	
	return 0;
}