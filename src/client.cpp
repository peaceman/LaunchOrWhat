#include <ncurses.h>
#include <zmq.hpp>
#include <stdio.h>
#include "message.h"
#include "window.h"
#include "time.h"
#include "unistd.h"

void displayHelp(char *execName);
void drawGraph(Window* win, const char* caption, unsigned int progress);

int main(int argc, char **argv) {
	initscr();
	cbreak();
	noecho();

	keypad(stdscr, true);
	curs_set(0);
	refresh();

	Window* w = new Window(LINES - 5, COLS - 10, 5, 0);
	WINDOW* win = w->getWindow();
	
	for (int i = 1; i <= 100; i++) {
		drawGraph(w, "fooMeter", i);
		sleep(1);
		drawGraph(w, "lordMeter", i - 1);
		sleep(1);
	}
	drawGraph(w, "foometer", 50);
	
	getch();
	endwin();
	if (argc < 3) {
		displayHelp(argv[0]);
		return 1;
	}

	char connectTo[32];
	sprintf(connectTo, "tcp://%s:%s", argv[1], argv[2]);
	printf("connectTo: %s\n", connectTo);

	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REQ);
	socket.connect(connectTo);

	message_t toSend = {23, 5, 13};

	while (true) {
		zmq::message_t request(sizeof(message_t));
		toSend.b++;
		memcpy((void*) request.data(), &toSend, sizeof(message_t));
		
		socket.send(request);
		socket.recv(&request);
	}
}

void displayHelp(char *execName) {
	printf("Usage: %s ip port\n", execName);
}

void drawGraph(Window* win, const char* caption, unsigned int progress)
{
	// print caption
	wmove(win->getWindow(), 1,1);
	wclrtoeol(win->getWindow());
	mvwprintw(win->getWindow(), 1, 1, "%s", caption);
	// draw progress bar
	wmove(win->getWindow(), 2,1);
	wclrtoeol(win->getWindow());
	int nrOfChars = (*win->getWidth() - 2) / 100.0 * progress;
	mvwhline(win->getWindow(), 2, 1, ACS_CKBOARD, nrOfChars);
	win->refreshWindow();
}
