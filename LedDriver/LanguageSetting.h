#pragma once
#include <string>
#include <vector>
#include "rapidjson/document.h"
class LanguageSetting
{
public:
	~LanguageSetting();
	static LanguageSetting* GetLanguageInstance();
	void SetCurrentLanguage(std::string &&cur_language);
	void SetLanguageText();
private:
	LanguageSetting();
	static LanguageSetting* languagesetting;
	std::string ChineseDecode(const char* strData);
	void SetChinese(rapidjson::Document &doc);
	void SetEnglish(rapidjson::Document &doc);
public:
	bool is_change_language;
	std::string m_nowLanguage;

	std::string m_drawMainTitle;
	std::string m_drawArea;
	std::string m_files;
	std::string m_newProject;
	std::string m_openProject;
	std::string m_saveProject;
	std::string m_option;
	std::string m_exportData;
	std::string m_serialPort;
	std::string m_languageOption;
	std::string m_language1;
	std::string m_language2;
	std::string m_windowOption;
	std::string m_manual;

	std::string m_initWindowTitle;
	std::string m_latticeArea;
	std::string m_rowSpacing;
	std::string m_colSpacing;
	std::string m_drawLattice;
	std::string m_clearLattice;
	std::string m_item0;
	std::string m_item1;
	std::string m_item2;
	std::string m_item3;
	std::string m_item4;
	std::string m_item5;
	std::string m_item6;
	std::string m_item7;
	std::string m_item8;
	std::string m_item9;
	std::string m_item10;
	std::string m_item11;
	std::string m_item12;
	std::string m_item13;
	std::string m_item14;
	std::string m_item15;
	std::string m_item16;
	std::string m_item17;
	std::string m_layoutDirection;
	std::string m_drawConcern;
	std::string m_settingDone;

	std::string m_controlWindowTitle;
	std::string m_modeSelect;
	std::string m_manualMode;
	std::string m_dynamicMode;
	std::string m_loadBackground;
	std::string m_showBackground;
	std::string m_addWay;
	std::string m_importImage;
	std::string m_selectPoint;
	std::string m_gradient1;
	std::string m_gradient2;
	std::string m_showTime;
	std::string m_wayConcern;
	std::string m_startPlay;
	std::string m_stopPlay;
	std::string m_loadVideo;
	std::string m_defaultEffect;
	std::vector<std::string> m_effectvec;

	std::string m_manualWindowTitle;
	std::string m_checkLine;
	std::string m_layoutDone;
};

const std::string GetExePath();

