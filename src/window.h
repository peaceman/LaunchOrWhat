#include <ncurses.h>

class Window {
	public:
		Window(int height, int width, int startx = 0, int starty = 0);
		~Window();
		WINDOW* getWindow();
		void refreshWindow() const;
		const int* getHeight() const;
		const int* getWidth() const;
	private:
		int height;
		int width;
		int startx;
		int starty;
		WINDOW* window;
};
