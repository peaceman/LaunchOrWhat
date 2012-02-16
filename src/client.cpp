#include <zmq.hpp>
#include <stdio.h>
#include "message.h"

void displayHelp(char *execName);

int main(int argc, char **argv) {
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
