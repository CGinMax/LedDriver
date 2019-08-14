#pragma once
#include <windows.h>

#include <GLFW/glfw3.h>

class LedWindow
{
public:
	LedWindow(int width, int hight, const char *title_name);
	~LedWindow();

	void Init();
	void PollEvent();
	void SwapBuffers();
	GLFWwindow *GetLedWindow();

private:
	GLFWwindow *m_pWindow;
	GLFWimage m_Image[2];
	int m_iWidth, m_iHight;
	const char *m_title_name;
};

