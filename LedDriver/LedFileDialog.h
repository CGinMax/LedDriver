#pragma once
#include <string>
class LedFileDialog
{
public:
	LedFileDialog();
	virtual ~LedFileDialog();
	virtual std::string OpenNewFileDialog();
	virtual std::string SelectFileNameDialog();
	virtual std::string OpenSaveFileDialog();
};

class ImageFileDialog : public LedFileDialog
{
public:
	ImageFileDialog() {}
	~ImageFileDialog() {}
	virtual std::string SelectFileNameDialog();

};

class VideoFileDialog : public LedFileDialog
{
public:
	VideoFileDialog() {}
	~VideoFileDialog() {}
	virtual std::string SelectFileNameDialog();

};

class ProjectFileDialog : public LedFileDialog
{
public:
	ProjectFileDialog() {}
	~ProjectFileDialog() {}
	virtual std::string OpenNewFileDialog();
	virtual std::string SelectFileNameDialog();
	virtual std::string OpenSaveFileDialog();
};

