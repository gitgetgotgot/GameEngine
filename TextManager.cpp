#include "TextManager.h"
#include <glm/gtc/type_ptr.hpp>

TextManager::TextManager(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
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

	setup_fonts(renderer);
}

TextManager::~TextManager() {
	
}

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
//#include <string>

static inline bool inBounds(int x, int y, int w, int h) {
	return (x >= 0 && x < w && y >= 0 && y < h);
}

void buildSDF_RGBA8(
	const uint8_t* srcRGBA,  // входная текстура (RGBA8), размер w*h*4
	int w, int h,
	int checkByte,           // индекс канала (0 = R), из которого берём маску
	std::vector<uint8_t>& outRGBA // выход RGBA8, размер w*h*4
) {
	outRGBA.resize(w * h * 4);

	// 1) Бинарная маска: foreground = пиксель буквы (true), background = фон (false)
	std::vector<uint8_t> mask(w * h);
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			int idx = (y * w + x) * 3 + checkByte;
			mask[y * w + x] = (srcRGBA[idx] > 127) ? 1 : 0;
		}
	}

	// 2) Два поля расстояний (float), инициализируем INF
	const float INF = 1e9f;
	std::vector<float> distToBg(w * h, INF); // расстояние до фона
	std::vector<float> distToFg(w * h, INF); // расстояние до «буквы»
	const float W1 = 1.0f;                   // ортогональный шаг
	const float W2 = 1.41421356237f;         // диагональный шаг (≈ sqrt(2))

	// Сеем нули: для distToBg — где фон, для distToFg — где буква
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			bool fg = mask[y * w + x] != 0;
			if (!fg) distToBg[y * w + x] = 0.0f;
			if (fg) distToFg[y * w + x] = 0.0f;
		}
	}

	auto relaxForward = [&](std::vector<float>& d) {
		for (int y = 0; y < h; ++y) {
			for (int x = 0; x < w; ++x) {
				float v = d[y * w + x];
				// Left
				if (inBounds(x - 1, y, w, h)) v = std::min(v, d[y * w + (x - 1)] + W1);
				// Up
				if (inBounds(x, y - 1, w, h)) v = std::min(v, d[(y - 1) * w + x] + W1);
				// Up-left
				if (inBounds(x - 1, y - 1, w, h)) v = std::min(v, d[(y - 1) * w + (x - 1)] + W2);
				// Up-right
				if (inBounds(x + 1, y - 1, w, h)) v = std::min(v, d[(y - 1) * w + (x + 1)] + W2);
				d[y * w + x] = v;
			}
		}
		};
	auto relaxBackward = [&](std::vector<float>& d) {
		for (int y = h - 1; y >= 0; --y) {
			for (int x = w - 1; x >= 0; --x) {
				float v = d[y * w + x];
				// Right
				if (inBounds(x + 1, y, w, h)) v = std::min(v, d[y * w + (x + 1)] + W1);
				// Down
				if (inBounds(x, y + 1, w, h)) v = std::min(v, d[(y + 1) * w + x] + W1);
				// Down-right
				if (inBounds(x + 1, y + 1, w, h)) v = std::min(v, d[(y + 1) * w + (x + 1)] + W2);
				// Down-left
				if (inBounds(x - 1, y + 1, w, h)) v = std::min(v, d[(y + 1) * w + (x - 1)] + W2);
				d[y * w + x] = v;
			}
		}
		};

	// 3) Двухпроходная релаксация — получаем аппроксимацию евклидового расстояния
	relaxForward(distToBg);
	relaxBackward(distToBg);
	relaxForward(distToFg);
	relaxBackward(distToFg);

	// 4) Собираем signed distance: внутри — +distToBg, снаружи — -distToFg
	// Нормализуем к [0,1]: 0.5 — контур. Выбери "spread" как радиус (в пикселях), напр. 16.
	const float spread = 16.0f;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			bool fg = mask[y * w + x] != 0;
			float dSigned = fg ? (-distToBg[y * w + x]) : (+distToFg[y * w + x]);

			float v = 0.5f + (dSigned / spread);
			v = std::clamp(v, 0.0f, 1.0f);

			uint8_t r = static_cast<uint8_t>(std::round(v * 255.0f));
			int idx = (y * w + x) * 4;
			outRGBA[idx + 0] = r;      // R = SDF
			outRGBA[idx + 1] = 255;    // G
			outRGBA[idx + 2] = 255;    // B
			outRGBA[idx + 3] = 255;    // A
		}
	}
}

