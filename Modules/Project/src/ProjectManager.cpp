#include <Project/ProjectManager.h>
#include <iostream>

bool ProjectManager::LoadProject(std::string& project_path) {
	if (!std::filesystem::exists(project_path))
		return false; //no sush folder

	std::filesystem::path mainPath = project_path;
	std::filesystem::path name = mainPath.filename();
	std::filesystem::path projFilePath = (mainPath / (name.string() + ".proj")).generic_string();
	if (!std::filesystem::exists(projFilePath) || !std::filesystem::is_regular_file(projFilePath))
		return false; //no such .proj file

	current_Project.project_main_path = project_path;
	current_Project.project_file_path = projFilePath;

	std::ifstream projFile(projFilePath, std::ios::binary);

	uint32_t proj_name_size;
	uint32_t scenes_count, scene_name_size;
	std::string scene_name;

	projFile.read(reinterpret_cast<char*>(&proj_name_size), sizeof(proj_name_size));
	current_Project.project_name.resize(proj_name_size);
	projFile.read(reinterpret_cast<char*>(current_Project.project_name.data()), proj_name_size * sizeof(char));

	projFile.read(reinterpret_cast<char*>(&scenes_count), sizeof(scenes_count));
	current_Project.project_scenes.resize(scenes_count);

	for (uint32_t i = 0; i < scenes_count; i++) {
		SceneInfo& sceneInfo = current_Project.project_scenes[i];

		projFile.read(reinterpret_cast<char*>(&scene_name_size), sizeof(scene_name_size));
		scene_name.resize(scene_name_size);
		projFile.read(reinterpret_cast<char*>(scene_name.data()), scene_name_size * sizeof(char));

		sceneInfo.scene_path = (mainPath / "Scenes" / (scene_name + ".scene")).generic_string();
	}
	projFile.close();

	/*std::cout << "\n[ProjectManager]\nProject name = " << current_Project.project_name << std::endl;
	std::cout << "Project main path:\n\t" << current_Project.project_main_path << std::endl;
	std::cout << "Project file path:\n\t" << current_Project.project_file_path << std::endl;
	std::cout << "Scenes count = " << scenes_count << std::endl;
	std::cout << "Scenes:" << std::endl;
	for (uint32_t i = 0; i < scenes_count; i++) {
		SceneInfo& sceneInfo = current_Project.project_scenes[i];

		std::ifstream sceneFile(sceneInfo.scene_path, std::ios::binary);
		uint32_t objects_count;
		sceneFile.read(reinterpret_cast<char*>(&objects_count), sizeof(objects_count));
		std::cout << "--Scene: " << sceneInfo.scene_path.filename() << std::endl;
		std::cout << "--Objects count = " << objects_count << std::endl;
		sceneFile.close();
	}*/

	std::cout << "\n[ProjectManager]: Project loaded: " << current_Project.project_name << "\n\n";
	return true;
}

void ProjectManager::CreateEmptyProject(std::string& project_path) {
	//create project main folder
	std::filesystem::path projectPath = project_path;
	std::filesystem::create_directory(projectPath);
	//create Assets folder
	std::filesystem::create_directory(projectPath / "Assets");
	//create Scenes folder
	std::filesystem::create_directory(projectPath / "Scenes");
	
	//create project binary data file
	std::filesystem::path projectName = projectPath.filename();
	std::filesystem::path file_path = projectPath / (projectName.string() + ".proj");

	current_Project.project_main_path = projectPath;
	current_Project.project_file_path = file_path;
	current_Project.project_name = projectName.string();

	std::ofstream projectFile(file_path, std::ios::binary);
	std::string name = projectName.string();
	uint32_t size = name.size();
	projectFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
	projectFile.write(name.data(), size * sizeof(char));

	//main scene info
	uint32_t scenes_count = 1;
	std::string scene_name = "Main";
	projectFile.write(reinterpret_cast<const char*>(&scenes_count), sizeof(scenes_count));
	size = scene_name.size();
	projectFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
	projectFile.write(scene_name.data(), size * sizeof(char));

	projectFile.close();

	//add main scene file
	file_path = projectPath / "Scenes" / (scene_name + ".scene");
	SceneManager::get_Instance()->CreateScene(file_path);

	current_Project.project_scenes.emplace_back().scene_path = file_path;

	std::cout << "\n[ProjectManager]: New Project created: " << current_Project.project_name << "\n\n";
}

void ProjectManager::SaveProject() {
	std::ofstream projectFile(current_Project.project_file_path, std::ios::binary);
	uint32_t size = current_Project.project_name.size();
	projectFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
	projectFile.write(current_Project.project_name.data(), size * sizeof(char));

	//scenes info
	uint32_t scenes_count = current_Project.project_scenes.size();
	projectFile.write(reinterpret_cast<const char*>(&scenes_count), sizeof(scenes_count));

	std::string scene_name;
	for (uint32_t i = 0; i < scenes_count; i++) {
		SceneInfo& sceneInfo = current_Project.project_scenes[i];
		scene_name = sceneInfo.scene_path.stem().string();
		size = scene_name.size();
		projectFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		projectFile.write(scene_name.data(), size * sizeof(char));
	}

	projectFile.close();
}

bool ProjectManager::AddNewScene(std::string& scene_name) {
	std::filesystem::path file_path = current_Project.project_main_path / "Scenes" / (scene_name + ".scene");

	if (std::filesystem::exists(file_path) && std::filesystem::is_regular_file(file_path))
		return false;

	SceneManager::get_Instance()->CreateScene(file_path);

	current_Project.project_scenes.emplace_back().scene_path = file_path;

	std::cout << "\n[ProjectManager]: New Scene added: " << scene_name << "\n\n";

	return true;
}
