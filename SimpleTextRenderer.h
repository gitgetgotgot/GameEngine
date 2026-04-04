#pragma once
#include "FontManager.h"
#include <memory>
#include "Renderer.h"

class SimpleTextRenderer {
public:
	SimpleTextRenderer(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);

	void setup_fonts();
	void add_font(const char* fontPath, bool isSDF, std::string fontName);

	//update buffer for just one single text
	void update_text(const char* text, float letter_height, glm::vec3 bottom_left_position, glm::vec4 text_color, uint32_t fontID);
	void update_centered_text(const char* text, float letter_height, glm::vec3 center_position, glm::vec4 text_color, uint32_t fontID);

	//allows to add any kind of text to buffer, then render all added text with 1 draw call
	void add_text_to_buffer(const char* text, float letter_height, glm::vec3 bottom_left_position, glm::vec4 text_color, uint32_t fontID);
	void add_centered_text_to_buffer(const char* text, float letter_height, glm::vec3 center_position, glm::vec4 text_color, uint32_t fontID);

	void render_buffered_text(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
private:
	
	struct TextBasicVertex {
		glm::vec3 pos;
		glm::vec2 UV;
		glm::vec4 color;
	};

	std::unique_ptr<Shader> shader;
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<UniformBuffer> uniformBuffer;

	int text_size = 0; //amount if indices needed
	static const int MAX_CHARACTERS_PER_DRAW = 500;
	TextBasicVertex text_buffer[4 * MAX_CHARACTERS_PER_DRAW];
	FontManager* fontManager;
};