void computeLocalSDF(
	const uint8_t* src,         // исходный bitmap (RGB или RGBA)
	int atlasW, int atlasH,     // размеры атласа (1024×1024)
	int numOfChannels,          // 3 или 4
	int glyphSize,              // 64
	std::vector<uint8_t>& out   // выходной RGBA8, размер atlasW * atlasH * 4
) {
	int glyphsX = atlasW / glyphSize;
	int glyphsY = atlasH / glyphSize;
	out.resize(atlasW * atlasH * 4);

	for (int gy = 0; gy < glyphsY; ++gy) {
		for (int gx = 0; gx < glyphsX; ++gx) {
			// Область текущего глифа
			int baseX = gx * glyphSize;
			int baseY = gy * glyphSize;

			// Собираем маску: 1 = чёрный пиксель, 0 = фон
			std::vector<uint8_t> mask(glyphSize * glyphSize);
			for (int y = 0; y < glyphSize; ++y) {
				for (int x = 0; x < glyphSize; ++x) {
					int atlasIdx = ((baseY + y) * atlasW + (baseX + x)) * numOfChannels;
					uint8_t r = src[atlasIdx]; // предполагаем, что R = яркость
					mask[y * glyphSize + x] = (r < 10) ? 1 : 0;
				}
			}

			// Для каждого пикселя считаем расстояние до ближайшего чёрного
			for (int y = 0; y < glyphSize; ++y) {
				for (int x = 0; x < glyphSize; ++x) {
					int minDist2 = std::numeric_limits<int>::max();
					for (int yy = 0; yy < glyphSize; ++yy) {
						for (int xx = 0; xx < glyphSize; ++xx) {
							if (mask[yy * glyphSize + xx]) {
								int dx = x - xx;
								int dy = y - yy;
								int dist2 = dx * dx + dy * dy;
								if (dist2 < minDist2) minDist2 = dist2;
							}
						}
					}
					float dist = std::sqrt((float)minDist2);
					float spread = 32.0f; // нормализация
					float v = dist / spread;
					if (v > 1.0f) v = 1.0f;

					uint8_t sdfByte = static_cast<uint8_t>(std::round(v * 255.0f));

					// Записываем в выходной RGBA
					int atlasIdx = ((baseY + y) * atlasW + (baseX + x)) * 4;
					out[atlasIdx + 0] = sdfByte; // R = расстояние
					out[atlasIdx + 1] = 255;
					out[atlasIdx + 2] = 255;
					out[atlasIdx + 3] = 255;
				}
			}
		}
	}
}


void computeGlyphSDFCell(
	const uint8_t* src,   // input atlas pixels (RGB or RGBA)
	int atlasW, int atlasH,
	int numChannels,      // 3 or 4
	int cellSize,         // 64
	int cellX, int cellY, // glyph index in grid (e.g., 1, 2)
	uint8_t blackThreshold, // e.g., 10: R < 10 => black (glyph)
	float spreadPixels,     // e.g., 24.0f
	std::vector<uint8_t>& outRGBA // output atlas RGBA8; must be atlasW*atlasH*4
) {
	// Validate and prepare output
	if (outRGBA.size() != static_cast<size_t>(atlasW * atlasH * 4))
		outRGBA.assign(atlasW * atlasH * 4, 255);

	// Compute region bounds
	int baseX = cellX * cellSize;
	int baseY = cellY * cellSize;
	if (!inBounds(baseX, baseY, atlasW, atlasH) ||
		!inBounds(baseX + cellSize - 1, baseY + cellSize - 1, atlasW, atlasH))
		return; // out of atlas bounds

	int w = cellSize, h = cellSize;
	const float INF = 1e9f;
	const float W1 = 1.0f;               // orthogonal step
	const float W2 = 1.41421356237f;     // diagonal step

	// 1) Build binary mask of the glyph (inside: 1, background: 0)
	std::vector<uint8_t> mask(w * h);
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			int sx = baseX + x;
			int sy = baseY + y;
			int sidx = (sy * atlasW + sx) * numChannels;
			uint8_t r = src[sidx]; // assume R encodes coverage/ink
			mask[y * w + x] = (r < blackThreshold) ? 1 : 0;
		}
	}

	// 2) Two distance fields: to background and to foreground
	std::vector<float> distToBg(w * h, INF);
	std::vector<float> distToFg(w * h, INF);

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			bool fg = mask[y * w + x] != 0;
			if (!fg) distToBg[y * w + x] = 0.0f; // seed background
			if (fg) distToFg[y * w + x] = 0.0f; // seed foreground
		}
	}

	auto relaxForward = [&](std::vector<float>& d) {
		for (int y = 0; y < h; ++y) {
			for (int x = 0; x < w; ++x) {
				float v = d[y * w + x];
				if (x - 1 >= 0)           v = std::min(v, d[y * w + (x - 1)] + W1);
				if (y - 1 >= 0)           v = std::min(v, d[(y - 1) * w + x] + W1);
				if (x - 1 >= 0 && y - 1 >= 0) v = std::min(v, d[(y - 1) * w + (x - 1)] + W2);
				if (x + 1 < w && y - 1 >= 0) v = std::min(v, d[(y - 1) * w + (x + 1)] + W2);
				d[y * w + x] = v;
			}
		}
		};
	auto relaxBackward = [&](std::vector<float>& d) {
		for (int y = h - 1; y >= 0; --y) {
			for (int x = w - 1; x >= 0; --x) {
				float v = d[y * w + x];
				if (x + 1 < w)           v = std::min(v, d[y * w + (x + 1)] + W1);
				if (y + 1 < h)           v = std::min(v, d[(y + 1) * w + x] + W1);
				if (x + 1 < w && y + 1 < h) v = std::min(v, d[(y + 1) * w + (x + 1)] + W2);
				if (x - 1 >= 0 && y + 1 < h) v = std::min(v, d[(y + 1) * w + (x - 1)] + W2);
				d[y * w + x] = v;
			}
		}
		};

	// 3) Chamfer relaxation (approximate Euclidean distance)
	relaxForward(distToBg);
	relaxBackward(distToBg);
	relaxForward(distToFg);
	relaxBackward(distToFg);

	// 4) Compose signed distance and write back to out atlas RGBA
	float spread = (spreadPixels > 0.0f) ? spreadPixels : float(cellSize) * 0.4f; // fall back
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			bool fg = mask[y * w + x] != 0;
			float dSigned = fg ? (+distToBg[y * w + x]) : (-distToFg[y * w + x]);

			float v = 0.5f + (dSigned / spread);
			v = std::max(0.0f, std::min(1.0f, v));

			uint8_t r = static_cast<uint8_t>(std::round(v * 255.0f));

			int dx = baseX + x;
			int dy = baseY + y;
			int didx = (dy * atlasW + dx) * 4;
			outRGBA[didx + 0] = r;    // R = SDF
			outRGBA[didx + 1] = 255;
			outRGBA[didx + 2] = 255;
			outRGBA[didx + 3] = 255;
		}
	}
}

