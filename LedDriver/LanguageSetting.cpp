#include "LanguageSetting.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <locale>
#include <codecvt>
#include <windows.h>
#include <Psapi.h>
#include "IconsFontAwesome5.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/istreamwrapper.h"

LanguageSetting *LanguageSetting::languagesetting;

LanguageSetting::LanguageSetting()
{
	m_effectvec.resize(7);
	std::ifstream ifs;
	ifs.open("Language\\config.data", std::ios::in);
	ifs >> m_nowLanguage;
	ifs.close();
	SetLanguageText();
	is_change_language = true;
}

std::string LanguageSetting::ChineseDecode(const char * strData)
{
	std::string res;
	const char *GBK_LOCALE_NAME = ".936";
	std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> convGBK(new std::codecvt_byname<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));
	std::wstring_convert<std::codecvt_utf8<wchar_t>> convl;
	std::wstring wstru8 = convGBK.from_bytes(strData);
	res = convl.to_bytes(wstru8);
	return res;
}

LanguageSetting::~LanguageSetting()
{
	if (languagesetting != nullptr) {
		delete languagesetting;
		languagesetting = NULL;
	}
}

LanguageSetting * LanguageSetting::GetLanguageInstance()
{
	if (languagesetting == nullptr) {
		languagesetting = new LanguageSetting();
	}
	return languagesetting;
}

void LanguageSetting::SetCurrentLanguage(std::string &&cur_language)
{
	//m_nowLanguage = cur_language;
	std::ofstream ofs;
	//ofs.write()
	ofs.open("Language\\config.data", std::ios::out);
	ofs << cur_language;
	ofs.close();
	m_nowLanguage.assign(cur_language);
	SetLanguageText();
}

