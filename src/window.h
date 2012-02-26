#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>
#include "Message.h"

class Window {
	public:
		Window(int height, int width, int startx = 0, int starty = 0);
		~Window();
		WINDOW* getWindow();
		void refreshWindow() const;
		const int* getHeight() const;
		const int* getWidth() const;
	protected:
		int height;
		int width;
		int startx;
		int starty;
		WINDOW* window;
};

class GraphWindow : Window {
	protected:
		std::map<unsigned short, choice_t> choices;
	public:
		GraphWindow(int height, int width, int startx = 0, int starty = 0);
		void drawGraphs();
		void addChoice(choice_t choice);
		choice_t* getChoiceById(unsigned short id);
};

#endif
