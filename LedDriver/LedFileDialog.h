#pragma once
#include <string>
class LedFileDialog
{
public:
	LedFileDialog();
	~LedFileDialog();
	static std::string SelectFileNameDialog();
	static std::string OpenSaveFileDialog();
};

