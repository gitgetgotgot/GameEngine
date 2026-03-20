#pragma once
#include "Renderer.h"

class Vulkan_Renderer : public RendererInterface {
public:
	virtual void init() override;
	virtual void clear(float r, float g, float b) override;
	virtual void present() override;
	virtual void renderData() override;
	virtual void renderIndexedData(
		std::unique_ptr<Shader>& shader,
		std::unique_ptr<VertexArray>& va,
		std::unique_ptr<VertexBuffer>& vb,
		std::unique_ptr<IndexBuffer>& ib,
		uint32_t indexCount) override;
	virtual void renderInstancedData() override;
	virtual std::unique_ptr<VertexArray> createVertexArray() override;
	virtual std::unique_ptr<VertexBuffer> createVertexBuffer() override;
	virtual std::unique_ptr<IndexBuffer> createIndexBuffer() override;
	virtual std::unique_ptr<UniformBuffer> createUniformBuffer() override;
	virtual std::unique_ptr<Shader> createShader(ShaderDesc desc) override;
private:

};