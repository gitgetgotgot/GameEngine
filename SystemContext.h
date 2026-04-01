#pragma once
#include <cstdint>
#include <GLFW/glfw3.h>

struct Screen {
	float width = 1920, height = 1080;
	float ratio = 1920.f / 1080.f;
	void update_resolution(int width, int height) {
		this->height = height;
		this->width = width;
		this->ratio = (float)width / (float)height;
	}
};

enum Key : uint16_t {
    KeyUnknown = -1,

    KeySpace = GLFW_KEY_SPACE,
    KeyApostrophe = GLFW_KEY_APOSTROPHE,
    KeyComma = GLFW_KEY_COMMA,
    KeyMinus = GLFW_KEY_MINUS,
    KeyPeriod = GLFW_KEY_PERIOD,
    KeySlash = GLFW_KEY_SLASH,

    Key0 = GLFW_KEY_0,
    Key1 = GLFW_KEY_1,
    Key2 = GLFW_KEY_2,
    Key3 = GLFW_KEY_3,
    Key4 = GLFW_KEY_4,
    Key5 = GLFW_KEY_5,
    Key6 = GLFW_KEY_6,
    Key7 = GLFW_KEY_7,
    Key8 = GLFW_KEY_8,
    Key9 = GLFW_KEY_9,

    KeySemicolon = GLFW_KEY_SEMICOLON,
    KeyEqual = GLFW_KEY_EQUAL,

    KeyA = GLFW_KEY_A,
    KeyB = GLFW_KEY_B,
    KeyC = GLFW_KEY_C,
    KeyD = GLFW_KEY_D,
    KeyE = GLFW_KEY_E,
    KeyF = GLFW_KEY_F,
    KeyG = GLFW_KEY_G,
    KeyH = GLFW_KEY_H,
    KeyI = GLFW_KEY_I,
    KeyJ = GLFW_KEY_J,
    KeyK = GLFW_KEY_K,
    KeyL = GLFW_KEY_L,
    KeyM = GLFW_KEY_M,
    KeyN = GLFW_KEY_N,
    KeyO = GLFW_KEY_O,
    KeyP = GLFW_KEY_P,
    KeyQ = GLFW_KEY_Q,
    KeyR = GLFW_KEY_R,
    KeyS = GLFW_KEY_S,
    KeyT = GLFW_KEY_T,
    KeyU = GLFW_KEY_U,
    KeyV = GLFW_KEY_V,
    KeyW = GLFW_KEY_W,
    KeyX = GLFW_KEY_X,
    KeyY = GLFW_KEY_Y,
    KeyZ = GLFW_KEY_Z,

    KeyLeftBracket = GLFW_KEY_LEFT_BRACKET,
    KeyBackslash = GLFW_KEY_BACKSLASH,
    KeyRightBracket = GLFW_KEY_RIGHT_BRACKET,
    KeyGraveAccent = GLFW_KEY_GRAVE_ACCENT,

    KeyEscape = GLFW_KEY_ESCAPE,
    KeyEnter = GLFW_KEY_ENTER,
    KeyTab = GLFW_KEY_TAB,
    KeyBackspace = GLFW_KEY_BACKSPACE,
    KeyInsert = GLFW_KEY_INSERT,
    KeyDelete = GLFW_KEY_DELETE,
    KeyRight = GLFW_KEY_RIGHT,
    KeyLeft = GLFW_KEY_LEFT,
    KeyDown = GLFW_KEY_DOWN,
    KeyUp = GLFW_KEY_UP,

    KeyF1 = GLFW_KEY_F1,
    KeyF2 = GLFW_KEY_F2,
    KeyF3 = GLFW_KEY_F3,
    KeyF4 = GLFW_KEY_F4,
    KeyF5 = GLFW_KEY_F5,
    KeyF6 = GLFW_KEY_F6,
    KeyF7 = GLFW_KEY_F7,
    KeyF8 = GLFW_KEY_F8,
    KeyF9 = GLFW_KEY_F9,
    KeyF10 = GLFW_KEY_F10,
    KeyF11 = GLFW_KEY_F11,
    KeyF12 = GLFW_KEY_F12,
};

struct Keyboard {
	bool keyStatesCurr[349]{};
	bool keyStatesPrev[349]{};

	bool key_is_pressed(int key) const { return !keyStatesPrev[key] && keyStatesCurr[key]; }
	bool key_is_released(int key) const { return keyStatesPrev[key] && !keyStatesCurr[key]; }
	bool key_is_held(int key) const { return keyStatesPrev[key] && keyStatesCurr[key]; }
	bool key_is_idle(int key) const { return !keyStatesPrev[key] && !keyStatesCurr[key]; }
};

struct Mouse {
	double x_pos = 0, y_pos = 0;
	float ortho_x_pos = 0, ortho_y_pos = 0;
	double last_x_pos = 0, last_y_pos = 0;
	float delta_x = 0, delta_y = 0;
	float wheel_offset = 0;
	void get_mouse_ortho_coords(Screen& screen) {
		ortho_x_pos = ((x_pos / screen.width) * 2.f - 1.f) * screen.ratio;
		ortho_y_pos = (y_pos / screen.height) * 2.f - 1.f;
	}
	bool lb_curr = false, lb_prev = false;
	bool rb_curr = false, rb_prev = false;

	bool lb_is_pressed() const { return !lb_prev && lb_curr; }
	bool lb_is_released() const { return lb_prev && !lb_curr; }
	bool lb_is_held() const { return lb_prev && lb_curr; }
	bool lb_is_idle() const { return !lb_prev && !lb_curr; }

	bool rb_is_pressed() const { return !rb_prev && rb_curr; }
	bool rb_is_released() const { return rb_prev && !rb_curr; }
	bool rb_is_held() const { return rb_prev && rb_curr; }
	bool rb_is_idle() const { return !rb_prev && !rb_curr; }
};

struct SystemContext {
	inline static Screen screen;
	inline static Keyboard keyBoard;
	inline static Mouse mouse;
};