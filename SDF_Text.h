#pragma once
#include "UI_Component.h"
#include "FontManager.h"

struct SDF_TextUpdater {
	inline static std::vector<uint32_t> dirtyTexts;
private:
	SDF_TextUpdater() {}
	~SDF_TextUpdater() {}
};

namespace Engine::Systems {
	class SDF_TextSystem;
}

enum TextHorizontalAlignment : uint8_t {
	horizLeft, horizCenter, horizRight
};
enum TextVerticalAlignment : uint8_t {
	vertUp, vertCenter, vertBottom
};

namespace Engine::UI {
	class SDF_Text : public UI_Component {
		friend class Engine::Systems::SDF_TextSystem;
	public:
		std::string text;
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float font_size = 0.25f;
		TextHorizontalAlignment horizAlign = TextHorizontalAlignment::horizLeft;
		TextVerticalAlignment vertAlign = TextVerticalAlignment::vertCenter;
		void set_horiz_align(TextHorizontalAlignment type);
		void set_vert_align(TextVerticalAlignment type);
		void set_height(float height);
		void set_color(glm::vec4 color);
		void set_text(std::string& text);
		void set_text(const char* text);
		void set_font(uint32_t id);
		void _internal_update_ui_data() override;
	private:
		uint32_t font_id = 0;
		void mark_dirty() override;
		std::vector<Engine::Systems::UI_InstanceData> data;
	};
}