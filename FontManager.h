#pragma once
#include "SparseSet.h"
#include <cstdint>
#include <charconv>
#include <string>

struct Glyph {
	float Xmin, Xmax, Ymin, Ymax, height_ratio;
};

struct Font {
public:
	Glyph glyph_coords[95];
	uint32_t texture_id;
	float character_height = 0.0625;
	uint32_t id;
	std::string name;
};

class FontManager {
public:
	static FontManager* get_Instance() {
		static FontManager fontMgr;
		return &fontMgr;
	}
	Font* add_font(const char* fontPath, bool isSDF, std::string fontName);
	Font* get_font(uint32_t id);
	bool remove_font(uint32_t id);
private:
	FontManager() {}
	~FontManager() {}
	sparse_set<Font> fonts;
	std::vector<uint32_t> free_ids;
};