/* bresenham.c -- program to draw ASCII circles with bresenham's algorithm.
 * Author: Luis Colorado <luicoloradourcola@gmail.com>
 * Date: Wed Jan 11 10:50:17 EET 2017
 * Disclaimer: (C) 2017 LUIS COLORADO. ALL RIGHTS RESERVED.
 * BSD 3-Clause License
 * 
 * Copyright (c) 2017, Luis Colorado
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define FL_FILL 	(1 << 0)
#define FL_TRACE	(1 << 1)

#define F(x) "%s:%d:%s: " x, __FILE__, __LINE__, __func__

/* draws a horizontal line at row y, between x1 and x2 */
static void hline(int x1, int x2, int y)
{
	static char theLine[] =
	"*****************************************"
	"*****************************************"
	"*****************************************"
	"*****************************************"
	"*****************************************"
	"*****************************************"
	"*****************************************"
	"*****************************************"
	"*****************************************";
	printf("\033[%d;%dH%.*s", y, x1<<1, (x2 - x1 + 1)<<1, theLine);
} /* hline */

/* draws a dot at coordinates (x, y) */
static void dot(int x, int y)
{
	printf("\033[%d;%dH*", y, x<<1);
} /* dot */

/* draws a circle of radius r and center at (cx, cy) */
void bh(int r, int cx, int cy, int flags)
{
	int r2 = r*r + r;
	int x = 0, x2 = 0, dx2 = 1;
	int y = r, y2 = y*y, dy2 = 2*y - 1;
	int sum = r2;

	while(x <= y) {
		if (flags & FL_TRACE) {
			printf(F("x=%3d, x2=%5d, dx2=%3d, y=%3d, y2=%5d, dy2=%3d, sum=%5d\n"),
				x, x2, dx2, y, y2, dy2, sum);
		} else {
			if (flags & FL_FILL) {
				hline(cx - y, cx + y, cy + x);
				hline(cx - y, cx + y, cy - x);
			} else {
				dot(cx - y, cy + x); dot(cx + y, cy + x);
				dot(cx - y, cy - x); dot(cx + y, cy - x);
				dot(cx - x, cy - y); dot(cx + x, cy - y);
				dot(cx - x, cy + y); dot(cx + x, cy + y);
			} /* if */
		} /* if */

		sum -= dx2;
		if (sum <= y2) {
			if (!(flags & FL_TRACE) && (flags & FL_FILL)) {
				hline(cx - x, cx + x, cy - y);
				hline(cx - x, cx + x, cy + y);
			} /* if */
			y--; y2 -= dy2; dy2 -= 2;
		} /* if */
		x++;
		x2 += dx2;
		dx2 += 2;
	} /* while */
} /* bh */

int main(int argc, char **argv)
{
	int i;
	char *cols = getenv("COLUMNS");
	char *lines = getenv("LINES");
	int cx, cy;
	int opt;
	int flags = 0;

	while ((opt = getopt(argc, argv, "fv")) != EOF) {
		switch(opt) {
		case 'f': flags |= FL_FILL; break;
		case 'v': flags |= FL_TRACE; break;
		} /* switch */
	} /* while */

	argc -= optind;
	argv += optind;

	if (cols && lines) {
		cx = atoi(cols);
		cy = atoi(lines);
	} else { /* try to get from tty */
		struct winsize win;
		int res = ioctl(0, TIOCGWINSZ, &win);
		if (res == 0) {
			cx = win.ws_col;
			cy = win.ws_row;
		} else {
			fprintf(stderr,
				F("TIOCGWINSZ: %s (errno=%d)\n"),
				strerror(errno), errno);
			cx = 80; cy = 24;
		} /* if */
	} /* if */

	/* center coordinates */
	cx /= 4;
	cy /= 2;

	if (!(flags & FL_TRACE))
		fputs("\033[2J", stdout);

	for (i = 0; i < argc; i++) {
		bh(atoi(argv[i]), cx, cy, flags);
	} /* for */
	if (!(flags & FL_TRACE)) {
		puts("");
		fflush(stdout);
	} /* if */
} /* main */
