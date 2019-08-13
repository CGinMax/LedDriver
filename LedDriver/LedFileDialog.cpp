#include "LedFileDialog.h"
#include <windows.h>


LedFileDialog::LedFileDialog()
{
}


LedFileDialog::~LedFileDialog()
{
}

std::string LedFileDialog::SelectFileNameDialog()
{
	
	std::string strFilename;
	TCHAR Filename[MAX_PATH] = { 0 };

	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = FindWindow(NULL, TEXT("Led Driver"));
	ofn.lpstrFilter = TEXT("视频文件(*.avi,*.mp4,*.wmv,*mpg)\0*.avi;*.mp4;*.wmv;*.mpg\0所有文件\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(Filename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("请选择一个文件");//使用系统默认标题留空即可
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	::GetOpenFileName(&ofn);
	
	strFilename.replace(strFilename.begin(), strFilename.end(), Filename);
	return strFilename;
}

std::string LedFileDialog::OpenSaveFileDialog()
{
	std::string strFilename;
	OPENFILENAME ofn = { 0 };
	TCHAR Filename[MAX_PATH] = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = FindWindow(NULL, TEXT("Led Driver"));
	ofn.lpstrFilter = TEXT("Led驱动器数据(.ldr)\0*.ldr\0所有文件\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(Filename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("保存到");//使用系统默认标题留空即可
	ofn.lpstrDefExt = TEXT("txt");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	::GetSaveFileName(&ofn);

	strFilename.replace(strFilename.begin(), strFilename.end(), Filename);
	return strFilename;
}
