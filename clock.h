#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

/* Macro */
#define HOURFRAMEW  54
#define MINFRAMEW  34

typedef struct {
	/* terminal variables */
	int bg;

	/* Running option */
	struct {
		bool hour;
		bool center;
		bool box;
		bool noquit;
		char format[100];
		int color;
	} option;

	/* Clock geometry */
	struct {
		int x, y, w, h;
		int a, b;
	} geo;

	struct
	{
		unsigned int time_left;
		unsigned short hour[2];
		unsigned short minute[2];
		unsigned short second[2];
		char datestr[256];
	} date;


	/* Clock member */
	char *meridiem;
	WINDOW *framewin;
} matinho_clock_t;

/* Number matrix */
const bool number[][15] =
{
	{1,1,1,1,0,1,1,0,1,1,0,1,1,1,1}, /* 0 */
	{0,0,1,0,0,1,0,0,1,0,0,1,0,0,1}, /* 1 */
	{1,1,1,0,0,1,1,1,1,1,0,0,1,1,1}, /* 2 */
	{1,1,1,0,0,1,1,1,1,0,0,1,1,1,1}, /* 3 */
	{1,0,1,1,0,1,1,1,1,0,0,1,0,0,1}, /* 4 */
	{1,1,1,1,0,0,1,1,1,0,0,1,1,1,1}, /* 5 */
	{1,1,1,1,0,0,1,1,1,1,0,1,1,1,1}, /* 6 */
	{1,1,1,0,0,1,0,0,1,0,0,1,0,0,1}, /* 7 */
	{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1}, /* 8 */
	{1,1,1,1,0,1,1,1,1,0,0,1,1,1,1}, /* 9 */
};

/* Global variable */
matinho_clock_t clock;

/* Prototypes */
void init();
void update_time();
void draw_number(int n, int x, int & y);
void draw_dots(int & y);
void draw_clock();

#endif