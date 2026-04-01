#include "OpenGL_Renderer.h"
#include "OpenGL_Shader.h"
#include "OpenGL_VertexArray.h"
#include "OpenGL_VertexBuffer.h"
#include "OpenGL_IndexBuffer.h"
#include "OpenGL_UniformBuffer.h"
#include "OpenGL_StorageBuffer.h"
#include "OpenGL_IndirectBuffer.h"
#include "OpenGL_Texture.h"

void Engine::Graphics::OpenGL_Renderer::init() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	this->DYNAMIC_USAGE = GL_DYNAMIC_DRAW;
	this->STATIC_USAGE = GL_STATIC_DRAW;

	/*GLint maxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
	std::cout << "Max texture slots: " << maxTextureUnits << std::endl;*/
}

void Engine::Graphics::OpenGL_Renderer::clear(float r, float g, float b) {
	glClearColor(r, g, b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::Graphics::OpenGL_Renderer::present() {
	glfwSwapBuffers(window);
}

void Engine::Graphics::OpenGL_Renderer::renderData(
	std::unique_ptr<Shader>& shader,
	std::unique_ptr<VertexArray>& va,
	std::unique_ptr<VertexBuffer>& vb
) {
	//probably not needed
}

void Engine::Graphics::OpenGL_Renderer::renderIndexedData(
	std::unique_ptr<Shader>& shader,
	std::unique_ptr<VertexArray>& va,
	std::unique_ptr<VertexBuffer>& vb,
	std::unique_ptr<IndexBuffer>& ib,
	uint32_t indexCount
) {
	shader->bind();
	va->bind();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void Engine::Graphics::OpenGL_Renderer::renderInstancedData(
	std::unique_ptr<Shader>& shader,
	std::unique_ptr<VertexArray>& va,
	std::unique_ptr<VertexBuffer>& vb,
	std::unique_ptr<IndexBuffer>& ib,
	uint32_t indexCount,
	uint32_t instanceCount
) {
	shader->bind();
	va->bind();
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
}

void Engine::Graphics::OpenGL_Renderer::renderMultiIndirectData(
	std::unique_ptr<Shader>& shader,
	std::unique_ptr<VertexArray>& va,
	std::unique_ptr<VertexBuffer>& vb,
	std::unique_ptr<IndexBuffer>& indexBuf,
	std::unique_ptr<IndirectBuffer>& indirectBuf,
	uint32_t commandCount,
	uint32_t sizeOfCommand
) {
	shader->bind();
	va->bind();
	glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, commandCount, sizeOfCommand);
}

std::unique_ptr<VertexArray> Engine::Graphics::OpenGL_Renderer::createVertexArray() {
	return std::make_unique<OpenGL_VertexArray>();
}

std::unique_ptr<VertexBuffer> Engine::Graphics::OpenGL_Renderer::createVertexBuffer() {
	return std::make_unique<OpenGL_VertexBuffer>();
}

std::unique_ptr<IndexBuffer> Engine::Graphics::OpenGL_Renderer::createIndexBuffer() {
	return std::make_unique<OpenGL_IndexBuffer>();
}

std::unique_ptr<UniformBuffer> Engine::Graphics::OpenGL_Renderer::createUniformBuffer() {
	return std::make_unique<OpenGL_UniformBuffer>();
}

std::unique_ptr<StorageBuffer> Engine::Graphics::OpenGL_Renderer::createStorageBuffer() {
	return std::make_unique<OpenGL_StorageBuffer>();
}

std::unique_ptr<IndirectBuffer> Engine::Graphics::OpenGL_Renderer::createIndirectBuffer() {
	return std::make_unique<OpenGL_IndirectBuffer>();
}

std::unique_ptr<Shader> Engine::Graphics::OpenGL_Renderer::createShader(ShaderDesc desc) {
	return std::make_unique<OpenGL_Shader>(desc);
}

std::unique_ptr<Texture> Engine::Graphics::OpenGL_Renderer::createTextureFromFile(const char* filePath, bool isPixelised) {
	return std::make_unique<OpenGL_Texture>(filePath, isPixelised);
}

std::unique_ptr<Texture> Engine::Graphics::OpenGL_Renderer::createTextureFromData(const unsigned char* image_bytes, int imgWidth, int imgHeight, int numOfColorChannels, bool isPixelised) {
	return std::make_unique<OpenGL_Texture>(image_bytes, imgWidth, imgHeight, numOfColorChannels, isPixelised);
}

std::unique_ptr<Texture> Engine::Graphics::OpenGL_Renderer::createR8TextureFromData(const unsigned char* image_bytes, int imgWidth, int imgHeight, bool isPixelised)
{
	return std::make_unique<OpenGL_Texture>(image_bytes, imgWidth, imgHeight, isPixelised);
}
