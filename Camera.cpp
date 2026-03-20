#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "SystemContext.h"

Engine::Component::Camera::Camera() {
	viewMatrix = glm::mat4(1.0f);
}

void Engine::Component::Camera::set_perspective(float near, float far) {
	projectionMatrix = glm::perspectiveLH(FOV, SystemContext::screen.ratio, near, far);
}

void Engine::Component::Camera::set_orthographic(float near, float far) {
	projectionMatrix = glm::orthoLH(-SystemContext::screen.ratio, SystemContext::screen.ratio, -1.0f, 1.0f, near, far);
}

glm::mat4 Engine::Component::Camera::get_view_matrix() {
	return viewMatrix;
}

glm::mat4 Engine::Component::Camera::get_projection_matrix() {
	return projectionMatrix;
}
