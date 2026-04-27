#pragma once
#include "SceneManager.h"

class ProjectSettings {
public:
	//lalala
};

class Project {
public:
	std::string project_name;
	std::filesystem::path project_file_path;
	std::filesystem::path project_main_path;

	std::vector<SceneInfo> project_scenes;
	uint32_t active_scene = 0;

	ProjectSettings project_settings;
};