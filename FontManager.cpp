#include "FontManager.h"
#include "stb/stb_image.h"
#include "TextureManager.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <FreeType/ft2build.h>
#include FT_FREETYPE_H
#include <stb_image_write.h>

Font* FontManager::add_font_old(const char* fontPath, bool isSDF, std::string fontName) {
	/*
	uint32_t id;
	if (free_ids.size()) {
		id = free_ids.back();
		free_ids.pop_back();
	}
	else id = fonts.size();
	Font& font = *fonts.add(Font{}, id);
	font.id = id;
	font.name = fontName;
	Glyph* glyphs = font.glyph_coords;

	int imgWidth, imgHeight, numOfChannels;
	stbi_set_flip_vertically_on_load(false);
	//load text atlas data from file, which should be a square (f.e. 1024x1024)

	unsigned char* image_bytes = stbi_load(fontPath, &imgWidth, &imgHeight, &numOfChannels, 0);

	//SETUP GLYPHS
	const int characters_on_line = 16; //16 by 16 atlas grid
	font.character_height = 1 / (float)characters_on_line;
	const int checkByte = 0;  //check R byte
	const float right_border_offset_percentage = 0.05f;
	int pixelsPerChar = imgHeight / characters_on_line;
	int Xstart{}, Ystart{};
	int Xpixel{}, Ypixel{};
	int xi{}, yj{};
	unsigned char red_byte{};
	for (int i = 0; i < 95; i++) { //from 0 to 94 (int(char) - 32) [from ASCII 32 to 127] english only
		Ystart = i / characters_on_line;
		Xstart = i % characters_on_line;
		Xpixel = Xstart * pixelsPerChar;
		Ypixel = Ystart * pixelsPerChar;

		for (xi = (Xpixel + pixelsPerChar - 1); xi > Xpixel; xi--) {     //calculate right border of character
			for (yj = (Ypixel + pixelsPerChar - 1); yj > Ypixel; yj--) { //calculate bottom border of character
				red_byte = image_bytes[(yj * imgHeight + xi) * numOfChannels + checkByte]; //check red byte
				if (red_byte < 1) break;
			}
			if (red_byte < 1) break;
		}
		xi += pixelsPerChar * right_border_offset_percentage; //add offset after right border
		if (xi > Xpixel + pixelsPerChar - 1) xi = Xpixel + pixelsPerChar - 1; //correct right border if needed
		if (i == 0) xi = Xstart + pixelsPerChar / 2;

		glyphs[i].Xmin = Xstart * font.character_height;
		glyphs[i].Xmax = Xstart * font.character_height + font.character_height * (xi - Xpixel) / (float)pixelsPerChar; //right border based on dx pixels
		glyphs[i].Ymin = Ystart * font.character_height + font.character_height;
		glyphs[i].Ymax = Ystart * font.character_height;
		glyphs[i].height_ratio = (xi - Xpixel) / (float)pixelsPerChar;
	}

	//SETUP TEXT TEXTURE
	unsigned char* img_data = new unsigned char[imgWidth * imgHeight] {255};
	if (isSDF) { //SDF IMITATION
		bool pixel_is_black;
		int black_pixels_nearby;
		int areaRadius = 2; // 1 -> 3x3, 2 -> 5x5, 3 -> 7x7
		int maxNeighbors = (2 * areaRadius + 1) * (2 * areaRadius + 1) - 1;

		for (int x = areaRadius; x < imgHeight - areaRadius; x++) {
			for (int y = areaRadius; y < imgHeight - areaRadius; y++) {
				pixel_is_black = (image_bytes[(y * imgHeight + x) * numOfChannels + checkByte] < 1);
				black_pixels_nearby = 0;

				for (int i = x - areaRadius; i <= x + areaRadius; i++) {
					for (int j = y - areaRadius; j <= y + areaRadius; j++) {
						if (i == x && j == y) continue;
						if (image_bytes[(j * imgHeight + i) * numOfChannels + checkByte] < 1)
							black_pixels_nearby++;
					}
				}
				if (pixel_is_black) { //black pixel
					float v = 0.4f + (black_pixels_nearby / float(maxNeighbors)) * 0.6f;
					img_data[(y * imgHeight + x)] = static_cast<uint8_t>(std::round(v * 255.0f));
				}
				else { // white pixel
					if (black_pixels_nearby > 0) {
						float v = 0.6f - (1.f / black_pixels_nearby) * 0.6f;
						img_data[(y * imgHeight + x)] = static_cast<uint8_t>(std::round(v * 255.0f));
					}
					else {
						img_data[(y * imgHeight + x)] = 0;
					}
				}
			}
		}
	}
	else {
		//just check if pixel is white or black
		for (int i = 0; i < imgWidth; i++) {
			for (int j = 0; j < imgHeight; j++) {
				img_data[(j * imgWidth + i)] = image_bytes[(j * imgWidth + i) * numOfChannels + checkByte] > 0 ? 0 : 255;
			}
		}
	}
	font.texture_id = TextureManager::get_Instance()->create_texture_from_data(img_data, imgWidth, imgHeight, 1, false)->texture_id;
	delete[] img_data;
	stbi_image_free(image_bytes);
	return fonts.get(id);

	*/
	return nullptr;
}

