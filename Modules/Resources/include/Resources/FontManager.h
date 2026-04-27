#pragma once
#include <Utility/SparseSet.h>
#include <cstdint>
#include <charconv>
#include <string>

constexpr char baseFontsDirectory[] = "Resources/Text/Fonts/";
constexpr char SDFFontAtlasesDirectory[] = "Resources/Text/SDF_Font_Atlases/";

class GlyphData {
	//default values are for SPACE(32)
public:
	float U0 = 0.0f, V0 = 0.0f, U1 = 0.0f, V1 = 0.0f;
	float advance_normalized = 0.5f;
	float bearingX_normalized = 0.0f;
	float bearingY_normalized = 0.0f;
	float baselineY_normalized = 0.0f;
	float height_normalized = 0.0f;
	float width_normalized = 0.0f;
};

struct Font {
public:
	uint32_t id;
	std::string name;
	uint32_t texture_id;
	GlyphData glyphsData[95]; //[32, 126]
	float ascender;
	float descender;
};

class FontManager {
public:
	static FontManager* get_Instance() {
		static FontManager fontMgr;
		return &fontMgr;
	}
	Font* add_font_old(const char* fontPath, bool isSDF, std::string fontName); //bad deprecated method
	bool generate_truetype_sdf_font(const char* fontPath, bool write_atlas_to_png);
	Font* load_sdf_font(const char* fontDataPath, std::string fontName);
	Font* get_font(uint32_t id);
	bool remove_font(uint32_t id);
private:
	FontManager() {}
	~FontManager() {}
	sparse_set<Font> fonts;
	std::vector<uint32_t> free_ids;
};