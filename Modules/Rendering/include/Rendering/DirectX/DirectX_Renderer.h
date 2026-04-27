#pragma once

#ifdef _WIN32
#include <Rendering/Interfaces/Renderer.h>
#include <d3d11.h>

namespace Engine::Graphics {
	class DirectX_Renderer : public RendererInterface {
	public:
		
	private:
		HWND hwnd;
	};
}
#endif