#pragma once
#include <cstdint>

struct Screen {
	float width = 1920, height = 1080;
	float ratio = 1920.f / 1080.f;
	void update_resolution(int width, int height) {
		this->height = height;
		this->width = width;
		this->ratio = (float)width / (float)height;
	}
};

enum KeyCode : uint16_t {
	Backspace = 259,
	Left_Shift = 340,
	Right_Shift = 344,
	Caps_Lock = 280
};
struct Keyboard {
	bool keyStates[350]{};
};

struct Mouse {
	double x_pos = 0, y_pos = 0;
	float ortho_x_pos = 0, ortho_y_pos = 0;
	double last_x_pos = 0, last_y_pos = 0;
	float delta_x = 0, delta_y = 0;
	float wheel_offset = 0;
	bool left_button = false, right_button = false;
	void get_mouse_ortho_coords(Screen& screen) {
		ortho_x_pos = ((x_pos / screen.width) * 2.f - 1.f) * screen.ratio;
		ortho_y_pos = (y_pos / screen.height) * 2.f - 1.f;
	}
};

struct SystemContext {
	inline static Screen screen;
	inline static Keyboard keyBoard;
	inline static Mouse mouse;
};