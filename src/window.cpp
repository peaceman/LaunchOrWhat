#include "window.h"
#include "Message.h"
#include "stdlib.h"
#include <ncurses.h>

Window::Window(int height, int width, int startx, int starty) {
	this->height = height;
	this->width = width;
	this->startx = startx;
	this->starty = starty;

	this->window = newwin(height, width, starty, startx);
	this->refreshWindow();
}

Window::~Window() {
	delwin(this->window);
}

WINDOW* Window::getWindow() {
	return this->window;
}

void Window::refreshWindow() const {
	box(this->window, 0, 0);
	if (wrefresh(this->window) != OK)
		exit(1);
}

const int* Window::getHeight() const {
	return (const int*)&this->height;
}

const int* Window::getWidth() const {
	return (const int*)&this->width;
}

GraphWindow::GraphWindow(int height, int width, int startx, int starty)
	: Window(height, width, startx, starty) {
}

void GraphWindow::addChoice(choice_t choice) {
		this->choices.insert(std::pair<unsigned short, choice_t>(choice.id, choice));
}

void GraphWindow::addChoices(const std::vector<choice_t>* newChoices) {
	std::vector<choice_t>::const_iterator it;
	if (newChoices->size() == 0) {
		return;
	}

	for (it = newChoices->begin(); it < newChoices->end(); it++) {
		this->choices.insert(std::pair<unsigned short, choice_t>(it->id, *it));
		this->addChoice(*it);
	}
}

choice_t* GraphWindow::getChoiceById(unsigned short id) {
	std::map<unsigned short, choice_t>::iterator it;
	it = this->choices.find(id);

	if (it == this->choices.end()) {
		return 0;
	}	

	return (choice_t*)&it->second;
}

void GraphWindow::drawGraphs() {
	std::map<unsigned short, choice_t>::iterator it;
	int i = 1;

	for (it = this->choices.begin(); it != this->choices.end(); it++) {
		// print caption
        wmove(this->window, i, 1);
        wclrtoeol(this->window);
        mvwprintw(this->window, i, 1, "%s", it->second.name.c_str());

        // draw progress bar
        wmove(this->window, i + 1, 1);
        wclrtoeol(this->window);
        int nrOfChars = (this->width - 2) / 100.0 * it->second.progress;
        mvwhline(this->window, i + 1, 1, ACS_CKBOARD, nrOfChars);

		i += 2;
	}

	this->refreshWindow();
}

void GraphWindow::updateProgressOfChoice(unsigned short choiceId, unsigned short progress) {
	std::map<unsigned short, choice_t>::iterator it = this->choices.find(choiceId);
	if (it == this->choices.end()) {
		// unknown choice id
		return;
	}

	it->second.progress = progress;
}
