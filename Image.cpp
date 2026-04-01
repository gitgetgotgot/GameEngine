#include "Image.h"

void Engine::UI::Image::set_sprite(Sprite* sprite) {
	this->sprite = sprite;
	mark_dirty();
}

void Engine::UI::Image::set_color(glm::vec4 color) {
	this->color = color;
	mark_dirty();
}

void Engine::UI::Image::flipX(bool is_flipped) {
	this->flip_X = is_flipped;
	mark_dirty();
}

void Engine::UI::Image::flipY(bool is_flipped) {
	this->flip_Y = is_flipped;
	mark_dirty();
}

void Engine::UI::Image::mark_dirty() {
	if (!isDirty) {
		isDirty = true;
		ImageUpdater::dirtyImages.emplace_back(obj_id);
	}
}

void Engine::UI::Image::update_model_matrix() {
	this->data.modelMatrix = transform_ptr->get_world_matrix();
}

void Engine::UI::Image::update_ui_data() {
	isDirty = false;
	data.color = this->color;
	if (flip_X) {
		if (flip_Y) {
			//Y and X are flipped
			data.UV[0] = sprite->UV[2];
			data.UV[1] = sprite->UV[3];
			data.UV[2] = sprite->UV[0];
			data.UV[3] = sprite->UV[1];
		}
		else {
			//X is flipped
			data.UV[0] = sprite->UV[3];
			data.UV[1] = sprite->UV[2];
			data.UV[2] = sprite->UV[1];
			data.UV[3] = sprite->UV[0];
		}
	}
	else {
		if (flip_Y) {
			//Y is flipped
			data.UV[0] = sprite->UV[1];
			data.UV[1] = sprite->UV[0];
			data.UV[2] = sprite->UV[3];
			data.UV[3] = sprite->UV[2];
		}
		else {
			//default
			data.UV[0] = sprite->UV[0];
			data.UV[1] = sprite->UV[1];
			data.UV[2] = sprite->UV[2];
			data.UV[3] = sprite->UV[3];
		}
	}
}
