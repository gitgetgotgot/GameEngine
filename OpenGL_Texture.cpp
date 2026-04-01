#include "OpenGL_Texture.h"
#include "stb/stb_image.h"

OpenGL_Texture::OpenGL_Texture(const char* filePath, bool isPixelised) {
	int imgWidth, imgHeight, numOfColorChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image_bytes = stbi_load(filePath, &imgWidth, &imgHeight, &numOfColorChannels, 0);

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isPixelised ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, isPixelised ? GL_NEAREST : GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum internalFormat;
	GLenum format;
	if (numOfColorChannels == 1) {
		internalFormat = GL_R8;
		format = GL_RED;
	}
	else if (numOfColorChannels == 2) {
		internalFormat = GL_RG8;
		format = GL_RG;
	}
	else if (numOfColorChannels == 3) {
		internalFormat = GL_RGB8;
		format = GL_RGB;
	}
	else {
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, image_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image_bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGL_Texture::OpenGL_Texture(const unsigned char* image_bytes, int imgWidth, int imgHeight, int numOfColorChannels, bool isPixelised) {
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isPixelised ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, isPixelised ? GL_NEAREST : GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum internalFormat;
	GLenum format;
	if (numOfColorChannels == 1) {
		internalFormat = GL_R8;
		format = GL_RED;
	}
	else if (numOfColorChannels == 2) {
		internalFormat = GL_RG8;
		format = GL_RG;
	}
	else if (numOfColorChannels == 3) {
		internalFormat = GL_RGB8;
		format = GL_RGB;
	}
	else {
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, image_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGL_Texture::OpenGL_Texture(const unsigned char* image_bytes, int imgWidth, int imgHeight, bool isPixelised) {
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isPixelised ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, isPixelised ? GL_NEAREST : GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, imgWidth, imgHeight, 0, GL_RED, GL_UNSIGNED_BYTE, image_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGL_Texture::~OpenGL_Texture() {
	glDeleteTextures(1, &id);
}

void OpenGL_Texture::bind(uint32_t slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void OpenGL_Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint64_t OpenGL_Texture::get_handle() {
	GLuint64 handle = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(handle);
	return handle;
}
