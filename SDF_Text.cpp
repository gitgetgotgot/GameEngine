#include "SDF_Text.h"
#include <glm/gtc/matrix_transform.hpp>

void Engine::UI::SDF_Text::set_color(glm::vec4 color) {
	this->color = color;
	mark_dirty();
}

void Engine::UI::SDF_Text::set_horiz_align(TextHorizontalAlignment type) {
	horizAlign = type;
	mark_dirty();
}

void Engine::UI::SDF_Text::set_vert_align(TextVerticalAlignment type) {
	vertAlign = type;
	mark_dirty();
}

void Engine::UI::SDF_Text::set_height(float height) {
	this->height = height;
	mark_dirty();
}

void Engine::UI::SDF_Text::set_text(std::string& text) {
	this->text = text;
	mark_dirty();
}

void Engine::UI::SDF_Text::set_text(const char* text) {
	this->text = text;
	mark_dirty();
}

void Engine::UI::SDF_Text::set_font(uint32_t id) {
	this->font_id = id;
	mark_dirty();
}

void Engine::UI::SDF_Text::_internal_update_ui_data() {
	if (!transform_ptr) return;

	data.resize(0);
	const int size = text.size();
	float stride = 0;
	Font* font = FontManager::get_Instance()->get_font(font_id);
	Glyph* glyphs = font->glyph_coords;

	UI::UI_Transform& tr = *transform_ptr;
	float rect_left_x, rect_bottom_y;
	//setup vertical align (WORKS NOW ONLY FOR A 1 ROW TEXT)
	if (vertAlign == TextVerticalAlignment::vertCenter) {
		rect_bottom_y = 0.f;
	}
	else if (vertAlign == TextVerticalAlignment::vertBottom) {
		rect_bottom_y = (height - tr.size.y) * 0.5f;
	}
	else {
		rect_bottom_y = (tr.size.y - height) * 0.5f;
	}

	float totalWidth = 0.f;
	for (int i = 0; i < size; i++) {
		totalWidth += height * glyphs[int(text[i]) - 32].height_ratio;
	}
	//setup horizontal align (WORKS NOW ONLY FOR A 1 ROW TEXT)
	if (horizAlign == TextHorizontalAlignment::horizCenter) {
		rect_left_x = -totalWidth * 0.5f;
	}
	else if (horizAlign == TextHorizontalAlignment::horizLeft) {
		rect_left_x = -0.5f * tr.size.x;
	}
	else {
		rect_left_x = 0.5f * tr.size.x - totalWidth;
	}
	//set glyphs data
	for (int i = 0; i < size; i++) {
		Glyph& glyph = glyphs[int(text[i]) - 32];
		Systems::UI_InstanceData glyphData;
		glyphData.color = this->color;
		glyphData.texture_id = font->texture_id;
		glyphData.UV[0] = { glyph.Xmin, glyph.Ymin };
		glyphData.UV[1] = { glyph.Xmin, glyph.Ymax };
		glyphData.UV[2] = { glyph.Xmax, glyph.Ymax };
		glyphData.UV[3] = { glyph.Xmax, glyph.Ymin };
		glyphData.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(rect_left_x + stride + height * glyph.height_ratio * 0.5f, rect_bottom_y, 0.f));
		glyphData.modelMatrix = glm::scale(glyphData.modelMatrix, glm::vec3(height * glyph.height_ratio, height, 0.f));
		data.emplace_back(glyphData);
		stride += height * glyph.height_ratio;
	}
	isDirty = false;
}

void Engine::UI::SDF_Text::mark_dirty() {
	if (!isDirty) {
		isDirty = true;
		SDF_TextUpdater::dirtyTexts.emplace_back(obj_id);
	}
}