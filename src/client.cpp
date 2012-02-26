#include <msgpack.hpp>
#include <ncurses.h>
#include <zmq.hpp>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Message.h"
#include "window.h"
#include "time.h"
#include "unistd.h"

void displayHelp(char *execName);

int main(int argc, char **argv) {
	initscr();
	cbreak();
	noecho();

	keypad(stdscr, true);
	curs_set(0);
	refresh();

	GraphWindow* w = new GraphWindow(LINES - 5, COLS - 10, 5, 0);
	w->drawGraphs();

	for (int i = 0; i < 50; i++) {
		choice_t* c = w->getChoiceById(1);
		c->progress++;	

		c = w->getChoiceById(2);
		c->progress--;

		w->drawGraphs();
		usleep(125000);
	}	

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

	GetAllChoicesRequestMessage gacrMsg;
	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, gacrMsg);

	zmq::message_t request(sbuf.size());
	memcpy((void*) request.data(), sbuf.data(), sbuf.size());

	socket.send(request);
	socket.recv(&request);

	return 0;

}

void displayHelp(char *execName) {
	printf("Usage: %s ip port\n", execName);
}
