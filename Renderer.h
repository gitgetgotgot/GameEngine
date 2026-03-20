#pragma once
#include <cstdint>
#include <memory>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "StorageBuffer.h"
#include "IndirectBuffer.h"
#include "Shader.h"
#include "Texture.h"

enum RENDER_API : uint8_t { OpenGL_API, DirectX_API, Vulkan_API };

struct DrawElementsIndirectCommand {
	DrawElementsIndirectCommand() {}
	DrawElementsIndirectCommand(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t baseVertex, uint32_t baseInstance) :
		indexCount{ indexCount }, instanceCount{ instanceCount }, firstIndex{ firstIndex }, baseVertex{ baseVertex }, baseInstance{ baseInstance } {}
	uint32_t indexCount{};
	uint32_t instanceCount{};
	uint32_t firstIndex{};    //first index in index buffer
	uint32_t baseVertex{};    //first vertex in vertex buffer
	uint32_t baseInstance{};  //first instance in instances
};

namespace Engine {
	namespace Graphics {
		class RendererInterface {
		public:
			virtual ~RendererInterface() {}
			virtual void init() = 0;
			virtual void clear(float r, float g, float b) = 0;
			virtual void present() = 0;
			virtual void renderData(
				std::unique_ptr<Shader>& shader,
				std::unique_ptr<VertexArray>& va,
				std::unique_ptr<VertexBuffer>& vb
			) = 0;
			virtual void renderIndexedData(
				std::unique_ptr<Shader>& shader,
				std::unique_ptr<VertexArray>& va,
				std::unique_ptr<VertexBuffer>& vb,
				std::unique_ptr<IndexBuffer>& ib,
				uint32_t indexCount
			) = 0;
			virtual void renderInstancedData(
				std::unique_ptr<Shader>& shader,
				std::unique_ptr<VertexArray>& va,
				std::unique_ptr<VertexBuffer>& vb,
				std::unique_ptr<IndexBuffer>& ib,
				uint32_t indexCount,
				uint32_t instanceCount
			) = 0;
			virtual void renderMultiIndirectData(
				std::unique_ptr<Shader>& shader,
				std::unique_ptr<VertexArray>& va,
				std::unique_ptr<VertexBuffer>& vb,
				std::unique_ptr<IndexBuffer>& indexBuf,
				std::unique_ptr<IndirectBuffer>& indirectBuf,
				uint32_t commandCount,
				uint32_t sizeOfCommand
			) = 0;

			virtual std::unique_ptr<VertexArray> createVertexArray() = 0;
			virtual std::unique_ptr<VertexBuffer> createVertexBuffer() = 0;
			virtual std::unique_ptr<IndexBuffer> createIndexBuffer() = 0;
			virtual std::unique_ptr<UniformBuffer> createUniformBuffer() = 0;
			virtual std::unique_ptr<StorageBuffer> createStorageBuffer() = 0;
			virtual std::unique_ptr<IndirectBuffer> createIndirectBuffer() = 0;
			virtual std::unique_ptr<Shader> createShader(ShaderDesc desc) = 0;

			virtual std::unique_ptr<Texture> createTextureFromFile(const char* filePath, bool isPixelised) = 0;
			virtual std::unique_ptr<Texture> createTextureFromData(unsigned char* image_bytes, int imgWidth, int imgHeight, int numOfColorChannels, bool isPixelised) = 0;
			virtual std::unique_ptr<Texture> createR8TextureFromData(unsigned char* image_bytes, int imgWidth, int imgHeight, bool isPixelised) = 0;
		public:
			uint32_t DYNAMIC_USAGE{}, STATIC_USAGE{};
			RENDER_API api{};
		};
	}
}

