#include <zmq.hpp>
#include <stdio.h>
#include <msgpack.hpp>
#include "Message.h"

void displayHelp(char *execName);

int main(int argc, char **argv) {
	if (argc < 3) {
		displayHelp(argv[0]);
		return 1;
	}

	char bindTo[32];
	sprintf(bindTo, "tcp://%s:%s", argv[1], argv[2]);
	printf("bindTo: %s\n", bindTo);

//	printf("argc: %i\n", argc);
//	for (int i = 0; i < argc; i++) {
//		printf("argv[%i]: %s\n", i, argv[i]);
//	}

	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind(bindTo);

	while (true) {
		zmq::message_t request;
		socket.recv(&request);
		
		msgpack::unpacked msg;
		msgpack::unpack(&msg, (const char*)request.data(), request.size());
		msgpack::object obj = msg.get();

		Message message;
		obj.convert(&message);
		std::cout << "message type: " << message.getType() << std::endl;

//		memcpy(&recd, request.data(), sizeof(message_t));
		
//		printf("received a: %i b: %i c: %i\n", recd.a, recd.b, recd.c);
//		printf("received: %s\n", (char*)request.data());
		socket.send(request);
	}
}

void displayHelp(char *execName) {
	printf("Usage: %s ip port\n", execName);
}
