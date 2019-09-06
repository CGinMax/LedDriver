#include "LedFileDialog.h"
#include <windows.h>

LedFileDialog::LedFileDialog()
{
}


LedFileDialog::~LedFileDialog()
{
}

std::string LedFileDialog::OpenNewFileDialog()
{
	return std::string();
}

std::string LedFileDialog::SelectFileNameDialog()
{
	return std::string();
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
	ofn.lpstrDefExt = TEXT("ldr");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if (::GetSaveFileName(&ofn)) {

		strFilename.replace(strFilename.begin(), strFilename.end(), Filename);
		//std::string strFilenameUtf8 = ToUTF8(strFilename);
	}
	
	return strFilename;
}


std::string ImageFileDialog::SelectFileNameDialog()
{
	std::string strFilename;
	TCHAR Filename[MAX_PATH] = { 0 };
	
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = FindWindow(NULL, TEXT("Led Driver"));
	ofn.lpstrFilter = TEXT("图片文件(*.png,*.bmp,*.jpg)\0*.png;*.bmp;*.jpg\0所有文件\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(Filename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("选择文件");//使用系统默认标题留空即可
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	if (::GetOpenFileName(&ofn)) {
		strFilename.replace(strFilename.begin(), strFilename.end(), Filename);
		/*std::string strFilenameUtf8 = ToUTF8(strFilename);
		return strFilenameUtf8;*/
	}

	return strFilename;
}

std::string VideoFileDialog::SelectFileNameDialog()
{
	std::string strFilename;
	TCHAR Filename[MAX_PATH] = { 0 };

	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = FindWindow(NULL, TEXT("Led Driver"));
	ofn.lpstrFilter = TEXT("gif或视频文件(*.gif,*.avi,*.mp4,*.wmv)\0*.gif;*.avi;*.mp4;*.wmv\0所有文件\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(Filename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("选择文件");//使用系统默认标题留空即可
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	if (::GetOpenFileName(&ofn)) {

		strFilename.replace(strFilename.begin(), strFilename.end(), Filename);
		/*std::string strFilenameUtf8 = ToUTF8(strFilename);
		return strFilenameUtf8;*/
	}

	return strFilename;
}

std::string ProjectFileDialog::OpenNewFileDialog()
{
	std::string strFilename;
	TCHAR Filename[MAX_PATH] = { 0 };

	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = FindWindow(NULL, TEXT("Led Driver"));
	ofn.lpstrFilter = TEXT("Led工程文件(.pld)\0*.pld\0所有文件\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(Filename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("新建工程");//使用系统默认标题留空即可
	ofn.lpstrDefExt = TEXT("pld");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	if (::GetSaveFileName(&ofn)) {

		strFilename.replace(strFilename.begin(), strFilename.end(), Filename);
		//std::string strFilenameUtf8 = ToUTF8(strFilename);
		//return strFilenameUtf8;
	}

	return strFilename;
}

std::string ProjectFileDialog::SelectFileNameDialog()
{
	std::string strFilename;
	TCHAR Filename[MAX_PATH] = { 0 };

	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = FindWindow(NULL, TEXT("Led Driver"));
	ofn.lpstrFilter = TEXT("Led工程文件(.pld)\0*.pld\0所有文件\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(Filename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("选择文件");//使用系统默认标题留空即可
	ofn.lpstrDefExt = TEXT("pld");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	
	if (::GetOpenFileName(&ofn)) {

		strFilename.replace(strFilename.begin(), strFilename.end(), Filename);
		//std::string strFilenameUtf8 = ToUTF8(strFilename);
		//return strFilenameUtf8;
	}

	return strFilename;
}

std::string ProjectFileDialog::OpenSaveFileDialog()
{
	std::string strFilename;
	OPENFILENAME ofn = { 0 };
	TCHAR Filename[MAX_PATH] = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = FindWindow(NULL, TEXT("Led Driver"));
	ofn.lpstrFilter = TEXT("Led工程文件(.pld)\0*.pld\0所有文件\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(Filename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("保存到");//使用系统默认标题留空即可
	ofn.lpstrDefExt = TEXT("pld");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if (0 != ::GetSaveFileName(&ofn)) {
		strFilename.replace(strFilename.begin(), strFilename.end(), Filename);

	}

	//std::string strFilenameUtf8 = ToUTF8(strFilename);
	return strFilename;
}
