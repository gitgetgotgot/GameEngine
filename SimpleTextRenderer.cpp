#include "SimpleTextRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include "TextureManager.h"

SimpleTextRenderer::SimpleTextRenderer(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	fontManager = FontManager::get_Instance();
	shader = renderer->createShader(ShaderDesc("", "Shaders/OpenGL/Basic Text Vert.glsl", "Shaders/OpenGL/Basic Text Frag.glsl"));
	shader->bind();

	vertexArray = renderer->createVertexArray();
	vertexArray->bind();

	vertexBuffer = renderer->createVertexBuffer();
	vertexBuffer->set_data(nullptr, MAX_CHARACTERS_PER_DRAW * 4 * sizeof(TextBasicVertex), renderer->STATIC_USAGE);

	vertexArray->add_attribute(VertexAttribute(0, 3, offsetof(TextBasicVertex, TextBasicVertex::pos), sizeof(TextBasicVertex)), vertexBuffer);
	vertexArray->add_attribute(VertexAttribute(1, 2, offsetof(TextBasicVertex, TextBasicVertex::UV), sizeof(TextBasicVertex)), vertexBuffer);
	vertexArray->add_attribute(VertexAttribute(2, 4, offsetof(TextBasicVertex, TextBasicVertex::color), sizeof(TextBasicVertex)), vertexBuffer);

	indexBuffer = renderer->createIndexBuffer();
	uint32_t* text_index_buffer = new uint32_t[MAX_CHARACTERS_PER_DRAW * 6];
	for (int i = 0; i < MAX_CHARACTERS_PER_DRAW; i++) {
		text_index_buffer[i * 6] = text_index_buffer[i * 6 + 3] = i * 4;
		text_index_buffer[i * 6 + 2] = text_index_buffer[i * 6 + 4] = text_index_buffer[i * 6] + 2;
		text_index_buffer[i * 6 + 1] = text_index_buffer[i * 6 + 2] - 1;
		text_index_buffer[i * 6 + 5] = text_index_buffer[i * 6 + 2] + 1;
	}
	indexBuffer->set_data(text_index_buffer, MAX_CHARACTERS_PER_DRAW * 6 * sizeof(uint32_t), renderer->STATIC_USAGE);
	delete[] text_index_buffer;
	vertexArray->unbind();
	indexBuffer->unbind();

	uniformBuffer = renderer->createUniformBuffer();
	BasicShaderUBData ubo;
	ubo.view = glm::mat4(1.f);
	float ratio = 1920.f / 1080.f;
	ubo.projection = glm::orthoLH(-ratio, ratio, -1.0f, 1.0f, 0.f, 10.f);
	uniformBuffer->bind(2);
	uniformBuffer->setData(&ubo, sizeof(BasicShaderUBData));

	setup_fonts();
}

void SimpleTextRenderer::setup_fonts() {
	std::string paths[] = {
			"Resources/Textures/Verdana.png",
			"Resources/Textures/Andy.png",
			"Resources/Textures/Minecraft.png",
			"Resources/Textures/Andy2k.png"
	};
	add_font(paths[0].c_str(), true, "Verdana");
	add_font(paths[1].c_str(), false, "Andy");
}

void SimpleTextRenderer::add_font(const char* fontPath, bool isSDF, std::string fontName) {
	fontManager->add_font_old(fontPath, isSDF, fontName);
}

void SimpleTextRenderer::update_text(const char* text, float letter_height, glm::vec3 bottom_left_position, glm::vec4 text_color, uint32_t fontID) {
	const int size = strlen(text);
	float stride = 0;
	text_size = size;
	TextBasicVertex* ptr = text_buffer;
	/*Glyph* glyphs = fontManager->get_font(fontID)->glyph_coords;
	for (int i = 0; i < size; i++) {
		Glyph& glyph = glyphs[int(text[i]) - 32];
		ptr->pos = { bottom_left_position.x + stride, bottom_left_position.y, bottom_left_position.z };
		ptr->UV = { glyph.Xmin, glyph.Ymin };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride, bottom_left_position.y + letter_height, bottom_left_position.z };
		ptr->UV = { glyph.Xmin, glyph.Ymax };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride + letter_height * glyph.height_ratio, bottom_left_position.y + letter_height, bottom_left_position.z };
		ptr->UV = { glyph.Xmax, glyph.Ymax };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride + letter_height * glyph.height_ratio, bottom_left_position.y, bottom_left_position.z };
		ptr->UV = { glyph.Xmax, glyph.Ymin };
		ptr->color = text_color;
		ptr++;

		stride += letter_height * glyph.height_ratio;
	}*/
}

