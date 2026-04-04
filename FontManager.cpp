#include "FontManager.h"
#include "stb/stb_image.h"
#include "TextureManager.h"

Font* FontManager::add_font(const char* fontPath, bool isSDF, std::string fontName) {
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
	if (isSDF) {
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
				img_data[(j * imgHeight + i)] = image_bytes[(j * imgHeight + i) * numOfChannels + checkByte] > 0 ? 0 : 255;
			}
		}
	}
	font.texture_id = TextureManager::get_Instance()->create_texture_from_data(img_data, imgWidth, imgHeight, 1, false)->texture_id;
	delete[] img_data;
	stbi_image_free(image_bytes);
	return fonts.get(id);
}

Font* FontManager::get_font(uint32_t id) {
    return fonts.get(id);
}

bool FontManager::remove_font(uint32_t id) {
	return fonts.remove(id);
	free_ids.emplace_back(id);
}
