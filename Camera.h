#pragma once
#include "glm/glm.hpp"

class CameraManager;

namespace Engine {
	namespace Component {
		struct Camera {
			friend class CameraManager;
		public:
			Camera();
			float FOV = 45.f;
			void set_perspective(float near, float far);
			void set_orthographic(float near, float far);
			glm::mat4 get_view_matrix();
			glm::mat4 get_projection_matrix();
		private:
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
		};
	}
}
