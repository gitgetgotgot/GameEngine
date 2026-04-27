#pragma once
#include "Project.h"

class ProjectManager {
public:
	static ProjectManager* get_Instance() {
		static ProjectManager projMgr;
		return &projMgr;
	}
	bool LoadProject(std::string& project_path);
	void CreateEmptyProject(std::string& project_path);
	void SaveProject();
	bool AddNewScene(std::string& scene_name);
private:
	ProjectManager() {}
	~ProjectManager() {}
	Project current_Project;
};