#include <zmq.hpp>
#include <stdio.h>

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
		
		printf("received: %s\n", (char*)request.data());
		socket.send(request);
	}
}

void displayHelp(char *execName) {
	printf("Usage: %s ip port\n", execName);
}