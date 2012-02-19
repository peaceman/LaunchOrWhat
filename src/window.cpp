#include "window.h"
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
