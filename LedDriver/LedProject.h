#pragma once
#include "CustomTypes.h"
#include <string>
class LedProject
{
public:
	LedProject();
	~LedProject();
public:
	bool NewProject(std::shared_ptr<CommonData> spData);
	bool OpenProject(std::shared_ptr<CommonData> spData);
	void SaveProject();
public:
	bool isStartProject;
private:
	std::string projectFile;
	std::shared_ptr<CommonData> projectData;
};

