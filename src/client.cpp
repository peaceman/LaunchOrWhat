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
void getInitialDataFromServer(GraphWindow* window, zmq::socket_t& socket);

template <typename Send, typename Receive>
Receive sendDataToServer(Send* toSend, zmq::socket_t& socket);

int main(int argc, char **argv) {
	initscr();
	cbreak();
	noecho();

	keypad(stdscr, true);
	curs_set(0);
	refresh();

	if (argc < 3) {
		endwin();
		displayHelp(argv[0]);
		return 1;
	}

	char connectTo[32];
	sprintf(connectTo, "tcp://%s:%s", argv[1], argv[2]);
//	printf("connectTo: %s\n", connectTo);

	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REQ);
	socket.connect(connectTo);

	GraphWindow* w = new GraphWindow(LINES - 5, COLS - 10, 5, 0);
	getInitialDataFromServer(w, socket);
	w->refreshWindow();

	NewChoiceRequestMessage newChoiceMsg("lulz");
	NewChoiceResultMessage response = sendDataToServer<NewChoiceRequestMessage, NewChoiceResultMessage>(&newChoiceMsg, socket);

	getch();
	endwin();
	return 0;
}

void displayHelp(char *execName) {
	printf("Usage: %s ip port\n", execName);
}

template <typename Send, typename Receive>
Receive sendDataToServer(Send* toSend, zmq::socket_t& socket) {
	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, *toSend);

	zmq::message_t request(sbuf.size());
	memcpy((void*)request.data(), sbuf.data(), sbuf.size());

	socket.send(request);
	socket.recv(&request);

	msgpack::unpacked msg;
	msgpack::unpack(&msg, (const char*)request.data(), request.size());
	msgpack::object obj = msg.get();

	Receive message;
	obj.convert(&message);

	return message;
}

void getInitialDataFromServer(GraphWindow* window, zmq::socket_t& socket) {
	GetAllChoicesRequestMessage gacrMsg;
	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, gacrMsg);

	zmq::message_t request(sbuf.size());
	memcpy((void*) request.data(), sbuf.data(), sbuf.size());

	socket.send(request);
	socket.recv(&request);

	msgpack::unpacked msg;
	msgpack::unpack(&msg, (const char*)request.data(), request.size());
	msgpack::object obj = msg.get();

	Message message;
	obj.convert(&message);

	if (message.getType() != 5) {
		return;
	}

	GetAllChoicesResultMessage response; 
	obj.convert(&response);
	const std::vector<choice_t>* newChoices = response.getChoices();
	window->addChoices(newChoices);
	window->drawGraphs();
}
