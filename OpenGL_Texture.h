#pragma once
#include "glad/glad.h"
#include "Texture.h"

class OpenGL_Texture : public Texture {
public:
	//create texture from file
	OpenGL_Texture(const char* filePath, bool isPixelised);
	//create texture from data
	OpenGL_Texture(unsigned char* image_bytes, int imgWidth, int imgHeight, int numOfColorChannels, bool isPixelised);
	//create R8 texture from data
	OpenGL_Texture(unsigned char* image_bytes, int imgWidth, int imgHeight, bool isPixelised);
	
	~OpenGL_Texture();
	void bind(uint32_t slot = 0) override;
	void unbind() override;
private:
	GLuint id;
};