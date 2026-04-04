#include "Button.h"
#include "SystemContext.h"

void Engine::UI::Button::set_callback(void(*staticCallbackWrapper)(void*), void* callback_obj) {
	this->callback = staticCallbackWrapper;
	this->callback_object = callback_obj;
}

void Engine::UI::Button::set_sprite(Sprite& sprite) {
	this->sprite = &sprite;
	mark_dirty();
}

void Engine::UI::Button::set_color(glm::vec4 color) {
	this->color = color;
	mark_dirty();
}

void Engine::UI::Button::on_click() {
	if (callback) callback(callback_object);
}

void Engine::UI::Button::_internal_update_ui_data()
{
	isDirty = false;
	data.color = this->color;
	data.texture_id = sprite->texture_id;
	//default
	data.UV[0] = sprite->UV[0];
	data.UV[1] = sprite->UV[1];
	data.UV[2] = sprite->UV[2];
	data.UV[3] = sprite->UV[3];
}

void Engine::UI::Button::_internal_update() {
	if (!transform_ptr) return;

	UI::UI_Transform& tr = *transform_ptr;
	float rect_left_x = tr.position.x - tr.size.x * tr.pivot.x;
	float rect_bottom_y = tr.position.y - tr.size.y * tr.pivot.y;
	float mouse_x = SystemContext::mouse.ortho_x_pos;
	float mouse_y = SystemContext::mouse.ortho_y_pos;
	isPointed = false;
	if (rect_left_x < mouse_x && mouse_x < rect_left_x + tr.size.x &&
		rect_bottom_y < mouse_y && mouse_y < rect_bottom_y + tr.size.y) {
		isPointed = true;
	}
	if (isPointed && SystemContext::mouse.lb_is_pressed()) {
		on_click();
	}
}

void Engine::UI::Button::mark_dirty() {
	if (!isDirty) {
		isDirty = true;
		ButtonUpdater::dirtyButtons.emplace_back(obj_id);
	}
}
