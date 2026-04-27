#pragma once
#include <Rendering/Interfaces/Renderer.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::Graphics {
	class OpenGL_Renderer : public RendererInterface {
	public:
		OpenGL_Renderer(GLFWwindow* window) : window{ window } { api = RENDER_API::OpenGL_API; }
		void init() override;
		void clear(float r, float g, float b) override;
		void present() override;
		void renderData(
			std::unique_ptr<Shader>& shader,
			std::unique_ptr<VertexArray>& va,
			std::unique_ptr<VertexBuffer>& vb
		) override;
		void renderIndexedData(
			std::unique_ptr<Shader>& shader,
			std::unique_ptr<VertexArray>& va,
			std::unique_ptr<VertexBuffer>& vb,
			std::unique_ptr<IndexBuffer>& ib,
			uint32_t indexCount
		) override;
		void renderInstancedData(
			std::unique_ptr<Shader>& shader,
			std::unique_ptr<VertexArray>& va,
			std::unique_ptr<VertexBuffer>& vb,
			std::unique_ptr<IndexBuffer>& ib,
			uint32_t indexCount,
			uint32_t instanceCount
		) override;
		void renderMultiIndirectData(
			std::unique_ptr<Shader>& shader,
			std::unique_ptr<VertexArray>& va,
			std::unique_ptr<VertexBuffer>& vb,
			std::unique_ptr<IndexBuffer>& indexBuf,
			std::unique_ptr<IndirectBuffer>& indirectBuf,
			uint32_t commandCount,
			uint32_t sizeOfCommand
		) override;

		std::unique_ptr<VertexArray> createVertexArray() override;
		std::unique_ptr<VertexBuffer> createVertexBuffer() override;
		std::unique_ptr<IndexBuffer> createIndexBuffer() override;
		std::unique_ptr<UniformBuffer> createUniformBuffer() override;
		std::unique_ptr<StorageBuffer> createStorageBuffer() override;
		std::unique_ptr<IndirectBuffer> createIndirectBuffer() override;
		std::unique_ptr<Shader> createShader(ShaderDesc desc) override;

		std::unique_ptr<Texture> createTextureFromFile(const char* filePath, bool isPixelised) override;
		std::unique_ptr<Texture> createTextureFromData(const unsigned char* image_bytes, int imgWidth, int imgHeight, int numOfColorChannels, bool isPixelised) override;
		std::unique_ptr<Texture> createR8TextureFromData(const unsigned char* image_bytes, int imgWidth, int imgHeight, bool isPixelised) override;
	private:
		GLFWwindow* window;
	};
}
