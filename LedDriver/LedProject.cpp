#include "LedProject.h"
#include "LedFileDialog.h"
#include <fstream>
#include <iostream>

LedProject::LedProject() :
	isStartProject(false)
{
}

LedProject::~LedProject()
{
}

bool LedProject::NewProject(std::shared_ptr<CommonData> spData)
{
	isStartProject = false;
	LedFileDialog *projectDialog = new ProjectFileDialog;
	std::string tmpFile = projectDialog->OpenNewFileDialog();
	if (!tmpFile.empty()) {
		projectFile = tmpFile;
		projectData.use_count() == 0 ? projectData = spData : NULL;
		projectData->Init();
		SaveProject();
		isStartProject = true;
	}
	
	delete projectDialog;
	return isStartProject;
}

bool LedProject::OpenProject(std::shared_ptr<CommonData> spData)
{
	isStartProject = false;
	LedFileDialog *projectDialog = new ProjectFileDialog;
	std::string tmpFile = projectDialog->SelectFileNameDialog();
	if (!tmpFile.empty()) {
		projectFile = tmpFile;
		projectData.use_count() == 0 ? projectData = spData : NULL;
		/*int width;
		int height;
		float row_dict;
		float col_dict;
		float cicle_size;
		int select_num;*/
		projectData->lineList.clear();
		LedInt2 pointCoordinate;
		FILE *iread;
		iread = fopen(projectFile.c_str(), "rb");
		fread(&(projectData->whMatrix[0]), sizeof(int), 1, iread);
		fread(&(projectData->whMatrix[1]), sizeof(int), 1, iread);
		fread(&(projectData->rowDict), sizeof(float), 1, iread);
		fread(&(projectData->colDict), sizeof(float), 1, iread);
		fread(&(projectData->cicleSize), sizeof(float), 1, iread);
		fread(&(projectData->comboSelect), sizeof(int), 1, iread);

		while (!feof(iread)) {
			if (fread(&pointCoordinate, sizeof(LedInt2), 1, iread) < 1) break;
			projectData->lineList.push_back(pointCoordinate);
		}
		fclose(iread);

		isStartProject = true;
	}
	delete projectDialog;
	return isStartProject;
}

void LedProject::SaveProject()
{
	LedInt2 pointCoordinate;

	FILE *iwrite;
	iwrite = fopen(projectFile.c_str(), "wb");
	fwrite(&(projectData->whMatrix[0]), sizeof(int), 1, iwrite);
	fwrite(&(projectData->whMatrix[1]), sizeof(int), 1, iwrite);
	fwrite(&(projectData->rowDict), sizeof(float), 1, iwrite);
	fwrite(&(projectData->colDict), sizeof(float), 1, iwrite);
	fwrite(&(projectData->cicleSize), sizeof(float), 1, iwrite);
	fwrite(&(projectData->comboSelect), sizeof(int), 1, iwrite);

	auto pointIter = projectData->lineList.begin();
	while (pointIter != projectData->lineList.end()) {
		pointCoordinate.x = (*pointIter).x;
		pointCoordinate.y = (*pointIter).y;
		fwrite(&pointCoordinate, sizeof(LedInt2), 1, iwrite);
		pointIter++;
	}
	fclose(iwrite);

}
