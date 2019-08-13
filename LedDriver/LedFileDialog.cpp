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
	ofn.lpstrFilter = TEXT("��Ƶ�ļ�(*.avi,*.mp4,*.wmv,*mpg)\0*.avi;*.mp4;*.wmv;*.mpg\0�����ļ�\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn.nMaxFile = sizeof(Filename);//����������
	ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��
	ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�");//ʹ��ϵͳĬ�ϱ������ռ���
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
	ofn.lpstrFilter = TEXT("Led����������(.ldr)\0*.ldr\0�����ļ�\0*.*\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = Filename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn.nMaxFile = sizeof(Filename);//����������
	ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��
	ofn.lpstrTitle = TEXT("���浽");//ʹ��ϵͳĬ�ϱ������ռ���
	ofn.lpstrDefExt = TEXT("txt");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	::GetSaveFileName(&ofn);

	strFilename.replace(strFilename.begin(), strFilename.end(), Filename);
	return strFilename;
}
