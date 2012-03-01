#include <zmq.hpp>
#include <stdio.h>
#include <msgpack.hpp>
#include "Message.h"

void displayHelp(char *execName);
void mapToVec(const std::map<unsigned short, choice_t>* srcMap, std::vector<choice_t>* destVec);

int main(int argc, char **argv) {
	if (argc < 3) {
		displayHelp(argv[0]);
		return 1;
	}

	char bindTo[32];
	sprintf(bindTo, "tcp://%s:%s", argv[1], argv[2]);
	printf("bindTo: %s\n", bindTo);

	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind(bindTo);

	std::map<unsigned short, choice_t> choices;
	unsigned short choiceId = 0;

	while (true) {
		zmq::message_t request;
		socket.recv(&request);
		
		msgpack::unpacked msg;
		msgpack::unpack(&msg, (const char*)request.data(), request.size());
		msgpack::object obj = msg.get();

		Message message;
		obj.convert(&message);
		std::cout << "message type: " << message.getType() << std::endl;

		msgpack::sbuffer sbuf;

		switch (message.getType()) {
			case 1: {//NewChoiceRequestMessage
				NewChoiceRequestMessage msg;
				obj.convert(&msg);

				choice_t newChoice;
				newChoice.name = msg.getChoice();
				newChoice.progress = 0;
				newChoice.id = choiceId++;
				
				choices.insert(std::pair<unsigned short, choice_t>(newChoice.id, newChoice));

				NewChoiceResultMessage ncrmMsg(newChoice.id);
				msgpack::pack(sbuf, ncrmMsg);

				zmq::message_t response(sbuf.size());
				memcpy((void*)response.data(), sbuf.data(), sbuf.size());

				socket.send(response);
				} break;
			case 4: {//GetAllChoicesRequestMessage
				std::vector<choice_t> choicesToSend;
				mapToVec(&choices, &choicesToSend);

				GetAllChoicesResultMessage gacrMsg(choicesToSend);
				msgpack::pack(sbuf, gacrMsg);
				
				zmq::message_t response(sbuf.size());
				memcpy((void*)response.data(), sbuf.data(), sbuf.size());
				
				socket.send(response);
				std::cout << "send " << choices.size() << " choices to client" << std::endl;
				std::cout << "send " << choicesToSend.size() << " choices to client" << std::endl;
				} break;
		}

//		memcpy(&recd, request.data(), sizeof(message_t));
		
//		printf("received a: %i b: %i c: %i\n", recd.a, recd.b, recd.c);
//		printf("received: %s\n", (char*)request.data());
	}
}

void displayHelp(char *execName) {
	printf("Usage: %s ip port\n", execName);
}

void mapToVec(const std::map<unsigned short, choice_t>* srcMap, std::vector<choice_t>* destVec) {
	for (std::map<unsigned short, choice_t>::const_iterator it = srcMap->begin(); it != srcMap->end(); ++it) {
		destVec->push_back(it->second);
	}
}