void LanguageSetting::SetLanguageText()
{
	//获取文件全路径
	std::string runPath(GetExePath());
	char filePath[50] = { 0 };
	sprintf(filePath, "\\Language\\%s.json", m_nowLanguage.c_str());
	runPath.append(filePath);

	//打开文件并读取
	FILE *fp = fopen(runPath.c_str(), "rb");
	char readBuffer[65536];
	rapidjson::FileReadStream frs(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document d;
	d.ParseStream(frs);
	if (d.IsObject()) {

		if (m_nowLanguage == "Chinese") SetChinese(d); else if (m_nowLanguage == "English") SetEnglish(d);
	}
	fclose(fp);

}

void LanguageSetting::SetChinese(rapidjson::Document & doc)
{
	m_drawMainTitle = ICON_FA_PENCIL_RULER + std::string(" ") + ChineseDecode(doc["drawWindowTitle"].GetString());
	m_drawArea = ChineseDecode(doc["drawArea"].GetString());

	rapidjson::Value &v1 = doc["menuBar"];
	m_files = ICON_FA_FILE + std::string(" ") + ChineseDecode(v1["files"].GetString());
	m_newProject = ICON_FA_FILE_SIGNATURE + std::string(" ") + ChineseDecode(v1["newProject"].GetString());
	m_openProject = ICON_FA_FOLDER_OPEN + std::string(" ") + ChineseDecode(v1["openProject"].GetString());
	m_saveProject = ICON_FA_SAVE + std::string(" ") + ChineseDecode(v1["saveProject"].GetString());
	m_option = ICON_FA_TOOLS + std::string(" ") + ChineseDecode(v1["option"].GetString());
	m_exportData = ICON_FA_SAVE + std::string(" ") + ChineseDecode(v1["export"].GetString());
	m_serialPort = ICON_FA_SD_CARD + std::string(" ") + ChineseDecode(v1["serialPort"].GetString());
	m_languageOption = ICON_FA_LANGUAGE + std::string(" ") + ChineseDecode(v1["languageOption"].GetString());
	m_language1 = ChineseDecode(v1["language1"].GetString());
	m_language2 = ChineseDecode(v1["language2"].GetString());
	m_windowOption = ICON_FA_WINDOW_MAXIMIZE + std::string(" ") + ChineseDecode(v1["windowOption"].GetString());
	m_manual = ICON_FA_HAND_PAPER + std::string(" ") + ChineseDecode(v1["manual"].GetString());

	rapidjson::Value &v2 = doc["initWindow"];
	m_initWindowTitle = ICON_FA_SLIDERS_H + std::string(" ") + ChineseDecode(v2["initWindowTitle"].GetString());
	m_latticeArea = ChineseDecode(v2["latticeArea"].GetString());
	m_rowSpacing = ChineseDecode(v2["rowSpacing"].GetString());
	m_colSpacing = ChineseDecode(v2["colSpacing"].GetString());
	m_drawLattice = ChineseDecode(v2["drawLattice"].GetString());
	m_clearLattice = ChineseDecode(v2["clearLattice"].GetString());
	m_item0 = ChineseDecode(v2["item0"].GetString());
	m_item1 = ChineseDecode(v2["item1"].GetString());
	m_item2 = ChineseDecode(v2["item2"].GetString());
	m_item3 = ChineseDecode(v2["item3"].GetString());
	m_item4 = ChineseDecode(v2["item4"].GetString());
	m_item5 = ChineseDecode(v2["item5"].GetString());
	m_item6 = ChineseDecode(v2["item6"].GetString());
	m_item7 = ChineseDecode(v2["item7"].GetString());
	m_item8 = ChineseDecode(v2["item8"].GetString());
	m_item9 = ChineseDecode(v2["item9"].GetString());
	m_item10 = ChineseDecode(v2["item10"].GetString());
	m_item11 = ChineseDecode(v2["item11"].GetString());
	m_item12 = ChineseDecode(v2["item12"].GetString());
	m_item13 = ChineseDecode(v2["item13"].GetString());
	m_item14 = ChineseDecode(v2["item14"].GetString());
	m_item15 = ChineseDecode(v2["item15"].GetString());
	m_item16 = ChineseDecode(v2["item16"].GetString());
	m_item17 = ChineseDecode(v2["item17"].GetString());
	m_layoutDirection = ChineseDecode(v2["layoutDirection"].GetString());
	m_drawConcern = ChineseDecode(v2["drawConcern"].GetString());
	m_settingDone = ChineseDecode(v2["settingDone"].GetString());

	rapidjson::Value &v3 = doc["controlWindow"];
	m_controlWindowTitle = ICON_FA_BRAILLE + std::string(" ") + ChineseDecode(v3["controlWindowTitle"].GetString());
	m_modeSelect = ChineseDecode(v3["modeSelect"].GetString());
	m_manualMode = ICON_FA_USER_COG + std::string(" ") + ChineseDecode(v3["manualMode"].GetString());
	m_dynamicMode = ICON_FA_VIDEO + std::string(" ") + ChineseDecode(v3["dynamicMode"].GetString());
	m_loadBackground = ChineseDecode(v3["loadBackground"].GetString());
	m_showBackground = ChineseDecode(v3["showBackground"].GetString());
	m_addWay = ChineseDecode(v3["addWay"].GetString());
	m_importImage = ChineseDecode(v3["importImage"].GetString());
	m_selectPoint = ChineseDecode(v3["selectPoint"].GetString());
	m_gradient1 = ChineseDecode(v3["gradient1"].GetString());
	m_gradient2 = ChineseDecode(v3["gradient2"].GetString());
	m_showTime = ChineseDecode(v3["showTime"].GetString());
	m_wayConcern = ChineseDecode(v3["wayConcern"].GetString());
	m_startPlay = ChineseDecode(v3["startPlay"].GetString());
	m_stopPlay = ChineseDecode(v3["stopPlay"].GetString());
	m_loadVideo = ChineseDecode(v3["loadVideo"].GetString());
	m_defaultEffect = ChineseDecode(v3["defaultEffect"].GetString());
	
	for (int i = 0; i < static_cast<int>(m_effectvec.size()); i++)
	{
		char effectName[10] = { 0 };
		sprintf(effectName, "effect%d", i + 1);
		m_effectvec.at(i) = ChineseDecode(v3[effectName].GetString());
	}

	rapidjson::Value &v4 = doc["manualLayoutWindow"];
	m_manualWindowTitle = ChineseDecode(v4["manualWindowTitle"].GetString());
	m_checkLine = ChineseDecode(v4["checkLine"].GetString());
	m_layoutDone = ChineseDecode(v4["layoutDone"].GetString());
	m_saveInformation = ChineseDecode(doc["saveInformation"].GetString());
}

void LanguageSetting::SetEnglish(rapidjson::Document & doc)
{
	m_drawMainTitle = ICON_FA_PENCIL_RULER + std::string(" ") + doc["drawWindowTitle"].GetString();
	m_drawArea = doc["drawArea"].GetString();

	rapidjson::Value &v1 = doc["menuBar"];
	m_files = ICON_FA_FILE + std::string(" ") + v1["files"].GetString();
	m_newProject = ICON_FA_FILE_SIGNATURE + std::string(" ") + v1["newProject"].GetString();
	m_openProject = ICON_FA_FOLDER_OPEN + std::string(" ") + v1["openProject"].GetString();
	m_saveProject = ICON_FA_SAVE + std::string(" ") + v1["saveProject"].GetString();
	m_option = ICON_FA_TOOLS + std::string(" ") + v1["option"].GetString();
	m_exportData = ICON_FA_SAVE + std::string(" ") + v1["export"].GetString();
	m_serialPort = ICON_FA_SD_CARD + std::string(" ") + v1["serialPort"].GetString();
	m_languageOption = ICON_FA_LANGUAGE + std::string(" ") + v1["languageOption"].GetString();
	m_language1 = v1["language1"].GetString();
	m_language2 = v1["language2"].GetString();
	m_windowOption = ICON_FA_WINDOW_MAXIMIZE + std::string(" ") + v1["windowOption"].GetString();
	m_manual = ICON_FA_HAND_PAPER + std::string(" ") + v1["manual"].GetString();

	rapidjson::Value &v2 = doc["initWindow"];
	m_initWindowTitle = ICON_FA_SLIDERS_H + std::string(" ") + v2["initWindowTitle"].GetString();
	m_latticeArea = v2["latticeArea"].GetString();
	m_rowSpacing = v2["rowSpacing"].GetString();
	m_colSpacing = v2["colSpacing"].GetString();
	m_drawLattice = v2["drawLattice"].GetString();
	m_clearLattice = v2["clearLattice"].GetString();
	m_item0 = v2["item0"].GetString();
	m_item1 = v2["item1"].GetString();
	m_item2 = v2["item2"].GetString();
	m_item3 = v2["item3"].GetString();
	m_item4 = v2["item4"].GetString();
	m_item5 = v2["item5"].GetString();
	m_item6 = v2["item6"].GetString();
	m_item7 = v2["item7"].GetString();
	m_item8 = v2["item8"].GetString();
	m_item9 = v2["item9"].GetString();
	m_item10 = v2["item10"].GetString();
	m_item11 = v2["item11"].GetString();
	m_item12 = v2["item12"].GetString();
	m_item13 = v2["item13"].GetString();
	m_item14 = v2["item14"].GetString();
	m_item15 = v2["item15"].GetString();
	m_item16 = v2["item16"].GetString();
	m_item17 = v2["item17"].GetString();
	m_layoutDirection = v2["layoutDirection"].GetString();
	m_drawConcern = v2["drawConcern"].GetString();
	m_settingDone = v2["settingDone"].GetString();

	rapidjson::Value &v3 = doc["controlWindow"];
	m_controlWindowTitle = ICON_FA_BRAILLE + std::string(" ") + v3["controlWindowTitle"].GetString();
	m_modeSelect = v3["modeSelect"].GetString();
	m_manualMode = ICON_FA_USER_COG + std::string(" ") + v3["manualMode"].GetString();
	m_dynamicMode = ICON_FA_VIDEO + std::string(" ") + v3["dynamicMode"].GetString();
	m_loadBackground = v3["loadBackground"].GetString();
	m_showBackground = v3["showBackground"].GetString();
	m_addWay = v3["addWay"].GetString();
	m_importImage = v3["importImage"].GetString();
	m_selectPoint = v3["selectPoint"].GetString();
	m_gradient1 = v3["gradient1"].GetString();
	m_gradient2 = v3["gradient2"].GetString();
	m_showTime = v3["showTime"].GetString();
	m_wayConcern = v3["wayConcern"].GetString();
	m_startPlay = v3["startPlay"].GetString();
	m_stopPlay = v3["stopPlay"].GetString();
	m_loadVideo = v3["loadVideo"].GetString();
	m_defaultEffect = v3["defaultEffect"].GetString();
	for (int i = 0; i < static_cast<int>(m_effectvec.size()); i++)
	{
		char effectName[10] = { 0 };
		sprintf(effectName, "effect%d", i + 1);
		m_effectvec.at(i) = v3[effectName].GetString();
	}

	rapidjson::Value &v4 = doc["manualLayoutWindow"];
	m_manualWindowTitle = v4["manualWindowTitle"].GetString();
	m_checkLine = v4["checkLine"].GetString();
	m_layoutDone = v4["layoutDone"].GetString();
	m_saveInformation = doc["saveInformation"].GetString();
}

const std::string GetExePath()
{
	std::string runPath;
	char runProcess[512];
	memset(runProcess, '\0', sizeof(runProcess));
	GetModuleFileName(NULL, runProcess, 512);
	runPath.assign(runProcess, strlen(runProcess) - 14);
	return runPath;
}