static uint8_t get_minimal_distance_value(int start_x, int start_y, float spread, FT_Bitmap& bmp) {
	int width = bmp.width;
	int height = bmp.rows;
	float min_dist_square = spread * spread; //set min to max possible

	uint8_t value;
	//if this pixel is outside if bitmap range, then it's value is 0
	if (start_x < 0 || start_x >= width || start_y < 0 || start_y >= height) {
		value = 0;
	}
	else {
		value = bmp.buffer[start_y * width + start_x] == 0 ? 0 : 1;
	}

	//set bounds and correct them
	int min_x = start_x - spread;
	int max_x = start_x + spread;
	int min_y = start_y - spread;
	int max_y = start_y + spread;
	if (min_x < 0) min_x = 0;
	if (max_x >= width) max_x = width - 1;
	if (min_y < 0) min_y = 0;
	if (max_y >= height) max_y = height - 1;
	//check all pixels that are available
	for (int y = min_y; y <= max_y; y++) {
		for (int x = min_x; x <= max_x; x++) {
			float dX = x - start_x;
			float dY = y - start_y;
			float dist_square = dX * dX + dY * dY;
			uint8_t current_value = bmp.buffer[y * width + x] == 0 ? 0 : 1;
			if (current_value != value)
				min_dist_square = std::min(dist_square, min_dist_square);
		}
	}

	float min_dist = std::sqrt(min_dist_square);
	/*float v = (min_dist - 0.5f) / (spread - 0.5f);
	v *= value == 0 ? -1 : 1;
	v = (v + 1) * 0.5f;
	v *= 255.0f;*/

	float sd = value == 0 ? -min_dist : +min_dist;
	float v = 0.5f + sd / spread;
	v = std::clamp(v, 0.0f, 1.0f) * 255.0f;

	return (uint8_t)v;
}