void SimpleTextRenderer::add_text_to_buffer(const char* text, float letter_height, glm::vec3 bottom_left_position, glm::vec4 text_color, uint32_t fontID) {
	TextBasicVertex* ptr = text_buffer;
	ptr += text_size * 4;
	const int size = strlen(text);
	float stride = 0;
	text_size += size;
	/*Glyph* glyphs = fontManager->get_font(fontID)->glyph_coords;
	for (int i = 0; i < size; i++) {
		Glyph& glyph = glyphs[int(text[i]) - 32];
		ptr->pos = { bottom_left_position.x + stride, bottom_left_position.y, bottom_left_position.z };
		ptr->UV = { glyph.Xmin, glyph.Ymin };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride, bottom_left_position.y + letter_height, bottom_left_position.z };
		ptr->UV = { glyph.Xmin, glyph.Ymax };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride + letter_height * glyph.height_ratio, bottom_left_position.y + letter_height, bottom_left_position.z };
		ptr->UV = { glyph.Xmax, glyph.Ymax };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride + letter_height * glyph.height_ratio, bottom_left_position.y, bottom_left_position.z };
		ptr->UV = { glyph.Xmax, glyph.Ymin };
		ptr->color = text_color;
		ptr++;

		stride += letter_height * glyph.height_ratio;
	}*/
}

void SimpleTextRenderer::update_centered_text(const char* text, float letter_height, glm::vec3 center_position, glm::vec4 text_color, uint32_t fontID) {
	const int size = strlen(text);
	float stride = 0;
	text_size = size;
	/*Glyph* glyphs = fontManager->get_font(fontID)->glyph_coords;
	float totalWidth = 0.f;
	for (int i = 0; i < size; i++) {
		totalWidth += letter_height * glyphs[int(text[i]) - 32].height_ratio;
	}
	glm::vec2 bottom_left_position(center_position.x - totalWidth * 0.5f, center_position.y - letter_height * 0.5);
	TextBasicVertex* ptr = text_buffer;
	for (int i = 0; i < size; i++) {
		Glyph& glyph = glyphs[int(text[i]) - 32];
		ptr->pos = { bottom_left_position.x + stride, bottom_left_position.y, center_position.z };
		ptr->UV = { glyph.Xmin, glyph.Ymin };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride, bottom_left_position.y + letter_height, center_position.z };
		ptr->UV = { glyph.Xmin, glyph.Ymax };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride + letter_height * glyph.height_ratio, bottom_left_position.y + letter_height, center_position.z };
		ptr->UV = { glyph.Xmax, glyph.Ymax };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride + letter_height * glyph.height_ratio, bottom_left_position.y, center_position.z };
		ptr->UV = { glyph.Xmax, glyph.Ymin };
		ptr->color = text_color;
		ptr++;

		stride += letter_height * glyph.height_ratio;
	}*/
}

void SimpleTextRenderer::add_centered_text_to_buffer(const char* text, float letter_height, glm::vec3 center_position, glm::vec4 text_color, uint32_t fontID) {
	TextBasicVertex* ptr = text_buffer;
	ptr += text_size * 4;
	const int size = strlen(text);
	float stride = 0;
	text_size += size;
	float totalWidth = 0.f;
	/*Glyph* glyphs = fontManager->get_font(fontID)->glyph_coords;
	for (int i = 0; i < size; i++) {
		totalWidth += letter_height * glyphs[int(text[i]) - 32].height_ratio;
	}
	glm::vec2 bottom_left_position(center_position.x - totalWidth * 0.5f, center_position.y - letter_height * 0.5);
	for (int i = 0; i < size; i++) {
		Glyph& glyph = glyphs[int(text[i]) - 32];
		ptr->pos = { bottom_left_position.x + stride, bottom_left_position.y, center_position.z };
		ptr->UV = { glyph.Xmin, glyph.Ymin };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride, bottom_left_position.y + letter_height, center_position.z };
		ptr->UV = { glyph.Xmin, glyph.Ymax };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride + letter_height * glyph.height_ratio, bottom_left_position.y + letter_height, center_position.z };
		ptr->UV = { glyph.Xmax, glyph.Ymax };
		ptr->color = text_color;
		ptr++;
		ptr->pos = { bottom_left_position.x + stride + letter_height * glyph.height_ratio, bottom_left_position.y, center_position.z };
		ptr->UV = { glyph.Xmax, glyph.Ymin };
		ptr->color = text_color;
		ptr++;

		stride += letter_height * glyph.height_ratio;
	}*/
}

void SimpleTextRenderer::render_buffered_text(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	vertexBuffer->update_data(text_buffer, text_size * 4 * sizeof(TextBasicVertex));
	Font& font = *fontManager->get_font(0);
	TextureManager::get_Instance()->get(font.texture_id)->bind();
	renderer->renderIndexedData(shader, vertexArray, vertexBuffer, indexBuffer, text_size * 6);
	text_size = 0;
}
