#include <UI/UIComponents/SDF_Text.h>
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

void Engine::UI::SDF_Text::set_height(float font_size) {
	this->font_size = font_size;
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
	GlyphData* glyphsData = font->glyphsData;

	UI::UI_Transform& tr = *transform_ptr;
	float rect_left_x, rect_bottom_y;
	//setup vertical align (WORKS NOW ONLY FOR A 1 ROW TEXT)
	if (vertAlign == TextVerticalAlignment::vertCenter) {
		rect_bottom_y = font_size * 0.5f * font->descender;
	}
	else if (vertAlign == TextVerticalAlignment::vertBottom) {
		rect_bottom_y = font_size * 0.5f * (font->ascender + font->descender) - tr.size.y * 0.5f;
	}
	else {
		rect_bottom_y = tr.size.y * 0.5f - font_size * 0.5f * (font->ascender - font->descender);
	}

	float totalWidth = 0.f;
	for (int i = 0; i < size; i++) {
		GlyphData& glyph = glyphsData[int(text[i]) - 32];
		totalWidth += glyph.advance_normalized;
	}
	totalWidth *= font_size;

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
	int char_code;
	float baseline_y_offset, glyph_w, glyph_h;
	rect_left_x -= font_size * 0.15f;
	for (int i = 0; i < size; i++) {
		char_code = int(text[i]) - 32;

		GlyphData& glyph = glyphsData[char_code];
		glyph_w = font_size * glyph.width_normalized;
		glyph_h = font_size * glyph.height_normalized;
		baseline_y_offset = font_size * glyph.baselineY_normalized;

		data.emplace_back();
		Systems::UI_InstanceData& glyphData = data.back();

		glyphData.color = this->color;
		glyphData.texture_id = font->texture_id;
		glyphData.UV[0] = { glyph.U0, glyph.V0 };
		glyphData.UV[1] = { glyph.U0, glyph.V1 };
		glyphData.UV[2] = { glyph.U1, glyph.V1 };
		glyphData.UV[3] = { glyph.U1, glyph.V0 };

		glyphData.modelMatrix[3] = glm::vec4(
			rect_left_x + stride + glyph_w * 0.5f + glyph.bearingX_normalized * font_size,
			rect_bottom_y - baseline_y_offset, 0.0f, 1.0f); //translate
		glyphData.modelMatrix[0][0] = glyph_w; //scale X
		glyphData.modelMatrix[1][1] = glyph_h; //scale Y

		stride += glyph.advance_normalized * font_size;
	}
	isDirty = false;
}

void Engine::UI::SDF_Text::mark_dirty() {
	if (!isDirty) {
		isDirty = true;
		SDF_TextUpdater::dirtyTexts.emplace_back(obj_id);
	}
}