static void get_glyph_SDF_data(
	std::vector<unsigned char>& out_sdf,
	GlyphData* glyphData,
	FT_Face face,
	uint32_t char_code,
	int sdf_size,
	int sdf_padding,
	int sdf_high_res_size,
	float spread
) {
	FT_UInt glyphIndex = FT_Get_Char_Index(face, char_code);
	
	float scale_factor = (float)sdf_high_res_size / sdf_size;
	int high_res_padding = scale_factor * sdf_padding;
	int pixel_size = sdf_high_res_size - high_res_padding * 2;

	FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER);
	FT_Bitmap& bmp = face->glyph->bitmap;

	int high_res_bearingX = face->glyph->metrics.horiBearingX >> 6;
	int high_res_bearingY = face->glyph->metrics.horiBearingY >> 6;
	int advance = face->glyph->advance.x >> 6;

	int sdf_atlas_x_offset = char_code % 16;
	int sdf_atlas_y_offset = char_code / 16 - 2;
	int sdf_atlas_width = sdf_size * 16;

	int atlas_x, atlas_y;
	int height_half_bias = (pixel_size - bmp.rows) / 2;
	int width_half_bias = (pixel_size - bmp.width) / 2;
	
	//setup sdf pixel values
	for (int y = 0; y < sdf_size; y++) {
		for (int x = 0; x < sdf_size; x++) {
			// map this coordinate on a high res bitmap
			// if high res size = 1024, usual size = 64 and padding = 8 --> high res bitmap desired size is 768, padding = 128, min is -128, max is 880)

			int high_res_x = x * scale_factor - high_res_padding - width_half_bias;
			int high_res_y = y * scale_factor - high_res_padding - height_half_bias;

			atlas_x = sdf_atlas_x_offset * sdf_size + x;
			atlas_y = sdf_atlas_y_offset * sdf_size + y;
			uint8_t pixel_value = get_minimal_distance_value(high_res_x, high_res_y, spread, bmp);
			out_sdf[atlas_y * sdf_atlas_width + atlas_x] = pixel_value;
		}
	}

	//get glyph box bounds for UV and glyph_size_ratio based on sdf pixel values
	int glyph_min_x = 0, glyph_max_x = sdf_size - 1;
	int glyph_min_y = 0, glyph_max_y = sdf_size - 1;

	for (int x = 0; x < sdf_size; x++) { //find min X bound of glyph box where pixel value > 0
		for (int y = 0; y < sdf_size; y++) {
			atlas_x = sdf_atlas_x_offset * sdf_size + x;
			atlas_y = sdf_atlas_y_offset * sdf_size + y;
			uint8_t pixel_value = out_sdf[atlas_y * sdf_atlas_width + atlas_x];
			if (pixel_value > 0) {
				glyph_min_x = x;
				x = sdf_size;
				break;
			}
		}
	}
	for (int y = 0; y < sdf_size; y++) { //find min Y bound of glyph box where pixel value > 0
		for (int x = 0; x < sdf_size; x++) {
			atlas_x = sdf_atlas_x_offset * sdf_size + x;
			atlas_y = sdf_atlas_y_offset * sdf_size + y;
			uint8_t pixel_value = out_sdf[atlas_y * sdf_atlas_width + atlas_x];
			if (pixel_value > 0) {
				glyph_min_y = y;
				y = sdf_size;
				break;
			}
		}
	}
	for (int x = sdf_size - 1; x >= 0; x--) { //find max X bound of glyph box where pixel value > 0
		for (int y = sdf_size - 1; y >= 0; y--) {
			atlas_x = sdf_atlas_x_offset * sdf_size + x;
			atlas_y = sdf_atlas_y_offset * sdf_size + y;
			uint8_t pixel_value = out_sdf[atlas_y * sdf_atlas_width + atlas_x];
			if (pixel_value > 0) {
				glyph_max_x = x;
				x = -1;
				break;
			}
		}
	}
	for (int y = sdf_size - 1; y >= 0; y--) { //find max Y bound of glyph box where pixel value > 0
		for (int x = sdf_size - 1; x >= 0; x--) {
			atlas_x = sdf_atlas_x_offset * sdf_size + x;
			atlas_y = sdf_atlas_y_offset * sdf_size + y;
			uint8_t pixel_value = out_sdf[atlas_y * sdf_atlas_width + atlas_x];
			if (pixel_value > 0) {
				glyph_max_y = y;
				y = -1;
				break;
			}
		}
	}

	float glyph_norm_width = float(glyph_max_x - glyph_min_x) / (sdf_size - sdf_padding * 2); //normalized width
	float glyph_norm_height = float(glyph_max_y - glyph_min_y) / (sdf_size - sdf_padding * 2); //normalized height
	
	float glyph_y_bearing = (float)high_res_bearingY / pixel_size; //normalized y bearing
	float glyph_x_bearing = (float)high_res_bearingX / pixel_size; //normalized x bearing
	float glyph_advance = (float)advance / pixel_size; //normalized advance
	float U0, U1, V0, V1; //UVs
	U0 = (sdf_atlas_x_offset * sdf_size + glyph_min_x) / (float)sdf_atlas_width;
	V0 = (sdf_atlas_y_offset * sdf_size + glyph_min_y) / (float)sdf_atlas_width;
	U1 = (sdf_atlas_x_offset * sdf_size + glyph_max_x) / (float)sdf_atlas_width;
	V1 = (sdf_atlas_y_offset * sdf_size + glyph_max_y) / (float)sdf_atlas_width;

	GlyphData& data = glyphData[char_code - 32];
	//use inverted UVs, because atlas data is inverted on Y
	data.U0 = U0;
	data.U1 = U1;
	data.V0 = V1;
	data.V1 = V0;
	data.width_normalized = glyph_norm_width;
	data.height_normalized = glyph_norm_height;
	data.bearingY_normalized = glyph_y_bearing;
	data.bearingX_normalized = glyph_x_bearing;
	data.baselineY_normalized = data.height_normalized * 0.5f - data.bearingY_normalized;
	data.advance_normalized = glyph_advance;
}

