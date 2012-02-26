#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <msgpack.hpp>

typedef struct
{
	unsigned short id;
	std::string name;
	unsigned short progress;
	MSGPACK_DEFINE(id, name, progress);
} choice_t;

class Message
{
	protected:
		unsigned short type;
	public:
		MSGPACK_DEFINE(type);
		Message(unsigned short type);
		Message();
		unsigned short getType();
};

class NewChoiceRequestMessage : public Message
{
	protected:
		std::string choice;	
	public:
		MSGPACK_DEFINE(type, choice);
		NewChoiceRequestMessage(std::string choice);
		std::string getChoice();

};

class NewChoiceResultMessage : public Message
{
	protected:
		unsigned short choiceId;
	public:
		MSGPACK_DEFINE(type, choiceId);
		NewChoiceResultMessage(unsigned short choiceId);
		unsigned short getChoiceId();
};

class ChooseRequestMessage : public Message
{
	protected:
		unsigned short choiceId;
	public:
		MSGPACK_DEFINE(type, choiceId);
		ChooseRequestMessage(unsigned short choiceId);
		unsigned short getChoiceId();
};

class GetAllChoicesRequestMessage : public Message
{
	public:
		GetAllChoicesRequestMessage();
		MSGPACK_DEFINE(type);
};

class GetAllChoicesResultMessage : public Message
{
	protected:
		std::vector<choice_t> choices;
	public:
		MSGPACK_DEFINE(type, choices);
		GetAllChoicesResultMessage(std::vector<choice_t> choices);
		std::vector<choice_t> getChoices();
};	

#endif
