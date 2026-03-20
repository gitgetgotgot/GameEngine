#pragma once
#include <string>
#include <fstream>

struct ShaderDesc {
	ShaderDesc() {}
	ShaderDesc(std::string filePath, std::string vertexEntry, std::string fragmentEntry)
		: filePath{ filePath }, vertexEntry{ vertexEntry }, fragmentEntry{ fragmentEntry } {}
	std::string filePath;
	std::string vertexEntry;
	std::string fragmentEntry;
};

std::string get_file_contents(const char*);

class Shader {
public:
	virtual void bind() = 0;
	~Shader() {}
};