bool FontManager::generate_truetype_sdf_font(const char* fontPath, bool write_atlas_to_png) {
	// FreeType setup
	FT_Library ft = nullptr;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "[FontManager] Error: Could not load FreeType library" << std::endl;
		return false;
	}

	FT_Face face = nullptr;
	if (FT_New_Face(ft, fontPath, 0, &face)) {
		std::cout << "[FontManager] Error: Could not load font file: " << fontPath << std::endl;
		return false;
	}

	int glyph_box_size = 64;
	int glyph_atlas_size = glyph_box_size * 16;
	int glyph_padding = 8;

	int sdf_high_res_size = 1024;
	float sdf_high_res_spread = 256;
	int high_res_padding = sdf_high_res_size * 0.125f;
	int high_res_pixel_size = sdf_high_res_size - high_res_padding * 2;
	float normilized_ascender, normilized_descender;

	FT_Set_Pixel_Sizes(face, 0, high_res_pixel_size);

	normilized_ascender = float(face->size->metrics.ascender >> 6) / high_res_pixel_size;
	normilized_descender = float(face->size->metrics.descender >> 6) / high_res_pixel_size;

	//font bitmap creation
	uint32_t first = 33, last = 127;
	std::vector<uint32_t> charset;
	charset.reserve(size_t(last - first));
	for (char32_t cp = first; cp < last; ++cp) {
		charset.push_back(cp);
	}

	//sdf font atlas bitmap
	std::vector<unsigned char> sdf(glyph_atlas_size * glyph_atlas_size, 0);

	//glyphs data
	GlyphData glyphsData[95];

	//setup bitmap and glyphs data
	for (auto& code : charset) {
		std::cout << "Code = " << code << ", Glyph: " << char(code) << std::endl;
		get_glyph_SDF_data(sdf, glyphsData, face, code, glyph_box_size, glyph_padding, sdf_high_res_size, sdf_high_res_spread);
	}

	//save atlas bitmap to file
	std::string file_path;
	
	std::string filename = std::filesystem::path(fontPath).filename().string();
	std::string name_no_ext = filename.substr(0, filename.find_last_of('.')) + "_SDF";
	file_path = SDFFontAtlasesDirectory + name_no_ext + ".atlas";

	std::ofstream atlas_file(file_path, std::ios::binary);

	atlas_file.write(reinterpret_cast<const char*>(&glyph_atlas_size), sizeof(glyph_atlas_size));
	atlas_file.write(reinterpret_cast<const char*>(sdf.data()), sdf.size());
	atlas_file.close();

	//save atlas meta to file
	file_path = SDFFontAtlasesDirectory + name_no_ext + ".meta";
	std::ofstream meta_file(file_path, std::ios::binary);

	uint32_t glyphs_count = 95;
	meta_file.write(reinterpret_cast<const char*>(&glyphs_count), sizeof(glyphs_count));
	meta_file.write(reinterpret_cast<const char*>(&normilized_ascender), sizeof(normilized_ascender));
	meta_file.write(reinterpret_cast<const char*>(&normilized_descender), sizeof(normilized_descender));
	for (auto& glyph : glyphsData) {
		meta_file.write(reinterpret_cast<const char*>(&glyph), sizeof(GlyphData));
	}
	meta_file.close();

	if (write_atlas_to_png) {
		file_path = SDFFontAtlasesDirectory + name_no_ext + ".png";
		stbi_write_png(file_path.c_str(), glyph_atlas_size, glyph_atlas_size, 1, sdf.data(), glyph_atlas_size);
	}

	return true;
}

Font* FontManager::load_sdf_font(const char* fontDataPath, std::string fontName) {
	//Font setup
	uint32_t id;
	if (free_ids.size()) {
		id = free_ids.back();
		free_ids.pop_back();
	}
	else id = fonts.size();
	Font& font = *fonts.add(Font{}, id);
	font.id = id;
	font.name = fontName;

	uint32_t glyph_atlas_size = 0;

	//load atlas bitmap
	std::string file_path;
	std::string filename = std::filesystem::path(fontDataPath).filename().string();

	file_path = SDFFontAtlasesDirectory + filename + ".atlas";
	std::ifstream atlas_file(file_path, std::ios::binary);

	atlas_file.read(reinterpret_cast<char*>(&glyph_atlas_size), sizeof(glyph_atlas_size));

	std::vector<unsigned char> sdf(glyph_atlas_size * glyph_atlas_size, 0);
	atlas_file.read(reinterpret_cast<char*>(sdf.data()), sdf.size());
	atlas_file.close();

	//load atlas meta
	file_path = SDFFontAtlasesDirectory + filename + ".meta";
	std::ifstream meta_file(file_path, std::ios::binary);

	uint32_t glyphs_count = 0;
	meta_file.read(reinterpret_cast<char*>(&glyphs_count), sizeof(glyphs_count));
	meta_file.read(reinterpret_cast<char*>(&font.ascender), sizeof(font.ascender));
	meta_file.read(reinterpret_cast<char*>(&font.descender), sizeof(font.descender));

	for (auto& glyph : font.glyphsData) {
		meta_file.read(reinterpret_cast<char*>(&glyph), sizeof(GlyphData));
	}
	meta_file.close();

	uint32_t tex_id = TextureManager::get_Instance()->create_texture_from_data(sdf.data(), glyph_atlas_size, glyph_atlas_size, 1, false)->texture_id;
	font.texture_id = tex_id;
	std::cout << "Glyph texture id = " << tex_id << std::endl;
	std::cout << "Font ascender = " << font.ascender << std::endl;
	std::cout << "Font descender = " << font.descender << std::endl;

	//return new font ptr
	return fonts.get(id);
}

Font* FontManager::get_font(uint32_t id) {
    return fonts.get(id);
}

bool FontManager::remove_font(uint32_t id) {
	return fonts.remove(id);
	free_ids.emplace_back(id);
}