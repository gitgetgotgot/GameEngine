#include <CustomScripts/PlayerController.h>
#include <SystemModule/SystemContext.h>
#include <SystemModule/TimeManager.h>

void PlayerController::OnConstruct() {
	//transform = object->add_component<Engine::Component::Transform>();
	//transform->translateLocal(0, 0, 2.f);
	//sc = object->add_component<Engine::Component::SpriteComponent>();
	
	//mc = object->add_component<Engine::Component::MeshComponent>();
	//Mesh& mesh = MeshManager::get_Instance()->get_mesh(4);
	//mc->mesh_id = mesh.id;
	std::cout << "Player controller constructed" << std::endl;
}

void PlayerController::Update() {
	if (SystemContext::keyBoard.key_is_pressed(KeyP)) {
		canControl = !canControl;
	}

	if (!canControl) return;

	float speed_m = TimeManager::deltaTime;
	if (SystemContext::keyBoard.key_is_held(KeyLeftShift))
		speed_m *= 5.0f;
	//translate
	if (SystemContext::keyBoard.key_is_held(KeyW)) {
		transform->translateLocal(0.f, 0.f, -speed_m);
	}
	if (SystemContext::keyBoard.key_is_held(KeyA)) {
		transform->translateLocal(-speed_m, 0.f, 0.f);
	}
	if (SystemContext::keyBoard.key_is_held(KeyS)) {
		transform->translateLocal(0.f, 0.f, speed_m);
	}
	if (SystemContext::keyBoard.key_is_held(KeyD)) {
		transform->translateLocal(speed_m, 0.f, 0.f);
	}
	if (SystemContext::keyBoard.key_is_held(KeySpace)) {
		transform->translateLocal(0.f, speed_m, 0.f);
	}
	if (SystemContext::keyBoard.key_is_held(KeyLeftCtrl)) {
		transform->translateLocal(0.f, -speed_m, 0.f);
	}
	//rotate with arrows
	if (SystemContext::keyBoard.key_is_held(KeyLeft)) {
		transform->rotate(false, true, false, -speed_m * cameraRotateSpeed);
	}
	else if (SystemContext::keyBoard.key_is_held(KeyRight)) {
		transform->rotate(false, true, false, speed_m * cameraRotateSpeed);
	}
	else if (SystemContext::keyBoard.key_is_held(KeyUp)) {
		transform->rotate(true, false, false, -speed_m * cameraRotateSpeed);
	}
	else if (SystemContext::keyBoard.key_is_held(KeyDown)) {
		transform->rotate(true, false, false, speed_m * cameraRotateSpeed);
	}
	//text control
	if (SystemContext::keyBoard.key_is_pressed(Key1)) {
		text->set_vert_align(TextVerticalAlignment::vertUp);
		text->set_font(0);
	}
	if (SystemContext::keyBoard.key_is_pressed(Key2)) {
		text->set_vert_align(TextVerticalAlignment::vertCenter);
		text->set_font(1);
	}
	if (SystemContext::keyBoard.key_is_pressed(Key3)) {
		text->set_vert_align(TextVerticalAlignment::vertBottom);
		text->set_font(2);
	}
	if (SystemContext::keyBoard.key_is_pressed(Key4)) {
		text->set_horiz_align(TextHorizontalAlignment::horizLeft);
	}
	if (SystemContext::keyBoard.key_is_pressed(Key5)) {
		text->set_horiz_align(TextHorizontalAlignment::horizCenter);
	}
	if (SystemContext::keyBoard.key_is_pressed(Key6)) {
		text->set_horiz_align(TextHorizontalAlignment::horizRight);
	}
	if (SystemContext::keyBoard.key_is_pressed(Key0)) {
		text_transform->set_rotation(0.0f, 0.0f, 0.0f);
	}
	if (SystemContext::mouse.wheel_offset != 0) {
		text_transform->rotate(false, false, true, SystemContext::mouse.wheel_offset * TimeManager::deltaTime * 50.f);
	}

	if (SystemContext::mouse.lb_is_pressed()) {
		image->flipX(!image->flip_X);
	}
	else if (SystemContext::mouse.rb_is_pressed()) {
		image->flipY(!image->flip_Y);
	}

	if (SystemContext::keyBoard.key_is_pressed(KeyDelete)) {
		object->Destroy();
		model->Destroy();
	}
}

void PlayerController::set_test_UI(
	EUI::UI_Component_Ptr<EUI::Image>& img_ptr,
	EUI::UI_Component_Ptr<EUI::Button>& button_ptr,
	EUI::UI_Component_Ptr<EUI::SDF_Text>& text_ptr,
	EUI::UI_Component_Ptr<EUI::UI_Transform>& text_transform
) {
	image = img_ptr;
	button = button_ptr;
	text = text_ptr;
	this->text_transform = text_transform;
	button->set_callback(&PlayerController::wrapper, this);
}

void PlayerController::set_test_transform(EC::Component_Ptr<EC::Transform>& tr_ptr) {
	this->transform = tr_ptr;
}

void PlayerController::change_image_color() {
	if (color_changed) {
		image->set_color(glm::vec4(0.0, 1.0, 1.0, 1.0));
		image->isActive = false;
	}
	else {
		image->set_color(glm::vec4(1.0, 0.0, 0.0, 1.0));
		image->isActive = true;

	}
	color_changed = !color_changed;
}

void PlayerController::wrapper(void* obj) {
	static_cast<PlayerController*>(obj)->change_image_color();
}
