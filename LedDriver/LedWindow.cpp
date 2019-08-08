#include "LedWindow.h"

#include <exception>


LedWindow::LedWindow(int width, int hight, const char * title_name)
	: m_iWidth(width), m_iHight(hight), m_title_name(title_name)
{
	this->Init();
}

LedWindow::~LedWindow()
{
	glfwTerminate();
}

void LedWindow::Init()
{
	/*��ʼ������*/
	if (!glfwInit())
	{
		throw std::exception("glfwInit Error");
	}

	/*����GLFW*/
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/*��������*/
	m_pWindow = glfwCreateWindow(this->m_iWidth, this->m_iHight, this->m_title_name, NULL, NULL);
	if (m_pWindow == NULL)
	{
		glfwTerminate();
		throw std::exception("Create Window Error");
	}
	
	/*����GLFW������*/
	glfwMakeContextCurrent(m_pWindow);

	glfwSwapInterval(1); 

	glfwGetFramebufferSize(m_pWindow, &m_iWidth, &m_iHight);
	glViewport(0, 0, m_iWidth, m_iHight);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	/*���ô��ھ�����ʾ*/
	const GLFWvidmode* centralmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_pWindow, (centralmode->width - m_iWidth) / 2, (centralmode->height - m_iHight) / 2);

}

void LedWindow::PollEvent()
{
	glfwPollEvents();
}

void LedWindow::SwapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}

GLFWwindow *LedWindow::GetLedWindow()
{
	
	return this->m_pWindow;
	
}