void TextManager::setup_fonts(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	std::string paths[] = {
			"Resources/Textures/Verdana.png",
			"Resources/Textures/Andy.png",
			"Resources/Textures/Minecraft.png",
			"Resources/Textures/Andy2k.png"
	};
	add_font(paths[0].c_str(), true, "Verdana", renderer);
	add_font(paths[1].c_str(), false, "Andy", renderer);
}

void TextManager::add_font(const char* fontPath, bool isSDF, std::string fontName, std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	fontManager.add_font(fontPath, isSDF, fontName, renderer);
}

void TextManager::update_text(const char* text, float letter_height, glm::vec3 bottom_left_position, glm::vec4 text_color, std::string fontName) {
	const int size = strlen(text);
	float stride = 0;
	text_size = size;
	TextBasicVertex* ptr = text_buffer;
	Glyph* glyphs = fontManager.get_font(fontName).glyph_coords;
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
	}
}

void TextManager::add_text_to_buffer(const char* text, float letter_height, glm::vec3 bottom_left_position, glm::vec4 text_color, std::string fontName) {
	TextBasicVertex* ptr = text_buffer;
	ptr += text_size * 4;
	const int size = strlen(text);
	float stride = 0;
	text_size += size;
	Glyph* glyphs = fontManager.get_font(fontName).glyph_coords;
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
	}
}

void TextManager::update_centered_text(const char* text, float letter_height, glm::vec3 center_position, glm::vec4 text_color, std::string fontName) {
	const int size = strlen(text);
	float stride = 0;
	text_size = size;
	Glyph* glyphs = fontManager.get_font(fontName).glyph_coords;
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
	}
}

void TextManager::add_centered_text_to_buffer(const char* text, float letter_height, glm::vec3 center_position, glm::vec4 text_color, std::string fontName) {
	TextBasicVertex* ptr = text_buffer;
	ptr += text_size * 4;
	const int size = strlen(text);
	float stride = 0;
	text_size += size;
	float totalWidth = 0.f;
	Glyph* glyphs = fontManager.get_font(fontName).glyph_coords;
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
	}
}

void TextManager::render_buffered_text(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	vertexBuffer->update_data(text_buffer, text_size * 4 * sizeof(TextBasicVertex));
	Font& font = fontManager.get_font("Verdana");
	font.texture->bind();
	renderer->renderIndexedData(shader, vertexArray, vertexBuffer, indexBuffer, text_size * 6);
	text_size = 0;
}
