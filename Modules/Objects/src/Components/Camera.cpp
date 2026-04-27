#include <Objects/Components/Camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <SystemModule/SystemContext.h>

Engine::Component::Camera::Camera() {
	viewMatrix = projectionMatrix = glm::mat4(1.0f);
}

void Engine::Component::Camera::set_perspective(float near, float far) {
	projectionMatrix = glm::perspectiveRH(FOV, SystemContext::screen.ratio, near, far);
}

void Engine::Component::Camera::set_orthographic(float near, float far) {
	projectionMatrix = glm::orthoRH(-SystemContext::screen.ratio, SystemContext::screen.ratio, -1.0f, 1.0f, near, far);
}

glm::mat4 Engine::Component::Camera::get_view_matrix() {
	return viewMatrix;
}

glm::mat4 Engine::Component::Camera::get_projection_matrix() {
	return projectionMatrix;
}
