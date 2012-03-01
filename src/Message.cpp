#include "Message.h"

Message::Message()
{
}

Message::Message(unsigned short type)
{
	this->type = type;
}

unsigned short Message::getType()
{
	return this->type;
}

NewChoiceRequestMessage::NewChoiceRequestMessage()
	: Message(1)
{
}

NewChoiceRequestMessage::NewChoiceRequestMessage(std::string choice)
	: Message(1)
{
	this->choice = choice;
}

std::string NewChoiceRequestMessage::getChoice()
{
	return this->choice;
}

NewChoiceResultMessage::NewChoiceResultMessage()
	: Message(2)
{
}

NewChoiceResultMessage::NewChoiceResultMessage(unsigned short choiceId)
	: Message(2)
{
	this->choiceId = choiceId;
}

unsigned short NewChoiceResultMessage::getChoiceId()
{
	return this->choiceId;
}

ChooseRequestMessage::ChooseRequestMessage(unsigned short choiceId)
	: Message(3)
{
	this->choiceId = choiceId;
}

unsigned short ChooseRequestMessage::getChoiceId()
{
	return this->choiceId;
}

GetAllChoicesRequestMessage::GetAllChoicesRequestMessage()
	: Message(4)
{
}

GetAllChoicesResultMessage::GetAllChoicesResultMessage()
	: Message(5)
{
}

GetAllChoicesResultMessage::GetAllChoicesResultMessage(std::vector<choice_t> choices)
	: Message(5)
{
	this->choices = choices;
}

const std::vector<choice_t>* GetAllChoicesResultMessage::getChoices()
{
	return &this->choices;
}
